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
package com.sun.javatest.audit;

import java.awt.event.ActionEvent;
import java.util.Map;
import javax.swing.Action;

import com.sun.javatest.tool.Desktop;
import com.sun.javatest.tool.Tool;
import com.sun.javatest.tool.ToolAction;
import com.sun.javatest.tool.ToolManager;


/**
 * The ToolManager for {@link AuditTool audit tool} window.
 */
public class AuditToolManager extends ToolManager
{
    public AuditToolManager(Desktop desktop) {
        super(desktop);
    }

    //----------------------------------------------------------------------------

    public Action[] getWindowOpenMenuActions() {
        Action a = new ToolAction(i18n, "tmgr.openAudit") {
            public void actionPerformed(ActionEvent e) {
                startTool();
            }
        };
        return new Action[] { a };
    }

    //----------------------------------------------------------------------------

    /**
     * Start the {@link AuditTool audit tool} window.
     */
    public Tool startTool() {
        AuditTool t = getTool();

        Desktop d = getDesktop();
        if (!d.containsTool(t))
            d.addTool(t);

        d.setSelectedTool(t);

        return t;
    }

    public Tool restoreTool(Map m) {
        AuditTool t = getTool();
        t.restore(m);
        return t;
    }

    //----------------------------------------------------------------------------


    AuditTool getTool() {
        if (tool == null) {
            tool = new AuditTool(this);
            tool.addObserver(new Tool.Observer() {
                    public void shortTitleChanged(Tool t, String newValue) { }

                    public void titleChanged(Tool t, String newValue) { }

                    public void toolDisposed(Tool t) {
                        if (t == tool)
                            tool = null;
                    }
                });
        }

        return tool;
    }

    private AuditTool tool;
}
