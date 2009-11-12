/*
 * $Id$
 *
 * Copyright 1996-2008 Sun Microsystems, Inc.  All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Sun designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Sun in the LICENSE file that accompanied this code.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa Clara,
 * CA 95054 USA or visit www.sun.com if you need additional information or
 * have any questions.
 */
package com.sun.javatest.exec;

import java.awt.EventQueue;
import java.util.Collections;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.HashSet;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Set;

import javax.swing.tree.TreeModel;
import javax.swing.tree.TreePath;
import javax.swing.event.TreeModelEvent;
import javax.swing.event.TreeModelListener;

import com.sun.javatest.JavaTestError;
import com.sun.javatest.Parameters;
import com.sun.javatest.TestFilter;
import com.sun.javatest.TestResult;
import com.sun.javatest.TestResultTable;
import com.sun.javatest.TestSuite;
import com.sun.javatest.WorkDirectory;
import com.sun.javatest.tool.UIFactory;
import com.sun.javatest.util.Debug;
import com.sun.javatest.util.DynamicArray;

/**
 * This is the data model bridge between JT Harness core and the GUI.
 */

// NOTE: if you are going to request the worker lock and the htLock, you
//       MUST do it in that order.  Be mindful of holding the htLock and
//       calling a synchronized method, that is illegal and you must
//       release the htLock before making the method call.
class TestTreeModel implements TreeModel, TestResultTable.TreeObserver {
    TestTreeModel(Parameters p, FilterSelectionHandler filterHandler, UIFactory uif) {
        this.filterHandler = filterHandler;
        this.uif = uif;

        cache = new Hashtable();
        cacheQueue = new LinkedList();
        suspendedQueue = new LinkedList();

        cacheWorker = new CacheWorker();
        cacheWorker.setPriority(Thread.MIN_PRIORITY + 1);
        cacheWorker.start();

        setParameters(p);

        watcher = new FilterWatcher();
        filterHandler.addObserver(watcher);
        lastFilter = filterHandler.getActiveFilter();

    }

    synchronized void dispose() {
        disposed = true;

        if (trt != null) {
            trt.removeObserver(this);
            trt.dispose();
            trt = null;
        }

        filterHandler.removeObserver(watcher);

        if (cacheWorker != null) {
            cacheWorker.interrupt();
            cacheWorker = null;
        }

        params = null;
    }

    TestFilter getTestFilter() {
        return filterHandler.getActiveFilter();
    }

    Parameters getParameters() {
        return params;
    }

    // TreeModel methods
    public void addTreeModelListener(TreeModelListener l) {
        treeModelListeners = (TreeModelListener[])DynamicArray.append(treeModelListeners, l);
    }

    public Object getChild(Object parent, int index) {
        if (parent instanceof TestResultTable.TreeNode) {
            TestResultTable.TreeNode node = (TestResultTable.TreeNode)parent;
            return node.getChild(index);
        }
        else
            return null;
    }

    public int getChildCount(Object parent)  {
        if (parent instanceof TestResultTable.TreeNode) {
            TestResultTable.TreeNode node = (TestResultTable.TreeNode)parent;
            return node.getChildCount();
        }
        else if (parent instanceof TestResult) {
            return 0;
        }
        else {
            return -1;
        }
    }

    public int getIndexOfChild(Object parent, Object child)  {
        if( !(parent instanceof TestResultTable.TreeNode) )
            throw new IllegalArgumentException(uif.getI18NString("tree.badParent"));

        TestResultTable.TreeNode pn = (TestResultTable.TreeNode)parent;
        return pn.getIndex(child);
    }

    public Object getRoot()  {
        if (disposed) {
            if (debug > 0)
                Debug.println("TTM - getRoot() ignored, model has been disposed.");
            return null;
        }

        // trt is never supposed to be null
        return trt.getRoot();
    }

    public boolean isLeaf(Object node)  {
        if (node == null)
            return true;

        if (node instanceof TestResultTable.TreeNode)
            return false;
        else if (node instanceof TestResult)
            return true;
        else
            throw new IllegalArgumentException(uif.getI18NString("tree.badType"));
    }

    public void removeTreeModelListener(TreeModelListener l)  {
        treeModelListeners = (TreeModelListener[])DynamicArray.remove(treeModelListeners, l);
    }

    public void valueForPathChanged(TreePath path, Object newValue) {
        // XXX
        System.err.println(getClass().getName() + ": VFPC");
    }


    // TRT TreeObserver methods
    public synchronized void nodeInserted(TestResultTable.TreeNode[] path, Object what, int index) {
        int[] inds = { index };
        Object[] nodes = { what };

        TreeModelEvent tme;

        if (what instanceof TestResultTable.TreeNode) {
            if (!relevantNodes.contains(path[path.length - 1]))
                return;

            if (debug > 0)
                Debug.println("TTM - Node " + what + " inserted, path len=" + path.length);

            if (path == null || path.length == 0)       // root
                tme = new TreeModelEvent(this, new TreePath(what));
            else                                        // not root
                tme = new TreeModelEvent(this, path, inds, nodes);

            notifyInserted(tme);
        }
        else {      // test result
            TestResult tr = (TestResult)what;

            if (relevantNodes.contains(path[path.length - 1])) {
                if (debug > 0) {
                    Debug.println("TTM - Node " + what + " inserted, path len=" + path.length);
                    Debug.println("   -> inserting " + tr.getTestName());
                    Debug.println("   -> mutable " + tr.isMutable());
                    Debug.println("   -> status " + tr.getStatus().getType());
                    Debug.println("   -> into " + path[0].getEnclosingTable());
                }

                notifyInserted(makeEvent(path, what, index));
            }

            for (int i = path.length - 1; i >= 0; i--) {
                // walk path thru cache
                TT_NodeCache ni = null;

                synchronized (htLock) {
                    ni = (TT_NodeCache)(cache.get(path[i]));
                }   // sync

                // notify cache nodes
                // most of the time ni would probably be null
                if (ni != null) {
                    boolean result = false;
                    synchronized (ni.getNode()) {
                        result = ni.add(path, (TestResult)what, index);
                    }   // sync

                    if (result && relevantNodes.contains(path[i])) {
                        // if change detected, and relevant, then broadcast
                        // update of summary numbers is passive because it retrieves
                        // updates itself.
                        // we need to tell the tree when to update though
                        if (path[i].isRoot())
                            tme = new TreeModelEvent(this, new TreePath(path[i]));
                        else
                            tme = makeEvent(TestResultTable.getObjectPath(path[i-1]), path[i], path[i-1].getIndex(path[i]));

                        notifyChanged(tme);
                    }
                }
            }   // for
        }
    }

    public synchronized void nodeChanged(TestResultTable.TreeNode[] path, Object what,
                            int index, Object old) {
        if (what instanceof TestResultTable.TreeNode) {
            // check relevance
            if (relevantNodes.contains(path[path.length-1])) {
                if (debug > 0)
                    Debug.println("TTM - nodeChanged: " + what);

                notifyChanged(makeEvent(path, what, index));
                notifyStructure( new TreeModelEvent(this, path, null, null) );
            }
        }
        else {      // test result
            if (relevantTests.contains(old) && old != what) {
                if (debug > 0)
                    Debug.println("TTM - nodeChanged: " + what);

                relevantTests.remove(old);
                relevantTests.add(what);
            }

            for (int i = path.length - 1; i >= 0; i--) {
                // walk path thru cache
                TT_NodeCache ni = null;

                synchronized (htLock) {
                    ni = (TT_NodeCache)(cache.get(path[i]));
                }   // sync

                // notify cache nodes
                if (ni != null) {
                    boolean result = ni.replace(path, (TestResult)what, index, (TestResult)old);

                    if (result && relevantNodes.contains(path[i])) {
                        // if change detected, and relevant, then broadcast
                        //notifyChanged(makeEvent(path, what, index));
                    }

                    if (relevantTests.contains((TestResult)what)) {
                        // if change detected, and relevant, then broadcast
                        notifyChanged(makeEvent(path, what, index));
                    }
                }
            }   // for
        }
    }

    public synchronized void nodeRemoved(TestResultTable.TreeNode[] path, Object what, int index) {
        if (what instanceof TestResultTable.TreeNode) {
            if (relevantNodes.contains(path[path.length - 1])) {
                if (debug > 0)
                    Debug.println("TTM - Node " + what + " removed, path len=" + path.length);

                notifyRemoved(makeEvent(path, what, index));
            }
        }
        else {      // test result
            TestResult tr = (TestResult)what;

            relevantTests.remove(tr);

            if (relevantNodes.contains(path[path.length - 1])) {
                if (debug > 0) {
                    Debug.println("TTM - Node " + what + " removed, path len=" + path.length);
                    Debug.println("    -> Removing " + tr.getTestName());
                }

                notifyRemoved(makeEvent(path, what, index));
            }

            for (int i = path.length - 1; i >= 0; i--) {
                // walk path thru cache
                TT_NodeCache ni = null;

                synchronized (htLock) {
                    ni = (TT_NodeCache)(cache.get(path[i]));
                }   // sync

                // notify cache nodes
                if (ni != null) {
                    // need to lock node before locking the cache node
                    boolean result = false;
                    synchronized (ni.getNode()) {
                        result = ni.remove(path, (TestResult)what, index);
                    }   // sync

                    if (result && relevantNodes.contains(path[i])) {
                        // if change detected, and relevant, then broadcast
                    }
                }
            }   // for
        }
    }

    //  ----- private -----

    void addRelevantNode(TestResultTable.TreeNode node) {
        relevantNodes.add(node);
    }

    void removeRelevantNode(TestResultTable.TreeNode node) {
        relevantNodes.remove(node);
    }

    void addRelevantTest(TestResult tr) {
        relevantTests.add(tr);
    }

    void removeRelevantTest(TestResult tr) {
        if (relevantTests != null && tr != null)
            relevantTests.remove(tr);
    }

    /**
     * Hint that the given node is of more importance than others.
     */
    void setActiveNode(TestResultTable.TreeNode node) {
        TT_NodeCache ni = null;

        synchronized (htLock) {
            ni = (TT_NodeCache)(cache.get(node));
        }

        if (ni != null)
            cacheWorker.requestActiveNode(ni);
        else {
            // uh...why don't we have a node?
            // create and schedule?
        }
    }

    /**
     * Create an event from the raw data.  This should just make the code a little nicer
     * to look at.
     */
    private TreeModelEvent makeEvent(Object[] path, Object target, int index) {
        int[] inds = { index };

        Object[] nodes = { target };

        /*
        // per the TreeModelEvent javadoc, the path is supposed to lead to the parent of the
        // changed node, so we need to edit
        TestResultTable.TreeNode[] parentPath = new TestResultTable.TreeNode[path.length-1];
        System.arraycopy(path, 1, parentPath, 0, path.length-1);
        */

        if (debug > 1) {
            Debug.println("TTM Broadcasing " + target + " change message...");
            //Debug.println("   -> Status = " + ((TestResult)target).getStatus().toString());
            Debug.println("   -> Path len=" + path.length);
            //Debug.println("   -> 1st = " + parentPath[0].getName());
            //Debug.println("   -> Last = " + parentPath[parentPath.length-1].getName());
            Debug.println("   -> Index = " + index);
            Debug.println("   -> TRT = " + trt);
        }

        return new TreeModelEvent(this, path, inds, nodes);
    }

    void setParameters(Parameters p) {
        if (p != null) {
            params = p;
            init();
        }
        else {
            TestResultTable dummy = new TestResultTable();
            setTestResultTable(dummy);

            if (debug > 0)
                Debug.println("TTM - dummy TRT, root = " + dummy.getRoot());
        }
    }

    synchronized private void init() {
        if (params == null)
            return;

        WorkDirectory wd = params.getWorkDirectory();
        TestSuite ts = params.getTestSuite();

        if (wd != null) {       // full config w/TS and WD
            if (debug > 0)
                Debug.println("TTM - initializing with workdir");

            TestResultTable newTrt = wd.getTestResultTable();
            setTestResultTable(newTrt);
        }
        else if (ts != null) {  // TS, but no workdir

            if (trt.getTestFinder() == null) {
                // configure a finder on the temporary TRT, this allows us to
                // populate the table before we have a workdir
                trt.setTestFinder(ts.getTestFinder());
                setTestResultTable(trt);

                if (debug > 0)
                    Debug.println("TTM - params set, no WD; setting finder on temp. TRT");

                //notifyFullStructure();
            }
            else {
                // already have a finder
                if (debug > 0)
                    Debug.println("TTM - temp. TRT already has finder");
            }
        }                       // no WD or TS
        else {
            if (debug > 0)
                Debug.println("TTM - params set, no WD or TS");

            // should we explicitly reset the model?
        }
    }

    void pauseWork() {
        cacheWorker.setPaused(true);
    }

    void unpauseWork() {
        invalidateNodeInfo();
        cacheWorker.setPaused(false);
    }

    /**
     * This method will attempt to swap the table if compatible to avoid a full JTree
     * swap.
     */
    private void setTestResultTable(TestResultTable newTrt) {
        // NOTE: may want to run this on when we figure out how important params are
        // see note in setParameters()
        //if (params == null)
        //    throw IllegalStateException();

        // assumptions:
        // - a model must have a non-null TRT
        if (isCompatible(trt, newTrt)) {
            swapTables(newTrt);
        }
        else {
            if (trt != null)
                trt.removeObserver(this);

            trt = newTrt;
            trt.addObserver(this);
        }

        // prime relevant nodes with root and first level
        relevantNodes = Collections.synchronizedSet(new HashSet());
        relevantTests = Collections.synchronizedSet(new HashSet());

        addRelevantNode(trt.getRoot());
        TestResultTable.TreeNode[] childs = trt.getRoot().getTreeNodes();
        if (childs != null)
            for (int i = 0; i < childs.length; i++)
                addRelevantNode(childs[i]);

        TestResult[] trs = trt.getRoot().getTestResults();
        if (trs != null)
            for (int i = 0; i < trs.length; i++)
                addRelevantTest(trs[i]);

        notifyFullStructure();

        if (debug > 0) {
            Debug.println("TTM - Model watching " + trt);
            if (trt.getWorkDir() != null) {
                Debug.println("   -> Workdir=" + trt.getWorkDir());
                Debug.println("   -> Workdir path=" + trt.getWorkDir().getPath());
                Debug.println("   -> root = " + trt.getRoot());
            }
        }
    }

    static boolean isCompatible(TestResultTable t1, TestResultTable t2) {
        // assumptions:
        // - a TRT must have a finder and testsuite root to be comparable
        // - the finder class objects must be equal for TRTs to be compatible
        // - testsuite root paths must be equal
        if (t1 == null || t2 == null ||
            t1.getTestSuiteRoot() == null || t2.getTestSuiteRoot() == null ||
            t1.getTestFinder() == null || t2.getTestFinder() == null) {
            if (debug > 1) {
                Debug.println("TTM - isCompatible() failed because one or both TRTs are incomplete.");
                if (t1 != null && t2 != null) {
                    Debug.println("t1 root = " + t1.getTestSuiteRoot());
                    Debug.println("t2 root = " + t2.getTestSuiteRoot());
                    Debug.println("t1 finder= " + t1.getTestFinder());
                    Debug.println("t2 finder= " + t2.getTestFinder());
                }
            }

            return false;
        }
        else if (!t1.getTestSuiteRoot().getPath().equals(t2.getTestSuiteRoot().getPath())) {
            if (debug > 1)
                Debug.println("TTM - isCompatible() failed because testsuite paths differ.");
            return false;
        }
        else if (t1.getTestFinder() != t2.getTestFinder()) {
            if (debug > 1)
                Debug.println("TTM - isCompatible() failed because TestFinders differ.");
            return false;
        }
        else
            return true;
    }


    TestResultTable getTestResultTable() {
        return trt;
    }

    /**
     * Retrieve the info about the given node.  The returned info object may be
     * either a running thread or a finished thread, depending on whether recent
     * data is available.
     * @param node The node to get information for.
     * @param highPriority Should the task of retrieving this information be
     *        higher than normal.
     */
    TT_NodeCache getNodeInfo(TestResultTable.TreeNode node, boolean highPriority) {
        TT_NodeCache ni = null;
        boolean wakeWorker = false;

        // this line is outside the synchronized zone to prevent a
        // deadlock when a filter change occurs and the cache is
        // invalidated (on the event thread)
        TestFilter activeFilter = filterHandler.getActiveFilter();

        synchronized (htLock) {
            ni = (TT_NodeCache)(cache.get(node));

            if (ni == null) {
                ni = new TT_NodeCache(node, activeFilter);
                cache.put(node, ni);

                if (!highPriority)      // high pri. case below
                    cacheQueue.addFirst(ni);

                wakeWorker = true;      // because this is a new node
            }
            else if (highPriority) {
                // reorder for high priority
                // could cancel current task, but that would complicate things
                int index = cacheQueue.indexOf(ni);

                if (index >= 0) {
                    cacheQueue.remove(index);
                }
            }
        }   // synchronized

        // this is done outside the htLock block to ensure proper locking
        // order.
        if (highPriority) {
            cacheWorker.requestActiveNode(ni);
            wakeWorker = true;
        }

        // forward info for persistent storage and later use...
        if (!statsForwarded && node.isRoot() && ni.isComplete()) {
            int[] stats = ni.getStats();
            int total = 0;

            for (int i = 0; i < stats.length; i++)
                total += stats[i];

            total += ni.getRejectCount();

            if (params != null) {
                WorkDirectory wd = params.getWorkDirectory();

                if (wd != null) {
                    wd.setTestSuiteTestCount(total);
                    statsForwarded = true;
                }
            }
        }

        if (wakeWorker)
            synchronized (cacheWorker) {
                cacheWorker.notify();
            }

        return ni;
    }

    /**
     * Invalidate any collected data about any of the nodes on the given path.
     * This operation includes stopping any threads scanning for info and
     * removing the entries from the cache.
     *
     * @param path Nodes to invalidate in the cache, must not be null.
     *             May be length zero.
     */
    void invalidateNodeInfo(TestResultTable.TreeNode[] path) {
        // do this as a batch
        // we can reverse the sync and for if stalls are noticeable
        synchronized (htLock) {
            for (int i = 0; i < path.length; i++) {
                TT_NodeCache info = (TT_NodeCache)(cache.get(path[i]));

                if (info != null) {
                    if (debug > 1) {
                        Debug.println("TTM - halting thread and removed from node cache");
                        Debug.println("   -> " + path[i]);
                        Debug.println("   -> " + info);
                    }

                    info.halt();
                    cache.remove(info);
                    boolean wasInQueue = cacheQueue.remove(info);

                    info = new TT_NodeCache(info.getNode(), filterHandler.getActiveFilter());
                    cache.put(info.getNode(), info);
                    cacheQueue.addFirst(info);
                }
            }   // for
        }   // synchronized

        // wake up the worker
        synchronized (cacheWorker) {
            cacheWorker.notify();
        }
    }

    /**
     * Invalidate all collected node information.
     * This is likely used when the internal contents of the parameters have
     * changed.  If the cache is not invalidated at certain points, rendering
     * of the tree may be incorrect.
     */
    void invalidateNodeInfo() {
        synchronized (htLock) {
            Enumeration e = cache.keys();
            while (e.hasMoreElements()) {
                ((TT_NodeCache)(cache.get(e.nextElement()))).invalidate();
            }   // while

            cache = new Hashtable();
            cacheQueue = new LinkedList();
            suspendedQueue = new LinkedList();
        }

        // reprocess any needed nodes
        Iterator it = relevantNodes.iterator();
        while (it.hasNext()) {
            getNodeInfo((TestResultTable.TreeNode)it.next(), false);
        }       // while
    }

    /**
     * Invalidate any collected data about the given node.
     * This operation includes stopping any thread scanning for info and
     * removing the entry from the cache.
     *
     * @param node The node to invalidate in the cache.  Must not be null.
     * @deprecated The cache will be smart enough to not need this.
     */
    void invalidateNodeInfo(TestResultTable.TreeNode node) {
        invalidateNodeInfo(new TestResultTable.TreeNode[] {node});
    }

    /**
     * Trusting method which assumes that the given TRT is compatible with the
     * current one.  Strange things will hapen if it is not!
     */
    void swapTables(TestResultTable newTrt) {
        if (newTrt == trt || newTrt == null)
            return;

        if (debug > 1) {
            Debug.println("Swapping TRTs under the covers.");
            Debug.println("   -> OLD=" + trt);
            Debug.println("   -> NEW=" + newTrt);
        }

        trt.removeObserver(this);
        trt = newTrt;
        trt.addObserver(this);
    }

    private void notifyInserted(TreeModelEvent e) {
        if (treeModelListeners != null)
            EventQueue.invokeLater(new Notifier(Notifier.INS, treeModelListeners, e, uif));
            //for (int i = 0; i < treeModelListeners.length; i++)
            //    ((TreeModelListener)treeModelListeners[i]).treeNodesInserted(e);
    }

    private void notifyChanged(TreeModelEvent e) {
        if (treeModelListeners != null) {
            EventQueue.invokeLater(new Notifier(Notifier.CHANGE, treeModelListeners, e, uif));
            //for (int i = 0; i < treeModelListeners.length; i++)
            //    ((TreeModelListener)treeModelListeners[i]).treeNodesChanged(e);
        }
    }

    private void notifyRemoved(TreeModelEvent e) {
        if (treeModelListeners != null)
            EventQueue.invokeLater(new Notifier(Notifier.DEL, treeModelListeners, e, uif));
    }

    private void notifyStructure(TreeModelEvent e) {
        if (treeModelListeners != null)
            EventQueue.invokeLater(new Notifier(Notifier.STRUCT, treeModelListeners, e, uif));
            //for (int i = 0; i < treeModelListeners.length; i++)
            //    ((TreeModelListener)treeModelListeners[i]).treeStructureChanged(e);
    }

    private void notifyFullStructure() {
        if (debug > 0)
            Debug.println("TTM - sending full structure change event to model listeners.");

        invalidateNodeInfo();

        Object[] path = {trt.getRoot()};
        TreeModelEvent e = new TreeModelEvent(this, path);
        notifyStructure(e);
    }

    protected void finalize() throws Throwable {
        super.finalize();

        if (trt != null)
            trt.removeObserver(this);
    }

    private UIFactory uif;
    private TestResultTable trt;
    private Parameters params;
    private FilterSelectionHandler filterHandler;
    private TestFilter lastFilter;
    private TreeModelListener[] treeModelListeners = new TreeModelListener[0];
    private boolean statsForwarded;
    private boolean disposed;

    private CacheWorker cacheWorker;
    private FilterWatcher watcher;

    private Set relevantNodes;
    private Set relevantTests;

    /**
     * Stores state info about individual nodes.
     * The key is a TestResultTable.TreeNode, and the value is a TT_NodeCache
     * object.  Use the htLock when accessing the cache.
     */
    protected Hashtable cache;

    /**
     * Queue of items to be processed.
     * "First" is the most recently added, "last" is the next to be processed.
     */
    protected LinkedList cacheQueue;
    /**
     * Queue of items which are in the middle of being processed.
     * "First" is the most recently added, "last" is the next to be processed.
     */
    protected LinkedList suspendedQueue;
    protected final Object htLock = new Object();

    private static final int CACHE_THREAD_PRI = Thread.MIN_PRIORITY;
    private static final int CACHE_NOTI_THR_PRI = Thread.MIN_PRIORITY;
    protected static int debug = Debug.getInt(TestTreeModel.class);

    // ************** inner classes ***************

    private class CacheWorker extends Thread {
        CacheWorker() {
            super("Test Tree Cache Worker");
        }

        public void run() {
            try {
                synchronized(CacheWorker.this) {
                    wait();
                }
            }
            catch (InterruptedException e) {
                // we're terminated I guess
                return;
            }

            while (!stopping) {
                if (paused)
                    try {
                        synchronized(CacheWorker.this) {
                            wait();
                        }
                        continue;
                    }
                    catch (InterruptedException e) {
                        // we're terminated I guess
                        stopping = true;
                        continue;
                    }

                // poll the queue for work to do
                currentUnit = getNextUnit();

                // nothing to do, wait
                if (currentUnit == null)
                    try {
                        synchronized (CacheWorker.this) {
                            wait();
                        }
                        continue;
                    }   // try
                    catch (InterruptedException e) {
                        // we're terminated I guess
                        stopping = true;
                        continue;
                    }   // catch
                else {
                    if (!currentUnit.canRun()) {
                        //throw new IllegalStateException("cache malfunction - attempting to re-populate node  " + currentUnit.isPaused() + "   " + currentUnit.isValid() + "  " + currentUnit.isComplete());
                        continue;
                    }

                    // do the work
                    if (debug > 0)
                        Debug.println("TTM cache processing " + currentUnit .getNode().getName());

                    currentUnit.run();

                    if (!currentUnit.isPaused() && currentUnit.isValid())
                        finishJob(currentUnit);
                }
            }   // while

            this.currentUnit = null;
            this.priorityUnit = null;

        }

        /**
         * Only call this to terminate all cache activity.
         * There is currently no way to restart the cache.
         */
        /* should not override Thread.interrupt
        public void interrupt() {
            stopping = true;
        }
        */

        /**
         * Find out which node is currently being processed.
         * @return The node which is currently being worked on, null if no work is
         *         in progress.
         */
        public TT_NodeCache getActiveNode() {
            return currentUnit;
        }

        void setPaused(boolean state) {
            synchronized (this) {
                if (state != paused) {
                    paused = state;
                    if (paused && currentUnit != null) {
                        currentUnit.pause();
                        suspendedQueue.addFirst(currentUnit);
                    }

                    // wake up
                    if (!paused)
                        this.notify();
                }
            }
        }

        // ------- private -------

        /**
         * Request that the worker process the given unit ASAP.
         * This method will first verify that the given node is a canidate for
         * work.  This method is synchronized to avoid context switching into
         * other parts of this class.
         * @param what Which node to give attention to.
         */
        synchronized void requestActiveNode(TT_NodeCache what) {
            if (what != null && currentUnit != what && what.canRun()) {
                // This will cause processing on the worker thread to terminate
                // and it will do selection via getNextUnit().  That will return
                // the priority unit below.
                priorityUnit = what;
                if (currentUnit != null) {
                    currentUnit.pause();
                    suspendedQueue.addFirst(currentUnit);
                }
            }
        }

        /**
         * From the available queues to be processed, select the most ripe.
         */
        private TT_NodeCache getNextUnit() {
            boolean wasPriority = false;
            TT_NodeCache next = null;

            synchronized (htLock) {
                // schedule next node in this priority:
                // 1) a priority unit
                // 2) a previously suspended unit
                // 3) the next unit in the queue
                if (priorityUnit != null) {
                    wasPriority = true;
                    next = priorityUnit;
                    priorityUnit = null;
                }
                else {
                    switch (SCHEDULING_ALGO) {
                        case QUEUE: next = selectByQueuing(); break;
                        case DEPTH: next = selectByDepth(); break;
                        default:
                            next = selectByQueuing();
                    }
                }

                // additional selection criteria
                if (next != null && !next.canRun())     // discard unrunnable jobs
                    return getNextUnit();
                else                                    // ok, schedule this one
                    return next;
            }   // sync
        }

        // -------- various scheduling algorithms ----------
        /**
         * Simply do it in the order the requests came in.
         * The root is excluded from selection until the last possible
         * time.
         */
        private TT_NodeCache selectByQueuing() {
            TT_NodeCache selection = null;

            if (suspendedQueue.size() > 0)
                selection = (TT_NodeCache)(suspendedQueue.removeLast());
            else if (cacheQueue.size() > 0)
                selection = (TT_NodeCache)(cacheQueue.removeLast());

            if (selection != null &&
                selection.getNode().isRoot() &&     // trying to avoid root
                (cacheQueue.size() > 0 || suspendedQueue.size() > 0) ) {
                cacheQueue.addFirst(selection);
                return selectByQueuing();
            }

            return selection;
        }

        /**
         * Select the deepest node in the cacheQueue.
         */
        private TT_NodeCache selectByDepth() {
            // note must already have htLock
            // note with this algo., the suspend and cache queues are
            //      basically equivalent
            TT_NodeCache selected = null;
            int depth = -1;
            LinkedList theList = cacheQueue;
            boolean notDone = true;
            int count = 0;

            if (cacheQueue.size() == 0)
                if (suspendedQueue.size() == 0)
                    notDone = false;
                else
                    theList = suspendedQueue;
            else {
            }

            while (notDone) {
                TT_NodeCache possible = (TT_NodeCache)(theList.get(count));
                int thisDepth = TestResultTable.getObjectPath(possible.getNode()).length;

                if (thisDepth > depth) {
                    theList.remove(count);

                    // requeue the last deepest node found
                    if (selected != null) {
                        cacheQueue.addFirst(selected);
                        // adjust the counter since we just added one
                        if (theList == cacheQueue) count++;
                    }
                    depth = thisDepth;
                    selected = possible;
                }

                count++;
                if (count >= theList.size())
                    if (theList == suspendedQueue)
                        notDone = false;
                    else if (suspendedQueue.size() != 0) {
                        theList = suspendedQueue;
                        count = 0;
                    }
                    else
                        notDone = false;
                else {
                }
            }   // while

            return selected;
        }

        private synchronized void finishJob(TT_NodeCache item) {
            // do not send update message if it's not important (onscreen)
            if (!relevantNodes.contains(item.getNode()))
                return;

            TreeModelEvent e = null;
            TestResultTable.TreeNode node = item.getNode();

            // switch event format if the node is the root
            if (node.isRoot()) {
                e = new TreeModelEvent(this, new Object[] {node}, (int[])null, (Object[])null);
            }
            else {
                // full path to the node, inclusive
                TestResultTable.TreeNode[] fp = TestResultTable.getObjectPath(node);

                // partial path - for JTree event
                TestResultTable.TreeNode[] pp = new TestResultTable.TreeNode[fp.length-1];
                System.arraycopy(fp, 0, pp, 0, fp.length-1);

                // index in parent of target node - required by JTree
                // ignore this operation if we are at the root (length==1)
                int index = 0;
                if (pp.length > 1)
                    index = pp[pp.length-1].getIndex(node);

                e = makeEvent(pp, node, index);
            }

            // dispatch event
            notifyChanged(e);
        }

        private volatile boolean paused;
        private volatile boolean stopping;
        private volatile TT_NodeCache priorityUnit;
        private volatile TT_NodeCache currentUnit;
        private static final int QUEUE = 0;
        private static final int DEPTH = 1;
        private static final int SCHEDULING_ALGO = DEPTH;
    }

    // inner class
    /**
     * Object used to physically dispatch any model update events onto the event thread.
     */
    private static class Notifier implements Runnable {
        /**
         * Create a event notification object to be scheduled on the GUI event thread.
         * The type translates to a switch between the different possible observer
         * methods.
         *
         * @param eType Type of observer message to generate.
         *        Must not be greater than zero, see the defined constants.
         * @param listeners The listeners to notify.  This is shallow copied immediately.
         *        Must not be null.  May be of zero length.
         * @param e The event to give to the listeners.
         *        Must not be null.
         */
        Notifier (int eType, TreeModelListener[] listeners, TreeModelEvent e,
                  UIFactory uif) {
            type = eType;
            this.e = e;
            this.uif = uif;

            // make shallow copy
            TreeModelListener[] copy = new TreeModelListener[listeners.length];
            System.arraycopy(listeners, 0, copy, 0, listeners.length);
            l = copy;
        }

        public void run() {
            switch (type) {
                case CHANGE:
                    for (int i = 0; i < l.length; i++)
                        ((TreeModelListener)l[i]).treeNodesChanged(e);
                    break;
                case STRUCT:
                    for (int i = 0; i < l.length; i++)
                        ((TreeModelListener)l[i]).treeStructureChanged(e);
                    break;
                case INS:
                    for (int i = 0; i < l.length; i++)
                        ((TreeModelListener)l[i]).treeNodesInserted(e);
                    break;
                case DEL:
                    for (int i = 0; i < l.length; i++)
                        ((TreeModelListener)l[i]).treeNodesRemoved(e);
                    break;
                default:
                    throw new JavaTestError(uif.getI18NString("tree.noEType"));
            }   // switch
        }

        TreeModelListener[] l;
        int type;
        TreeModelEvent e;
        UIFactory uif;

        static final int CHANGE = 0;
        static final int STRUCT = 1;
        static final int INS = 2;
        static final int DEL = 3;
    }

    // FilterSelectionHandler.Observer - may not be on event thread
    private class FilterWatcher implements FilterSelectionHandler.Observer {
        public void filterUpdated(TestFilter f) {
            //notifyFullStructure();
            invalidateNodeInfo();
        }

        public void filterSelected(TestFilter f) {
            //notifyFullStructure();
            if (!lastFilter.equals(f))
                invalidateNodeInfo();

            lastFilter = f;
        }

        public void filterAdded(TestFilter f) {
            // don't care
        }

        public void filterRemoved(TestFilter f) {
            // don't care
        }

    }

    /**
     * Waits for a node cache thread to complete, then dispatches a tree update
     * event onto the GUI event thread.  This is how icons in the tree get repainted
     * when we know what color to make them.
    private class CacheNotifier extends Thread {
*/
        /**
         * @param nc The cache thread/node that we are interested in watching
         * @param node The node that the cache pretains to
        CacheNotifier(TT_NodeCache nc) {
            super("TestTreeModel.CacheNotifier for " + nc.getNode().getName());
            this.nc = nc;
            this.node = nc.getNode();
        }

        public void run() {
            // intended effect is that we wait if isAlive() is true, and continue
            // otherwise, assuming that data in the nc is ready
            try {
                nc.join(0);
            }
            catch (InterruptedException e) {
                // we will ignore it
                if (debug > 0)
                    e.printStackTrace(Debug.getWriter());
                return;
            }

            TreeModelEvent e = null;

            // switch event format if the node is the root
            if (node.isRoot()) {
                e = new TreeModelEvent(this, new Object[] {node}, (int[])null, (Object[])null);
            }
            else {
                // full path to the node, inclusive
                TestResultTable.TreeNode[] fp = TestResultTable.getObjectPath(node);

                // partial path - for JTree event
                TestResultTable.TreeNode[] pp = new TestResultTable.TreeNode[fp.length-1];
                System.arraycopy(fp, 0, pp, 0, fp.length-1);

                // index in parent of target node - required by JTree
                // ignore this operation if we are at the root (length==1)
                int index = 0;
                if (pp.length > 1)
                    index = pp[pp.length-1].getIndex(node);

                e = makeEvent(pp, node, index);
            }

            // dispatch event
            notifyChanged(e);
        }

        private TestResultTable.TreeNode node;
        private TT_NodeCache nc;
    }
         */
}
