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
package com.sun.javatest.report;

import java.io.File;
import java.io.IOException;
import java.util.Vector;

import com.sun.javatest.CompositeFilter;
import com.sun.javatest.InterviewParameters;
import com.sun.javatest.Status;
import com.sun.javatest.TestFilter;
import com.sun.javatest.TestResultTable;
import com.sun.javatest.TestSuite;
import com.sun.javatest.tool.Preferences;
import com.sun.javatest.util.BackupUtil;
import com.sun.javatest.util.HTMLWriter;
import com.sun.javatest.util.I18NResourceBundle;
import com.sun.javatest.util.StringArray;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.InputStreamReader;
import java.io.Writer;
import java.text.SimpleDateFormat;
import java.util.Collection;
import java.util.Collections;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Set;

/**
 * A report generator for sets of test results.
 */
public class Report implements ReportModel {
    public Report() {
    }

    /**
     * Creates and initializes an instance of the report generator.
     * @param params Configuration parameters to be included in the report.
     * @param dir The directory to which to write the report.
     * @deprecated It is expected that you call writeReport() if you use this
     *             constructor.
     */
    public Report(InterviewParameters params, File dir) {
        this.params = params;
        //this.resultTable = params.getWorkDirectory().getTestResultTable();

        reportDir = dir.getAbsoluteFile();
    }

    /**
     * Creates and initializes an instance of the report generator.
     * @param params Configuration parameters to be included in the report.
     * @param dir The directory to which to write the report.
     * @param tf The test filter to be used to filter out tests in the report.
     * @deprecated It is expected that you call writeReport() if you use this
     *             constructor.
     */
    public Report(InterviewParameters params, File dir, TestFilter tf) {
        this(params, dir);

        paramFilters = new TestFilter[] { tf };
    }

    // --------- Execution methods -----------
    /**
     * Checks whether writing this report will overwrite any existinng files.
     * @return an array of files that will be overrwritten, or null if none.
     */
    public File[] checkExistingFiles() {
        Vector v = new Vector();
        String[] files = HTMLReport.getFilenamesUsed();
        String[] list = reportDir.list();

        for (int i = 0; i < files.length; i++)
            for (int j = 0; j < list.length; j++)
                if (files[i].equals(list[j]))
                    v.addElement(new File(reportDir, list[j]));

        if (v.size() == 0)
            return null;
        else {
            File[] result = new File[v.size()];
            v.copyInto(result);
            return result;
        }
    }

    /**
     * Write report files using the given settings, to the given location.
     * This is the execution entry point for GUI mode.  The settings used
     * are written into the JT Harness preferences automatically at the end
     * of this method, unless this method exits with an exception.
     */
    public void writeReport(Settings s, File dir) throws IOException {
        writeReports(s, dir, Collections.EMPTY_LIST);
    }

    public void writeReports(Settings s, File dir, Collection customReports) throws IOException {
        reportDir = dir.getAbsoluteFile();
        // ensure test result table is stable
        s.ip.getWorkDirectory().getTestResultTable().waitUntilReady();

        backupReports(reportDir, s/*, customReports*/);

        if (s.isHtmlEnabled())
            writeReportHTML(s);

        if (Thread.currentThread().isInterrupted()) return;

        // plain text report
        if (s.isPlainEnabled())
            writeSummaryTXT(s);

        if (Thread.currentThread().isInterrupted()) return;

        // XML report
        CustomReport.ReportEnviroment re = new CustomReport.ReportEnviroment(s);
        if (s.isXmlEnabled()) {
            notifyStartGenListeners(s, "xml");
            writeReportXML(re);
        }

        if (Thread.currentThread().isInterrupted()) {
            re.cleanup();
            return;
        }

        // write settings to Preferences
        writePrefs(s);

        if (Thread.currentThread().isInterrupted()) {
            re.cleanup();
            return;
        }

        Iterator it = customReports.iterator();
        HashMap crMap = new HashMap();
        while (it.hasNext()) {
            CustomReport cr = (CustomReport) it.next();
            cr.setEnviroment(re);
            notifyStartGenListeners(s, cr.getName());
            File f = cr.createReport(new File(reportDir, cr.getReportId()));

            if(f != null) {
                crMap.put(f, cr);
            }

            if (Thread.currentThread().isInterrupted()) {
                re.cleanup();
                return;
            }
        }

        re.cleanup();

        updateStaffFiles(reportDir, s, crMap);
    }

    /**
     * Writes a report about a set of test results.
     * This is the execution entry point for batch mode.  The default settings
     * from the preferences will be used.
     * @throws IOException if the is a problem while writing the reports.
     * @deprecated Use <code>writeReport(Settings, File)</code>
     *             It is expected that you use a deprecated constructor
     *             if you use this method.
     */
    public void writeReport() throws IOException {
        // make a settings object - default settings
        //Settings settings = new Settings(params);
        Settings settings = getSettingsPrefs();
        settings.ip = params;

        // this setting can orginate in a legacy constructor
        if (paramFilters != null)
            if (paramFilters.length == 1)
                settings.filter = paramFilters[0];
            else
                settings.filter = new CompositeFilter(paramFilters);

        // ensure test result table is stable
        params.getWorkDirectory().getTestResultTable().waitUntilReady();

        backupReports(reportDir, settings);

        writeReportHTML(settings);
        writeSummaryTXT(settings);

        updateStaffFiles(reportDir, settings, null);
    }

    /**
     * New entry point for batch mode (only).
     * @throws IllegalArgumentException if the type parameter does not
     *         identify a proper report type.
     * @param type The report type identifier, may be a custom type.
     */
    public void writeReport(String type) throws IOException {
        // make a settings object - default settings
        //Settings settings = new Settings(params);
        Settings settings = getSettingsPrefs();
        settings.ip = params;

        // this setting can orginate in a legacy constructor
        if (paramFilters != null)
            if (paramFilters.length == 1)
                settings.filter = paramFilters[0];
            else
                settings.filter = new CompositeFilter(paramFilters);

        // ensure test result table is stable
        params.getWorkDirectory().getTestResultTable().waitUntilReady();

        backupReports(reportDir, settings);

        if (type.equalsIgnoreCase("html"))
            writeReportHTML(settings);
        else if (type.equalsIgnoreCase("txt"))
            writeSummaryTXT(settings);
        else if (type.equalsIgnoreCase("xml")) {
            CustomReport.ReportEnviroment re =
                new CustomReport.ReportEnviroment(settings);
            notifyStartGenListeners(settings, "xml");
            writeReportXML(re);
        }
        else {
            // ---- code from ExecTool
            // introduces a depedency on exec pkg, which isn't good
            String cls = null;
            TestSuite testSuite = params.getTestSuite();
            cls = testSuite.getTestSuiteInfo("tmcontext");
            CustomReportManager cm = null;

            try {
                if (cls == null) {
                    // use default implementation
                    cm = (CustomReportManager)((Class.forName(
                            "com.sun.javatest.exec.ContextManager")).newInstance());
                } else {
                    cm = (CustomReportManager)((Class.forName(cls, true,
                            testSuite.getClassLoader())).newInstance());
                }
            } catch (ClassNotFoundException e) {
                e.printStackTrace();        // XXX rm
                return;
                // should print log entry
            } catch (InstantiationException e) {
                e.printStackTrace();        // XXX rm
                return;
                // should print log entry
            } catch (IllegalAccessException e) {
                e.printStackTrace();        // XXX rm
                return;
                // should print log entry
            }
            // ----

            CustomReport[] customReports = cm.getCustomReports();
            if (customReports == null || customReports.length == 0) {
                throw new IllegalStateException("Unknown report type: " + type);
            }

            boolean result = false;
            HashMap crMap = new HashMap();
            for (int i = 0; i < customReports.length; i++) {
                if (customReports[i].getReportId().equalsIgnoreCase(type)) {
                    try {
                        CustomReport.ReportEnviroment re =
                                new CustomReport.ReportEnviroment(params, settings.filter);
                        customReports[i].setEnviroment(re);
                        notifyStartGenListeners(settings, customReports[i].getName());
                        File f = customReports[i].createReport(null, reportDir,
                                params, settings.filter);

                        if(f != null) {
                            crMap.put(f, customReports[i]);
                        }

                        result = true;      // success
                        break;
                    } catch (CustomReport.ReportException e) {
                        // XXX need better error reporting
                        e.printStackTrace();  // send to logging instead
                        return;
                    }   // catch
                }
            }   // for

            if (!result)
                throw new IllegalStateException("Unknown report type: " + type);

            updateStaffFiles(reportDir, settings, crMap);
            return;
        }
        updateStaffFiles(reportDir, settings, null);
    }

    /**
     * Checks if the input directory contains JT Harness reports.
     * @param d The directory to be checked.
     * @return true if the directory contains JT Harness reports.
     */
    public static boolean isReportDirectory(File d) {

        String[] list = d.list();
        if (list == null)
            return false;

        for(int i = 0; i < list.length; i++) {
            if(list[i].equals(MARKER_FILE_NAME)) {
                return true;
            }
        }

        if(isLegacyReportDirectory(d))
            return true;
        // no matches
        return false;
    }

    static boolean isLegacyReportDirectory(File d) {
        String[] list = d.list();
        if(list == null)
            return false;

        String[] htmlNames = HTMLReport.getFilenamesUsed();
        String[] textNames = PlainTextReport.getFilenamesUsed();

        for(int i = 0; i < list.length; i++) {
            for(int j = 0; j < htmlNames.length; j++) {
                if(htmlNames[j].equals(HTMLReport.NEW_REPORT_NAME)) {
                    continue;
                } else if(list[i].equals(htmlNames[j])) {
                    return true;
                }
            }

            for(int j = 0; j < textNames.length; j++) {
                if(list[i].equals(textNames[j])) {
                    return true;
                }
            }
        }
        return false;
    }


// Report Backup methods-------------------------------------------------------------------

    /**
     * This is entry point to report backup mechanism.
     * Invokes methods, which rename existing report subdirs, index.html file;
     * updates index.html backupped versions to have correct links
     * Checks, if we able to perform backup
     * @param dir root report directory, where we perform backup
     * @param s Settings, collected for report creation
     */
    private void backupReports(File dir, Settings s/*, Collection customReports*/) {
        if(s.isBackupsEnabled()) {
            BackupUtil.backupAllSubdirs(dir, s.backups);

            backupIndexFile(dir, s.backups);
        }
    }

    /**
     * Backups index.html file using BackupUtil.backupFile() method. Then update
     * links in backupped versions of index.html
     * @param dir Root reportDir, where index.html file situates
     * @param maxBackups maximum allowed number of backupped versions to exist
     */
    private void backupIndexFile(File dir, int maxBackups) {
        int nbackups = BackupUtil.backupFile(new File(dir, INDEX_FILE_NAME), maxBackups);

        for(int i = 1; i <= nbackups; i++) {
            updateIndexLinks(new File(dir, INDEX_FILE_NAME + "~" + Integer.toString(i) + "~"), i);
        }
    }

    private class LinkFinder /*extends HTMLEditorKit.ParserCallback*/ {
        private File index;

        public LinkFinder(File index) {
            this.index = index;
        }

        public Vector getLinks() {
            Vector links = new Vector();
            StringBuffer content = new StringBuffer();
            try {
                BufferedReader r = new BufferedReader(new InputStreamReader(new FileInputStream(index)));
                String line;
                while( (line = r.readLine()) != null ) {
                    content.append(line);
                    content.append("\n");
                }
            } catch (IOException e) {}
            int i = 0;
            while (i < (content.length() - 1) ) {
                if(content.charAt(i) == '<' && content.charAt(i + 1) == 'a') {
                    StringBuffer link = new StringBuffer();
                    link.append(content.charAt(i));
                    i++;
                    link.append(content.charAt(i));
                    i++;

                    boolean end = false;
                    while (!end && i < (content.length() - 1) ) {
                        if (content.charAt(i) == '/' && content.charAt(i+1) == 'a') {
                            link.append(content.charAt(i));
                            i++;
                            link.append(content.charAt(i));
                            i++;
                            link.append(content.charAt(i));
                            i++;
                            end = true;
                        }
                        else {
                            link.append(content.charAt(i));
                            i++;
                        }
                    }
                    links.add(link.toString());
                }
                else {
                    i++;
                }
            }
            return links;
        }
    };


    /**
     * Parses backupped version of index.html file to update links, which points
     * to files in backupped subdirs. Searchs for subdirs with the same backup
     * suffix, as selected index.html has. Then checks all links in file to find
     * those pointing to backupped subdirs and updates them.
     * @param index backupped version of index.html file we work with
     * @param backupNumb index in backupped version of index.html file we work with
     */
    private void updateIndexLinks(File index, int backupNumb) {
        StringBuffer sb = new StringBuffer();
        try {
            BufferedReader r = new BufferedReader(new InputStreamReader(new FileInputStream(index)));
            String line;
            while( (line = r.readLine()) != null ) {
                sb.append(line);
                sb.append("\n");
            }
        } catch (IOException e) {}

        String oldId = backupNumb == 1 ? "" : "~" + (backupNumb - 1) + "~";
        String newId = "~" + backupNumb + "~";

        File reportDir = index.getParentFile();
        File[] files = reportDir.listFiles();
        Vector subdirs = new Vector();
        for(int i = 0; i < files.length; i ++) {
            if(files[i].isDirectory() &&
                    files[i].getName().lastIndexOf(newId) != -1) {
                subdirs.add(files[i]);
            }
        }

        LinkFinder finder = new LinkFinder(index);
        Vector links = finder.getLinks();
        for(int i = 0; i < links.size(); i++) {
            String link = (String)links.get(i);
            for(int j = 0; j < subdirs.size(); j++) {
                String newName = ((File)subdirs.get(j)).getAbsolutePath();
                String oldName = newName.replaceAll(newId, oldId);
                if(link.lastIndexOf(oldName) != -1) {
                    StringBuffer newLink = new StringBuffer(link);
                    int link_start = newLink.indexOf(oldName);
                    newLink.replace(link_start, link_start + oldName.length(), newName);
                    int start = sb.indexOf(link);
                    sb.replace(start, start + link.length(), newLink.toString());
                    break;
                }
            }
        }

        try {
            Writer writer = new BufferedWriter(new FileWriter(index));
            writer.write(sb.toString());
            writer.flush();
            writer.close();
        }
        catch (IOException ex) {}
    }


    private void updateStaffFiles(File dir, Settings s, /*Collection crs,
                                                            Vector displFiles*/HashMap crs) {
        updateMarkerFile(dir);
        updateIndexFile(dir, s, crs);
    }

    private void updateMarkerFile(File dir) {
        File f = new File(dir, MARKER_FILE_NAME);
        if(!f.exists()) {
            try {
                f.createNewFile();
            } catch (IOException e) {
                return;
            }
        }
    }

    private void updateIndexFile(File dir, Settings s, HashMap crs) {
        File f = new File(dir, INDEX_FILE_NAME);
        if(f.exists()) {
            f.delete();
        }
        try {
            f.createNewFile();
            fillIndexFile(f, s, crs);
        } catch (IOException e) {

        }
    }

    private void fillIndexFile(File index, Settings s, HashMap crs) {
        try {
            Writer writer = new BufferedWriter(new FileWriter(index));
            HTMLWriter out = new HTMLWriter(writer);
            out.setI18NResourceBundle(i18n);

            out.startTag(HTMLWriter.HTML);

            out.startTag(HTMLWriter.HEAD);
            out.startTag(HTMLWriter.TITLE);
            out.writeI18N("index.title");

            out.endTag(HTMLWriter.TITLE);
            out.endTag(HTMLWriter.HEAD);

            out.startTag(HTMLWriter.BODY);

            out.startTag(HTMLWriter.H1);
            out.write(s.ip.getTestSuite().getName());
            out.endTag(HTMLWriter.H1);

            out.newLine();
            Date date = new Date();
            SimpleDateFormat format= new SimpleDateFormat(i18n.getString("index.dateFormat"));
            out.write(i18n.getString("index.date.txt", format.format(date)));
            out.newLine();

            File f;
            if (s.indexHtml) {
                f = new File("html" + File.separator + "index.html");
            } else if (s.reportHtml) {
                f = new File("html" + File.separator + "report.html");
            } else {
                f = new File("html" + File.separator);
            }

            File absolute = new File(reportDir, f.getPath());
            if (absolute.exists()) {
                out.startTag(HTMLWriter.P);
                out.writeLink(f, i18n.getString("index.htmltype.txt"));
                out.startTag(HTMLWriter.BR);
                out.writeI18N("index.desc.html");
            }

            f = new File("text" + File.separator + "summary.txt");
            absolute = new File(reportDir, f.getPath());
            if (absolute.exists()) {
                out.startTag(HTMLWriter.P);
                out.writeLink(f, i18n.getString("index.plaintype.txt"));
                out.startTag(HTMLWriter.BR);
                out.writeI18N("index.desc.txt");
            }

            f = new File("xml" + File.separator + "report.xml");
            absolute = new File(reportDir, f.getPath());
            if (absolute.exists()) {
                out.startTag(HTMLWriter.P);
                out.writeLink(f, i18n.getString("index.xmltype.txt"));
                out.startTag(HTMLWriter.BR);
                out.writeI18N("index.desc.xml");
            }

            if (crs != null) {
                Set keys = crs.keySet();
                Iterator iter = keys.iterator();
                while(iter.hasNext()) {
                    File indexFile = (File)iter.next();
                    CustomReport cr = (CustomReport) crs.get(indexFile);
                    if (!indexFile.isAbsolute()) {
                        indexFile = new File(cr.getReportId() + File.separator +
                                indexFile.getPath());
                    }
                    else {
                        String absPath = indexFile.getAbsolutePath();
                        String relPath = absPath.substring(
                                reportDir.getAbsolutePath().length() + 1);
                        indexFile = new File(relPath);
                    }

                    out.startTag(HTMLWriter.P);
                    out.writeLink(indexFile, cr.getName());
                    out.startTag(HTMLWriter.BR);
                    out.write(cr.getDescription());
                }
            }

            out.endTag(HTMLWriter.BODY);
            out.endTag(HTMLWriter.HTML);
            out.flush();
            out.close();

        } catch (IOException e) {
            return;
        }
    }

// END Report Backup methods-------------------------------------------------------

    /**
     * Gets the report directory that is currently defined.
     * @return The report directory.
     */
    public File getReportDir() {
        return reportDir;
    }

    public static String[] getHtmlReportFilenames() {
        return HTMLReport.getReportFilenames();
    }

    /*
    public static String[] getXmlReportFilenames() {
        return XMLReport.getReportFilenames();
    }
     */

    public static String[] getPlainReportFilenames() {
        return PlainTextReport.getReportFilenames();
    }

    /**
     * Returns all filenames that may be used when creating a report
     * of any type (HTML, plain text, XML, etc...).
     */
    public static String[] getFilenamesUsed() {
        String[] htmlNames = HTMLReport.getFilenamesUsed();
        String[] plainTextNames = PlainTextReport.getFilenamesUsed();
        String[] xmlNames = XMLReport.getFilenamesUsed();

        String[] filenames = new String[htmlNames.length + plainTextNames.length + xmlNames.length];

        for(int i = 0; i <htmlNames.length; i++) {
            filenames[i] = "html/" + htmlNames[i];
        }
        for(int i = htmlNames.length; i < plainTextNames.length; i++) {
            filenames[i] = "text/" + plainTextNames[i];
        }
        for(int i = plainTextNames.length; i < xmlNames.length; i++) {
            filenames[i] = "xml/" + plainTextNames[i];
        }
        return filenames;
    }

    //---------- primary (HTML) report ---------------------------------------

    private void writeReportHTML(Settings settings) throws IOException {
        HTMLReport rpt = new HTMLReport(i18n);
        File out = new File(reportDir, "html");
        out.mkdir();
        notifyStartGenListeners(settings, "html");
        rpt.write(settings, out);
    }

    //---------- XML report ---------------------------------------

    private void writeReportXML(CustomReport.ReportEnviroment settings) throws IOException {
        XMLReport rpt = new XMLReport();
        File out = new File(reportDir, "xml");
        out.mkdir();
        rpt.write(settings, out);
    }

    //---------- plain text summary -----------------------------------------

    private void writeSummaryTXT(Settings settings) throws IOException {
        PlainTextReport rpt = new PlainTextReport(i18n);
        File out = new File(reportDir, "text");
        out.mkdir();
        notifyStartGenListeners(settings, "pt");
        rpt.write(settings, out);
    }

    //-----------------------------------------------------------------------

    private void notifyStartGenListeners(Settings s, String reportID) {
        if(startGenListeners != null) {
            for(int i = 0; i < startGenListeners.size(); i ++) {
                StartGenListener sgl = (StartGenListener)startGenListeners.get(i);
                sgl.startReportGeneration(s, reportID);
            }
        }
    }


    public static void writePrefs(Settings s) {
        Preferences prefs = Preferences.access();
        s.write(prefs);
    }

    public static Settings getSettingsPrefs() {
        Preferences prefs = Preferences.access();
        return Settings.create(prefs);
    }

    /**
     * Specify what parts of the reports to generate.
     */
    public static class Settings {
        public Settings() {
            for (int i = 0; i < stateFiles.length; i++) {
                stateFiles[i] = true;
            }
        }

        public Settings(InterviewParameters p) {
            this();
            ip = p;
        }

        public void write(Preferences prefs) {
            prefs.setPreference(PREFS_GEN_HTML, Boolean.toString(genHtml));
            prefs.setPreference(PREFS_GEN_PLAIN, Boolean.toString(genPlain));
            prefs.setPreference(PREFS_GEN_XML, Boolean.toString(genXml));

            prefs.setPreference(PREFS_HTML_CONFIG, Boolean.toString(genConfig));
            prefs.setPreference(PREFS_HTML_QL, Boolean.toString(genQl));
            prefs.setPreference(PREFS_HTML_ENV, Boolean.toString(genEnv));
            prefs.setPreference(PREFS_HTML_STD, Boolean.toString(genStd));
            prefs.setPreference(PREFS_HTML_RES, Boolean.toString(genResults));
            prefs.setPreference(PREFS_HTML_KWS, Boolean.toString(genKws));
            prefs.setPreference(PREFS_HTML_REPORTF, Boolean.toString(reportHtml));
            prefs.setPreference(PREFS_HTML_INDEXF, Boolean.toString(indexHtml));

            // html state files
            // encoded as a comma sep. list
            StringBuffer sb = new StringBuffer();
            for (int i = 0; i < stateFiles.length; i++) {
                sb.append(Boolean.toString(stateFiles[i]));
                if (i+1 < stateFiles.length) sb.append(",");
            }   // for

            prefs.setPreference(PREFS_HTML_STATEF, sb.toString());

            prefs.setPreference(PREFS_BACK, Boolean.toString(doBackups));
            prefs.setPreference(PREFS_BACK_NUM, Integer.toString(backups));
        }

        public static Settings create(Preferences prefs) {
            Settings result = new Settings();

            // special check to see if pref settings are available
            // if not we will use the defaults
            String tst = prefs.getPreference(PREFS_GEN_HTML);
            if (tst == null)
                return result;
            else
                result.genHtml = parseBoolean(tst);

            result.genPlain = parseBoolean(prefs.getPreference(PREFS_GEN_PLAIN));
            result.genXml = parseBoolean(prefs.getPreference(PREFS_GEN_XML));

            result.genConfig = parseBoolean(prefs.getPreference(PREFS_HTML_CONFIG));
            result.genQl = parseBoolean(prefs.getPreference(PREFS_HTML_QL));
            result.genEnv = parseBoolean(prefs.getPreference(PREFS_HTML_ENV));
            result.genStd = parseBoolean(prefs.getPreference(PREFS_HTML_STD));
            result.genResults = parseBoolean(prefs.getPreference(PREFS_HTML_RES));
            result.genKws = parseBoolean(prefs.getPreference(PREFS_HTML_KWS));
            result.reportHtml = parseBoolean(prefs.getPreference(PREFS_HTML_REPORTF));
            result.indexHtml = parseBoolean(prefs.getPreference(PREFS_HTML_INDEXF));

            // html state files
            // encoded as a comma sep. list
            String[] states=
                    StringArray.splitList(prefs.getPreference(PREFS_HTML_STATEF), ",");

            if (states != null)
                for (int i = 0; i < states.length; i++) {
                result.stateFiles[i] = parseBoolean(states[i]);
                }       // for

            result.doBackups = parseBoolean(prefs.getPreference(PREFS_BACK));
            try {
                result.backups = Integer.parseInt(prefs.getPreference(PREFS_BACK_NUM));
            } catch (NumberFormatException e) {
                // leave as default
            }

            return result;
        }

        public void setInterview(InterviewParameters p) {
            ip = p;
        }

        public void setFilter(TestFilter f) {
            filter = f;
        }

        public void setEnableHtmlReport(boolean state) {
            genHtml = state;
        }

        public void setEnableXmlReport(boolean state) {
            genXml = state;
        }

        public void setEnablePlainReport(boolean state) {
            genPlain = state;
        }

        public void setShowConfigSection(boolean state) {
            genConfig = state;
        }

        public void setShowQuestionLog(boolean state) {
            genQl = state;
        }

        public void setShowEnvLog(boolean state) {
            genEnv = state;
        }

        public void setShowStdValues(boolean state) {
            genStd = state;
        }

        public void setShowResults(boolean state) {
            genResults = state;
        }

        public void setShowKeywordSummary(boolean state) {
            genKws = state;
        }

        /**
         * @param status PASS, FAIL, ERROR, NOT_RUN constant from Status
         */
        public void setEnableHtmlStateFile(int status, boolean state) {
            if (status >= stateFiles.length)
                return;     // error condition

            stateFiles[status] = state;
            /*
            switch (status) {
                case Status.PASSED:
                    genPass = state;
                    break;
                case Status.FAILED:
                    genFail = state;
                    break;
                case Status.ERROR:
                    genErr = state;
                    break;
                case Status.NOT_RUN:
                    genNr = state;
                    break;
                default:
                    // oh well
            }
             */
        }

        public void setHtmlMainReport(boolean reporthtml, boolean indexhtml) {
            reportHtml = reporthtml;
            indexHtml = indexhtml;
        }

        public void setEnableBackups(boolean state) {
            doBackups = state;
        }

        public void setBackupLevels(int n) {
            if (n > 0)
                backups = n;
        }

        public boolean isHtmlEnabled() {
            return genHtml;
        }

        public boolean isXmlEnabled() {
            return genXml;
        }

        public boolean isPlainEnabled() {
            return genPlain;
        }

        public boolean isConfigSectionEnabled() {
            return genConfig;
        }

        public boolean isQuestionLogEnabled() {
            return genQl;
        }

        public boolean isEnvEnabled() {
            return genEnv;
        }

        public boolean isStdEnabled() {
            return genStd;
        }

        public boolean isResultsEnabled() {
            return genResults;
        }

        public boolean isKeywordSummaryEnabled() {
            return genKws;
        }

        public boolean isIndexHtmlEnabled() {
            return indexHtml;
        }

        public boolean isReportHtmlEnabled() {
            return reportHtml;
        }

        public boolean isStateFileEnabled(int status) {
            if (status >= stateFiles.length)
                return false;       // error condition

            return stateFiles[status];
        }

        public boolean isBackupsEnabled() {
            return doBackups;
        }

        public int getBackupLevel() {
            return backups;
        }

        public void setAllowInitFilesOptimize(boolean s) {
            optimizeInitUrl = s;
        }

        public File[] getInitialFiles() {
            if (optimizeInitUrl && initFiles == null) {
                String[] s = ip.getTests();

                if (s == null)
                    s = null;
                else {
                    initFiles = new File[s.length];
                    for (int i = 0; i< s.length; i++)
                        initFiles[i] = new File(s[i]);
                }
            }

            return initFiles;
        }

        public TestFilter getTestFilter() {
            return filter;
        }

        public InterviewParameters getInterview() {
            return ip;
        }

        private static boolean parseBoolean(String s) {
            if (s == null)
                return false;
            else
                return (s.equalsIgnoreCase("true"));
        }

        InterviewParameters ip;
        TestFilter filter;

        // default (legacy) values provided
        boolean genHtml = true;         // generate HTML?
        boolean genPlain = true;        // generate summary.txt?
        boolean genXml = false;         // generate summary.xml?

        boolean genConfig = true;       // generate config section
        boolean genQl = true;
        boolean genEnv = true;
        boolean genStd = true;
        boolean genResults = true;
        boolean genKws = true;

        boolean reportHtml = true;      // use report.html
        boolean indexHtml = false;      // use index.html
        boolean[] stateFiles = new boolean[Status.NUM_STATES];
        boolean doBackups = true;
        int backups = 1;                // backup levels

        // derived info - for caching
        File[] initFiles;
        boolean optimizeInitUrl = false;
    }

    public interface CustomReportManager {
        CustomReport[] getCustomReports();
    }

    public interface StartGenListener {
        void startReportGeneration(Settings s, String reportID);
    }

    public void addStartGenListener(StartGenListener l) {
        if(startGenListeners == null) {
            startGenListeners = new Vector();
        }

        startGenListeners.add(l);
    }

    public void removeStartGeneratingListener(StartGenListener l)  {
        if(startGenListeners != null) {
            startGenListeners.remove(l);
        }
    }

    //---------- data members -----------------------------------------------

    private InterviewParameters params;     // legacy
    private TestResultTable resultTable;        // legacy
    private TestFilter[] paramFilters;      // legacy
    private String[] initUrls;      // legacy
    private File[] initFiles;       // legacy

    // preference constants
    private static final String PREFS_GEN_HTML="rpt.type.html";
    private static final String PREFS_GEN_PLAIN="rpt.type.plain";
    private static final String PREFS_GEN_XML="rpt.type.xml";

    private static final String PREFS_HTML_CONFIG="rpt.html.config";
    private static final String PREFS_HTML_QL="rpt.html.ql";
    private static final String PREFS_HTML_ENV="rpt.html.env";
    private static final String PREFS_HTML_STD="rpt.html.std";
    private static final String PREFS_HTML_RES="rpt.html.res";
    private static final String PREFS_HTML_KWS="rpt.html.kws";
    private static final String PREFS_HTML_REPORTF="rpt.html.reportf";
    private static final String PREFS_HTML_INDEXF="rpt.html.htmlf";
    private static final String PREFS_HTML_STATEF="rpt.html.statef";

    private static final String PREFS_BACK="rpt.bak.enable";
    private static final String PREFS_BACK_NUM="rpt.bak.num";

    private static File reportDir;

    private static I18NResourceBundle i18n = I18NResourceBundle.getBundleForClass(Report.class);

    public static final String MARKER_FILE_NAME = "reportdir.dat";
    public static final String INDEX_FILE_NAME = "index.html";

    private Vector startGenListeners;

}
