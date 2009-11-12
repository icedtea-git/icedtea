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
import java.util.Iterator;
import java.util.SortedSet;
import java.util.TreeSet;

import com.sun.javatest.JavaTestError;
import com.sun.javatest.Status;
import com.sun.javatest.TestDescription;
import com.sun.javatest.TestFilter;
import com.sun.javatest.TestResult;
import com.sun.javatest.TestResultTable;
import com.sun.javatest.util.HTMLWriter;

/**
 * Summarize the status, pass/fail/error of the tests which we are reporting on.
 * Also generate output in failed.html, error.html, etc...
 */
class ResultSection extends HTMLSection {
    ResultSection(HTMLReport parent, Report.Settings settings, File dir) {
        super(i18n.getString("result.title"), settings, dir, parent);

        workDirRoot = settings.ip.getWorkDirectory().getRoot();

        // if the report directory is in the work directory,
        // adjust workDirRoot to be the relative path from the
        // report dir back to the work dir root

        /*
        File reportDir = model.getReportDir();
        try {
            reportDir = reportDir.getCanonicalFile();
        }
        catch (IOException e) {
            // ignore, leave reportDir set to absolute file
        }
        */

        String workPath ;
        String reportDirPath ;

        try {
            workPath = workDirRoot.getCanonicalPath();
            reportDirPath = reportDir.getCanonicalPath();
        } catch (IOException e) {
            workPath = workDirRoot.getPath();
            reportDirPath = reportDir.getPath();
        }

        if (!workPath.endsWith(File.separator))
            workPath += File.separator;

        if (reportDirPath.startsWith(workPath)) {
            File d = reportDir;
            StringBuffer sb = new StringBuffer();
            try {
                while (d != null && !d.getCanonicalPath().equals(workDirRoot.getCanonicalPath())) {
                    sb.append("../");
                    d = d.getParentFile();
                }
            } catch (IOException e) {
                d = null;
            }
            if (d != null) {
                workDirRoot = new File(sb.toString());
            }
        }

        resultTable = settings.ip.getWorkDirectory().getTestResultTable();
        initFiles = settings.getInitialFiles();

        lists = new SortedSet[Status.NUM_STATES];
        for (int i = 0; i < lists.length; i++ )
            lists[i] = new TreeSet(new TestResultsByStatusAndTitleComparator());

        Iterator iter;
        try {
            TestFilter[] fs = null;

            // Note: settings.filter should not really be null, modernized clients
            //   of this class should set the filter before asking for a report.
            if (settings.filter == null)
                fs = new TestFilter[0];
            else
                fs = new TestFilter[] {settings.filter};


            iter = ((initFiles == null)
                    ? resultTable.getIterator(fs)
                    : resultTable.getIterator(initFiles, fs));
        }
        catch (TestResultTable.Fault f) {
            throw new JavaTestError(i18n.getString("result.testResult.err"));
        }

        for (; iter.hasNext(); ) {
            TestResult tr = (TestResult) (iter.next());
            Status s = tr.getStatus();
            SortedSet list = lists[s == null ? Status.NOT_RUN : s.getType()];
            list.add(tr);
            totalFound++;
        }
    }

    void writeSummary(ReportWriter out) throws IOException {
        super.writeSummary(out);

        out.startTag(HTMLWriter.TABLE);
        out.writeAttr(HTMLWriter.BORDER, 1);

        for (int i = 0; i < lists.length; i++ ) {
            // since reportFile is in reportDir, generate a relative URL
            String reportFile = HTMLReport.files[fileCodes[i]];
            SortedSet l = lists[i];

            int n = l.size();
            if (n > 0) {
                out.startTag(HTMLWriter.TR);
                out.writeTH(headings[i], HTMLWriter.ROW);
                out.startTag(HTMLWriter.TD);

                // no link if file generation for that status
                // is disabled
                if (settings.isStateFileEnabled(i))
                    out.writeLink(reportFile, Integer.toString(n));
                else
                    out.write(Integer.toString(n));

                out.endTag(HTMLWriter.TD);
                out.endTag(HTMLWriter.TR);
            }
        }

        out.startTag(HTMLWriter.TR);
        out.writeTH(i18n.getString("result.total"), HTMLWriter.ROW);
        out.writeTD(Integer.toString(totalFound));

        out.endTag(HTMLWriter.TR);
        out.endTag(HTMLWriter.TABLE);
    }

    void writeExtraFiles() throws IOException {
        writeStatusFiles();
    }

    private void writeStatusFiles() throws IOException {
        for (int i = 0; i < lists.length; i++ ) {
            // each file is optional
            if (!settings.isStateFileEnabled(i))
                continue;

            ReportWriter out = openAuxFile(fileCodes[i], headings[i], i18n);
            out.write(i18n.getString("result.groupByStatus"));
            try {
                SortedSet list = lists[i];
                if (list.size() > 0) {
                    boolean inList = false;
                    String currentHead = null;
                    for (Iterator iter = list.iterator(); iter.hasNext(); ) {
                        TestResult e = (TestResult) (iter.next());
                        String title;
                        try {
                            TestDescription e_td = e.getDescription();
                            title = e_td.getTitle();
                        }
                        catch (TestResult.Fault ex) {
                            title = null;
                        }

                        Status e_s = e.getStatus();
                        if (!e_s.getReason().equals(currentHead)) {
                            currentHead = e_s.getReason();
                            if (inList) {
                                inList = false;
                                out.endTag(HTMLWriter.UL);
                                out.newLine();
                            }
                            out.startTag(HTMLWriter.H4);
                            out.write(currentHead.length() == 0 ? i18n.getString("result.noReason") : currentHead);
                            out.endTag(HTMLWriter.H4);
                            out.newLine();
                        }
                        if (!inList) {
                            inList = true;
                            out.startTag(HTMLWriter.UL);
                        }
                        out.startTag(HTMLWriter.LI);

                        //File eFile = e.getFile();
                        String eWRPath = e.getWorkRelativePath();
                        File eFile = new File(workDirRoot, eWRPath.replace('/', File.separatorChar));
                        String eName = e.getTestName();
                        if (eFile == null || e_s.getType() == Status.NOT_RUN)
                            out.write(eName);
                        else
                            out.writeLink(eFile, eName);

                        if (title != null)
                            out.write(": " + title);
                        out.newLine();
                    }
                    if (inList) {
                        inList = false;
                        out.endTag(HTMLWriter.UL);
                    }
                }
            }
            finally {
                out.close();
            }
       }
    }

    private File workDirRoot;
    private TestResultTable resultTable;
    private File[] initFiles;
    private TestFilter[] paramFilters;

    private SortedSet[] lists;
    private int totalFound;

    private final int[] fileCodes = {
        HTMLReport.PASSED_HTML,
        HTMLReport.FAILED_HTML,
        HTMLReport.ERROR_HTML,
        HTMLReport.NOTRUN_HTML
    };

    private final String[] headings = {
        i18n.getString("result.heading.passed"),
        i18n.getString("result.heading.failed"),
        i18n.getString("result.heading.errors"),
        i18n.getString("result.heading.notRun")
    };
}
