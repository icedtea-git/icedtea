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
import java.util.Iterator;
import java.util.SortedSet;
import java.util.TreeSet;

import com.sun.javatest.JavaTestError;
import com.sun.javatest.TestFilter;
import com.sun.javatest.TestResult;
import com.sun.javatest.TestResultTable;
import com.sun.javatest.util.BackupPolicy;
import com.sun.javatest.util.I18NResourceBundle;
import com.sun.javatest.util.TextWriter;
import java.io.BufferedWriter;
import java.io.FileWriter;

/**
 * Plain text version of the report.
 */
class PlainTextReport implements ReportFormat {
    PlainTextReport(I18NResourceBundle bundle) {
        i18n = bundle;
    }

    public static String[] getReportFilenames() {
        return files;
    }

    public static String[] getFilenamesUsed() {
        return files;
    }

    public void write(Report.Settings s, File dir) throws IOException {
        TestResultTable resultTable = s.ip.getWorkDirectory().getTestResultTable();

        File[] initFiles = s.getInitialFiles();

        SortedSet tests = new TreeSet(new TestResultsByFileComparator());
        int width = 0;

        Iterator iter = null;
        try {
            if (initFiles == null)
                iter = resultTable.getIterator(new TestFilter[] {s.filter});
            else
                iter = resultTable.getIterator(initFiles,
                                        new TestFilter[] {s.filter});
        }
        catch (TestResultTable.Fault f) {
            throw new JavaTestError(i18n.getString("report.testResult.err"));
        }   // catch

        for (; iter.hasNext(); ) {
            TestResult tr = (TestResult) (iter.next());
            // build a list of TestResults, sorted by test name
            width = Math.max(width, tr.getTestName().length());
            tests.add(tr);
        }

        TextWriter out = new TextWriter(openWriter(dir, files[SMRY_TXT]));
        for (iter = tests.iterator(); iter.hasNext(); ) {
            TestResult tr = (TestResult) (iter.next());
            String u = tr.getTestName();
            out.print(u);
            for (int sp = u.length(); sp < width; sp++)
                out.print(" ");
            out.print("  ");
            out.println(tr.getStatus().toString());
        }
        out.close();
    }

    private Writer openWriter(File reportDir, String filename) throws IOException {
        return new BufferedWriter(new FileWriter(new File(reportDir, filename)));
    }

    // these fields must have synchronized indexes
    private static final String[] files = {"summary.txt"};
    private static final int SMRY_TXT = 0;

    private I18NResourceBundle i18n;
    private BackupPolicy backupPolicy;
}
