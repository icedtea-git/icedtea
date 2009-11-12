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

import java.awt.CardLayout;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.FocusTraversalPolicy;
import java.awt.KeyboardFocusManager;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;
import java.util.TreeSet;
import javax.swing.BorderFactory;
import javax.swing.ButtonGroup;
import javax.swing.JCheckBoxMenuItem;
import javax.swing.JComponent;
import javax.swing.JFileChooser;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JRadioButtonMenuItem;
import javax.swing.JSplitPane;
import javax.swing.KeyStroke;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.event.MenuEvent;
import javax.swing.event.MenuListener;

import com.sun.interview.Interview;
import com.sun.interview.Question;
import com.sun.javatest.InterviewParameters;
import com.sun.javatest.JavaTestError;
import com.sun.javatest.TemplateUtilities;
import com.sun.javatest.TestSuite;
import com.sun.javatest.tool.FileChooser;
import com.sun.javatest.tool.FileHistory;
import com.sun.javatest.tool.HelpLink;
import com.sun.javatest.tool.Preferences;
import com.sun.javatest.tool.ToolDialog;
import com.sun.javatest.tool.UIFactory;
import java.awt.AWTEvent;
import javax.swing.SwingUtilities;
import javax.swing.WindowConstants;

class ConfigEditor extends ToolDialog
{
    ConfigEditor(JComponent parent, InterviewParameters config, ExecModel model, UIFactory uif) {
        super(parent, uif, "ce");

        this.model = model;

        history = FileHistory.getFileHistory(config.getWorkDirectory(), "configHistory.jtl");
        historyTemplate = FileHistory.getFileHistory(config.getWorkDirectory(), "templateHistory.jtl");

        mainConfig = config;
        try {
            viewConfig = config.getTestSuite().createInterview();
            viewConfig.setWorkDirectory(mainConfig.getWorkDirectory());
            copy(mainConfig, mainConfig); // for synchronization
            copy(mainConfig, viewConfig); // init view to be the same as main
        }
        catch (Interview.Fault e) {
            // ignore, for now; should not happen
        }
        catch (TestSuite.Fault e) {
            // ignore, for now; should not happen
        }

    }

    public void clear() {
        clear(false);
    }

    void clear(boolean isTemplate) {
        templateMode = isTemplate;

        if (stdView == null)
            initGUI();

        if (currView != null && currView.isShowing())
            currView.save();

        // viewConfig.isEdited() will be true unless viewConfig is already clear,
        // in which case there is no need to do anything except make sure the view
        // is visible.
        if (viewConfig.isEdited()) {
            // if viewConfig differs from mainConfig, we show a warning, giving the user
            // a chance to back out
            // (don't use isEdited() because that would call currView.save() again)
            if (!equal(mainConfig, viewConfig)) {
                // show a warning
                int rc = uif.showYesNoCancelDialog("ce.clear.warn");

                switch (rc) {
                case JOptionPane.YES_OPTION:
                    if (save0())
                        break;
                    else
                        return;

                case JOptionPane.NO_OPTION:
                    break;

                default:
                    return;
                }
            }

            // clear viewConfig, but leave mainConfig alone for now(until we exit/save)
            // so that the user can revert the clear if so desired
            viewConfig.clear();
            viewConfig.setEdited(false);
        }

        viewConfig.setTemplate(templateMode);
        mainConfig.setTemplate(templateMode);

        File f = TemplateUtilities.getTemplateFile(mainConfig.getWorkDirectory());
        if (f != null) {
            try {
                model.getTestSuite().loadInterviewFromTemplate(f, viewConfig);
            }
            catch (IOException e) {
                e.printStackTrace();        // need better error
            }
            catch (TestSuite.Fault fault) {
                fault.printStackTrace();        // need better error
            }
        }

        updateTitle();
        // always set the full view, because that is the only way you can
        // enter a full configuration; standard values "won't cut it" :-)
        setView(fullView);
        setVisible(true);
    }

    public void load(boolean isTemplate) {
        JComponent jparent = (JComponent) SwingUtilities.getAncestorOfClass(JComponent.class, parent);
        WorkDirChooseTool tool;
        if (isTemplate) {
            tool = WorkDirChooseTool.getTool(jparent, uif, model,
                    WorkDirChooseTool.LOAD_TEMPLATE, model.getTestSuite(), true);
        } else {
            tool = WorkDirChooseTool.getTool(jparent, uif, model,
                    WorkDirChooseTool.LOAD_CONFIG, model.getTestSuite(), true);
        }
        tool.setConfigEditor(this);
        tool.doTool();

    }

    public void load(File file) {
        if (file == null)
            throw new NullPointerException();
        load0(file, true);
    }

    public void loadNoUI(File file) {
        if (file == null)
            throw new NullPointerException();
        load0(file, false);
    }


    private void load0(File file, boolean showUI) {
        if (isEdited()) {
            // show a warning
            int rc = uif.showYesNoCancelDialog("ce.load.warn");

            switch (rc) {
            case JOptionPane.YES_OPTION:
            if (save0())
                break;
            else
                return;

            case JOptionPane.NO_OPTION:
            break;

            default:
            return;
            }
        }

        // check feature manager
        ContextManager cm = model.getContextManager();
        FeatureManager fm = cm.getFeatureManager();

        if (fm != null &&
            !fm.isEnabled(FeatureManager.WD_WITHOUT_TEMPLATE) &&
            !fm.isEnabled(FeatureManager.TEMPLATE_LOADING)) {
            try {
                TemplateUtilities.ConfigInfo info =
                            TemplateUtilities.getConfigInfo(file);

                if (info != null) {
                    String wdTmpl =
                         TemplateUtilities.getTemplatePath(model.getWorkDirectory());
                    if (info.getTemplatePath() != null &&
                        !info.getTemplatePath().equals(wdTmpl)) {
                        uif.showError("ce.load.cmismatch", new Object[] {wdTmpl,
                                        info.getTemplatePath()});
                        // workdir template and config template don't match
                        return;
                    }
                }
            }
            catch (Throwable e) {
                uif.showError("ce.load.badfile", file.getPath());
                return;
            }
        }
        else {
        }

        File mainConfigFile = mainConfig.getFile();

        FileChooser fileChooser = getFileChooser();

        if (mainConfigFile != null)
            fileChooser.setCurrentDirectory(mainConfigFile.getParentFile());

        if (showUI) {
            file = loadConfigFile(model.getContextManager(), parent, uif, fileChooser);
        }

        if (file == null)
            return;

        try {
            mainConfig.load(file);
            copy(mainConfig, viewConfig);
            if (currView != null && currView.isShowing())
                currView.load();
            templateMode = mainConfig.isTemplate();
            if (templateMode)
                historyTemplate.add(file);
            else
                history.add(file);
            updateTitle();

            if (observer != null)
                observer.loaded(mainConfig);
        }
        catch (FileNotFoundException e) {
            uif.showError("ce.load.cantFindFile", file);
        }
        catch (IOException e) {
            uif.showError("ce.load.error", new Object[] { file, e } );
        }
        catch (Interview.Fault e) {
            uif.showError("ce.load.error", new Object[] { file, e.getMessage() } );
        }
    }

    public void save() {
        save0();
    }

    // backdoor to allow saving of external changes
    void saveMain() {
        File file = mainConfig.getFile();
        if (file == null)
            throw new IllegalStateException("No configuration file saved yet, cannot update!");

        try {
            mainConfig.save(file);

            if (observer != null)
                observer.saved(mainConfig);
        }
        catch (IOException e) {
            if (!file.canWrite())
                uif.showError("ce.save.cantWriteFile", file);
            else if (e instanceof FileNotFoundException)
                uif.showError("ce.save.cantFindFile", file);
            else
                uif.showError("ce.save.error", new Object[] { file, e } );
        }
        catch (Interview.Fault e) {
            uif.showError("ce.save.error", new Object[] { file, e.getMessage() } );
        }
    }

    // return true if saved, false if cancelled/error
    private boolean save0() {
        // Use the filename saved in the viewConfig.
        // By default, this will have been copied from the mainConfig,
        // but it may have been cleared if clear() has been called, thereby
        // making "save" behave as "saveAs".
        return save0(viewConfig.getFile());
    }

    public void saveAs() {
        save0(null);
    }

    // return true if saved, false if cancelled/error
    private boolean save0(File file) {
        if (file == null) {
            File mainConfigFile = mainConfig.getFile();
            File mainConfigDir = (mainConfigFile == null ? null : mainConfigFile.getParentFile());
            file = getSaveFile(mainConfigDir);
            if (file == null)
                return false; // exit without saving
        }

        try {
            if (currView != null) {
                currView.save();
                copy(viewConfig, mainConfig, false);
                // don't bother to copy filename since we're about to save
                // mainConfig in "file"
            }
            mainConfig.save(file, templateMode);
            viewConfig.setFile(file);   // for subsequent use
            if (templateMode) {
                // set up this template for WD
                TemplateUtilities.setTemplateFile(viewConfig.getWorkDirectory(),
                                                    file, true);
                historyTemplate.add(file);
            }
            else
                history.add(file);

            updateTitle();

            if (observer != null)
                observer.saved(mainConfig);
            return true;
        }
        catch (IOException e) {
            if (!file.canWrite())
                uif.showError("ce.save.cantWriteFile", file);
            else if (e instanceof FileNotFoundException)
                uif.showError("ce.save.cantFindFile", file);
            else
                uif.showError("ce.save.error", new Object[] { file, e } );
        }
        catch (Interview.Fault e) {
            uif.showError("ce.save.error", new Object[] { file, e.getMessage() } );
        }

        return false;
    }

    private File getSaveFile(File dir) {
        FileChooser fileChooser = getFileChooser();
        if (this.templateMode)
            fileChooser.setDialogTitle(uif.getI18NString("ce.save.titlet"));
        else
            fileChooser.setDialogTitle(uif.getI18NString("ce.save.title"));
        return saveConfigFile(model.getContextManager(), parent, uif, fileChooser, dir,
                this.templateMode);
    }

    private FileChooser getFileChooser() {

        FileChooser fileChooser = new FileChooser(true);
        if (templateMode) {
            fileChooser.addChoosableExtension(JTM,
                uif.getI18NString("ce.jtmFiles"));
        } else {
            fileChooser.addChoosableExtension(JTI,
                uif.getI18NString("ce.jtiFiles"));
        }
        return fileChooser;
    }

    public void saveAsTemplate() {
        if (templateDialog == null)
            templateDialog = new CE_TemplateDialog(getTool(), viewConfig, model, uif);

        if (currView != null)
            currView.save();

        templateDialog.setVisible(true);
    }

    public void revert() {
        if (!isEdited())
            return;

        int rc = uif.showOKCancelDialog("ce.revert.warn");
        if (rc != JOptionPane.OK_OPTION)
            return;

        try {
            copy(mainConfig, viewConfig);
            if (currView != null && currView.isShowing())
                currView.load();
            updateTitle();
        }
        catch (Interview.Fault e) {
            uif.showError("ce.revert", e.getMessage());
        }
    }

    public void setRunPending(boolean b) {
        runPending = b;
    }

    public boolean isRunPending() {
        return runPending;
    }

    public void show(boolean isTemplateMode) {
        if (stdView == null)
            initGUI();

        show(DEFAULT_MODE, isTemplateMode);
    }

    public void show(int mode, boolean isTemplateMode) {
        this.templateMode = isTemplateMode;
        if (stdView == null)
            initGUI();
        updateTitle();
        fullView.setCustomRenderers(customRenderersMap);
        switch (mode) {
        case DEFAULT_MODE:
            show(currView == null ? getDefaultView() : currView);
            break;

        case FULL_MODE:
            show(fullView);
            break;

        case STD_MODE:
            show(stdView);
            break;

        case STD_TESTS_MODE:
            stdView.showTab(CE_StdView.TESTS_PANE);
            show(stdView);
            break;

        case STD_EXCLUDE_LIST_MODE:
            stdView.showTab(CE_StdView.EXCLUDE_LIST_PANE);
            show(stdView);
            break;

        case STD_KEYWORDS_MODE:
            stdView.showTab(CE_StdView.KEYWORDS_PANE);
            show(stdView);
            break;

        case STD_PRIOR_STATUS_MODE:
            stdView.showTab(CE_StdView.PRIOR_STATUS_PANE);
            show(stdView);
            break;

        case STD_ENVIRONMENT_MODE:
            stdView.showTab(CE_StdView.ENVIRONMENT_PANE);
            show(stdView);
            break;

        case STD_CONCURRENCY_MODE:
            stdView.showTab(CE_StdView.CONCURRENCY_PANE);
            show(stdView);
            break;

        case STD_TIMEOUT_FACTOR_MODE:
            stdView.showTab(CE_StdView.TIMEOUT_FACTOR_PANE);
            show(stdView);
            break;

        default:
            throw new IllegalArgumentException();
        }
    }

    public void show(ActionListener closeListener, boolean isTemplateMode) {
        this.closeListener = closeListener;
        show(isTemplateMode);
    }

    public void show(int mode, ActionListener closeListener, boolean isTemplateMode) {
        this.closeListener = closeListener;
        show(mode, isTemplateMode);
    }

    public static final int DEFAULT_MODE = 0;
    public static final int FULL_MODE = 1;
    public static final int STD_MODE = 2;
    public static final int STD_TESTS_MODE = 3;
    public static final int STD_EXCLUDE_LIST_MODE = 4;
    public static final int STD_KEYWORDS_MODE = 5;
    public static final int STD_PRIOR_STATUS_MODE = 6;
    public static final int STD_ENVIRONMENT_MODE = 7;
    public static final int STD_CONCURRENCY_MODE = 8;
    public static final int STD_TIMEOUT_FACTOR_MODE = 9;
    public static final int TEMPLATE_FULL_MODE = 10;

    private void show(CE_View newView) {
        // update viewConfig from currView (if showing) else mainConfig
        if (currView != null && currView.isShowing())
            currView.save();
        else {
            try {
                copy(mainConfig, viewConfig);
            }
            catch (Interview.Fault e) {
                uif.showError("ce.show.error", e.getMessage());
            }
        }

        setView(newView);

        setVisible(true);
    }

    private void setView(CE_View newView) {
        if (newView == null)
            throw new NullPointerException();

        if (currView != null && currView == newView) {
            currView.load();
        }

        if (currView != newView) {
            // note whether the focus is in the current view
            KeyboardFocusManager kfm = KeyboardFocusManager.getCurrentKeyboardFocusManager();
            Component fo = kfm.getPermanentFocusOwner();
            boolean focusInView = (fo != null && currView != null && currView.isAncestorOf(fo));

            currView = newView;

            // update currView from viewConfig
            currView.load();

            // set up the appropriate view and controls
            ((CardLayout)(views.getLayout())).show(views, currView.getName());

            // The following is a workaround for what may be a JDK bug.
            // As a result of changing the view, the permanent focus owner may no longer
            // be showing, and therefore not accepting any keyboard input.
            if (focusInView) {
                Container fcr = (currView.isFocusCycleRoot() ? currView : currView.getFocusCycleRootAncestor());
                FocusTraversalPolicy ftp = fcr.getFocusTraversalPolicy();
                Component c = ftp.getDefaultComponent(fcr);
                if (c != null)
                    c.requestFocusInWindow();
            }

            boolean currIsFull = (currView == fullView);
            markerMenu.setEnabled(currIsFull);
            searchMenu.setEnabled(currIsFull);
            (currIsFull ? viewFullBtn : viewStdBtn).setSelected(true);
            viewTagCheckBox.setEnabled(currIsFull);

            if (detailsBrowser != null)
                detailsBrowser.setQuestionInfoEnabled(currIsFull);

            updateTitle();
        }
    }

    public void close() {
        if (currView != null && !currView.isOKToClose()) {
            if (afterCloseCommand != null) {
                afterCloseCommand.run();
                afterCloseCommand = null;
            }
            return;
        }

        close(true);
    }

    protected void windowClosingAction(AWTEvent e) {
        if (!canInterruptTemplateCreation()) {
            uif.showError("ce.force_close");
            return;
        }

        if(fullView.isVisible()) {
            fullView.prepareClosing();
        }
        close();
    }

    private void close(boolean checkIfEdited) {
        if (currView == null)
            return;

        if (!isShowing())
            return;

        if (checkIfEdited && isEdited()) {
            int rc = uif.showYesNoCancelDialog("ce.close.warn");
            switch (rc) {
            case JOptionPane.YES_OPTION:
                if (save0()) {
                    break;
                } else {
                    if (afterCloseCommand != null) {
                        afterCloseCommand.run();
                        afterCloseCommand = null;
                    }
                    return;
                }

            case JOptionPane.NO_OPTION:
                try {
                    copy(mainConfig, viewConfig);
                } catch (Exception exc) {}
                break;

            default:
                if (afterCloseCommand != null) {
                    afterCloseCommand.run();
                    afterCloseCommand = null;
                }
                return;
            }
        }

        setVisible(false);

        // closeListener may have been set by show(ActionListener)
        if (closeListener != null) {
            ActionEvent e = new ActionEvent(this,
                                            ActionEvent.ACTION_PERFORMED,
                                            CLOSE);
            closeListener.actionPerformed(e);
            closeListener = null;
        }

        if (afterCloseCommand != null) {
            afterCloseCommand.run();
            afterCloseCommand = null;
        }

    }

    void setCheckExcludeListListener(ActionListener l) {
        if (stdView == null)
            initGUI();

        stdView.setCheckExcludeListListener(l);
    }

    boolean isCurrentQuestionChanged() {
        if (currView != null && currView.isShowing())
            currView.save();

        Question mq = mainConfig.getCurrentQuestion();
        Question vq = viewConfig.getCurrentQuestion();
        return !equal(mq.getTag(), vq.getTag());
    }


    boolean isEdited() {
        if (currView != null && currView.isShowing())
            currView.save();

        return !equal(mainConfig, viewConfig);
    }

    private static boolean equal(InterviewParameters a, InterviewParameters b) {
        // do ez checks first
        if (a.getMarkersEnabled() != b.getMarkersEnabled()
            || a.getMarkersFilterEnabled() != b.getMarkersFilterEnabled()) {
            return false;
        }

        Map aQuestions = a.getAllQuestions();
        Map bQuestions = b.getAllQuestions();

        Set keys = new TreeSet();
        keys.addAll(aQuestions.keySet());
        keys.addAll(bQuestions.keySet());

        for (Iterator iter = keys.iterator(); iter.hasNext(); ) {
            String key = (String) (iter.next());
            Question aq = (Question) aQuestions.get(key);
            Question bq = (Question) bQuestions.get(key);
            if (aq == null || bq == null) {
                return false;
            }
            else if(!aq.equals(bq)) {
                boolean eq = (aq.getStringValue() == null && bq.getStringValue() == "") ||
                        (aq.getStringValue() == "" && bq.getStringValue() == null);
                if(!eq) {
                    return false;
                }
            }
        }

        return true;
    }

    void setObserver(Observer o) {
        observer = o;
    }

    private static boolean equal(String a, String b) {
        return (a == null || b == null ? a == b : a.equals(b));
    }

    protected void initGUI() {
        listener = new Listener();

        updateTitle();

        fullView = new CE_FullView(viewConfig, uif, listener);
        stdView = new CE_StdView(viewConfig, uif, listener);
        stdView.setParentToolDialog(this);

        initMenuBar();

        views = uif.createPanel("ce.views", new CardLayout(), false);
        views.add(fullView, fullView.getName());
        views.add(stdView, stdView.getName());

            Preferences p = Preferences.access();
            boolean prefMoreInfo = p.getPreference(MORE_INFO_PREF, "true").equals("true");
            views.setBorder(BorderFactory.createEmptyBorder(1, 1, 1, 1));
            body = views;

        // Don't register "shift alt D" on body, because body might change
        // if the more info is opened/closed.
        // Instead, register it on views and infoPanel
        views.registerKeyboardAction(listener, DETAILS, detailsKey,
                                           JComponent.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT);

        setBody(body);

        setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);
    }

    private void initMenuBar() {
        JMenuBar menuBar = uif.createMenuBar("ce.menub");

        // file menu
        String[] fileMenuItems;
        ContextManager cm = model.getContextManager();
        if (cm != null && !cm.getFeatureManager().isEnabled(FeatureManager.TEMPLATE_CREATION)) {
            fileMenuItems = new String[] { SAVE, SAVE_AS, REVERT, null, NEW, LOAD, null, CLOSE };
        } else {
            fileMenuItems = new String[] { SAVE, SAVE_AS, REVERT, null, NEW, LOAD, null, NEWT, LOADT, null, CLOSE };
        }
        int historyIndex = fileMenuItems.length - 5;
        JMenu fileMenu = uif.createMenu("ce.file", fileMenuItems, listener);
        FileHistory h = FileHistory.getFileHistory(viewConfig.getWorkDirectory(), "configHistory.jtl");
        FileHistory.Listener l = new FileHistory.Listener(h, 0, (ActionListener)listener);
            JMenu mm = uif.createMenu("ce.history");
        mm.addMenuListener(l);
        fileMenu.insert(mm, historyIndex);
        if (cm == null || cm != null && cm.getFeatureManager().isEnabled(
                FeatureManager.TEMPLATE_CREATION)) {
            int historyTemplateIndex = fileMenuItems.length - 1;
            FileHistory ht = FileHistory.getFileHistory(viewConfig
                    .getWorkDirectory(), "templateHistory.jtl");
            FileHistory.Listener lt = new FileHistory.Listener(ht, 0,
                    (ActionListener) listener);
            JMenu mmt = uif.createMenu("ce.templatehistory");
            mmt.addMenuListener(lt);
            fileMenu.insert(mmt, historyTemplateIndex);
        }
        menuBar.add(fileMenu);

        // marker menu
        markerMenu = fullView.getMarkerMenu();
        menuBar.add(markerMenu);

        // search menu
        searchMenu = fullView.getSearchMenu();
        menuBar.add(searchMenu);

        // view menu
        viewMenu = uif.createMenu("ce.view");
        viewMenu.addMenuListener(listener);
        ButtonGroup viewGroup = new ButtonGroup();

        viewFullBtn = uif.createRadioButtonMenuItem("ce.view", CE_View.FULL);
        viewFullBtn.setSelected(true);
        viewFullBtn.setActionCommand(CE_View.FULL);
        viewFullBtn.addActionListener(listener);
        viewGroup.add(viewFullBtn);
        viewMenu.add(viewFullBtn);

        viewStdBtn = uif.createRadioButtonMenuItem("ce.view", CE_View.STD);
        viewStdBtn.setActionCommand(CE_View.STD);
        viewStdBtn.addActionListener(listener);
        viewGroup.add(viewStdBtn);
        viewMenu.add(viewStdBtn);

        viewMenu.addSeparator();

        viewTagCheckBox = uif.createCheckBoxMenuItem("ce.view", "tag", false);
        viewTagCheckBox.setAccelerator(KeyStroke.getKeyStroke("control T"));
        viewTagCheckBox.addChangeListener(listener);
        viewMenu.add(viewTagCheckBox);

        viewMenu.addSeparator();

        viewRefreshItem = uif.createMenuItem("ce.view", "refresh", listener);
        viewRefreshItem.setAccelerator(KeyStroke.getKeyStroke("F5"));
        viewMenu.add(viewRefreshItem);

        menuBar.add(viewMenu);

        menuBar.add(uif.createHorizontalGlue("ce.pad"));

        // help menu
        JMenu helpMenu = uif.createMenu("ce.help");

        menuBar.add(helpMenu);

        setJMenuBar(menuBar);
    }

    private void updateTitle() {
        File f = viewConfig.getFile();
        if (templateMode) {
            setI18NTitle("ce.titlet",
                    new Object[] { new Integer(currView == fullView ? 0 : 1),
                    new Integer(f == null ? 0 : 1), f });
        } else {
            setI18NTitle("ce.title",
                    new Object[] { new Integer(currView == fullView ? 0 : 1),
                    new Integer(f == null ? 0 : 1), f });
        }
    }

    private CE_View getDefaultView() {
        Preferences p = Preferences.access();
        String prefView = p.getPreference(VIEW_PREF, CE_View.FULL);
        if (prefView.equals(CE_View.STD))
            return stdView;
        else
            return fullView;
    }

    private void perform(String cmd) {
        if (cmd.equals(NEW))
            clear();
        else if (cmd.equals(LOAD))
            load(false);
        else if (cmd.equals(LOADT))
            load(true);
        else if (cmd.equals(NEWT))
            clear(true);
        else if (cmd.equals(SAVE))
            save();
        else if (cmd.equals(SAVE_AS))
            saveAs();
        else if (cmd.equals(SAVE_AS_TEMPLATE))
            saveAsTemplate();
        else if (cmd.equals(REVERT))
            revert();
        else if (cmd.equals(CE_View.FULL))
            show(fullView);
        else if (cmd.equals(CE_View.STD))
            show(stdView);
        else if (cmd.equals(CLOSE)) {
            if (canInterruptTemplateCreation()) {
                close();
            }
            else {
                uif.showError("ce.force_close");
            }
        }
        else if (cmd.equals(DONE)) {
            if (currView != null && !currView.isOKToClose())
                return;

            if (!canInterruptTemplateCreation() && !viewConfig.isFinishable()) {
                uif.showError("ce.force_close");
                return;
            }

            currView.save();
            if (!viewConfig.isFinishable()) {
                Integer rp = new Integer(runPending ? 1 : 0);
                int rc = uif.showOKCancelDialog("ce.okToClose", rp);
                if (rc != JOptionPane.OK_OPTION)
                    return;
            }

            if (isEdited() || isCurrentQuestionChanged())
                saveRequired = true;

            if (saveRequired) {
                if (!save0()) {
                    // save failed, stay in CE, don't clear saveRequired flag
                    return;
                }

                // save succeeded, so safe to clear saveRequired flag
                saveRequired = false;
            }

            close(false);
        }
        else if (cmd.equals(REFRESH)) {
            if (currView != null)
                currView.refresh();
        }
        else if (cmd.equals(DETAILS)) {
            if (detailsBrowser == null) {
                detailsBrowser = new DetailsBrowser(body, viewConfig);
                detailsBrowser.setQuestionInfoEnabled(currView == fullView);
            }

            detailsBrowser.setVisible(true);
        }
        else
            throw new IllegalArgumentException(cmd);
    }

    private boolean canInterruptTemplateCreation () {
        ContextManager cm = model.getContextManager();
        String wdTmpl = TemplateUtilities.getTemplatePath(model.getWorkDirectory());
        if (mainConfig.isTemplate() &&
                !cm.getFeatureManager().isEnabled(FeatureManager.WD_WITHOUT_TEMPLATE) &&
                wdTmpl == null) {
            return false;
        }
        return true;
    }

    private void copy(InterviewParameters from, InterviewParameters to)
        throws Interview.Fault
    {
        copy(from, to, true); // copy filename as well, by default
    }

    private void copy(InterviewParameters from, InterviewParameters to,
                      boolean copyFile)
        throws Interview.Fault
    {
        //System.err.println("CE.copy from " + (from==mainConfig?"main":from==viewConfig?"view":from.toString()) + " to " + (to==mainConfig?"main":to==viewConfig?"view":to.toString()));
        HashMap data = new HashMap();
        from.save(data);
        to.load(data, false);

        if (copyFile)
            to.setFile(from.getFile());
    }


    /**
    * Checks default settings relate to config file load fron the default location
    * @param cm <code>ContextManager</code> object defining current harness' context. The following methods
    *           affect this method functionality:
    * <ul>
    * <li><code>getDefaultConfigLoadPath()</code>
    * <li><code>getAllowConfigLoadOutsideDefault()</code>
    * </ul>
    * @throws <code>IllegalArgumentException</code> if the following configuration errors found:
    * <ul>
    * <li> <code>getDefaultConfigLoadPath()</code> returns <code>null</code> when <code>getAllowConfigLoadOutsideDefault()</code> returns <code>false</code>
    * <li> <code>getDefaultConfigLoadPath()</code> returns not absolute path
    * <li> <code>getDefaultConfigLoadPath()</code> returns a file (not a directory)
    * </ul>
    * @see ContextManager#setDefaultConfigLoadPath(java.io.File)
    * @see ContextManager#setAllowConfigLoadOutsideDefault(boolean state)
    * @see ContextManager#getDefaultConfigLoadPath()
    * @see ContextManager#getAllowConfigLoadOutsideDefault()
    */

    public static File checkLoadConfigFileDefaults(ContextManager cm) {
        if (cm == null)
            return null;

        File defaultConfigLoadPath = cm.getDefaultConfigLoadPath();
        boolean allowConfigLoadOutsideDefault = cm.getAllowConfigLoadOutsideDefault();

        if (defaultConfigLoadPath == null && !allowConfigLoadOutsideDefault)
            throw new IllegalArgumentException("Default directory not specified for " +
                "load operation when allowConfigLoadOutsideDefault is false");

        if (defaultConfigLoadPath != null) {
            if (!defaultConfigLoadPath.isAbsolute())
                throw new IllegalArgumentException("Relative paths not " +
                    "currently supported. The following setting is incorrect: " +
                    "\"" + defaultConfigLoadPath.getPath() + "\" selected for " +
                    "load operation");

            if (defaultConfigLoadPath.isFile())
                throw new IllegalArgumentException("Filename selected unexpectedly " +
                    "as a default directory: " +
                    "\"" + defaultConfigLoadPath.getPath() + "\" for " +
                    "load operation");
        }

        return defaultConfigLoadPath;
    }

    /**
    * Provides capabiltiies for configuration file loading. Method takes into
    * account context settings relating to default locations for configuration
    * files loading and behaves according to them.
    * @param cm <code>ContextManager</code> object defining current harness' context. The following methods
    *           affect this method functionality:
    * <li><code>getDefaultConfigLoadPath()</code>
    * <li><code>getAllowConfigLoadOutsideDefault()</code>
    * </ul>
    * @param parent A parent frame to be used for <code>fileChooser</code>/warning dialogs
    * @param uif The UIFactory used to for configuration file loading operation
    * @param fileChooser The <code>FileChooser</code> used for configuration file loading
    * @return The configuration file selected by user if this file loading is allowed by
    *         harness' contest settings
    * @see ContextManager#setDefaultConfigLoadPath(java.io.File)
    * @see ContextManager#setAllowConfigLoadOutsideDefault(boolean state)
    * @see ContextManager#getDefaultConfigLoadPath()
    * @see ContextManager#getAllowConfigLoadOutsideDefault()
    */

    static File loadConfigFile(ContextManager cm, Component parent, UIFactory uif, FileChooser fileChooser) {

        if (cm == null)
            return null;

        File defaultConfigLoadPath = checkLoadConfigFileDefaults(cm);
        boolean allowConfigLoadOutsideDefault = cm.getAllowConfigLoadOutsideDefault();

        File file = null;

        fileChooser.setDialogTitle(uif.getI18NString("ce.load.title"));

        if (defaultConfigLoadPath != null) {
            if (!allowConfigLoadOutsideDefault) {
                if (!(new File(defaultConfigLoadPath.getAbsolutePath())).canRead()) {
                    uif.showError("ce.load.defDirNotExists", defaultConfigLoadPath);
                    return null;
                }
                fileChooser.enableDirectories(false);
            } else
                fileChooser.enableDirectories(true);
            fileChooser.setCurrentDirectory(defaultConfigLoadPath);
        }

        boolean isMatch = true;

        while (file == null) {
            int rc = fileChooser.showDialog(parent, uif.getI18NString("ce.load.btn"));
            if (rc != JFileChooser.APPROVE_OPTION)
                return null;

            file = fileChooser.getSelectedFile();

            if (!allowConfigLoadOutsideDefault) {
                if (defaultConfigLoadPath == null)
                    return null;

                File f = new File(file.getAbsolutePath().substring(0, file.getAbsolutePath().lastIndexOf(File.separator)));

                try {
                    isMatch = (f.getCanonicalPath().indexOf((defaultConfigLoadPath.getCanonicalPath())) == 0);
                } catch ( IOException ioe) {
                    ioe.printStackTrace(System.err);
                    return null;
                }

                if (!isMatch) {
                    uif.showError("ce.load.notAllowedDir", defaultConfigLoadPath);


                    file = null;
                    fileChooser.setCurrentDirectory(defaultConfigLoadPath);
                    continue;  // choose another file
                }
            }
        }

        if (file != null) {
            String path = file.getPath();
            String ext = fileChooser.getChosenExtension();
            if (ext == null) {
                ext = JTI;
            }
            if (!path.endsWith(ext))
                file = new File(path + ext);
        }

        return file;
    }

    /**
    * Provides as the user with a dialog to chooser where to save a config. Method takes into account
    * context settings relating to default locations for configuration files saving and behaves
    * according to them.
    * @param cm <code>ContextManager</code> object defining current harness' context. The following methods
    *           affect this method functionality:
    * <ul>
    * <li><code>getDefaultConfigSavePath()</code>
    * <li><code>getAllowConfigSaveOutsideDefault()</code>
    * </ul>
    * @param parent A parent frame to be used for <code>fileChooser</code>/warning dialogs
    * @param uif The UIFactory used to for configuration file saving operation
    * @param fileChooser The <code>FileChooser</code> used for configuration file saving
    * @return The configuration file selected by user if this file saving is allowed by
    *         harness' contest settings
    * @throws <code>IllegalArgumentException</code> if the following configuration errors found:
    * <ul>
    * <li> <code>getDefaultConfigSavePath()</code> returns <code>null</code> when <code>getAllowConfigSaveOutsideDefault()</code> returns <code>false</code>
    * <li> <code>getDefaultConfigSavePath()</code> returns not absolute path
    * <li> <code>getDefaultConfigSavePath()</code> returns a file (not a directory)
    * </ul>
    * @see ContextManager#setDefaultConfigSavePath(java.io.File)
    * @see ContextManager#setAllowConfigSaveOutsideDefault(boolean state)
    * @see ContextManager#getDefaultConfigSavePath()
    * @see ContextManager#getAllowConfigSaveOutsideDefault()
    */

    static File saveConfigFile(ContextManager cm, Component parent, UIFactory uif, FileChooser fileChooser, File dir,
            boolean isTemplate) {
        if (cm == null)
            return null;

        File defaultSavePath;
        if (isTemplate) {
            defaultSavePath = cm.getDefaultTemplateSavePath();
        } else {
            defaultSavePath = cm.getDefaultConfigSavePath();
        }
        boolean allowSaveOutsideDefault;
        if (isTemplate) {
            allowSaveOutsideDefault = cm.getAllowTemplateSaveOutsideDefault();
        } else {
            allowSaveOutsideDefault = cm.getAllowConfigSaveOutsideDefault();
        }


        if (defaultSavePath == null && !allowSaveOutsideDefault)
            throw new IllegalArgumentException("Default directory not specified for " +
                "save operation when allowConfigSaveOutsideDefault is false");

        if (defaultSavePath != null) {
            if (!defaultSavePath.isAbsolute())
                throw new IllegalArgumentException("Relative paths not " +
                    "currently supported. The following setting is incorrect: " +
                    "\"" + defaultSavePath.getPath() + "\" selected for " +
                    "save operation");

            if (defaultSavePath.isFile())
                throw new IllegalArgumentException("Filename selected unexpectedly " +
                    "as a default directory: " +
                    "\"" + defaultSavePath.getPath() + "\" for " +
                    "save operation");

            if (!allowSaveOutsideDefault) {
                if (!defaultSavePath.canWrite()) {
                    uif.showError("ce.save.defDirNotExists", defaultSavePath);
                    return null;
                }
                fileChooser.enableDirectories(false);
            } else
                fileChooser.enableDirectories(true);

            fileChooser.setCurrentDirectory(defaultSavePath);
        } else
            if (dir != null)
                fileChooser.setCurrentDirectory(dir);

        File file = null;
        boolean isMatch = true;

        while (file == null) {
            int rc = fileChooser.showDialog(parent, uif.getI18NString("ce.save.btn"));
            if (rc != JFileChooser.APPROVE_OPTION)
                // user has canceled or closed the chooser
                return null;

            file = fileChooser.getSelectedFile();
            if (file == null) // just making sure
                continue;

            File f = new File(file.getAbsolutePath().substring(0, file.getAbsolutePath().lastIndexOf(File.separator)));

            if (!allowSaveOutsideDefault) {
                if (defaultSavePath == null)
                    return null;

                try {
                    isMatch = defaultSavePath.getCanonicalPath().equals(f.getCanonicalPath());
                } catch ( IOException ioe) {
                    ioe.printStackTrace(System.err);
                    return null;
                }

                if (!isMatch) {
                    uif.showError("ce.save.notAllowedDir", defaultSavePath);
                    file = null;
                    fileChooser.setCurrentDirectory(defaultSavePath);
                    continue;  // choose another file
                }
            }

            if (file.isDirectory()) {
                uif.showError("ce.save.fileIsDir", file);
                file = null;
                continue;  // choose another file
            }

            File parentFile = file.getParentFile();
            if (parentFile != null) {
                if (parentFile.exists() && !parentFile.isDirectory()) {
                    uif.showError("ce.save.parentNotADir", parentFile);
                    file = null;
                    continue;  // choose another file
                } else if (!parentFile.exists()) {
                    rc = uif.showYesNoDialog("ce.save.createParentDir",
                                             parentFile);
                    if (rc == JOptionPane.YES_OPTION) {
                        if (!parentFile.mkdirs()) {
                             uif.showError("ce.save.cantCreateParentDir",
                                           parentFile);
                             file = null;
                             continue;  // choose another file
                        }
                    } else {
                        file = null;
                        continue;  // choose another file
                    }
                }
            }

            // if file exists, leave well enough alone;
            // otherwise, make sure it ends with .jti or .jtm
            if (!file.exists()) {
                String path = file.getPath();
                String ext = fileChooser.getChosenExtension();
                if (ext != null && !path.endsWith(ext))
                    file = new File(path + ext);
            }

            // if file exists, make sure user wants to overwrite it
            if (file.exists()) {
                rc = uif.showYesNoDialog("ce.save.warn");
                switch (rc) {
                    case JOptionPane.YES_OPTION:
                        break;  // use this file

                    case JOptionPane.NO_OPTION:
                        fileChooser.setSelectedFile(null);
                        file = null;
                        continue;  // choose another file
                }
            }
        }
        return file;
    }

    void setAfterCloseCommand(Runnable runnable) {
        afterCloseCommand = runnable;
    }

    private Runnable afterCloseCommand;
    private boolean templateMode = false;

    private boolean runPending;

    private InterviewParameters mainConfig;
    private InterviewParameters viewConfig;
    private FileHistory history;
    private FileHistory historyTemplate;
    private boolean saveRequired;

    private JMenu markerMenu;
    private JMenu searchMenu;
    private JMenu viewMenu;
    private JRadioButtonMenuItem viewFullBtn;
    private JRadioButtonMenuItem viewStdBtn;
    private JCheckBoxMenuItem viewTagCheckBox;
    private JMenuItem viewRefreshItem;
    private JComponent body;
    private JPanel views;
    private CE_FullView fullView;
    private CE_StdView stdView;
    private CE_View currView;
    private Listener listener;
    //private TemplatesUI templatesUI;

    private Map customRenderersMap;
    private CE_TemplateDialog templateDialog;
    private ActionListener closeListener;
    private ExecModel model;
    private Observer observer;

    private DetailsBrowser detailsBrowser;
    private static final KeyStroke detailsKey = KeyStroke.getKeyStroke("shift alt D");

    // XXX this isn't the right class to define these in
    //     do not make more public than package private
    static final String JTI = ".jti";
    static final String TEMPLATE_EXTENSION = ".jtm";
    static final String JTM = TEMPLATE_EXTENSION;

    private static final String NEW = "new";
    private static final String LOAD = "load";
    private static final String NEWT = "newt";
    private static final String LOADT = "loadt";
    private static final String SAVE = "save";
    private static final String SAVE_AS = "saveAs";
    private static final String SAVE_AS_TEMPLATE = "saveAsTemplate";
    private static final String REVERT = "revert";
    private static final String DONE = "done";
    private static final String REFRESH = "refresh";
    private static final String DETAILS = "details";
            static final String CLOSE = "close";

    static final String MORE_INFO_PREF = "exec.config.moreInfo";
    static final String VIEW_PREF = "exec.config.view";

    public void setCustomRenderers(Map renderersMap) {
        customRenderersMap = renderersMap;
    }

    private class Listener
        implements ActionListener, ChangeListener, MenuListener
    {
        // ---------- from ActionListener -----------

        public void actionPerformed(ActionEvent e) {
            Object src = e.getSource();
            if (src instanceof JMenuItem) {
                JMenuItem mi = (JMenuItem) src;
                File f = (File) (mi.getClientProperty(FileHistory.FILE));
                if (f != null) {
                    loadNoUI(f);
                    return;
                }
            }
            perform(e.getActionCommand());
        }

        // ---------- from ChangeListener -----------

        public void stateChanged(ChangeEvent e) {
            Object src = e.getSource();
            if (src == viewTagCheckBox)
                fullView.setTagVisible(viewTagCheckBox.isSelected());
        }

        // ---------- from MenuListener -----------

        public void menuSelected(MenuEvent e) {
            Object src = e.getSource();
            if (src == viewMenu)
                viewTagCheckBox.setSelected(fullView.isTagVisible());
        }

        public void menuDeselected(MenuEvent e) {
        }

        public void menuCanceled(MenuEvent e) {
        }

    };

    /**
     * For private communication with ConfigHandler, not for broadcast outside
     * of core JT.
     */
    interface Observer {
        // right now, these mean that this now the current config/filename
        public void loaded(InterviewParameters p);
        public void saved(InterviewParameters p);
    }
}
