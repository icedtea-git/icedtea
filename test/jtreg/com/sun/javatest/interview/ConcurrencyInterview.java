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

import com.sun.interview.FinalQuestion;
import com.sun.interview.IntQuestion;
import com.sun.interview.Interview;
import com.sun.interview.Question;
import com.sun.javatest.Parameters;

/**
 * This interview collects the concurrency parameter. It is normally used as
 * one of a series of sub-interviews that collect the parameter information for
 * a test run.
 */
public class ConcurrencyInterview
    extends Interview
    implements Parameters.MutableConcurrencyParameters
{
    /**
     * Create an interview.
     * @param parent The parent interview of which this is a child.
     * @throws Interview.Fault if there is a problem while creating the interview.
     */
    public ConcurrencyInterview(Interview parent)
        throws Interview.Fault
    {
        super(parent, "concurrency");
        this.parent = parent;
        setResourceBundle("i18n");
        setFirstQuestion(qConcurrency);
    }

    /**
     * Get the concurrency value from the interview.
     * @return an integer representing the desired concurrency for a test run.
     * @see #setConcurrency
     */
    public int getConcurrency() {
        return qConcurrency.getValue();
    }

    /**
     * Set the concurrency value in the interview.
     * @param conc The desired concurrency value.
     * @see #getConcurrency
     */
    public void setConcurrency(int conc) {
        qConcurrency.setValue(conc);
    }

    //----------------------------------------------------------------------------
    //
    // Concurrency

    private IntQuestion qConcurrency = new IntQuestion(this, "concurrency") {
        {
            setBounds(Parameters.ConcurrencyParameters.MIN_CONCURRENCY,
                      Parameters.ConcurrencyParameters.MAX_CONCURRENCY);
        }

        protected Question getNext() {
            return qEnd;
        }

        public void clear() {
            setValue(1);
        }
    };

    //----------------------------------------------------------------------------
    //
    // End

    private Question qEnd = new FinalQuestion(this);

    //--------------------------------------------------------

    private Interview parent;
}
