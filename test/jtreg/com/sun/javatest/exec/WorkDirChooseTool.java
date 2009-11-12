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
import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.HeadlessException;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.Frame;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.io.File;
import java.io.IOException;
import javax.swing.AbstractAction;
import javax.swing.BorderFactory;
import javax.swing.ButtonGroup;
import javax.swing.event.TreeExpansionEvent;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JComponent;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JScrollPane;
import javax.swing.JTextField;
import javax.swing.ListSelectionModel;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;
import javax.swing.border.Border;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import com.sun.interview.Interview;
import com.sun.javatest.TestSuite;
import com.sun.javatest.TemplateUtilities;
import com.sun.javatest.WorkDirectory;
import com.sun.javatest.exec.FileSystemTableModel.FileTableFilter;
import com.sun.javatest.tool.FileChooser;
import com.sun.javatest.tool.Preferences;
import com.sun.javatest.tool.SelectedWorkDirApprover;
import com.sun.javatest.tool.TestSuiteChooser;
import com.sun.javatest.tool.UIFactory;
import com.sun.javatest.tool.WorkDirChooser;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import javax.swing.JFileChooser;

import javax.swing.JViewport;
import javax.swing.WindowConstants;
import javax.swing.table.TableModel;

public class WorkDirChooseTool extends JDialog {
    /**
     * Create a WorkDirChooser, initially showing the user's current directory.
     */
    public WorkDirChooseTool(Frame parent, TestSuite testSuite, final UIFactory uif, int mode) {
        super(parent, true);
        this.uif = uif;
        this.parent = parent;
        this.testSuite = testSuite;
        this.mode = mode;

        swda = new SelectedWorkDirApprover(uif, mode, parent);
        if (defaultDir == null) {
            //defaultDir = new File(System.getProperty("user.dir"));
            try {
                defaultDir = new File(".").getCanonicalFile();
            }
            catch (IOException e) {
                defaultDir = new File(System.getProperty("user.dir"));
            }
        }

        if (currentTemplateDir == null) {
            //currentTemplateDir = new File(System.getProperty("user.dir"));
            try {
                currentTemplateDir = new File(".").getCanonicalFile();
            }
            catch (IOException e) {
                currentTemplateDir = new File(System.getProperty("user.dir"));
            }
        }

        this.setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);

        final int currMode = mode;
        addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                if (currMode == LOAD_TEMPLATE &&
                        !wdWithoutTemplatePermitted(em.getWorkDirectory())) {
                    uif.showError("ce.force_close");
                }
                else {
                    ((JDialog)e.getSource()).setVisible(false);
                }
            }
        });

    }

    public void setDefaultDirectory(File f, boolean isNonDefaultDirAllowed) {
        setDefaultDirectoryNoPrefs(f, isNonDefaultDirAllowed);
        if(f != null) {
            Preferences prefs = Preferences.access();
            try {
                prefs.setPreference(DEFAULT_WD_PREF_NAME, f.getCanonicalPath());
            }
            catch (IOException e) {}
        }
    }

    void setDefaultDirectoryNoPrefs(File f, boolean isNonDefaultDirAllowed) {
        if (f != null) {
            defaultDir = f;
        }
        this.isNonDefaultDirAllowed = isNonDefaultDirAllowed;
    }

    public void updateDefaultDirectory(boolean isNonDefaultDirAllowed) {
        Preferences prefs = Preferences.access();
        String prefsDefaultDir = prefs.getPreference(DEFAULT_WD_PREF_NAME);

        if(prefsDefaultDir != null) {
            setDefaultDirectoryNoPrefs(new File(prefsDefaultDir), isNonDefaultDirAllowed);
        }
    }

    public void setDefaultTemplateDir(File f, boolean isNonDefaultDirAllowed) {
        if (f != null) {
            currentTemplateDir = f;
            defaultTemplateDir = f;
        }
    }

    public void setAllowTraversDirs(boolean allow) {
        this.allowTraversDirs = allow;
    }

    public void setWithoutTemplateMode(boolean withoutTemplate) {
        this.withoutTemplate = withoutTemplate;
    }

    public void initGUI() {

        main = new JPanel() {
            public Dimension getPreferredSize() {
                int dpi = uif.getDotsPerInch();
                if (!hideTemplates) {
                    return new Dimension(5 * dpi, 4 * dpi);
                } else {
                    Dimension d = super.getPreferredSize();
                    d.width = 5 * dpi;
                    return d;
                }
            }
        };

        // convert to switch statement probably?
        // what should the default: title be?
        if (mode == LOAD_TEMPLATE) {
            setTitle(uif.getI18NString("wdc.loadtemplatetitle"));
        } else if (mode == LOAD_CONFIG) {
            setTitle(uif.getI18NString("wdc.loadconfig"));
        } else {
            setTitle(uif.getI18NString("wdc.createtitle"));
        }

        main.setName("wdc.body");
        main.setFocusable(false);
        main.setLayout(new GridBagLayout());
        main.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
        GridBagConstraints lc = new GridBagConstraints();

        if (mode != LOAD_TEMPLATE && mode != LOAD_CONFIG) {
            // CREATE WORKDIR
            // first row, directory name
            JLabel dirLabel = uif.createLabel("wdc.dir.name", true);
            lc.gridx = 0;
            lc.gridy = 0;
            lc.anchor = GridBagConstraints.WEST;
            lc.insets.right = 10;
            lc.insets.bottom = 11;
            main.add(dirLabel, lc);

            tField = uif.createInputField("wdc.namefield", dirLabel);
            tField.addKeyListener(new KeyAdapter() {
                public void keyTyped(KeyEvent evt) {
                    updateCreateBtn();
                }
            });

            lc = new GridBagConstraints();
            lc.gridwidth = 2;
            lc.fill = GridBagConstraints.HORIZONTAL;
            lc.weightx = 1.0;
            main.add(tField, lc);

            // second row, chooser
            JLabel dirPLabel = uif.createLabel("wdc.dir.path", true);
            lc = new GridBagConstraints();
            lc.gridy = 1;
            lc.anchor = java.awt.GridBagConstraints.WEST;

            if (!withoutTemplate) lc.insets.bottom = 10;
            main.add(dirPLabel, lc);

            updateDefaultDirectory(true);

            dirField = uif.createInputField("wdc.savefield", dirPLabel);
            dirField.setText(defaultDir.getAbsolutePath());
            dirField.setEditable(false);
            lc = new GridBagConstraints();
            lc.gridx = 1;
            lc.fill = GridBagConstraints.HORIZONTAL;
            lc.weightx = 1.0;
            if (!withoutTemplate) lc.insets.bottom = 10;
            main.add(dirField, lc);


            JButton browseBtn = uif.createButton("wdc.browse",
                    new ActionListener() {
                public void actionPerformed(ActionEvent ae) {
                    if (fileChooser == null)
                        fileChooser = new FileChooser(true);

                    fileChooser.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
                    fileChooser.setDialogTitle(uif.getI18NString("wdc.filechoosertitle"));

                    File f = new File(dirField.getText());
                    if (f.isDirectory())
                        fileChooser.setCurrentDirectory(f);

                    int returnVal = fileChooser.showOpenDialog(parent);
                    if (returnVal == JFileChooser.APPROVE_OPTION)
                        dirField.setText(fileChooser.getSelectedFile().getAbsolutePath());
                }
            });

            browseBtn.setBorder(BorderFactory.createCompoundBorder(
                    BorderFactory.createEtchedBorder(),
                    BorderFactory.createEmptyBorder(0,3,0,3)));
            lc = new GridBagConstraints();
            lc.gridy = 1;
            lc.insets.left = 11;
            if (!withoutTemplate) lc.insets.bottom = 10;
            main.add(browseBtn, lc);

            // third row, no template
            ButtonGroup group = new ButtonGroup();
            noTemplateCB = uif.createRadioButton("wdc.notemplate", group);
            noTemplateCB.setSelected(true);
            noTemplateCB.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    template = false;
                    //makeBottom();
                    setTemplatesEnabled(false);
                }
            });
            lc = new GridBagConstraints();
            lc.gridy = 2;
            lc.gridwidth = 3;
            lc.anchor = GridBagConstraints.WEST;

            if (!hideTemplates) {
                main.add(noTemplateCB, lc);
            }

            // fourth row, template
            templateCB = uif.createRadioButton("wdc.template", group);
            template = true;
            templateCB.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    template = true;
                    //makeBottom();
                    setTemplatesEnabled(true);
                }
            });

            lc = new GridBagConstraints();
            lc.gridy = 3;
            lc.gridwidth = 3;
            lc.anchor = GridBagConstraints.WEST;

            if (!hideTemplates) {
                main.add(templateCB, lc);
            }

            // just hide radio buttons
            // if tempates are mandatory

            if (!withoutTemplate) {
                noTemplateCB.setVisible(false);
                templateCB.setVisible(false);
            }

        }

        bottom = uif.createPanel("wdc.bottom", false);
        bottom.setLayout(new GridBagLayout());

        Border tb = null;
        switch (mode) {
            case LOAD_TEMPLATE:
            case WorkDirChooser.NEW:
            case WorkDirChooser.OPEN_FOR_ANY_TESTSUITE:
                tb = uif.createTitledBorder("wdc.templateFile");
                break;
            case LOAD_CONFIG:
                tb = uif.createTitledBorder("wdc.configFile");
                break;
            default:        // should not be used, make sure it isn't
                tb = BorderFactory.createEmptyBorder();
        }   // switch

        bottom.setBorder(BorderFactory.createCompoundBorder(
                            tb,
                            BorderFactory.createEmptyBorder(12,12,12,12)));

        templatePLabel = uif.createLabel("wdc.template.path", true);
        lc = new GridBagConstraints();

        lc.insets.right = 10;
        lc.insets.bottom = 11;
        bottom.add(templatePLabel, lc);

        templateField = uif.createInputField("wdc.templatefield", templatePLabel);
        templateField.setEditable(false);
        templateField.setText(currentTemplateDir.getAbsolutePath());

        lc = new GridBagConstraints();
        lc.gridx = 1;
        lc.fill = GridBagConstraints.HORIZONTAL;
        lc.weightx = 1.0;
        bottom.add(templateField, lc);

        makeBrowsTemplateButton();

        if (allowTraversDirs) {
            lc = new GridBagConstraints();
            lc.gridx = 2;

            lc.insets.left = 11;
            lc.weightx = 0.0;
            bottom.add(browseTmplBtn, lc);
        }

        lc.gridwidth = GridBagConstraints.REMAINDER;
        lc.fill = GridBagConstraints.BOTH;

        makeFileList();

        lc = new GridBagConstraints();
        lc.fill = GridBagConstraints.BOTH;
        lc.gridwidth = 3;
        lc.gridy = 1;
        lc.insets.top = 5;

        lc.weightx = 1.0;
        lc.weighty = 1.0;
        lc.fill = GridBagConstraints.BOTH;

        bottom.add(treePanel, lc);
        lc = new GridBagConstraints();
        lc.gridy = 4;
        lc.gridwidth = 3;
        lc.weightx = 1.0;
        lc.weighty = 1.0;
        lc.fill = GridBagConstraints.BOTH;

        if (! hideTemplates) {
            main.add(bottom, lc);
        }

        lc = new GridBagConstraints();
        lc.gridy= 5;
        lc.gridwidth= 3;
        lc.anchor = GridBagConstraints.WEST;

        launchEditorCB = uif.createCheckBox("wdc.launncheditor");
        launchEditorCB.addItemListener(new ItemListener() {
            public void itemStateChanged(ItemEvent e) {
                showConfigEditorFlag = launchEditorCB.isSelected();
            }
        });

        if (!hideTemplates) {
            main.add(launchEditorCB, lc);
        }

        if (mode == WorkDirChooser.NEW) {
            createBtn = uif.createButton("wdc.create" , new CreateWDAction());

        } else if (mode == LOAD_TEMPLATE ) {
            createBtn = uif.createButton("wdc.load" , new LoadTemplateAction());
        } else if ( mode == LOAD_CONFIG) {
            createBtn = uif.createButton("wdc.load" , new LoadConfigAction());
        }
        createBtn.setEnabled(false);
        cancelBtn = uif.createCancelButton("wdc.cancel",
                new ActionListener() {
            public void actionPerformed(ActionEvent ae) {
                setVisible(false);
            }
        });

        if (mode == LOAD_TEMPLATE &&
                !wdWithoutTemplatePermitted(em.getWorkDirectory())) {
            cancelBtn.setEnabled(false);
        }

        setButtons(new JButton[] { createBtn, cancelBtn }, cancelBtn);
        setSize(getPreferredSize());
        setLocationRelativeTo(parent);

        // templates off by default
        switch (mode) {
            case LOAD_TEMPLATE:
                setTemplatesEnabled(true);
                break;
            case LOAD_CONFIG:
                setTemplatesEnabled(true);
                break;
            case WorkDirChooser.NEW:
            case WorkDirChooser.OPEN_FOR_ANY_TESTSUITE:
                setTemplatesEnabled(!withoutTemplate);
                break;
            default:        // should not be used, make sure it isn't
                tb = BorderFactory.createEmptyBorder();
        }   // switch

        pack();
        setVisible(true);
    }


    void openSimpleChooser() {
        updateDefaultDirectory(true);

        WorkDirChooser wdc = new WorkDirChooser(defaultDir);
        wdc.setMode(mode);
        wdc.setTestSuite(testSuite);
        wdc.setAllowNoTemplate(withoutTemplate);

        int action = wdc.showDialog(parent);
        if (action == JFileChooser.APPROVE_OPTION) {
            WorkDirectory wd = wdc.getSelectedWorkDirectory();

            if (!wdWithoutTemplatePermitted(wd)) {
                uif.showError("ce.force_close");
                return;
            }


            setDefaultDirectory(wd.getRoot().getParentFile(), true);

            try {
                if (mode == WorkDirChooser.OPEN_FOR_ANY_TESTSUITE) {
                    em.getExecToolManager().showWorkDirectory(wd);
                } else  if (mode == WorkDirChooser.OPEN_FOR_GIVEN_TESTSUITE ||
                        mode == WorkDirChooser.NEW) {
                    if (em.getTestSuite() == null)
                        em.getExecToolManager().showWorkDirectory(wd);
                    else
                        em.setWorkDir(wd, true);
                }
            } catch (Interview.Fault e) {
                uif.showError("exec.wd.errorOpeningWD", e);
            } catch (TestSuite.Fault e) {
                uif.showError("exec.wd.errorOpeningWD", e);
            }
        }

        // clean up dangling references
        wdc.setSelectedWorkDirectory(null);
        wdc.setTestSuite(null);

    }

    private boolean wdWithoutTemplatePermitted(WorkDirectory wd) {
        String wdTmpl = TemplateUtilities.getTemplatePath(wd);
        FeatureManager fm = em.getContextManager().getFeatureManager();
        if (!fm.isEnabled(fm.WD_WITHOUT_TEMPLATE) && wdTmpl == null) {
            return false;
        }
        return true;
    }

    private class CreateWDAction extends AbstractAction {

        public void actionPerformed(ActionEvent ae) {
            File dir = null;
            if ((dirField.getText() == null) || dirField.getText().equals("")) {
                uif.showError("wdc.dirnotselected");
                return;
            }

            if ((tField.getText() == null) || tField.getText().equals("")) {
                uif.showError("wdc.namenotdefined");
                return;
            }

            dir = new File(dirField.getText());
            if (!dir.isDirectory()) {
                uif.showError("wdc.incorrectdirname");
                return;
            }

            if (selectedTemplate != null) {
                String templateFName = selectedTemplate.getName();
                // what are we using to determine the template filename extension?
                if (!templateFName.endsWith(uif.getI18NString("template.ext"))) {
                    uif.showError("wdc.nottemplatefile");
                    return;
                }
            }
            else {      // null template
                if (!em.getContextManager().getFeatureManager().isEnabled(
                        FeatureManager.WD_WITHOUT_TEMPLATE) && !hideTemplates ) {
                    uif.showError("wdc.wdNeedTemplate");
                    return;
                }
            }

            wd = new File(dir, tField.getText());
            if(!swda.approveNewSelection(wd, testSuite))
                return;

            setDefaultDirectory(dir, true);

            doDone();
        }
    }

    private class LoadTemplateAction extends AbstractAction {

        public void actionPerformed(ActionEvent ae) {
            if (selectedTemplate != null) {
                String templateFName = selectedTemplate.getName();
                if (!templateFName.endsWith(uif.getI18NString("template.ext"))) {
                    uif.showError("wdc.nottemplatefile");
                    return;
                }

            }

            doDone();
        }
    }

    private class LoadConfigAction extends AbstractAction {
        public void actionPerformed(ActionEvent ae) {
            if (selectedTemplate != null) {
                String templateFName = selectedTemplate.getName();
                if (!templateFName.endsWith(uif.getI18NString("config.ext"))) {
                    uif.showError("wdc.notconfigfile");
                    return;
                }
            }

            doDone();
        }
    }


    private void makeFileList() {

        fsm = new FileSystemTableModel(currentTemplateDir.getAbsolutePath(),
                        getTableFilter(), defaultTemplateDir, allowTraversDirs);
        setUpTree(new FileTable(fsm, uif));
    }

    private void setTableListeners() {

        ListSelectionModel rowSM = fileTable.getSelectionModel();
        rowSM.addListSelectionListener(new ListSelectionListener() {
            public void valueChanged(ListSelectionEvent e) {
                //Ignore extra messages.
                if (e.getValueIsAdjusting()) return;

                ListSelectionModel lsm =
                        (ListSelectionModel)e.getSource();
                if (!lsm.isSelectionEmpty()) {
                    int selectedRow = lsm.getMinSelectionIndex();
                    FileSystemTableModel model = (FileSystemTableModel) fileTable.getModel();
                    File selected = model.getNode(selectedRow);
                    if(selected != null && selected.isFile())
                        selectedTemplate = selected;
                    else
                        selectedTemplate = null;
                    updateCreateBtn();
                }
            }
        });

        fileTable.addMouseListener(new MouseAdapter() {
            public void mouseClicked(MouseEvent e) {
                if (e.getClickCount() == 2) {
                    if (fileTable.getSelectedRow() != -1) {
                        FileSystemTableModel model = (FileSystemTableModel) fileTable.getModel();
                        File selected = model.getNode(fileTable.getSelectedRow());
                        if (selected.isDirectory()) {
                            File oldDir = model.getCurrentDir();
                            currentTemplateDir = selected;
                            templateField.setText(currentTemplateDir.getAbsolutePath());
                            model.resetTable(selected.getAbsolutePath());
                            for(int i = 0; i < model.getRowCount(); i++) {
                                if(model.getNode(i).equals(oldDir)) {
                                    fileTable.setRowSelectionInterval(i, i);
                                    scrollIfNeed(i);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        });

        fileTable.addKeyListener(new KeyAdapter() {
            public void keyPressed(KeyEvent e) {
                if(e.getKeyCode() == e.VK_ENTER) {
                    if (fileTable.getSelectedRow() != -1) {
                        FileSystemTableModel model = (FileSystemTableModel) fileTable.getModel();
                        File selected = model.getNode(fileTable.getSelectedRow());
                        if (selected.isDirectory()) {
                            File oldDir = model.getCurrentDir();
                            currentTemplateDir = selected;
                            templateField.setText(currentTemplateDir.getAbsolutePath());
                            model.resetTable(selected.getAbsolutePath());
                            for(int i = 0; i < model.getRowCount(); i++) {
                                if(model.getNode(i).equals(oldDir)) {
                                    try{
                                        fileTable.setRowSelectionInterval(i -1, i - 1);
                                    } catch (IllegalArgumentException exc) {
                                        if(i == 0)
                                            fileTable.setRowSelectionInterval(fileTable.getRowCount() - 1, fileTable.getRowCount() - 1);
                                        else if(i == (fileTable.getRowCount() - 1))
                                            fileTable.setRowSelectionInterval(fileTable.getRowCount() - 2, fileTable.getRowCount() - 2);
                                    }
                                    scrollIfNeed(i);
                                    break;
                                }

                            }
                        }
                        else {
                            ActionEvent ae = new ActionEvent(createBtn, ActionEvent.ACTION_PERFORMED, "pressed");
                            ActionListener[] als = createBtn.getActionListeners();
                            if(als != null)
                                for(int i = 0; i < als.length; i++)
                                    als[i].actionPerformed(ae);
                        }
                    }

                }
                else if((e.getModifiersEx() & e.CTRL_DOWN_MASK) != 0) {
                    return;
                }
                else if((e.getModifiersEx() & e.SHIFT_DOWN_MASK) != 0 && e.getKeyCode() == e.VK_TAB) {
                    int i = fileTable.getSelectedRow();
                    if(i == -1)
                        return;
                    else if(i == 0)
                        i = fileTable.getRowCount() - 1;
                    else
                        i--;
                    fileTable.setRowSelectionInterval(i, i);
                    e.consume();
                    fileTable.scrollRectToVisible(fileTable.getCellRect(i , 0, true));
                }

                else if(e.getKeyCode() == e.VK_TAB) {
                    int i = fileTable.getSelectedRow();
                    if(i == -1)
                        return;
                    else if(i == fileTable.getRowCount() - 1)
                        i = 0;
                    else
                        i++;
                    fileTable.setRowSelectionInterval(i, i);
                    e.consume();
                    fileTable.scrollRectToVisible(fileTable.getCellRect(i , 0, true));
                }
            }

        });

    }

    private void scrollIfNeed(int rowIndex) {
        if(!isCellVisible(rowIndex, 0))
            scrollToCenter(rowIndex, 0);
    }
    private boolean isCellVisible(int rowIndex, int vColIndex) {
        if (!(fileTable.getParent() instanceof JViewport)) {
            return false;
        }
        JViewport viewport = (JViewport)fileTable.getParent();

        Rectangle rect = fileTable.getCellRect(rowIndex, vColIndex, true);
        Point pt = viewport.getViewPosition();
        rect.setLocation(rect.x-pt.x, rect.y-pt.y);

        return new Rectangle(viewport.getExtentSize()).contains(rect);
    }


    public void scrollToCenter(int rowIndex, int vColIndex) {
        if (!(fileTable.getParent() instanceof JViewport)) {
            return;
        }
        JViewport viewport = (JViewport)fileTable.getParent();

        Rectangle rect = fileTable.getCellRect(rowIndex, vColIndex, true);
        Rectangle viewRect = viewport.getViewRect();
        rect.setLocation(rect.x-viewRect.x, rect.y-viewRect.y);

        int centerX = (viewRect.width-rect.width)/2;
        int centerY = (viewRect.height-rect.height)/2;

        if (rect.x < centerX) {
            centerX = -centerX;
        }
        if (rect.y < centerY) {
            centerY = -centerY;
        }
        rect.translate(centerX, centerY);

        viewport.scrollRectToVisible(rect);
    }


    private void makeBrowsTemplateButton() throws HeadlessException {

        browseTmplBtn = uif.createButton("wdc.template.browse", new ActionListener() {
            public void actionPerformed(ActionEvent ae) {
                fileChooser = new FileChooser(true);
                fileChooser.setFileSelectionMode(JFileChooser.FILES_AND_DIRECTORIES);
                switch (mode) {
                    case WorkDirChooser.NEW:
                    case LOAD_TEMPLATE:
                        fileChooser.setDialogTitle(uif.getI18NString("wdc.templchoosertitle"));
                        fileChooser.addChoosableExtension(JTM, uif.getI18NString("ce.jtmFiles"));
                        break;
                    case LOAD_CONFIG:
                        fileChooser.setDialogTitle(uif.getI18NString("wdc.configchoosertitle"));
                        fileChooser.addChoosableExtension(JTI, uif.getI18NString("ce.jtiFiles"));
                        break;
                    default:
                }
                fileChooser.setCurrentDirectory(currentTemplateDir);

                File selectedFile = null;

                int returnVal = fileChooser.showOpenDialog(parent);
                if (returnVal == JFileChooser.APPROVE_OPTION) {
                    currentTemplateDir = fileChooser.getSelectedFile();
                    if (currentTemplateDir.isFile()) {
                        selectedFile = currentTemplateDir;
                        currentTemplateDir = currentTemplateDir.getParentFile();
                    }
                    if (currentTemplateDir != null) {
                        templateField.setText(currentTemplateDir.getAbsolutePath());
                        fsm = new FileSystemTableModel(currentTemplateDir.getAbsolutePath(),  getTableFilter(), defaultTemplateDir, allowTraversDirs);
                        setUpTree(new FileTable(fsm, uif));
                        if (selectedFile != null) {
                            TableModel mod = fileTable.getModel();
                            for (int i = 0 ; i < mod.getRowCount(); i++) {
                                FileTableNode ftn = (FileTableNode) mod.getValueAt(i, 0);
                                if (selectedFile.equals(ftn.getFile())) {
                                    fileTable.getSelectionModel().setSelectionInterval(i, i);
                                    fileTable.scrollRectToVisible(fileTable.getCellRect(i, 0, true));
                                    break;
                                }
                            }
                       }
                    }
                }
            }

        });
        browseTmplBtn.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createEtchedBorder(),
                BorderFactory.createEmptyBorder(0,3,0,3)));
    }

    private void setUpTree(FileTable table) {
        if (treePanel == null) { // first time
            treePanel = new JPanel();
            treePanel.setLayout(new BorderLayout());
        }
        scPane = uif.createScrollPane(table);
        enabledColor = table.getBackground();
        disabledColor = UIManager.getDefaults().getColor("Button.disabledForeground");
        scPane.getViewport().setBackground(enabledColor);
        treePanel.removeAll();
        treePanel.add(scPane, BorderLayout.CENTER);
        fileTable = table;
        setTableListeners();
        main.revalidate();
    }

    private FileTableFilter getTableFilter() {
        if (mode == LOAD_CONFIG) {
            return new FileTableFilter(JTI);
        } else {
            return new FileTableFilter(JTM);
        }
    }


    /**
     * Set the test suite for this chooser.
     * @param ts The test suite to be used when opening or creating a work directory.
     */
    public void setTestSuite(TestSuite ts) {
        testSuite = ts;
    }

    public WorkDirectory getWorkDirectory() {
        return swda.getWorkDirectory();
    }

    private void doDone() {
        File templateName = null;
        if (mode != LOAD_TEMPLATE && mode != LOAD_CONFIG) {
            // CREATE WORKDIR
            // record template path if user selected one or was required to
            // select one
            if (selectedTemplate != null &&
                (templateCB.isSelected() ||
                 !em.getContextManager().getFeatureManager().isEnabled(
                        FeatureManager.WD_WITHOUT_TEMPLATE))) {
                templateName = selectedTemplate;
            }
        } else {
            templateName = selectedTemplate;
        }
        try {
            if (mode == WorkDirChooser.OPEN_FOR_ANY_TESTSUITE) {
                em.getExecToolManager().showWorkDirectory(getWorkDirectory());
            } else  if (mode == WorkDirChooser.OPEN_FOR_GIVEN_TESTSUITE ) {
                em.setWorkDir(getWorkDirectory(), true);
            } else  if (mode == WorkDirChooser.NEW) {
                TemplateUtilities.setTemplateFile(getWorkDirectory(), templateName, true);
                em.setWorkDir(getWorkDirectory(), true);
            } else  if (mode == LOAD_TEMPLATE) {
                TemplateUtilities.setTemplateFile(em.getWorkDirectory(), templateName, true);
                ce.loadNoUI(templateName);
            } else  if (mode == LOAD_CONFIG) {
                ce.loadNoUI(templateName);
            }
        } catch (IOException e) {
            uif.showError("exec.wd.errorOpeningWD", e);
        } catch (Interview.Fault e) {
            uif.showError("exec.wd.errorOpeningWD", e);
        } catch (TestSuite.Fault e) {
            uif.showError("exec.wd.errorOpeningWD", e);
        }



        if (showConfigEditorFlag) {
            if (mode == LOAD_TEMPLATE) {
                em.showTemplateEditor();
            } else {
                em.showConfigEditor(false);
            }
        }

        setVisible(false);
    }

    protected void setButtons(JButton[] buttons, JButton defaultButton) {
        this.buttons = buttons;

        cancelButton = null;
        if (buttons != null) {
            for (int i = 0; i < buttons.length && cancelButton == null; i++) {
                if (buttons[i].getActionCommand().equals(UIFactory.CANCEL))
                    cancelButton = buttons[i];
            }
        }
        initMain();
        setContentPane(main);
        getRootPane().setDefaultButton(defaultButton);
    }


    private void initMain() {
        JPanel m = uif.createPanel(uiKey + ".main", false);
        m.setLayout(new BorderLayout());
        m.add(main, BorderLayout.CENTER);

        // set all the buttons to the same preferred size, per JL&F
        Dimension maxBtnDims = new Dimension();
        for (int i = 0; i < buttons.length; i++) {
            Dimension d = buttons[i].getPreferredSize();
            maxBtnDims.width = Math.max(maxBtnDims.width, d.width);
            maxBtnDims.height = Math.max(maxBtnDims.height, d.height);
        }

        for (int i = 0; i < buttons.length; i++)
            buttons[i].setPreferredSize(maxBtnDims);

        Container p = uif.createPanel(uiKey + ".btns", false);
        p.setLayout(new GridBagLayout());
        GridBagConstraints c = new GridBagConstraints();
        c.anchor = GridBagConstraints.EAST;
        c.insets.top = 0;
        c.insets.bottom = 11;  // value from JL&F Guidelines
        c.insets.right = 11;   // value from JL&F Guidelines
        c.weightx = 1;         // first button absorbs space to the left

        for (int i = 0; i < buttons.length; i++) {
            p.add(buttons[i], c);
            c.weightx = 0;
        }

        m.add(p, BorderLayout.SOUTH);

        main = m;
    }

    private void updateCreateBtn() {
        if (mode == WorkDirChooser.NEW) {
            if ("".equals(tField.getText().trim())) {
                createBtn.setEnabled(false);
            } else {
                createBtn.setEnabled(noTemplateCB.isSelected() || selectedTemplate != null );
            }
        } else if (mode == LOAD_TEMPLATE || mode == LOAD_CONFIG) {
            createBtn.setEnabled(selectedTemplate != null);
        }
    }


    public void setExecModel(ExecModel em) {
        this.em = em;
    }

    private void setTemplatesEnabled(boolean t) {
        browseTmplBtn.setEnabled(t);
        fileTable.setEnabled(t);
        Color c = t ? enabledColor : disabledColor ;
        fileTable.setBackground(c);
        scPane.getViewport().setBackground(c);
        templatePLabel.setEnabled(t);
        templateField.setEnabled(t);
        updateCreateBtn();
    }

    private void setHideTemplateButtons(boolean show) {
        this.hideTemplates = ! show;
    }

    public void doTool() {
        if (mode == WorkDirChooser.NEW || mode == LOAD_TEMPLATE || mode == LOAD_CONFIG) {
            initGUI();
        } else {
            openSimpleChooser();
        }
    }

    public static WorkDirChooseTool getTool(JComponent parent, UIFactory ui, ExecModel em, int mode,
                                            TestSuite ts, boolean showTemplateStuff) {
        // WorkDirChooser.OPEN_FOR_ANY_TESTSUITE
        // WorkDirChooser.OPEN_FOR_GIVEN_TESTSUITE
        // WorkDirChooser.NEW
        // WorkDirChooseTool.LOAD_CONFIG
        // WorkDirChooseTool.LOAD_TEMPLATE

        Frame aFrame = (Frame) SwingUtilities.getAncestorOfClass(Frame.class, parent);
        WorkDirChooseTool wdct = new WorkDirChooseTool(aFrame, ts, ui, mode);
        //wdct.setTestSuiteChooser(getTestSuiteChooser());
        wdct.setDefaultDirectoryNoPrefs(em.getContextManager().getDefaultWorkDirPath(), true);

        if (mode != LOAD_CONFIG) {
            wdct.setDefaultTemplateDir(em.getContextManager().getDefaultTemplateLoadPath(), true);
            wdct.setAllowTraversDirs(em.getContextManager().getAllowTemplateLoadOutsideDefault());
        } else {
            wdct.setDefaultTemplateDir(em.getContextManager().getDefaultConfigLoadPath(), true);
            wdct.setAllowTraversDirs(em.getContextManager().getAllowConfigLoadOutsideDefault());
        }

        wdct.setWithoutTemplateMode(em.getContextManager().getFeatureManager().isEnabled(
                FeatureManager.WD_WITHOUT_TEMPLATE));

        wdct.setExecModel(em);
        wdct.setHideTemplateButtons(showTemplateStuff);

        return wdct;
    }

    void setConfigEditor(ConfigEditor e) {
        ce = e;
    }

    private boolean isNonDefaultDirAllowed = true;
    private boolean template, showConfigEditorFlag;
    private boolean withoutTemplate = true;
    private int mode;
    private boolean allowTraversDirs;

    private Color disabledColor;
    private Color enabledColor;

    private Component parent;
    private File selectedTemplate;
    private File wd;
    private File defaultDir;
    private File currentTemplateDir;
    private File defaultTemplateDir;
    private FileSystemTableModel fsm;
    private FileTable fileTable;
    private JButton createBtn, cancelBtn, browseTmplBtn;
    //private JButton defaultButton;
    private JButton[] buttons;
    private JCheckBox launchEditorCB;
    private FileChooser fileChooser;
    private JLabel templatePLabel;
    private JPanel main, bottom;
    private JPanel treePanel;
    private JRadioButton noTemplateCB, templateCB;
    private JScrollPane scPane;
    private JTextField tField, dirField, templateField;
    private Object cancelButton;
    private SelectedWorkDirApprover swda;
    private ExecModel em;
    private TestSuite testSuite;
    private TestSuiteChooser testSuiteChooser;
    private TreeExpansionEvent event;
    private UIFactory uif;
    private ConfigEditor ce;

    private boolean hideTemplates = false;

    private static final String uiKey = "wdc";

    // values for mode
    // value 1 is WorkDirChooser.NEW
    public static final int LOAD_TEMPLATE = 3;
    public static final int LOAD_CONFIG = 4;

    public static final String DEFAULT_WD_PREF_NAME = "wdct.default_wd_path";

    static final String JTI = ".jti";
    static final String JTM = ".jtm";


}
