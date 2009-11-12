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
package com.sun.javatest.exec;

import java.io.File;

import com.sun.javatest.CompositeFilter;
import com.sun.javatest.InitialUrlFilter;
import com.sun.javatest.Parameters;
import com.sun.javatest.TestDescription;
import com.sun.javatest.ObservableTestFilter;
import com.sun.javatest.TestFilter;
import com.sun.javatest.util.I18NResourceBundle;
import com.sun.javatest.util.StringArray;

/**
 * This filter knows how to deal with the the Parameters interface to get
 * the necessary filtering effect.
 *
 * @see com.sun.javatest.Parameters
 */

class ParameterFilter extends ObservableTestFilter {
    ParameterFilter() {
        super();
    }

    // ------- this class' methods -------
    /**
     * Should this filter remove tests not specified in the initial URLs.  By
     * default, the tests which are not included in the initial files are
     * rejected by the filter.
     *
     * @param state True if you want non-initial URLs tests to be rejected.
     * @see #isTestsFiltered()
     */
    public void setFilterTests(boolean state) {
        rmInitFiles = state;
    }

    /**
     * Are initial files being filtered out?
     *
     * @see #setFilterTests(boolean)
     */
    public boolean isTestsFiltered() {
        return rmInitFiles;
    }

    // ------- TestFilter ---------
    public String getName() {
        return i18n.getString("pFilter.name");
    }

    public String getDescription() {
        return i18n.getString("pFilter.desc");
    }

    public String getReason() {
        return i18n.getString("pFilter.reason");
    }

    public boolean accepts(TestDescription td) throws Fault {
        return accepts(td, null);
    }

    public boolean accepts(TestDescription td, TestFilter.Observer o) throws Fault {
        // need to handle the initial url filter as a special case
        if (filters == null || filters.length == 0)
            if (iurlFilter == null)
                return true;
            else {
                boolean result = iurlFilter.accepts(td);
                if (!result && o != null)
                    o.rejected(td, iurlFilter);
            }

        for (int i = 0; i < filters.length; i++) {
            boolean result = filters[i].accepts(td);
            if (!result) {
                if (o != null)
                    o.rejected(td, filters[i]);

                return false;
            }
        }   // for

        // check initial URL filter
        if (rmInitFiles && iurlFilter != null && !iurlFilter.accepts(td)) {
            if (o != null)
                o.rejected(td, iurlFilter);

            return false;
        }

        // test accepted
        return true;
    }

    // ------- Composite overrides ---------
    /**
     * Gets the set of filters that the parameters have supplied.
     * Depending on the initial url setting, an InitialUrlFilter may or may not
     * be included in this set.  The returned array has already be shallow copied.
     *
     * @return The filters in use.  This is affected by the isFilterTests()
     *         state.
     * @see com.sun.javatest.InitialUrlFilter
     */
    public TestFilter[] getTestFilters() {
        TestFilter[] copy = null;

        // allocate shallow copy array
        // change size depending on whether url filter is to be included
        if (rmInitFiles && iurlFilter != null)
            copy = new TestFilter[filters.length + 1];
        else
            copy = new TestFilter[filters.length];

        System.arraycopy(filters, 0, copy, 0, filters.length);

        // add the final filter
        if (rmInitFiles) {
            // init. file filter being used, append it
            copy[copy.length-1] = iurlFilter;
        }

        return copy;
    }

    // ---------- methods for this class -----------

    /**
     * Should be called whenever the parameters or filters inside
     * may have changed.
     */
    void update(Parameters p) {
        if (p == null)
            return;

        boolean wasUpdated = false;

        if (params == null) {       // first time
            // record and send observer msg
            params = p;
            filters = p.getFilters();
            wasUpdated = true;
        }
        else if (p != params) {     // ref. change
            // record and send observer msg
            params = p;
            filters = p.getFilters();
            wasUpdated = true;
        }
        else {                      // internal update only
            // if same as present, compare filters
            TestFilter[] newFilters = p.getFilters();

            if (newFilters == null && filters == null) {
                // do nothing, no change
            }
            else if ((newFilters == null && filters != null) ||
                     (filters == null && newFilters != null)) {
                filters = newFilters;
                wasUpdated = true;
            }
            else if (newFilters.length == filters.length) {
                // do set comparison on the old and new filters
                if (!CompositeFilter.equals(newFilters, filters)) {
                    filters = newFilters;
                    wasUpdated = true;
                }
            }
            else {   // there are more or fewer filters than before
                filters = newFilters;
                wasUpdated = true;
            }
        }

        // null or empty check is done by the filter class
        // should be smart about setting wasUpdated flag

        String[] initStrings = p.getTests();
        File[] initFiles = stringsToFiles(initStrings);

        // could optimize out this code if rmInitFiles is false
        iurlFilter = new InitialUrlFilter(initFiles);
        wasUpdated = (wasUpdated || !StringArray.join(initStrings).equals(lastInitStrings));
        lastInitStrings = StringArray.join(initStrings);

        if (wasUpdated)
            notifyUpdated(this);
    }

    private static File[] stringsToFiles(String[] tests) {
        if (tests == null)
            return null;

        File[] files = new File[tests.length];
        for (int i = 0; i < tests.length; i++)
            files[i] = new File(tests[i]);

        return files;
    }

    private InitialUrlFilter iurlFilter;    // not appended into filters
    private boolean rmInitFiles = true;
    private String lastInitStrings;
    private TestFilter[] filters;
    private String[] initUrls;
    private Parameters params;
    private static I18NResourceBundle i18n = I18NResourceBundle.getBundleForClass(ParameterFilter.class);
}
