/*
 * Copyright 2007 Sun Microsystems, Inc.  All Rights Reserved.
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

/*
 * TODO: XMLReporter
 * TODO: filter options
 * TODO: comparator option
 * TODO: css option
 **/

package com.sun.javatest.diff;

import com.sun.javatest.regtest.BadArgs;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.List;

import com.sun.javatest.Status;
import com.sun.javatest.TestResult;
import com.sun.javatest.TestSuite;
import com.sun.javatest.WorkDirectory;
import com.sun.javatest.regtest.AntOptionDecoder;
import com.sun.javatest.regtest.Option;
import com.sun.javatest.regtest.OptionDecoder;
import com.sun.javatest.util.I18NResourceBundle;

import static com.sun.javatest.regtest.Option.ArgType.*;

/**
 * Main entry point for jtdiff.
 */
public class Main {
    
    /**
     * Exception to report a problem while executing in Main.
     */
    public static class Fault extends Exception {
        static final long serialVersionUID = 1607979458544175906L;
        Fault(I18NResourceBundle i18n, String s, Object... args) {
            super(i18n.getString(s, args));
        }
    }
    //---------- command line option decoding ----------------------------------
  
    private static final String COMPARE = "compare";
    private static final String OUTPUT = "output";
    private static final String DOC = "doc";
    private static final String FILES = "files";
    
    Option[] options = {
        new Option(NONE, COMPARE, "r", "reason") {
            public void process(String opt, String arg) {
                includeReason = true;
            }
        },
        new Option(OLD, OUTPUT, "o", "o", "outFile") {
            public void process(String opt, String arg) {
                outFile = new File(arg);
            }
        },
        new Option(STD, OUTPUT, "format", "format") {
            public void process(String opt, String arg) {
                format = arg;
            }
        },
        new Option(OLD, OUTPUT, "title", "title") {
            public void process(String opt, String arg) {
                title = arg;
            }
        },
        new Option(REST, DOC, "help", "h", "help", "usage") {
            public void process(String opt, String arg) {
                if (help == null)
                    help = new Help(options);
                help.setCommandLineHelpQuery(arg);
            }
        },
        new Option(NONE, DOC, "help", "version") {
            public void process(String opt, String arg) {
                if (help == null)
                    help = new Help(options);
                help.setVersionFlag(true);
            }
        },
        new Option(FILE, FILES, null) {
            public void process(String opt, String arg) {
                File f = new File(arg);
                fileArgs.add(f);
            }
        }
    };
    
    //---------- Command line invocation support -------------------------------
    
    /**
     * Standard entry point. Only returns if GUI mode is initiated; otherwise, it calls System.exit
     * with an appropriate exit code.
     * @param args An array of args, such as might be supplied on the command line.
     */
    public static void main(String[] args) {
        PrintWriter out = new PrintWriter(System.out, true);
        PrintWriter err = new PrintWriter(System.err, true);
        Main m = new Main(out, err);
        try {
            boolean ok;
            try {
                ok = m.run(args);
            } finally {
                out.flush();
            }
            
            if (!ok) {
                // take care not to exit if GUI might be around,
                // and take care to ensure JavaTestSecurityManager will
                // permit the exit
                exit(1);
            }
        } catch (Fault e) {
            err.println(i18n.getString("main.error", e.getMessage()));
            exit(2);
        } catch (BadArgs e) {
            err.println(i18n.getString("main.badArgs", e.getMessage()));
            new Help(m.options).showCommandLineHelp(out);
            exit(2);
        } catch (InterruptedException e) {
            err.println(i18n.getString("main.interrupted"));
            exit(2);
        } catch (Exception e) {
            err.println(i18n.getString("main.unexpectedException"));
            e.printStackTrace();
            exit(3);
        }
    } // main()
    
    public Main() {
        this(new PrintWriter(System.out, true), new PrintWriter(System.err, true));
    }
    
    public Main(PrintWriter out, PrintWriter err) {
        this.out = out;
        this.err = err;
    }
    
    /**
     * Decode command line args and perform the requested operations.
     * @param args An array of args, such as might be supplied on the command line.
     * @throws BadArgs if problems are found with any of the supplied args
     * @throws Fault if exception problems are found while trying to compare the results
     * @throws InterruptedException if the tool is interrupted while comparing the results
     */
    public final boolean run(String[] args) throws BadArgs, Fault, InterruptedException {
        new OptionDecoder(options).decodeArgs(args);
            
        return run();
    }
    
    private boolean run() throws Fault, InterruptedException {
        if (fileArgs.size() == 0 && help == null) {
            help = new Help(options);
            help.setCommandLineHelpQuery(null);
        }
        
        if (help != null) {
            help.show(out);
            return true;
        }
        
        List<DiffReader> list = new ArrayList<DiffReader>();
        for (File f: fileArgs)
            list.add(open(f));
        
        PrintWriter prevOut = out;
        if (outFile != null) {
            try {
                out = new PrintWriter(new BufferedWriter(new FileWriter(outFile))); // FIXME don't want to use PrintWriter
            } catch (IOException e) {
                throw new Fault(i18n, "main.cantOpenFile", outFile, e);
            }
        }
        
        try {
            initComparator();  
            
            initReporter();  
            reporter.setTitle(title); 
            reporter.setComparator(comparator);
            reporter.setReaders(list);

            List<int[]> testCounts = new ArrayList<int[]>();
            MultiMap<String,TestResult> table = new MultiMap<String,TestResult>();
            for (DiffReader r: list) {
                int index = table.add(r.getFile().getPath());
                int[] counts = new int[Status.NUM_STATES];
                for (TestResult tr: r) {
                    table.add(index, tr.getTestName(), tr);
                    counts[tr.getStatus().getType()]++;
                }
                testCounts.add(counts);
            }

            reporter.setTestCounts(testCounts);
            
            try {
                reporter.write(table);
            } catch (IOException e) {
                throw new Fault(i18n, "main.ioError", e);
            }

            return (reporter.diffs == 0);
        } finally {
            if (out != prevOut) {
//                try {
                    out.close();
//                } catch (IOException e) {
//                    throw new Fault(i18n, "main.ioError", e);
//                }
                out = prevOut;
            }
        }
    }
    
    private void initFormat() {
        if (format == null && outFile != null) {
            String name = outFile.getName();
            int dot = name.lastIndexOf(".");
            if (dot != -1) 
                format = name.substring(dot + 1).toLowerCase();
        }
    }
    
    private void initReporter() throws Fault {
        if (reporter == null) {
            try {
                initFormat();
                if (format != null && format.equals("html"))
                    reporter = new HTMLReporter(out);
                if (reporter == null)
                    reporter = new SimpleReporter(out); 
            } catch (IOException e) {
                throw new Fault(i18n, "main.cantOpenReport", e);
            }
        }
    }
    
    private void initComparator() {
        if (comparator == null)
            comparator = new StatusComparator(includeReason); 
    }
    
    private DiffReader open(File f) throws Fault {
        if (!f.exists())
            throw new Fault(i18n, "main.cantFindFile", f);
        
        try {
            if (WorkDirectoryReader.accepts(f)) 
                return new WorkDirectoryReader(f);
            
            if (ReportReader.accepts(f)) 
                return new ReportReader(f);
            
            throw new Fault(i18n, "main.unrecognizedFile", f);
            
        } catch (TestSuite.Fault e) {
            throw new Fault(i18n, "main.cantOpenFile", f, e);
        } catch (WorkDirectory.Fault e) {
            throw new Fault(i18n, "main.cantOpenFile", f, e);
        } catch (IOException e) {
            throw new Fault(i18n, "main.cantOpenFile", f, e);
        }
        
    }
    
    private static void exit(int exitCode) {
        System.exit(exitCode);
    }
    
    private PrintWriter out;
    private PrintWriter err;
    private Comparator<TestResult> comparator;
    private Reporter reporter;
    
    private boolean includeReason;
    private String format;
    private String title;
    private File outFile;
    private List<File> fileArgs = new ArrayList<File>();
    private Help help;
    
    private static I18NResourceBundle i18n = I18NResourceBundle.getBundleForClass(Main.class);
}
