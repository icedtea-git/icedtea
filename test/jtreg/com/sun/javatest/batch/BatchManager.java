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
package com.sun.javatest.batch;

import java.io.File;
import java.util.ListIterator;

import com.sun.javatest.report.ReportManager;
import com.sun.javatest.tool.Command;
import com.sun.javatest.tool.CommandContext;
import com.sun.javatest.tool.CommandManager;
import com.sun.javatest.util.HelpTree;
import com.sun.javatest.util.I18NResourceBundle;

/**
 * A command manager to provide commands for batch execution of tests.
 */
public class BatchManager
    extends CommandManager
{
    static {
        RunTestsCommand.initVerboseOptions();
    }

    public HelpTree.Node getHelp() {
        HelpTree.Node[] cmdNodes = {
            getCommandHelp(BatchCommand.getName()),
            ObserverCommand.getHelp(),
            getCommandHelp(RunTestsCommand.getName())
        };
        return new HelpTree.Node(i18n, "cmgr.help", cmdNodes);

    }

    private HelpTree.Node getCommandHelp(String name) {
        return new HelpTree.Node(i18n, "cmgr.help." + name);
    }

    public boolean parseCommand(String cmd, ListIterator argIter, CommandContext ctx)
        throws Command.Fault
    {
        if (isMatch(cmd, BatchCommand.getName())) {
            ctx.addCommand(new BatchCommand());
            return true;
        }

        if (isMatch(cmd, ObserverCommand.getName())) {
            ctx.addCommand(new ObserverCommand(argIter));
            return true;
        }

        if (isMatch(cmd, RunTestsCommand.getName())) {
            ctx.addCommand(new RunTestsCommand(argIter));
            return true;
        }

        return false;
    }

    private static I18NResourceBundle i18n = I18NResourceBundle.getBundleForClass(BatchManager.class);

    //--------------------------------------------------------------------------

    static class BatchCommand
        extends Command
    {
        static String getName() {
            return "batch";
        }

        BatchCommand() {
            super(getName());
        }

        public int getDesktopMode() {
            return DESKTOP_NOT_REQUIRED_DTMODE;
        }

        public void run(CommandContext ctx) throws Fault {
            ctx.setAutoRunCommand(new AutoRunCommand());
            ctx.setCloseDesktopWhenDoneEnabled(true);
        }
    }

    static class AutoRunCommand extends RunTestsCommand
    {
        public void run(CommandContext ctx) throws Fault {
            super.run(ctx);

            File reportDir = ctx.getAutoRunReportDir();
            if (reportDir != null)
                ReportManager.writeReport(reportDir, ctx);
        }
    }
}
