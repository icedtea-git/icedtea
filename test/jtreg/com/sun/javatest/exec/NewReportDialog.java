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
import com.sun.javatest.TestFilter;
import com.sun.javatest.report.CustomReport.ReportConfigPanel;
import com.sun.javatest.report.CustomReport;
import com.sun.javatest.report.Report;
import com.sun.javatest.report.ReportDirChooser;
import com.sun.javatest.Status;
import com.sun.javatest.report.Report.StartGenListener;
import com.sun.javatest.tool.ToolDialog;
import com.sun.javatest.tool.UIFactory;
import com.sun.javatest.util.DynamicArray;

import java.awt.BorderLayout;
import java.awt.CardLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.EventQueue;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Map;
import java.util.Vector;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.ActionMap;
import javax.swing.BorderFactory;
import javax.swing.DefaultListModel;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.event.MouseInputAdapter;
import javax.swing.ComponentInputMap;
import javax.swing.InputMap;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JComponent;
import javax.swing.JDialog;
import javax.swing.JFileChooser;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.KeyStroke;
import javax.swing.ListCellRenderer;
import javax.swing.ListModel;
import javax.swing.ListSelectionModel;
import javax.swing.plaf.metal.MetalLookAndFeel;
import javax.swing.ScrollPaneConstants;
import javax.swing.SwingConstants;
import javax.swing.text.Keymap;
import javax.swing.Timer;
import javax.swing.text.JTextComponent;

class NewReportDialog extends ToolDialog
{

    //----------------------------------------------------------------------------

    private static final String ACTION_MAP_KEY = "listMnemonics";

    interface Observer {
        // if ever made public, should rethink method names and if any helpful
        // parameters are passed

       /**
         * The dialog is done. Notify the observer to keep a snapshot of
         * current state.
         */
        public void update(Map l);

        // starting
        public void writingReport();

        // done
        public void wroteReport();

        // error creating report
        // upgrade to use resource bundle+key as message
        // the given error occured, and the user was informed
        public void errorWriting(String problem);
    }

    NewReportDialog(Component parent, UIFactory uif, FilterConfig f,
                    ReportBrowser reportBrowser, ExecModel model) {
        super(parent, uif, "nrd");

        this.model = model;
        filterHandler = f.createFilterSelectionHandler();
        this.reportBrowser = reportBrowser;
    }

    public void setInterviewParameters(InterviewParameters p) {
         interviewParams = p;
    }

    // ---------------------------------------------------------------------------

    void setLastState(Map h) {
        String rd = (String) (h.get(REPORT_DIR));
        String filter = (String) (h.get(FILTER));

        if (dirBox == null)
            initGUI();

        dirBox.removeAllItems();
        if (rd != null && rd.length() > 0)
           dirBox.addItem(rd);
        else
           dirBox.addItem("");

        if (filter != null && filter.length() > 0)
           filterHandler.setFilter(filter);
    }

    Map getLastState() {
        String rd = (String) (dirBox.getSelectedItem());
        String filter = filterHandler.getActiveFilter().getName();
        Map lastState = new HashMap();

        if (rd != null && rd.length() > 0)
           lastState.put(REPORT_DIR, rd);

        if (filter != null && filter.length() > 0)
           lastState.put(FILTER, filter);

        return lastState;
    }

    void addObserver(Observer o) {
        obs = (Observer[])DynamicArray.append(obs, o);
    }

    void removeObserver(Observer o) {
        obs = (Observer[])DynamicArray.remove(obs, o);
    }

    // Notify the observers that a change has been made.
    private void notifyUpdate(Map s) {
        for (Observer o : obs) {
            o.update(s);
        }
    }

    private void notifyStarting() {
        for (Observer o : obs) {
            o.writingReport();
        }
    }

    private void notifyDone() {
        for (Observer o : obs) {
            o.wroteReport();
        }
    }

    private void notifyError(String problem) {
        for (Observer o : obs) {
            o.wroteReport();
        }
    }

    private void notifyErrorWriting(String problem) {
        for (Observer o : obs) {
            o.errorWriting(problem);
        }
    }

    // ---------------------------------------------------------------------------

    protected void initGUI() {
        setI18NTitle("nrd.title");

        JPanel body = new JPanel() {
                public Dimension getPreferredSize() {
                    Dimension d = super.getPreferredSize();
                    int dpi = uif.getDotsPerInch();
                    return new Dimension(Math.max(d.width, 5 * dpi), d.height);
                }
            };
        body.setName("nrd.body");
        body.setFocusable(false);
        body.setLayout(new GridBagLayout());
        body.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));

        int dpi = uif.getDotsPerInch();

        GridBagConstraints lc = new GridBagConstraints();
        lc.anchor = GridBagConstraints.EAST;
        lc.insets.right = 10;
        lc.insets.bottom = 11;
        lc.weighty = 0.0;
        lc.weightx = 0.0;
        lc.fill = GridBagConstraints.HORIZONTAL;
        lc.gridwidth = 1;

        // first row, directory name
        JLabel dirLabel = uif.createLabel("nrd.dir", true);
        body.add(dirLabel, lc);

        dirBox = uif.createChoice("nrd.dir.choice", true, dirLabel);
        dirBox.setMaximumRowCount(1);
        lc.weightx = 3.0;
        body.add(dirBox, lc);

        browseBtn = uif.createButton("nrd.browse", listener);
        browseBtn.setBorder(BorderFactory.createCompoundBorder(
            BorderFactory.createEtchedBorder(),
            BorderFactory.createEmptyBorder(0,3,0,3)));
        lc.gridwidth = GridBagConstraints.REMAINDER;
        lc.insets.left = 11;
        lc.weightx = 0.0;
        body.add(browseBtn, lc);

        // second row, filter
        lc.insets.left = 0;
        lc.gridwidth = 1;

        // add filter selector
        if (filterHandler != null) {
            JLabel filterLabel = uif.createLabel("nrd.filter", true);
            body.add(filterLabel, lc);

            JComponent selector = filterHandler.getFilterSelector();
            filterLabel.setLabelFor(selector);
            lc.gridwidth = GridBagConstraints.REMAINDER;
            lc.weightx = 4.0;
            lc.anchor = GridBagConstraints.WEST;
            lc.fill = GridBagConstraints.NONE;
            body.add(selector, lc);
        }

        // third row, the tabs
        lc.anchor = GridBagConstraints.NORTHWEST;
        lc.gridwidth = GridBagConstraints.REMAINDER;
        lc.fill = GridBagConstraints.BOTH;
        lc.insets.bottom = 12;
        lc.weighty = 4.0;
        lc.weighty = 1.0;

//------------------------------------------------------------------------

        JPanel middle = uif.createPanel("nrd.middle", false);
        middle.setLayout(new BorderLayout());
        middle.setBorder(BorderFactory.createCompoundBorder(
                        uif.createTitledBorder("nrd.middle"),
                        BorderFactory.createEmptyBorder(12,12,12,12)));

        panes = new JComponent[] {
            createHtmlBlock(),
            createFilesBlock()
        };

        tabs = uif.createTabbedPane("nrd.tabs", panes);
        tabs.setTabPlacement(SwingConstants.TOP);
        tabs.setBorder(BorderFactory.createEmptyBorder());

        listModel = new DefaultListModel();

        // populate list and card panel
        final CardLayout cards = new CardLayout();
        final JPanel p = uif.createPanel("nrd.typecards", cards, false);

        cbHtml = uif.createCheckBox("nrd.type.html", true);
        //cbHtml.addActionListener(cbListener);
        listModel.addElement(cbHtml);
        p.add("nrd.type.html", tabs);

        cbPlain = uif.createCheckBox("nrd.type.pt", true);
        listModel.addElement(cbPlain);
        p.add("nrd.type.pt", uif.createPanel("nrd.blank", false));

        cbXml = uif.createCheckBox("nrd.type.xml", false);
        listModel.addElement(cbXml);
        p.add("nrd.type.xml", uif.createPanel("nrd.blank", false));

        getCustomReports(p);

        list = uif.createList("nrd.typel", listModel);
        list.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        SelectListener sl = new SelectListener(list, p, cards);
        list.addMouseListener(sl);
        list.addKeyListener(sl);
        list.addListSelectionListener(sl);
        list.setCellRenderer(new CheckBoxListCellRenderer());
        list.setBorder(BorderFactory.createCompoundBorder(
            BorderFactory.createEtchedBorder(),
            BorderFactory.createEmptyBorder(0,3,0,3)));

        infoArea = uif.createMessageArea("nrd.info");
        infoArea.setRows(3);
        Font f = infoArea.getFont();
        f = f.deriveFont(Font.PLAIN, f.getSize2D()-1);
        infoArea.setFont(f);

        // create container for right side (next to list)
        JPanel right = uif.createPanel("nrd.rptright", false);
        right.setLayout(new BorderLayout());
        right.add(infoArea, BorderLayout.PAGE_START);
        right.add(p, BorderLayout.CENTER);
        middle.add(right, BorderLayout.CENTER);

        middle.add(list, BorderLayout.WEST);
        body.add(middle, lc);
//-----------------------------------------------------------------

        Action listMnemonics = new AbstractAction() {

                        public void actionPerformed(ActionEvent e) {
                                int ord = Integer.parseInt(e.getActionCommand()) - 1;
                                if (ord == -1) ord = 9;
                                if (ord <= list.getModel().getSize()) {
                                        list.requestFocusInWindow();
                                        list.setSelectedIndex(ord);
                                }
                        }

        };

        ActionMap listActions = new ActionMap();
        InputMap listInput = new ComponentInputMap(list);

        list.setActionMap(listActions);
        list.setInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW, listInput);

        listActions.put(ACTION_MAP_KEY, listMnemonics);

        int itemCount = listModel.size();
        if (itemCount > 9) itemCount = 9;
        for (int i = 1; i <= itemCount; ++i) {
                listInput.put(KeyStroke.getKeyStroke(i + '0', InputEvent.ALT_DOWN_MASK), ACTION_MAP_KEY);
                JCheckBox box = ((JCheckBox)listModel.getElementAt(i - 1));
                box.setMnemonic(i + '0');
                box.setText(Character.toString((char)('0' + i)) + " " + box.getText());
        }

        if (listModel.size() == 10){
                listInput.put(KeyStroke.getKeyStroke('0', InputEvent.ALT_DOWN_MASK), ACTION_MAP_KEY);
                JCheckBox box = ((JCheckBox)listModel.getElementAt(9));
                box.setMnemonic('0');
                box.setText("0 " + box.getText());
        }

//-----------------------------------------------------------------
        JPanel bottom = createBackupOpsPane();
        bottom.setBorder(BorderFactory.createCompoundBorder(
                        uif.createTitledBorder("nrd.backup"),
                        BorderFactory.createEmptyBorder(12,12,12,12)));

        body.add(bottom, lc);
        setBody(body);
//--------------------------------------------------------------------------------------

        okBtn = uif.createButton("nrd.ok", listener);
        cancelBtn = uif.createCancelButton("nrd.cancel", listener);
        setButtons(new JButton[] { okBtn, cancelBtn, }, cancelBtn);

        list.setSelectedIndex(0);
        setState(Report.getSettingsPrefs());
    }

    /**
     * Options for the HTML report.
     */
    private JComponent createHtmlBlock() {
        JPanel p = uif.createPanel("nrd.htmlops", new GridBagLayout(),
                                    false);
        p.setName("htmlops");
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.fill = GridBagConstraints.NONE;
        gbc.anchor = GridBagConstraints.WEST;
        gbc.gridwidth = 2;
        gbc.gridy = 0;

        // info about this block of buttons
        JTextArea info = uif.createMessageArea("nrd.htmlops");
        //info.setBorder(BorderFactory.createLineBorder(Color.black));
        p.add(info, gbc);

        // checkboxes
        // these are given settings here, but setting will be reloaded
        // elsewhere before the user sees them
        JCheckBox cb = uif.createCheckBox("nrd.htmlops.config", true);
        cbConfig = cb;
        cbConfig.addActionListener(cbListener);
        htmlGroup.addElement(cb);
        gbc.gridy = 1;
        p.add(cb, gbc);

        // question log
        gbc.gridwidth = 1;
        gbc.gridy = 2;
        cb = cbQl = uif.createCheckBox("nrd.htmlops.ql", true);
        configGroup.addElement(cb);
        htmlGroup.addElement(cb);
        p.add(uif.createHorizontalStrut(25), gbc);
        p.add(cb, gbc);

        // environment
        gbc.gridy = 3;
        cb = cbEnv = uif.createCheckBox("nrd.htmlops.env", false);
        configGroup.addElement(cb);
        htmlGroup.addElement(cb);
        p.add(uif.createHorizontalStrut(25), gbc);
        p.add(cb, gbc);

        // standard values
        gbc.gridy = 4;
        cb = cbStd = uif.createCheckBox("nrd.htmlops.std", false);
        configGroup.addElement(cb);
        htmlGroup.addElement(cb);
        p.add(uif.createHorizontalStrut(25), gbc);
        p.add(cb, gbc);

        // result summary
        gbc.gridy = 5;
        gbc.gridwidth = 2;
        cb = cbResults = uif.createCheckBox("nrd.htmlops.res", true);
        htmlGroup.addElement(cb);
        p.add(cb, gbc);

        // keyword summary
        cb = cbKws = uif.createCheckBox("nrd.htmlops.kw", true);
        htmlGroup.addElement(cb);
        gbc.gridy = 6;
        p.add(cb, gbc);

        JScrollPane sp = uif.createScrollPane(p,
                ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED,
                ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED);

        sp.setViewportBorder(BorderFactory.createCompoundBorder(
            BorderFactory.createEmptyBorder(10, 5, 5, 5),
            uif.createTitledBorder("nrd.htmlops")
        ));
        sp.addPropertyChangeListener("enabled", new PanelEnableListener(sp));
        return sp;
    }

    /**
     * Options for the HTML report files.
     */
    private JComponent createFilesBlock() {
        JPanel p = uif.createPanel("nrd.htmlf", new GridBagLayout(),
                                    false);
        p.setName("htmlf");
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.fill = GridBagConstraints.BOTH;
        gbc.anchor = GridBagConstraints.WEST;
        //gbc.gridx = 0;
        gbc.gridwidth = 3;
        gbc.gridy = 0;

        // info about this block of buttons
        JTextArea info = uif.createMessageArea("nrd.htmlf");
        //info.setBorder(BorderFactory.createLineBorder(Color.black));
        p.add(info, gbc);

        // settings are given here, but settings will be reloaded
        // elsewhere before the user sees them

        // index.html and/or report.html
        JLabel lab = uif.createLabel("nrd.htmlf.main", false);
        gbc.gridy = 1;
        p.add(lab, gbc);

        gbc.gridwidth = 1;
        gbc.gridy = 2;
        JCheckBox cb = cbHtmlRpt = uif.createCheckBox("nrd.htmlf.rpt", true);
        cbHtmlRpt.addActionListener(cbListener);
        htmlGroup.addElement(cb);
        p.add(uif.createHorizontalStrut(25), gbc);
        gbc.gridwidth = 2;
        p.add(cb, gbc);

        gbc.gridy = 3;
        gbc.gridwidth = 1;
        cb = cbHtmlInd = uif.createCheckBox("nrd.htmlf.idx", false);
        cbHtmlInd.addActionListener(cbListener);
        htmlGroup.addElement(cb);
        p.add(uif.createHorizontalStrut(25), gbc);
        gbc.gridwidth = 2;
        p.add(cb, gbc);

        // pass, fail, error, notrun files
        gbc.gridy = 4;
        gbc.gridwidth = 3;
        lab = uif.createLabel("nrd.htmlf.xtra", false);
        p.add(lab, gbc);

        gbc.gridy = 5;
        cb = uif.createCheckBox("nrd.htmlf.pass", true);
        htmlGroup.addElement(cb);
        cbPass = cb;
        gbc.gridwidth = 1;
        p.add(uif.createHorizontalStrut(25), gbc);
        p.add(cb, gbc);

        cb = cbErr = uif.createCheckBox("nrd.htmlf.err", true);
        htmlGroup.addElement(cb);
        p.add(cb, gbc);

        gbc.gridy = 6;
        cb = cbFail = uif.createCheckBox("nrd.htmlf.fail", true);
        htmlGroup.addElement(cb);
        p.add(uif.createHorizontalStrut(25), gbc);
        p.add(cb, gbc);

        cb = cbNr = uif.createCheckBox("nrd.htmlf.nr", true);
        htmlGroup.addElement(cb);
        p.add(cb, gbc);

        JScrollPane sp = uif.createScrollPane(p,
                ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED,
                ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED);

        sp.setViewportBorder(BorderFactory.createCompoundBorder(
            BorderFactory.createEmptyBorder(10, 5, 5, 5),
            uif.createTitledBorder("nrd.htmlf")
        ));

        sp.addPropertyChangeListener("enabled", new PanelEnableListener(sp));
        return sp;
    }

    private JPanel createBackupOpsPane() {
        JPanel p = uif.createPanel("nrd.backup", new GridBagLayout(),
                                    false);
        p.setName("backup");
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.fill = GridBagConstraints.BOTH;
        gbc.anchor = GridBagConstraints.WEST;
        gbc.gridx = 0;
        gbc.gridwidth = 3;

        // info about this block of buttons
        JTextArea info = uif.createMessageArea("nrd.backup");
        //info.setBorder(BorderFactory.createLineBorder(Color.black));
        p.add(info, gbc);

        // checkboxes
        // these are given settings here, but setting will be reloaded
        // elsewhere before the user sees them
        JCheckBox cb = cbBak = uif.createCheckBox("nrd.backup.bak", true);
        cbBak.addChangeListener(new ChangeListener() {
                public void stateChanged(ChangeEvent e) {
                    if (numBak != null) {
                        numBak.setEnabled(cbBak.isSelected());
                    }
                }
            });
        p.add(cb, gbc);

        gbc.gridwidth = 1;
        gbc.gridy = 2;
        gbc.gridx = GridBagConstraints.RELATIVE;
        p.add(uif.createHorizontalStrut(25), gbc);

        JLabel lab = uif.createLabel("nrd.backup.bak.level", true);
        p.add(lab, gbc);

        // backup levels
        numBak = uif.createInputField("nrd.backup.bak.level", 2, lab);
        gbc.insets.left = 12;
        numBak.setText("1");

        Action action = new AbstractAction() {
            public void actionPerformed(ActionEvent e) {
                    JTextField src = (JTextField) (e.getSource());
                    String cmd = e.getActionCommand();
                    char ch = cmd.charAt(0);
                    int savedDot = src.getCaret().getDot();
                    int savedMark = src.getCaret().getMark();
                    String savedText = src.getText();

                    // reject all non-numeric chars
                    if (Character.isDigit(ch) && ch != '0') {
                        Action delegate = new javax.swing.text.DefaultEditorKit.InsertContentAction();
                        delegate.actionPerformed(e);

                        String newText = src.getText();
                        // reject new input if it leaves an invalid value,
                        // i.e. more than one digit (>9)
                        if (newText.length() > 1) {
                            src.setText(savedText);
                        }
                    }
                }
            };
        Keymap keymap = numBak.addKeymap("intField", numBak.getKeymap());
        keymap.setDefaultAction(action);
        numBak.setKeymap(keymap);

        p.add(numBak, gbc);

        p.setMaximumSize(p.getPreferredSize());
        return p;
    }


    /**
     * @return number of custom reports added
     */
    private int getCustomReports(JPanel p) {
        int result = 0;

        ContextManager cm = model.getContextManager();
        if (cm == null)
            return 0;

        customReports = cm.getCustomReports();
        if (customReports == null || customReports.length == 0) {
            customReports = null;
            return 0;
        }

        customBoxes = new HashMap();

        for (int i = 0; i < customReports.length; i++) {
            JCheckBox cb = new JCheckBox(customReports[i].getName());
            cb.setName(customReports[i].getReportId());
            listModel.addElement(cb);
            customBoxes.put(cb, customReports[i]);

            ReportConfigPanel[] ops = customReports[i].getOptionPanes();
            if (ops == null || ops.length == 0) {
                // no config panels, use blank
                p.add(customReports[i].getReportId(),
                        uif.createPanel("nrd.blank", false));
            }
            else {
                // tabbed pane for all supplied panels
                JTabbedPane tp = uif.createTabbedPane("nrd.custom.tabs");
                for (int j = 0; j < ops.length; j++)
                    tp.addTab(ops[j].getPanelName(), ops[j]);

                p.add(customReports[i].getReportId(), tp);
            }
            result++;
        }   // for

        return result;
    }

    private void showReportChooserDialog() {
        ReportDirChooser rdc = getReportDirChooser();
        rdc.setMode(ReportDirChooser.NEW);
        int option = rdc.showDialog(dirBox);
        if (option != JFileChooser.APPROVE_OPTION)
            return;

        dirBox.setSelectedItem(rdc.getSelectedFile().getAbsolutePath());
    }

    private ReportDirChooser getReportDirChooser() {
        if (reportDirChooser == null)
            reportDirChooser = new ReportDirChooser();
        return reportDirChooser;
    }

    void showReportBrowser(File f) {
        if(f.equals(new File(reportDir, Report.INDEX_FILE_NAME))) {
            reportBrowser.show(f);
        }
        else {
            String[] names = Report.getHtmlReportFilenames();
            Vector possible = new Vector();

            for (int i = 0; i < names.length; i++) {
                File rpt = new File(reportDir, names[i]);
                if (rpt.exists() && rpt.canRead())
                    possible.add(rpt);
                else
                    rpt = null;
            }

            if (possible.size() > 0) {
                // show the most recently dated file
                File rpt = null;
                File newestF = null;
                long newestTime = 0;

                for (int i = 0; i < possible.size(); i++) {
                    File file = (File)(possible.elementAt(i));
                    if (file.lastModified() > newestTime) {
                        newestF = file;
                        newestTime = file.lastModified();
                    }
                }       // for
                reportBrowser.show(newestF);
            }
            else
                reportBrowser.show(reportDir);
        }
    }

    /**
     * Normally called by the background thread.
     */
    private void runReport(Report.Settings settings) throws IOException {
        if (interviewParams != null) {
            Report r = new Report();

            // if no work directory selected, throw exception.
            if (interviewParams.getWorkDirectory() == null) {
                throw new IOException(uif.getI18NString("nrd.noWorkDir.err"));
            }
            r.addStartGenListener(new ReportGenListener());
            r.writeReports(settings, reportDir, getActiveCustomReports());

        }
        else {
            throw new IOException(uif.getI18NString("nrd.nullParams.err"));
        }
    }

    private ArrayList getActiveCustomReports() {

        ArrayList customReps = new ArrayList();
        if (customBoxes != null && customBoxes.size() > 0) {
            Iterator it = customBoxes.keySet().iterator();
            while (it.hasNext()) {
                JCheckBox box = (JCheckBox)(it.next());
                if (box.isSelected()) {
                    customReps.add(customBoxes.get(box));
                }
            }
        }
        return customReps;
    }

    private boolean isEmptyDirectory(File f) {
        return (f.isDirectory() && f.list().length == 0);
    }

    private void updateCheckboxStates() {
        // config buttons done before html buttons to ensure that
        // config gets disabled if HTML reporting is off

        if (!cbHtml.isSelected()) {
            for (int i = 0; i < htmlGroup.size(); i++)
                ((JCheckBox)(htmlGroup.elementAt(i))).setEnabled(cbHtml.isSelected());
        }
        else {
            for (int i = 0; i < htmlGroup.size(); i++)
                ((JCheckBox)(htmlGroup.elementAt(i))).setEnabled(cbHtml.isSelected());
            for (int i = 0; i < configGroup.size(); i++)
                ((JCheckBox)(configGroup.elementAt(i))).setEnabled(cbConfig.isSelected());
        }
    }

    private Report.Settings captureState() {
        Report.Settings snap = new Report.Settings(interviewParams);

        snap.setEnableHtmlReport(cbHtml.isSelected());
        snap.setEnableXmlReport(cbXml.isSelected());
        snap.setEnablePlainReport(cbPlain.isSelected());

        snap.setShowConfigSection(cbConfig.isSelected());
        snap.setShowEnvLog(cbEnv.isSelected());
        snap.setShowQuestionLog(cbQl.isSelected());
        snap.setShowStdValues(cbStd.isSelected());
        snap.setShowResults(cbResults.isSelected());
        snap.setShowKeywordSummary(cbKws.isSelected());

        snap.setHtmlMainReport(cbHtmlRpt.isSelected(), cbHtmlInd.isSelected());
        snap.setEnableHtmlStateFile(Status.PASSED, cbPass.isSelected());
        snap.setEnableHtmlStateFile(Status.FAILED, cbFail.isSelected());
        snap.setEnableHtmlStateFile(Status.ERROR, cbErr.isSelected());
        snap.setEnableHtmlStateFile(Status.NOT_RUN, cbNr.isSelected());

        snap.setEnableBackups(cbBak.isSelected());

        try {
            int i = Integer.parseInt(numBak.getText());
            snap.setBackupLevels(i);
        }
        catch (NumberFormatException e) {
        }

        TestFilter filter = filterHandler.getActiveFilter();

        // see Report.Settings for info on what this is:
        if (filter instanceof ParameterFilter)
            snap.setAllowInitFilesOptimize(true);
        else
            snap.setAllowInitFilesOptimize(false);

        snap.setFilter(filter);

        return snap;
    }

    private void setState(Report.Settings snap) {
        if (snap == null)
            return;

        lastSettings = snap;


        cbHtml.setSelected(snap.isHtmlEnabled());
        cbXml.setSelected(snap.isXmlEnabled());
        cbPlain.setSelected(snap.isPlainEnabled());

        // ---
        boolean section = snap.isConfigSectionEnabled();
        cbConfig.setSelected(section);

        cbEnv.setSelected(snap.isEnvEnabled());
        cbQl.setSelected(snap.isQuestionLogEnabled());
        cbStd.setSelected(snap.isStdEnabled());

        cbEnv.setEnabled(section);
        cbQl.setEnabled(section);
        cbStd.setEnabled(section);

        // ---
        cbResults.setSelected(snap.isResultsEnabled());
        cbKws.setSelected(snap.isKeywordSummaryEnabled());

        cbHtmlRpt.setSelected(snap.isReportHtmlEnabled());
        cbHtmlInd.setSelected(snap.isIndexHtmlEnabled());
        cbPass.setSelected(snap.isStateFileEnabled(Status.PASSED));
        cbFail.setSelected(snap.isStateFileEnabled(Status.FAILED));
        cbErr.setSelected(snap.isStateFileEnabled(Status.ERROR));
        cbNr.setSelected(snap.isStateFileEnabled(Status.NOT_RUN));

        cbBak.setSelected(snap.isBackupsEnabled());
        numBak.setText(Integer.toString(snap.getBackupLevel()));

        // a null value will be ignored
        filterHandler.setFilter(snap.getTestFilter());
    }

    private ActionListener cbListener = new ActionListener() {
        public void actionPerformed(ActionEvent e) {
            Object src = e.getSource();

            if (src == cbHtml || src == cbConfig)
                updateCheckboxStates();
            else if (src == cbBak) {
                numBak.setEnabled(cbBak.isSelected());
            }
        }
    };

    private ActionListener listener = new ActionListener() {
        public void actionPerformed(ActionEvent e) {
            Object src = e.getSource();

            if (src == browseBtn) {
                showReportChooserDialog();
            }
            else if (src == cancelBtn) {
                // revert settings
                // hide dialog
                setState(lastSettings);
                setVisible(false);
            }
            else if (src == okBtn) {
                String s = (String) dirBox.getSelectedItem();

                // check null input
                if (s == null || s.length() == 0) {
                    uif.showError("nrd.emptyInput");
                    return;
                }

                // check for empty input
                if (cbBak.isSelected() && numBak.getText().equals("")) {
                    uif.showError("nrd.emptyBak");
                    // select the last tab (assumes this has the backup settings)
                    tabs.setSelectedIndex(tabs.getTabCount()-1);
                    return;
                }

                // validate custom reports
                Iterator it = getActiveCustomReports().iterator();
                while (it.hasNext()) {
                    CustomReport cr = (CustomReport) it.next();
                    String error = cr.validateOptions();
                    if (error != null) {
                        for (int i = 0; i < listModel.getSize(); i++ ) {
                            JCheckBox cb = (JCheckBox)listModel.elementAt(i);
                            if (cb.getName().equals(cr.getReportId())) {
                                list.setSelectedIndex(i);
                            }
                        }
                        uif.showError("nrd.optionsErr", new Object[] {error} );
                        return;
                    }
                }

                reportDir = new File(s);

                try {
                    // check if dir needs to be created.
                    if (!reportDir.isDirectory()) {
                        reportDir.mkdirs();
                    }
                    else {
                            // check if dir is a report dir
                        if (!Report.isReportDirectory(reportDir) &&
                            !isEmptyDirectory(reportDir)) {
                            uif.showError("nrd.cantUse", reportDir);
                            return;
                        }
                    }

                    // XXX save settings in prefs

                    setVisible(false);
                    lastSettings = captureState();
                    notifyStarting();
                    doBgReport(lastSettings);
                }
                catch (SecurityException se) {
                    uif.showError("nrd.cantCreate", se.getMessage());
                }
            }
        }

        // does work on background thread
        private void doBgReport(final Report.Settings snap) {
            /*final JDialog*/ waitDialog = uif.createWaitDialog("nrd.wait", parent);

            final Stopper stopper = new Stopper();
            final Thread worker = new Thread() {
                public void run() {
                    try {
                        runReport(snap);
                        // go away to switch back to GUI thread
                        finishReport(waitDialog, snap, stopper);
                    }
                    catch (CustomReport.ReportException ie) {
                        showError("nrd.custom", ie.getMessage(), waitDialog);
                        notifyError(ie.getMessage());
                    }
                    catch (IOException ie) {
                        showError("nrd.cantWrite", ie.getMessage(), waitDialog);
                        notifyError(ie.getMessage());
                        // should reshow dialog
                    }
                    catch (SecurityException se) {
                        showError("nrd.cantCreate", se.getMessage(), waitDialog);
                        notifyError(se.getMessage());
                        // should reshow dialog
                    }
                    catch (RuntimeException re) {
                        showError("nrd.errorWriting", re.getMessage(), waitDialog);
                        notifyErrorWriting(re.getMessage());
                    }
                }   // run()
            };  // thread

            // this code is copied from ReportTool....
            GridBagConstraints gbc = new GridBagConstraints();
            gbc.fill = GridBagConstraints.NONE;
            gbc.anchor = GridBagConstraints.CENTER;
            gbc.insets.bottom = 10;
            gbc.insets.top = 10;
            gbc.gridy = 2;
            gbc.gridx = 0;
            JButton cancelBtn = uif.createButton("nrd.cancel");
            waitDialog.getContentPane().add(cancelBtn, gbc);
            waitDialog.pack();
            final String cancelling = uif.getI18NString("nrd.cancelling");
            cancelBtn.addActionListener( new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    JButton butt = (JButton) e.getSource();
                    butt.setEnabled(false);
                    Component[] cmp = waitDialog.getContentPane().getComponents();
                    if (worker != null && worker.isAlive()) {
                        stopper.waitWasHidden = true;
                        worker.interrupt();
                    }
                    for ( int i = 0; i < cmp.length; i++) {
                        if("nrd.wait".equals(cmp[i].getName())) {
                            if (cmp[i] instanceof JTextComponent) {
                                ((JTextComponent)cmp[i]).setText(cancelling);
                            }
                            break;
                        }
                    }

                }
            });




            ActionListener al = new ActionListener() {
                public void actionPerformed(ActionEvent evt) {
                    // show dialog if still processing
                    if (worker.isAlive()) {
                        waitDialog.show();
                    }
                }
            };

            // show wait dialog if operation is still running after
            // WAIT_DIALOG_DELAY
            Timer timer = new Timer(WAIT_DIALOG_DELAY, al);
            timer.setRepeats(false);
            timer.start();

            // do it!
            worker.start();
        }

        private void finishReport(final JDialog waitDialog,
                                  final Report.Settings snap, final Stopper stopper) {
            // done generating report, switch back to GUI thread
            EventQueue.invokeLater(new Runnable() {
                    public void run() {
                        waitDialog.hide();
                        if(!stopper.waitWasHidden) {
                            int r = uif.showYesNoDialog("nrd.showReport");

                            switch (r) {
                            case JOptionPane.YES_OPTION:
                                File index = new File(reportDir, Report.INDEX_FILE_NAME);
                                if(index.exists() && index.canRead()) {
                                    showReportBrowser(index);
                                }
                                else {
                                    showReportBrowser(selectFileToShow(snap));
                                }
                                break;
                            case JOptionPane.NO_OPTION:
                                break;
                            default:
                                break;
                            }
                        }

                        notifyDone();
                        notifyUpdate(getLastState());
                    }
                }
            );
        }


        /**
         * @param uiKey Key to use to call <tt>UIFactory.showError()</tt>
         * @param msg The localized error message to show.
         */
        private void showError(final String uiKey, final String msg,
                                final JDialog waitDialog) {
            // switch back to GUI thread
            EventQueue.invokeLater(new Runnable() {
                    public void run() {
                        waitDialog.hide();
                        uif.showError(uiKey, msg);
                    }
                }
            );
        }

        /**
         * Decide which file in the report dir to show to the user when
         * they ask to view a generated 'report'.
         */
        private File selectFileToShow(Report.Settings snap) {
            File result = new File(reportDir, Report.INDEX_FILE_NAME);

            if(!result.exists()) {
                if (snap.isHtmlEnabled()) {
                    // show main report file or file listing
                    result = searchForFile(reportDir, Report.getHtmlReportFilenames());
                    if (result == null)
                        result = reportDir;
                }
                /* not useful right now to see raw xml, could do XSLT later
                else if (snap.isPlainEnabled()) {
                    // show that file
                    result = searchForFile(reportDir, Report.getPlainReportFilenames());
                    if (result == null)
                        result = reportDir;
                }
                else if (snap.isXmlEnabled()) {
                    result = new File(reportDir, "xml");
                }
                */
                else {
                    result = reportDir;
                }
            }

            return result;
        }

        private File searchForFile(File dir, String[] names) {
            for (int i = 0; i < names.length; i++) {
                File f = new File(reportDir, names[i]);
                if (f.exists()) {
                    return f;
                }
            }   // for

            return null;
        }
    };


    //------------------------------end of listener--------------------------

    private static class Stopper {
        boolean waitWasHidden = false;
    }

    private class ReportGenListener implements StartGenListener {
        public void startReportGeneration(Report.Settings s, String reportID) {
            String reportName;
            if(reportID.equals("xml") || reportID.equals("pt") ||
                                                reportID.equals("html")) {
                reportName = uif.getI18NString("nrd.type." + reportID + ".ckb");
            }
            else {
                reportName = reportID;
            }

            String status = uif.getI18NString("nrd.wait.report_gen",
                                                new String[] {reportName});

            Component[] cmp = waitDialog.getContentPane().getComponents();
            for ( int i = 0; i < cmp.length; i++) {
                if("nrd.wait".equals(cmp[i].getName())) {
                    if (cmp[i] instanceof JTextComponent) {
                        ((JTextComponent)cmp[i]).setText(status);
                    }
                    break;
                }
            }
        }
    }

    /**
     * This listener changes options state against checkboxes
     */
    private class SelectListener extends MouseInputAdapter implements KeyListener, ListSelectionListener {

        /**
         * @param lst JList of checkboxes
         * @param p parent Panel
         * @param cardLayout The CardLayout for options
         */
        SelectListener(JList lst, JPanel p, CardLayout cardLayout ) {
            list = lst;
            listModel = list.getModel();
            lastSelected = listModel.getElementAt(0);
            panel = p;
            cards = cardLayout;
        }

        public void keyTyped(KeyEvent e) {
            if (e.getKeyChar() == ' ') {
                process(list.getSelectedIndex());
            }
        }

        public void mouseClicked(MouseEvent e) {
            if (e.getPoint().getX() <= emptyCBW) {
                process(list.locationToIndex(e.getPoint()));
            }
        }

        public void valueChanged(ListSelectionEvent e) {
            int index = list.getSelectedIndex();
            JCheckBox box = (JCheckBox)(listModel.getElementAt(index));

            if (lastSelected != box) {
                cards.show(panel, box.getName());
                lastSelected = box;
            }
            enablePanel(box);
        }

        private void enablePanel(final JCheckBox box) {
            for (int i = 0; i < panel.getComponentCount(); i++) {
                JComponent tab = (JComponent) panel.getComponent(i);
                tab.setEnabled(box.isSelected());
            }
            if (customBoxes != null) {
                CustomReport rep = (CustomReport) customBoxes.get(box);
                if (rep != null && rep.getOptionPanes() != null) {
                    for (int i = 0; i < rep.getOptionPanes().length; i++) {
                        rep.getOptionPanes()[i].setEnabled(box.isSelected());
                    }
                } else if (box == cbHtml) {
                    for (int i = 0; i < panes.length; i++) {
                        panes[i].setEnabled(box.isSelected());
                    }
                }
            }

            if (box == cbHtml)
                infoArea.setText(uif.getI18NString("nrd.info.html.txt"));
            else if (box == cbPlain)
                infoArea.setText(uif.getI18NString("nrd.info.plain.txt"));
            else if (box == cbXml)
                infoArea.setText(uif.getI18NString("nrd.info.xml.txt"));
            else if (customBoxes != null) {
                CustomReport rep = (CustomReport) customBoxes.get(box);
                if (rep != null)
                    infoArea.setText(rep.getDescription());
            }

        }

        private void process(final int index) {
            JCheckBox box = (JCheckBox)(listModel.getElementAt(index));

            if (lastSelected == box) {
                box.doClick();
                list.repaint();     // important!
                enablePanel(box);
            }
            lastSelected = box;
        }

        public void keyReleased(KeyEvent e) {
        }

        public void keyPressed(KeyEvent e) {
        }

        Object lastSelected;
        JList list;
        ListModel listModel;
        JPanel panel;
        CardLayout cards;
        double emptyCBW = new JCheckBox("").getPreferredSize().getWidth() + 2;
    }

    /*
     * PropertyChangeListener for enabling/disabling container's content
     */
    private class PanelEnableListener implements PropertyChangeListener {
        /**
         * @param container Container for controlling
         */
        PanelEnableListener(Container container) {
            theContainer = container;
        }
        /**
         * Catches changes of "enabled" property
         * and changes enabled status for all child components
         */
        public void propertyChange(PropertyChangeEvent evt) {
            if ("enabled".equals(evt.getPropertyName())) {
                boolean oldV = ((Boolean) evt.getOldValue()).booleanValue();
                boolean newV = ((Boolean) evt.getNewValue()).booleanValue();
                if (oldV && !newV) {
                    // disable
                    Iterator chIt = collectChildren(theContainer, new ArrayList()).iterator();
                    enabledComp = new HashSet();
                    while (chIt.hasNext()) {
                        Component c = (Component) chIt.next();
                        if (c.isEnabled()) {
                            enabledComp.add(c);
                            c.setEnabled(false);
                        }
                    }

                } else if (!oldV && newV && enabledComp != null) {
                    // enable
                    Iterator chIt = collectChildren(theContainer, new ArrayList()).iterator();
                    while (chIt.hasNext()) {
                        Component c = (Component) chIt.next();
                        if (enabledComp.contains(c)) {
                            c.setEnabled(true);
                        }
                    }
                }
            }
        }

        /**
         * Recursively gathers all children components
         */
        private Collection collectChildren(Container comp, Collection c) {
            Component [] ch = comp.getComponents();
            for(int i = 0; i < ch.length; i++) {
                c.add(ch[i]);
                if (ch[i] instanceof Container) {
                    collectChildren((Container) ch[i],  c);
                }
            }
            return c;
        }

        private Container theContainer;
        private HashSet enabledComp;
    }

    private class CheckBoxListCellRenderer implements ListCellRenderer {
        public Component getListCellRendererComponent(
                    JList list,
                    Object value,
                    int index,
                    boolean isSelected,
                    boolean cellHasFocus) {
            // assert: value is a JCheckBox
            JComponent comp = (JComponent)value;
            if (isSelected) {
                comp.setOpaque(true);
                comp.setBackground(MetalLookAndFeel.getTextHighlightColor());
            }
            else {
                comp.setOpaque(false);
                comp.setForeground(Color.black);
            }

            return comp;
         }
    }

    private Observer[] obs = new Observer[0];
    private ExecModel model;

    private ReportDirChooser reportDirChooser;

    private DefaultListModel listModel;
    private JList list;
    private JTextArea infoArea;
    private CustomReport[] customReports;
    private HashMap customBoxes;

    private JComponent[] panes;
    private JCheckBox cbHtml;
    private JCheckBox cbPlain;
    private JCheckBox cbXml;

    private JCheckBox cbConfig;
    private JCheckBox cbQl;
    private JCheckBox cbEnv;
    private JCheckBox cbStd;
    private JCheckBox cbResults;
    private JCheckBox cbKws;

    private JCheckBox cbPass;
    private JCheckBox cbFail;
    private JCheckBox cbErr;
    private JCheckBox cbNr;
    private JCheckBox cbHtmlRpt;
    private JCheckBox cbHtmlInd;

    private JCheckBox cbBak;
    private JTextField numBak;

    private Vector htmlGroup = new Vector();
    private Vector configGroup = new Vector();

    private File reportDir;
    private JButton browseBtn;
    private JButton okBtn;
    private JButton cancelBtn;
    private JComboBox dirBox;

    private JTabbedPane tabs;

    private Report.Settings lastSettings;

    private ReportBrowser reportBrowser;
    private InterviewParameters interviewParams;
    private FilterSelectionHandler filterHandler;

    // keys for option values used for save & restore
    static final String REPORT_DIR = "reportDir";
    private static final String FILTER = "filter";
    private static final int WAIT_DIALOG_DELAY = 3000;      // 3 second delay

    private JDialog waitDialog;

}
