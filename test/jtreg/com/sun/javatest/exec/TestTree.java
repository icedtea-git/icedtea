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

import java.util.Enumeration;

import javax.swing.JTree;
import javax.swing.ToolTipManager;
import javax.swing.event.TreeModelEvent;
import javax.swing.event.TreeModelListener;
import javax.swing.tree.TreeModel;
import javax.swing.tree.TreePath;

import com.sun.javatest.Parameters;
import com.sun.javatest.TestFilter;
//import com.sun.javatest.TestResult;
import com.sun.javatest.TestResultTable;
import com.sun.javatest.tool.UIFactory;
import com.sun.javatest.util.Debug;
import com.sun.javatest.util.DynamicArray;

/**
 * Tree to render test structure.
 */
class TestTree extends JTree {
    /**
     * @param uif The UI factory object to use.
     * @param model The GUI model object.
     * @param fConfig The active filter configuration object.
     * @param treeModel The data model.
     */
    public TestTree(UIFactory uif, TreePanelModel model, FilterSelectionHandler fh,
                    TestTreeModel treeModel) {
        super();

        this.uif = uif;
        this.tpm = model;
        this.filterHandler = fh;

        filterHandler.addObserver(watcher);

        //currModel = new TestTreeModel(null);
        currModel = treeModel;
        setModel(currModel);
        setLargeModel(true);
        setScrollsOnExpand(true);
        setName("tree");
        ToolTipManager.sharedInstance().registerComponent(this);
    }

    /**
     * Call when it is likely that the Parameters or filters have changed internally.
     */
    public void updateGUI() {
        // delete this if the model becomes top-level

        // invalidate the node info cache because we don't know what part
        // of the parameters have changed.  A filter change is likely if
        // the user has just completed an interview
        repaint();
    }

    public void setModel(TreeModel newModel) {
        super.setModel(newModel);
        newModel.addTreeModelListener(watcher);
    }

    // ---------- private ----------
    void setParameters(Parameters p) {
        // this is only here to notify the model when we change things which
        // affect the tree structure itself.
        // otherwise, most changes are the result of filter changes
    }

    /**
     * For proper operation, this method is recommended over the method on JTree.
     *
     * @param mod The new data model.
     */
    void setTreeModel(TestTreeModel mod) {
        if (currModel != null)
            currModel.removeTreeModelListener(watcher);

        currModel = mod;
        setModel(currModel);
    }

    TestResultTable getTestResultTable() {
        // remove this method when the model becomes independent
        return currModel.getTestResultTable();
    }

    /**
     * Record which tree nodes are currently visible.
     * @see #restorePaths
     */
    TreePath[] snapshotOpenPaths() {
        // currModel.getRoot()==null may indicate that the GUI has been
        // disposed already (?).  We have seen exceptions in the code
        // below because this situation occurs.
        if (currModel == null || currModel.getRoot() == null)
            return null;

        TreePath[] paths = new TreePath[0];
        Enumeration e = getDescendantToggledPaths(new TreePath(currModel.getRoot()));

        while (e != null && e.hasMoreElements()) {
            TreePath tp = (TreePath)(e.nextElement());
            if (!isVisible(tp))     // if we can't see it, we don't care
                continue;
            if (!isExpanded(tp))    // if it's not expanded, we don't need it
                continue;

            paths = (TreePath[])DynamicArray.append(paths, tp);
        }   // while

        return paths;
    }

    /**
     * Attempt to restore paths which were previously recorded.
     * @see #snapshotOpenPaths
     */
    void restorePaths(TreePath[] paths) {
        // make sure root still matches
        if (paths == null || paths.length == 0)
            return;

        for (int i = 0; i < paths.length; i++) {
            //expandPath(paths[i]);
            setExpandedState(paths[i], true);
            makeVisible(paths[i]);
        }   // for
    }

    private UIFactory uif;
    private TestTreeModel currModel;
    private TreePanelModel tpm;
    private FilterSelectionHandler filterHandler;
    private EventWatcher watcher = new EventWatcher();

    //protected static boolean debug = boolean.getboolean("debug." + testtree.class.getname());
    private static int debug = Debug.getInt("debug." + TestTree.class.getName());

    private class EventWatcher implements TreeModelListener, FilterSelectionHandler.Observer {
        // TreeModelListener - on event thread
        public void treeNodesChanged(TreeModelEvent e) {
            // NOTE: cannot get selected item from tree because if it has
            // been replaced already, there may be nothing selected

            /*
            String activeTest = tpm.getSelectedTest();
            if (activeTest == null) {
                return;
            }

            Object[] targets = e.getChildren();
            if (targets == null)
                return;     // don't care then

            for (int i = 0; i < targets.length; i++)
                if (targets[i] instanceof TestResult) {
                    TestResult tr = (TestResult)(targets[i]);

                    if (tr.getTestName().equals(activeTest))
                        tpm.showTest(tr);
                }
            */
        }

        public void treeNodesInserted(TreeModelEvent e) {
            // NOTE: cannot get selected item from tree because if it has
            // been replaced already, there may be nothing selected

            /*
            String activeTest = tpm.getSelectedTest();
            if (activeTest == null) {
                return;
            }

            Object[] targets = e.getChildren();
            if (targets == null)
                return;     // don't care then

            for (int i = 0; i < targets.length; i++)
                if (targets[i] instanceof TestResult) {
                    TestResult tr = (TestResult)(targets[i]);

                    if (tr.getTestName().equals(activeTest))
                        tpm.showTest(tr);
                }
            */
        }

        public void treeNodesRemoved(TreeModelEvent e) {
            // XXX need to handle case when shown node is removed
            // select root in that case?
        }

        public void treeStructureChanged(TreeModelEvent e) {
            // watch for full structure changes, reselect root if it happens
            if (e.getPath()[0] == getModel().getRoot() && e.getPath().length == 1) {
                // in case we caused a structure change, we need to reselect the root
                //TestResultTable.TreeNode root = (TestResultTable.TreeNode)(e.getPath()[0]);
                TestResultTable.TreeNode root = (TestResultTable.TreeNode)(getModel().getRoot());
                tpm.showNode(root, new TreePath(root));
            }
        }

        // FilterConfig.Observer - may not be on event thread
        public void filterUpdated(TestFilter f) {
            updateGUI();
        }

        public void filterSelected(TestFilter f) {
            updateGUI();
        }

        public void filterAdded(TestFilter f) {
            // don't care
        }

        public void filterRemoved(TestFilter f) {
            // don't care
        }

    }
}
