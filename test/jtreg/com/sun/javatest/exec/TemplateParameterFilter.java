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

import com.sun.interview.Interview;
import com.sun.javatest.InterviewParameters;
import com.sun.javatest.TemplateUtilities;
import com.sun.javatest.TestDescription;
import com.sun.javatest.TestFilter;
import com.sun.javatest.TestSuite;
import com.sun.javatest.util.I18NResourceBundle;
import java.io.File;
import java.io.IOException;

class TemplateParameterFilter extends ParameterFilter {
    TemplateParameterFilter() {
        super();
    }

    void update(InterviewParameters ip) {

        File jtm = null;
        if (! ip.isTemplate() && ip.getTemplatePath() != null) {
            jtm = new File(ip.getTemplatePath());
        } else if (ip.getWorkDirectory() != null) {
            String jtmF = TemplateUtilities.getTemplatePath(ip.getWorkDirectory());
            if (jtmF != null) {
                jtm = new File(jtmF);
            }
        }

        if (jtm != null) {
            TestSuite ts = ip.getTestSuite();
            try {
                InterviewParameters tip = ts.createInterview();
                tip.load(jtm);
                super.update(tip);
                templateEnabled = true;
                return;
            } catch (TestSuite.Fault ex) {
                //
            } catch (Interview.Fault ex) {
                //
            } catch (IOException ex) {
                //
            }
        } else if (ip.isTemplate()) {
            super.update(ip);
            templateEnabled = true;
            return;
        }
        templateEnabled = false;
    }


    public boolean accepts(TestDescription td) throws Fault {
        if (templateEnabled) {
            return super.accepts(td);
        } else {
            return false;
        }
    }

    public boolean accepts(TestDescription td, TestFilter.Observer o) throws TestFilter.Fault {
        if (templateEnabled) {
            return super.accepts(td, o);
        } else {
            if (o != null) {
                o.rejected(td, this);
            }
            return false;
        }
    }

    public String getName() {
        return i18n.getString("tFilter.name");
    }

    public String getDescription() {
        return i18n.getString("tFilter.desc");
    }

    public String getReason() {
        if (!templateEnabled) {
            return i18n.getString("tFilter.notAvailable");
        }
        return i18n.getString("tFilter.reason");
    }


    private boolean templateEnabled = false;
    private static I18NResourceBundle i18n = I18NResourceBundle.getBundleForClass(TemplateParameterFilter.class);

}
