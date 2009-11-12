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

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.nio.channels.FileChannel;
import java.util.Vector;

import com.sun.javatest.TestSuite;
import com.sun.javatest.WorkDirectory;
import com.sun.javatest.util.I18NResourceBundle;
import com.sun.javatest.util.XMLWriter;

public class Main {
        /**
         * Thrown when a bad command line argument is encountered.
         */
        public static class BadArgs extends Exception {
                /**
                 * Serialization support
                 */
                private static final long serialVersionUID = 4638654313770205243L;

                /**
                 * Create a BadArgs exception.
                 * @param i18n A resource bundle in which to find the detail message.
                 * @param key The key for the detail message.
                 */
                BadArgs(I18NResourceBundle i18n, String key) {
                        super(i18n.getString(key));
                }

                /**
                 * Create a BadArgs exception.
                 * @param i18n A resource bundle in which to find the detail message.
                 * @param key The key for the detail message.
                 * @param arg An argument to be formatted with the detail message by
                 * {@link java.text.MessageFormat#format}
                 */
                BadArgs(I18NResourceBundle i18n, String key, Object arg) {
                        super(i18n.getString(key, arg));
                }

                /**
                 * Create a BadArgs exception.
                 * @param i18n A resource bundle in which to find the detail message.
                 * @param key The key for the detail message.
                 * @param args An array of arguments to be formatted with the detail message by
                 * {@link java.text.MessageFormat#format}
                 */
                BadArgs(I18NResourceBundle i18n, String key, Object[] args) {
                        super(i18n.getString(key, args));
                }
        }

        /**
         * This exception is used to report problems that occur while the program is running.
         */
        public static class Fault extends Exception {
                /**
                 * Serialization support
                 */
                private static final long serialVersionUID = -4066018402688615825L;

                /**
                 * Create a Fault.
                 * @param i18n A resource bundle in which to find the detail message.
                 * @param s The key for the detail message.
                 */
                Fault(I18NResourceBundle i18n, String s) {
                        super(i18n.getString(s));
                }

                /**
                 * Create a Fault.
                 * @param i18n A resource bundle in which to find the detail message.
                 * @param s The key for the detail message.
                 * @param o An argument to be formatted with the detail message by
                 * {@link java.text.MessageFormat#format}
                 */
                Fault(I18NResourceBundle i18n, String s, Object o) {
                        super(i18n.getString(s, o));
                }

                /**
                 * Create a Fault.
                 * @param i18n A resource bundle in which to find the detail message.
                 * @param s The key for the detail message.
                 * @param o An array of arguments to be formatted with the detail message by
                 * {@link java.text.MessageFormat#format}
                 */
                Fault(I18NResourceBundle i18n, String s, Object[] o) {
                        super(i18n.getString(s, o));
                }
        }

        private static final String COF_SCHEMA = "COF2_0_2.xsd";

        private static I18NResourceBundle i18n = I18NResourceBundle
                        .getBundleForClass(Main.class);

        private static File suiteCopy;

        private static void exit(int exitCode) {
                System.exit(exitCode);
                throw new Error();
        }

        public static void main(String[] args) {

                PrintWriter out = new PrintWriter(System.err);

                try {
                        Main m = new Main();
                        m.run(args, out);
                        out.flush();
                        exit(0);
                } catch (BadArgs e) {
                        out.println(e.getMessage());
                        //showCommandLineHelp(out);
                        out.flush();
                        exit(1);
                } catch (Fault e) {
                        out.println(e.getMessage());
                        out.flush();
                        exit(2);
                }
        }

        private static TestSuite openTestSuite(File path) throws Fault {
                try {
                        return TestSuite.open(path);
                } catch (FileNotFoundException e) {
                        // should not happen, because of prior dir.exists() check
                        throw new Fault(i18n, "main.cantFindTestSuite", path);
                } catch (TestSuite.Fault f) {
                        throw new Fault(i18n, "main.cantOpenTestSuite", new Object[] {
                                        path, f });
                }
        }

        private static WorkDirectory openWorkDirectory(File wdPath, File tsPath)
                        throws Fault {
                try {
                        WorkDirectory wd;
                        if (tsPath == null)
                                wd = WorkDirectory.open(wdPath);
                        else {
                                // with non default suite WorkDirectory.open overwrites
                                // original suite.
                                // so copy before open
                                File original = new File(wdPath.getPath()+"/jtData/testsuite");
                                suiteCopy = File.createTempFile("cof",".tmp");
                                copyFile(original, suiteCopy);
                                wd = WorkDirectory.open(wdPath, openTestSuite(tsPath));
                        }
                        return wd;
                } catch (FileNotFoundException e) {
                        // should not happen, because of prior dir.exists() check
                        throw new Fault(i18n, "main.cantFindWorkDir", wdPath);
                } catch (WorkDirectory.Fault f) {
                        throw new Fault(i18n, "main.cantOpenWorkDir", new Object[] {
                                        wdPath, f });
                } catch (IOException e) {
                        throw new RuntimeException(e);
                }
        }

        private static void showCommandLineHelp(PrintWriter out) {
                String progName = System.getProperty("program", "java " + Main.class
                                .getName());

                out.println(i18n.getString("main.cmdLine.proto", progName));
                out.println();
                out.println(i18n.getString("main.cmdLine.optHead"));
                out.println(i18n.getString("main.cmdLine.file"));
                out.println(i18n.getString("main.cmdLine.help"));
                out.println(i18n.getString("main.cmdLine.out"));
                out.println(i18n.getString("main.cmdLine.ts"));
                out.println(i18n.getString("main.cmdLine.xsd"));
                out.println(i18n.getString("main.cmdLine.no-testcases"));
                out.println(i18n.getString("main.cmdLine.data"));
                out.println();
                out.println(i18n.getString("main.cmdLine.filesHead"));
                out.println(i18n.getString("main.cmdLine.dir"));
                out.println();
                out.println(i18n.getString("main.copyright.txt"));
        }

        void run(String[] args, PrintWriter log) throws BadArgs, Fault {
                boolean helpFlag = false;
                boolean xsdFlag = false;
                File outputFile = null;

                if (args.length == 0)
                        helpFlag = true;
                else {
                        for (int i = 0; i < args.length; i++) {
                                if (args[i].equals("-o") && (i + 1 < args.length)) {
                                        outputFile = new File(args[++i]);
                                } else if (args[i].equals("-f") && (i + 1 < args.length)) {
                                        data.add(new File(args[++i]));
                                } else if (args[i].equalsIgnoreCase("-help") || args[i]
                                                .equalsIgnoreCase("-usage")
                                                || args[i].equalsIgnoreCase("/?")) {
                                        helpFlag = true;
                                } else if (args[i].equalsIgnoreCase("-no-testcases")) {
                                        COFTest.noTestCases = true;
                                        COFTest.xmlElements.remove("testcases");
                                } else if (args[i].equalsIgnoreCase("-xsd")) {
                                        xsdFlag = true;
                                } else if (args[i].equalsIgnoreCase("-ts") && (i + 1 < args.length)) {
                                        tsPath = new File(args[++i]);
                                } else if (args[i].startsWith("-")) {
                                        throw new BadArgs(i18n, "main.badOpt", args[i]);
                                } else if (args[i].indexOf("=") != -1) {
                                        data.add(args[i]);
                                } else if (i == args.length - 1)
                                        // currently, only accept one directory;
                                        // we could accept more and turn them into
                                        // separate testsuites in the report, but
                                        // would have trouble identifying environments
                                        // for them
                                        dir = new File(args[i]);
                                else
                                        throw new BadArgs(i18n, "main.badArg", args[i]);
                        }
                }

                if (helpFlag) {
                        showCommandLineHelp(log);
                        if (dir == null && outputFile == null)
                                return;
                }

                if (outputFile == null)
                        throw new BadArgs(i18n, "main.noOutputFile");

                if (dir == null)
                        throw new BadArgs(i18n, "main.noResults");

                if (!dir.exists())
                        throw new BadArgs(i18n, "main.cantFindResults");

                COFData cofData;
                try {
                        cofData = new COFData(data);
                } catch (IllegalArgumentException e) {
                        throw new Fault(i18n, "main.badOpt", e.toString());
                } catch (IOException e) {
                        throw new Fault(i18n, "main.cantReadFile", e);
                }

                try {
                        Report r = fillReport(cofData);
                        XMLWriter out = new XMLWriter(new FileWriter(outputFile));
                        r.write(out);
                        out.close();
                } catch (IOException e) {
                        throw new Fault(i18n, "main.cantWriteFile", new Object[] {
                                        outputFile, e });
                }

                if (xsdFlag) {
                        File xsdFile = new File(outputFile.getAbsoluteFile().getParent(),
                                        COF_SCHEMA);
                        try {
                                InputStream in = new BufferedInputStream(getClass()
                                                .getResourceAsStream(COF_SCHEMA));
                                OutputStream out = new BufferedOutputStream(
                                                new FileOutputStream(xsdFile));
                                byte[] buf = new byte[4096];
                                int n;
                                while ((n = in.read(buf, 0, buf.length)) != -1)
                                        out.write(buf, 0, n);
                                in.close();
                                out.close();
                        } catch (IOException e) {
                                throw new Fault(i18n, "main.cantWriteFile", new Object[] {
                                                xsdFile, e });
                        }
                }
                if (suiteCopy!=null) {
                        File original = new File(dir.getPath()+"/jtData/testsuite");
                        copyFile(suiteCopy, original);
                        suiteCopy.delete();
                }
        }

        private Report fillReport(COFData cofData) throws Fault {
                COFEnvironment cof_env = new COFEnvironment(cofData);
                cof_env.setId(cofData.get("environment.id","env:0"));

                COFTestSuite cof_ts;

                if (WorkDirectory.isWorkDirectory(dir)) {
                        cofData.put("workdir", dir.getAbsolutePath());
                        cof_ts = new COFTestSuite(openWorkDirectory(dir, tsPath), cofData);
                } else
                        cof_ts = new COFTestSuite(dir, cofData);


                Report r = new Report(cof_env, cof_ts);
                r.setHarness(cofData.get("report.harness"));

                COFSWEntities entities = new COFSWEntities();
                COFSWEntity entity = new COFSWEntity();
                entity.setId(cofData.get("swentity.id", "sw:0"));
                entity.setName(cofData.get("swentity.name", "JDK"));
                entity.setType(cofData.get("swentity.type", "java"));
                entity.setVersion(cofData.get("swentity.version"));
                entity.setDescription(cofData.get("swentity.description"));
                entities.getSwentity().add(entity);
                r.setSwentities(entities);

                COFApplications apps= new COFApplications();
                COFApplication app = new COFApplication();
                app.setEnvironmentid(cof_env.getId());
                app.setSwentityid(entity.getId());
                apps.getApplication().add(app);
                r.setApplications(apps);
                return r;
        }

        public static void copyFile(File in, File out) {
                try {
                        FileChannel sourceChannel = new FileInputStream(in).getChannel();
                        FileChannel destinationChannel = new FileOutputStream(out).getChannel();
                        sourceChannel.transferTo(0, sourceChannel.size(), destinationChannel);
                        sourceChannel.close();
                        destinationChannel.close();
                } catch (FileNotFoundException e) {
                        System.out.println(e.getLocalizedMessage());
                } catch (IOException e) {
                        System.out.println(e.getLocalizedMessage());
                }
        }

        File dir = null;
        File tsPath = null;
        Vector data = new Vector();

}
