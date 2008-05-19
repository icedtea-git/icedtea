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

import javax.swing.JComponent;
import com.sun.javatest.Harness;
import com.sun.javatest.InterviewParameters;
import com.sun.javatest.WorkDirectory;
import com.sun.javatest.tool.UIFactory;

public class AccessWrapper
{
    private AccessWrapper() { }

    public static ReportHandler createReportHandler(JComponent parent, ExecModel model, Harness harness, UIFactory uif) {
        return new ReportHandler(parent, model,harness, uif);
    }

    public static ReportBrowser createReportBrowser(JComponent parent, ExecModel model, UIFactory uif, ReportHandler rh) {
        return new ReportBrowser(parent, model, uif, rh);
    }

    public static NewReportDialog createNewReportDialog(JComponent parent, UIFactory uif, FilterConfig f, Object rb, ExecModel model) {
        return new NewReportDialog(parent, uif, f, (ReportBrowser)rb, model);
    }

    public static ConfigEditor createConfigEditor(JComponent parent, InterviewParameters config, ExecModel model, UIFactory uif) {
        return new ConfigEditor(parent, config, model, uif);
    }

    public static CE_TemplateDialog createTemplateDialog(JComponent parent, InterviewParameters config, ExecModel model, UIFactory uif) {
        return new CE_TemplateDialog(parent, config, model, uif);
    }

    public static ET_FilterHandler createFilterHandler(JComponent parent, ExecModel model, UIFactory uif, Harness h) {
        ET_FilterHandler fh = new ET_FilterHandler(parent, model, h, uif, null);
        fh.loadFilters();
        return fh;
    }

    public static LogViewer createLogViewer(WorkDirectory workDir, UIFactory uif, JComponent parent) {
        return new LogViewer(workDir, uif, parent);
    }

    public static BranchPanel createBranchPanel(UIFactory uif, TreePanelModel model, Harness h, ExecModel em, JComponent parent,
                FilterSelectionHandler filterHandler, TestTreeModel ttm) {
        return new BranchPanel(uif, model, h, em, parent, filterHandler, ttm);
    }
}
