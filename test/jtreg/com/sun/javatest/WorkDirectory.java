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
package com.sun.javatest;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileNotFoundException;
import java.io.InputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;

import com.sun.javatest.logging.LoggerFactory;
import com.sun.javatest.util.I18NResourceBundle;
import com.sun.javatest.util.LogFile;


/**
 * A class providing access to the working state of a test run, as embodied
 * in a work directory.
 */
public class WorkDirectory
{
    /**
     * This exception is used to report problems that arise when using
     * work directories.
     */
    public static class Fault extends Exception
    {
        Fault(I18NResourceBundle i18n, String s) {
            super(i18n.getString(s));
        }

        Fault(I18NResourceBundle i18n, String s, Object o) {
            super(i18n.getString(s, o));
        }

        Fault(I18NResourceBundle i18n, String s, Object[] o) {
            super(i18n.getString(s, o));
        }
    }


    /**
     * Signals that there is a serious, unrecoverable problem when trying to
     * open or create a work directory.
     */
    public static class BadDirectoryFault extends Fault
    {
        BadDirectoryFault(I18NResourceBundle i18n, String key, File f) {
            super(i18n, key, f.getPath());
        }

        BadDirectoryFault(I18NResourceBundle i18n, String key, File f, Throwable t) {
            super(i18n, key, new Object[] {f.getPath(), t.toString()});
        }
    }

    /**
     * Signals that a directory (while valid in itself) is not a valid work directory.
     */
    public static class NotWorkDirectoryFault extends Fault
    {
        NotWorkDirectoryFault(I18NResourceBundle i18n, String key, File f) {
            super(i18n, key, f.getPath());
        }
    }

    /**
     * Signals that a work directory already exists when an attempt is made
     * to create one.
     */
    public static class WorkDirectoryExistsFault extends Fault
    {
        WorkDirectoryExistsFault(I18NResourceBundle i18n, String key, File f) {
            super(i18n, key, f.getPath());
        }
    }

    /**
     * Signals that a work directory does not match the given test suite.
     */
    public static class MismatchFault extends Fault
    {
        MismatchFault(I18NResourceBundle i18n, String key, File f) {
            super(i18n, key, f.getPath());
        }
    }

    /**
     * Signals that there is a problem trying to determine the test suite
     * appropriate for the work directory.
     */
    public static class TestSuiteFault extends Fault
    {
        TestSuiteFault(I18NResourceBundle i18n, String key, File f, Object o) {
            super(i18n, key, new Object[] {f.getPath(), o});
        }
    }

    /**
     * Signals that there is a problem trying to initialize from the data in
     * the work directory.
     */
    public static class InitializationFault extends Fault
    {
        InitializationFault(I18NResourceBundle i18n, String key, File f, Object o) {
            super(i18n, key, new Object[] {f.getPath(), o});
        }
    }

    /**
     * Signals that a problem occurred while trying to purge files in work directory.
     */
    public static class PurgeFault extends Fault
    {
        PurgeFault(I18NResourceBundle i18n, String key, File f, Object o) {
            super(i18n, key, new Object[] {f.getPath(), o});
        }
    }

    /**
     * Check if a directory is a work directory. This is intended to be a quick
     * check, rather than exhaustive one; as such, it simply checks for the
     * existence of the "jtData" subdirectory.
     * @param dir the directory to be checked
     * @return true if and only if the specified directory appears to be
     * a work directory
     */
    public static boolean isWorkDirectory(File dir) {
        //System.err.println("WorkDirectory.isWorkDirectory: " + dir);
        File jtData = new File(dir, JTDATA);

        if (jtData.exists() && jtData.isDirectory())
            // should consider checking for existence of test suite data
            return true;
        else
            return false;
    }

    /**
     * Check if a directory is an empty directory.
     * @param dir the directory to be checked
     * @return true if and only if the directory is empty
     */
    public static boolean isEmptyDirectory(File dir) {
        if (dir.exists() && dir.canRead() && dir.isDirectory()) {
            String[] list = dir.list();
            return (list == null || list.length == 0);
        }
        else
            return false;
    }

    /**
     * Do sanity check of workdir.  All critical areas must be
     * read-write.
     */
    public static boolean isUsableWorkDirectory(File dir) {
        if (dir == null || !isUsable(dir))
            return false;

        try {
            File canonDir = canonicalize(dir);
            File jtData = new File(canonDir, JTDATA);

            // could call isWorkDirectory(File)
            if (!isUsable(jtData))
                return false;

            // all files in jtData must be read-write
            File[] content = jtData.listFiles();

            // could even look for key files while doing this loop
            if (content != null && content.length > 0)
                for (int i = 0; i < content.length; i++)
                    if (!isUsable(content[i]))
                        return false;
        }
        catch (BadDirectoryFault f) {
            return false;
        }

        return true;
    }

    private static boolean isUsable(File f) {
        if (!f.exists())
            return false;

        if (!f.canRead())
            return false;

        if (!f.canWrite())
            return false;

        return true;
    }

    /**
     * Create a new work directory with a given name, and for a given test suite.
     * @param dir the directory to be created as a work directory.
     * This directory may (but need not) exist; if it does exist, it must be empty.
     * @param ts the test suite for which this will be a work directory
     * @return the WorkDirectory that was created
     * @throws WorkDirectory.WorkDirectoryExistsFault if the work directory
     *          could not be created because it already exists.
     *          If this exception is thrown, you may want to call {@link #open}
     *          instead.
     * @throws WorkDirectory.BadDirectoryFault is there was a problem creating
     *          the work directory.
     * @throws WorkDirectory.InitializationFault if there are unrecoverable problems encountered
     *         while reading the data present in the work directory
     * @see #convert
     * @see #open
     */
    public static WorkDirectory create(File dir, TestSuite ts)
        throws BadDirectoryFault, WorkDirectoryExistsFault, InitializationFault
    {
        //System.err.println("WD.create: " + dir);
        return createOrConvert(dir, ts, true);
    }

    /**
     * Convert an existing directory into a work directory.
     * @param dir the directory to be converted to a work directory
     * @param ts  the test suite for which this will be a work directory
     * @return the WorkDirectory that was created
     * @throws FileNotFoundException if the directory to be converted does
     *          not exist
     * @throws WorkDirectory.WorkDirectoryExistsFault if the work directory
     *          could not be created because it already exists.
     *          If this exception is thrown, you may want to call {@link #open}
     *          instead.
     * @throws WorkDirectory.BadDirectoryFault is there was a problem creating
     *          the work directory.
     * @throws WorkDirectory.InitializationFault if there are unrecoverable problems encountered
     *         while reading the data present in the work directory
     * @see #create
     * @see #open
     */
    public static WorkDirectory convert(File dir, TestSuite ts)
        throws BadDirectoryFault, WorkDirectoryExistsFault,
               FileNotFoundException, InitializationFault
    {
        if (!dir.exists())
            throw new FileNotFoundException(dir.getPath());
        return createOrConvert(dir, ts, false);
    }

    private static WorkDirectory createOrConvert(File dir, TestSuite ts, boolean checkEmpty)
        throws BadDirectoryFault, WorkDirectoryExistsFault, InitializationFault
    {
        File canonDir;
        File jtData;
        ArrayList undoList = new ArrayList();

        try {
            if (dir.exists()) {
                canonDir = canonicalize(dir);
                jtData = new File(canonDir, JTDATA);


                if (!canonDir.isDirectory())
                    throw new BadDirectoryFault(i18n, "wd.notDirectory", canonDir);

                if (!canonDir.canRead())
                    throw new BadDirectoryFault(i18n, "wd.notReadable", canonDir);

                if (jtData.exists() && jtData.isDirectory())
                    throw new WorkDirectoryExistsFault(i18n, "wd.alreadyExists", canonDir);

                if (checkEmpty) {
                    String[] list = canonDir.list();
                    if (list != null && list.length > 0)
                        throw new BadDirectoryFault(i18n, "wd.notEmpty", canonDir);
                }

                // actively flush the dirMap for canonDir?
            }
            else {
                if (!mkdirs(dir, undoList))
                    throw new BadDirectoryFault(i18n, "wd.cantCreate", dir);
                canonDir = canonicalize(dir);
                jtData = new File(canonDir, JTDATA);
            }

            if (!mkdirs(jtData, undoList))
                throw new BadDirectoryFault(i18n, "wd.cantCreate", canonDir);

            try {
                WorkDirectory wd;

                synchronized (dirMap) {
                    wd = new WorkDirectory(canonDir, ts, null);
                    dirMap.put(canonDir, new WeakReference(wd));
                }

                wd.saveTestSuiteInfo();

                // create successful -- so zap the undoList
                undoList = null;

                return wd;
            }
            catch (IOException e) {
                throw new BadDirectoryFault(i18n, "wd.cantWriteTestSuiteInfo", canonDir, e);
            }
        }
        finally {
            if (undoList != null)
                undo(undoList);
        }
    }

    public String getLogFileName() {
        return logFileName;
    }

    private static boolean mkdirs(File dir, ArrayList undoList) {
        File parent = dir.getParentFile();
        if (parent != null && !parent.exists()) {
            if (!mkdirs(parent, undoList))
                return false;
        }

        if (dir.mkdir()) {
            //System.err.println("WD.mkdir " + dir);
            undoList.add(dir);
            return true;
        }

        return false;
    }

    private static void undo(ArrayList undoList) {
        for (int i = undoList.size() - 1; i >= 0; i--) {
            File f = (File) (undoList.get(i));
            delete(f);
        }
    }

    private static void delete(File f) {
        //System.err.println("WD.delete " + f);
        if (f.isDirectory()) {
            File[] ff = f.listFiles();
            for (int i = 0; i < ff.length; i++) {
                if (ff[i].isDirectory() || ff[i].isFile())
                    delete(ff[i]);
            }
        }
        f.delete();
    }

    /**
     * Open an existing work directory, using the default test suite associated with it.
     * @param dir the directory to be opened as a WorkDirectory
     * @return the WorkDirectory that is opened
     * @throws FileNotFoundException if the directory identified by <code>dir</code> does
     *          not exist. If this exception is thrown, you may want to call {@link #create}
     *          instead.
     * @throws WorkDirectory.BadDirectoryFault if there was a problem opening the
     *          work directory.
     * @throws WorkDirectory.NotWorkDirectoryFault if the directory identified
     *          by <code>dir</code> is a valid directory, but has not yet been
     *          initialized as a work directory. If this exception is thrown,
     *          you may want to call {@link #create} instead.
     * @throws WorkDirectory.MismatchFault if the test suite recorded in
     *          the work directory does not match the test suite's ID recorded
     *          in the work directory.
     * @throws WorkDirectory.TestSuiteFault if there was a problem determining
     *          the test suite for which this is a work directory.
     *          If this exception is thrown, you can override the test suite
     *          using the other version of {@link #open(File,TestSuite)}.
     * @throws WorkDirectory.InitializationFault if there are unrecoverable
     *         problems encountered while reading the data present in the
     *         work directory
     */
    public static WorkDirectory open(File dir)
        throws FileNotFoundException,
               BadDirectoryFault,
               NotWorkDirectoryFault,
               MismatchFault,
               TestSuiteFault,
               InitializationFault
    {
        //System.err.println("WD.open: " + dir);
        if (!dir.exists())
            throw new FileNotFoundException(dir.getPath());

        File canonDir = canonicalize(dir);

        if (!canonDir.isDirectory())
            throw new BadDirectoryFault(i18n, "wd.notDirectory", canonDir);

        if (!canonDir.canRead())
            throw new BadDirectoryFault(i18n, "wd.notReadable", canonDir);

        File jtData = new File(canonDir, JTDATA);
        if (!jtData.exists())
            throw new NotWorkDirectoryFault(i18n, "wd.notWorkDir", canonDir);

        WorkDirectory wd;

        synchronized (dirMap) {
            // sync-ed to make dirMap data consistent
            WeakReference ref = (WeakReference)(dirMap.get(canonDir));
            wd = (ref == null ? null : (WorkDirectory) (ref.get()));

            if (wd != null)
                return wd;

            Properties tsInfo;
            TestSuite ts;

            try {
                tsInfo = loadTestSuiteInfo(jtData);
                String root = tsInfo.getProperty(TESTSUITE_ROOT);
                if (root == null)
                    throw new BadDirectoryFault(i18n, "wd.noTestSuiteRoot", canonDir);

                File tsr = new File(root);
                if (!tsr.exists())
                    throw new TestSuiteFault(i18n, "wd.cantFindTestSuite", canonDir, tsr.getPath());

                ts = TestSuite.open(tsr);

                String wdID = (tsInfo == null ? null : (String) (tsInfo.get(TESTSUITE_ID)));
                String tsID = ts.getID();
                if (!(wdID == null ? "" : wdID).equals(tsID == null ? "" : tsID))
                    throw new MismatchFault(i18n, "wd.mismatchID", canonDir);

            }   // try
            catch (FileNotFoundException e) {
                throw new BadDirectoryFault(i18n, "wd.noTestSuiteFile", canonDir);
            }
            catch (IOException e) {
                throw new BadDirectoryFault(i18n, "wd.badTestSuiteFile", canonDir, e);
            }
            catch (TestSuite.Fault e) {
                throw new TestSuiteFault(i18n, "wd.cantOpenTestSuite", canonDir, e.toString());
            }   // catch

            wd = new WorkDirectory(canonDir, ts, tsInfo);

            dirMap.put(canonDir, new WeakReference(wd));
        }   // sync block

        return wd;
    }

    /**
     * Open an existing work directory, using an explicit test suite. Any information
     * about the test suite previously associated with this work directory is overwritten
     * and lost. Therefore this method should be used with care: normally, a work directory
     * should be opened with {@link #open(File)}.
     *
     * @param dir The directory to be opened as a WorkDirectory.
     * @param testSuite The test suite to be associated with this work directory.
     * @return The WorkDirectory that is opened.
     * @throws FileNotFoundException if the directory identified by <code>dir</code> does
     *          not exist. If this exception is thrown, you may want to call {@link #create}
     *          instead.
     * @throws WorkDirectory.BadDirectoryFault if there was a problem opening
     *          the work directory.
     * @throws WorkDirectory.NotWorkDirectoryFault if the directory identified by
     *          <code>dir</code> is a valid directory, but has not yet been
     *          initialized as a work directory. f this exception is thrown,
     *          you may want to call {@link #create} instead.
     * @throws WorkDirectory.MismatchFault if the specified test suite does not
     *          match the ID recorded in the work directory.
     * @throws WorkDirectory.InitializationFault if there are unrecoverable
     *         problems encountered while reading the data present in the
     *         work directory
     */
    public static WorkDirectory open(File dir, TestSuite testSuite)
        throws FileNotFoundException,
               BadDirectoryFault,
               NotWorkDirectoryFault,
               MismatchFault,
               InitializationFault
    {
        //System.err.println("WD.open: " + dir + " ts=" + testSuite.getPath());
        if (!dir.exists())
            throw new FileNotFoundException(dir.getPath());

        File canonDir = canonicalize(dir);

        if (!canonDir.isDirectory())
            throw new BadDirectoryFault(i18n, "wd.notDirectory", canonDir);

        if (!canonDir.canRead())
            throw new BadDirectoryFault(i18n, "wd.notReadable", canonDir);

        File jtData = new File(canonDir, JTDATA);
        if (!jtData.exists())
            throw new NotWorkDirectoryFault(i18n, "wd.notWorkDir", canonDir);

        WorkDirectory wd = null;
        synchronized (dirMap) {
            WeakReference ref = (WeakReference)(dirMap.get(canonDir));
            if (ref != null)
                wd = (WorkDirectory)(ref.get());

            if (wd == null) {
                Properties tsInfo;
                try {
                    tsInfo = loadTestSuiteInfo(jtData);
                }
                catch (IOException e) {
                    tsInfo = null;
                }

                String wdID = (tsInfo == null ? null : (String) (tsInfo.get(TESTSUITE_ID)));
                String tsID = testSuite.getID();
                if (!(wdID == null ? "" : wdID).equals(tsID == null ? "" : tsID))
                    throw new MismatchFault(i18n, "wd.mismatchID", canonDir);

                // no existing instance, create one
                try {
                    wd = new WorkDirectory(canonDir, testSuite, tsInfo);
                    wd.saveTestSuiteInfo();
                    dirMap.put(canonDir, new WeakReference(wd));
                }
                catch (IOException e) {
                    throw new BadDirectoryFault(i18n, "wd.cantWriteTestSuiteInfo", canonDir, e);
                }
            }   // if
        }   // sync

        return wd;
    }

    /**
     * Create a WorkDirectory object for a given directory and testsuite.
     * The directory is assumed to be valid (exists(), isDirectory(), canRead() etc)
     */
    private WorkDirectory(File root, TestSuite testSuite, Map tsInfo)
    {
        if (root == null || testSuite == null)
            throw new NullPointerException();
        this.root = root;
        this.testSuite = testSuite;
        jtData = new File(root, JTDATA);

        if (jtData != null) {
            logFileName = jtData.getParent() + File.separator +
                LoggerFactory.LOGFILE_NAME + "." + LoggerFactory.LOGFILE_EXTENSION;
            testSuite.setLogFilePath(this);
            File loggerFile = new File(logFileName);
            try {
                loggerFile.createNewFile();
            } catch (IOException ioe) {
                testSuite.getNotificationLog(this).throwing("WorkDirectory", "WorkDirectory(File,TestSuite,Map)", ioe);
            }

        }

        // should consider saving parameter interview here;
        // -- possibly conditionally (don't need to write it in case of normal open)

        if (tsInfo != null) {
            String testC = (String) (tsInfo.get(TESTSUITE_TESTCOUNT));
            int tc;
            if (testC == null)
                tc = -1;
            else {
                try {
                    tc = Integer.parseInt(testC);
                }
                catch (NumberFormatException e) {
                    tc = -1;
                }
            }
            testCount = tc;
        }
        else
            testCount = testSuite.getEstimatedTestCount();

        testSuiteID = testSuite.getID();
        if (testSuiteID == null)
            testSuiteID = "";

    }

    /**
     * Get the root directory for this work directory.
     * @return the root directory for this work directory
     */
    public File getRoot() {
        return root;
    }

    /**
     * Get the root directory for this work directory.
     * @return the path of the root directory for this work directory
     */
    public String getPath() {
        return root.getPath();
    }

    /**
     * Get the data directory for this work directory.
     * @return the system (jtData) directory for this work directory
     */
    public File getJTData() {
        return jtData;
    }

    /**
     * Get a file in this work directory.
     * @param name the name of a file within this work directory
     * @return the full (absolute) name of the specified file
     */
    public File getFile(String name) {
        return new File(root, name);
    }

    /**
     * Get a file in the system directory for this work directory.
     * @param name the name of a file within the wsystem (jtData) directory
     * @return the full (absolute) name of the specified file
     */
    public File getSystemFile(String name) {
        return new File(jtData, name);
    }

    /**
     * Get the test suite for this work directory.
     * @return the test suite for which this is a work directory
     */
    public TestSuite getTestSuite() {
        return testSuite;
    }

    /**
     * Find out the number of tests in the entire test suite.
     * This number is collected from either a previous iteration of the
     * testsuite or from the TestSuite object.
     * @return the number of tests in the test suite, -1 if not known.
     * @see #setTestSuiteTestCount
     * @see TestSuite#getEstimatedTestCount
     */
    public int getTestSuiteTestCount() {
        return testCount;
    }

    /**
     * Specify the total number of tests found in this testsuite.
     * When available, this class prefers to use this number rather
     * than that provided by a TestSuite object.
     * @param num the number of tests in the test suite
     * @see #getTestSuiteTestCount
     * @see TestSuite#getEstimatedTestCount
     */
    public void setTestSuiteTestCount(int num) {
        if (num != testCount) {
            testCount = num;

            try {
                saveTestSuiteInfo();
            }
            catch (IOException e) {
                // oh well, this isn't critical
            }
        }
    }


    /**
     * Get a test result table containing the test results in this work directory.
     * @return a test result table containing the test results in this work directory
     * @see #setTestResultTable
     */
    public TestResultTable getTestResultTable() {
        if (testResultTable == null)
            testResultTable = new TestResultTable(this);

        return testResultTable;
    }

    /**
     * Set a test result table containing the test descriptions for the tests in this
     * test suite.
     * @param trt a test result table containing the test descriptions for the tests
     * in this work directory
     * @throws NullPointerException if trt is null.
     * @throws IllegalArgumentException in the test result table has not been initialized
     * with the identical same finder as that obtained from the test suite of the work
     * directory, or if the test result table has been initialized with a different
     * work directory.
     * @throws IllegalStateException if the test result table has already been set to
     * a different value.
     * @see #getTestResultTable
     */
    public void setTestResultTable(TestResultTable trt) {
        if (trt == null)
            throw new NullPointerException();

        if (trt == testResultTable) {
            // already set to the correct value
            return;
        }

        if (testResultTable != null && testResultTable != trt) {
            // already set to something else
            throw new IllegalStateException();
        }

        WorkDirectory trt_wd = trt.getWorkDirectory();
        if (trt_wd != null && trt_wd != this)
            throw new IllegalArgumentException();

        if (trt_wd == null)
            trt.setWorkDirectory(this);

        testResultTable = trt;
    }

    /**
     * Print a text message to the workdir logfile.
     * A single line of text which is as short as possible is highly
     * recommended for readability purposes.
     *
     * @param i18n a resource bundle containing the localized messages
     * @param key a key into the resource bundle for the required message
     *
     * @since 3.0.1
     */
    public void log(I18NResourceBundle i18n, String key) {
        ensureLogFileInitialized();
        logFile.log(i18n, key);
    }

    /**
     * Print a text message to the workdir logfile.
     * A single line of text which is as short as possible is highly
     * recommended for readability purposes.
     *
     * @param i18n a resource bundle containing the localized messages
     * @param key a key into the resource bundle for the required message
     * @param arg An argument to be formatted into the specified message.
     *          If this is a <code>Throwable</code>, its stack trace
     *          will be included in the log.
     * @since 3.0.1
     */
    public void log(I18NResourceBundle i18n, String key, Object arg) {
        ensureLogFileInitialized();
        logFile.log(i18n, key, arg);
    }

    /**
     * Print a text message to the workdir logfile.
     * A single line of text which is as short as possible is highly
     * recommended for readability purposes.
     *
     * @param i18n a resource bundle containing the localized messages
     * @param key a key into the resource bundle for the required message
     * @param args An array of arguments to be formatted into the specified message.
     *          If the first arg is a <code>Throwable</code>, its stack
     *          trace will be included in the log.
     * @since 3.0.1
     */
    public void log(I18NResourceBundle i18n, String key, Object[] args) {
        ensureLogFileInitialized();
        logFile.log(i18n, key, args);
    }

    private void ensureLogFileInitialized() {
        if (logFile == null)
            logFile = new LogFile(getSystemFile("log.txt"));
    }

    /**
     * Clean the contents of the given path.  If <tt>path</tt> is a
     * directory, it is recursively deleted.  If it is a file, that file
     * is removed.
     * Any user confirmation should be done before calling this method.
     * If the path does not exist in this work directory, false is returned.
     * @param path Path to a directory in this work directory or a path to
     *        a jtr file.  A zero length string removes the root.
     * @return true is the purge occured normally, false if the purge did not
     *         complete for some reason.  Most failures to purge will be
     *         announced by Faults.  A null parameter will result in
     *         false.
     * @throws WorkDirectory.PurgeFault If the file cannot be removed; the message field
     *         may not contain any useful information due to deficiencies in
     *         java.io.File.delete()..
     */
    public boolean purge(String path) throws PurgeFault {
        if (path == null)
            return false;

        boolean result = true;

        File f = (path.length() == 0 ? root : getFile(path));

        if (!f.exists())
            return false;

        if (f.isDirectory())
            result = recursivePurge(f, path);
        else {
            // single test
            result = f.delete();
            testResultTable.resetTest(path);
        }

        return result;
    }


    // ------------ PRIVATE --------------

    /**
     * @return False if any part of the removal process failed to complete.
     * @throws PurgeFault If the file cannot be removed; the message field
     *         may not contain any useful information due to deficiencies in
     *         java.io.File.delete()..
     */
    private boolean recursivePurge(File dir, String pathFromRoot) throws PurgeFault {
        boolean result = true;
        File[] files = dir.listFiles();

        for (int i = 0; i < files.length; i++) {
            File f = files[i];

            String p; // root-relative path for f
            if (pathFromRoot.length() == 0)
                p = f.getName();
            else
                p = pathFromRoot + "/" + f.getName();

            if (f.isFile()) {
                result &= f.delete();
                if (f.getName().endsWith(TestResult.EXTN))
                    testResultTable.resetTest(p);
            }
            else if (!p.equals(JTDATA)) {
                // directory, make sure not to delete jtData
                result &= recursivePurge(f, p);
                result &= f.delete();
            }
        }

        return result;
    }

    private static Properties loadTestSuiteInfo(File jtData) throws FileNotFoundException, IOException {
        File f = new File(jtData, TESTSUITE);
        InputStream in = new BufferedInputStream(new FileInputStream(f));
        Properties p = new Properties();
        p.load(in);
        in.close();
        return p;
    }

    private synchronized void saveTestSuiteInfo() throws IOException {
        File f = File.createTempFile(TESTSUITE, ".new", jtData);
        OutputStream out = new BufferedOutputStream(new FileOutputStream(f));
        Properties p = new Properties();
        p.put(TESTSUITE_ROOT, testSuite.getPath());
        String name = testSuite.getName();
        if (name != null)
            p.put(TESTSUITE_NAME, name);

        if (testCount > 0)
            p.put(TESTSUITE_TESTCOUNT, Integer.toString(testCount));

        if (testSuiteID != null && testSuiteID.length() > 0)
            p.put(TESTSUITE_ID, testSuiteID);

        p.save(out, "JT Harness Work Directory: Test Suite Info");
        out.close();

        // trying to get near-atomic updates to this file
        File to = new File(jtData, TESTSUITE);
        if (!f.renameTo(to)) {
            // it happends
            to.delete();
            f.renameTo(to);
        }
    }

    private static File canonicalize(File dir) throws BadDirectoryFault {
        try {
            return dir.getCanonicalFile();
        }
        catch (IOException e) {
            throw new BadDirectoryFault(i18n, "wd.cantCanonicalize", dir, e);
        }
    }

    private File root;
    private TestSuite testSuite;
    private String testSuiteID;
    private int testCount = -1;
    private TestResultTable testResultTable;
    private File jtData;
    private String logFileName;
    private LogFile logFile;
    private static HashMap dirMap = new HashMap(2);     // must be manually synchronized
    private static final String JTDATA = "jtData";
    private static final String TESTSUITE = "testsuite";
    private static final String TESTSUITE_ID = "id";
    private static final String TESTSUITE_NAME = "name";
    private static final String TESTSUITE_ROOT = "root";
    private static final String TESTSUITE_TESTCOUNT = "testCount";

    private static I18NResourceBundle i18n = I18NResourceBundle.getBundleForClass(WorkDirectory.class);
}
