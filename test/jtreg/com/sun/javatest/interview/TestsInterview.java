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
package com.sun.javatest.interview;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.Arrays;
import java.util.TreeSet;
import java.util.Vector;

import com.sun.interview.ChoiceQuestion;
import com.sun.interview.ErrorQuestion;
import com.sun.interview.FileQuestion;
import com.sun.interview.FinalQuestion;
import com.sun.interview.Interview;
import com.sun.interview.Question;
import com.sun.interview.TreeQuestion;
import com.sun.interview.YesNoQuestion;
import com.sun.javatest.InterviewParameters;
import com.sun.javatest.Parameters;
import com.sun.javatest.TestResult;
import com.sun.javatest.TestResultTable;
import com.sun.javatest.WorkDirectory;

/**
 * This interview collects the "initial files" parameter. It is normally used as
 * one of a series of sub-interviews that collect the parameter information for
 * a test run.
 */
public class TestsInterview
    extends Interview
    implements Parameters.MutableTestsParameters
{
    /**
     * Create an interview.
     * @param parent The parent interview of which this is a child.
     * @throws Interview.Fault if there is a problem while creating the interview.
     */
    public TestsInterview(InterviewParameters parent)
        throws Interview.Fault
    {
        super(parent, "tests");
        this.parent = parent;
        setResourceBundle("i18n");
        setFirstQuestion(qNeedTests);
    }

    /**
     * Get the initial files from the interview.
     * @return a list of initial files to be read, to determine the tests to be selected
     * @see #setTests
     */
    public String[] getTests() {
        if (qNeedTests.getValue() == YesNoQuestion.YES) {
            if (qTreeOrFile.getValue() == TREE)
                return qTestTree.getValue();
            else
                return getTests(qTestFile.getValue());
        }
        else
            return null;
    }

    public void setTests(String[] tests) {
        if (tests == null)
            setTestsMode(ALL_TESTS);
        else {
            setTestsMode(SPECIFIED_TESTS);
            setSpecifiedTests(tests);
        }
    }

    public int getTestsMode() {
        return (qNeedTests.getValue() == YesNoQuestion.YES ? SPECIFIED_TESTS : ALL_TESTS);
    }

    public void setTestsMode(int mode) {
        switch (mode) {
        case ALL_TESTS:
            qNeedTests.setValue(YesNoQuestion.NO);
            break;

        case SPECIFIED_TESTS:
            qNeedTests.setValue(YesNoQuestion.YES);
            break;

        default:
            throw new IllegalArgumentException();
        }
    }

    public String[] getSpecifiedTests() {
        // return paths sorted and uniqified
        //String[] v = qTests.getValue();

        String[] v;
        if (qTreeOrFile.getValue() == TREE)
            v = qTestTree.getValue();
        else
            v = getTests(qTestFile.getValue());

        if (v == null)
            return null;

        TreeSet ts = new TreeSet(Arrays.asList(v));
        return (String[]) (ts.toArray(new String[ts.size()]));

    }

    public void setSpecifiedTests(String[] tests) {
        qTreeOrFile.setValue(TREE);
        qTestTree.setValue(tests);
    }

    //----------------------------------------------------------------------------
    //
    // Need tests

    private YesNoQuestion qNeedTests = new YesNoQuestion(this, "needTests", YesNoQuestion.NO) {
        protected Question getNext() {
            if (value == null)
                return null;
            else if (value == YES)
                return qTreeOrFile;
            else
                return qEnd;
        }
    };

    //----------------------------------------------------------------------------
    //
    // Tree or file

    private static final String FILE = "file";
    private static final String TREE = "tree";

    private ChoiceQuestion qTreeOrFile = new ChoiceQuestion(this, "treeOrFile") {
        {
            setChoices(new String[] { TREE, FILE }, true);
        }

        protected Question getNext() {
            if (value == TREE)
                return qTestTree;
            else
                return qTestFile;
        }
    };

    //----------------------------------------------------------------------------
    //
    // file

    private FileQuestion qTestFile = new FileQuestion(this, "testFile") {
        public boolean isValueValid() {
            return (value != null && value.getPath().length() > 0);
        }

        protected Question getNext() {
            if (value == null || value.getPath().length() == 0)
                return null;

            String[] tests = getTests(value);
            if (tests == null)
                return cachedTestsError;

            validateTests(tests);

            if (cachedTestsError != null)
                return cachedTestsError;
            else
                return qEnd;
        }
    };

    private String[] getTests(File file) {
        Vector paths = new Vector();
        try {
            BufferedReader in = new BufferedReader(new FileReader(file));
            String line;
            while ((line = in.readLine()) != null) {
                line = line.trim();
                if (line.length() == 0 || line.startsWith("#"))
                    continue;
                int sp = line.indexOf(' ');
                paths.add(sp == -1 ? line : line.substring(0, sp));
            }
            in.close();
        }
        catch (FileNotFoundException e) {
            cachedTestsError = qCantFindFileError;
            cachedTestsErrorArgs = new Object[] { file };
            return null;
        }
        catch (IOException e) {
            cachedTestsError = qCantFindFileError;
            cachedTestsErrorArgs = new Object[] { file, e.toString() };
            return null ;
        }

        return (String[]) (paths.toArray(new String[paths.size()]));
    }

    private Question qCantFindFileError  = new ErrorQuestion(this, "cantFindFile") {
        protected Object[] getTextArgs() {
            return cachedTestsErrorArgs;
        }
    };

    private Question qCantReadFileError  = new ErrorQuestion(this, "cantReadFile") {
        protected Object[] getTextArgs() {
            return cachedTestsErrorArgs;
        }
    };

    //----------------------------------------------------------------------------
    //
    // Tests

    private TreeQuestion.Model model = new TreeQuestion.Model() {
        public Object getRoot() {
            return parent.getWorkDirectory().getTestResultTable().getRoot();
        }

        public int getChildCount(Object node) {
            if (node == null)
                throw new NullPointerException();
            else if (node instanceof TestResultTable.TreeNode)
                return ((TestResultTable.TreeNode) node).getChildCount();
            else if (node instanceof TestResult)
                return 0;
            else
                throw new IllegalArgumentException();
        }

        public Object getChild(Object node, int index) {
            if (node == null)
                throw new NullPointerException();
            else if (node instanceof TestResultTable.TreeNode)
                return ((TestResultTable.TreeNode) node).getChild(index);
            else if (node instanceof TestResult)
                return null;
            else
                throw new IllegalArgumentException();
        }

        public String getName(Object node) {
            if (node == null)
                throw new NullPointerException();
            else if (node instanceof TestResultTable.TreeNode)
                return ((TestResultTable.TreeNode) node).getName();
            else if (node instanceof TestResult) {
                TestResult tr = (TestResult) node;
                String fullName = tr.getTestName();
                int lastSlash = fullName.lastIndexOf("/");
                return (lastSlash == -1
                        ? fullName
                        : fullName.substring(lastSlash+1));

            }
            else
                throw new IllegalArgumentException();
        }

        public String getPath(Object node) {
            if (node == null)
                throw new NullPointerException();
            else if (node instanceof TestResult)
                return ((TestResult) node).getTestName();
            else if (node instanceof TestResultTable.TreeNode) {
                TestResultTable.TreeNode tn = (TestResultTable.TreeNode) node;
                if (tn.isRoot())
                    return tn.getName();
                else
                    return getPath(tn.getParent() + "/" + tn.getName());
            }
            else
                throw new IllegalArgumentException();
        }

        public boolean isLeaf(Object node) {
            if (node == null)
                throw new NullPointerException();
            else if (node instanceof TestResult)
                return true;
            else if (node instanceof TestResultTable.TreeNode)
                return false;
            else
                throw new IllegalArgumentException();
        }
    };

    private TreeQuestion qTestTree = new TreeQuestion(this, "tests", model) {
        /* Ugly, this would seem to be helpful, but it precludes getting to the
           ErrorQuestion
        public boolean isValueValid() {
            validateTests();
            return (cachedTestsError == null);
        }
        */

        protected Question getNext() {
            validateTests(value);

            // value of null currently means everything;
            // this is a corollary of having an anonymous
            // test suite root; to fix, we would have to use
            // a pseudo-name in the saved value for "ALL"
            /*if (value == null || value.length == 0)
                return null;
            else */if (cachedTestsError != null)
                return cachedTestsError;
            else
                return qEnd;
        }
    };

    //----------------------------------------------------------------------------

    private void validateTests(String[] tests) {

        if (equal(tests, cachedTestsValue))
            return;

        cachedTestsValue = tests;
        cachedTestsError = null; // default

        WorkDirectory wd = parent.getWorkDirectory();
        if (wd == null)
            return;

        TestResultTable trt = wd.getTestResultTable();
        if (tests == null || tests.length == 0) {
            // currently, empty selection means everything
            // as a corollary that the path of the root node
            // is saved as an empty string.
            //      cachedTestsError = qNoTestsError;
            return;
        }
        else {
            Vector v = new Vector();
            StringBuffer sb = new StringBuffer();
            for (int i = 0; i < tests.length; i++) {
                String test = tests[i];
                // ignore duplicates, and look for otherwise bad tests
                if (!v.contains(test) && !trt.validatePath(test)) {
                    // if too many, abbreviate the list, to stop it being
                    // unboundedly huge
                    if (sb.length() > 32) {
                        sb.append("...");
                        break;
                    }

                    if (sb.length() > 0)
                        sb.append(' ');
                    sb.append(test);
                    v.add(test);
                }
            }

            if (sb.length() > 0) {
                // got bad tests
                cachedTestsError = qBadTestsError;
                cachedTestsErrorArgs = new Object[] { new Integer(v.size()), sb.toString() };
            }
        }
    }

    private Question qNoTestsError = new ErrorQuestion(this, "noTests");

    private ErrorQuestion qBadTestsError = new ErrorQuestion(this, "badTests") {
        protected Object[] getTextArgs() {
            return cachedTestsErrorArgs;
        }
    };
    private String[] cachedTestsValue;
    private Question cachedTestsError;
    private Object[] cachedTestsErrorArgs;

    //----------------------------------------------------------------------------
    //
    // End

    private Question qEnd = new FinalQuestion(this);

    //----------------------------------------------------------------------------

    private static boolean equal(String[] s1, String[] s2) {
        if (s1 == null || s2 == null)
            return (s1 == s2);

        if (s1.length != s2.length)
            return false;

        for (int i = 0; i < s1.length; i++) {
            if (s1[i] != s2[i])
                return false;
        }

        return true;
    }

    //--------------------------------------------------------

    private InterviewParameters parent;
}
