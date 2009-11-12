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

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.Color;
import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.util.Date;
import java.util.Map;
import javax.swing.event.MenuEvent;
import javax.swing.event.MenuListener;
import javax.swing.plaf.metal.MetalLookAndFeel;
import javax.swing.JComponent;
import javax.swing.KeyStroke;
import javax.swing.ActionMap;
import javax.swing.InputMap;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import javax.swing.JLabel;
import javax.swing.BorderFactory;
import javax.swing.JTextField;
import javax.swing.Action;
import javax.swing.JPanel;
import javax.swing.JOptionPane;
import javax.swing.JSeparator;

import com.sun.interview.Interview;
import com.sun.interview.Question;

import com.sun.javatest.ExcludeListUpdateHandler;
import com.sun.javatest.InterviewParameters;
import com.sun.javatest.Parameters.LegacyEnvParameters;
import com.sun.javatest.Parameters.MutableConcurrencyParameters;
import com.sun.javatest.Parameters.MutableExcludeListParameters;
import com.sun.javatest.Parameters.MutableKeywordsParameters;
import com.sun.javatest.Parameters.MutablePriorStatusParameters;
import com.sun.javatest.Parameters.MutableTestsParameters;
import com.sun.javatest.Parameters.MutableTimeoutFactorParameters;
import com.sun.javatest.TemplateUtilities;
import com.sun.javatest.TestSuite;
import com.sun.javatest.WorkDirectory;
import com.sun.javatest.tool.FileHistory;
import com.sun.javatest.tool.ToolAction;
import com.sun.javatest.tool.UIFactory;
import com.sun.javatest.util.I18NResourceBundle;

/**
 * ConfigHandler is responsible for providing and maintaining support
 * for the configuration data (ie InterviewParameters) used by the
 * Test Manager window (i.e. ExecTool et al.)
 */
class ConfigHandler
{

    ConfigHandler(JComponent parent, ExecModel model, UIFactory uif) {
        this.parent = parent;
        this.model = model;
        this.uif = uif;
        initActions();
    }

    void addConfigEditorAccelerator(JComponent comp) {
        final String SHOW_FULL_CONFIG_EDITOR = "showFullConfigEditor";

        InputMap imap = comp.getInputMap(JComponent.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT);
        imap.put(configEditorAccelerator, SHOW_FULL_CONFIG_EDITOR);

        ActionMap amap = comp.getActionMap();
        amap.put(SHOW_FULL_CONFIG_EDITOR, showFullConfigAction);
    }

    private static KeyStroke configEditorAccelerator =
        KeyStroke.getKeyStroke(KeyEvent.VK_E, InputEvent.CTRL_MASK);
    private static KeyStroke tEditorAccelerator =
        KeyStroke.getKeyStroke(KeyEvent.VK_T, InputEvent.CTRL_MASK);

    void dispose() {
        if (configEditor != null)
            configEditor.dispose();

        if (checkListBrowser != null)
            checkListBrowser.dispose();

        if (environmentBrowser != null)
            environmentBrowser.dispose();

        if (excludeListBrowser != null)
            excludeListBrowser.dispose();

        if (questionLogBrowser != null)
            questionLogBrowser.dispose();
    }

    JMenu getMenu() {

        changeMenu = new ChangeConfigMenu();

        JMenu menu = uif.createMenu("ch");

        ContextManager cm = model.getContextManager();

        // add Edit Configuration
        menuEdit = uif.createMenuItem(showFullConfigAction);
        menuEdit.setIcon(null);
        menuEdit.setAccelerator(configEditorAccelerator);
        menu.add(menuEdit);

        menuEditT = uif.createMenuItem(showFullConfigTAction);
        menuEditT.setAccelerator(tEditorAccelerator);

        if (isTemplateCreationEnabled()) {
            menu.add(menuEditT);
        }

        // add Edit Quick Set
        menu.add(changeMenu);

        menu.addSeparator();
        menu.add(uif.createMenuItem(newConfigAction));
        menu.add(uif.createMenuItem(loadConfigAction));
        /* pre 4.0, moved to view menu

        menu.add(uif.createMenuItem(showChecklistAction));
        menu.add(uif.createMenuItem(showExcludeListAction));
        menu.add(uif.createMenuItem(showEnvironmentAction));
        menu.add(uif.createMenuItem(showQuestionLogAction));
        menu.addSeparator();
        */
        // custom menu items
        JavaTestMenuManager menuManager = null;
        if (cm != null) {
            menuManager = cm.getMenuManager();
            if (menuManager != null) {
                JMenuItem[] items =
                    menuManager.getMenuItems(JavaTestMenuManager.CONFIG_PRIMARY);
                if (items != null)
                    for (int i = 0; i < items.length; i++)
                        menu.add(items[i]);
            }
        }

        configHistoryListener = new FileHistory.Listener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    JMenuItem mi = (JMenuItem) (e.getSource());
                    File f = (File) (mi.getClientProperty(FileHistory.FILE));
                    if (f != null) {
                        ensureConfigEditorInitialized();
                        configEditor.loadNoUI(f);
                        showConfig();
                    }
                }
            });
        configTemplateHistoryListener = new FileHistory.Listener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                JMenuItem mi = (JMenuItem) (e.getSource());
                File f = (File) (mi.getClientProperty(FileHistory.FILE));
                if (f != null) {
                    ensureConfigEditorInitialized();
                    configEditor.loadNoUI(f);
                    showTemplate(ConfigEditor.FULL_MODE);
                }
            }
        });


        //menu.addMenuListener(configHistoryListener);
        menuHistory = uif.createMenu("ch.history");
        menuHistory.addMenuListener(configHistoryListener);
        menu.add(menuHistory);

        templateSeparator = new JPopupMenu.Separator();
        menuNewTemplate = uif.createMenuItem(newConfigTAction);
        menuEditTemplate = uif.createMenuItem(showConfigTAction);
        menuTemplateHistory = uif.createMenu("ch.templatehistory");
        menuTemplateHistory.addMenuListener(configTemplateHistoryListener);

        FeatureManager fm = null;
        if (cm != null) {
            fm = cm.getFeatureManager();
        }

        if (cm == null || fm.isEnabled(FeatureManager.TEMPLATE_CREATION)) {
            menu.add(templateSeparator);
            menu.add(menuNewTemplate);
            menu.add(menuEditTemplate);
            menu.add(menuTemplateHistory);
        }

        if (cm == null || fm.isEnabled(FeatureManager.SHOW_TEMPLATE_UPDATE)) {
            menu.addSeparator();
            menu.add(uif.createMenuItem(checkUpdatesAction));
        }

        if (cm != null) {
            menuManager = cm.getMenuManager();
            if (menuManager != null) {
                JMenuItem[] items =
                    menuManager.getMenuItems(JavaTestMenuManager.CONFIG_OTHER);
                if (items != null) {
                    menu.addSeparator();
                    for (int i = 0; i < items.length; i++)
                        menu.add(items[i]);
                }   // innerest if
            }
        }

        return menu;
    }

    private boolean isTemplateCreationEnabled() {
        ContextManager cm = model.getContextManager();
        if (cm != null) {
            FeatureManager fm = cm.getFeatureManager();
            return fm.isEnabled(FeatureManager.TEMPLATE_CREATION);
        } else {
            return true;
        }
    }

    private boolean isTemplateEditingEnabled() {
        if (interviewParams == null || !isTemplateCreationEnabled()) {
            return false;
        }
        return interviewParams.getTemplatePath() != null ||
                (interviewParams.getFile() != null && interviewParams.getFile().getName().endsWith(ConfigEditor.TEMPLATE_EXTENSION));
    }

    java.util.List getStatusStrip() {
        //JPanel ss = uif.createPanel("ch.strip", false);

        JLabel lab = uif.createLabel("ch.cfname");
        lab.setDisplayedMnemonic(uif.getI18NString("ch.cfname.mne").charAt(0));

            lab.setBackground(MetalLookAndFeel.getMenuBackground());
            lab.setForeground(new Color(102,102,102));   // #666666
            Font f = MetalLookAndFeel.getSystemTextFont();
            lab.setFont(new Font("Ariel", Font.BOLD, f.getSize()));
        lab.setHorizontalAlignment(JLabel.RIGHT);

        configNameField = uif.createOutputField("ch.cfname", lab);
        configNameField.setColumns(0);
        configNameField.setBackground(MetalLookAndFeel.getMenuBackground());

        configNameField.setForeground(new Color(102,102,102));   // #666666
            configNameField.setFont(new Font("Ariel", Font.BOLD, f.getSize()));
        configNameField.setBorder(BorderFactory.createEmptyBorder());
        configNameField.setText(uif.getI18NString("ch.none"));
        configNameField.setHorizontalAlignment(JTextField.RIGHT);

        showConfigName(interviewParams);

        JLabel lab1 = uif.createLabel("ch.tmplname");
        lab1.setDisplayedMnemonic(uif.getI18NString("ch.tmplname.mne").charAt(0));

            lab1.setBackground(MetalLookAndFeel.getMenuBackground());
            lab1.setForeground(new Color(102,102,102));   // #666666
            f = MetalLookAndFeel.getSystemTextFont();
            lab1.setFont(new Font("Ariel", Font.BOLD, f.getSize()));
        lab1.setHorizontalAlignment(JLabel.LEADING);


        tmplNameField = uif.createOutputField("ch.tmplname", lab1);
        tmplNameField.setColumns(0);
        tmplNameField.setBackground(MetalLookAndFeel.getMenuBackground());
            tmplNameField.setForeground(new Color(102,102,102));   // #666666
            tmplNameField.setFont(new Font("Ariel", Font.BOLD, f.getSize()));
        tmplNameField.setBorder(BorderFactory.createEmptyBorder());
        tmplNameField.setText(uif.getI18NString("ch.none"));
        tmplNameField.setHorizontalAlignment(JTextField.LEADING);

        showTemplateName(interviewParams);

        return java.util.Arrays.asList(new JComponent[]{
                lab, configNameField, lab1, tmplNameField
        });
    }


    private void showConfigName(InterviewParameters ip) {
        if (interviewParams != null && !interviewParams.isTemplate()) {
            // because of the two copies of the interview, interviewParams may
            // be marked as not a template yet may have it's filename set to *.jtm
            if (interviewParams.getFile() != null &&
                !interviewParams.getFile().getName().endsWith(ConfigEditor.TEMPLATE_EXTENSION))
                configNameField.setText(interviewParams.getFile().getName());
            else
                configNameField.setText(uif.getI18NString("ch.none"));
        }
        else {
            // this is the case when a template is loaded for editing
            configNameField.setText(uif.getI18NString("ch.none"));
        }
    }

    private void showTemplateName(InterviewParameters ip) {
        if (interviewParams != null) {
            if (interviewParams.getFile() != null &&
                interviewParams.getFile().getName().endsWith(ConfigEditor.TEMPLATE_EXTENSION)) {
                tmplNameField.setText(interviewParams.getFile().getName());
            }
            else if (interviewParams.getTemplatePath() != null) {
                String path = interviewParams.getTemplatePath();
                int sep = path.lastIndexOf(File.separator);
                path = path.substring(sep < 0 ? 0 : sep+1);
                tmplNameField.setText(path);
            }
        }
        else if (backupTemplateName != null) {
            tmplNameField.setText(backupTemplateName);
        }
        else {
            tmplNameField.setText(uif.getI18NString("ch.none"));
        }
    }


    /**
     * Workaround for the period of time during which the interviewParams
     * is null.
     */
    void setTemplateName(String name) {
        backupTemplateName = name;
    }

    /**
     * Only meant to be called once by core JT.
     */
    JMenuItem[] getConfigViewMenuItems() {
        JMenuItem[] items = new JMenuItem[4];
        items[0] = uif.createMenuItem(showChecklistAction);
        items[1] = uif.createMenuItem(showExcludeListAction);
        items[2] = uif.createMenuItem(showEnvironmentAction);
        items[3] = uif.createMenuItem(showQuestionLogAction);

        return items;
    }

    Action[] getToolBarActions() {
        return new Action[] {
            showFullConfigAction,
            showStdConfigAction
        };
    }

    boolean isConfigEdited() {
        return (configEditor != null && configEditor.isEdited());
    }

    void checkExcludeListUpdate(JComponent parent, boolean quietIfNoUpdate) {
        try {
            InterviewParameters.ExcludeListParameters elp = interviewParams.getExcludeListParameters();
            if ( !(elp instanceof InterviewParameters.MutableExcludeListParameters))
                return;
            InterviewParameters.MutableExcludeListParameters melp = (InterviewParameters.MutableExcludeListParameters) elp;

            URL remote = testSuite.getLatestExcludeList();
            File local = workDir.getSystemFile("latest.jtx");
            ExcludeListUpdateHandler eluh = new ExcludeListUpdateHandler(remote, local);

            if (quietIfNoUpdate && !eluh.isUpdateAvailable())
                return;

            JPanel info = new JPanel(new GridBagLayout());
            info.setBorder(BorderFactory.createEmptyBorder(20, 10, 20, 10));
            GridBagConstraints lc = new GridBagConstraints();
            lc.anchor = GridBagConstraints.EAST;
            GridBagConstraints fc = new GridBagConstraints();
            fc.gridwidth = GridBagConstraints.REMAINDER;
            fc.fill = GridBagConstraints.HORIZONTAL;
            fc.weightx = 1;

            JLabel remoteLbl = uif.createLabel("ch.elu.remote");
            info.add(remoteLbl, lc);

            JTextField remoteText = uif.createOutputField("ch.elu.remote", remoteLbl);
            remoteText.setBorder(null);
            // should consider better date formatting; is this i18n-ok?
            long remoteDate =  eluh.getRemoteURLLastModified();
            String remoteDateText = (remoteDate <= 0 ?
                                     uif.getI18NString("ch.elu.notAvailable")
                                     : new Date(remoteDate).toString());
            remoteText.setText(remoteDateText);
            remoteText.setColumns(remoteDateText.length());
            info.add(remoteText, fc);

            JLabel localLbl = uif.createLabel("ch.elu.local");
            info.add(localLbl, lc);

            JTextField localText = uif.createOutputField("ch.elu.local", localLbl);
            localText.setBorder(null);
            // should consider better date formatting; is this i18n-ok?
            long localDate =  eluh.getLocalFileLastModified();
            String localDateText = (localDate <= 0 ?
                                    uif.getI18NString("ch.elu.notAvailable")
                                    : new Date(localDate).toString());
            localText.setText(localDateText);
            localText.setColumns(localDateText.length());
            info.add(localText, fc);

            if (eluh.isUpdateAvailable()) {
                String title = uif.getI18NString("ch.elu.update.title");
                String head = uif.getI18NString("ch.elu.update.head");
                String foot = uif.getI18NString("ch.elu.update.foot");
                int rc = JOptionPane.showConfirmDialog(parent,
                                                       new Object[] { head, info, foot },
                                                       title,
                                                       JOptionPane.YES_NO_OPTION );
                if (rc == JOptionPane.YES_OPTION)
                    eluh.update(); // should we show message if successful?
            }
            else {
                String title = uif.getI18NString("ch.elu.noUpdate.title");
                String head = uif.getI18NString("ch.elu.noUpdate.head");
                JOptionPane.showMessageDialog(parent,
                                              new Object[] { head, info },
                                              title,
                                              JOptionPane.INFORMATION_MESSAGE );
            }
        }
        catch (IOException e) {
            I18NResourceBundle i18n = uif.getI18NResourceBundle();
            workDir.log(i18n, "ch.elu.logError", e);
            uif.showError("ch.elu.error", e);
        }

        // The following lines are to keep the i18N checks happy, because it is difficult
        // to invoke the various code paths that create the JOptionPanes
        //      getI18NString("ch.elu.local.lbl")
        //      getI18NString("ch.elu.local.tip")
        //      getI18NString("ch.elu.remote.lbl")
        //      getI18NString("ch.elu.remote.tip")

    }

    boolean ensureInterviewUpToDate() {
        try {
            if (interviewParams.isFileNewer())  {
                interviewParams.load();
            }
            return true;
        }
        catch (IOException ex) {
            uif.showError("exec.loadInterview", ex.toString());
            return false;
        }
        catch (InterviewParameters.Fault ex) {
            uif.showError("exec.loadInterview", ex.getMessage());
            return false;
        }
    }

    void ensureConfigEditorInitialized() {
        if (workDir == null)
            throw new IllegalStateException();

        if (configEditor == null) {
            configEditor = new ConfigEditor(parent, interviewParams, model, uif);
            configEditor.setCustomRenderers(customRenderersMap);
            configEditor.setCheckExcludeListListener(new ActionListener() {
                    public void actionPerformed(ActionEvent e) {
                        Object src = e.getSource();
                        JComponent p = (src instanceof JComponent ? (JComponent) src : parent);
                        checkExcludeListUpdate(p, false);
                    }
                });

            configEditor.setObserver(new ConfigEditor.Observer() {
                    public void saved(InterviewParameters p) {
                        updateGUI();
                        showConfigName(p);
                        showTemplateName(p);
                    }
                    public void loaded(InterviewParameters p) {
                        updateGUI();
                        showConfigName(p);
                        showTemplateName(p);
                    }
            });

        }
    }

    void showConfigEditor() {
        if (workDir == null) {
            model.showWorkDirDialog(true);
            if (workDir == null)
                return;
        }

        ensureConfigEditorInitialized();
        configEditor.show(ConfigEditor.FULL_MODE, false);
    }

    void showTemplateEditor() {
        if (workDir == null) {
            model.showWorkDirDialog(true);
            if (workDir == null)
                return;
        }

        ensureConfigEditorInitialized();
        configEditor.show(ConfigEditor.FULL_MODE, true);
    }


    void showConfigEditor(ActionListener l) {
        if (workDir == null) {
            model.showWorkDirDialog(true);
            if (workDir == null)
                return;
        }

        ensureConfigEditorInitialized();
        configEditor.show(ConfigEditor.FULL_MODE, l, false);
    }

    void showTemplateEditor(ActionListener l) {
        if (workDir == null) {
            model.showWorkDirDialog(true);
            if (workDir == null)
                return;
        }

        ensureConfigEditorInitialized();
        configEditor.show(ConfigEditor.FULL_MODE, l, true);
    }

    // should really be observing ExecModel
    void updateGUI() {
        testSuite = model.getTestSuite();
        workDir = model.getWorkDirectory();
        interviewParams = model.getInterviewParameters();

        if (interviewParams != null)
            ensureInterviewUpToDate();

        boolean testSuiteSet = (testSuite != null);

        /*
        boolean configurationSet = (testSuiteSet &&
                interviewParams != null &&
                !interviewParams.isTemplate() &&
                interviewParams.getFile() != null);


        menuEdit.setEnabled(configurationSet);
        changeMenu.setEnabled(configurationSet);
        // toolbar actions
        showFullConfigAction.setEnabled(configurationSet);
        showStdConfigAction.setEnabled(configurationSet);
        */

        boolean templateMenuVisible = false;
        if (testSuiteSet && testSuite != null) {
            templateMenuVisible = isTemplateCreationEnabled();
        }

        templateSeparator.setVisible(templateMenuVisible);
        menuNewTemplate.setVisible(templateMenuVisible);
        menuEditTemplate.setVisible(templateMenuVisible);
        menuTemplateHistory.setVisible(templateMenuVisible);
        menuEditT.setVisible(templateMenuVisible);

        showFullConfigAction.setEnabled(testSuiteSet);
        showStdConfigAction.setEnabled(testSuiteSet);

        // you can only configure tests if the test suite is set
        newConfigAction.setEnabled(testSuiteSet);
        loadConfigAction.setEnabled(testSuiteSet);
        showExcludeListAction.setEnabled(testSuiteSet);
        showEnvironmentAction.setEnabled(testSuiteSet);
        showQuestionLogAction.setEnabled(testSuiteSet);
        showChecklistAction.setEnabled(testSuiteSet
                                       && interviewParams != null
                                       && !interviewParams.isChecklistEmpty());
        newConfigTAction.setEnabled(testSuiteSet);
        showConfigTAction.setEnabled(testSuiteSet);
        checkUpdatesAction.setEnabled(testSuiteSet
                                       && interviewParams != null
                                       && !interviewParams.isTemplate()
                                       && interviewParams.getTemplatePath() != null);

        menuHistory.setEnabled(testSuiteSet);
        if (model.getContextManager() == null
                || model.getContextManager().getFeatureManager().
                isEnabled(FeatureManager.TEMPLATE_CREATION) )
        menuTemplateHistory.setEnabled(testSuiteSet);

        if (interviewParams != null && !interviewParams.isTemplate() && interviewParams.getFile() != null) {

            // there is a template and empty interview
            boolean noEdit = interviewParams.getTemplatePath() == null &&
                    (interviewParams.getWorkDirectory() != null &&
                    TemplateUtilities.getTemplatePath(interviewParams.getWorkDirectory()) != null );

            showFullConfigAction.setEnabled(testSuiteSet && !noEdit);
            showStdConfigAction.setEnabled(testSuiteSet && !noEdit);

            boolean enabled = interviewParams.getTemplatePath() != null ||
                    !interviewParams.isTemplate() ||
                    (interviewParams.getWorkDirectory() != null &&
                    TemplateUtilities.getTemplatePath(interviewParams.getWorkDirectory()) != null );
            showFullConfigTAction.setEnabled(enabled && testSuiteSet && isTemplateEditingEnabled());
        }
        else {
            showFullConfigAction.setEnabled(false);
            showStdConfigAction.setEnabled(false);

            showFullConfigTAction.setEnabled(testSuiteSet && isTemplateEditingEnabled());
        }

        if (workDir != null && configHistoryListener.getFileHistory() == null) {
            FileHistory h = FileHistory.getFileHistory(workDir, "configHistory.jtl");
            configHistoryListener.setFileHistory(h);
        }

        if (workDir != null && configTemplateHistoryListener.getFileHistory() == null) {
            FileHistory h = FileHistory.getFileHistory(workDir, "templateHistory.jtl");
            configTemplateHistoryListener.setFileHistory(h);
        }

        if (interviewParams != null && observer == null) {
            observer = new Interview.Observer() {
                    public void currentQuestionChanged(Question q) {
                    }
                    public void pathUpdated() {
                        showChecklistAction.setEnabled(!interviewParams.isChecklistEmpty());
                    }
                };
            interviewParams.addObserver(observer);
        }

        showTemplateName(interviewParams);
        showConfigName(interviewParams);
    }

    // special method to allow saving when the interview was externally modified
    // for JDTS project, may change in the future
    void syncInterview() {
        ensureConfigEditorInitialized();
        configEditor.saveMain();
    }

    //----------------------------------------------------------------------------
    //
    // what follows are the methods invoved by the various menu and toolbar actions

    void loadConfig() {
        if (workDir == null) {
            model.showWorkDirDialog(true);
            if (workDir == null)
                return;
        }

        ensureConfigEditorInitialized();
        WorkDirChooseTool tool = WorkDirChooseTool.getTool(parent, uif, model,
                                    WorkDirChooseTool.LOAD_CONFIG, testSuite, true);
        tool.setConfigEditor(configEditor);
        tool.doTool();

        //if (templatesUI == null)
        //templatesUI = new TemplatesUI(parent, uif);

        //templatesUI.setMode(TemplatesUI.LOAD_CONFIG);
        //templatesUI.setConfigEditor(configEditor);
        //templatesUI.initGUI();

        //updateGUI();
        showTemplateName(null);
        showConfigName(null);

        showFullConfigAction.setEnabled(true);

        showFullConfigTAction.setEnabled(isTemplateEditingEnabled());
    }

    void newConfig() {
        if (workDir == null) {
            model.showWorkDirDialog(true);
            if (workDir == null)
                return;
        }

        ensureConfigEditorInitialized();
        interviewParams.setTemplate(false);
        configEditor.clear();

        updateGUI();
        showTemplateName(null);
        showConfigName(null);

        showFullConfigAction.setEnabled(true);

        showFullConfigTAction.setEnabled(isTemplateEditingEnabled());
    }

    void showConfig() {
        if (workDir == null) {
            model.showWorkDirDialog(true);
            if (workDir == null)
                return;
        }

        ensureConfigEditorInitialized();
        configEditor.show(false);
    }

    void showTemplate(int mode) {
        if (workDir == null) {
            model.showWorkDirDialog(true);
            if (workDir == null)
                return;
        }

        ensureConfigEditorInitialized();
        configEditor.show(mode, true);
    }

    void showConfig(int mode) {
        if (workDir == null) {
            model.showWorkDirDialog(true);
            if (workDir == null)
                return;
        }

        ensureConfigEditorInitialized();
        configEditor.show(mode, false);
    }

    void newTemplate() {
        if (workDir == null) {
            model.showWorkDirDialog(false);
            if (workDir == null)
                return;
        }

        ensureConfigEditorInitialized();
        interviewParams.setTemplate(true);
        interviewParams.setTemplatePath(null);
        configEditor.clear(true);

        //updateGUI();
        showTemplateName(null);
        showConfigName(null);

        showFullConfigAction.setEnabled(false);
        showFullConfigTAction.setEnabled(true);
    }

    void loadTemplate() {
        if (workDir == null) {
            model.showWorkDirDialog(false);
            if (workDir == null)
                return;
        }
        ensureConfigEditorInitialized();
        //if (templatesUI == null)
//        templatesUI = new TemplatesUI(parent, uif);
//        templatesUI.setMode(TemplatesUI.OPEN_TEMPLATE);
//        templatesUI.setConfigEditor(configEditor);
//        templatesUI.initGUI();

        WorkDirChooseTool tool = WorkDirChooseTool.getTool(parent, uif, model, WorkDirChooseTool.LOAD_TEMPLATE, testSuite, true);
        tool.setConfigEditor(configEditor);
        tool.doTool();
        //updateGUI();
        showTemplateName(null);
        showConfigName(null);

        showFullConfigAction.setEnabled(false);
        showFullConfigTAction.setEnabled(true);
    }

    void showEnvironment() {
        ensureInterviewUpToDate();

        if (isConfigEdited() && !isOKToContinue())
            return;

        if (environmentBrowser == null)
            environmentBrowser = new EnvironmentBrowser(parent, uif);

        environmentBrowser.show(interviewParams);
    }

    void showExcludeList() {
        ensureInterviewUpToDate();

        if (isConfigEdited() && !isOKToContinue())
            return;

        if (excludeListBrowser == null)
            excludeListBrowser = new ExcludeListBrowser(parent, uif);

        excludeListBrowser.show(interviewParams);
    }

    void showChecklist() {
        ensureInterviewUpToDate();

        if (isConfigEdited() && !isOKToContinue())
            return;

        if (checkListBrowser == null)
            checkListBrowser = new ChecklistBrowser(parent, model, uif);

        checkListBrowser.setVisible(true);
    }

    void showQuestionLog() {
        ensureInterviewUpToDate();

        if (isConfigEdited() && !isOKToContinue())
            return;

        if (questionLogBrowser == null)
            questionLogBrowser = new QuestionLogBrowser(parent, model, uif);

        questionLogBrowser.setVisible(true);
    }

    void checkUpdate() {

        try {
            if (interviewParams.isFileNewer())  {
                interviewParams.load();
            } else {
                interviewParams.checkForUpdates();
            }
        }
        catch (IOException ex) {
            uif.showError("exec.loadInterview", ex.toString());
        }
        catch (InterviewParameters.Fault ex) {
            uif.showError("exec.loadInterview", ex.getMessage());
        }

    }


    //----------------------------------------------------------------------------
    //
    // internal methods

    private boolean isOKToContinue() {
        int rc = uif.showOKCancelDialog("ch.edited.warn");
        return (rc == JOptionPane.OK_OPTION);
    }

    // cannot use instance initialization for the actions because they depend
    // on uif being initialized in the constructor
    private void initActions() {
        loadConfigAction = new ToolAction(uif, "ch.load") {
            public void actionPerformed(ActionEvent e) {
                loadConfig();
            }
        };

        newConfigAction = new ToolAction(uif, "ch.new") {
            public void actionPerformed(ActionEvent e) {
                newConfig();
            }
        };

        newConfigTAction = new ToolAction(uif, "ch.newt") {
            public void actionPerformed(ActionEvent e) {
                newTemplate();
            }
        };


        showConfigTAction = new ToolAction(uif, "ch.opent") {
            public void actionPerformed(ActionEvent e) {
                loadTemplate();
            }
        };


        showFullConfigAction = new ConfigAction(uif, "ch.full",
                                                ConfigEditor.FULL_MODE, true);


        showStdConfigAction = new ConfigAction(uif, "ch.std",
                                                ConfigEditor.STD_MODE, true) {
            public void setEnabled(boolean newValue) {
                super.setEnabled(newValue);
                if (changeMenu != null) {
                    changeMenu.checkEnabled();
                }
            }
            public void actionPerformed(ActionEvent e) {
                if (!isEnabled())
                    return;

                Object control = e.getSource();
                if (control instanceof JMenuItem) {
                    JMenuItem mi = (JMenuItem)control;
                    Integer miMode = (Integer)getValue(mi.getName());
                    performAction(miMode);
                }
                else {
                    performAction(mode);
                }
            }
        };

        showFullConfigTAction = new ConfigAction(uif, "ch.fullT",
                                        ConfigEditor.TEMPLATE_FULL_MODE, false);

        showEnvironmentAction = new ToolAction(uif, "ch.env") {
            public void actionPerformed(ActionEvent e) {
                showEnvironment();
            }
        };

        showExcludeListAction = new ToolAction(uif, "ch.excl") {
            public void actionPerformed(ActionEvent e) {
                showExcludeList();
            }
        };

        showChecklistAction = new ToolAction(uif, "ch.checkList") {
            public void actionPerformed(ActionEvent e) {
                showChecklist();
            }
        };

        showQuestionLogAction = new ToolAction(uif, "ch.quLog") {
            public void actionPerformed(ActionEvent e) {
                showQuestionLog();
            }
        };
        checkUpdatesAction = new ToolAction(uif, "ch.chUpdate") {
            public void actionPerformed(ActionEvent e) {
                checkUpdate();
            }
        };
    }

    void loadInterview(File file) {
        ensureConfigEditorInitialized();
        configEditor.loadNoUI(file);
    }

    private JComponent parent;
    private UIFactory uif;
    private ExecModel model;

    private TestSuite testSuite;
    private WorkDirectory workDir;
    private InterviewParameters interviewParams;
    private Interview.Observer observer;
    private Map customRenderersMap;

    private ChangeConfigMenu changeMenu;
    private JMenu menuHistory;
    private JMenu menuTemplateHistory;
    private JMenuItem menuEdit;
    private JMenuItem menuEditT;

    private JSeparator templateSeparator;
    private JMenuItem menuEditTemplate;
    private JMenuItem menuNewTemplate;

    private Action loadConfigAction;
    private Action newConfigAction;
    private Action newConfigTAction;
    private Action showConfigTAction;
    private Action showFullConfigAction;
    private Action showFullConfigTAction;
    private Action showStdConfigAction;
    private Action showEnvironmentAction;
    private Action showExcludeListAction;
    private Action showChecklistAction;
    private Action showQuestionLogAction;
    private Action checkUpdatesAction;

    private ConfigEditor configEditor;
    private FileHistory.Listener configHistoryListener;
    private FileHistory.Listener configTemplateHistoryListener;
    private ChecklistBrowser checkListBrowser;
    private EnvironmentBrowser environmentBrowser;
    private ExcludeListBrowser excludeListBrowser;
    private QuestionLogBrowser questionLogBrowser;
    //private TemplatesUI templatesUI;

    private JTextField configNameField;
    private JTextField tmplNameField;
    private String backupTemplateName;

    void setCustomRenderersMap(Map customRenderersMap) {
        this.customRenderersMap = customRenderersMap;
        if (configEditor != null) {
            configEditor.setCustomRenderers(customRenderersMap);
        }
    }


    private class ConfigAction extends ToolAction {
        public int mode;

        public ConfigAction(UIFactory uif, String key, int mode, boolean needIcon) {
            super(uif, key, needIcon);
            this.mode = mode;
        }

        public void actionPerformed(ActionEvent e) {
            if (!isEnabled())
                return;

            performAction(mode);
        }

        protected void performAction(Integer mode) {
            if (mode != null) {
                if (workDir == null) {
                    ConfigHandler.this.model.showWorkDirDialog(true);
                    if (workDir == null)
                        return;
                }

                ensureConfigEditorInitialized();
                if (mode.intValue() == configEditor.TEMPLATE_FULL_MODE &&
                                                !interviewParams.isTemplate())  {
                    File tFile = null;
                    final File saved = interviewParams.getFile();
                    if (interviewParams.getTemplatePath() != null) {
                        tFile = new File(interviewParams.getTemplatePath());
                    } else if (interviewParams.getWorkDirectory() != null &&
                        TemplateUtilities.getTemplateFile(interviewParams.getWorkDirectory()) != null) {
                        tFile = TemplateUtilities.getTemplateFile(interviewParams.getWorkDirectory());
                    }

                    if (tFile != null) {
                        try {
                            TemplateUtilities.setTemplateFile(ConfigHandler.this.model.getWorkDirectory(), tFile, true);
                            configEditor.loadNoUI(tFile);
                            if (saved != null) {
                                configEditor.setAfterCloseCommand(new Runnable() {
                                    public void run() {
                                        configEditor.loadNoUI(saved);
                                    }
                                });
                            }
                        } catch (IOException ex) {
                            // nonechangeMenu
                        }
                    }
                }

                if (mode.intValue() == configEditor.TEMPLATE_FULL_MODE) {
                    mode = new Integer(configEditor.FULL_MODE);
                }

                configEditor.show(mode.intValue(), interviewParams.isTemplate());
            }
        }

    }

    private class ChangeConfigMenu extends JMenu
        implements MenuListener
    {
        ChangeConfigMenu() {
            uif.initMenu(this, "ch.change");

            tests       = addMenuItem(CHANGE_TESTS,             ConfigEditor.STD_TESTS_MODE);
            excludeList = addMenuItem(CHANGE_EXCLUDE_LIST,      ConfigEditor.STD_EXCLUDE_LIST_MODE);
            keywords    = addMenuItem(CHANGE_KEYWORDS,          ConfigEditor.STD_KEYWORDS_MODE);
            priorStatus = addMenuItem(CHANGE_PRIOR_STATUS,      ConfigEditor.STD_PRIOR_STATUS_MODE);
            environment = addMenuItem(CHANGE_ENVIRONMENT,       ConfigEditor.STD_ENVIRONMENT_MODE);
            concurrency = addMenuItem(CHANGE_CONCURRENCY,       ConfigEditor.STD_CONCURRENCY_MODE);
            timeoutFactor = addMenuItem(CHANGE_TIMEOUT_FACTOR,  ConfigEditor.STD_TIMEOUT_FACTOR_MODE);

            addSeparator();
 /*
            JMenuItem other = addMenuItem(CHANGE_OTHER, ConfigEditor.FULL_MODE);
            other.setAccelerator(configEditorAccelerator);
*/
            addMenuListener(this);

        }

        public void checkEnabled() {
            if (!(tests.isEnabled() || excludeList.isEnabled() ||
                    keywords.isEnabled() || priorStatus.isEnabled() ||
                    environment.isEnabled() || concurrency.isEnabled() ||
                    timeoutFactor.isEnabled())) {
                setEnabled(false);
            }
            else {
                setEnabled(true);
            }
        }

        private JMenuItem addMenuItem(String action, int configEditorMode) {
            JMenuItem mi = uif.createMenuItem(showStdConfigAction);
            mi.setIcon(null);
            mi.setName(action);
            mi.setText(uif.getI18NString("ch.change" + "." + action + ".mit"));
            mi.setMnemonic(uif.getI18NMnemonic("ch.change" + "." + action + ".mne"));
            showStdConfigAction.putValue(action, configEditorMode);
            add(mi);
            return mi;
        }

        // ---------- from MenuListener -----------
        public void menuSelected(MenuEvent e) {
            InterviewParameters c = interviewParams; // alias, to save typing
            if (c == null) // if null, should not even be enabled
                return;

            // Update the various menu items depending whether the corresponding parameters
            // can be handled by the Standard Values view -- ie whether the corresponding
            // parameters are mutable
            // Note: can't ask configEditor and hence stdView directly, since they
            // may not have been initialized yet

            update(tests,       c.getTestsParameters(),         MutableTestsParameters.class);
            update(excludeList, c.getExcludeListParameters(),   MutableExcludeListParameters.class);
            update(keywords,    c.getKeywordsParameters(),      MutableKeywordsParameters.class);
            update(priorStatus, c.getPriorStatusParameters(),   MutablePriorStatusParameters.class);
            update(environment, c.getEnvParameters(),           LegacyEnvParameters.class);
            update(concurrency, c.getConcurrencyParameters(),   MutableConcurrencyParameters.class);
            update(timeoutFactor, c.getTimeoutFactorParameters(), MutableTimeoutFactorParameters.class);
        }

        private void update(JMenuItem mi, Object o, Class c) {
            mi.setVisible(o != null && c.isAssignableFrom(o.getClass()));
        }

        public void menuDeselected(MenuEvent e) {
        }

        public void menuCanceled(MenuEvent e) {
        }

        void loadInterview(File file)
        {
            configEditor.load(file);
        }


        // ----------

        private JMenuItem tests;
        private JMenuItem excludeList;
        private JMenuItem keywords;
        private JMenuItem priorStatus;
        private JMenuItem environment;
        private JMenuItem concurrency;
        private JMenuItem timeoutFactor;

        private static final String CHANGE_TESTS = "test";
        private static final String CHANGE_EXCLUDE_LIST = "excl";
        private static final String CHANGE_KEYWORDS = "keyw";
        private static final String CHANGE_PRIOR_STATUS = "stat";
        private static final String CHANGE_ENVIRONMENT = "envt";
        private static final String CHANGE_CONCURRENCY = "conc";
        private static final String CHANGE_TIMEOUT_FACTOR = "time";
        //private static final String CHANGE_OTHER = "other";

    };

}
