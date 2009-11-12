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

import java.io.File;
import java.io.IOException;
import java.io.Writer;

import com.sun.javatest.util.HTMLWriter;
import com.sun.javatest.util.I18NResourceBundle;

/**
 * A segment of the main top level HTML report.
 */
abstract class HTMLSection {
    HTMLSection(String n, Report.Settings s, File dir, HTMLReport parent) {
        name = n;
        settings = s;
        reportDir = dir;
        this.parent = parent;
    }

    /*
    HTMLSection(String n, Parameters p) {
        name = n;
    }

    HTMLSection(String n, Parameters p, ReportModel m) {
        name = n;
        params = p;
        model = m;
    }
    */

    Writer openWriter(int code) throws IOException {
        return parent.openWriter(reportDir, code);
    }

    String getName() {
        return name;
    }

    void writeContents(ReportWriter out) throws IOException {
        out.writeLink('#' + name,  name);
    }

    void writeSummary(ReportWriter out) throws IOException {
        out.startTag(HTMLWriter.H2);
        out.writeLinkDestination(name, name);
        out.endTag(HTMLWriter.H2);
    }

    void writeExtraFiles() throws IOException {
    }

    protected ReportWriter openAuxFile(int code, String title,
                                I18NResourceBundle i18n) throws IOException {
        return new ReportWriter(openWriter(code), title, i18n);
    }


    protected String name;
    protected File reportDir;
    protected Report.Settings settings;
    protected HTMLReport parent;

    static I18NResourceBundle i18n;
}
