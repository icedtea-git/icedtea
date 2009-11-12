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
import java.awt.EventQueue;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.util.HashMap;

import javax.swing.Action;
import javax.swing.DefaultListModel;
import javax.swing.JComponent;
import javax.swing.JList;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.ScrollPaneConstants;

import com.sun.interview.Interview;
import com.sun.javatest.Harness;
import com.sun.javatest.InterviewParameters;
import com.sun.javatest.Parameters;
import com.sun.javatest.TestResult;
import com.sun.javatest.TestSuite;
import com.sun.javatest.WorkDirectory;
import com.sun.javatest.tool.ToolAction;
import com.sun.javatest.tool.UIFactory;
import com.sun.javatest.util.BackupPolicy;
import com.sun.javatest.util.I18NResourceBundle;
import com.sun.javatest.util.StringArray;

class RunTestsHandler
{
    RunTestsHandler(JComponent parent, ExecModel model,
                    ConfigHandler configHandler, UIFactory uif) {
        this.parent = parent;
        this.model = model;
        this.configHandler = configHandler;
        this.uif = uif;

        initActions();
        initHarness();
    }

    void setTreePanelModel(TreePanelModel tpm) {
        this.tpm = tpm;

        if (progMonitor != null)
            progMonitor.setTreePanelModel(tpm);
    }

    JMenu getMenu() {
        JMenu menu = uif.createMenu("rh");
        menu.add(uif.createMenuItem(startAction));

        /* not ready yet, save for 3.1
        pauseCheckBox = uif.createCheckBoxMenuItem("rh", "pause", false);
        pauseCheckBox.setEnabled(false);
        runMenu.add(pauseCheckBox);
        */

        menu.add(uif.createMenuItem(stopAction));

        // custom menu items
        ContextManager cm = model.getContextManager();
        JavaTestMenuManager mm = null;
        if (cm != null) {
            mm = cm.getMenuManager();
            if (mm != null) {
                JMenuItem[] items =
                    mm.getMenuItems(JavaTestMenuManager.RUN_PRIMARY);
                if (items != null)
                    for (int i = 0; i < items.length; i++)
                        menu.add(items[i]);
            }
        }

        menu.addSeparator();
        menu.add(uif.createMenuItem(showProgressAction));

        // custom menu items, at the end
        if (mm != null) {
            JMenuItem[] items =
                mm.getMenuItems(JavaTestMenuManager.RUN_OTHER);
            if (items != null) {
                menu.addSeparator();
                for (int i = 0; i < items.length; i++)
                    menu.add(items[i]);
            }
        }

        return menu;
    }

    Action[] getToolBarActions() {
        return new Action[] {
            startAction,
            stopAction
        };
    }

    Harness getHarness() {
        return harness;
    }

    MessageStrip getMessageStrip() {
        if (messageStrip == null) {
            Monitor[] monitors = new Monitor[2];
            monitors[0] = new ElapsedTimeMonitor(mState, uif);
            monitors[1] = new RunProgressMonitor(mState, uif);

            ActionListener zoom = (new ActionListener() {
                    public void actionPerformed(ActionEvent e) {
                        setProgressMonitorVisible(!isProgressMonitorVisible());
                    }
                });
            messageStrip = new MessageStrip(uif, monitors, mState, zoom);
            messageStrip.setRunningMonitor(monitors[1]);
            messageStrip.setIdleMonitor(monitors[0]);
            harness.addObserver(messageStrip);
        }

        return messageStrip;
    }

    synchronized void dispose() {
        if (harness != null) {
            harness.stop();
            harness = null;
        }

        parent = null;
        model = null;
        configHandler = null;
        tpm = null;

        if (progMonitor != null) {
            progMonitor.dispose();
        }
    }

    // should really be observing ExecModel
    void updateGUI() {
        testSuite = model.getTestSuite();
        workDir = model.getWorkDirectory();
        interviewParams = model.getInterviewParameters();

        boolean testSuiteSet = (testSuite != null);

        // initialize the start button once the test suite is set
        if (testSuiteSet) {
            if (!startAction.isEnabled() && !stopAction.isEnabled())
                startAction.setEnabled(true);
        }
        else
            startAction.setEnabled(false);
    }

    void start() {
        // UPDATE executeImmediate() if you change code here

        startAction.setEnabled(false);

        if (workDir == null) {
            model.showWorkDirDialog(true);
            if (workDir == null) {
                startAction.setEnabled(true);
                return;
            }
        }

        // should check via configHandler if interview is
        // open for editing
        configHandler.ensureInterviewUpToDate();
        interviewParams = model.getInterviewParameters();

        if (!interviewParams.isTemplate() && interviewParams.isFinishable())  {
            startHarness(interviewParams);
        }
        else {
            // configuration is incomplete, possibly not even started:
            // post a message explaining that the wizard is required.
            int option;

            if (interviewParams.isStarted())
                option = uif.showOKCancelDialog("rh.completeConfigure");
            else
                option = uif.showOKCancelDialog("rh.startConfigure");

            if (option != JOptionPane.OK_OPTION) {
                startAction.setEnabled(true);
                return;
            }

            configHandler.showConfigEditor(new ActionListener() {
                    public void actionPerformed(ActionEvent e2) {
                        // configuration has been completed: post a message to remind
                        // user that tests will now be run
                        int option = uif.showOKCancelDialog("rh.configDone");
                        if (option != JOptionPane.OK_OPTION) {
                            startAction.setEnabled(true);
                            return;
                        }
                        startHarness(interviewParams);
                    }
                });
        }
    }

    /**
     * Handles special reconfiguration required for quick-pick test execution.
     * @param tests Null or zero length indicates all tests.  Otherwise,
     *        the strings must be root relative locations in the testsuite.
     */
    void executeImmediate(String[] paths)
        throws Interview.Fault, TestSuite.Fault
    {
        startAction.setEnabled(false);

        if (workDir == null) {
            model.showWorkDirDialog(true);
            if (workDir == null) {
                startAction.setEnabled(true);
                return;
            }
        }

        // should check via configHandler if interview is
        // open for editing
        configHandler.ensureInterviewUpToDate();

        if (interviewParams.isTemplate() || !interviewParams.isFinishable()) {
            // configuration is incomplete, possibly not even started:
            // post a message explaining that the wizard is required.
            int option = uif.showOKCancelDialog("rh.mustConfigure");

            // show the config editor if user clicks ok
            if (option == JOptionPane.OK_OPTION)
                configHandler.showConfigEditor(null);

            // continuing with the operation is not supported
            startAction.setEnabled(true);
            return;
        }

        // if we reach this point, we have a usable interview which
        // we can now alter it
        Object[] items = {interviewParams.getEnv().getName(),
                TestTreePanel.createNodeListString(TestTreePanel.createNodeList(paths))};
        int option = 0;
        if (paths[0].equals(""))
            option = uif.showYesNoDialog("rh.confirmQuickAll",
                        new Object[] {interviewParams.getEnv().getName()});
        else {
            JPanel p = uif.createPanel("rh.confirmPanel", false);
            JTextArea msg = uif.createMessageArea("rh.confirmQuick",
                        new Object[] {interviewParams.getEnv().getName()});
            p.setLayout(new BorderLayout());
            p.add(msg, BorderLayout.NORTH);
            DefaultListModel model = new DefaultListModel();
            for (int i = paths.length; i > 0; i--)
                model.add(model.getSize(), paths[model.getSize()]);

            JList list = uif.createList("rh.confirmList", model);
            p.add(uif.createScrollPane(list,
                ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED,
                ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED), BorderLayout.CENTER);

            option = uif.showCustomYesNoDialog("rh.confirmQuick", p);
        }

        if (option != JOptionPane.YES_OPTION) {
            startAction.setEnabled(true);
            return;
        }

        // copy interview
        InterviewParameters localParams = workDir.getTestSuite().createInterview();
        localParams.setWorkDirectory(interviewParams.getWorkDirectory());
        HashMap data = new HashMap();
        interviewParams.save(data);
        localParams.load(data, false);

        // alter tests in interview
        // (should verify that TestsParameters is mutable)
        Parameters.TestsParameters tps = localParams.getTestsParameters();
        Parameters.MutableTestsParameters mtps = (Parameters.MutableTestsParameters)tps;

        if (paths == null || paths.length == 0 || paths[0].equals("")) {
            mtps.setTestsMode(Parameters.MutableTestsParameters.ALL_TESTS);
        }
        else {
            mtps.setTestsMode(Parameters.MutableTestsParameters.SPECIFIED_TESTS);
            // validate them?
            mtps.setTests(paths);
        }

        // start harness
        startHarness(localParams);
    }

    private void startHarness(InterviewParameters ips) {
        if (getNeedToAutoCheckExcludeList()) {
            configHandler.checkExcludeListUpdate(parent, false);
            // need to update local file time-stamp
        }

        try {
            if (!interviewParams.getWorkDirectory().getTestResultTable().isReady()) {
                // get bundle not done inline to avoid i18n check problems
                I18NResourceBundle i18n = uif.getI18NResourceBundle();
                messageStrip.showMessage(i18n, "rh.waitToStart.txt");
            }

            harness.start(ips);
        }
        catch (Harness.Fault e) {
            uif.showError("rh", e.toString());
        }
    }

    // arguably, this ought to be in InterviewParameters/BasicParameters
    private boolean getNeedToAutoCheckExcludeList() {
        InterviewParameters.ExcludeListParameters elp = interviewParams.getExcludeListParameters();
        if ( !(elp instanceof InterviewParameters.MutableExcludeListParameters))
            return false;

        InterviewParameters.MutableExcludeListParameters melp = (InterviewParameters.MutableExcludeListParameters) elp;
        if (melp.getExcludeMode() != InterviewParameters.MutableExcludeListParameters.LATEST_EXCLUDE_LIST)
            return false;

        // double check there is a URL to download from
        if (interviewParams.getTestSuite().getLatestExcludeList() == null)
            return false;

        if (!melp.isLatestExcludeAutoCheckEnabled())
            return false;

        if (melp.getLatestExcludeAutoCheckMode() == InterviewParameters.MutableExcludeListParameters.CHECK_EVERY_RUN)
            return true;

        File local = workDir.getSystemFile("latest.jtx");
        if (!local.exists())
            return true;

        long localLastModified = local.lastModified();
        long now = System.currentTimeMillis();
        int ageInDays = melp.getLatestExcludeAutoCheckInterval();
        long ageInMillis = ageInDays * 24 * 60 * 60 * 1000;

        return (ageInDays > 0) && (now > (localLastModified + ageInMillis));
    }

    /**
     * Initialize the harness object used by the tool to run tests.
     */
    private void initHarness() {
        harness = new Harness();

        // FIX
        // Set backup parameters; in time this might become more versatile:
        // for example, using preferences
        // Should also probably be static or at least shared between instances
        // Should be moved down into harness land? or at least reused by
        // batch mode and regtest; note: Preferences are currently a GUI feature.
        BackupPolicy backupPolicy = new BackupPolicy() {
            public int getNumBackupsToKeep(File file) {
                return numBackupsToKeep;
            }
            public boolean isBackupRequired(File file) {
                if (ignoreExtns != null) {
                    for (int i = 0; i < ignoreExtns.length; i++) {
                        if (file.getPath().endsWith(ignoreExtns[i]))
                            return false;
                    }
                }
                return true;
            }
            private int numBackupsToKeep = Integer.getInteger("javatest.backup.count", 5).intValue();
            private String[] ignoreExtns = StringArray.split(System.getProperty("javatest.backup.ignore", ".jtr"));
        };

        harness.setBackupPolicy(backupPolicy);
        observer = new HarnessObserver();
        harness.addObserver(observer);

        mState = new MonitorState(harness);
    }

    private boolean isProgressMonitorVisible() {
        if (progMonitor == null || !progMonitor.isVisible())
            return false;
        else
            return true;
    }

    private void setProgressMonitorVisible(boolean state) {
        if (progMonitor == null) {
            progMonitor = new ProgressMonitor(parent, uif, mState);
            progMonitor.setTreePanelModel(tpm);
        }

        progMonitor.setVisible(state);
    }

    private void initActions() {
        showProgressAction = new ToolAction(uif, "rh.progress") {
                public void actionPerformed(ActionEvent e) {
                    setProgressMonitorVisible(true);
                }
            };

        startAction = new ToolAction(uif, "rh.start", true) {
                public void actionPerformed(ActionEvent e) {
                    start();
                }
            };

        stopAction = new ToolAction(uif, "rh.stop", true) {
            {
                // this action is initially disabled, and only enabled when
                // tests are actually being run
                setEnabled(false);
            }

            public void actionPerformed(ActionEvent e) {
                harness.stop();
            }
        };

    }

    private JComponent parent;
    private ExecModel model;
    private ConfigHandler configHandler;
    private UIFactory uif;
    private TreePanelModel tpm;

    private TestSuite testSuite;
    private WorkDirectory workDir;
    private InterviewParameters interviewParams;

    private Action showProgressAction;
    private Action startAction;
    private Action stopAction;

    private Harness harness;
    private HarnessObserver observer;
    private MonitorState mState;

    private MessageStrip messageStrip;
    private ProgressMonitor progMonitor;

    private class HarnessObserver implements Harness.Observer {
        public void startingTestRun(final Parameters params) {
            EventQueue.invokeLater(new Runnable() {
                public void run() {
                    startAction.setEnabled(false);
                    //pauseCheckBox.setEnabled(true);
                    stopAction.setEnabled(true);

                    // get bundle not done inline to avoid i18n check problems
                    I18NResourceBundle i18n = uif.getI18NResourceBundle();
                    messageStrip.showMessage(i18n, "rh.starting.txt");
                }
            });
        }

        public void startingTest(TestResult tr) {
        }

        public void finishedTest(TestResult tr) {
        }

        public void stoppingTestRun() {
        }

        public void finishedTesting() {
        }

        public void finishedTestRun(boolean allOK) {
            EventQueue.invokeLater(new Runnable() {
                public void run() {
                    startAction.setEnabled(true);
                    //pauseCheckBox.setEnabled(false);
                    stopAction.setEnabled(false);
                }
            });
        }

        public void error(final String msg) {
            EventQueue.invokeLater(new Runnable() {
                public void run() {
                    uif.showError("rh.error", msg);
                }
            });
        }
    }
}
