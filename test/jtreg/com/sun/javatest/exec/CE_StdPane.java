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

import java.awt.BorderLayout;
import javax.swing.BorderFactory;
import javax.swing.JPanel;

import com.sun.javatest.InterviewParameters;
import com.sun.javatest.tool.ToolDialog;
import com.sun.javatest.tool.UIFactory;

abstract class CE_StdPane extends JPanel
{
    protected CE_StdPane(UIFactory uif, InterviewParameters config, String name) {
        this.uif = uif;
        this.config = config;

        setName(name);
        setLayout(new BorderLayout());

        // We should be able to use a composite border here, but they don't
        // seem to work with titled borders. So use nested panels with separate
        // borders.

        setBorder(BorderFactory.createEmptyBorder(10, 5, 5, 5));

        main = uif.createPanel("ce.std", new BorderLayout(), false);
        String captionText = uif.getI18NString("ce." + name + ".caption");
        main.setBorder(BorderFactory.createTitledBorder(captionText));
        add(main);

        uif.setToolTip(this, "ce." + name);
    }

    protected void addBody(JPanel p) {
        p.setBorder(BorderFactory.createEmptyBorder(10, 5, 5, 5));
        main.add(p);
    }

    public void setParentToolDialog(ToolDialog d) {
        toolDialog = d;
    }

    public ToolDialog getParentToolDialog() {
        return toolDialog;
    }


    abstract boolean isOKToClose();
    abstract void load();
    abstract void save();

    protected ToolDialog toolDialog;
    protected final UIFactory uif;
    protected final InterviewParameters config;
    private JPanel main;
}
