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
package com.sun.javatest.finder;

import java.io.File;
import com.sun.javatest.TestDescription;
import com.sun.javatest.TestEnvironment;
import com.sun.javatest.TestFinder;
import com.sun.javatest.util.I18NResourceBundle;

/**
 * A test finder that reads tests from a delegate, and returns the
 * results in the reverse order. This is primarily for debugging
 * and testing purposes.
 */
public class ReverseTestFinder extends TestFinder
{
    /**
     * Initialize the test finder.
     * @param args The first entry in the array should be the name
     *          of the test finder to be used to actually read the tests;
     *          subsequent entries in the array will be passed through to
     *          the init method for that class.
     * @param testSuiteRoot The root file of the test suite to be read.
     * @param env An environment for the test finder to use if required.
     *          The ReverseTestFinder does not use this value directly;
     *          it just passes it on to the test finder to which it
     *          delegates the reading.
     * @throws TestFinder.Fault if any problems occur during initialization.
     */
    public synchronized void init(String[] args, File testSuiteRoot,
                     TestEnvironment env) throws Fault {
         String delegateClassName = args[0];
         try {
             Class  delegateClass = Class.forName(delegateClassName);
             delegate = (TestFinder)(delegateClass.newInstance());
             args = shift(args, 1);
             delegate.init(args, testSuiteRoot, env);
         }
         catch (Throwable t) {
             throw new Fault(i18n, "reverse.cantInitDelegate", t);
         }
    }

    public File getRoot() {
        return delegate.getRoot();
    }

    public File getRootDir() {
        return delegate.getRootDir();
    }

    public void read(File file) {
        delegate.read(file);
    }

    public TestDescription[] getTests() {
        TestDescription[] tds = delegate.getTests();
        if (tds != null) {
            int n = tds.length;
            for (int i = 0; i < n/2; i++) {
                TestDescription temp = tds[i];
                tds[i] = tds[n - 1 - i];
                tds[n - 1 - i] = temp;
            }
        }
        return tds;
    }

    public File[] getFiles() {
        File[] fs = delegate.getFiles();
        if (fs != null) {
            int n = fs.length;
            for (int i = 0; i < n/2; i++) {
                File temp = fs[i];
                fs[i] = fs[n - 1 - i];
                fs[n - 1 - i] = temp;
            }
        }
        return fs;
    }

    protected void scan(File file) {
        throw new Error("should not be called!");
    }

    private String[] shift(String[] args, int n) {
        String[] result = new String[args.length - n];
        System.arraycopy(args, n, result, 0, result.length);
        return result;
    }

    private TestFinder delegate;

    private static I18NResourceBundle i18n = I18NResourceBundle.getBundleForClass(TagTestFinder.class);

}
