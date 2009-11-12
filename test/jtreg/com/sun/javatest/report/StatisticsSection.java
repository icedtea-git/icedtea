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
import java.util.Arrays;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Vector;

import com.sun.javatest.JavaTestError;
import com.sun.javatest.Status;
import com.sun.javatest.TestDescription;
import com.sun.javatest.TestFilter;
import com.sun.javatest.TestResult;
import com.sun.javatest.TestResultTable;
import com.sun.javatest.util.HTMLWriter;
import com.sun.javatest.util.StringArray;

class StatisticsSection extends HTMLSection {
    StatisticsSection(HTMLReport parent, Report.Settings set, File dir) {
        super(i18n.getString("stats.title"), set, dir, parent);
        initFiles = settings.getInitialFiles();

        resultTable = settings.ip.getWorkDirectory().getTestResultTable();
        Iterator iter =  null;
        try {
            iter = (initFiles == null) ?
                        resultTable.getIterator(new TestFilter[] {settings.filter}) :
                        resultTable.getIterator(initFiles, new TestFilter[]
                                                {settings.filter});
        }
        catch (TestResultTable.Fault f) {
            throw new JavaTestError(i18n.getString("stats.testResult.err"));
        }       // catch

        for (; iter.hasNext(); ) {
            TestResult tr = (TestResult) (iter.next());

            try {
                Status s = tr.getStatus();
                TestDescription td = tr.getDescription();

                String[] keys = td.getKeywords();
                Arrays.sort(keys);
                String sortedKeys = StringArray.join(keys);

                int[] v = (int[])(keywordTable.get(sortedKeys));
                if (v == null) {
                    v = new int[Status.NUM_STATES];
                    keywordTable.put(sortedKeys, v);
                }
                v[s.getType()]++;

                statusTotals[s.getType()]++;
            }
            catch (TestResult.Fault ex) {
                // hmmm. Could count problem files here and report on them later
            }
        }
    }

    void writeContents(ReportWriter out) throws IOException {
        // arguably, this should be conditional on whether
        // the test suite has tests that use keywords!

        super.writeContents(out);

        out.startTag(HTMLWriter.UL);
        out.startTag(HTMLWriter.LI);
        out.writeLink("#" + HTMLReport.anchors[HTMLReport.KEYWORD_ANCHOR],
                      i18n.getString("stats.keywordValue"));
        out.endTag(HTMLWriter.UL);
        out.newLine();
    }

    void writeSummary(ReportWriter out) throws IOException {
        // arguably, this should be conditional on whether
        // the test suite has tests that use keywords!

        super.writeSummary(out);
        writeKeywordSummary(out);
    }

    private void writeKeywordSummary(ReportWriter out) throws IOException {
        // arguably, the following logic to create the keyword table
        // should be done in the constructor, so that we can optimize
        // out the contents and summary if the do not provide any
        // significant data
        // -- or else, we could just report "test suite does not use keywords"
        // instead of a mostly empty table

        // compute the keyword statistics

        int ncols = 2; // keywords, total
        for (int i = 0; i < statusTotals.length; i++)
            if (statusTotals[i] > 0)
                ncols++;

        String[] head = new String[ncols];
        {
            int c = 0;
            head[c++] = i18n.getString("stats.keyword");
            for (int i = 0; i < statusTotals.length; i++)
                if (statusTotals[i] > 0)
                    head[c++] = headings[i];
            head[c] = i18n.getString("stats.total");
        }

        Vector v = new Vector();
        for (Iterator iter = keywordTable.entrySet().iterator(); iter.hasNext(); ) {
            Map.Entry e = (Map.Entry) (iter.next());
            String k = (String) (e.getKey());
            int[] kv = (int[]) (e.getValue());
            String[] newEntry = new String[ncols];
            int c = 0, total = 0;
            newEntry[c++] = k;
            for (int i = 0; i < kv.length; i++) {
                if (statusTotals[i] != 0)
                    newEntry[c++] = (kv[i] == 0 ? "" : Integer.toString(kv[i]));
                total += kv[i];
            }
            newEntry[c] = Integer.toString(total);

        sortedInsert:
            {
                for (int i = 0; i < v.size(); i++) {
                    String[] entry = (String[])v.elementAt(i);
                    if (k.compareTo(entry[0]) < 0) {
                        v.insertElementAt(newEntry, i);
                        break sortedInsert;
                    }
                }
                v.addElement(newEntry);
            }
        }

        {
            String[] totalsEntry = new String[ncols];
            int c = 0, total = 0;
            totalsEntry[c++] = i18n.getString("stats.total");
            for (int i = 0; i < statusTotals.length; i++) {
                if (statusTotals[i] != 0)
                    totalsEntry[c++] = Integer.toString(statusTotals[i]);
                total += statusTotals[i];
            }
            totalsEntry[c] = Integer.toString(total);
            v.addElement(totalsEntry);
        }

        String[][] table = new String[v.size()][];
        v.copyInto(table);

        // write out the keyword statistics

        out.startTag(HTMLWriter.H3);
        out.writeLinkDestination(HTMLReport.anchors[HTMLReport.KEYWORD_ANCHOR],
                      i18n.getString("stats.keywordValue"));
        out.endTag(HTMLWriter.H3);
        out.newLine();

        // write out the table of keyword statistics

        out.startTag(HTMLWriter.TABLE);
        out.writeAttr(HTMLWriter.BORDER, 1);

        out.startTag(HTMLWriter.TR);
        for (int c = 0; c < head.length; c++) {
            out.startTag(HTMLWriter.TH);
            out.writeAttr(HTMLWriter.ALIGN, (c == 0 ? HTMLWriter.LEFT : HTMLWriter.RIGHT));
            out.write(head[c]);
            out.endTag(HTMLWriter.TH);
        }
        out.endTag(HTMLWriter.TR);

        for (int r = 0; r < table.length; r++) {
            out.startTag(HTMLWriter.TR);
            for (int c = 0; c < table[r].length; c++) {
                out.startTag(HTMLWriter.TD);
                out.writeAttr(HTMLWriter.ALIGN, (c == 0 ? HTMLWriter.LEFT : HTMLWriter.RIGHT));
                out.write(table[r][c]);
                out.endTag(HTMLWriter.TD);
            }
            out.endTag(HTMLWriter.TR);
        }
        out.endTag(HTMLWriter.TABLE);
    }

    //-----------------------------------------------------------------------

    private TestResultTable resultTable;
    private File[] initFiles;
    private Map keywordTable = new HashMap();
    private int[] statusTotals = new int[Status.NUM_STATES];

    private final String[] headings = {
        i18n.getString("stats.heading.passed"),
        i18n.getString("stats.heading.failed"),
        i18n.getString("stats.heading.error"),
        i18n.getString("stats.heading.notRun")
    };
}
