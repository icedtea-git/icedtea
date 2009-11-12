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

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.EventQueue;
import java.awt.Font;
import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.Map;
import java.util.Vector;

import javax.swing.AbstractAction;
import javax.swing.DefaultListModel;
import javax.swing.JComponent;
import javax.swing.JDialog;
import javax.swing.JList;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.KeyStroke;
import javax.swing.ListModel;
import javax.swing.MenuElement;
import javax.swing.ScrollPaneConstants;
import javax.swing.Timer;
import javax.swing.event.TreeExpansionEvent;
import javax.swing.event.TreeExpansionListener;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.plaf.metal.MetalLookAndFeel;
import javax.swing.tree.TreeModel;
import javax.swing.tree.TreeSelectionModel;
import javax.swing.tree.TreePath;

import com.sun.javatest.tool.Deck;
import com.sun.javatest.tool.UIFactory;

import com.sun.javatest.Harness;
import com.sun.javatest.JavaTestError;
import com.sun.javatest.Parameters;
import com.sun.javatest.TestResult;
import com.sun.javatest.TestResultTable;
import com.sun.javatest.TestResultTable.TreeNode;
import com.sun.javatest.TestSuite;
import com.sun.javatest.WorkDirectory;
import com.sun.javatest.tool.Preferences;
import com.sun.javatest.util.Debug;
import com.sun.javatest.util.DynamicArray;
import com.sun.javatest.util.I18NResourceBundle;
import com.sun.javatest.util.StringArray;

/**
 * This panel is a split panel which has the testsuite tree on the left and a context
 * sensitive panel on the right.
 */

class TestTreePanel extends JPanel {
    /**
     * @param map Saved state map, may be null.
     */
    public TestTreePanel(UIFactory uif, Harness h, ExecModel em,
                         FilterSelectionHandler fh, JComponent parent,
                         Map map) {
        this.uif = uif;
        this.harness = h;
        this.filterHandler = fh;
        this.execModel = em;
        this.parent = parent;
        this.stateMap = map;

        pm = new PanelModel();
        harness.addObserver(pm);

        initGUI();

        uif.setAccessibleInfo(this, "treep");
    }

    /**
     * Returns the current TRT, or null if no table is available.
     */
    public TestResultTable getTestResultTable() {
        /*
        WorkDirectory wd = params.getWorkDirectory();
            if (wd == null)
                return null;
            else
                return wd.getTestResultTable();
        */
        return treeModel.getTestResultTable();
    }

    /**
     * Discover which parameters this panel is using.
     */
    public Parameters getParameters() {
        return params;
    }

    public TreePanelModel getTreePanelModel() {
        return pm;
    }

    public void dispose() {
        disposed = true;

        // zap cache worker thread
        if (treeModel != null) {
            treeModel.dispose();
            treeModel = null;
        }

        // zap counter thread
        if (brPanel != null) {
            brPanel.dispose();
            brPanel = null;
        }

        // zap bg workers for purge or refresh
        if (bgThread != null && bgThread.isAlive())
            bgThread.interrupt();

        popup = null;
        lastPopupPath = null;
        testMenus = folderMenus = mixedMenus = customMenus = null;
    }

    // --------- private ---------

    /**
     * This method should only be called to indicate that a change has occured
     * which replaces the active TRT.  In most cases, a call to this method
     * should be followed by on to updateGUI().  Changes to the parameters
     * (filters, initial URLs, etc... should propagate thru the FilterConfig
     * system.
     *
     * @see #updateGUI
     * @see com.sun.javatest.exec.FilterConfig
     */
    void setParameters(Parameters p) {
        if (p == null)
            return;

        String[] paths = null;

        // this is important because there are no params at construction
        // time, so we need to make sure to make use of the preferences
        if (params == null && stateMap != null) {
            // trying to retrieve previous state
            String tmp = (String)(stateMap.get(OPEN_PATHS_PREF));
            if (tmp != null)
                paths = StringArray.split(tmp);
        }
        else
            paths = getOpenPaths();

        // saved some state, now make the change
        this.params = p;

        if (treeModel != null) {
            treeModel.setParameters(params);
        }

        if (tree != null) {
            tree.setParameters(params);
            treeRend.setParameters(params);
        }

        if (testPanel != null) {
            testPanel.setTestSuite(params.getTestSuite());
        }

        if (brPanel != null) {
            brPanel.setParameters(params);
        }

        // all this to restore open paths...
        TestResultTable trt = getTestResultTable();
        TreePath[] translatedPaths = getTreePaths(paths);
        if (translatedPaths != null) {
            // schedule restoration task seperately
            restore(translatedPaths, tree);
        }   // if
    }

    private TreePath[] getTreePaths(String[] paths) {
        TestResultTable trt = getTestResultTable();
        if (paths != null && trt != null) {
            TreePath[] translatedPaths = new TreePath[0];

            for (int i = 0; i < paths.length; i ++) {
                // the paths need to be reconditioned because the JTree will not
                // accept them if they refer to a different model/TRT.
                Object targetNode = trt.resolveUrl(paths[i]);

                TreePath tp = nodeToPath(targetNode, trt);
                if (tp != null)
                    translatedPaths =
                        (TreePath[])DynamicArray.append(translatedPaths, tp);
            }   // for (i)

            return translatedPaths;
        }
        else
            return null;
    }

    void save(Map m) {
        // save the open paths
        String[] paths = getOpenPaths();
        Preferences p = Preferences.access();
        m.put(OPEN_PATHS_PREF, StringArray.join(paths));
    }

    /**
     * Translates open tree paths into string URLs.
     * @return null if no paths are open or the tree is not available.
     */
    private String[] getOpenPaths() {
        if (tree == null) {
            return null;
        }

        TreePath[] paths = tree.snapshotOpenPaths();
        TestResultTable trt = getTestResultTable();
        Vector urls = new Vector();

        if (paths != null && trt != null) {
            for (int i = 0; i < paths.length; i++) {
                Object last = paths[i].getLastPathComponent();
                String url;

                if (last instanceof TestResult) {
                    // get url
                    url = ((TestResult)last).getTestName();
                }
                else {  // tree node
                    // get url
                    url = TestResultTable.getRootRelativePath((TreeNode)last);
                }

                urls.addElement(url);
            }   // for
        }

        if (urls == null || urls.size() == 0)
            return null;
        else {
            String[] result = new String[urls.size()];
            urls.copyInto(result);
            return result;
        }
    }

    private TreePath nodeToPath(Object node, TestResultTable trt) {
        if (node instanceof TestResult) {
            TestResult tr = (TestResult)node;

            // trans url to path
            Object[] ptr = TestResultTable.getObjectPath(tr);
            Object[] longPath = new Object[ptr.length+1];
            System.arraycopy(ptr, 0, longPath, 0, ptr.length);
            longPath[longPath.length-1] = tr;

            return new TreePath(longPath);
        }
        else {  // tree node
            // trans url to path
            Object[] ttp = TestResultTable.getObjectPath((TreeNode)node);

            if (ttp != null && ttp.length > 0)
                return new TreePath(ttp);
            else
                return null;
        }
    }

    private void setPopupItemsEnabled(boolean state) {
        if (state) {
            // enable all if possible
            boolean haveWorkDir = (params != null && params.getWorkDirectory() != null);
            purgeMI.setEnabled(haveWorkDir);
            refreshMI.setEnabled(params != null && params.getTestSuite() != null);

            // maybe should be based on whether the interview is complete
            runMI.setEnabled(haveWorkDir);
        }
        else {
            // disable all
            purgeMI.setEnabled(false);
            refreshMI.setEnabled(false);
            runMI.setEnabled(false);
        }
    }

    private void clearNodes(final TreePath[] what) {
        final WorkDirectory wd = execModel.getWorkDirectory();

        if (wd == null) {
            JOptionPane.showMessageDialog(parent,
                              uif.getI18NString("treep.cantPurgeNoWd.msg"),
                              uif.getI18NString("treep.cantPurgeNoWd.title"),
                              JOptionPane.WARNING_MESSAGE);
            return;
        }

        if (harness.isRunning()) {
            JOptionPane.showMessageDialog(parent,
                              uif.getI18NString("treep.cantPurgeRunning.msg"),
                              uif.getI18NString("treep.cantPurgeRunning.title"),
                              JOptionPane.WARNING_MESSAGE);
            return;
        }

        boolean ack = false;
        Object[] toPurge = new Object[what.length];

        if (what.length > 1) {
            //int confirm = uif.showYesNoDialog("treep.purgeItemsSure",
                                    //createNodeListString(createNodeList(what)));

            String[] paths = createNodeList(what);
            DefaultListModel model = new DefaultListModel();
            for (int i = paths.length; i > 0; i--)
                model.add(model.getSize(), paths[model.getSize()]);

            int confirm = showConfirmListDialog("treep.purgeItemsSure", null, model);

            // user backs out
            if (confirm == JOptionPane.NO_OPTION)
                return;
            else {
                ack = true;
                for (int i = 0; i < what.length; i++) {
                    Object item = what[i].getLastPathComponent();
                    if (item instanceof TestResult)
                        toPurge[i] = ((TestResult)item).getWorkRelativePath();
                    else if (item instanceof TreeNode) {
                        TreeNode tn = (TreeNode)item;
                        if (tn.isRoot()) {
                            toPurge = new Object[1];
                            toPurge[0] = "";
                            break;      // no need to process the rest of the list
                        }
                        else
                            toPurge[i] = TestResultTable.getRootRelativePath(tn);
                    }
                    else { }
                }   // for
            }
        }
        else if (what[0].getLastPathComponent() instanceof TestResult) {
            TestResult tr = (TestResult)(what[0].getLastPathComponent());

            int confirm = uif.showYesNoDialog("treep.purgeTestSure",
                                    tr.getTestName());

            // user backs out
            if (confirm == JOptionPane.NO_OPTION)
                return;
            else {
                //wd.purge(tr.getWorkRelativePath());
                ack = true;
                toPurge[0] = tr.getWorkRelativePath();
            }
        }
        else {
            TreeNode tn = (TreeNode)(what[0].getLastPathComponent());

            int confirm = 0;
            if (tn.isRoot())
                confirm = uif.showYesNoDialog("treep.purgeRootSure",
                                        TestResultTable.getRootRelativePath(tn));
            else
                confirm = uif.showYesNoDialog("treep.purgeNodeSure",
                                        TestResultTable.getRootRelativePath(tn));

            // user backs out
            if (confirm == JOptionPane.NO_OPTION)
                return;
            else {
                ack = true;
                toPurge[0] = TestResultTable.getRootRelativePath(tn);
            }
        }

        // only go in here if user confirmed the operation
        if (ack) {
            // this block is intended to do the following:
            // - disable menu items
            // - start purge on background thread
            // - show a wait dialog if the operation exceeds a min. time
            // - hide dialog and re-enable menu item when thread finishes
            final JDialog d = uif.createWaitDialog("treep.waitPurge", this);
            final String[] finalList = createNodeList(toPurge);

            // disable all menu items
            setPopupItemsEnabled(false);

            final Thread t = new Thread() {
                public void run() {
                    for (int i = 0; i < what.length; i++) {
                        try {
                            // this may take a long while...
                            for (int j = 0; j < finalList.length; j++)
                                wd.purge(finalList[j]);
                        }   // try
                        catch (WorkDirectory.PurgeFault f) {
                            // print something in log...
                            I18NResourceBundle i18n = uif.getI18NResourceBundle();
                            wd.log(i18n, "treep.purgeFail.err", f);
                        }   // catch
                        finally {
                            // fixup GUI on GUI thread
                            try {
                                EventQueue.invokeAndWait(new Runnable() {
                                        public void run() {
                                            if (d.isShowing())
                                                d.hide();
                                            // enable all menu items
                                            setPopupItemsEnabled(true);

                                            // reselect tree nodes
                                            TreePath[] translatedPaths = getTreePaths(finalList);
                                            if (translatedPaths != null && tree != null)
                                                tree.setSelectionPaths(translatedPaths);
                                        }
                                    });
                            }
                            catch (InterruptedException e) {
                            }
                            catch (java.lang.reflect.InvocationTargetException e) {
                            }
                        }   // outer try
                    }   // for
                }   // run()
            };  // thread

            ActionListener al = new ActionListener() {
                public void actionPerformed(ActionEvent evt) {
                    // show dialog if still processing
                    if (t == null)
                        return;
                    else if (t.isAlive() && !d.isVisible()) {
                        d.show();
                    }
                    else if (!t.isAlive() && d.isVisible()) {
                        // just in case...a watchdog type check
                        d.hide();
                    }
                }
            };

            bgThread = t;

            // show wait dialog if operation is still running after
            // WAIT_DIALOG_DELAY
            Timer timer = new Timer(WAIT_DIALOG_DELAY, al);
            timer.setRepeats(false);

            // do it!
            // in this order to reduce race condition
            timer.start();
            t.start();
        }   // outer if
    }

    // XXX need to find a shared place for these two methods to live
    static String createNodeListString(String[] items) {
        StringBuffer sb = new StringBuffer();

        for (int i = 0; i < items.length; i++) {
            sb.append("      ");
            sb.append(items[i]);

            if (i+1 < items.length)
                sb.append("\n");
        }   // for

        return sb.toString();
    }

    static String[] createNodeList(Object[] items) {
        String[] result = new String[items.length];

        for (int i = 0; i < items.length; i++) {
            Object item = items[i];

            if (item instanceof TreePath)
                item = ((TreePath)item).getLastPathComponent();

            if (item instanceof TestResult) {
                TestResult tr = (TestResult)item;
                result[i] = tr.getTestName();
            }
            else if (item instanceof TestResultTable.TreeNode) {
                TestResultTable.TreeNode tn = (TestResultTable.TreeNode)item;
                result[i] = TestResultTable.getRootRelativePath(tn);
            }
            else        // should not happen
                result[i] = items[i].toString();
        }   // for

        return result;
    }

    private void showNodeInfoDialog(TreePath what) {
        if (what.getLastPathComponent() instanceof TreeNode) {
            TreeNode tn = (TreeNode)(what.getLastPathComponent());
            Debug.println("info for this node not implemented" + tn.getName() + " (" + tn + ")");
        }
    }

    private void runNodes(TreePath[] what) {
        if (harness.isRunning()) {
            JOptionPane.showMessageDialog(parent,
                              uif.getI18NString("treep.cantRunRunning.msg"),
                              uif.getI18NString("treep.cantRunRunning.title"),
                              JOptionPane.WARNING_MESSAGE);
            return;
        }

        execModel.runTests(createNodeList(what));
    }

    private static void restore(final TreePath[] paths, final TestTree targetTree) {
        if (paths == null || targetTree == null)
            return;

        // we do it this way so that the tree updates itself, THEN we
        // ask it to restore
        Runnable restorer = new Runnable() {
            public void run() {
                targetTree.restorePaths(paths);
            }
        };      // Runnable

        EventQueue.invokeLater(restorer);
    }

    private void refreshNodes(TreePath[] what) {
        // dialog to confirm wipe of results with refresh?

        if (harness.isRunning()) {
            JOptionPane.showMessageDialog(parent,
                              uif.getI18NString("treep.cantRefreshRunning.msg"),
                              uif.getI18NString("treep.cantRefreshRunning.title"),
                              JOptionPane.WARNING_MESSAGE);
            return;
        }

        final TestResultTable trt = treeModel.getTestResultTable();
        boolean ack = false;
        Object[] ackTargets = new Object[what.length];

        if (what.length > 1) {
            //int confirm = uif.showYesNoDialog("treep.refreshNodeSure",
                //              createNodeListString(createNodeList(what)));

            String[] paths = createNodeList(what);
            DefaultListModel model = new DefaultListModel();
            for (int i = paths.length; i > 0; i--)
                model.add(model.getSize(), paths[model.getSize()]);

            int confirm = showConfirmListDialog("treep.refreshNodeSure", null, model);

            if (confirm == JOptionPane.NO_OPTION)
                return;

            for (int i = 0; i < what.length; i++) {
                Object item = what[i].getLastPathComponent();
                if (item instanceof TestResult)
                    ackTargets[i] = ((TestResult)item).getTestName();
                else if (item instanceof TreeNode) {
                    TreeNode tn = (TreeNode)item;
                    if (tn.isRoot()) {
                        ackTargets = new Object[1];
                        ackTargets[0] = null;
                    }
                    else
                        ackTargets[i] = TestResultTable.getRootRelativePath(tn);
                }
                else { }
            }   // for
        }
        else if (what[0].getLastPathComponent() instanceof TestResult) {
            final TestResult tr = (TestResult)(what[0].getLastPathComponent());

            int confirm = uif.showYesNoDialog("treep.refreshTestSure",
                                    tr.getTestName());

            // user backs out
            if (confirm == JOptionPane.NO_OPTION)
                return;

            ack = true;
            ackTargets[0] = tr.getTestName();
        }
        else {
            final TreeNode tn = (TreeNode)(what[0].getLastPathComponent());

            int confirm = JOptionPane.NO_OPTION;
            if (tn.isRoot())
                confirm = uif.showYesNoDialog("treep.refreshRootSure");
            else
                confirm = uif.showYesNoDialog("treep.refreshNodeSure",
                                TestResultTable.getRootRelativePath(tn));

            // user backs out
            if (confirm == JOptionPane.NO_OPTION)
                return;

            ack = true;
            ackTargets[0] = tn;
        }   // else

        if (ack) {
            final JDialog d = uif.createWaitDialog("treep.waitRef", this);
            final Object[] finalTargets = ackTargets;

            // disable all menu items
            setPopupItemsEnabled(false);
            final Thread t = new Thread() {
                { setName("Tree refresh"); }

                public void run() {
                    try {
                        // avoid deadlocking while other things are going on
                        trt.waitUntilReady();
                        treeModel.pauseWork();
                        for (int i = 0; i < finalTargets.length; i++)
                            try {
                                // this may take a long while...
                                    if (finalTargets[i] instanceof String)
                                        trt.refreshIfNeeded((String)finalTargets[i]);
                                    else
                                        trt.refreshIfNeeded((TreeNode)finalTargets[i]);
                            }   // try
                            catch (TestResultTable.Fault f) {
                                // log the error
                                final WorkDirectory wd = execModel.getWorkDirectory();
                                if (wd != null) {
                                    I18NResourceBundle i18n = uif.getI18NResourceBundle();
                                    wd.log(i18n, "treep.refFail",
                                            new Object[] {
                                                (finalTargets[i] instanceof String ?
                                                 finalTargets[i] :
                                                 TestResultTable.getRootRelativePath((TreeNode)finalTargets[i])
                                                 ),
                                                 f.getMessage()
                                            });
                                }
                            }   // catch
                    }
                    finally {
                        treeModel.unpauseWork();
                        // fixup GUI on GUI thread
                        try {
                            EventQueue.invokeAndWait(new Runnable() {
                                    public void run() {
                                        if (d.isShowing())
                                            d.hide();
                                        // enable all menu items
                                        setPopupItemsEnabled(true);
                                    }
                                });
                        }
                        catch (InterruptedException e) {
                        }
                        catch (java.lang.reflect.InvocationTargetException e) {
                        }   // catch
                    }   // finally
                }   // run()
            };  // thread

            ActionListener al = new ActionListener() {
                public void actionPerformed(ActionEvent evt) {
                    // show dialog if still processing
                    if (t == null)
                        return;
                    else if (t.isAlive() && !d.isVisible()) {
                        d.show();
                    }
                    else if (!t.isAlive() && d.isVisible()) {
                        // just in case...a watchdog type check
                        d.hide();
                    }
                }
            };

            bgThread = t;

            // show wait dialog if operation is still running after
            // WAIT_DIALOG_DELAY
            Timer timer = new Timer(WAIT_DIALOG_DELAY, al);
            timer.setRepeats(true);

            // do it!
            // in this order to reduce race condition
            timer.start();
            t.start();
        }   // outer if
    }

    /**
     * Call when it is likely that the Parameters have changed internally.
     * A call to this method may should be made if setParameters() is called.
     * This method should not be used to force updates when the filters have
     * changed, that should be done through the View mechanism.
     *
     * @see #setParameters
     */
    void updateGUI() {
        if (debug)
            Debug.println("TTP.updateGUI()");

        if (disposed)
            return;

        // enabled/disable popup menu items based on whether we have a
        // workdir or testsuite
        if (popup != null && params != null) {
            MenuElement[] elems = popup.getSubElements();
            if (elems != null) {
                boolean haveWorkDir = (params.getWorkDirectory() != null);
                purgeMI.setEnabled(haveWorkDir);
                refreshMI.setEnabled(params.getTestSuite() != null);

                // maybe should be based on whether the interview is complete
                runMI.setEnabled(haveWorkDir);
            }
            else { }
        }
        else { }

        tree.updateGUI();
        brPanel.updateGUI();

        TestResultTable trt = getTestResultTable();
        if (trt != null && pm != null) {
            trt.addObserver(pm);
        }
    }

    private void initGUI() {
        setName("treeAndDetails");

        JSplitPane splitPane = uif.createSplitPane(JSplitPane.HORIZONTAL_SPLIT);
        listener = new Listener();
        // null params is okay
        treeModel = new TestTreeModel(params, filterHandler, uif);
        treeRend = new TT_Renderer(uif, filterHandler, pm);

        tree = new TestTree(uif, pm, filterHandler, treeModel);
        tree.setTreeModel(treeModel);
        tree.setCellRenderer(treeRend);
        tree.addTreeSelectionListener(listener);
        tree.addTreeExpansionListener(listener);
        tree.addMouseListener(listener);
        tree.getSelectionModel().setSelectionMode(TreeSelectionModel.DISCONTIGUOUS_TREE_SELECTION);
        tree.getInputMap(JComponent.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT).put(
                        KeyStroke.getKeyStroke(KeyEvent.VK_F10,
                                        InputEvent.SHIFT_MASK,
                                        false),
                        "triggerPopup");
        tree.getActionMap().put("triggerPopup",
                    new TreePopupAction(uif.getI18NResourceBundle(), "treep.popup"));
        uif.setAccessibleInfo(tree, "treep.tree");

        JPanel left = uif.createPanel("tree", new BorderLayout(), false);

        left.add(uif.createScrollPane(tree, JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED,
                                 JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED),
                 BorderLayout.CENTER);
        splitPane.setLeftComponent(left);

        deck = new Deck() {
            public Dimension getPreferredSize() {
                int dpi = uif.getDotsPerInch();
                return new Dimension(6 * dpi, 4 * dpi);
            }
        };

        testPanel = new TestPanel(uif, harness, execModel.getContextManager());
        brPanel = new BranchPanel(uif, pm, harness, execModel, parent, filterHandler, treeModel);
        msPanel = new MultiSelectPanel(uif, pm, treeModel);
        deck.add(testPanel);
        deck.add(brPanel);
        deck.add(msPanel);

        deckPanel = uif.createPanel("main", false);
        deckPanel.setLayout(new BorderLayout());
        deckPanel.add(deck, BorderLayout.CENTER);

        // title strip, above the tabs, not above the tree
        titleField = uif.createOutputField("treep.title");
        titleField.setBorder(null);
        titleField.setText(uif.getI18NString("treep.title.noSelection.txt"));
        titleField.setBackground(MetalLookAndFeel.getMenuBackground());
        //titleField.setForeground(MetalLookAndFeel.getUserTextColor());
        titleField.setForeground(new Color(102,102,102));   // #666666
        Font f = MetalLookAndFeel.getSystemTextFont();
        titleField.setFont(new Font("Ariel", Font.BOLD, f.getSize()));
        titleField.setEnabled(true);
        titleField.setEditable(false);

        deckPanel.add(titleField, BorderLayout.NORTH);

        splitPane.setRightComponent(deckPanel);
        testPanel.setVisible(false);
        brPanel.setVisible(false);
        deck.setVisible(true);
        deckPanel.setVisible(true);
        splitPane.setDividerLocation(0.3);
        splitPane.setResizeWeight(0.3);     // 30% to left tree pane

        // make sure some node is always selected
        if (tree.getSelectionPath() == null) {
            TreeModel tm = tree.getModel();
            try {
                TreeNode root = (TreeNode)(tm.getRoot());
                //selectBranch(root, new TreePath(root));
                tree.setRootVisible(true);
                EventQueue.invokeLater(new Selector(root, new TreePath(root)));
            }
            catch (ClassCastException e) {
                // shouldn't happen really
                if (debug)
                    e.printStackTrace(Debug.getWriter());
            }
        }

        popup = uif.createPopupMenu("treep.popup.mnu");
        popup.add(runMI = uif.createMenuItem("treep.popup", "run", listener));
        popup.addSeparator();
        popup.add(refreshMI = uif.createMenuItem("treep.popup", "refresh", listener));
        popup.add(purgeMI = uif.createMenuItem("treep.popup", "clear", listener));

        // get items from the test suite
        ContextManager cm = execModel.getContextManager();
        if (cm != null) {
            JavaTestContextMenu[] cms = cm.getContextMenus();
            if (cms != null) {
                popup.addSeparator();

                for (int i = 0; i < cms.length; i++) {
                    // filter out types of menus we don't want (not needed yet)
                    // keep track of them for later
                    switch (cms[i].getMenuApplication()) {
                        case JavaTestContextMenu.TESTS_AND_FOLDERS:
                            if (mixedMenus == null)
                                mixedMenus = new ArrayList();
                            mixedMenus.add(cms[i]);
                            break;
                        case JavaTestContextMenu.TESTS_ONLY:
                            if (testMenus == null)
                                testMenus = new ArrayList();
                            testMenus.add(cms[i]);
                            break;
                        case JavaTestContextMenu.FOLDERS_ONLY:
                            if (folderMenus == null)
                                folderMenus = new ArrayList();
                            folderMenus.add(cms[i]);
                            break;
                        case JavaTestContextMenu.CUSTOM:
                            if (customMenus == null)
                                customMenus = new ArrayList();
                            customMenus.add(cms[i]);
                            break;
                        default:
                    }
                    popup.add(cms[i].getMenu());
                }   // for
            }
        }

        // disabled by default
        // will immediately be reevaluated when setParameters() is called
        runMI.setEnabled(false);
        refreshMI.setEnabled(false);
        purgeMI.setEnabled(false);

        // setup top toolbar
        /*
        JPanel topPanel = uif.createPanel("treeTb", new GridBagLayout(), false);
        // NOTE intentional space added below to defeat
        //  i18n check in build while this is commented out
        JLabel lab = uif.createLabel ("treep.filter", true);

        JComponent selector = filterHandler.getFilterSelector();
        lab.setLabelFor(selector);

        GridBagConstraints gbc = new GridBagConstraints();
        gbc.gridy = 0;
        gbc.ipadx = 12;             // JL&F
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.anchor = GridBagConstraints.CENTER;
        gbc.gridwidth = GridBagConstraints.REMAINDER;

        gbc.gridy = 1;
        gbc.gridwidth = 1;
        gbc.fill = GridBagConstraints.NONE;
        gbc.anchor = GridBagConstraints.WEST;

        topPanel.add(lab, gbc);
        topPanel.add(selector, gbc);

        topPanel.add(uif.createHorizontalStrut(15), gbc);

        gbc.anchor = GridBagConstraints.EAST;
        gbc.fill = GridBagConstraints.BOTH;
        gbc.weightx = 3;
        topPanel.add(uif.createMessageArea("treep.filter"), gbc);
        */

        setLayout(new BorderLayout());

        add(splitPane, BorderLayout.CENTER);
        //add(topPanel, BorderLayout.NORTH);

    }

    private void updatePopupMenu() {
        TreePath[] paths = tree.getSelectionPaths();

        // neither of these should be possible, since the tree should
        // always have something selected
        if (paths == null || paths.length == 0)
            return;

        if (paths.length == 1) {
            Object item = paths[0];

            if (item instanceof TreePath)
                item = ((TreePath)item).getLastPathComponent();
            // determine leaf type
            if (item instanceof TestResult) {
                if (testMenus != null)
                    for (int i = 0; i < testMenus.size(); i++) {
                        ((JavaTestContextMenu)(testMenus.get(i))).getMenu().setEnabled(true);
                        ((JavaTestContextMenu)(testMenus.get(i))).updateState((TestResult)item);
                    }   // for
                if (customMenus != null)
                    for (int i = 0; i < customMenus.size(); i++) {
                        ((JavaTestContextMenu)(customMenus.get(i))).updateState((TestResult)item);
                    }   // for
                if (mixedMenus != null)
                    for (int i = 0; i < mixedMenus.size(); i++) {
                        ((JavaTestContextMenu)(mixedMenus.get(i))).getMenu().setEnabled(true);
                        ((JavaTestContextMenu)(mixedMenus.get(i))).updateState((TestResult)item);
                    }   // for
                if (folderMenus != null)
                    for (int i = 0; i < folderMenus.size(); i++) {
                        ((JavaTestContextMenu)(folderMenus.get(i))).getMenu().setEnabled(false);
                    }   // for
            }
            else if (item instanceof TestResultTable.TreeNode) {
                TestResultTable.TreeNode tn = (TestResultTable.TreeNode)item;
                String url = TestResultTable.getRootRelativePath(tn);

                if (testMenus != null)
                    for (int i = 0; i < testMenus.size(); i++) {
                        ((JavaTestContextMenu)(testMenus.get(i))).getMenu().setEnabled(false);
                    }   // for
                if (customMenus != null)
                    for (int i = 0; i < customMenus.size(); i++) {
                        ((JavaTestContextMenu)(customMenus.get(i))).updateState(url);
                    }   // for
                if (mixedMenus != null)
                    for (int i = 0; i < mixedMenus.size(); i++) {
                        ((JavaTestContextMenu)(mixedMenus.get(i))).getMenu().setEnabled(true);
                        ((JavaTestContextMenu)(mixedMenus.get(i))).updateState(url);
                    }   // for
                if (folderMenus != null)
                    for (int i = 0; i < folderMenus.size(); i++) {
                        ((JavaTestContextMenu)(folderMenus.get(i))).getMenu().setEnabled(true);
                        ((JavaTestContextMenu)(folderMenus.get(i))).updateState(url);
                    }   // for
            }
            else {      // should not happen!
                if (testMenus != null)
                    for (int i = 0; i < testMenus.size(); i++) {
                        ((JavaTestContextMenu)(testMenus.get(i))).getMenu().setEnabled(false);
                    }   // for
                if (customMenus != null)
                    for (int i = 0; i < customMenus.size(); i++) {
                        ((JavaTestContextMenu)(customMenus.get(i))).getMenu().setEnabled(false);
                    }   // for
                if (mixedMenus != null)
                    for (int i = 0; i < mixedMenus.size(); i++) {
                        ((JavaTestContextMenu)(mixedMenus.get(i))).getMenu().setEnabled(false);
                    }   // for
                if (folderMenus != null)
                    for (int i = 0; i < folderMenus.size(); i++) {
                        ((JavaTestContextMenu)(folderMenus.get(i))).getMenu().setEnabled(false);
                    }   // for
                throw new JavaTestError("Unknown node type from JTree!");
            }
        }
        else {      // multiple nodes selected
            ArrayList tests = new ArrayList();
            ArrayList folders = new ArrayList();

            for (int i = 0; i < paths.length; i++) {
                Object item = paths[i];

                if (item instanceof TreePath)
                    item = ((TreePath)item).getLastPathComponent();
                // determine leaf type

                if (item instanceof TestResult)
                    tests.add(item);
                else
                    folders.add(TestResultTable.getRootRelativePath(
                        (TestResultTable.TreeNode)item));
            }   // for

            TestResult[] t = null;
            if (tests.size() > 0)
                tests.toArray(new TestResult[tests.size()]);

            String[] f = null;
            if (folders.size() > 0)
                folders.toArray(new String[folders.size()]);

            // currently enable/disabled state is only determined by the menu's
            // ability to deal with multi selection
            if (testMenus != null)
                for (int i = 0; i < testMenus.size(); i++) {
                    JavaTestContextMenu m = (JavaTestContextMenu)(testMenus.get(i));
                    m.getMenu().setEnabled(m.isMultiSelectAllowed());
                    m.updateState(f, t);
                }   // for
            if (customMenus != null)
                for (int i = 0; i < customMenus.size(); i++) {
                    JavaTestContextMenu m = (JavaTestContextMenu)(customMenus.get(i));
                    m.getMenu().setEnabled(m.isMultiSelectAllowed());
                    m.updateState(f, t);
                }   // for
            if (mixedMenus != null)
                for (int i = 0; i < mixedMenus.size(); i++) {
                    JavaTestContextMenu m = (JavaTestContextMenu)(mixedMenus.get(i));
                    m.getMenu().setEnabled(m.isMultiSelectAllowed());
                    m.updateState(f, t);
                }   // for
            if (folderMenus != null)
                for (int i = 0; i < folderMenus.size(); i++) {
                    JavaTestContextMenu m = (JavaTestContextMenu)(folderMenus.get(i));
                    m.getMenu().setEnabled(m.isMultiSelectAllowed());
                    m.updateState(f, t);
                }   // for
        }


        /*
        // implies
        if (paths.length > 1) {
            for (int i = 0; i < mixed
        }

        if (paths.length == 1) {
            Object item = items[i];

            if (item instanceof TreePath)
                item = ((TreePath)item).getLastPathComponent();

            // determine leaf type
            if (item instanceof TestResult) {
                tests.put((TestResult)item);
            }
            else if (item instanceof TestResultTable.TreeNode) {
                TestResultTable.TreeNode tn = (TestResultTable.TreeNode)item;
                folders.put(TestResultTable.getRootRelativePath(tn));
            }
            else        // should not happen!
                folders.put(items[i].toString()); //could throw exception instead
        }

        ArrayList folders = new ArrayList();
        ArrayList tests = new ArrageList();


        for (int i = 0; i < paths.length; i++) {
            Object item = items[i];

            if (item instanceof TreePath)
                item = ((TreePath)item).getLastPathComponent();

            // determine leaf type
            if (item instanceof TestResult) {
                tests.put((TestResult)item);
            }
            else if (item instanceof TestResultTable.TreeNode) {
                TestResultTable.TreeNode tn = (TestResultTable.TreeNode)item;
                folders.put(TestResultTable.getRootRelativePath(tn));
            }
            else        // should not happen!
                folders.put(items[i].toString()); //could throw exception instead
        }   // for
        */
    }

    /**
     * This method should be called on the AWT event thread.
     */
    private void selectTest(TestResult tr, TreePath path) {
        if (debug) {
            Debug.println("TTP showing: " + tr);
            Debug.println("   -> path provided is " + path.getPathCount() + " components long.");
        }

        if (disposed) {
            if (debug)
                Debug.println("TTP - selectTest() not running, panel is disposed.");
            return;
        }

        if (!isPopulated()) {
            if (debug)
                Debug.println("TTP - no data, cannot display a leaf. No action.");

            return;
        }

        brPanel.setVisible(false);
        deck.show(testPanel);

        // setup the tree
        if (!tree.isPathSelected(path)) {
            tree.clearSelection();
            tree.setSelectionPath(path);
        }

        if (!tree.isVisible(path))
            tree.scrollPathToVisible(path);

        // setup the test panel
        if (tr != testPanel.getTest()) {
            // select new node
            testPanel.setTest(tr);
            activeTest = tr.getTestName();
        }
        else {
            // should we ask for a refresh if == ?
            // XXX could force an update here, which is needed
            // if test panel does not have dynmaic update
            // no way to force as of 9/18/2002 though
            // this is a problem if the user selects a test while
            // it is in the running state, because the TR object
            // does not change, but the status of that object does
            // change
        }

        // configure the right side title
        titleField.setText(uif.getI18NString("treep.test", tr.getTestName()));

        testPanel.setVisible(true);

        tree.repaint();
        deckPanel.repaint();
    }

    /**
     * This method should be called on the AWT event thread.
     */
    private void selectBranch(TreeNode tn, TreePath path) {
        if (debug) {
            Debug.println("TTP showing: " + tn.getName());
            Debug.println("   -> " + tn);
            Debug.println("   -> path provided is " + path.getPathCount() + " components long.");
        }

        if (disposed) {
            if (debug)
                Debug.println("TTP - selectBranch() not running, panel is disposed.");
            return;
        }

        if (!isPopulated()) {
            if (debug)
                Debug.println("TTP - no data, cannot display a branch. No action.");

            return;
        }

        // get rid of the test panel and show the branch panel
        if (deck.getCurrentCard() != brPanel) {
            deck.show(brPanel);
            testPanel.setVisible(false);
            activeTest = null;
        }

        // setup the tree
        if (!tree.isPathSelected(path)) {
            tree.clearSelection();
            tree.setSelectionPath(path);
        }

        if (!tree.isVisible(path))
            tree.scrollPathToVisible(path);

        // setup the branch panel
        if (tn != brPanel.getNode()) {
            // select new node
            brPanel.setNode(tn);
            treeModel.setActiveNode(tn);        // hint to cache

        }
        else {
            // should we ask for a refresh if == ?
        }

        // configure the right side title
        if (tn.isRoot()) {              // root node, has no name
            TestSuite ts = params.getTestSuite();
            String tsn = ts.getName();
            if (tsn != null)    // use descriptive name of TestSuite
                titleField.setText(uif.getI18NString("treep.ts", tsn));
            else                        // TestSuite has no name, use root path
                titleField.setText(uif.getI18NString("treep.ts", ts.getPath()));
        }
        else {
            String nName = TestResultTable.getRootRelativePath(tn);
            titleField.setText(uif.getI18NString("treep.node", nName));
        }

        if (!brPanel.isVisible()) {
            brPanel.setVisible(true);
        }

        deckPanel.repaint();
        tree.repaint();
    }

    private void selectNodes(String[] paths) {
    }

    /**
     * Called when multiple nodes in the tree have been selected.
     * This method is invoked repeatably as the user adds more nodes to
     * the selection.
     */
    private void selectNodes(TreePath[] paths) {
        if (!isPopulated()) {
            if (debug)
                Debug.println("TTP - no data, cannot display selections. No action.");

            return;
        }

        Object[] leaves = new Object[paths.length];
        for (int i = 0; i < paths.length; i++)
            leaves[i] = paths[i].getLastPathComponent();

        msPanel.setNodes(leaves);

        // get rid of the test panel and show the branch panel
        if (deck.getCurrentCard() != msPanel) {
            deck.show(msPanel);
            activeTest = null;
        }

        // configure the right side title
        titleField.setText(uif.getI18NString("treep.ms"));

        msPanel.setVisible(true);
        deckPanel.repaint();
    }

    /**
     * @param prefix i18n bundle prefix
     * @param args Arguments for the user message string, which is prefix.txt.
     */
    private int showConfirmListDialog(String prefix, Object[] args, ListModel model) {
        // resources needed:
        // prefix.title
        JPanel p = uif.createPanel("ttp.confirmPanel", false);
        JTextArea msg = uif.createMessageArea(prefix, args);
        p.setLayout(new BorderLayout());
        p.add(msg, BorderLayout.NORTH);

        JList list = uif.createList("treep.nodeList", model);
        p.add(uif.createScrollPane(list,
            ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED,
            ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED), BorderLayout.CENTER);

        return uif.showCustomYesNoDialog(prefix, p);
    }

    /**
     * Does this panel currently have data to work from.
     */
    private boolean isPopulated() {
        if (params == null || params.getTestSuite() == null)
            return false;
        else
            return true;
    }

    private UIFactory uif;
    private Harness harness;
    private FilterSelectionHandler filterHandler;
    private ExecModel execModel;
    private JComponent parent;
    private Thread bgThread;        // in case disposal is required
    private Map stateMap;                   // startup state, read-only
    private boolean disposed;

    private PanelModel pm;

    private TestTree tree;
    private TT_Renderer treeRend;

    private TestPanel testPanel;
    private BranchPanel brPanel;
    private MultiSelectPanel msPanel;

    private JPopupMenu popup;
    private TreePath lastPopupPath;
    private JMenuItem refreshMI;
    private JMenuItem purgeMI;
    private JMenuItem runMI;
    private String activeTest;

    private ArrayList testMenus, folderMenus, mixedMenus, customMenus;

    private TestTreeModel treeModel;

    private Deck deck;
    private JPanel deckPanel;

    private JTextField titleField;
    private Listener listener;

    private TestSuite lastTs;       // last recorded testsuite
    private Parameters params;
    private static final int WAIT_DIALOG_DELAY = 3000;      // 3 second delay
    private static final String OPEN_PATHS_PREF = "openpaths";
    static protected boolean debug = Boolean.getBoolean("debug." + TestTreePanel.class.getName());

    private class Listener extends MouseAdapter
                    implements ActionListener, TreeSelectionListener,
                               TreeExpansionListener {
            // --- TreeSelectionListener ---
            public void valueChanged(TreeSelectionEvent e) {
                // make sure source is our tree
                // ignore the message if we are unselecting a path
                if (e.getSource() == tree) {
                    TreePath[] tp = e.getPaths();
                    TestTree source = (TestTree)(e.getSource());
                    dispatchSelection(source);
                }
            }

            // --- TreeExpansionListener ---
            // the path indicates the path to the node which contains the
            // now-(in)visible nodes
            public void treeCollapsed(TreeExpansionEvent event) {
                if (disposed)
                    return;

                // basically send a hint to the model indicating that some nodes
            // are no longer visible
            TreePath tp = event.getPath();
            //Debug.println("collapsed " + ((TreeNode)(tp.getLastPathComponent())).getName());
            TreeNode tn = (TreeNode)(tp.getLastPathComponent());

            TreeNode[] childs = tn.getTreeNodes();
            if (childs != null)
                for (int i = 0; i < childs.length; i++)
                    treeModel.removeRelevantNode(childs[i]);

            TestResult[] trs = tn.getTestResults();
            if (trs != null)
                for (int i = 0; i < trs.length; i++)
                    treeModel.removeRelevantTest(trs[i]);
        }

        public void treeExpanded(TreeExpansionEvent event) {
            if (disposed)
                return;

            // basically send a hint to the model indicating that some nodes
            // are now visible
            TreePath tp = event.getPath();
            TreeNode tn = (TreeNode)(tp.getLastPathComponent());
            //Debug.println("expanded " + ((TreeNode)(tp.getLastPathComponent())).getName());

            TreeNode[] childs = tn.getTreeNodes();
            if (childs != null)
                for (int i = 0; i < childs.length; i++)
                    treeModel.addRelevantNode(childs[i]);

            TestResult[] trs = tn.getTestResults();
            if (trs != null)
                for (int i = 0; i < trs.length; i++)
                    treeModel.addRelevantTest(trs[i]);
        }

        private void dispatchSelection(TestTree source) {
            TreePath[] paths = source.getSelectionPaths();

            if (paths == null || paths.length == 0)
                return;
            else if (paths.length == 1) {
                Object target = paths[0].getLastPathComponent();

                // single selection
                if (target instanceof TestResult) {
                    //EventQueue.invokeLater(new Selector((TestResult)target, path));
                    selectTest((TestResult)target, paths[0]);
                }
                else if (target instanceof TreeNode) {
                    //EventQueue.invokeLater(new Selector((TreeNode)target, path));
                    selectBranch((TreeNode)target, paths[0]);
                }
                else {
                    // unknown target, ignore it I guess
                    if (debug) {
                        Debug.println("Unknown node click target in TestTreePanel: ");
                        Debug.println("   => " + target);
                    }
                    else { }
                }
            }   // outer elseif
            else {
                // multiselection
                selectNodes(paths);
            }
        }

        // MouseAdapter for tree popup
        public void mousePressed(MouseEvent e) {
            maybeShowPopup(e);
        }

        public void mouseReleased(MouseEvent e) {
            maybeShowPopup(e);
        }

        private void maybeShowPopup(MouseEvent e) {
            if (e.isPopupTrigger() && e.getComponent() == tree) {
                lastX = e.getX();
                lastY = e.getY();
                // filter out clicks which don't hit the tree
                /*
                TreePath target = tree.getPathForLocation(lastX, lastY);

                if (target != null) {
                    lastPopupPath = target;
                    */
                updatePopupMenu();
                popup.show(e.getComponent(), lastX, lastY);

                /*
                    // select node which was clicked on
                    if (tree.getSelectionPath() == null)
                        tree.setSelectionPath(target);
                }
                */
            }
        }

        // ActionListener
        public void actionPerformed(ActionEvent e) {
            TreePath[] paths = tree.getSelectionPaths();

            if (e.getActionCommand().equals("clear")) {
                if (paths != null && paths.length > 0)
                    clearNodes(tree.getSelectionPaths());
                else {
                    // XXX show error dialog
                }
            }
            /*
            else if (e.getActionCommand().equals("info")) {
                showNodeInfoDialog(tree.getSelectionPaths());
            }
            */
            else if (e.getActionCommand().equals("run")) {
                if (paths != null && paths.length > 0) {
                    // special case check, remove all items if root
                    // is selected
                    for (int i = 0; i < paths.length; i++) {
                        if (paths[i].getPathCount() > 1)
                            continue;

                        Object target = paths[i].getPathComponent(0);

                        if (target instanceof TreeNode &&
                            ((TreeNode)target).isRoot()) {
                            paths = new TreePath[] {new TreePath(target)};
                            break;
                        }
                    }   // for
                    runNodes(paths);
                }
                else {
                    // XXX show error dialog
                }
            }
            else if (e.getActionCommand().equals("refresh")) {
                if (paths != null && paths.length > 0)
                    refreshNodes(tree.getSelectionPaths());
                else {
                    // XXX show error dialog
                }
            }
        }

        private int lastX;
        private int lastY;
    }

    /**
     * Utility class for scheduling an update using the GUI thread.
     */
    class Selector implements Runnable {
        Selector(TestResult tr, TreePath path) {
            this.tr = tr;
            this.tp = path;
        }

        Selector(TreeNode tn, TreePath path) {
            this.tn = tn;
            this.tp = path;
        }

        public void run() {
            if (tr != null)
                selectTest(tr, tp);
            else
                selectBranch(tn, tp);
        }

        private TreePath tp;
        private TestResult tr;
        private TreeNode tn;
    }

    private class TreePopupAction extends AbstractAction {
        TreePopupAction(I18NResourceBundle bund, String key) {
            desc = bund.getString(key + ".desc");
            name = bund.getString(key + ".act");
        }

        public void actionPerformed(ActionEvent e) {
            if (disposed)
                return;

            lastPopupPath = tree.getSelectionPath();
            Rectangle loc = tree.getPathBounds(lastPopupPath);
            if (loc == null)
                return;

            updatePopupMenu();
            popup.show(tree,
                        loc.x + (int)loc.getWidth(),
                        loc.y + (int)loc.getHeight());
        }

        public Object getValue(String key) {
            if (key == null)
                throw new NullPointerException();

            if (key.equals(NAME))
                return name;
            else if (key.equals(SHORT_DESCRIPTION))
                return desc;
            else
                return null;
        }

        private String name;
        private String desc;
    }

    private class PanelModel implements TreePanelModel, Harness.Observer, TestResultTable.Observer {
        PanelModel() {
            runningTests = new Hashtable();
            activeNodes = new Hashtable();
        }

        public void pauseWork() {
            treeModel.pauseWork();
        }

        public void unpauseWork() {
            treeModel.unpauseWork();
        }

        public void nodeSelected(Object node, TreePath path) {
        }

        public void testSelected(TestResult node, TreePath path) {
        }

        public void nodeUnSelected(Object node, TreePath path) {
        }

        public void testUnSelected(TestResult node, TreePath path) {
        }


        public void showNode(Object node, TreePath path) {
            selectBranch((TreeNode)(node), path);
        }

        public void showNode(String url) {
            TestResultTable trt = getTestResultTable();

            if (trt == null)
                return;
            else {
                TreeNode node = trt.findNode(trt.getRoot(), url);
                if (node != null)
                    selectBranch(node, new TreePath(trt.getObjectPath(node)));
            }
        }

        public void showTest(TestResult node, TreePath path) {
            selectTest(node, path);
        }

        public void showTest(TestResult tr) {
            TreeNode[] path = TestResultTable.getObjectPath(tr);
            Object[] fp = new Object[path.length + 1];
            System.arraycopy(path, 0, fp, 0, path.length);
            fp[fp.length-1] = tr;

            showTest(tr, new TreePath(fp));
        }

        public void showTest(String url) {
            TestResultTable trt = getTestResultTable();

            if (trt == null)
                return;
            else {
                TestResult tr = trt.lookup(TestResult.getWorkRelativePath(url));
                if (tr != null)
                    showTest(tr);
            }
        }

        public void hideNode(Object node, TreePath path) {
        }

        public void hideTest(TestResult node, TreePath path) {
        }

        public TestResultTable getTestResultTable() {
            return treeModel.getTestResultTable();
        }

        public String getSelectedTest() {
            return activeTest;
        }

        public boolean isActive(TestResult tr) {
            if (runningTests.containsKey(tr.getTestName()))
                return true;
            else
                return false;
        }

        public boolean isActive(TreeNode node) {
            if (activeNodes.containsKey(node))
                return true;
            else
                return false;
        }

        // Harness.Observer
        public void startingTestRun(Parameters params) {
            runMI.setEnabled(false);
            purgeMI.setEnabled(false);
            refreshMI.setEnabled(false);
        }

        public void startingTest(TestResult tr) {
            if (treeModel == null)  // may occur during shutdown, see dispose()
                return;

            runningTests.put(tr.getTestName(), tr);

            TestResult lookupTr = treeModel.getTestResultTable().lookup(tr.getWorkRelativePath());
            TreeNode[] nodes = TestResultTable.getObjectPath(lookupTr);
            for (int i = 0; i < nodes.length; i++) {
                Object hit = activeNodes.get(nodes[i]);
                if (hit == null)
                    // not currently an active node
                    activeNodes.put(nodes[i], ONE);
                else {
                    activeNodes.put(nodes[i],
                        new Integer( 1+(((Integer)hit).intValue()) )
                    );
                }
            }

            tree.repaint();     // we're allowed to call this on any thread
        }

        public void finishedTest(TestResult tr) {
            if (treeModel == null)  // may occur during shutdown, see dispose()
                return;

            runningTests.remove(tr.getTestName());


            // it's very important for items to be correctly removed
            // from the hashtables
            TestResult lookupTr = treeModel.getTestResultTable().lookup(tr.getWorkRelativePath());
            TreeNode[] nodes = TestResultTable.getObjectPath(lookupTr);

            if (tr.getTestName().equals(activeTest)) {
                Object[] p = new Object[nodes.length + 1];
                System.arraycopy(nodes, 0, p, 0, nodes.length);
                p[p.length-1] = tr;
                EventQueue.invokeLater(new Selector(tr, new TreePath(p)));
            }

            if (nodes != null)
                for (int i = 0; i < nodes.length; i++) {
                    Object hit = activeNodes.get(nodes[i]);
                    if (hit == null) {
                        // should only really happen when test run finished, not
                        // during the run
                        continue;
                    }
                    if (hit == ONE)
                        activeNodes.remove(nodes[i]);
                    else {
                        int currHits = ((Integer)hit).intValue();
                        activeNodes.put(nodes[i],
                            (currHits == 2 ?
                                ONE : new Integer(--currHits)));
                    }
                }   // for

            tree.repaint();     // we're allowed to call this on any thread
        }

        public void stoppingTestRun() {
        }

        public void finishedTesting() {
        }

        public void finishedTestRun(boolean allOK) {
            runningTests.clear();
            activeNodes.clear();

            runMI.setEnabled(true);
            purgeMI.setEnabled(true);
            refreshMI.setEnabled(true);

            tree.repaint();     // we're allowed to call this on any thread
        }

        public void error(String msg) {
        }

        // TestResultTable.Observer
        public void update(TestResult oldValue, TestResult newValue) {
            if (treeModel == null || treeModel.getTestResultTable() == null)
                return;

            // this is primarily to ensure that the subpanel gets updated when
            // asynchronous (non-user GUI) events occur
            // example - if a "clear" operation is done, which includes the shown test
            // SWITCH ONTO EVENT THREAD
            if (newValue.getTestName().equals(activeTest)) {
                TestResult lookupTr = treeModel.getTestResultTable().lookup(
                                            newValue.getWorkRelativePath());
                TreeNode[] nodes = TestResultTable.getObjectPath(lookupTr);

                Object[] p = new Object[nodes.length + 1];
                System.arraycopy(nodes, 0, p, 0, nodes.length);
                p[p.length-1] = newValue;
                EventQueue.invokeLater(new Selector(newValue, new TreePath(p)));
            }
        }

        public void updated(TestResult whichTR) {
            // ignore
        }


        private Hashtable runningTests;
        private Hashtable activeNodes;
        private Integer ONE = new Integer(1);
    }
}
