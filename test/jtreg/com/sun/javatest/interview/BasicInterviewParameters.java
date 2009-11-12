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

import java.text.MessageFormat;
import java.util.MissingResourceException;
import com.sun.interview.FinalQuestion;
import com.sun.interview.NullQuestion;
import com.sun.interview.StringQuestion;
import com.sun.interview.Question;
import com.sun.javatest.InterviewParameters;
import com.sun.javatest.Parameters;
import com.sun.javatest.TestSuite;
import com.sun.javatest.WorkDirectory;
import com.sun.javatest.util.I18NResourceBundle;

/**
 * A basic implementation of InterviewParameters that uses standard
 * interviews for all the various interview sections, except the environment
 * section, which remains to be implemented by subtypes.
 */
public abstract class BasicInterviewParameters extends InterviewParameters
{
    /**
     * Create a BasicInterviewParameters object.
     * The test suite for which this interview applies should be set
     * with setTestSuite.
     * @param tag the tag used to qualify questions in this interview
     * @throws Interview.Fault if there is a problem creating this object
     */
    protected BasicInterviewParameters(String tag)
        throws Fault
    {
        super(tag);
        // leave this to the subtype to decide whether to provide or not
        //setHelpSet("/com/sun/javatest/moreInfo/moreInfo.hs");

        iTests = new TestsInterview(this);
        iExcludeList = new ExcludeListInterview(this);
        iKeywords = new KeywordsInterview(this);
        iPriorStatus = new PriorStatusInterview(this);
        iConcurrency = new ConcurrencyInterview(this);
        iTimeoutFactor = new TimeoutFactorInterview(this);

        setFirstQuestion(qProlog);
    }

    /**
     * Create a BasicInterviewParameters object.
     * @param tag the tag used to qualify questions in this interview
     * @param ts The test suite to which this interview applies.
     * @throws Interview.Fault if there is a problem creating this object
     */
    protected BasicInterviewParameters(String tag, TestSuite ts)
        throws Fault
    {
        this(tag);
        setTestSuite(ts);
    }

    /**
     * Specify whether or not to include standard questions in the
     * prolog to get a name and description for this configuration.
     * If these standard questions are not used, it is the responsibility
     * of the EnvParameters interview to get the name and description.
     * If the standard prolog is bypassed by using setFirstQuestion,
     * this method has no effect.
     * @param on if true, questions will be included in the standard
     * prolog to get a name and description for this configuration.
     * @see #isNameAndDescriptionInPrologEnabled
     */
    public void setNameAndDescriptionInPrologEnabled(boolean on) {
        if (on) {
            // defer the initialization of these to avoid possible
            // conflicts in subtypes
            initNameQuestion();
            initDescriptionQuestion();
        }

        nameAndDescriptionInPrologEnabled = on;
    }

    /**
     * Check whether or not to include standard questions in the
     * prolog to get a name and description for this configuration.
     * If these standard questions are not used, it is the responsibility
     * of the EnvParameters interview to get the name and description.
     * @return true if the standard questions should be included,
     * and false otherwise.
     * @see #setNameAndDescriptionInPrologEnabled
     */
    public boolean isNameAndDescriptionInPrologEnabled() {
        return nameAndDescriptionInPrologEnabled;
    }

    /**
     * Get the name for this configuration.
     * If the standard question for the name has been included in the prolog,
     * it will be used to get the result; otherwise the default
     * implementation to get the name from the environment will be used.
     * @return the name for this configuration, or null if not known
     * @see #setNameAndDescriptionInPrologEnabled
     */
    public String getName() {
        if (nameAndDescriptionInPrologEnabled)
            return qName.getValue();
        else
            return super.getName();
    }

    /**
     * Get a description for this configuration.
     * If the standard question for the description has been included in the prolog,
     * it will be used to get the result; otherwise the default
     * implementation to get the description from the environment will be used.
     * @return a description for this configuration, or null if not known
     */
    public String getDescription() {
        if (nameAndDescriptionInPrologEnabled)
            return qDescription.getValue();
        else
            return super.getDescription();
    }


    public TestSuite getTestSuite() {
        return testSuite;
    }

    /**
     * Set the test suite for the test run. The test suite may only be set once.
     * @param ts the test suite to be set.
     * @see #getTestSuite
     * @throws NullPointerException if ts is null
     * @throws IllegalStateException if the test suite has already been set to
     * something different
     */
    public void setTestSuite(TestSuite ts) {
        if (ts == null)
            throw new NullPointerException();

        if (testSuite != null && testSuite != ts)
            throw new IllegalStateException();

        testSuite = ts;
    }

    public WorkDirectory getWorkDirectory() {
        return workDir;
    }

    /**
     * Set the work directory for the test run.
     * The work directory may only be set once.
     * If the test suite has already been set, it must exactly match the test suite
     * for the work directory; if the test suite has not yet been set, it will
     * be set to the test suite for this work directory.
     * @param wd the work directory to be set.
     * @see #getWorkDirectory
     * @throws NullPointerException if wd is null
     * @throws IllegalStateException if the work directory has already been set to
     * something different
     */
    public void setWorkDirectory(WorkDirectory wd) {
        if (wd == null)
            throw new NullPointerException();

        if (workDir != null && workDir != wd)
            throw new IllegalStateException();

        workDir = wd;
    }

    //--------------------------------------------------------------------------

    public Parameters.TestsParameters getTestsParameters() {
        return iTests;
    }

    protected Question getTestsFirstQuestion() {
        return callInterview(iTests, getTestsSuccessorQuestion());
    }

    //--------------------------------------------------------------------------

    public Parameters.ExcludeListParameters getExcludeListParameters() {
        return iExcludeList;
    }

    protected Question getExcludeListFirstQuestion() {
        return callInterview(iExcludeList, getExcludeListSuccessorQuestion());
    }

    //--------------------------------------------------------------------------

    public Parameters.KeywordsParameters getKeywordsParameters() {
        TestSuite ts = getTestSuite();
        String[] kw = (ts == null ? null : ts.getKeywords());
        return (kw == null || kw.length > 0 ? iKeywords : null);
    }

    protected Question getKeywordsFirstQuestion() {
        TestSuite ts = getTestSuite();
        String[] kw = (ts == null ? null : ts.getKeywords());
        if (kw == null || kw.length > 0)
            return callInterview(iKeywords, getKeywordsSuccessorQuestion());
        else
            return getKeywordsSuccessorQuestion();
    }

    //--------------------------------------------------------------------------

    public Parameters.PriorStatusParameters getPriorStatusParameters() {
        return iPriorStatus;
    }

    protected Question getPriorStatusFirstQuestion() {
        return callInterview(iPriorStatus, getPriorStatusSuccessorQuestion());
    }

    //--------------------------------------------------------------------------

    public Parameters.ConcurrencyParameters getConcurrencyParameters() {
        return iConcurrency;
    }

    protected Question getConcurrencyFirstQuestion() {
        return callInterview(iConcurrency, getConcurrencySuccessorQuestion());
    }

    //--------------------------------------------------------------------------

    public Parameters.TimeoutFactorParameters getTimeoutFactorParameters() {
        return iTimeoutFactor;
    }

    protected Question getTimeoutFactorFirstQuestion() {
        return callInterview(iTimeoutFactor, getTimeoutFactorSuccessorQuestion());
    }

    //--------------------------------------------------------------------------

    protected Question getEpilogFirstQuestion() {
        return qEpilog;
    }

    //--------------------------------------------------------------------------

    private String getResourceStringX(String key) {
        // try and get it from the interview bundles first
        String s = getResourceString(key, true);
        if (s != null)
            return s;

        // otherwise, default to using the bundle for this class
        try {
            return i18n.getString(key);
        }
        catch (MissingResourceException e) {
            return key;
        }
    }


    private TestSuite testSuite;
    private WorkDirectory workDir;
    private TestsInterview iTests;
    private ExcludeListInterview iExcludeList;
    private KeywordsInterview iKeywords;
    private PriorStatusInterview iPriorStatus;
    private ConcurrencyInterview iConcurrency;
    private TimeoutFactorInterview iTimeoutFactor;
    private boolean nameAndDescriptionInPrologEnabled;


    private NullQuestion qProlog = new NullQuestion(this, "prolog") {
            public Question getNext() {
                if (nameAndDescriptionInPrologEnabled)
                    return qName;
                else
                    return getPrologSuccessorQuestion();
            }

            public String getSummary() {
                if (summary == null)
                    summary = getResourceStringX("BasicInterviewParameters.prolog.smry");
                return summary;
            }

            public String getText() {
                if (text == null)
                    text = getResourceStringX("BasicInterviewParameters.prolog.text");
                return MessageFormat.format(text, getTextArgs());
            }

            public Object[] getTextArgs() {
                String name = (testSuite == null ? null : testSuite.getName());
                return new Object[] { new Integer(name == null ? 0 : 1), name };
            }

            private String summary;
            private String text;
        };

    private StringQuestion qName;
    private void initNameQuestion() {
        qName = new StringQuestion(this, "name") {
                public boolean isValueValid() {
                    return isValidIdentifier(value);
                }

                public Question getNext() {
                    return qDescription;
                }

                public String getSummary() {
                    if (summary == null)
                        summary = getResourceStringX("BasicInterviewParameters.name.smry");
                    return summary;
                }

                public String getText() {
                    if (text == null)
                        text = getResourceStringX("BasicInterviewParameters.name.text");
                    return MessageFormat.format(text, getTextArgs());
                }

                public Object[] getTextArgs() {
                    String name = (testSuite == null ? null : testSuite.getName());
                    return new Object[] { new Integer(name == null ? 0 : 1), name };
                }

                private String summary;
                private String text;
            };
    }

    private static boolean isValidIdentifier(String name) {
        if (name == null || name.length() == 0)
            return false;

        // first character must be a letter
        if (!Character.isLetter(name.charAt(0)))
            return false;

        // subsequent characters must be a letter or digit or _
        for (int i = 1; i < name.length(); i++) {
            char c = name.charAt(i);
            if ( !Character.isLetterOrDigit(c) && c != '_')
                return false;
        }

        // all tests passed
        return true;
    }

    private StringQuestion qDescription;
    private void initDescriptionQuestion() {
        qDescription = new StringQuestion(this, "description") {
                public boolean isValueValid() {
                    return (value != null && value.length() > 0);
                }

                public Question getNext() {
                    return getPrologSuccessorQuestion();
                }

                public String getSummary() {
                    if (summary == null)
                        summary = getResourceStringX("BasicInterviewParameters.description.smry");
                    return summary;
                }

                public String getText() {
                    if (text == null)
                        text = getResourceStringX("BasicInterviewParameters.description.text");
                    return MessageFormat.format(text, getTextArgs());
                }

                public Object[] getTextArgs() {
                    String name = (testSuite == null ? null : testSuite.getName());
                    return new Object[] { new Integer(name == null ? 0 : 1), name };
                }

                private String summary;
                private String text;
            };
    }

    private FinalQuestion qEpilog = new FinalQuestion(this, "epilog") {

            public String getSummary() {
                if (summary == null)
                    summary = getResourceStringX("BasicInterviewParameters.epilog.smry");
                return summary;
            }

            public String getText() {
                if (text == null)
                    text = getResourceStringX("BasicInterviewParameters.epilog.text");
                return MessageFormat.format(text, getTextArgs());
            }

            private String summary;
            private String text;
        };

    private static I18NResourceBundle i18n = I18NResourceBundle.getBundleForClass(BasicInterviewParameters.class);
}
