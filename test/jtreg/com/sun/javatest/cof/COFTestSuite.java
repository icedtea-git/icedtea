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
package com.sun.javatest.cof;

import java.io.File;
import java.io.IOException;
import java.util.Iterator;
import java.util.regex.Pattern;

import com.sun.javatest.Status;
import com.sun.javatest.TestResult;
import com.sun.javatest.TestResultTable;
import com.sun.javatest.TestSuite;
import com.sun.javatest.WorkDirectory;
import com.sun.javatest.util.I18NResourceBundle;
import com.sun.javatest.util.XMLWriter;

class COFTestSuite extends COFItem {

        private static final String[] cofStatus = new String[Status.NUM_STATES];

        private static I18NResourceBundle i18n = I18NResourceBundle
                        .getBundleForClass(COFTestSuite.class);

        static {
                cofStatus[Status.PASSED] = "pass";
                cofStatus[Status.FAILED] = "fail";
                cofStatus[Status.ERROR] = "error";
                cofStatus[Status.NOT_RUN] = "did_not_run";
        }

        private COFData cofData;

        private boolean legacyMode = false; // modern workdir or not

        private String name;

        protected Pattern testCasePattern = Pattern
                        .compile("^(\\S+): (Passed\\.|Failed\\.|Error\\.|Not\\ run\\.)(.*)");


        private TestResultTable trt;

        COFTestSuite(File dir) {
                trt = new TestResultTable();
                scan(dir);
                legacyMode = true;
        }

        COFTestSuite(File dir, COFData cd) {
                cofData = cd;
                trt = new TestResultTable();
                name = cofData.get("testsuites.testsuite.name");
                scan(dir);
                legacyMode = true;
        }

        COFTestSuite(TestResultTable trt) {
                this.trt = trt;
        }

        COFTestSuite(WorkDirectory wd) {
                this(wd, null);
        }

        COFTestSuite(WorkDirectory wd, COFData cd) {
                cofData = cd;
                TestSuite ts = wd.getTestSuite();
                name = ts.getID();
                trt = wd.getTestResultTable();
        }

        void scan(File dir) {
                String[] entries = dir.list();
                if (entries != null) {
                        for (int i = 0; i < entries.length; i++) {
                                File f = new File(dir, entries[i]);
                                if (f.isDirectory())
                                        scan(f);
                                else if (TestResult.isResultFile(f)) {
                                        try {
                                                TestResult tr = new TestResult(f);
                                                trt.update(tr);
                                        } catch (TestResult.Fault e) {
                                                // ignore errors for now
                                                // an error handler might report errors to stderr
                                                System.err.println(i18n.getString("ts.badTest",
                                                                new Object[] { f, e.getMessage() }));
                                        }
                                }
                        }
                }
        }

        void write(XMLWriter out) throws IOException {

                out.startTag("testsuite");
                out.writeAttr("id", "unknownTestSuite:0");

                // name
                out.startTag("name");
                out.write(name == null ? "unknown" : name);
                out.endTag("name");
                // version (optional)
                // tests
                out.startTag("tests");

                // might need to wait for workdir to fully load
                if (!legacyMode)
                        trt.waitUntilReady();
                for (Iterator iter = trt.getIterator(); iter.hasNext();) {
                        TestResult tr = (TestResult) (iter.next());
                        out.newLine();
                        new COFTest(tr, cofData).write(out);
                }
                out.endTag("tests");
                out.endTag("testsuite");
        }
}
