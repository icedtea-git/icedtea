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

import com.sun.interview.Interview;
import java.awt.event.ActionEvent;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;
import javax.swing.Action;
import javax.swing.JFileChooser;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.KeyStroke;

import com.sun.javatest.Harness;
import com.sun.javatest.InterviewParameters;
import com.sun.javatest.JavaTestError;
//import com.sun.javatest.TemplateUtilities;
import com.sun.javatest.TestSuite;
import com.sun.javatest.WorkDirectory;
import com.sun.javatest.tool.Desktop;
import com.sun.javatest.tool.FileHistory;
import com.sun.javatest.tool.FileOpener;
import com.sun.javatest.tool.Preferences;
import com.sun.javatest.tool.UIFactory;
import com.sun.javatest.tool.TestSuiteChooser;
import com.sun.javatest.tool.Tool;
import com.sun.javatest.tool.ToolAction;
import com.sun.javatest.tool.ToolManager;
import com.sun.javatest.tool.WorkDirChooser;


/**
 * The ToolManager for {@link ExecTool test manager} windows.
 */
public class ExecToolManager extends ToolManager
{
    /**
     * Create an ExecManager to manage the test manager windows on a desktop.
     * @param desktop the desktop for which this manager is responsible
     */
    public ExecToolManager(Desktop desktop) {
        super(desktop);
    }

    public FileOpener[] getFileOpeners() {
        return fileOpeners;
    }

    public Action[] getFileMenuActions() {
        // The newWorkDirAction is enabled if we can reasonably deduce a
        // test suite for it to operate on, so, without opening anything new,
        // we check through a candidate list of test suites. If we find one,
        // we enabled the newWorkDirAction, and stash some private data
        // in it that will enable us to determine the test suite if that
        // action is performed.
        // The openWorkDirAction is always enabled, but we stash private data
        // on it in case a default test suite is required.

        boolean done = false;
        Object tsInfo = null;
        Desktop d = getDesktop();

        // check if the current tool is an ExecTool with a test suite loaded
        // if so, set tsInfo to the test suite
        Tool t = d.getSelectedTool();
        if (t != null && (t instanceof ExecTool)) {
            TestSuite ts = ((ExecTool) t).getTestSuite();
            if (ts != null) {
                newWorkDirAction.setEnabled(true);
                newWorkDirAction.putValue("testSuite", new WeakReference(ts));
                openWorkDirAction.putValue("testSuite", new WeakReference(ts));
                done = true;
            }
        }

        // if not got a test suite yet, check if the collected set of current tools
        // define a unique test suite; if they define one, use it; if they define more
        // than one, give up.
        if (!done) {
            Tool[] tools = d.getTools();
            Set s = new HashSet();
            for (int i = 0; i < tools.length; i++) {
                TestSuite[] tss = tools[i].getLoadedTestSuites();
                if (tss != null)
                    s.addAll(Arrays.asList(tss));
            }
            if (s.size() == 1) {
                TestSuite ts = (TestSuite) (s.toArray()[0]);
                newWorkDirAction.setEnabled(true);
                newWorkDirAction.putValue("testSuite", new WeakReference(ts));
                openWorkDirAction.putValue("testSuite", new WeakReference(ts));
            }
            else
                newWorkDirAction.setEnabled(false);
            done = (s.size() > 0);
        }

        // if not got a test suite yet, check if the user's directory is a test suite;
        // if so, remember that filename (don't open the test suite because it might not
        // be needed.)
        if (!done) {
            try {
                if (TestSuite.isTestSuite(userDir)) {
                    newWorkDirAction.setEnabled(true);
                    newWorkDirAction.putValue("testSuitePath", userDir);
                    openWorkDirAction.putValue("testSuitePath", userDir);
                    done = true;
                }
            }
            catch (Exception e) {
            }
        }

        // if not got a test suite yet, check if the user's directory is a work directory;
        // if so, remember that filename (don't open the test suite because it might not
        // be needed.)
        if (!done) {
            try {
                if (WorkDirectory.isWorkDirectory(userDir)) {
                    newWorkDirAction.setEnabled(true);
                    newWorkDirAction.putValue("workDirPath", userDir);
                    openWorkDirAction.putValue("workDirPath", userDir);
                    done = true;
                }
            }
            catch (Exception e) {
            }
        }


        // if not got a test suite yet, check if the JT installation directory or its parent
        // is a test suite; if so, remember that filename (don't open the test suite because
        // it might not be needed.)
        if (!done) {
            try {
                File classDir = Harness.getClassDir();
                File installDir = (classDir == null ? null : classDir.getParentFile());
                File installParentDir = (installDir == null ? null : installDir.getParentFile());
                if (installDir != null && TestSuite.isTestSuite(installDir)) {
                    newWorkDirAction.setEnabled(true);
                    newWorkDirAction.putValue("testSuitePath", installDir);
                    openWorkDirAction.putValue("testSuitePath", installDir);
                    done = true;
                }
                else if (installParentDir != null && TestSuite.isTestSuite(installParentDir)) {
                    newWorkDirAction.setEnabled(true);
                    newWorkDirAction.putValue("testSuitePath", installParentDir);
                    openWorkDirAction.putValue("testSuitePath", installParentDir);
                    done = true;
                }
            }
            catch (Exception e) {
            }
        }

        if (!done)
            newWorkDirAction.setEnabled(false);

        return fileMenuActions;
    }

    public JMenuItem[] getFileMenuPrimaries() {
        JMenu openMenu = new JMenu(i18n.getString("tmgr.openMenu.menu"));
        openMenu.setName("tmgr.openMenu");
        // this craziness usually done by UIFactory
        String keyString = i18n.getString("tmgr.openMenu.mne");
        KeyStroke keyStroke = KeyStroke.getKeyStroke(keyString);
        openMenu.setMnemonic(keyStroke.getKeyCode());
        openMenu.getAccessibleContext().setAccessibleDescription(i18n.getString("tmgr.openMenu.desc"));
        openMenu.add(new JMenuItem(openWorkDirAction));
        openMenu.add(new JMenuItem(openTestSuiteAction));

        return new JMenuItem[] {openMenu};
    }

    public JMenuItem[] getHelpPrimaryMenus() {
        Desktop d = getDesktop();
        Tool t = d.getSelectedTool();
        if (t != null && (t instanceof ExecTool)) {
            ExecTool et = (ExecTool)t;
            ContextManager context = et.getContextManager();
            if (context == null)
                return null;

            JavaTestMenuManager mm = context.getMenuManager();
            if (mm != null)
                return mm.getMenuItems(JavaTestMenuManager.HELP_PRIMARY);
            else
                return null;
        }
        return null;
    }

    public JMenuItem[] getHelpTestSuiteMenus() {
        Desktop d = getDesktop();
        Tool t = d.getSelectedTool();
        if (t != null && (t instanceof ExecTool)) {
            ExecTool et = (ExecTool)t;
            ContextManager context = et.getContextManager();
            if (context == null)
                return null;

            JavaTestMenuManager mm = context.getMenuManager();
            if (mm != null)
                return mm.getMenuItems(JavaTestMenuManager.HELP_TESTSUITE);
            else
                return null;

        }
        return null;
    }

    public JMenuItem[] getHelpAboutMenus() {
        Desktop d = getDesktop();
        Tool t = d.getSelectedTool();
        if (t != null && (t instanceof ExecTool)) {
            ExecTool et = (ExecTool)t;
            ContextManager context = et.getContextManager();
            if (context == null)
                return null;

            JavaTestMenuManager mm = context.getMenuManager();
            if (mm != null)
                return mm.getMenuItems(JavaTestMenuManager.HELP_ABOUT);
            else
                return null;
        }
        return null;
    }

    public Action[] getTaskMenuActions() {
        return null;
    }

    public Action[] getWindowOpenMenuActions() {
        Action a = new ToolAction(i18n, "tmgr.openTm") {
            public void actionPerformed(ActionEvent e) {
                doneQuickStart = false;
                ExecTool t = (ExecTool)startTool();
                Desktop d = getDesktop();
                d.setSelectedTool(t);
                t.showQuickStartWizard();
            }
        };
        return new Action[] { a };
    }
    public Preferences.Pane getPrefsPane() {
        if (prefsPane == null)
            prefsPane = new PrefsPane();
        return prefsPane;
    }


    /**
     * If ExecTool have SINGLE_TEST_MANAGER enabled then
     * this method check SINGLE_TEST_MANAGER in all
     * loaded tools and return false if such found.
     *
     * @param newTool new tool which is added to Dektop
     * @param d Desktop to add
     * @return true if there is no conflict with SINGLE_TEST_MANAGER
     *             false otherwise
     */
    boolean checkOpenNewTool(ExecTool newTool, Desktop d) {
        return checkOpenNewTool(d, newTool.getContextManager());
    }

    boolean checkOpenNewTool(Desktop d, ContextManager conManager) {
        if (conManager != null  && conManager.getFeatureManager().isEnabled(
                        FeatureManager.SINGLE_TEST_MANAGER)) {
            Tool[] tools = d.getTools();
            ArrayList list = new ArrayList();
            for (int i = 0; i < tools.length; i++) {
                if (tools[i] instanceof ExecTool) {
                    ExecTool tool = (ExecTool) tools[i];
                    ContextManager cm = tool.getContextManager();
                    if (cm != null) {
                        FeatureManager fm = cm.getFeatureManager();
                        if (fm.isEnabled(FeatureManager.SINGLE_TEST_MANAGER)) {
                            // only single test manager
                            list.add(tools[i]);
                        }
                    }
                }
            }
            if (list.isEmpty()) {
                return true;
            }
            if (list.size() == 1) {
                if (showCloseQuestion() == JOptionPane.YES_OPTION) {
                    ExecTool old = (ExecTool) list.get(0);
                    old.getDesktop().removeTool(old);
                    old.dispose();
                    return true;
                } else {
                    return false;
                }
            }
            showError("tse.single");
            return false;
        }
        return true;
    }


    public Tool startTool() {
        Desktop d = getDesktop();



        ExecTool t = new ExecTool(this);

        if (!checkOpenNewTool(t, d)) {
            return null;
        }
        d.addTool(t);
        if (d.isFirstTime() && !doneQuickStart) {
            t.showQuickStartWizard();
            doneQuickStart = true;
        }

        return t;
    }

    /**
     * Start an ExecTool for a particular configuration.
     * @param p the configuration defining the tests and test results to be
     * displayed
     * @return the tool created to show the tests and test results specified
     * by the configuration
     */
    public Tool startTool(InterviewParameters p) {
        Desktop d = getDesktop();
        ExecTool t = new ExecTool(this, p);
        if (!checkOpenNewTool(t, d)) {
            return null;
        }
        d.addTool(t);
        d.setSelectedTool(t);
        return t;
    }

    public Tool restoreTool(Map m) throws Fault {
        try {
            return new ExecTool(this, m);
        }
        catch (InterviewParameters.WorkDirFault e) {
            throw new Fault(i18n, "mgr.restoreFaultWD", e.getMessage());
        }
        catch (InterviewParameters.TestSuiteFault e) {
            throw new Fault(i18n, "mgr.restoreFaultTS", e.getMessage());
        }
        catch (InterviewParameters.JTIFault e) {
            throw new Fault(i18n, "mgr.restoreFaultJTI", e.getMessage());
        }
        catch (InterviewParameters.Fault e) {
            throw new Fault(i18n, "mgr.restoreFault", e.getMessage());
        }
    }

    //-------------------------------------------------------------------------

    /**
     * Create an ExecTool instance using the given test suite.
     * @param ts the test suite to seed the new tool with
     * @return tool instance now associated with the given test suite
     * @throws Interview.Fault if there is a problem initializing
     *         the test suite interview parameters
     * @throws TestSuite.Fault if there is a problem while accessing the test
     *         suite object
     */
    public ExecTool showTestSuite(TestSuite ts)
        throws InterviewParameters.Fault, TestSuite.Fault
    {
        // check to see if there is an empty tool; if so select it
        Desktop d = getDesktop();
        ExecTool usableExecTool = null;

        Tool[] tools = d.getTools();
        if (tools != null) {
            // check to see if there is an empty tool; if so, note it
            for (int i = 0; i < tools.length && usableExecTool == null; i++) {
                if (tools[i] instanceof ExecTool) {
                    ExecTool t = (ExecTool) tools[i];
                    if (t.isEmpty() && !(t.isQuickStartWizardShowing()))
                        usableExecTool = t;
                }
            }
        }

        // create or update a suitable exec tool
        ExecTool t;
        if (usableExecTool != null) {
            t = usableExecTool;
            t.setTestSuite(ts);
        }
        else {
            t = new ExecTool(this, ts);
            if (!checkOpenNewTool(t, d)) {
                return null;
            }
            d.addTool(t);
        }
        d.setSelectedTool(t);
        //d.addToFileHistory(ts.getRoot(), testSuiteOpener);
        return t;
    }

    private TestSuiteChooser getTestSuiteChooser() {
        if (testSuiteChooser == null)
            testSuiteChooser = new TestSuiteChooser();

        return testSuiteChooser;
    }

    void addToFileHistory(TestSuite ts) {
        // for 4.0, we think adding test suites is not useful
        //getDesktop().addToFileHistory(ts.getRoot(), testSuiteOpener);
    }

    //-------------------------------------------------------------------------

    /**
     * Create an ExecTool instance using the given work directory.
     * @param wd the work directory to open
     * @return tool instance now associated with the given work directory
     * @throws Interview.Fault if there is a problem initializing
     *         the test suite interview parameters
     * @throws TestSuite.Fault if there is a problem while accessing the test
     *         suite object
     */
    public ExecTool showWorkDirectory(WorkDirectory wd)
        throws InterviewParameters.Fault, TestSuite.Fault
    {
        // check to see if there is a matching tool; if so, select it
        Desktop d = getDesktop();
        ExecTool usableExecTool = null;
        Tool[] tools = d.getTools();

        if (tools != null) {
            // look for a good match first, but note the first empty one too.
            for (int i = 0; i < tools.length; i++) {
                if (tools[i] instanceof ExecTool) {
                    ExecTool t = (ExecTool) tools[i];
                    if (usableExecTool == null && t.isEmpty() && !(t.isQuickStartWizardShowing() )) {
                        // save for later in case no better match
                        usableExecTool = t;
                    }
                    else if (t.getWorkDirectory() == null
                             && t.containsTestSuite(wd.getTestSuite())) {
                        // this is as good as it gets -- so use it
                        usableExecTool = t;
                        break;
                    }
                }
            }
        }

        // create or update a suitable exec tool
        ExecTool t;
        if (usableExecTool != null) {
            t = usableExecTool;

            if (t.isEmpty()) {
                d.removeTool(t);
                t = new ExecTool(this, wd);
                d.addTool(t);
            }
            else {
                t.setWorkDirectory(wd);
            }
        }
        else {
            /*
            t = new ExecTool(this, wd);
            */

            TestSuite ts = wd.getTestSuite();
            InterviewParameters params = ts.createInterview();
            params.setWorkDirectory(wd);

            FileHistory h = FileHistory.getFileHistory(wd, "configHistory.jtl");
            File latestConfigFile = h.getLatestEntry();

            if (latestConfigFile != null) {
                try {
                    params.load(latestConfigFile);
                }
                catch (IOException e) {
                    showError("tmgr.cantLoadDefaultConfig", new Object[] { latestConfigFile, e });
                }
                catch (InterviewParameters.Fault e) {
                    showError("tmgr.cantLoadDefaultConfig", new Object[] { latestConfigFile, e.getMessage() });
                }
            }

            t = new ExecTool(this, params);
            if (!checkOpenNewTool(t, d)){
                return null;
            }
            d.addTool(t);

        }
        d.setSelectedTool(t);
        d.addToFileHistory(wd.getRoot(), workDirOpener);
        t.getContextManager().setWorkDir(wd);
        return t;
    }

    void addToFileHistory(WorkDirectory wd) {
        getDesktop().addToFileHistory(wd.getRoot(), workDirOpener);
    }

    void showError(String key) {
        showError(key, (String[]) null);
    }

    void showError(String key, Object arg) {
        showError(key, new Object[] { arg });
    }

    void showError(String key, Object[] args) {
        if (uif == null)
                uif = new UIFactory(getClass(), getDesktop().getDialogParent());
        uif.showError(key, args);
    }

    int showCloseQuestion() {
        if (uif == null)
                uif = new UIFactory(getClass(), getDesktop().getDialogParent());
        return uif.showYesNoDialog("tse.closeCurrent");
    }



    //-------------------------------------------------------------------------

    private Action openQuickStartAction = new ToolAction(i18n, "mgr.openQuickStart") {
        public void actionPerformed(ActionEvent e) {
            Desktop d = getDesktop();

            ExecTool t = new ExecTool(ExecToolManager.this);
            if (!checkOpenNewTool(t, d)){
                return;
            }
            d.addTool(t);
            d.setSelectedTool(t);

            t.showQuickStartWizard();
        }
    };

    //-------------------------------------------------------------------------

    private Action openTestSuiteAction = new ToolAction(i18n, "mgr.openTestSuite") {
        public void actionPerformed(ActionEvent e) {
            //System.err.println("EM:openTestSuiteAction " + e);
            try {
                TestSuiteChooser tsc = getTestSuiteChooser();

                int action = tsc.showDialog(getDesktop().getDialogParent());
                if (action != JFileChooser.APPROVE_OPTION)
                    return;

                showTestSuite(tsc.getSelectedTestSuite());
                tsc.setSelectedTestSuite(null);
            }
            catch (InterviewParameters.Fault ex) {
                showError("tmgr.cantOpenTestSuite", ex.getMessage());
            }
            catch (TestSuite.Fault ex) {
                showError("tmgr.cantOpenTestSuite", ex.getMessage());
            }
        }
    };

    //-------------------------------------------------------------------------

    private Action openWorkDirAction = new ToolAction(i18n, "mgr.openWorkDir") {
        public void actionPerformed(ActionEvent e) {
            File tsp = ((File) getValue("testSuitePath"));
            File wdp = ((File) getValue("workDirPath"));

            TestSuite ts = null;
            try {
                if (tsp != null)
                    ts = TestSuite.open(tsp);
                else if (wdp != null)
                    ts = WorkDirectory.open(wdp).getTestSuite();
            }
            catch (Exception ignore) {
                // we're only looking for a default test suite,
                // so ignore any problems trying to find one
            }

            Desktop d = getDesktop();

            // ExecTool t = new ExecTool(ExecToolManager.this);
            if (uif == null)
                uif = new UIFactory(getClass(), getDesktop().getDialogParent());

            Tool currentTool = d.getSelectedTool();
            ExecTool newET = null;
            if (currentTool != null && (currentTool instanceof ExecTool) && ((ExecTool)currentTool).getWorkDirectory() == null) {
                // reuse current tool
                newET = (ExecTool)currentTool;
            } else {
                if (ts != null) {
                    try {
                        newET = new ExecTool(ExecToolManager.this, ts);
                    } catch (TestSuite.Fault ex) {
                        showError("tmgr.cantCreateWorkDir", ex.getMessage());
                        return;
                    } catch (Interview.Fault ex) {
                        showError("tmgr.cantCreateWorkDir", ex.getMessage());
                        return;
                    }
                } else {
                    newET = new ExecTool(ExecToolManager.this);
                }

                if (currentTool instanceof ExecTool) {
                    // currentTool can be null if all TestManagers closed and no test suite specified
                    if (!checkOpenNewTool((ExecTool)currentTool, d)) {
                        newET.dispose();
                        return;
                    }
                }

                d.addTool(newET);
                d.setSelectedTool(newET);
            }
            WorkDirChooseTool wdTool = WorkDirChooseTool.getTool(newET, newET.getUIF(), newET, WorkDirChooser.OPEN_FOR_ANY_TESTSUITE, ts, true);
            wdTool.doTool();
        }
    };


    //-------------------------------------------------------------------------

    private Action newWorkDirAction = new ToolAction(i18n, "mgr.newWorkDir") {
        public void actionPerformed(ActionEvent e) {
            //System.err.println("EM.newWorkDir");
            WeakReference tsr = ((WeakReference) getValue("testSuite"));
            TestSuite tsv = ((TestSuite) (tsr == null ? null : tsr.get()));
            File tsp = ((File) getValue("testSuitePath"));
            File wdp = ((File) getValue("workDirPath"));
            try {
                TestSuite ts;
                if (tsv != null)
                    ts = tsv;
                else if (tsp != null)
                    ts = TestSuite.open(tsp);
                else if (wdp != null)
                    ts = WorkDirectory.open(wdp).getTestSuite();
                else
                    throw new JavaTestError("ExecManager.newWorkDirAction");

                Desktop d = getDesktop();
                ExecTool t = null;

                Tool tool = d.getSelectedTool();
                ExecTool oldTool = (tool != null && (tool instanceof ExecTool)) ? (ExecTool) tool : null;
                if (oldTool != null && ((ExecTool)tool).getWorkDirectory() == null) {
                    t = (ExecTool)tool;
                } else {

                    if (ts != null) {
                        try {
                            t = new ExecTool(ExecToolManager.this, ts);
                        } catch (TestSuite.Fault ex) {
                            showError("tmgr.cantCreateWorkDir", ex.getMessage());
                            return;
                        } catch (Interview.Fault ex) {
                            showError("tmgr.cantCreateWorkDir", ex.getMessage());
                            return;
                        }
                    } else {
                        t = new ExecTool(ExecToolManager.this);
                    }

                    if (!checkOpenNewTool(t, d)) {
                        t.dispose();
                        return;
                    }

                    d.addTool(t);
                    d.setSelectedTool(t);
                }
                if (uif == null)
                    uif = new UIFactory(getClass(), getDesktop().getDialogParent());

                ExecTool et = (ExecTool)t;
                WorkDirChooseTool.getTool(et, et.getUIF(), et, WorkDirChooser.NEW, ts, true).doTool();
            }

            catch (TestSuite.Fault ex) {
                showError("tmgr.cantCreateWorkDir", ex.getMessage());
            }
            catch (WorkDirectory.Fault ex) {
                showError("tmgr.cantCreateWorkDir", ex.getMessage());
            }
            catch (FileNotFoundException ex) {
                // should not happen because the action should not be enabled
                // and the paths should not be set if they don't exist
                JavaTestError.unexpectedException(ex);
            }
        }
    };

    //-------------------------------------------------------------------------

    private FileOpener testSuiteOpener = new FileOpener() {
        public String getFileType() {
            return "testSuite";
        }

        public void open(File f) throws FileNotFoundException, Fault {
            try {
                showTestSuite(TestSuite.open(f));
            }
            catch (InterviewParameters.Fault e) {
                throw new Fault(i18n, "mgr.errorOpeningTestSuite", new Object[] { f, e });
            }
            catch (TestSuite.Fault e) {
                throw new Fault(i18n, "mgr.errorOpeningTestSuite", new Object[] { f, e });
            }
        }
    };

    //-------------------------------------------------------------------------

    private FileOpener workDirOpener = new FileOpener() {
        public String getFileType() {
            return "workDirectory";
        }

        public void open(File f) throws FileNotFoundException, Fault {
            try {
                showWorkDirectory(WorkDirectory.open(f));
            }
            catch (InterviewParameters.Fault e) {
                throw new Fault(i18n, "mgr.errorOpeningWorkDirectory", new Object[] { f, e.getMessage() });
            }
            catch (TestSuite.Fault e) {
                throw new Fault(i18n, "mgr.errorOpeningWorkDirectory", new Object[] { f, e.getMessage() });
            }
            catch (WorkDirectory.Fault e) {
                throw new Fault(i18n, "mgr.errorOpeningWorkDirectory", new Object[] { f, e.getMessage() });
            }

            Preferences prefs = Preferences.access();
            try {
                prefs.setPreference(WorkDirChooseTool.DEFAULT_WD_PREF_NAME,
                                        f.getParentFile().getCanonicalPath());
            }
            catch (IOException e) {}
        }
    };

    //-------------------------------------------------------------------------

    private TestSuiteChooser testSuiteChooser;
    private PrefsPane prefsPane;
    private boolean doneQuickStart, doneWDChoser;
    private UIFactory uif;

    private static final String EXEC = "exec";
    private static final File userDir = new File(System.getProperty("user.dir"));

    private FileOpener[] fileOpeners = {
        testSuiteOpener,
        workDirOpener
    };

    private Action[] fileMenuActions = {
        openQuickStartAction,
        newWorkDirAction,
        //openWorkDirAction,
        //openTestSuiteAction,
    };
}
