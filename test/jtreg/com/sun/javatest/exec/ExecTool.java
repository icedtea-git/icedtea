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

import com.sun.javatest.tool.UIFactory;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.EventQueue;
import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.HierarchyEvent;
import java.awt.event.HierarchyListener;
import java.io.File;
import java.io.IOException;
import java.util.Arrays;
import java.util.Map;
import java.util.ResourceBundle;
import java.util.Vector;
import java.util.logging.Handler;
import java.util.logging.Logger;
import javax.swing.Action;
import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JCheckBoxMenuItem;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.JToolBar;
import javax.swing.SwingUtilities;
import javax.swing.plaf.metal.MetalLookAndFeel;

import com.sun.interview.Interview;
import com.sun.interview.Question;
import com.sun.javatest.Harness;
import com.sun.javatest.InterviewParameters;
import com.sun.javatest.TemplateUtilities;
import com.sun.javatest.TestEnvironment;
import com.sun.javatest.TestFinder;
import com.sun.javatest.TestResultTable;
import com.sun.javatest.TestSuite;
import com.sun.javatest.WorkDirectory;
import com.sun.javatest.logging.WorkDirLogHandler;
import com.sun.javatest.tool.FileHistory;
import com.sun.javatest.tool.Preferences;
import com.sun.javatest.tool.TestSuiteChooser;
import com.sun.javatest.tool.Tool;
import com.sun.javatest.tool.ToolAction;
import com.sun.javatest.tool.WorkDirChooser;

import com.sun.javatest.util.I18NResourceBundle;
import java.awt.print.Printable;

/**
 * The "Test Manager" tool, which allows a user to browse, configure,
 * and run tests.
 */
public class ExecTool extends Tool implements ExecModel {
    /**
     * Create a default, uninitialized ExecTool.
     *
     * @param mgr the manager for this tool
     */
    public ExecTool(ExecToolManager mgr) {
        this(mgr, (InterviewParameters) null);
    }


    /**
     * Create an ExecTool for a specific test suite.
     *
     * @param mgr       the manager for this tool
     * @param testSuite The test suite to be shown in this tool.
     * @throws TestSuite.Fault if a problem occurs creating the
     *                         configuration interview for this test suite.
     * @throws Interview.Fault if a problem occurs creating the
     *                         configuration interview for this test suite.
     */
    public ExecTool(ExecToolManager mgr, TestSuite testSuite)
            throws Interview.Fault, TestSuite.Fault {
        this(mgr, testSuite.createInterview());
    }


    /**
     * Create an ExecTool for a specific test suite and work directory.
     *
     * @param mgr     the manager for this tool
     * @param workDir The work directory to be shown in this tool.
     * @throws Interview.Fault if a problem occurs creating the
     *                         configuration interview for the test suite.
     * @throws TestSuite.Fault if a problem occurs creating the
     *                         configuration interview for the test suite.
     */
    public ExecTool(ExecToolManager mgr, WorkDirectory workDir)
            throws Interview.Fault, TestSuite.Fault {
        this(mgr, getInterview(workDir));
    }

    private static InterviewParameters getInterview(WorkDirectory workDir)
            throws Interview.Fault, TestSuite.Fault {
        TestSuite ts = workDir.getTestSuite();
        InterviewParameters params = ts.createInterview();
        params.setWorkDirectory(workDir);

        FileHistory h = FileHistory.getFileHistory(workDir, "configHistory.jtl");
        File latestConfigFile = h.getLatestEntry();

        if (latestConfigFile != null) {
            try {
                params.load(latestConfigFile);
            }
            catch (IOException e) {
                // ignore?
                /*
                uif.showError("exec.cantLoadDefaultConfig",
                              new Object[] { latestConfigFile, e });
                */
            }
        }

        return params;
    }

    /**
     * Create an ExecTool initialized to the contents of an interview object.
     *
     * @param mgr             the manager for this tool
     * @param interviewParams The interview object containing the test suite and
     *                        work directory to be displayed.
     */
    public ExecTool(ExecToolManager mgr, InterviewParameters interviewParams) {
        this(mgr, interviewParams, null);
    }

    public void dispose() {
        // standard cleanup (remove refs to child components)
        super.dispose();

        // dispose dialogs like configEditor, various popup browsers, etc

        /* some test suites want this, but other part of the code assume that
         * the test suite object is reused, further study needed.
        ContextManager cm = getContextManager();
        if (cm != null &&
            cm.getFeatureManager().isEnabled(FeatureManager.SINGLE_TEST_MANAGER))
                TestSuite.close(testSuite.getRoot());
        */

        configHandler.dispose();

        // ensure no tests running
        runTestsHandler.dispose();

        // ensure cache worker thread killed
        testTreePanel.dispose();

        reportHandler.dispose();

        // no need to track changes in preferences any more
        Preferences p = Preferences.access();
        p.removeObserver(TOOLBAR_PREF, prefsObserver);

        // remove the error handler
        if (testSuite != null) {
            // hmm, not good that errors are limited to a single handler/exectool
            TestFinder tf = testSuite.getTestFinder();
            if (tf.getErrorHandler() == testFinderErrorHandler)
                tf.setErrorHandler(null);
        }
        TU_ViewManager.dispose(this);
    }

    protected String[] getCloseAlerts() {
        Vector v = null;
        String configName = null;

        if (workDir == null)  // can't be any alerts if work dir not set
            return null;


        if (configHandler.isConfigEdited()) {
            v = new Vector();
            configName = getConfigName();

            v.add(uif.getI18NString("exec.alert.unsavedData",
                    new Object[]{configName, workDir.getPath()}));
        }

        Harness h = runTestsHandler.getHarness();
        if (h != null && h.isRunning()) {
            if (v == null)
                v = new Vector();
            if (configName == null)
                configName = getConfigName();

            v.add(uif.getI18NString("exec.alert.taskRunning",
                    new Object[]{configName, workDir.getPath()}));
        }

        closeLogger();

        if (v == null)
            return null;
        else {
            String[] alerts = new String[v.size()];
            v.copyInto(alerts);
            return alerts;
        }
    }

    private String getConfigName() {
        if (interviewParams != null) {
            TestEnvironment env = interviewParams.getEnv();
            if (env != null)
                return env.getName();
        }

        return uif.getI18NString("exec.unknownConfig");
    }

    public UIFactory getUIF() {
        return uif;
    }

    // part of ExecModel
    /**
     * @return Null if the test suite has not been established, or if there is
     *         no context mananger available.
     */
    public ContextManager getContextManager() {
        if (context != null)
            return context;

        context = createContextManager(testSuite);

        if (context != null) {
            context.setInterview(interviewParams);
            context.setTool(this);
            context.setWorkDir(workDir);
        }

        return context;
    }


    static ContextManager createContextManager(TestSuite testSuite) {

        ContextManager context = null;

        String cls = null;
        if (testSuite != null)
            cls = testSuite.getTestSuiteInfo("tmcontext");

        try {
            if (cls == null) {
                // use default implementation
                context = (ContextManager) ((Class.forName(
                        "com.sun.javatest.exec.ContextManager")).newInstance());
            } else {
                context = (ContextManager) ((Class.forName(cls, true,
                        testSuite.getClassLoader())).newInstance());
            }
            context.setTestSuite(testSuite);
        }
        catch (ClassNotFoundException e) {
            e.printStackTrace();        // XXX rm
            // should print log entry
        }
        catch (InstantiationException e) {
            e.printStackTrace();        // XXX rm
            // should print log entry
        }
        catch (IllegalAccessException e) {
            e.printStackTrace();        // XXX rm
            // should print log entry
        }

        return context;
    }

    /**
     * Get the tool menu bar.
     */
    public JMenuBar getMenuBar() {
        return menuBar;
    }

    public Dimension getPreferredSize() {
        Dimension d = super.getPreferredSize();
        int dpi = uif.getDotsPerInch();
        d.width = Math.max(d.width, 6 * dpi);
        d.height = Math.max(d.height, 3 * dpi);
        return d;
    }

    /**
     * Check if this tool is empty: in other words, check if it has
     * a test suite loaded or not.
     *
     * @return true if there is no test suite loaded
     */
    public boolean isEmpty() {
        return (testSuite == null);
    }

    /**
     * Get the currently loaded test suite, if any.
     *
     * @return the currently loaded test suite, or null if none.
     */
    public TestSuite getTestSuite() {
        return testSuite;
    }

    public TestSuite[] getLoadedTestSuites() {
        return (testSuite == null ? null : new TestSuite[]{testSuite});
    }

    /**
     * Check if this tool contains a given test suite.
     *
     * @param ts The test suite to check for.
     * @return true if this tool contains the given test suite.
     */
    public boolean containsTestSuite(TestSuite ts) {
        return (testSuite != null && testSuite.getRoot().equals(ts.getRoot()));
    }

    /**
     * Check if this tool contains a test suite whose root file matches
     * a specified file.
     *
     * @param f The root file of the test suite for which to check.
     * @return true if this tool contains a test suite whose root file
     *         is the same as the specified file.
     */
    public boolean containsTestSuite(File f) {
        return (testSuite != null && testSuite.getRoot().equals(f));
    }

    void setTestSuite(TestSuite testSuite)
            throws Interview.Fault, TestSuite.Fault {
        init(testSuite, null);
    }


    /**
     * Get the currently loaded work directory, if any.
     *
     * @return the currently loaded work directory, or null if none.
     */
    public WorkDirectory getWorkDirectory() {
        return workDir;
    }

    public WorkDirectory[] getLoadedWorkDirectories() {
        return (workDir == null ? null : new WorkDirectory[]{workDir});
    }

    /**
     * Check if this tool contains a work directory whose root file matches
     * a specified file.
     *
     * @param f The root file of the work directory for which to check.
     * @return true if this tool contains a work directory whose root file
     *         is the same as the specified file.
     */
    public boolean containsWorkDirectory(File f) {
        return (workDir != null && workDir.getRoot().equals(f));
    }

    void setWorkDirectory(WorkDirectory workDir)
            throws Interview.Fault, TestSuite.Fault {
        //System.err.println("ET.setWorkDir: testSuite=" + testSuite + " wd.testSuite=" + workDir.getTestSuite());
        init(null, workDir);
        if (context != null)
            context.setWorkDir(workDir);
    }

    // used by quick start wizard
    void setInterviewParameters(InterviewParameters interviewParams) {
        if (interviewParams == null)
            throw new NullPointerException();

        if (testSuite != null || workDir != null)
            throw new IllegalStateException();

        this.interviewParams = interviewParams;
        testSuite = interviewParams.getTestSuite();
        // reset existing (most likely default) context
        context = null;
        getContextManager();
        workDir = interviewParams.getWorkDirectory();
        captureWorkDirInfo();

        if (context != null)
            context.setWorkDir(workDir);

        updateGUI();
    }

    /**
     * Get the current configuration data for this tool.
     *
     * @return the current configuration data for this tool
     */
    public InterviewParameters getInterviewParameters() {
        return interviewParams;
    }

    /**
     * Get the currently loaded filter config, if any.
     *
     * @return the currently loaded filter config, or null if none.
     */
    public FilterConfig getFilterConfig() {
        return filterHandler.getFilterConfig();
    }

    /**
     * Show the Quick Start Wizard.
     *
     * @throws IllegalStateException if the tool already has a test suite
     *                               loaded.
     */
    public void showQuickStartWizard() {
        if (testSuite != null)
            throw new IllegalStateException();

        quickStartWizard = new QuickStartWizard(this, uif);

        if (isShowing())
            quickStartWizard.setVisible(true);
        else {
            addHierarchyListener(new HierarchyListener() {
                public void hierarchyChanged(HierarchyEvent e) {
                    if (isShowing()) {
                        quickStartWizard.setVisible(true);
                        removeHierarchyListener(this);
                    }
                }
            });
        }
    }

    /**
     * Check if the Quick Start Wizard for this ExecTool is showing
     *
     * @return true if and only if the Quick Start Guide for this ExecTool is showing
     */
    boolean isQuickStartWizardShowing() {
        return (quickStartWizard != null && quickStartWizard.isShowing());
    }

//    boolean isWDChooserShowing() {
//        return (wdct != null && wdct.isShowing());
//    }

    // part of ExecModel

    public void showConfigEditor(boolean runTestsWhenDone) {
        if (runTestsWhenDone) {
            ActionListener l = new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    if (interviewParams.isFinishable()) {

                        // configuration has been completed: post a message to remind
                        // user that tests will now be run
                        int option = uif.showOKCancelDialog("exec.configDone");
                        if (option == JOptionPane.OK_OPTION)
                            runTestsHandler.start();
                    }
                }
            };
            configHandler.showConfigEditor(l);
        } else
            configHandler.showConfigEditor();
    }

    public void showTemplateEditor() {
        configHandler.showTemplateEditor();
    }
    /*
public void showTemplateEditor() {
     if (runTestsWhenDone) {
        ActionListener l = new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    if (interviewParams.isFinishable()) {

                        // configuration has been completed: post a message to remind
                        // user that tests will now be run
                        int option = uif.showOKCancelDialog("exec.configDone");
                        if (option == JOptionPane.OK_OPTION)
                            runTestsHandler.start();
                    }
                }
            };
        configHandler.showTemplateEditor(l);
    }
    else
    configHandler.showTemplateEditor();
}
    */

    /**
     * Run the tests specified by the current configuration for this tool.
     */
    public void runTests() {
        runTestsHandler.start();
    }

    // part of ExecModel
    public void runTests(String[] urls) {
        if (urls == null || urls.length == 0)
            // error dialog?
            return;

        try {
            runTestsHandler.executeImmediate(urls);
        }
        catch (Interview.Fault f) {
            f.printStackTrace();
        }
        catch (TestSuite.Fault f) {
            f.printStackTrace();
        }
    }


    /**
     * Save the primary state for this tool, for later restoration.
     *
     * @param m The map in which to save the data
     */
    public void save(Map m) {
        // save window info (super.save(prefix, d)?)
        // save test suite
        if (testSuite != null && testSuite.getRoot() != null)
            m.put("testSuite", testSuite.getRoot().getPath());
        // save work directory
        if (workDir != null)
            m.put("workDir", workDir.getPath());
        // save name of interview file
        if (interviewParams != null && interviewParams.getFile() != null)
            m.put("config", interviewParams.getFile().getPath());
        // save report input options
        reportHandler.save(m);

        testTreePanel.save(m);

        // NOTE: filters are saved to prefs., not the desktop state
        filterHandler.save(m);

        toolBarManager.save(m);
    }


    /**
     * Create an ExecTool from its saved state
     */
    ExecTool(ExecToolManager mgr, Map m) throws Interview.Fault {
        this(mgr, getInterview(m), m);
        reportHandler.restore(m);
    }

    /**
     * Create an ExecTool initialized to the contents of an interview object and
     * previous state information.
     *
     * @param mgr             the manager for this tool
     * @param interviewParams The interview object containing the test suite and
     *                        work directory to be displayed.
     * @param map             Object with saved state, may be null.
     */
    private ExecTool(ExecToolManager mgr, InterviewParameters interviewParams, Map map) {
        super(mgr, "exec");

        manager = mgr;
        this.interviewParams = interviewParams;

        if (interviewParams != null) {
            testSuite = interviewParams.getTestSuite();
            workDir = interviewParams.getWorkDirectory();
            captureWorkDirInfo();

            String testSuiteName = testSuite.getName();
            if (testSuiteName != null)
                setShortTitle(testSuiteName);
        }

        configHandler = new ConfigHandler(this, this, uif);
        runTestsHandler = new RunTestsHandler(this, this, configHandler, uif);
        Harness harness = runTestsHandler.getHarness();
        reportHandler = new ReportHandler(this, this, harness, uif);
        filterHandler = new ET_FilterHandler(this, this, runTestsHandler.getHarness(),
                uif, map);
        if (this.interviewParams != null) {
            this.interviewParams.setTemplateManger(this.getContextManager());
        }

        if (templatePath != null) {
            int sep = templatePath.lastIndexOf(File.separator);
            configHandler.setTemplateName(templatePath.substring(sep < 0 ? 0 : sep + 1));
        }

        configHandler.setCustomRenderersMap(getContextManager().getCustomRenderersMap());

        initGUI(map);
    }

    private static InterviewParameters getInterview(Map m) throws Interview.Fault {
        String tsp = (String) (m.get("testSuite"));
        String wdp = (String) (m.get("workDir"));
        String cfp = (String) (m.get("config"));

        if (isEmpty(tsp) && isEmpty(wdp) && isEmpty(cfp))
            return null;

        return InterviewParameters.open(tsp, wdp, cfp);
    }

    private void init(TestSuite newTestSuite, WorkDirectory newWorkDir)
            throws TestSuite.Fault, Interview.Fault {
        context = null;
        // if called with both args null, that's a user error
        if (newTestSuite == null && newWorkDir == null)
            throw new IllegalArgumentException();

        // if the new test suite is null, default it to the test suite
        // of the new work directory
        if (newTestSuite == null)
            newTestSuite = newWorkDir.getTestSuite();

        // assert(testSuite == null || testSuite == newTestSuite);
        if (testSuite != null && testSuite != newTestSuite)
            throw new IllegalStateException();

        // assert(workDir == null || workDir == newWorkDir);
        if (workDir != null && workDir != newWorkDir) {
            // throw new IllegalStateException();
            manager.showWorkDirectory(newWorkDir);
            return;
        }

        // assert(interviewParams == null || interviewParams.getTestSuite() == newTestSuite);
        if (interviewParams != null && interviewParams.getTestSuite() != newTestSuite)
            throw new IllegalStateException();

        // invariants OK; update state

        if (workDir == null && newWorkDir != null) {
        }

        testSuite = newTestSuite;
        workDir = newWorkDir;
        captureWorkDirInfo();

        if (interviewParams == null) {
            interviewParams = testSuite.createInterview();
        }

        if (templatePath != null) {
            try {
                testSuite.loadInterviewFromTemplate(new File(templatePath), interviewParams);
                interviewParams.setTemplatePath(templatePath);
            }
            catch (TestSuite.Fault f) {
                // also send to logging
                uif.showError("exec.cantLoadTmpl", f.getMessage());
            }
            catch (IOException e) {
                // also send to logging
                uif.showError("exec.cantLoadTmpl", e.getMessage());
            }
        }

        interviewParams.setTemplateManger(this.getContextManager());

        if (templatePath != null) {
            interviewParams.setTemplatePath(templatePath);
        }

        if (workDir != null) {
            if (workDirReset && newWorkDir.getTestResultTable() == null) {
                // reset TestResultTable for new WorkDirectory
                newWorkDir.setTestResultTable(new TestResultTable(newWorkDir));
                workDirReset = false;
            }

            // reuse test result table if a dummy one has been created
            TestResultTable trt = testTreePanel.getTestResultTable();
            if (trt != null) {
                try {
                    workDir.setTestResultTable(trt);
                }
                catch (IllegalStateException e) {
                    // ignore the exception if the work directory already has a TRT set;
                    // in future, it would be good to query the workDir to see if it has
                    // a TRT set
                }
            }

            interviewParams.setWorkDirectory(workDir);

            FileHistory h = FileHistory.getFileHistory(workDir, "configHistory.jtl");
            File latestConfigFile = h.getLatestEntry();

            // validate location of config file if needed
            if (latestConfigFile != null) {
                try {
                    if (!context.getAllowConfigLoadOutsideDefault()) {
                        File defaultConfigLoadPath = ConfigEditor.checkLoadConfigFileDefaults(getContextManager());

                        File dir = new File(latestConfigFile.getAbsolutePath().substring(
                                0, latestConfigFile.getAbsolutePath().
                                lastIndexOf(File.separator)));

                        boolean isMatch = true;

                        if (dir != null && defaultConfigLoadPath != null)
                            try {
                                isMatch = (dir.getCanonicalPath().indexOf
                                        ((defaultConfigLoadPath.getCanonicalPath())) == 0);
                            } catch (IOException ioe) {
                                // use logging subsystem instead when available
                                // Internal error in ExecToolManager: exception thrown:
                                uif.showError("exec.internalError", ioe);
                                return;
                            }

                        if (!isMatch) {
                            resetWorkDirectory();
                            uif.showError("ce.load.notAllowedDir",
                                    defaultConfigLoadPath);
                        } else {
                            interviewParams.load(latestConfigFile);
                        }
                    } else {
                        interviewParams.load(latestConfigFile);
                    }
                }
                catch (IOException e) {
                    uif.showError("exec.cantLoadDefaultConfig",
                            new Object[]{latestConfigFile, e});
                }
            }
        }
        updateGUI();
    }

    /**
     * Initialize the GUI for the tool.
     *
     * @param map Object with saved state.
     */
    private void initGUI(Map map) {

        context = null;

        setShortTitle(uif.getI18NString("exec.shortTitle"));

        configHandler.addConfigEditorAccelerator(this);

        if (menuBar != null)
            throw new IllegalStateException();

        ContextManager cm = getContextManager();
        menuManager = cm.getMenuManager();

        menuBar = new JMenuBar();
        menuBar.add(configHandler.getMenu());
        menuBar.add(runTestsHandler.getMenu());
        menuBar.add(reportHandler.getMenu());

        Action[] viewActions = {
                propertiesAction,
                testSuiteErrorsAction,
                logViewerAction
        };

        JMenu viewMenu = uif.createMenu("exec.view", viewActions);

        viewMenu.insertSeparator(0);
        JMenuItem fmItem = createFilterMenu();

        menuBar.add(viewMenu);

        menuBar.add(uif.createHorizontalGlue("exec.pad"));

        setLayout(new BorderLayout());

        Vector v = new Vector();
        v.addAll(Arrays.asList(configHandler.getToolBarActions()));
        v.add(null);
        v.addAll(Arrays.asList(runTestsHandler.getToolBarActions()));
        v.add(null);
        Action[] toolBarActions = new Action[v.size()];
        v.copyInto(toolBarActions);

        Preferences p = Preferences.access();

        toolBar = uif.createToolBar("exec.toolbar");
        toolBar.setFloatable(false);
        toolBar.setVisible(p.getPreference(TOOLBAR_PREF, "true").equals("true"));
        toolBar.getMargin().left = 10;
        toolBar.getMargin().right = 10;

        JLabel lab = uif.createLabel("exec.filter", false);
        JComponent selector = filterHandler.getFilterSelectionHandler().getFilterSelector();
        //lab.setLabelFor(selector);
        lab.setMaximumSize(lab.getPreferredSize());

        toolBar.add(lab);
        toolBar.addSeparator();
        toolBar.add(selector);
        toolBar.addSeparator();

        // now add all the other buttons
        uif.addToolBarActions(toolBar, toolBarActions);

        ToolBarPanel toolBarPanel = new ToolBarPanel();
        toolBarPanel.add(toolBar);

        toolBarManager = getContextManager().getToolBarManager();
        toolBarManager.setUIFactory(uif);
        toolBarManager.load(map);
        toolBarManager.setPanel(toolBarPanel);

        //add(toolBar, BorderLayout.NORTH);
        add(toolBarPanel, BorderLayout.NORTH);

        viewMenu.insert(toolBarManager.getToolbarMenu(), 0);
        viewMenu.insert(fmItem, 0);
        viewMenu.insert(createViewConfigMenu(), 0);

        prefsObserver = (new Preferences.Observer() {
            public void updated(String name, String newValue) {
                if (name.equals(TOOLBAR_PREF)) {
                    boolean visible = "true".equals(newValue);
                    toolBar.setVisible(visible);
                    toolBarManager.setVisibleFromPrefs(visible);
                }
            }
        });
        p.addObserver(TOOLBAR_PREF, prefsObserver);

        Harness harness = runTestsHandler.getHarness();
        testTreePanel = new TestTreePanel(uif, harness,
                this, filterHandler.getFilterSelectionHandler(),
                this, map);

        if (shouldPauseTree)
            testTreePanel.getTreePanelModel().pauseWork();

        runTestsHandler.setTreePanelModel(testTreePanel.getTreePanelModel());
        add(testTreePanel, BorderLayout.CENTER);

        // this panel contains two full-width panels
        JPanel statusStrips = uif.createPanel("exec.strips", false);
        MessageStrip messageStrip = runTestsHandler.getMessageStrip();
        statusStrips.setLayout(new BorderLayout());
        statusStrips.add(makeStatusStrip(), BorderLayout.NORTH);
        statusStrips.add(messageStrip, BorderLayout.SOUTH);

        add(statusStrips, BorderLayout.SOUTH);

        TU_ViewManager.register(this, uif, configHandler);
        updateGUI();
        if (getInterviewParameters() != null) {
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    getInterviewParameters().checkForUpdates();
                }
            });
        }
    }

    /*
    private void initTestSuitePrefs() {
        String cls = null;
        if (testSuite != null)
            cls = testSuite.getTestSuiteInfo("prefsPane");

        try {
            if (cls != null) {
                Preferences.Pane pane = (Preferences.Pane)((Class.forName(cls, true,
                                    testSuite.getClassLoader())).newInstance());
                Preferences prefs = Preferences.access();
            }
        }
        catch (ClassNotFoundException e) {
            e.printStackTrace();        // XXX rm
            // should print log entry
        }
        catch (InstantiationException e) {
            e.printStackTrace();        // XXX rm
            // should print log entry
        }
        catch (IllegalAccessException e) {
            e.printStackTrace();        // XXX rm
            // should print log entry
        }

    }
    */

    /**
     * Grab info from the currently set workDir.
     * This is mainly used to capture template information.
     */
    private void captureWorkDirInfo() {
        if (workDir == null)
            templatePath = null;
        else
            templatePath = TemplateUtilities.getTemplatePath(workDir);
    }

    private JComponent makeStatusStrip() {
        Vector vStrips = new Vector();

        vStrips.addAll(makeWDField());
        vStrips.addAll(configHandler.getStatusStrip());

        JPanel statusStrips = uif.createPanel("exec.status", false);
        statusStrips.setLayout(new GridBagLayout());

        GridBagConstraints gbc;

        for (int i = 0; i < vStrips.size() - 1; i += 2) {
            gbc = new GridBagConstraints();
            gbc.weightx = 0.0;


            statusStrips.add((JLabel) vStrips.elementAt(i), gbc);
            gbc = new GridBagConstraints();
            gbc.fill = GridBagConstraints.HORIZONTAL;
            gbc.anchor = GridBagConstraints.WEST;
            gbc.weightx = 1.0;

            JTextField tf = (JTextField) vStrips.elementAt(i + 1);
            tf.setHorizontalAlignment(JTextField.LEFT);
            statusStrips.add(tf, gbc);


            gbc = new GridBagConstraints();
            gbc.fill = GridBagConstraints.BOTH;
            gbc.weightx = 0.0;

            final JTextField ellipsis = new JTextField("...");
            ellipsis.setMinimumSize(ellipsis.getPreferredSize());
            ellipsis.setMaximumSize(ellipsis.getPreferredSize());
            ellipsis.setFont(tf.getFont());
            ellipsis.setBackground(tf.getBackground());
            ellipsis.setDisabledTextColor(tf.getForeground());
            ellipsis.setBorder(BorderFactory.createEmptyBorder());
            ellipsis.setEnabled(false);
            ellipsis.setEditable(false);

            statusStrips.add(ellipsis, gbc);


            tf.addComponentListener(new ComponentAdapter() {
                public void componentResized(ComponentEvent e) {
                    process(e, ellipsis);
                }

                public void componentMoved(ComponentEvent e) {
                    process(e, ellipsis);
                }

            });

            tf.addFocusListener(new FocusListener() {
                public void focusGained(FocusEvent e) {
                    process(e, ellipsis);
                }

                public void focusLost(FocusEvent e) {
                    process(e, ellipsis);
                }
            });
        }

        return statusStrips;
    }

    private void process(ComponentEvent e, JTextField ellipsis) {
        JTextField c = (JTextField) e.getComponent();
        if (c.getSize().getWidth() < c.getPreferredSize().getWidth() && !c.isFocusOwner()) {
            ellipsis.setVisible(true);
        } else {
            ellipsis.setVisible(false);
        }

        c.getParent().validate();
    }


    private java.util.List makeWDField() {
        JLabel lab = uif.createLabel("exec.bar.wdname");
        lab.setDisplayedMnemonic(uif.getI18NString("exec.bar.wdname.mne").charAt(0));

        lab.setBackground(MetalLookAndFeel.getMenuBackground());
        lab.setForeground(new Color(102, 102, 102));   // #666666
        Font f = MetalLookAndFeel.getSystemTextFont();
        lab.setFont(new Font("Ariel", Font.BOLD, f.getSize()));
        lab.setHorizontalAlignment(JLabel.RIGHT);


        wdNameField = uif.createOutputField("exec.bar.wdnamef", 0, lab);
        wdNameField.setHorizontalAlignment(JTextField.LEADING);
        wdNameField.setBorder(BorderFactory.createEmptyBorder());
        wdNameField.setBackground(MetalLookAndFeel.getMenuBackground());
        wdNameField.setForeground(new Color(102, 102, 102));   // #666666
        wdNameField.setFont(new Font("Ariel", Font.BOLD, f.getSize()));
        wdNameField.setText(uif.getI18NString("exec.bar.none"));

        return java.util.Arrays.asList(new JComponent[]{lab, wdNameField});
    }

    /**
     * Create the items required to get the filter popup menu.
     *
     * @return The menu item to be shown in the top level view menu.
     */
    private JMenuItem createFilterMenu() {
        return filterHandler.getMenu();
    }


    private JMenuItem createViewConfigMenu() {
        JMenuItem[] items = configHandler.getConfigViewMenuItems();
        if (items != null) {
            JMenu m = uif.createMenu("exec.view.cfg");
            for (int i = 0; i < items.length; i++)
                m.add(items[i]);

            // add custom menu items if needed
            if (menuManager != null) {
                items = menuManager.getMenuItems(JavaTestMenuManager.CONFIG_VIEW);
                if (items != null)
                    // could add a seperator here
                    for (int i = 0; i < items.length; i++)
                        m.add(items[i]);
            }

            return m;
        } else
            return null;
    }


    /**
     * Update the GUI after the test suite and/or work directory have been changed.
     */
    private void updateGUI() {
        configHandler.updateGUI(); // better done with ExecModel.Observer
        runTestsHandler.updateGUI(); // better done with ExecModel.Observer
        reportHandler.updateGUI(); // better done with ExecModel.Observer

        if (interviewParams != null && interviewObserver == null) {
            interviewObserver = new Interview.Observer() {
                public void currentQuestionChanged(Question q) {
                }

                public void pathUpdated() {
                    filterHandler.updateParameters();
                }
            };
            interviewParams.addObserver(interviewObserver);
        }

        // set the title for the tool, based on the following info:
        // - test suite name
        // - test suite path
        // - work dir path
        // all of which may be null.
        String testSuiteName = (testSuite == null ? null : testSuite.getName());
        String workDirPath = (workDir == null ? null : workDir.getRoot().getPath());

        if (wdNameField != null && workDirPath != null) {
            int slash = workDirPath.lastIndexOf(File.separator);
            wdNameField.setText(workDirPath.substring(slash < 0 ? 0 : slash + 1));
        }

        if (testSuite == null)
            setI18NTitle("exec.title.noTS.txt");
        else if (workDirPath == null) {
            if (testSuiteName == null)
                setI18NTitle("exec.title.noWD.txt");
            else {
                setShortTitle(testSuiteName);
                setI18NTitle("exec.title.tsName.txt", testSuiteName);
            }
        } else {
            if (testSuiteName == null)
                if (workDirPath != null) {
                    setI18NTitle("exec.title.wd.txt", workDirPath);
                } else
                    setI18NTitle("exec.title.noTsName.txt");
            else {
                setShortTitle(testSuiteName);
                setI18NTitle("exec.title.tsName_wd.txt", new Object[]{testSuiteName, workDirPath});
            }
        }

        if (testSuite != null) {
            final TestFinder tf = testSuite.getTestFinder();
            tf.setErrorHandler(testFinderErrorHandler);
            testSuiteErrorsAction.setEnabled(tf.getErrorCount() > 0);
            if (workDir != null && workDir.getLogFileName() != null)
                logViewerAction.setEnabled(true);
            else
                logViewerAction.setEnabled(false);
        } else
            logViewerAction.setEnabled(false);

        testTreePanel.setParameters(interviewParams);
        filterHandler.updateParameters();
        testTreePanel.updateGUI();
    }

    private void setTestSuite(TestSuite ts, boolean addToFileHistory)
            throws Interview.Fault, TestSuite.Fault {
        /*
        testSuite = ts;
        interviewParams = testSuite.createInterview();
        workDir = null;
        updateGUI();
        */
        init(ts, null);
        if (addToFileHistory)
            manager.addToFileHistory(ts);
    }

    public void setWorkDir(WorkDirectory wd, boolean addToFileHistory)
            throws Interview.Fault, TestSuite.Fault {
        /*
        // ensure the current test suite is set to that for the work directory
        //System.err.println("ET.setWorkDir: testSuite=" + testSuite + " wd.testSuite=" + wd.getTestSuite());
        if (testSuite != wd.getTestSuite()) {
            testSuite = wd.getTestSuite();
            interviewParams = testSuite.createInterview();
        }

        // ensure the interviewParams's workDir is set
        if (interviewParams.getWorkDirectory() == null) {
            interviewParams.setWorkDirectory(wd);
        }

        workDir = wd;

        updateGUI();
        */
        init(null, wd);

        if (addToFileHistory)
            manager.addToFileHistory(wd);
    }

    // uugh temp public because implements ExecModel, should not implement
    // ExecModel directly
    public void showWorkDirDialog(boolean allowTemplates) {
        ActionListener optionListener = new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                Component c = (Component) (e.getSource());
                JOptionPane op = (JOptionPane) SwingUtilities.getAncestorOfClass(JOptionPane.class, c);
                op.setValue(c); // JOptionPane expects the value to be set to the selected button
                op.setVisible(false);
            }
        };

        JTextArea msg = uif.createMessageArea("exec.wd.need");
        String title = uif.getI18NString("exec.wd.need.title");
        JButton[] options = {
                uif.createButton("exec.wd.open", optionListener),
                uif.createButton("exec.wd.new", optionListener),
                uif.createCancelButton("exec.wd.cancel", optionListener)
        };
        int option = JOptionPane.showOptionDialog(this,
                msg,
                title,
                JOptionPane.YES_NO_CANCEL_OPTION,
                JOptionPane.QUESTION_MESSAGE,
                null,
                options,
                null);
        int mode;

        switch (option) {
            case JOptionPane.YES_OPTION:
                mode = WorkDirChooser.OPEN_FOR_GIVEN_TESTSUITE;
                break;
            case JOptionPane.NO_OPTION:
                mode = WorkDirChooser.NEW;
                break;
            default:
                return;
        }
        WorkDirChooseTool.getTool(this, uif, this, mode, testSuite, allowTemplates).doTool();
    }

    // part of ExecModel
    public void showMessage(ResourceBundle msgs, String key) {
        // why is messageStrip part of the runTestsHandler?
        runTestsHandler.getMessageStrip().showMessage(msgs, key);
    }

    public void printSetup() {
        getDesktop().printSetup();
    }

    public void print(Printable p) {
        getDesktop().print(p);
    }

    /**
     * Show the configuration editor for this tool.
     */
    public void showConfigEditor() {
        configHandler.showConfigEditor();
    }

    /**
     * Show the configuration editor for this tool, notifying an
     * action listener when the editor is closed.
     *
     * @param l the action listener that will be notified when the
     *          configuration editor is closed.
     */
    public void showConfigEditor(ActionListener l) {
        configHandler.showConfigEditor(l);
    }

    /**
     * Clears the current work directory
     */
    public void resetWorkDirectory() throws TestSuite.Fault {
        interviewParams = testSuite.createInterview();

        workDir = interviewParams.getWorkDirectory();
        captureWorkDirInfo();

        workDirReset = true;

        updateGUI();
    }

    /**
     * Closes and removes log handler corresponding to this tool
     */
    public void closeLogger() {
        if (workDir == null)
            return;

        Logger l = testSuite.getNotificationLog(workDir);
        Handler[] hs = l.getHandlers();
        for (int i = 0; i < hs.length; i++)
            if (hs[i] instanceof WorkDirLogHandler)
                if (((WorkDirLogHandler) hs[i]).getPattern().equals(workDir.getLogFileName())) {
                    hs[i].close();
                    l.removeHandler(hs[i]);
                }
    }

    /**
     * The interview may have been externally update, check and save if needed.
     *
     * @since 4.0
     */
    void syncInterview() {
        configHandler.syncInterview();
    }

    public TestResultTable getActiveTestResultTable() {
        if (workDir != null)
            return workDir.getTestResultTable();
        else if (testTreePanel != null)
            return testTreePanel.getTestResultTable();
        else
            return null;
    }

    public ExecToolManager getExecToolManager() {
        return manager;
    }

    TestSuiteChooser getTestSuiteChooser() {
        if (testSuiteChooser == null)
            testSuiteChooser = new TestSuiteChooser();
        return testSuiteChooser;
    }

    void pauseTreeCacheWork() {
        if (testTreePanel != null &&
                testTreePanel.getTreePanelModel() != null)
            testTreePanel.getTreePanelModel().pauseWork();
        else
            shouldPauseTree = true;
    }

    void unpauseTreeCacheWork() {
        if (testTreePanel != null &&
                testTreePanel.getTreePanelModel() != null)
            testTreePanel.getTreePanelModel().unpauseWork();
        else
            shouldPauseTree = false;
    }


    private static boolean isEmpty(String s) {
        return (s == null || s.length() == 0);
    }

    static final String TOOLBAR_PREF = "exec.toolbar";
    static final String FILTER_WARN_PREF = "exec.filterWarn";
    static final String ACTIVE_FILTER = "filter";

    // core
    private ExecToolManager manager;
    private TestSuite testSuite;
    private WorkDirectory workDir;
    private InterviewParameters interviewParams;
    private String templatePath;

    private boolean shouldPauseTree;

    private Interview.Observer interviewObserver;

    // context info
    private ContextManager context;

    // filter stuff
    private FilterConfig fConfig;

    private ConfigHandler configHandler;
    private RunTestsHandler runTestsHandler;
    private ReportHandler reportHandler;
    private ET_FilterHandler filterHandler;

    private ToolBarManager toolBarManager;          // custom toolbars
    private JavaTestMenuManager menuManager;        // custom menus

    private JMenuBar menuBar;
    private JToolBar toolBar;
    private JPopupMenu filterMenu;
    private JCheckBoxMenuItem pauseCheckBox;
    private TestTreePanel testTreePanel;
    private JTextField wdNameField;

    private TestSuiteErrorsDialog testSuiteErrorsDialog;
    private TestSuiteChooser testSuiteChooser;
    private Preferences.Observer prefsObserver;
    private QuickStartWizard quickStartWizard;

    private static final File userDir = new File(System.getProperty("user.dir"));
    private static I18NResourceBundle i18n = I18NResourceBundle.getBundleForClass(ExecTool.class);

    private TestFinderErrorHandler testFinderErrorHandler = new TestFinderErrorHandler();

    private boolean workDirReset = false;

    private Action propertiesAction = new ToolAction(uif, "exec.view.props") {
        public void actionPerformed(ActionEvent e) {
            if (propertiesBrowser == null) {
                propertiesBrowser = new PropertiesBrowser(ExecTool.this, uif);
            }
            propertiesBrowser.showDialog(testSuite, workDir, interviewParams);
        }

        private PropertiesBrowser propertiesBrowser;
    };

    private Action testSuiteErrorsAction = new ToolAction(uif, "exec.view.testSuiteErrors") {
        public void actionPerformed(ActionEvent e) {
            if (testSuiteErrorsDialog == null)
                testSuiteErrorsDialog = new TestSuiteErrorsDialog(ExecTool.this, uif);
            testSuiteErrorsDialog.show(testSuite);
        }
    };

    private Action logViewerAction = new ToolAction(uif, "exec.view.logviewer") {
        public void actionPerformed(ActionEvent e) {

            if (workDir != null)
                openLogViewer();
            else
                // should not happen: menu item is disabled in this case
                testSuite.getNotificationLog(null).info(uif.getI18NString("exec.view.logviewer.noworkdir"));
        }
    };

    private void openLogViewer() {
        new LogViewer(workDir, uif, this);
    }

    void loadInterview(File file) {
        configHandler.loadInterview(file);
    }

    private class TestFinderErrorHandler implements TestFinder.ErrorHandler {
        public void error(final String message) {
            if (!EventQueue.isDispatchThread()) {
                // redispatch method on event thread
                EventQueue.invokeLater(new Runnable() {
                    public void run() {
                        error(message);
                    }
                });
                return;
            }

            // update the log if it is visible
            if (testSuiteErrorsDialog != null && testSuiteErrorsDialog.isShowing())
                testSuiteErrorsDialog.show(testSuite);

            // enable the View>Test Suite Errors action
            testSuiteErrorsAction.setEnabled(true);

            // inform the user
            if (!shownErrorDialog) {
                shownErrorDialog = true;
                uif.showError("exec.testFinderErr", message);
            }
        }

        private boolean shownErrorDialog = false;
    }
}
