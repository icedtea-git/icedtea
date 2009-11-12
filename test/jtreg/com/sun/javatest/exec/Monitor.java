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

import javax.swing.Icon;
import javax.swing.JComponent;
import com.sun.javatest.tool.UIFactory;

/**
 * This class provides an interface for exec tool to retrieve two
 * seperate monitoring tools.  These monitors are intended to be
 * items like clocks, progress bars and other text/graphical items.
 * Each tool can have a small version which goes on the MessageStrip
 * at the bottom of the exec tool, and a large item which can be
 * displayed in its own window or panel.
 */

abstract class Monitor {

    protected Monitor(MonitorState ms, UIFactory uif) {
        this.state = ms;
        this.uif = uif;
    }

    public abstract String getSmallMonitorName();
    public abstract Icon getSmallMonitorIcon();
    public abstract JComponent getSmallMonitor();

    public abstract String getLargeMonitorName();
    public abstract Icon getLargeMonitorIcon();
    public abstract JComponent getLargeMonitor();

    protected MonitorState state;
    protected UIFactory uif;
}
