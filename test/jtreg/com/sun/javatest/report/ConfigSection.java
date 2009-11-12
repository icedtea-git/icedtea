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
import java.text.NumberFormat;
import java.util.Comparator;
import java.util.Iterator;
import java.util.SortedSet;
import java.util.TreeSet;

import com.sun.interview.WizPrint;
import com.sun.javatest.ExcludeList;
import com.sun.javatest.Keywords;
import com.sun.javatest.Parameters;
import com.sun.javatest.TestEnvironment;
import com.sun.javatest.TestSuite;
import com.sun.javatest.WorkDirectory;
import com.sun.javatest.util.HTMLWriter;
import com.sun.javatest.util.StringArray;

/**
 * A report generator for sets of test results.
 */
class ConfigSection extends HTMLSection {
    ConfigSection(HTMLReport parent, Report.Settings s, File dir) {
        super(i18n.getString("config.title"), s, dir, parent);
    }

    void writeContents(ReportWriter out) throws IOException {
        super.writeContents(out);

        if (settings.isQuestionLogEnabled()) {
            out.startTag(HTMLWriter.UL);
            out.startTag(HTMLWriter.LI);
            out.writeLink(HTMLReport.files[HTMLReport.CONFIG_HTML],
                          i18n.getString("config.confInterview"));
            out.endTag(HTMLWriter.LI);
            out.endTag(HTMLWriter.UL);
        }

        if (settings.isStdEnabled()) {
            out.startTag(HTMLWriter.UL);
            out.startTag(HTMLWriter.LI);
            out.writeLink("#" + HTMLReport.anchors[HTMLReport.SELECT_ANCHOR],
                          i18n.getString("config.selectValue"));
            out.endTag(HTMLWriter.LI);

            if (settings.isEnvEnabled()) {
                out.startTag(HTMLWriter.LI);
                out.writeLink("#" + HTMLReport.anchors[HTMLReport.EXEC_ANCHOR],
                              i18n.getString("config.execValue"));
                out.endTag(HTMLWriter.LI);
            }

            out.startTag(HTMLWriter.LI);
            out.writeLink("#" + HTMLReport.anchors[HTMLReport.LOC_ANCHOR],
                          i18n.getString("config.locValue"));
            out.endTag(HTMLWriter.LI);
            out.endTag(HTMLWriter.UL);
        }
    }

    void writeSummary(ReportWriter out) throws IOException {
        super.writeSummary(out);

        // info about test suite
        out.startTag(HTMLWriter.TABLE);
        out.writeAttr(HTMLWriter.BORDER, 1);
        out.startTag(HTMLWriter.TR);
        out.writeTH(i18n.getString("config.testSuite"), HTMLWriter.ROW);
        out.startTag(HTMLWriter.TD);

        TestSuite ts = settings.ip.getTestSuite();
        if (ts != null)
            out.writeLink(ts.getRoot());
        else
            out.write(i18n.getString("config.noTestSuite"));

        out.endTag(HTMLWriter.TD);
        out.endTag(HTMLWriter.TR);
        out.endTag(HTMLWriter.TABLE);

        // standard values
        if (settings.isStdEnabled())
            writeStdValSummary(out);

        // optional section
        if (settings.isEnvEnabled())
            writeExecutionSummary(out);

        // non-optional section
        // shows workdir, and report dir
        writeLocationSummary(out);
    }

    void writeExtraFiles() throws IOException {
        // optional section
        // tied to question log option
        if (settings.isQuestionLogEnabled())
            writeConfigInterview();

        // optional section
        // tied to writing env values in main report
        if (settings.isEnvEnabled())
            writeEnvironment();

        // tied to writing standard values in main report
        if (settings.isStdEnabled())
            writeExcludeList();
    }

    private void writeStdValSummary(ReportWriter out) throws IOException {
        out.startTag(HTMLWriter.H3);
        out.writeLinkDestination(HTMLReport.anchors[HTMLReport.SELECT_ANCHOR],
                      i18n.getString("config.selectValue"));
        out.endTag(HTMLWriter.H3);

        out.startTag(HTMLWriter.TABLE);
        out.writeAttr(HTMLWriter.BORDER, 1);

        TestSuite ts = settings.ip.getTestSuite();

        String[] tests = settings.ip.getTests();
        out.startTag(HTMLWriter.TR);
        out.writeTH(i18n.getString("config.tests.hdr"), HTMLWriter.ROW);
        out.startTag(HTMLWriter.TD);

        if (tests != null && tests.length > 0) {
            for (int i = 0; i < tests.length; i++) {
                if (i > 0)
                    out.startTag(HTMLWriter.BR);
                File file = new File(ts.getTestsDir(), tests[i]);

                // don't try to link if it doesn't exist _now_
                // this is a combination of legacy behavior (link always)
                // and better behavior for new test suites which are more
                // virtual and that may remap file locations
                if (file.exists())
                    out.writeLink(file, tests[i]);
                else
                    out.write(tests[i]);
            }
        }
        else {
            out.write(i18n.getString("config.tests.all"));
        }
        out.endTag(HTMLWriter.TD);
        out.endTag(HTMLWriter.TR);

        out.startTag(HTMLWriter.TR);
        out.writeTH(i18n.getString("config.previous.hdr"), HTMLWriter.ROW);

        boolean[] b = settings.ip.getPriorStatusValues();
        if (b != null) {
            String[] ss = { i18n.getString("config.status.passed"),
                            i18n.getString("config.status.failed"),
                            i18n.getString("config.status.error"),
                            i18n.getString("config.status.notRun") };

            StringBuffer sb = new StringBuffer();

            for (int i = 0; i < b.length; i++) {
                if (b[i]) {
                    if (sb.length() > 0)
                        sb.append(" or "); // XXX needs i18n
                    sb.append(ss[i]);
                }
             }  // for

            out.writeTD(sb.toString());
        }
        else
            out.writeTD(i18n.getString("config.previous.none"));

        out.endTag(HTMLWriter.TR);

        // Print exclude list summary
        // NOTE: there may be more than one exclude list
        //       the single ExcludeList from params contains the union

        out.startTag(HTMLWriter.TR);
        out.writeTH(i18n.getString("config.excludeTests"), HTMLWriter.ROW);
        out.startTag(HTMLWriter.TD);

        ExcludeList excludeList = settings.ip.getExcludeList();
        if (excludeList != null) {
            // content cell
            out.writeI18N("config.entries", new Object[] { new Integer(excludeList.size()) });

            Parameters.ExcludeListParameters exclParams = settings.ip.getExcludeListParameters();
            File[] excludeFiles = null;
            if (exclParams instanceof Parameters.MutableExcludeListParameters)
                excludeFiles =
                    ((Parameters.MutableExcludeListParameters) (exclParams)).getExcludeFiles();

            if (excludeFiles != null && excludeFiles.length > 0) {
                for (int i = 0; i < excludeFiles.length; i++) {
                    out.startTag(HTMLWriter.BR);
                    out.writeLink(HTMLReport.files[HTMLReport.EXCLUDED_HTML],
                                    excludeFiles[i].getPath());
                }   // for
            }
        }
        else
            out.write(i18n.getString("config.jtx.nofiles"));

        out.endTag(HTMLWriter.TD);
        out.endTag(HTMLWriter.TR);

        // do not display if immutable?
        int concurrency = settings.ip.getConcurrency();
        out.startTag(HTMLWriter.TR);
        out.writeTH(i18n.getString("config.concurrency"), HTMLWriter.ROW);
        out.startTag(HTMLWriter.TD);
        out.write(Integer.toString(concurrency));
        out.endTag(HTMLWriter.TD);
        out.endTag(HTMLWriter.TR);

        float timeout = settings.ip.getTimeoutFactor();
        out.startTag(HTMLWriter.TR);
        out.writeTH(i18n.getString("config.timeOut"), HTMLWriter.ROW);
        out.startTag(HTMLWriter.TD);
        out.write(NumberFormat.getInstance().format(timeout));
        out.endTag(HTMLWriter.TD);
        out.endTag(HTMLWriter.TR);

        // keywords
        out.startTag(HTMLWriter.TR);
        out.writeTH(i18n.getString("config.keywords.hdr"), HTMLWriter.ROW);

        Keywords keywords = settings.ip.getKeywords();
        if (keywords != null) {
            out.writeTD(settings.ip.getKeywords().toString());
        }
        else {
            out.writeTD(i18n.getString("config.keywords.none"));
        }
        out.endTag(HTMLWriter.TR);

        out.endTag(HTMLWriter.TABLE);
    }

    private void writeExecutionSummary(ReportWriter out) throws IOException {
        out.startTag(HTMLWriter.H3);
        out.writeLinkDestination(HTMLReport.anchors[HTMLReport.EXEC_ANCHOR],
                      i18n.getString("config.execValue"));
        out.endTag(HTMLWriter.H3);

        out.startTag(HTMLWriter.TABLE);
        out.writeAttr(HTMLWriter.BORDER, 1);

        File[] envFiles = null;

        // Question Log
        if (settings.isQuestionLogEnabled()) {
            out.startTag(HTMLWriter.TR);
            out.writeTH(i18n.getString("config.configInterview"), HTMLWriter.ROW);
            out.startTag(HTMLWriter.TD);
            String name = settings.ip.getName();
            if (name == null)
                out.writeLink(HTMLReport.files[HTMLReport.CONFIG_HTML],
                              i18n.getString("config.confInterview"));
            else
                out.writeLink(HTMLReport.files[HTMLReport.CONFIG_HTML], name);

            if (!settings.ip.isValid())
               out.writeWarning(i18n.getString("config.intIncomplete"));
            out.endTag(HTMLWriter.TD);
            out.endTag(HTMLWriter.TR);
        }

        // env
        Parameters.EnvParameters envParams = settings.ip.getEnvParameters();
        if (envParams != null &&
            envParams instanceof Parameters.LegacyEnvParameters) {
            envFiles = ((Parameters.LegacyEnvParameters) envParams).getEnvFiles();
            if (envFiles != null) {
                out.startTag(HTMLWriter.TR);
                out.writeTH(i18n.getString("config.envFiles"), HTMLWriter.ROW);
                out.startTag(HTMLWriter.TD);
                for (int i = 0; i < envFiles.length; i++) {
                    out.writeLink(new File(envFiles[i].getPath()));
                    out.startTag(HTMLWriter.BR);
                }
                out.endTag(HTMLWriter.TD);
                out.endTag(HTMLWriter.TR);
            }
        }

        TestEnvironment env = settings.ip.getEnv();
        String envName = (env != null ? env.getName() : null);

        if (envName != null && envName.length() > 0) {
            out.startTag(HTMLWriter.TR);
            out.writeTH(i18n.getString("config.env"), HTMLWriter.ROW);
            out.startTag(HTMLWriter.TD);
            out.writeLink(HTMLReport.files[HTMLReport.ENV_HTML], envName);
            out.endTag(HTMLWriter.TD);
            out.endTag(HTMLWriter.TR);
        }

        out.endTag(HTMLWriter.TABLE);
    }

    private void writeLocationSummary(ReportWriter out) throws IOException {
        WorkDirectory workDir = settings.ip.getWorkDirectory();
        out.startTag(HTMLWriter.H3);
        out.writeLinkDestination(HTMLReport.anchors[HTMLReport.LOC_ANCHOR],
                      i18n.getString("config.locValue"));
        out.endTag(HTMLWriter.H3);

        out.startTag(HTMLWriter.TABLE);
        out.writeAttr(HTMLWriter.BORDER, 1);

        out.startTag(HTMLWriter.TR);
        out.writeTH(i18n.getString("config.wd"), HTMLWriter.ROW);
        out.startTag(HTMLWriter.TD);
        out.writeLink(workDir.getRoot());
        out.endTag(HTMLWriter.TD);
        out.endTag(HTMLWriter.TR);

        out.startTag(HTMLWriter.TR);
        out.writeTH(i18n.getString("config.rd"), HTMLWriter.ROW);
        out.startTag(HTMLWriter.TD);
        out.writeLink(reportDir);
        out.endTag(HTMLWriter.TD);
        out.endTag(HTMLWriter.TR);

        out.endTag(HTMLWriter.TABLE);
    }

    private void writeConfigInterview() throws IOException {
        WizPrint wp = new WizPrint(settings.ip);
        wp.setShowResponses(true);
        wp.write(openWriter(HTMLReport.CONFIG_HTML));
    }

    private void writeEnvironment() throws IOException {
        TestEnvironment env = settings.ip.getEnv();
        String envName = (env != null) ? env.getName() : null;

        ReportWriter out = openAuxFile(HTMLReport.ENV_HTML,
                    i18n.getString("config.env.title", envName), i18n);

        if (env == null) {
            out.write(i18n.getString("config.noEnv"));
            return;
        }


        String desc = env.getDescription();

        if (desc != null) {
            out.startTag(HTMLWriter.H2);
            out.write(desc);
            out.endTag(HTMLWriter.H2);
        }

        SortedSet envTable = new TreeSet(new StringArrayComparator());

        for (Iterator i = env.elements().iterator(); i.hasNext(); ) {
            TestEnvironment.Element envElem = (TestEnvironment.Element) (i.next());
            String[] envTableRow = {envElem.getKey(), envElem.getValue()};
            envTable.add(envTableRow);
        }

        out.startTag(HTMLWriter.TABLE);
        out.writeAttr(HTMLWriter.BORDER, 1);
        for (Iterator i = envTable.iterator(); i.hasNext(); ) {
            String[] envEntry = (String[]) (i.next());
            out.startTag(HTMLWriter.TR);

            for (int j = 0; j < envEntry.length; j++ ) {
                out.startTag(HTMLWriter.TD);
                out.write(envEntry[j]);
                out.endTag(HTMLWriter.TD);
            }

            out.endTag(HTMLWriter.TR);
        }
        out.endTag(HTMLWriter.TABLE);
        out.close();
    }

    private void writeExcludeList() throws IOException {
        ReportWriter out = openAuxFile(HTMLReport.EXCLUDED_HTML,
                            i18n.getString("config.excludedTests"), i18n);

        ExcludeList excludeList = settings.ip.getExcludeList();

        if (excludeList == null || excludeList.size() == 0){
            out.writeI18N("config.excl.none");
        }
        else {
            SortedSet sortedEntries = new TreeSet(new ExcludeListEntryComparator());
            for (Iterator iter = excludeList.getIterator(false); iter.hasNext(); )
                sortedEntries.add(iter.next());

            out.startTag(HTMLWriter.TABLE);
            out.writeAttr(HTMLWriter.BORDER, 1);
            out.startTag(HTMLWriter.TR);
            out.startTag(HTMLWriter.TH);
            out.writeI18N("config.excl.name");
            out.endTag(HTMLWriter.TH);
            out.startTag(HTMLWriter.TH);
            out.writeI18N("config.excl.testcase");
            out.endTag(HTMLWriter.TH);
            out.startTag(HTMLWriter.TH);
            out.writeI18N("config.excl.bugids");
            out.endTag(HTMLWriter.TH);
            out.startTag(HTMLWriter.TH);
            out.writeI18N("config.excl.platforms");
            out.endTag(HTMLWriter.TH);
            out.startTag(HTMLWriter.TH);
            out.writeI18N("config.excl.synopsis");
            out.endTag(HTMLWriter.TH);
            out.endTag(HTMLWriter.TR);

            for (Iterator iter = sortedEntries.iterator(); iter.hasNext(); ) {
                ExcludeList.Entry e = (ExcludeList.Entry) (iter.next());
                out.startTag(HTMLWriter.TR);
                writeTD(out, e.getRelativeURL());
                writeTD(out, e.getTestCases());

                out.startTag(HTMLWriter.TD);
                String[] bugIds = e.getBugIdStrings();
                for (int i = 0; i < bugIds.length; i++) {
                    if (i > 0)
                        out.write(" ");
                    out.write(bugIds[i]);
                }
                out.endTag(HTMLWriter.TD);

                writeTD(out, StringArray.join(e.getPlatforms()));
                writeTD(out, e.getSynopsis());

                out.endTag(HTMLWriter.TR);
            }
            out.endTag(HTMLWriter.TABLE);
        }

        out.newLine();
        out.close();
    }

    private void writeTD(ReportWriter out, String text) throws IOException {
        out.startTag(HTMLWriter.TD);
        if (text == null || text.length() ==0)
            out.writeEntity("&nbsp;");
        else
            out.write(text);
        out.endTag(HTMLWriter.TD);
    }

    private static class ExcludeListEntryComparator implements Comparator
    {
        public int compare(Object o1, Object o2) {
            ExcludeList.Entry e1 = (ExcludeList.Entry) o1;
            ExcludeList.Entry e2 = (ExcludeList.Entry) o2;
            int x = compare(e1.getRelativeURL(), e2.getRelativeURL());
            if (x == 0)
                x = compare(e1.getTestCases(), e2.getTestCases());
            return x;

        }
        private static int compare(String[] a, String[] b) {
            int alen = (a == null ? 0 : a.length);
            int blen = (b == null ? 0 : b.length);
            for (int i = 0; i < Math.min(alen, blen); i++) {
                int c = compare(a[i], b[i]);
                if (c != 0)
                    return c;
            }
            return (alen < blen ? -1 : alen == blen ? 0 : +1);
    }

        private static int compare(String a, String b) {
            return (a == null && b == null ? 0
                    : a == null ? -1
                    : b == null ? +1
                    : a.compareTo(b));
        }
    }
}
