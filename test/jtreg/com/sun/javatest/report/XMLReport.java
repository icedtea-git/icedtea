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

import com.sun.interview.ChoiceArrayQuestion;
import com.sun.interview.ChoiceQuestion;
import com.sun.interview.FileListQuestion;
import com.sun.interview.PropertiesQuestion;
import com.sun.interview.Question;
import com.sun.interview.StringListQuestion;
import com.sun.interview.TreeQuestion;
import com.sun.interview.YesNoQuestion;
import com.sun.javatest.ExcludeList;
import com.sun.javatest.JavaTestError;
import com.sun.javatest.Keywords;
import com.sun.javatest.Parameters;
import com.sun.javatest.Status;
import com.sun.javatest.TemplateUtilities;
import com.sun.javatest.TemplateUtilities.ConfigInfo;
import com.sun.javatest.TestDescription;
import com.sun.javatest.TestEnvironment;
import com.sun.javatest.TestFilter;
import com.sun.javatest.TestResult;
import com.sun.javatest.TestResult.Section;
import com.sun.javatest.TestResultTable;
import com.sun.javatest.util.BackupPolicy;
import java.io.File;
import java.io.IOException;
import com.sun.javatest.util.I18NResourceBundle;
import java.io.BufferedWriter;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Enumeration;
import java.util.Iterator;
import java.util.Locale;
import java.util.Map;
import org.xml.sax.SAXException;


/**
 * XML report (dump).
 */
class XMLReport  {

    public void write(CustomReport.ReportEnviroment sett, File dir) throws IOException {

        BackupPolicy backupPolicy;

        File repFile = new File(dir, REPORT_NAME);
        Writer w = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(repFile), XMLReportMaker.XML_CHARSET));
        try {
            write(w, sett);
        } catch (SAXException ex) {
            throw new JavaTestError(i18n.getString("report.writing.err"), ex);
        }
        w.close();
        sett.xmlReportFile = repFile;

    }

    public void write(Writer w, CustomReport.ReportEnviroment sett) throws IOException, SAXException {

        XMLReportMaker maker = new XMLReportMaker(w);
        maker.sDocument();
        maker.sReport();
        maker.sWorkdirectories();
        File jti = null;
        if (sett.getIp() != null) {
            jti = sett.getIp().getFile();
        }
        maker.sWorkdirectory( jti == null ? null : jti.getPath());

        writeTemplateInfo(maker, sett);
        writeInterview(maker, sett);
        writeStandardValues(maker, sett);
        writeEnvironment(maker, sett);

        maker.eWorkdirectory();
        maker.eWorkdirectories();
        writeResults(maker, sett);
        maker.eReport();
        maker.eDocument();

    }

    private void writeResults(final XMLReportMaker maker, final CustomReport.ReportEnviroment sett) throws SAXException, JavaTestError, IOException {
        maker.sTestResults();

        File[] initFiles = sett.getInitFiles();
        Iterator iter = null;
        TestResultTable resultTable = sett.getIp().getWorkDirectory().getTestResultTable();

        try {
            if (initFiles == null) {
                iter = resultTable.getIterator(new TestFilter[] {sett.getFilter()});
            } else {
                iter = resultTable.getIterator(initFiles, new TestFilter[] {sett.getFilter()});
            }
            for (; iter.hasNext(); ) {
                TestResult tr = (TestResult) (iter.next());
                writeResult(maker, tr);
            }
            maker.eTestResults();
        } catch (TestResultTable.Fault f) {
            throw new JavaTestError(i18n.getString("report.testResult.err"), f);
        }
    }

    private void writeResult(final XMLReportMaker maker, final TestResult testResult) throws SAXException, IOException {

        maker.sTestResult(testResult.getTestName(), testResult.getStatus(), 1);
        try {
            writeDescriptionData(maker, testResult);
            writeEnvironment(maker, testResult);
            writeResultProps(maker, testResult);
            writeSections(maker, testResult);
        } catch (TestResult.Fault  e) {
            System.err.println(i18n.getString("report.writing.err"));
            e.printStackTrace();
        } finally {
            maker.eTestResult();
        }
    }

    private void writeSections(final XMLReportMaker maker, final TestResult testResult) throws SAXException, IOException, TestResult.Fault {

        maker.sSections();
        for (int i = 0; i < testResult.getSectionCount() ; i++) {
            String st = testResult.getSectionTitles()[i];
            Section sec = testResult.getSection(i);
            Status stat = sec.getStatus();
            maker.sSection(st, stat);
            for (int j=0; j < sec.getOutputCount(); j++) {
                String oName = sec.getOutputNames()[j];
                maker.sOutput(oName, sec.getOutput(oName));
                maker.eOutput();
            }
            maker.eSection();
        }
        maker.eSections();
    }

    private void writeResultProps(final XMLReportMaker maker, final TestResult testResult) throws SAXException, TestResult.Fault {

        String time = testResult.getProperty(TestResult.END);

        maker.sResultProps(time);
        Enumeration en = testResult.getPropertyNames();
        while (en.hasMoreElements()) {
            String key = (String) en.nextElement();
            String val = (String) testResult.getProperty(key);
            if (!TestResult.END.equals(key)) {
                maker.makeProperty(key, val);
            }
        }
        maker.eResultProps();
    }

    private void writeEnvironment(final XMLReportMaker maker, final TestResult testResult) throws SAXException {

        Iterator keysIt;
        try {
            Map m = testResult.getEnvironment();
            keysIt =  m.keySet().iterator();
            maker.sTestEnvironment();
            while (keysIt.hasNext()) {
                String key = (String) keysIt.next();
                String val = (String) m.get(key);
                maker.makeProperty(key, val);
            }
            maker.eTestEnvironment();
        } catch (TestResult.Fault e) {

        }
    }

    private void writeDescriptionData(final XMLReportMaker maker, final TestResult testResult) throws SAXException, TestResult.Fault {
        maker.sDescriptionData();

        TestDescription td = testResult.getDescription();
        Iterator keysIt = td.getParameterKeys();

        maker.makeProperty("$root", td.getRootDir());
        maker.makeProperty("$file", td.getFile().getPath());

        while (keysIt.hasNext()) {
            String key = (String) keysIt.next();
            // dump keywords separately
            if (!"keywords".equals(key)) {
                String val = (String) td.getParameter(key);
                maker.makeProperty(key, val);
            }
        }

        String [] kws = td.getKeywords();
        if (kws != null && kws.length > 0 ) {
            maker.sKeyWords();
            maker.makeItems(kws);
            maker.eKeyWords();
        }
        maker.eDescriptionData();
    }

    private void writeEnvironment(final XMLReportMaker maker, final CustomReport.ReportEnviroment sett) throws SAXException {

        String name = null, descr = null;

        if (sett.getIp().getEnv() != null) {
            name = sett.getIp().getEnv().getName();
            descr= sett.getIp().getEnv().getDescription();
        }

        maker.sEnvironment(name, descr);
        TestEnvironment env = sett.getIp().getEnv();
        if (env != null) {
            for (Iterator i = env.elements().iterator(); i.hasNext(); ) {
                TestEnvironment.Element envElem = (TestEnvironment.Element) (i.next());
                maker.makeProperty(envElem.getKey(), envElem.getValue());
            }
        }
        maker.eEnvironment();
    }

    private void writeStandardValues(final XMLReportMaker maker, CustomReport.ReportEnviroment sett) throws SAXException {
        maker.sStdValues();
        writeTestsToRun(maker, sett);
        writePriorStatusList(maker, sett);
        writeExcludeLists(maker, sett);
        maker.conCur(sett.getIp().getConcurrency());
        maker.timeOut(sett.getIp().getTimeoutFactor());
        writeKeyWords(maker, sett);
        maker.eStdValues();
    }

    private void writeKeyWords(final XMLReportMaker maker, final CustomReport.ReportEnviroment sett) throws SAXException {
        // kwywords
        Keywords keywords = sett.getIp().getKeywords();
        if (keywords != null) {
            maker.sKeyWords(sett.getIp().getKeywords().toString());
            maker.eKeyWords();
        }
    }

    private void writeExcludeLists(final XMLReportMaker maker, final CustomReport.ReportEnviroment sett) throws SAXException {
        // Exclude
        ExcludeList excludeList = sett.getIp().getExcludeList();
        if (excludeList != null) {
            Parameters.ExcludeListParameters exclParams = sett.getIp().getExcludeListParameters();
            File[] excludeFiles = null;
            if (exclParams instanceof Parameters.MutableExcludeListParameters)
                excludeFiles =
                        ((Parameters.MutableExcludeListParameters) (exclParams)).getExcludeFiles();

            if (excludeFiles != null && excludeFiles.length > 0) {
                maker.sExclList();
                maker.makeItems(excludeFiles);
                maker.eExclList();
            }
        }
    }

    private void writePriorStatusList(final XMLReportMaker maker, final CustomReport.ReportEnviroment sett) throws SAXException {
        boolean[] b = sett.getIp().getPriorStatusValues();
        if (b != null) {
            int[] ss = {Status.PASSED, Status.FAILED, Status.ERROR, Status.NOT_RUN};
            maker.sPriorStatusList();
            for (int i = 0; i < b.length; i++) {
                if (b[i]) {
                    maker.makeItem(Utils.statusIntToString(ss[i]));
                }
            }
            maker.ePriorStatusList();
        }
    }

    private void writeTestsToRun(final XMLReportMaker maker, final CustomReport.ReportEnviroment sett) throws SAXException {
        maker.sTests();
        String [] tests = sett.getIp().getTests();
        if (tests != null && tests.length > 0) {
            for (int i = 0; i < tests.length; i++) {
                maker.makeItem(tests[i]);
            }
        } else {
            maker.makeEntireTestTree();
        }
        maker.eTests();
    }

    private void writeTemplateInfo(final XMLReportMaker maker, final CustomReport.ReportEnviroment sett) throws SAXException {
        String tPath = sett.getIp().getTemplatePath();
        if (tPath != null) {
            try {
                ConfigInfo ci = TemplateUtilities.getConfigInfo(new File(tPath));
                maker.makeTemplateInfo(tPath, ci.getName(), ci.getDescription());
            } catch (IOException ex) {
                // do nothing
            }
        }
    }


    private void writeInterview(final XMLReportMaker maker, final CustomReport.ReportEnviroment sett) throws SAXException {

        maker.sInterview();
        Question [] questions = sett.getIp().getPath();
        for (int i = 0; i < questions.length; i++) {
            Question q = questions[i];
            maker.sQuestion(q.getStringValue(), q.getText(), q.getSummary());
            if (q instanceof TreeQuestion || q instanceof StringListQuestion || q instanceof FileListQuestion) {
                writeListQuestion(maker, q);
            } else if (q instanceof ChoiceQuestion || q instanceof YesNoQuestion || q instanceof ChoiceArrayQuestion) {
                writeChoiceQuestion(maker, q);
            } else if (q instanceof PropertiesQuestion) {
                writePropertiesQuestion(maker, q);
            }
            maker.eQuestion();
        }
        maker.eInterview();
    }

    private void writeListQuestion(XMLReportMaker maker, Question q) throws SAXException {
        maker.sListQuestion();
        if (q instanceof TreeQuestion) {
            TreeQuestion tq = (TreeQuestion) q;
            maker.makeItems(tq.getValue());
        } else if (q instanceof StringListQuestion ) {
            StringListQuestion slq = (StringListQuestion) q;
            maker.makeItems(slq.getValue());
        } else if (q instanceof FileListQuestion) {
            FileListQuestion flq = (FileListQuestion) q;
            maker.makeItems(flq.getValue());
        }
        maker.eListQuestion();
    }

    private void writePropertiesQuestion(XMLReportMaker maker, Question q) throws SAXException {
        maker.sPropertiesQuestion();
        PropertiesQuestion pq = (PropertiesQuestion) q;
        String [] grs = pq.getGroups();
        String h1 = pq.getKeyHeaderName();
        String h2 = pq.getValueHeaderName();
        if (grs != null) {
            for (int i = 0; i < grs.length; i++) {
                String name = grs[i];
                maker.sGroup(name, h1, h2);
                if (name != null) {
                    String[][] table = pq.getGroup(name);
                    writeTable(maker, table);
                }
                maker.eGroup(name, h1, h2);
            }
        }
        maker.ePropertiesQuestion();
    }

    private void writeChoiceQuestion(XMLReportMaker maker, Question q) throws SAXException {
        maker.sChoiceQuestion();
        if (q instanceof ChoiceQuestion ) {
            ChoiceQuestion cq = (ChoiceQuestion) q;
            maker.makeChoices(cq.getChoices(), cq.getDisplayChoices());
        } else if (q instanceof YesNoQuestion ) {
            YesNoQuestion ynq = (YesNoQuestion) q;
            maker.makeChoices(ynq.getChoices(), ynq.getDisplayChoices());
        } else if (q instanceof ChoiceArrayQuestion) {
            ChoiceArrayQuestion caq = (ChoiceArrayQuestion) q;
            maker.makeChoices(caq.getChoices(), caq.getDisplayChoices(), caq.getValue());
        }
        maker.eChoiceQuestion();
    }

    private void writeTable(XMLReportMaker maker, String[][] table) throws SAXException {
        if (table != null ) {
            for (int i=0 ; i < table.length ;i++) {
                maker.makeRow(table[i][0], table[i][1]);
            }
        }
    }

    /**
     * Utility class
     */
    static class Utils {

        /**
         * Convert date to string in ISO-8601 or xs:dateTime format
         * @param date Date
         * @return ISO-8601 String
         */
        static String dateToISO8601(Date date) {
            DateFormat dfISO8601 = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ssZ");
            // format in (almost) ISO8601 format
            String dateStr = dfISO8601.format(date);
            // remap the timezone from 0000 to 00:00 (starts at char 22)
            return dateStr.substring(0, 22) + ":" + dateStr.substring(22);
        }

        /**
         * Convert string from JTR format to java.util.Date
         * @throws java.text.ParseException
         * @param dateStr String
         * @return Date
         */
        static Date jtrToDate(String dateStr) throws ParseException {
            DateFormat df = new SimpleDateFormat("EEE MMM dd HH:mm:ss zzz yyyy",
                    Locale.US);
            return df.parse(dateStr);
        }

        /**
         * Convert Status object to String
         */
        static String statusToString(Status st) {
            if (st.isError()) return "ERROR";
            if (st.isFailed()) return "FAILED";
            if (st.isNotRun()) return "NOT_RUN";
            if (st.isPassed()) return "PASSED";
            return "UNKNOWN";
        }

        /**
         * Convert Status integer value to String
         */
        static String statusIntToString(int st) {
            if (st == Status.ERROR) return "ERROR";
            if (st == Status.FAILED) return "FAILED";
            if (st == Status.NOT_RUN) return "NOT_RUN";
            if (st == Status.PASSED) return "PASSED";
            return "UNKNOWN";
        }

    }

    private static I18NResourceBundle i18n = I18NResourceBundle.getBundleForClass(XMLReport.class);

    // The name of the root file for a set of report files.
    static final String REPORT_NAME = "report.xml";

    public static String[] getFilenamesUsed() {
        return new String[] {REPORT_NAME};
    }

}
