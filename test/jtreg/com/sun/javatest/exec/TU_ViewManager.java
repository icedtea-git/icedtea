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

import com.sun.javatest.InterviewParameters;
import com.sun.javatest.InterviewPropagator;
import com.sun.javatest.TestSuite;
import com.sun.javatest.WorkDirectory;
import com.sun.javatest.tool.UIFactory;
import com.sun.javatest.util.I18NResourceBundle;
import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Frame;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JEditorPane;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.JTextArea;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;


class TU_ViewManager implements InterviewPropagator.ViewManager, InterviewPropagator.TestRefresher {

    public void showView(InterviewPropagator prop, InterviewParameters intTerview) {
        ExecTool tool = getExecTool(intTerview);
        if (tool != null)  {
            Object[] vals = (Object[]) store.get(tool);
            Frame aFrame = (Frame) SwingUtilities.getAncestorOfClass(Frame.class, tool);
            TU_dialog tu = new TU_dialog(aFrame, (UIFactory)vals[0], prop, intTerview, (ConfigHandler)vals[1]);
            return;
        }
    }

    private ExecTool getExecTool(InterviewParameters intTerview) {
        Iterator it = store.keySet().iterator();
        while (it.hasNext()) {
            ExecTool tool = (ExecTool) it.next();
            if (tool.getContextManager().getInterview() == intTerview)  {
                return tool;
            }
        }
        return null;
    }

    public void notifyError(String message, InterviewParameters intT) {
        ExecTool tool = getExecTool(intT);
        if (tool != null)  {
            TestSuite ts = tool.getTestSuite();
            if (tool.getWorkDirectory() != null) {
                Logger logger = TU_dialog.makeNotificationLogger(tool.getWorkDirectory());
                if (logger != null) {
                    logger.log(Level.SEVERE, message);
                }
            }
        }
    }

    public void logException(Throwable th, InterviewParameters intT) {
        ExecTool tool = getExecTool(intT);
        if (tool != null)  {
            TestSuite ts = tool.getTestSuite();
            if (tool.getWorkDirectory() != null) {
                Logger logger = TU_dialog.makeLogger(tool.getWorkDirectory());
                if (logger != null) {
                    logger.log(Level.SEVERE, th.getMessage(), th);
                }
            }
        }
    }

    static synchronized void register(ExecTool p,  UIFactory u, ConfigHandler h) {
        store.put(p, new Object[] {u, h} );
        TU_ViewManager tuv = new TU_ViewManager();
        InterviewPropagator.setViewManager(tuv);
        InterviewPropagator.setTestRefresher(tuv);
    }

    static synchronized void dispose(ExecTool tool) {
        store.remove(tool);
    }

    private static HashMap store = new HashMap();


    public void refreshTestTree(InterviewParameters ip) {
        ExecTool tool = getExecTool(ip);
        if (tool != null)  {
            tool.getContextManager().refreshTests();
        }

    }
}

class TU_dialog extends  JDialog {

    TU_dialog(Frame parent,  UIFactory uiF, InterviewPropagator prop, InterviewParameters intV, ConfigHandler h) {
        super(parent, true);
        setTitle(i18n.getString("tu_dialog.title"));
        //super(parent, uif, "tu");
        uif = uiF;
        propagator = prop;
        interview = intV;
        handler =h;
        boolean UItestMode = (propagator == null);
        if (!UItestMode) {
            hasUpdates = propagator.getPropagateMap().hasUpdates();
            hasConflicts = propagator.getPropagateMap().hasConflicts();
            log = makeLogger(intV.getWorkDirectory());
        }
        initGUI();
        pack();
        if (!UItestMode) {
            updateData();
            setLocationRelativeTo(parent);
            setVisible(true);
        }
    }

    protected void initGUI() {

        JPanel body = new JPanel();/*uif.createPanel("tu.body", false);*/

        GridBagConstraints gridBagConstraints;

        temName = new JLabel();
        updTime = new JLabel();
        msgConflict = uif.createTextArea("tu_dialog.text_area");
        tabs = new JTabbedPane();
        tabConflicts = new JPanel();
        tabUpdates = new JPanel();
        msgUpdate = uif.createTextArea("tu_dialog.text_area");


        jEditorPane1 = new JEditorPane();
        jEditorPane2 = new JEditorPane();
        uif.setToolTip(jEditorPane1, "tu_dialog.pane_conflicts");
        uif.setAccessibleName(jEditorPane1, "tu_dialog.pane_conflicts");
        jEditorPane1.setName("tu_dialog.pane_conflicts");
        uif.setToolTip(jEditorPane2, "tu_dialog.pane_update");
        uif.setAccessibleName(jEditorPane2, "tu_dialog.pane_update");
        jEditorPane2.setName("tu_dialog.pane_update");
        jScrollPane1 = new JScrollPane();
        jScrollPane2 = new JScrollPane();

        body.setLayout(new GridBagLayout());

        gridBagConstraints = new GridBagConstraints();
        gridBagConstraints.gridx = 0;
        gridBagConstraints.gridy = 0;
        gridBagConstraints.fill = GridBagConstraints.HORIZONTAL;
        gridBagConstraints.anchor = GridBagConstraints.NORTHWEST;
        gridBagConstraints.insets = new Insets(5, 5, 5, 5);
        body.add(temName, gridBagConstraints);

        gridBagConstraints = new GridBagConstraints();
        gridBagConstraints.gridx = 0;
        gridBagConstraints.gridy = 1;
        gridBagConstraints.fill = GridBagConstraints.HORIZONTAL;
        gridBagConstraints.anchor = GridBagConstraints.NORTHWEST;
        gridBagConstraints.insets = new Insets(5, 5, 5, 5);
        body.add(updTime, gridBagConstraints);

        tabConflicts.setLayout(new BorderLayout());
        jScrollPane1.setViewportView(jEditorPane1);
        tabConflicts.add(jScrollPane1, BorderLayout.CENTER);

        if (hasConflicts)
            tabs.addTab(i18n.getString("tu_dialog.tabConflicts.title"), tabConflicts);

        tabUpdates.setLayout(new BorderLayout());
        jScrollPane2.setViewportView(jEditorPane2);
        tabUpdates.add(jScrollPane2, BorderLayout.CENTER);

        if (hasUpdates)
            tabs.addTab(i18n.getString("tu_dialog.tabUpdates.title"), tabUpdates);

        gridBagConstraints = new GridBagConstraints();
        gridBagConstraints.gridx = 0;
        gridBagConstraints.gridy = 2;
        gridBagConstraints.fill = GridBagConstraints.BOTH;
        gridBagConstraints.anchor = GridBagConstraints.NORTHWEST;
        gridBagConstraints.weightx = 1.0;
        gridBagConstraints.weighty = 1.0;
        gridBagConstraints.insets = new Insets(5, 5, 5, 5);

        tabs.setPreferredSize(new Dimension(600, 350));

        body.add(tabs, gridBagConstraints);

        msgUpdate.setBackground(UIManager.getDefaults().getColor("Panel.background"));
        msgUpdate.setEditable(false);
        msgUpdate.setRows(3);

        if (hasConflicts) {
            //Do you want to change the configuration values to the Template values?
            msgUpdate.setText(i18n.getString("tu_dialog.msgUpdates.text"));
            gridBagConstraints = new GridBagConstraints();
            gridBagConstraints.gridx = 0;
            gridBagConstraints.gridy = 3;
            gridBagConstraints.fill = GridBagConstraints.HORIZONTAL;
            gridBagConstraints.anchor = GridBagConstraints.NORTHWEST;
            gridBagConstraints.insets = new Insets(5, 5, 5, 5);
            body.add(msgUpdate, gridBagConstraints);

        }

        makeButons();

        gridBagConstraints = new GridBagConstraints();
        gridBagConstraints.gridx = 0;
        gridBagConstraints.gridy = 4;
        gridBagConstraints.fill = GridBagConstraints.NONE;
        gridBagConstraints.anchor = GridBagConstraints.SOUTHEAST;
        gridBagConstraints.insets = new Insets(5, 5, 5, 5);
        body.add(buttonPanel, gridBagConstraints);

        setContentPane(body);

    }

    private void makeButons() {

        buttonPanel = new JPanel();

        btnChangeNow  = uif.createButton("tu_dialog.btnChangeNow");
        btnDontChange = uif.createButton("tu_dialog.btnDontChange");
        btnLater      = uif.createButton("tu_dialog.btnRemindLater");
        btnClose      = uif.createButton("tu_dialog.btnClose");

        btnChangeNow.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
               propagator.acceptAll();
               setVisible(false);
            }
        });

        btnDontChange.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
               propagator.rejectAll();
               setVisible(false);
            }
        });

        btnLater.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
               setVisible(false);
            }
        });

        btnClose.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
               setVisible(false);
            }
        });

        buttonPanel.setLayout(new GridLayout(1, 4, 10, 10));
        if (hasConflicts) {
            buttonPanel.add(btnChangeNow);
            buttonPanel.add(btnDontChange);
            buttonPanel.add(btnLater);
        } else {
            buttonPanel.add(btnClose);
        }
    }



    private void updateData() {

        if (interview.getTemplatePath() != null) {
            File template = new File(interview.getTemplatePath());
            temName.setText(i18n.getString("tu_dialog.lblTemName.text", template.getName() ));
            Date dUpd = new Date(template.lastModified());
            String dateStr = new SimpleDateFormat(i18n.getString("tu_dialog.dateFormat")).format(dUpd);
            updTime.setText(i18n.getString("tu_dialog.lblUpdateTime.text", dateStr));
        } else {
            updTime.setText("");
        }
        InterviewPropagator.PropogateMap pm = propagator.getPropagateMap();
        if (pm.hasConflicts() && pm.getConflictReportFile().exists()) {
            try {
                URL url = pm.getConflictReportFile().toURL();
                jEditorPane1.setContentType("text/html");
                jEditorPane1.setPage(url);
            } catch (IOException ex) {
                log.log(Level.SEVERE, "getConflictReportFile().toURL()", ex);
            }
        }
        if (pm.hasUpdates() && pm.getUpdatesReportFile().exists()) {
            try {
                URL url = pm.getUpdatesReportFile().toURL();
                jEditorPane2.setContentType("text/html");
                jEditorPane2.setPage(url);
            } catch (IOException ex) {
                log.log(Level.SEVERE, "getUpdatesReportFile().toURL()", ex);
            }
        }
    }

    static Logger makeLogger(WorkDirectory workDir) {
        Logger res = null;
        try {
            res = workDir.getTestSuite().createLog(workDir, null, logName);
        } catch (TestSuite.DuplicateLogNameFault ex) {
            try {
                res = workDir.getTestSuite().getLog(workDir, logName);
            } catch (TestSuite.NoSuchLogFault exe) {
                exe.printStackTrace();
            }
        }
        return res;
    }

    static Logger makeNotificationLogger(WorkDirectory workDir) {
        return workDir.getTestSuite().getNotificationLog(workDir);
    }


    private Logger log;

    private JButton btnChangeNow;
    private JButton btnDontChange;
    private JButton btnLater;
    private JButton btnClose;

    private JLabel temName;
    private JLabel updTime;
    private JPanel tabConflicts;
    private JPanel tabUpdates;
    private JTabbedPane tabs;
    private JTextArea msgConflict;
    private JTextArea msgUpdate;
    private JPanel buttonPanel;

    private JEditorPane jEditorPane1;
    private JEditorPane jEditorPane2;
    private JScrollPane jScrollPane1;
    private JScrollPane jScrollPane2;

    private InterviewPropagator propagator;
    private InterviewParameters interview;
    private ConfigHandler handler;

    private static I18NResourceBundle i18n = I18NResourceBundle.getBundleForClass(TU_ViewManager.class);
    private static final String logName = i18n.getString("tu.logname");

    private boolean hasUpdates;
    private boolean hasConflicts;

    private UIFactory uif;

}
