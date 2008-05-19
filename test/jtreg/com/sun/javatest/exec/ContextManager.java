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

import java.io.File;
import java.util.Map;
import java.util.HashMap;
import java.awt.EventQueue;

import com.sun.javatest.InterviewParameters;
import com.sun.javatest.TestResultTable;
import com.sun.javatest.TestSuite;
import com.sun.javatest.WorkDirectory;
import com.sun.javatest.report.CustomReport;
import com.sun.javatest.report.Report;
import com.sun.javatest.tool.ToolDialog;
import com.sun.javatest.tool.UIFactory;
import com.sun.javatest.util.I18NResourceBundle;
import com.sun.interview.Question;
import com.sun.interview.wizard.QuestionRenderer;


public class ContextManager implements InterviewParameters.TemplateManager,
        Report.CustomReportManager {

    // GUI CUSTOMIZATION

    /**
     * Get the custom menu manager for this Test Manager instance.
     * @return The custom menu manager.  If null, it can be assumed that there
     *         are no custom menus.
     */
    public JavaTestMenuManager getMenuManager() {
        return null;
    }

    public ToolBarManager getToolBarManager() {
        return new ToolBarManager();
    }

    /**
     * Get the context (popup) custom menus to be added in the GUI.  This
     * method is only called when the GUI is initialized, so the value should
     * not change after its first invocation.  Any state changes (enable,
     * disable, hide, text changes) with the menu items should occur inside the
     * JavaTestContextMenu instances, not be adding and removing them from the
     * array returned by this method.
     * @return The menus to be added.  Null if there are no custom menus (the
     *         default).
     * @see JavaTestContextMenu
     */
    public JavaTestContextMenu[] getContextMenus() {
        return null;
    }

    /**
     * Get custom report types.
     * @return Null if no custom types are requested.
     */
    public CustomReport[] getCustomReports() {
        return null;
    }

    /**
     * Get the context custom test result viewers to be added in the GUI.  This
     * method is only called when the GUI is initialized, so the value should
     * not change after its first invocation.
     * @return The menus to be added.  Null if there are no custom viewers (the
     *         default).
     * @see CustomTestResultViewer
     */
    public CustomTestResultViewer[] getCustomResultViewers() {
        return null;
    }


    /**
     * Get the active test suite.
     * @return The current test suite.
     */
    public TestSuite getTestSuite() {
        return testSuite;
    }


    /**
     * Get the active work directory.
     * @return The current work directory, null if it has not been set.
     */
    public WorkDirectory getWorkDirectory() {
        return workdir;
    }

    // INTERVIEW ACCESS
    /**
     * Get the active interview, for query or modification.
     * Note that there is currently no API support for "locking" the interview, which
     * means that multiple parts of the system could work against each other.  The main
     * risk is test-suite specific code competing with the Configuration Editor.
     * @return The active interview instance.
     */
    public InterviewParameters getInterview() {
        return interview;
    }

    /**
     * Request that the harness reload the test suite structure from the
     * test suite.  If called on the GUI event thread, it will start a new
     * thread before executing the operation, to avoid blocking the GUI.
     * It is recommended that the caller use a different thread and probably
     * show the user a "Please wait" message until this method returns.
     */
    public void refreshTests() {
        synchronized (this) {
            if (pendingRefresh)
                return;

            pendingRefresh = true;
        }   // sync

        // start the task on a new thread if needed to release
        // the GUI
        if (EventQueue.isDispatchThread()) {
            Runnable cmd = new Runnable() {
                public void run() {
                    refreshTestsImpl();
                }   // run()
            };
            Thread t = new Thread(cmd, "ContextMgr Refresh Defer");
            t.start();
            return;
        }

        // this will occur if the caller already put this call on
        // a different thread
        //refreshTests0();

        try {
            Thread.currentThread().sleep(4000);
        }
        catch (InterruptedException e) {
        }

        refreshTestsImpl();
    }

    /**
     * Method to be called, not on the event thread.
     * We have this method because we cannot guarantee which thread
     * <code>refreshTests()</code> will be called on.  The function
     * of this method is to shove the operation to the back of the
     * GUI event queue, then run it for real.
     */
     /*
    private void refreshTests0() {
        Runnable cmd = new Runnable() {
            public void run() {
                // now do the task for real on a background thread
                Runnable cmd2 = new Runnable() {
                    public void run() {
                        refreshTestsImpl();
                    }   // run()
                };
                Thread t = new Thread(cmd2, "ContextMgr Refresh");
                t.start();
                return;
            }   // run()
        };

        // this shoves the request to the end of the event queue
        // unfortunately necessary to allow any GUI work to complete
        // before executing the operation.
        // effectively, we would like to defer the operation until as
        // late as possible to avoid conflicts with the rest of the
        // system at startup, which would decrease performance
        EventQueue.invokeLater(cmd);
    }
    */


    private void refreshTestsImpl() {
        synchronized (this) {
            pendingRefresh = false;
        }

        if (parentTool != null) {
            TestResultTable trt = parentTool.getActiveTestResultTable();
            if (trt != null)
                try {
                    parentTool.pauseTreeCacheWork();
                    // refresh entire tree
                    //trt.refreshIfNeeded(trt.getRoot());

                    // use more minimal refresh - need to check functionality in
                    // batch mode
                    // XXX need to make access indirect, TRT_TreeNode should not be
                    //     public
                    ( (com.sun.javatest.TRT_TreeNode)(trt.getRoot()) ).refreshIfNeeded();
                }
                //catch (TestResultTable.Fault f) {
                    // ignore?  log?
                //}
                finally {
                    parentTool.unpauseTreeCacheWork();
                }   // finally
        }
    }

    /**
     * Write the active interview to disk if possible.
     * For this to work, getInterview() must be non-null.  This also implies that
     * there is a test suite and work directory selected already.
     * @throws IllegalStateException if there is no interview available.
     */
    public void syncInterview() {
        parentTool.syncInterview();
    }


    /**
     * Get feature manager from this ContextManager instance.
     * @return current feature manager
     */
    public FeatureManager getFeatureManager() {
        return featureManager;
    }

    /**
     * Set given feature manager for this ContextManager instance.
     * @param featureManager new feature manager
     */
    public void setFeatureManager(FeatureManager featureManager) {
        this.featureManager = featureManager;
    }

    // TEMPLATE SAVING BEHAVIOUR

    /**
     * This method is invoked each time before saving template.
     * The template will be saved only if this method returns true.
     * The default implementation always returns true.
     * @param file template file
     * @return true if this operation is allowed, false otherwise
     */
    public boolean canSaveTemplate(File file) {
        return true;
    }

    // WORK DIRECTORY OPTIONS
    /**
     * Default path presented to user when they are prompted to create
     * a work directory.  This method does not imply any requirement that
     * the user actually load/save the workdir in the given location.
     * @param dir The initial directory where workdirs should be loaded/saved
     *            to.
     * @throws NullPointerException if the parameter given is null.
     * @see #getDefaultWorkDirPath()
     */
    public void setDefaultWorkDirPath(File dir) {
        if (dir == null)
            throw new NullPointerException();
        this.wdPath = dir;
    }

    /**
     * Get the default path for work directory.
     * @return The initial directory to load and create work directories.
     * @see  #setDefaultWorkDirPath(File)
     */
    public File getDefaultWorkDirPath() {
        return this.wdPath;
    }

    // TEMPLATE OPTIONS
    /**
     * Set the default path from which template files are loaded.
     * Does not imply a requirement that the template be loaded from that
     * location.
     * @param dir The initial directory where template files should be
     *        loaded from.
     * @throws NullPointerException if the parameter given is null.
     * @see #getDefaultTemplateLoadPath
     * @see #setAllowTemplateLoadOutsideDefault
     */
    public void setDefaultTemplateLoadPath(File dir) {
        if (dir == null)
            throw new NullPointerException();
        templateLoadPath = dir;
    }

    /**
     * Get the default path from which template files are loaded.
     * @return The initial directory where template files should be
     *         loaded from.  Null if not set.
     * @see #setDefaultTemplateLoadPath
     * @see #setAllowTemplateLoadOutsideDefault
     */
    public File getDefaultTemplateLoadPath() {
        return templateLoadPath;
    }

    /**
     * Set the default path to which template files are saved.
     * Does not imply a requirement that the template must be saved to that
     * location.
     * @param dir The initial directory where template should be saved
     *            to.
     * @throws NullPointerException if the parameter given is null.
     * @see #getDefaultTemplateLoadPath
     * @see #setAllowTemplateLoadOutsideDefault
     */
    public void setDefaultTemplateSavePath(File dir) {
        if (dir == null)
            throw new NullPointerException();
        templateSavePath = dir;
    }

    /**
     * Get the default path to which template files are saved.
     * @return The initial directory where template files should be
     *         saved to.  Null if not set.
     * @see #setDefaultTemplateSavePath(File)
     * @see #setAllowTemplateSaveOutsideDefault(boolean)
     */
    public File getDefaultTemplateSavePath() {
        return templateSavePath;
    }

    /**
     * Set ability to load templates outside default directory.
     * @param state new state
     * @see #getAllowTemplateLoadOutsideDefault()
     */
    public void setAllowTemplateLoadOutsideDefault(boolean state) {
        templateLoadOutside = state;
    }

    /**
     * Get ability to load templates outside default directory
     * @return true if the loading outside default directory is allowed or false otherwise
     * @see #setAllowTemplateLoadOutsideDefault(boolean)
     */
    public boolean getAllowTemplateLoadOutsideDefault() {
        return templateLoadOutside;
    }

    /**
     * Set ability to save templates outside default directory.
     * @param state new state
     * @see #getAllowTemplateSaveOutsideDefault()
     */
    public void setAllowTemplateSaveOutsideDefault(boolean state) {
        templateSaveOutside = state;
    }

    /**
     * Get ability to save templates outside default directory
     * @return true if the saving outside default directory is allowed or false otherwise
     * @see #setAllowTemplateSaveOutsideDefault(boolean)
     */
    public boolean getAllowTemplateSaveOutsideDefault() {
        return templateSaveOutside;
    }

    // CONFIGURATION FILE OPTIONS
    /**
     * Set the default path from which configuration files are loaded.
     * Does not imply a requirement that the config be loaded from that
     * location.
     * @param dir The initial directory where configuration files should be
     *        loaded from.
     * @throws NullPointerException if the parameter given is null.
     * @see #getDefaultConfigLoadPath
     * @see #setAllowConfigLoadOutsideDefault
     */
    public void setDefaultConfigLoadPath(File dir) {
        if (dir == null)
            throw new NullPointerException();
        configLoadPath = dir;
    }

    /**
     * Get the default path from which configuration files are loaded.
     * @return The initial directory where configuration files should be
     *         loaded from.  Null if not set.
     * @see #setDefaultConfigLoadPath
     * @see #setAllowConfigLoadOutsideDefault
     */
    public File getDefaultConfigLoadPath() {
        return configLoadPath;
    }

    /**
     * Set the default path to which configuration files are saved.
     * Does not imply a requirement that the config must be saved to that
     * location.
     * @param dir The initial directory where workdirs should be saved
     *            to.
     * @throws NullPointerException if the parameter given is null.
     * @see #getDefaultConfigLoadPath
     * @see #setAllowConfigLoadOutsideDefault
     */
    public void setDefaultConfigSavePath(File dir) {
        if (dir == null)
            throw new NullPointerException();

        // verify that it exists?
        // verify that it is a dir?
        configSavePath = dir;
    }

    /**
     * Get the default path from which configuration files are loaded.
     * @return The initial directory where configuration files should be
     *         loaded from.  Null if not set.
     * @see #setDefaultConfigSavePath(File)
     * @see #setAllowConfigSaveOutsideDefault(boolean)
     */
    public File getDefaultConfigSavePath() {
        return configSavePath;
    }

    /**
     * Set ability to load config outside default directory.
     * @param state new state
     * @see #getAllowConfigLoadOutsideDefault()
     */
    public void setAllowConfigLoadOutsideDefault(boolean state) {
        configLoadOutside = state;
    }

    /**
     * Get ability to load config outside default directory
     * @return true if the loading outside default directory is allowed or false otherwise
     * @see #setAllowConfigLoadOutsideDefault(boolean)
     */
    public boolean getAllowConfigLoadOutsideDefault() {
        return configLoadOutside;
    }

    /**
     * Set ability to save config outside default directory.
     * @param state new state
     * @see #getAllowConfigSaveOutsideDefault()
     */
    public void setAllowConfigSaveOutsideDefault(boolean state) {
        configSaveOutside = state;
    }

    /**
     * Get ability to load config outside default directory
     * @return true if the saving outside default directory is allowed or false otherwise
     * @see #setAllowConfigLoadOutsideDefault(boolean)
     */
    public boolean getAllowConfigSaveOutsideDefault() {
        return configSaveOutside;
    }



    public void loadConfiguration(File file) {
        parentTool.loadInterview(file);
    }

    /**
     * @deprecated use #setWorkDirectory(WorkDirectory) instead
     * @see #setWorkDirectory(WorkDirectory)
     */
    protected void setWorkDir(WorkDirectory w) {
        setWorkDirectory(w);
    }

    protected void setWorkDirectory(WorkDirectory w) {
        workdir = w;
    }

    protected void setTestSuite(TestSuite ts) {
        testSuite = ts;
    }

    Map getCustomRenderersMap() {
        return customRenderers;
    }

    /**
     * Register custom config editor's question renderer for specified question class.
     * It is better to register custom renderer BEFORE Configuration Editor is constructed,
     * for example in ContextManager's constructor.
     * @param question Question's class
     * @param renderer Custom question renderer fot this question
     */
    protected void registerCustomQuestionRenderer(Class<? extends Question> question,
                                                  QuestionRenderer renderer) {
        if (customRenderers == null) {
            customRenderers = new HashMap();
        }
        customRenderers.put(question, renderer);
    }

    /**
     * Special class for creating dialogs which should be attached to the
     * context of this test manager.  It is important to use this class
     * to assure proper component parenting, and because the system will
     * properly track your dialogs if the user changes the desktop style
     * (switching to internal frames interface for example).
     */
    public abstract static class TestManagerDialog extends ToolDialog {
        /**
         * @param context The context object associated with this dialog,
         *        this parameter is required so that the proper parenting
         *        can be calculated.
         * @param uif The interface factory associated with this dialog.
         * @param key Resource key to be used when retrieving items from the
         *        uif.
         */
        public TestManagerDialog(ContextManager context, UIFactory uif,
                                 String key) {
            super(context.parentTool, uif, key);
        }
    }

    // ------------------------ PRIVATE -----------------------------

    void setInterview(InterviewParameters i) {
        interview = i;
    }

    void setTool(ExecTool t) {
        parentTool = t;
    }


    protected File configLoadPath;
    protected File configSavePath;

    protected boolean configLoadOutside = true;
    protected boolean configSaveOutside = true;

    protected File templateLoadPath;
    protected File templateSavePath;

    protected boolean templateLoadOutside = true;
    protected boolean templateSaveOutside = true;

    protected File wdPath;


    protected FeatureManager featureManager = new FeatureManager();

    private WorkDirectory workdir;
    private TestSuite testSuite;
    private InterviewParameters interview;
    private ExecTool parentTool;
    private Map customRenderers;

    private volatile boolean pendingRefresh = false;

    private static I18NResourceBundle i18n = I18NResourceBundle.getBundleForClass(ContextManager.class);
}
