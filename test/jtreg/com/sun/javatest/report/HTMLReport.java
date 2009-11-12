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
import java.io.Writer;
import java.util.Vector;

import com.sun.javatest.Status;
import com.sun.javatest.util.BackupPolicy;
import com.sun.javatest.util.HTMLWriter;
import com.sun.javatest.util.I18NResourceBundle;
import java.io.BufferedWriter;
import java.io.FileWriter;

/**
 * HTML format of the report.
 */
class HTMLReport implements ReportFormat {
    HTMLReport(I18NResourceBundle rb) {
        i18n = rb;
        HTMLSection.i18n = rb;
    }

    public void write(Report.Settings s, File dir) throws IOException {
        reportDir = dir;

        Vector mainSecs = new Vector(3);
        Vector auxSecs = new Vector(3);

        // optional section
        ConfigSection cs = new ConfigSection(this, s, dir);
        if (s.isConfigSectionEnabled()) {
            mainSecs.addElement(cs);
            auxSecs.addElement(cs);
        }

        // optional section
        // create instance only if we are generating the summary or
        // one or more result files (failed.html, ...)

        // slightly workaround ifs here to prevent unnecessary
        // initialization if it is not going to be used
        ResultSection rs = null;
        if (s.isResultsEnabled()) {
            rs = new ResultSection(this, s, dir);
            mainSecs.addElement(rs);
        }

        if (s.isStateFileEnabled(Status.PASSED) ||
            s.isStateFileEnabled(Status.ERROR) ||
            s.isStateFileEnabled(Status.NOT_RUN) ||
            s.isStateFileEnabled(Status.FAILED)) {
            if (rs == null)
                rs = new ResultSection(this, s, dir);
            auxSecs.addElement(rs);
        }

        // optional section
        if (s.isKeywordSummaryEnabled()) {
            mainSecs.addElement(new StatisticsSection(this, s, dir));
            auxSecs.addElement(new StatisticsSection(this, s, dir));
        }

        HTMLSection[] mainSections = new HTMLSection[mainSecs.size()];
        mainSecs.copyInto(mainSections);

        HTMLSection[] auxSections = new HTMLSection[auxSecs.size()];
        auxSecs.copyInto(auxSections);

        // prepare main report file
        Writer writer = null;
        if (s.reportHtml && s.indexHtml) {
            writer = new DuplexWriter(
                        openWriter(reportDir, REPORT_HTML),
                        openWriter(reportDir, INDEX_HTML));
        }
        else if (s.reportHtml) {
            writer = openWriter(reportDir, REPORT_HTML);
        }
        else if (s.indexHtml) {
            writer = openWriter(reportDir, INDEX_HTML);
        }
        else {
            // no main report output specified in settings
        }

        // if the writer is null, the user did not ask for the main
        // report
        ReportWriter.initializeDirectory(reportDir);
        if (writer != null) {
            ReportWriter out = new ReportWriter(writer,
                            i18n.getString("report.title"), i18n);

            // test suite name
            String testSuiteName = s.ip.getTestSuite().getName();
            if (testSuiteName != null) {
                out.startTag(HTMLWriter.H2);
                out.writeI18N("report.testSuite", testSuiteName);
                out.endTag(HTMLWriter.H2);
            }

            // info from sections for main report
            out.startTag(HTMLWriter.UL);
            for (int i = 0; i < mainSections.length; i++) {
                out.startTag(HTMLWriter.LI);
                mainSections[i].writeContents(out);
                out.endTag(HTMLWriter.LI);
            }
            out.endTag(HTMLWriter.UL);

            for (int i = 0; i < mainSections.length; i++) {
                out.startTag(HTMLWriter.P);
                out.startTag(HTMLWriter.HR);
                mainSections[i].writeSummary(out);
                out.newLine();
            }

            out.close();
        }

        for (int i = 0; i < auxSections.length; i++) {
            auxSections[i].writeExtraFiles();
        }
    }

    // --------------- Utility Methods --------------------------------------

    /**
     * Gets the standard report file name used in JT Harness.
     * Note that this returns the file names which are used for the main
     * report only, not the aux. HTML files.
     * @return The report name.
     */
    public static String[] getReportFilenames() {
        return new String[] {REPORT_NAME, NEW_REPORT_NAME};
    }

    /**
     * Gets the file name based one the input code.
     * @param code The code name for the file.
     * @return The file name.
     */
    public static String getFile(int code) {
        return files[code];
    }

    static String[] getFilenamesUsed() {
        return files;
    }

    File getReportDirectory() {
        return reportDir;
    }

    Writer openWriter(File reportDir, int code) throws IOException {
        return new BufferedWriter(new FileWriter(new File(reportDir, files[code])));
    }

    // ----------------------------------------------------------------------

    File reportDir;
    private BackupPolicy backupPolicy;
    private I18NResourceBundle i18n;

    // The name of the root file for a set of report files.
    static final String REPORT_NAME = "report.html";
    static final String NEW_REPORT_NAME = "index.html";

    // html anchors to be used in the output
    static final String[] anchors = {
       "selection",
       "execution",
       "locations",
       "keywordSummary"
    };

    // The following must be kept in sync with the preceding list
    static final int
        SELECT_ANCHOR  = 0,
        EXEC_ANCHOR    = 1,
        LOC_ANCHOR     = 2,
        KEYWORD_ANCHOR = 3;

    static final String[] files = {
        REPORT_NAME,
        NEW_REPORT_NAME,
        "config.html",
        "env.html",
        "excluded.html",
        "passed.html",
        "failed.html",
        "error.html",
        "notRun.html"
    };

    // The following must be kept in sync with the preceding list
    static final int
        REPORT_HTML = 0,
        INDEX_HTML = 1,
        CONFIG_HTML = 2,
        ENV_HTML = 3,
        EXCLUDED_HTML = 4,
        PASSED_HTML = 5,
        FAILED_HTML = 6,
        ERROR_HTML = 7,
        NOTRUN_HTML = 8;

    // -------------------- Inner Class --------------------------------------

    /**
     * Duplicates output onto n writers.
     */
    static class DuplexWriter extends Writer {
        public DuplexWriter(Writer[] writers) {
            if (writers == null)
                return;

            targets = new Writer[writers.length];
            System.arraycopy(writers, 0, targets, 0, writers.length);
        }

        public DuplexWriter(Writer w1, Writer w2) {
            targets = new Writer[2];
            targets[0] = w1;
            targets[1] = w2;
        }

        public void close() throws IOException {
            for (int i = 0; i < targets.length; i++)
                targets[i].close();
        }

        public void flush() throws IOException {
            for (int i = 0; i < targets.length; i++)
                targets[i].flush();
        }

        public void write(char[] cbuf) throws IOException {
            for (int i = 0; i < targets.length; i++)
                targets[i].write(cbuf);
        }

        public void write(char[] cbuf, int off, int len) throws IOException {
            for (int i = 0; i < targets.length; i++)
                targets[i].write(cbuf, off, len);
        }

        public void write(int c) throws IOException {
            for (int i = 0; i < targets.length; i++)
                targets[i].write(c);
        }

        public void write(String str) throws IOException {
            for (int i = 0; i < targets.length; i++)
                targets[i].write(str);
        }

        public void write(String str, int off, int len) throws IOException {
            for (int i = 0; i < targets.length; i++)
                targets[i].write(str, off, len);
        }

        private Writer[] targets;
    }

}
