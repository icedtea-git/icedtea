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
package com.sun.javatest.tool;

import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.EventQueue;
import java.awt.Rectangle;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.StringWriter;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.net.URL;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Properties;
import java.util.Set;
import java.util.Vector;
import javax.swing.BorderFactory;
import javax.swing.Icon;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JDialog;
import javax.swing.JEditorPane;
import javax.swing.JFrame;
import javax.swing.JMenuBar;
import javax.swing.JOptionPane;
import javax.swing.JTextArea;
import javax.swing.KeyStroke;
import javax.swing.Timer;
import javax.swing.ToolTipManager;
import javax.swing.UIManager;

import com.sun.javatest.InterviewParameters;
import com.sun.javatest.JavaTestError;
import com.sun.javatest.TestSuite;
import com.sun.javatest.util.BackupPolicy;
import com.sun.javatest.util.HTMLWriter;
import com.sun.javatest.util.I18NResourceBundle;
import com.sun.javatest.util.LogFile;
import com.sun.javatest.util.SortedProperties;
import java.awt.print.Printable;
import java.awt.print.PrinterJob;
import javax.print.Doc;
import javax.print.DocFlavor;
import javax.print.DocPrintJob;
import javax.print.PrintException;
import javax.print.PrintService;
import javax.print.PrintServiceLookup;
import javax.print.ServiceUI;
import javax.print.SimpleDoc;
import javax.print.attribute.HashPrintRequestAttributeSet;
import javax.print.attribute.PrintRequestAttributeSet;

/**
 * Desktop is the host for a series of Tools,
 * which may be displayed as in a number of styles,
 * provided by a DeskView.
 * <p>Much of the functionality of a desktop is provided by the current view,
 * and because of that, many of the methods here simply call through to the
 * underlying current view object.
 * @see DeskView
 */
public class Desktop
{
    /**
     * Create a desktop using a style determined according to the
     * user's preferences.
     */
    public Desktop() {
        this(getPreferredStyle());
    }

    /**
     * New desktop, using preferrred style and given context.
     */
    public Desktop(CommandContext ctx) {
        this(getPreferredStyle(), ctx);
    }


    public Desktop(int style, CommandContext ctx) {
        commandContext = ctx;

        String val = preferences.getPreference(TTIP_PREF);
        boolean t = (val == null || val.equalsIgnoreCase("true"));
        setTooltipsEnabled(t);

        int delay = getTooltipDelay(preferences);
        setTooltipDelay(delay);

        int duration = getTooltipDuration(preferences);
        setTooltipDuration(duration);

        String soe = preferences.getPreference(SAVE_ON_EXIT_PREF);
        setSaveOnExit(soe == null || soe.equalsIgnoreCase("true"));

        File f = getDesktopFile();
        firstTime = !(f != null && f.exists());

        if (System.getProperty("systemLAF") != null) {
            try {
                UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
            } catch(Exception e) {
                System.out.println("Error setting native LAF: " + e);
            }
        }

        uif = new UIFactory(getClass());

        initToolManagers();

        /* defer view initialization in case we do a restore
        initView(style);
        */
        this.style = style;
    }

    /**
     * Create a desktop using a specified style.
     * @param style a value indicating the desired desktop style.
     * @see #MDI_STYLE
     * @see #SDI_STYLE
     * @see #TAB_STYLE
     */
    public Desktop(int style) {
        this(style, null);
    }

    /**
     * Get a value indicating the current style of the desktop.
     * @return a value indicating the current style of the desktop
     * @see #setStyle
     * @see #MDI_STYLE
     * @see #SDI_STYLE
     * @see #TAB_STYLE
     */
    public int getStyle() {
        return (currView == null ? style : currView.getStyle());
    }

    /**
     * Get a value indicating the user's preferred desktop style,
     * as recorded in the user's preferences.
     * @return a value indicating the user's preferred desktop style
     * @see #MDI_STYLE
     * @see #SDI_STYLE
     * @see #TAB_STYLE
     */
    public static int getPreferredStyle() {
        // would be better(?) to use classname, perhaps
        String prefStyleName = preferences.getPreference(STYLE_PREF);
        int i = indexOf(prefStyleName, styleNames);
        return (i != -1 ? i : TAB_STYLE);
    }

    /**
     * Set the current style of the desktop.
     * @param style a value indicating the current style of the desktop
     * @see #getStyle
     * @see #MDI_STYLE
     * @see #SDI_STYLE
     * @see #TAB_STYLE
     */
    public void setStyle(int style) {
        //System.err.println("Desktop.setStyle: " + style);
        if (style == getStyle())
            return;

        if (currView == null) {
            this.style = style;
            return;
        }

        DeskView oldView = currView;
        //System.err.println("DT: creating new desktop (" + style + ")");
        switch (style) {
        case MDI_STYLE:
            currView = new MDIDeskView(oldView);
            break;

        case SDI_STYLE:
            currView = new SDIDeskView(oldView);
            break;

        case TAB_STYLE:
            currView = new TabDeskView(oldView);
            break;

        default:
            throw new IllegalArgumentException();
        }

        //System.err.println("DT: disposing old deskview");
        oldView.dispose();

        //System.err.println("DT: setStyle done ");
    }

    /**
     * Determine if this is the first time that JT Harness has been run.
     * This is determined by checking if a saved desktop exists from
     * a prior run of JT Harness.
     * @return true if this appears to be the first time the user has
     * run JT Harness, and false otherwise
     */
    public boolean isFirstTime() {
        return firstTime;
    }

    /**
     * Set the flag indicating whether or not this is the first time
     * that JT Harness has been run.
     * @param b true if JT Harness should behave as though this is th
     * first time JT Harness has been run
     * @see #isFirstTime
     */
    public void setFirstTime(boolean b) {
        firstTime = b;
    }

    /**
     * Check whether the desktop is empty of any tools.
     * @return true if there are no tools on the desktop, and false otherwise
     */
    public boolean isEmpty() {
        return (currView == null ? true : currView.isEmpty());
    }

    /**
     * Get the set of tools currently on the desktop.
     * @return the set of tools currently on the desktop
     */
    public Tool[] getTools() {
        return (currView == null ? new Tool[0] : currView.getTools());
    }

    /**
     * Add a new tool to the desktop.
     * @param t the tool to be added
     * @see #removeTool
     */
    public void addTool(Tool t) {
        ensureViewInitialized();
        currView.addTool(t);
    }

    /**
     * Remove a tool from the desktop.
     * @param t the tool to be removed
     * @see #addTool
     */
    public void removeTool(Tool t) {
        if (currView != null)
            currView.removeTool(t);
    }

    /**
     * Get the currently selected tool on the desktop.
     * @return the currently selected tool on the desktop
     * @see #setSelectedTool
     */
    public Tool getSelectedTool() {
        return (currView == null ? null : currView.getSelectedTool());
    }

    /**
     * Set the currently selected tool on the desktop.
     * @param t the the tool to be selected on the desktop
     * @see #getSelectedTool
     */
    public void setSelectedTool(Tool t) {
        ensureViewInitialized();
        currView.setSelectedTool(t);
    }

    /**
     * Add a new default tool to the desktop. The default can be set via the
     * system property "javatest.desktop.defaultTool", which should identify
     * the class name of an appropriate tool manager; if not set, the default
     * is com.sun.javatest.exec.ExecManager.
     * @see #removeTool
     */
    public void addDefaultTool() {
        if (!EventQueue.isDispatchThread()) {
            invokeOnEventThread(new Runnable() {
                    public void run() {
                        addDefaultTool();
                    }
                });
            return;
        }

        for (int i = 0; i < toolManagers.length; i++) {
            ToolManager m = toolManagers[i];
            if (m.getClass().getName().equals(defaultToolManager)) {
                m.startTool();
                return;
            }
        }
    }


    /**
     * Add a new default tool to the desktop. The default can be set via the
     * system property "javatest.desktop.defaultTool", which should identify
     * the class name of an appropriate tool manager; if not set, the default
     * is com.sun.javatest.exec.ExecManager.
     * @param ip a configuration to be passed to the default tool manager's startTool
     * method
     * @see #removeTool
     */
    public void addDefaultTool(InterviewParameters ip) {
        for (int i = 0; i < toolManagers.length; i++) {
            ToolManager mgr = toolManagers[i];
            if (mgr.getClass().getName().equals(defaultToolManager)) {
                try {
                    // this is to avoid a class dependency to exec package, which is
                    // normally not allowed in this package
                    Method m = mgr.getClass().getMethod("startTool",
                                            new Class[] { InterviewParameters.class} );
                    m.invoke(mgr, new Object[] { ip });
                    return;
                }
                catch (NoSuchMethodException e) {
                    // ignore??
                }
                catch (IllegalAccessException e) {
                    // ignore??
                }
                catch (InvocationTargetException e) {
                    // ignore??
                }
            }
        }
    }

    /**
     * Check if a tool is present on the desktop.
     * @param t the tool for which to check
     * @return true if the specified tool exists on the desktop, and false otherwise
     */
    public boolean containsTool(Tool t) {
        Tool[] tools = getTools();
        for (int i = 0; i < tools.length; i++) {
            if (t == tools[i])
                return true;
        }
        return false;
    }

    /**
     * Get the set of tool managers associated with this desktop.
     * The managers are determined from resource files named
     * "JavaTest.toolMgrs.lst" on the main JT Harness classpath.
     * @return the set of tool managers associated with this desktop
     */
    public ToolManager[] getToolManagers() {
        return toolManagers;
    }

    /**
     * Get the instance of a tool manager for this desktop of a specific class.
     * @param c the class of the desired tool manager.
     * @return a tool manager of the desired type, or null if none found
     */
    public ToolManager getToolManager(Class c) {
        for (int i = 0; i < toolManagers.length; i++) {
            ToolManager m = toolManagers[i];
            if (c.isInstance(m))
                return m;
        }
        return null;
    }

    /**
     * Get the instance of a tool manager for this desktop of a specific class.
     * @param className the name of the class of the desired tool manager.
     * @return a tool manager of the desired type, or null if none found
     */
    public ToolManager getToolManager(String className) {
        for (int i = 0; i < toolManagers.length; i++) {
            ToolManager m = toolManagers[i];
            if (m.getClass().getName().equals(className))
                return m;
        }
        return null;
    }

    /**
     * Get the top level frames that make up this desktop. TAB and MDI style
     * desktops just have a single frame; An SDI style desktop may have more
     * than one frame.
     * @return the top level frames of this desktop
     */
    public JFrame[] getFrames() {
        ensureViewInitialized();
        return currView.getFrames();
    }

    /**
     * Get a parent component for a dialog to use.
     * @return Component which can be used as a parent, or null if none
     *         is available.
     */
    public Component getDialogParent() {
        ensureViewInitialized();
        return currView.getDialogParent();
    }

    /**
     * Add a file and a corresponding file opener to the file history
     * that appears on the File menu.
     * @param f The file to be added
     * @param fo A FileOpener object to be used to open the file if necessary
     */
    public void addToFileHistory(File f, FileOpener fo) {
        // if it is already in the history, remove it
        for (Iterator i = fileHistory.iterator(); i.hasNext(); ) {
            FileHistoryEntry h = (FileHistoryEntry) (i.next());
            if (h.fileOpener == fo && h.file.equals(f)) {
                i.remove();
                break;
            }
        }

        // add it to the front of the list
        fileHistory.addFirst(new FileHistoryEntry(fo, f));

        // throw away old entries in the list
        while (fileHistory.size() > FILE_HISTORY_MAX_SIZE)
            fileHistory.removeLast();
    }

    /**
     * Get a list of the current entries on the file history associated with this desktop.
     * @return  a list of the current entries on the file history associated with this desktop
     * @see #addToFileHistory
     */
    List getFileHistory()
    {
        return fileHistory;
    }

    /**
     * Check if the top level windows of the desktop are visible or not.
     * @return true if the top level windows are visible; otherwise, return false
     * @see #setVisible
     */
    public boolean isVisible() {
        return (currView == null ? false : currView.isVisible());
    }

    /**
     * Set whether or not the top level windows of the desktop should be visible.
     * @param b If true, the top level windows will be made visible; if false, they
     * will be hidden.
     */
    public void setVisible(final boolean b) {
        if (!EventQueue.isDispatchThread()) {
            invokeOnEventThread(new Runnable() {
                    public void run() {
                        setVisible(b);
                    }
                });
            return;
        }

        ensureViewInitialized();
        currView.setVisible(b);
    }

    /**
     * Create a dialog.
     * @param tool the parent tool for the dialog
     * @param uiKey a string which is to be used as the base name for any
     * resources that may be required
     * @param title the title for the dialog
     * @param menuBar the menu bar for the dialog
     * @param body the body component for the dialog
     * @param bounds the size and position for the dialog
     * @return a JDialog or JInternalDialog built from the supplied values.
     */
    public Container createDialog(Tool tool, String uiKey, String title,
                                           JMenuBar menuBar, Container body,
                                           Rectangle bounds)
    {
        ensureViewInitialized();
        return currView.createDialog(tool, uiKey, title, menuBar, body, bounds);
    }

    /**
     * Check if the tool's parent Window is the owner of a dialog.
     * This may become false if the desktop style is changed after the dialog
     * was created.
     * @param tool the tool from which to determine the parent Window
     * @param dialog the dialog to be checked
     * @return true if the tool's parent Window is the owner of the dialog, and
     * false otherwise.
     */
    public boolean isToolOwnerForDialog(Tool tool, Container dialog) {
        ensureViewInitialized();
        return currView.isToolOwnerForDialog(tool, dialog);
    }

    /**
     * Check all the tools on the desktop to see if they have open state
     * that should be saved or processes running. If there is open state
     * or active processes, a confirmation dialog will be displayed.
     * If the user confirms OK, or if there was no need to show the
     * confirmation dialog, the desktop will be saved and disposed.
     * @param parent A parent frame to be used if a confirmation dialog
     * is necessary
     * @see #isOKToExit
     */
    public void checkToolsAndExitIfOK(JFrame parent) {
        if (isOKToExit(parent)) {
            boolean saveOnExit = getSaveOnExit();
            if (saveOnExit)
                save();
            else {
                File f = getDesktopFile();
                if (f != null && f.exists())
                    f.delete();
            }

            dispose();
        }
    }

    /**
     * Check if it is OK to close a tool. If the tool has important
     * state that needs to be saved, or any processes running, a confirmation
     * dialog will be shown, to allow the user to cancel the operation if
     * necessary.
     * @param t The tool to be checked
     * @param parent A parent frame to be used if a confirmation dialog
     * is necessary
     * @return true if it is OK to close the tool
     */
    public boolean isOKToClose(Tool t, JFrame parent) {
        if (confirmDialog != null) {
            Toolkit.getDefaultToolkit().beep();
            confirmDialog.toFront();
            return false;
        }

        String[] alerts = t.getCloseAlerts();
        if (alerts == null || alerts.length == 0)
            return true;
        else
            return isOKToExitOrClose(parent, alerts, CLOSE);
    }

    /**
     * Check if it is OK to close all tools and exit the desktop.
     * If any tools have important state that needs to be saved, or active tasks
     * running, a confirmation dialog will be shown to allow the user to
     * cancel the operation in progress.
     * @param parent A parent frame to be used if a confirmation dialog
     * is necessary
     * @return true if it is OK to exit the desktop, and false otherwise.
     */
    public boolean isOKToExit(JFrame parent) {
        if (confirmDialog != null) {
            Toolkit.getDefaultToolkit().beep();
            confirmDialog.toFront();
            return false;
        }

        Vector v = new Vector();

        Tool[] tools = getTools();
        for (int ti = 0; ti < tools.length; ti++) {
            String[] alerts = tools[ti].getCloseAlerts();
            if (alerts != null)
                v.addAll(Arrays.asList(alerts));
        }

        if (v.size() == 0)
            return true;
        else {
            String[] allAlerts = new String[v.size()];
            v.copyInto(allAlerts);
            return isOKToExitOrClose(parent, allAlerts, EXIT);
        }
    }

    private static final int CLOSE = 0;
    private static final int EXIT = 1;
    private JDialog confirmDialog;

    private boolean isOKToExitOrClose(JFrame parent, String[] alerts, int mode) {
        if (confirmDialog != null) {
            Toolkit.getDefaultToolkit().beep();
            confirmDialog.toFront();
            return false;
        }

        Integer m = new Integer(mode);

        if (alerts.length > 0) {
            // protect against reentrant calls by setting confirmDialog
            // while showing it

            StringWriter sw = new StringWriter();
            try {
                HTMLWriter out = new HTMLWriter(sw, uif.getI18NResourceBundle());
                out.startTag(HTMLWriter.HTML);
                out.startTag(HTMLWriter.BODY);
                out.writeStyleAttr("font-family: SansSerif");
                out.startTag(HTMLWriter.P);
                out.writeStyleAttr("margin-top:0");
                out.startTag(HTMLWriter.B);
                out.writeI18N("dt.confirm.head", m);
                out.endTag(HTMLWriter.B);
                out.endTag(HTMLWriter.P);
                out.startTag(HTMLWriter.P);
                out.startTag(HTMLWriter.I);
                out.writeI18N("dt.confirm.warn", m);
                out.endTag(HTMLWriter.I);
                out.endTag(HTMLWriter.P);
                out.startTag(HTMLWriter.UL);
                out.writeStyleAttr("margin-top:0; margin-bottom:0; margin-left:30");
                for (int i = 0; i < alerts.length; i++) {
                    out.startTag(HTMLWriter.LI);
                    out.write(alerts[i]);
                }
                out.endTag(HTMLWriter.UL);
                out.startTag(HTMLWriter.P);
                out.writeStyleAttr("margin-top:5");
                out.writeI18N("dt.confirm.warn2", m);
                out.endTag(HTMLWriter.P);
                out.startTag(HTMLWriter.P);
                out.writeStyleAttr("margin-bottom:0");
                out.writeI18N("dt.confirm.tail", m);
                out.endTag(HTMLWriter.P);
                out.endTag(HTMLWriter.BODY);
                out.endTag(HTMLWriter.HTML);
                out.close();
            }
            catch (IOException e) {
                JavaTestError.unexpectedException(e);
            }

            JEditorPane body = new JEditorPane();
            body.setOpaque(false);
            body.setContentType("text/html");
            body.setText(sw.toString());
            body.setEditable(false);
            body.setBorder(BorderFactory.createEmptyBorder(5, 5, 5, 5));
            body.setSize(new Dimension(5 * uif.getDotsPerInch(), Integer.MAX_VALUE));
            //System.err.println("DT.isOK size=" + body.getSize());
            //System.err.println("DT.isOK psize=" + body.getPreferredSize());
            String title = uif.getI18NString("dt.confirm.title", m);
            // can't use JOptionPane convenience methods because we want to set
            // default option to "No"
            final JOptionPane pane = new JOptionPane(body, JOptionPane.WARNING_MESSAGE);
            ActionListener l = new ActionListener() {
                    public void actionPerformed(ActionEvent e) {
                        pane.setValue(e.getSource());
                        pane.setVisible(false);
                    }
                };
            JButton yesBtn = uif.createButton("dt.confirm.yes", l);
            JButton noBtn = uif.createButton("dt.confirm.no", l);
            pane.setOptions(new JComponent[] { yesBtn, noBtn });
            pane.setInitialValue(noBtn);

            confirmDialog = pane.createDialog(parent, title);
            confirmDialog.setVisible(true);
            confirmDialog.dispose();
            confirmDialog = null;

            if (pane.getValue() != yesBtn)
                return false;
        }

        return true;
    }

    /**
     * Check if it is OK to automatically exit JT Harness.
     * A warning dialog is posted to the user for a reasonable but short while
     * allowing the user to cancel the exit.
     * @return true if the user does not respond within the available time,
     * or if the user allows the request; and false otherwise
     */
    public boolean isOKToAutoExit() {
        final int delay = 30/*seconds*/;
        final JTextArea body = new JTextArea();
        body.setOpaque(false);
        body.setText(uif.getI18NString("dt.autoExit.txt", new Integer(delay)));
        body.setEditable(false);
        body.setBorder(BorderFactory.createEmptyBorder(5, 5, 5, 5));
        body.setSize(new Dimension(4 * uif.getDotsPerInch(), Integer.MAX_VALUE));

        final JOptionPane pane = new JOptionPane(body, JOptionPane.WARNING_MESSAGE, JOptionPane.OK_CANCEL_OPTION);
        String title = uif.getI18NString("dt.confirm.title", new Integer(EXIT));
        final JDialog dialog = pane.createDialog(null, title);

        final Timer timer = new Timer(1000, new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    if (--timeRemaining == 0) {
                        pane.setValue(new Integer(JOptionPane.OK_OPTION));
                        dialog.setVisible(false);
                    }
                    else
                        body.setText(uif.getI18NString("dt.autoExit.txt", new Integer(timeRemaining)));
                }

                private int timeRemaining = delay;
            });

        timer.start();
        dialog.setVisible(true);
        timer.stop();

        Object value = pane.getValue();
        return (value != null && value.equals(new Integer(JOptionPane.OK_OPTION)));
    }


    /**
     * Save the current state of the desktop in the user's standard desktop file.
     */
    public void save() {
        save(getDesktopFile());
    }

    /**
     * Save the current state of the desktop in a specified desktop file.
     * @param f the file in which to save the desktop
     */
    public void save(File f) {
        //System.err.println("DT: save to " + f);
        if (f == null)
            return;

        Properties p = new SortedProperties();

        int s = getStyle();
        if (s < NUM_STYLES) {
            p.put("dt.style", styleNames[s]);
            // backwards compatibility for JT3.1.x
            p.put("dt.class", jt31StyleClassNames[s]);
        }

        ensureViewInitialized();
        currView.saveDesktop(p);

        p.put("file.count", String.valueOf(fileHistory.size()));
        int n = 0;
        for (Iterator i = fileHistory.iterator(); i.hasNext(); ) {
            FileHistoryEntry h = (FileHistoryEntry) (i.next());
            p.put("fileHistory." + n + ".type", h.fileOpener.getFileType());
            p.put("fileHistory." + n + ".path", h.file.getPath());
            n++;
        }

        try {
            File dir = f.getParentFile();
            if (dir != null && !dir.exists())
                dir.mkdirs();
            OutputStream out = new BufferedOutputStream(new FileOutputStream(f));
            p.store(out, "JT Harness Desktop");
            out.close();
        }
        catch (IOException e) {
            System.err.println(uif.getI18NString("dt.cantWriteDt.txt", e.getMessage()));
            //System.err.println("Error writing desktop file: " + e);
        }
    }

    /**
     * Restore the desktop from information in a saved desktop file.
     * If no such file exists, or if no tools are successfully started
     * from the info in the file, add a default tool.
     * The work will automatically performed on the main AWT EventQueue
     * thread.
     */
    public void restore() {
        restore(getDesktopFile());
    }

    /**
     * Restore the desktop from information in a specified file.
     * If no such file exists, or if no tools are successfully started
     * from the info in the file, add a default tool.
     * The work will automatically performed on the main AWT EventQueue
     * thread.
     * @param file the file from which to load the data
     */
    public void restore(final File file) {
        Properties p = getPreviousDesktop(file);
        restore0(p);
    }

    public void restoreHistory() {
        restoreHistory(getPreviousDesktop(getDesktopFile()));
    }

    private void restore0(final Properties p) {
        //System.err.println("DT: restore " + file);
        if (!EventQueue.isDispatchThread()) {
            invokeOnEventThread(new Runnable() {
                    public void run() {
                        restore0(p);
                    }
                });
            return;
        }

        restoreHistory(p);

        // ALERT!! NEEDS FIXING!
        // should use saved view info
        /*
        String dtClassName = (String) p.getProperty("dt.class");
        if (dtClassName != null) {
            try {
                if (theOne != null)
                    theOne.dispose();
                theOne = (Desktop) (Class.forName(dtClassName).newInstance());
            }
            catch (Throwable e) {
                // I18N
                System.err.println("Error loading saved desktop class: " + e);
            }
        }
        */

        if (currView != null) {
            style = currView.getStyle(); // set default in case no valid style in desktop file
            currView.dispose();
        }

        int savedStyle;
        String s = (String) (p.get("dt.style"));
        if (s != null)
            savedStyle = indexOf(s, styleNames);
        else {
            // javatest 3.1 compatibility
            String c = (String) (p.get("dt.class"));
            savedStyle = (c == null ? -1 : indexOf(c, jt31StyleClassNames));
        }

        if (savedStyle != -1)
            style = savedStyle;

        ensureViewInitialized();
        currView.restoreDesktop(p);

        if (getTools().length == 0)
            addDefaultTool();

        // select the previously selected tool, if given
        // else a default, if available
        Tool t = getSelectedTool();
        if (t == null) {
            Tool[] tools = getTools();
            if (tools.length > 0)
                t = tools[0];
        }
        if (t != null)
            setSelectedTool(t);

        //System.err.println("DT.restore: set visible");
        setVisible(true);
        //System.err.println("DT: restore done");

    }

    private void restoreHistory(Properties p) {
        HashMap allOpeners = new HashMap();
        for (int i = 0; i < toolManagers.length; i++) {
            ToolManager m = toolManagers[i];
            FileOpener[] mgrOpeners = m.getFileOpeners();
            if (mgrOpeners != null) {
                for (int j = 0; j < mgrOpeners.length; j++) {
                    FileOpener fo = mgrOpeners[j];
                    allOpeners.put(fo.getFileType(), fo);
                }
            }
        }

        try {
            fileHistory.clear();
            String c = (String) (p.get("file.count"));
            if (c != null) {
                int count = Integer.parseInt(c);
                for (int i = 0; i < count; i++) {
                    try {
                        String ft = (String) (p.get("fileHistory." + i + ".type"));
                        FileOpener fo = (FileOpener) (allOpeners.get(ft));
                        if (fo != null) {
                            String path = (String) (p.get("fileHistory." + i + ".path"));
                            if (path != null && path.length() > 0)
                                fileHistory.add(new FileHistoryEntry(fo, new File(path)));
                        }
                    }
                    catch (Throwable e) {
                        // I18N
                        //System.err.println("Error loading saved file: " + e);
                        System.err.println(uif.getI18NString("dt.cantLoadHist.txt"));
                        e.printStackTrace();
                    }
                }
            }
        }
        catch (NumberFormatException ignore) {
            // ignore, for now
        }
    }

    static Properties getPreviousDesktop(File file) {
        if (file == null)
            file = getDesktopFile();

        Properties p = new Properties();

        if (file != null && file.exists()) {
            try {
                InputStream in = new BufferedInputStream(new FileInputStream(file));
                p.load(in);
                in.close();
            }
            catch (IOException e) {
                // I18N
                System.err.println("Error reading desktop file: " + e);
            }
        }

        return p;
    }

    /**
     * Show a Preferences window.
     * @param parent the parent frame to be used for the preferences dialog
     */
    public void showPreferences(JFrame parent) {
        if (prefsPane == null)
            prefsPane = new DesktopPrefsPane(this, uif);

        Vector v = new Vector();
        v.addElement(prefsPane);
        for (int i = 0; i < toolManagers.length; i++) {
            ToolManager m = toolManagers[i];
            Preferences.Pane p = m.getPrefsPane();
            if (p != null)
                v.addElement(p);
        }

        Preferences.Pane[] custom = getCustomPreferences();
        if (custom != null)
           for (int i = 0; i < custom.length; i++)
               v.add(custom[i]);

        Preferences.Pane[] panes = new Preferences.Pane[v.size()];
        v.copyInto(panes);
        preferences.showDialog(parent, panes);
    }

    /**
     * Allow for other custom prefs panes.  Current implementation scans
     * test suite properties for one called "prefsPane", which should be a
     * class name referring to a class which subclasses <code>Preferences.Pane</code>.
     * That class will be loaded and instantiated using the test suite's class loader.
     * @return A set of prefs panes, beyond that of the currently active Tools.
     *         Null if none.
     */
    private Preferences.Pane[] getCustomPreferences() {
        ArrayList al = new ArrayList();

        HashSet customPrefsClasses = new HashSet();

        Tool[] tools = getTools();
        for (int i = 0; i < tools.length; i++) {
            TestSuite[] tss = tools[i].getLoadedTestSuites();
            if (tss != null && tss.length > 0) {
                for (int j = 0; j < tss.length; j++) {
                    // only process each test suite once
                    if (customPrefsClasses.contains(tss[j].getID()))
                        continue;
                    else
                        customPrefsClasses.add(tss[j].getID());

                    String cls = tss[j].getTestSuiteInfo("prefsPane");
                    try {
                        if (cls != null) {
                            Preferences.Pane pane =
                                (Preferences.Pane)((Class.forName(cls, true,
                                tss[j].getClassLoader())).newInstance());
                            al.add(pane);
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
                    }   // try
                    finally {
                    }
                }   // inner for j
            }
        }   // for i

        if (al.size() > 0) {
            Preferences.Pane[] panes = new Preferences.Pane[al.size()];
            al.toArray(panes);
            return panes;
        }
        else
            return null;
    }

    /**
     * Get an icon containing the JT Harness logo.
     * @return an icon containing the JT Harness logo
     */
    public Icon getLogo() {
        return uif.createIcon("dt.logo");
    }

    /**
     * Dispose of any resources used by this object.
     */
    public void dispose() {
        if (currView != null)
            currView.dispose();
    }

    /**
     * Print a text message to the desktop logfile.
     * A single line of text which is as short as possible is highly
     * recommended for readability purposes.
     *
     * @param i18n a resource bundle containing the localized messages
     * @param key a key into the resource bundle for the required message
     *
     * @since 3.0.1
     */
    public void log(I18NResourceBundle i18n, String key) {
        ensureLogFileInitialized();
        logFile.log(i18n, key);
    }

    /**
     * Print a text message to the desktop logfile.
     * A single line of text which is as short as possible is highly
     * recommended for readability purposes.
     *
     * @param i18n a resource bundle containing the localized messages
     * @param key a key into the resource bundle for the required message
     * @param arg An argument to be formatted into the specified message.
     *          If this is a <code>Throwable</code>, its stack trace
     *          will be included in the log.
     * @since 3.0.1
     */
    public void log(I18NResourceBundle i18n, String key, Object arg) {
        ensureLogFileInitialized();
        logFile.log(i18n, key, arg);
    }

    /**
     * Print a text message to the desktop logfile.
     * A single line of text which is as short as possible is highly
     * recommended for readability purposes.
     *
     * @param i18n a resource bundle containing the localized messages
     * @param key a key into the resource bundle for the required message
     * @param args An array of arguments to be formatted into the specified message.
     *          If the first arg is a <code>Throwable</code>, its stack
     *          trace will be included in the log.
     * @since 3.0.1
     */
    public void log(I18NResourceBundle i18n, String key, Object[] args) {
        ensureLogFileInitialized();
        logFile.log(i18n, key, args);
    }

    private void ensureLogFileInitialized() {
        if (logFile == null) {
            File f;
            String s = System.getProperty("javatest.desktop.log");
            if (s == null) {
                File userDir = new File(System.getProperty("user.home"));
                File jtDir = new File(userDir, ".javatest"); // mild uugh
                f = new File(jtDir, "log.txt");
            }
            else if (s.equals("NONE")) {
                f = null;
            }
            else
                f = new File(s);

            try {
                BackupPolicy p = BackupPolicy.simpleBackups(5);
                p.backup(f);
            }
            catch (IOException e) {
                // ignore? or save exception to write to logFile
            }

            logFile = (f == null ? new LogFile() : new LogFile(f));
        }
    }

    // the order of the styles is the presentation order in the preferences panel
    /**
     * A constant to indicate the tabbed-style desktop:
     * a single window for the desktop, using a tabbed pane for the tools.
     */
    public static final int TAB_STYLE = 0;

    /**
     * A constant to indicate the MDI-style desktop:
     * a single window for the desktop, containing multiple internal windows, one per tool.
     */
    public static final int MDI_STYLE = 1;

    /**
     * A constant to indicate the SDI-style desktop:
     * multiple top-level windows, one per tool.
     */
    public static final int SDI_STYLE = 2;

    static final int NUM_STYLES = 3;
    static final String[] styleNames = {"tab", "mdi", "sdi"};

    private static final String[] jt31StyleClassNames = {
        "com.sun.javatest.tool.TabDesktop",
        "com.sun.javatest.tool.MDIDesktop",
        "com.sun.javatest.tool.SDIDesktop"
    };

    /**
     * Check whether or not the desktop will save its state when the VM exits.
     * @return true if the desktop will save its state when the VM exits, and false otherwise
     * @see #setSaveOnExit
     */
    public boolean getSaveOnExit() {
        return saveOnExit;
    }

    /**
     * Specify whether or not the desktop will save its state when the VM exits.
     * @param b true if the desktop should save its state when the VM exits, and false otherwise
     * @see #getSaveOnExit
     */
    public void setSaveOnExit(boolean b) {
        saveOnExit = b;
    }

    /**
     * Get Tooltip delay from prefs in ms.
     * @return Range is 0-Integer.MAX_VALUE
     */
    static int getTooltipDelay(Preferences p) {
        String val = p.getPreference(TTIP_DELAY);
        int result = TTIP_DELAY_DEFAULT;

        try {
            // expected range from prefs in 0-Integer.MAX_VALUE
            result = Integer.parseInt(val);
        }
        catch (NumberFormatException e) {
            // default to no delay
            result = TTIP_DELAY_DEFAULT;
        }

        if (result < 0)
            result = TTIP_DELAY_DEFAULT;;

        return result;
    }

    /**
     * Get tooltip duration from prefs in ms.
     * This is the translated value, so the "forever" value has been
     * transformed into something useful.
     * @return Range is 0-Integer.MAX_VALUE
     */
    static int getTooltipDuration(Preferences p) {
        String val = p.getPreference(TTIP_DURATION);
        int result = TTIP_DURATION_DEFAULT;

        try {
            // expected range from prefs in -1-Integer.MAX_VALUE
            result = Integer.parseInt(val);
        }
        catch (NumberFormatException e) {
            // default to no delay
            result = TTIP_DURATION_DEFAULT;
        }

        if (result < 0)
            if (result == TTIP_DURATION_FOREVER)        // indicates forever duration
                result = Integer.MAX_VALUE;
            else                        // -2 or less, unknown value
                result = TTIP_DURATION_DEFAULT;
        else { }

        return result;
    }

    // these are here to be shared with DesktopPrefsPane.
    void setTooltipsEnabled(boolean state) {
        ToolTipManager.sharedInstance().setEnabled(state);
    }

    /**
     * Unconditionally set the tooltip delay to the given setting.
     * @param delay Delay time in ms or TTIP_DELAY_NONE.
     */
    void setTooltipDelay(int delay) {
        ToolTipManager.sharedInstance().setInitialDelay(delay);
    }

    /**
     * Unconditionally set the tooltip duration to the given setting.
     * @param duration Duration time in ms or TTTIP_DURATION_FOREVER.
     */
    void setTooltipDuration(int duration) {
        if (duration == TTIP_DURATION_FOREVER)
            ToolTipManager.sharedInstance().setDismissDelay(Integer.MAX_VALUE);
        else
            ToolTipManager.sharedInstance().setDismissDelay(duration);
    }

    public void printSetup() {
        ensurePrintAttrsInitialized();
        PrinterJob job = PrinterJob.getPrinterJob();
        job.pageDialog(printAttrs);
    }

    public void print(Printable printable) {

        DocFlavor flavor = DocFlavor.SERVICE_FORMATTED.PRINTABLE;
        PrintService[] services = PrintServiceLookup.lookupPrintServices(flavor, null);

        if(services.length > 0) {
            ensurePrintAttrsInitialized();

            Component parent = getDialogParent();
            int x = (int)parent.getLocationOnScreen().getX() + parent.getWidth() / 2 - 250;
            int y = (int)parent.getLocationOnScreen().getY() + parent.getHeight() / 2 - 250;

            PrintService service = ServiceUI.printDialog(null, x, y, services,
                    services[0], flavor, printAttrs);
            if(service != null) {
                DocPrintJob job = service.createPrintJob();
                try {
                    Doc doc = new SimpleDoc(printable, flavor, null);

                    job.print(doc, printAttrs);
                }
                catch (PrintException e) {
                    e.printStackTrace();
                }
            }
        }

    }

    private void ensurePrintAttrsInitialized() {
        if(printAttrs == null) {
            printAttrs = new HashPrintRequestAttributeSet();
        }
    }

    private void initToolManagers() {
        // locate init file and load up the managers
        //System.err.println("Desktop.initToolManagers");

        try {
            ManagerLoader ml = new ManagerLoader(ToolManager.class, System.err);
            ml.setManagerConstructorArgs(new Class[] { Desktop.class }, new Object[] { this });
            Set s = ml.loadManagers(TOOLMGRLIST);
            toolManagers = (ToolManager[]) (s.toArray(new ToolManager[s.size()]));
        }
        catch (IOException e) {
            throw new JavaTestError(uif.getI18NResourceBundle(),
                                    "dt.cantAccessResource", new Object[] { TOOLMGRLIST, e } );
        }
    }

    private void ensureViewInitialized() {
        if (currView != null)
            return;

        switch (style) {
        case MDI_STYLE:
            currView = new MDIDeskView(this);
            break;

        case SDI_STYLE:
            currView = new SDIDeskView(this);
            break;

        default:
            currView = new TabDeskView(this);
            break;
        }
    }

    private static void appendStrings(StringBuffer sb, String[] msgs) {
        if (msgs != null) {
            for (int i = 0; i < msgs.length; i++) {
                sb.append(msgs[i]);
                if (!msgs[i].endsWith("\n"))
                    sb.append('\n');
            }
        }
    }

    /**
     * Get the file in which the desktop is (to be) stored.
     * The standard location is the platform equivalent of
     * $HOME/.javatest/desktop
     * It can be overridden by setting the system property
     * "javatest.desktop.file", which can be set to "NONE"
     * to disable the feature.
     */
    private static File getDesktopFile() {
        String s = System.getProperty("javatest.desktop.file");
        if (s == null) {
            File userDir = new File(System.getProperty("user.home"));
            File jtDir = new File(userDir, ".javatest"); // mild uugh
            return new File(jtDir, "desktop");
        }
        else if (!s.equals("NONE"))
            return new File(s);
        else
            return null;
    }

    static void addHelpDebugListener(Component c) {
        JComponent root;
        if (c instanceof JFrame)
            root = ((JFrame) c).getRootPane();
        else if (c instanceof JDialog)
            root = ((JDialog) c).getRootPane();
        else
            throw new IllegalArgumentException();

        ActionListener showFocusListener = new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                Component src = (Component) e.getSource();
                Component comp = javax.swing.SwingUtilities.findFocusOwner(src);
                System.err.println("ALT-F2: source=" + src);
                System.err.println("ALT-F2:  focus=" + comp);
            }
        };

        root.registerKeyboardAction(showFocusListener,
                                    KeyStroke.getKeyStroke("alt F2"),
                                    JComponent.WHEN_IN_FOCUSED_WINDOW);
    }

    static void addPreferredSizeDebugListener(Component c) {
        JComponent root;
        if (c instanceof JFrame)
            root = ((JFrame) c).getRootPane();
        else if (c instanceof JDialog)
            root = ((JDialog) c).getRootPane();
        else
            throw new IllegalArgumentException();

        ActionListener showPrefSizeListener = new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                Component src = (Component) e.getSource();
                Component c = javax.swing.SwingUtilities.findFocusOwner(src);
                while (c != null) {
                    Dimension d = c.getPreferredSize();
                    System.err.println("ALT-1: comp=" + c.getName() + "(" + c.getClass().getName() + ") "
                                     + "[w:" + d.width + ",h:" + d.height + "]");
                    c = c.getParent();
                }
            }
        };

        root.registerKeyboardAction(showPrefSizeListener,
                                    KeyStroke.getKeyStroke("alt 1"),
                                    JComponent.WHEN_IN_FOCUSED_WINDOW);
    }

    private static void invokeOnEventThread(Runnable r) {
        try {
            EventQueue.invokeAndWait(r);
        }
        catch (InterruptedException e) {
        }
        catch (InvocationTargetException e) {
            Throwable t = e.getTargetException();
            if (t instanceof RuntimeException)
                throw ((RuntimeException) t);
            else
                throw ((Error) t);
        }
    }

    private static int indexOf(String s, String[] a) {
        for (int i = 0; i < a.length; i++) {
            if (s == null ? a[i] == null : s.equals(a[i]))
                return i;
        }
        return -1;
    }

    private final UIFactory uif;
    private CommandContext commandContext;
    private DeskView currView;
    private int style; // used until currView is set, then style comes from that
    private Dimension preferredSize;
    private Preferences.Pane prefsPane;
    private ToolManager[] toolManagers;
    private LogFile logFile;
    private boolean firstTime;
    private boolean saveOnExit;
    private static int frameIndex;
    private PrintRequestAttributeSet printAttrs;

    private LinkedList fileHistory = new LinkedList();
    private static final int FILE_HISTORY_MAX_SIZE = 10;

    private static Preferences preferences = Preferences.access();


    static final String STYLE_PREF = "tool.appearance.style";
    static final String TTIP_PREF = "tool.appearance.ttipToggle";
    static final String TTIP_DELAY= "tool.appearance.ttipDelay";
    static final String TTIP_DURATION = "tool.appearance.ttipDuration";
    static final int TTIP_DURATION_FOREVER = -1;
    static final int TTIP_DELAY_NONE = 0;
    static final int TTIP_DELAY_DEFAULT = 0;
    static final int TTIP_DURATION_DEFAULT = 5000;
    static final String SAVE_ON_EXIT_PREF = "tool.appearance.saveOnExit";

    private static final String TOOLMGRLIST = "JavaTest.toolMgrs.lst";
    private static final String defaultToolManager =
        System.getProperty("javatest.desktop.defaultToolManager", "com.sun.javatest.exec.ExecToolManager");


    //-------------------------------------------------------------------------

    /**
     * A class for an entry on the file history list.
     * It defines a file, and an object to open that file if required.
     */
    static class FileHistoryEntry {
        FileHistoryEntry(FileOpener fo, File f) {
            fileOpener = fo;
            file = f;
        }

        FileOpener fileOpener;
        File file;
    }
}
