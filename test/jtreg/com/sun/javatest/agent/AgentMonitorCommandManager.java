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
package com.sun.javatest.agent;

import java.io.IOException;
import java.util.ListIterator;

import com.sun.javatest.tool.Command;
import com.sun.javatest.tool.CommandContext;
import com.sun.javatest.tool.CommandManager;
import com.sun.javatest.tool.Desktop;
import com.sun.javatest.util.HelpTree;
import com.sun.javatest.util.I18NResourceBundle;


/**
 * The ToolManager for {@link AgentMonitorTool agent monitor} window.
 */
public class AgentMonitorCommandManager extends CommandManager
{

    public HelpTree.Node getHelp() {
        String[] cmds = {
            AgentPoolPortCommand.getName(),
            AgentPoolTimeoutCommand.getName(),
            StartAgentPoolCommand.getName(),
            ShowMonitorCommand.getName()
        };
        return new HelpTree.Node(i18n, "cmgr.help", cmds);
    }

    //----------------------------------------------------------------------------

    public boolean parseCommand(String cmd, ListIterator argIter, CommandContext ctx)
        throws Command.Fault
    {
        if (cmd.equalsIgnoreCase(AgentPoolPortCommand.getName())) {
            ctx.addCommand(new AgentPoolPortCommand(argIter));
            return true;
        }

        if (cmd.equalsIgnoreCase(AgentPoolTimeoutCommand.getName())) {
            ctx.addCommand(new AgentPoolTimeoutCommand(argIter));
            return true;
        }

        if (cmd.equalsIgnoreCase(ShowMonitorCommand.getName())) {
            ctx.addCommand(new ShowMonitorCommand());
            return true;
        }

        if (cmd.equalsIgnoreCase(StartAgentPoolCommand.getName())) {
            ctx.addCommand(new StartAgentPoolCommand(argIter));
            return true;
        }

        return false;
    }

    private static I18NResourceBundle i18n = I18NResourceBundle.getBundleForClass(AgentMonitorCommandManager.class);

    //----------------------------------------------------------------------------

    private static class AgentPoolPortCommand extends Command {
        static String getName() {
            return "agentPoolPort";
        }

        AgentPoolPortCommand(ListIterator argIter) throws Fault {
            super(getName());

            if (!argIter.hasNext())
                throw new Fault(i18n, "cmgr.missingArg.err");

            try {
                port = Integer.parseInt(nextArg(argIter));
            }
            catch (NumberFormatException e) {
                throw new Command.Fault(i18n, "cmgr.badNumber.err");
            }
        }

        public void run(CommandContext ctx) {
            AgentManager mgr = AgentManager.access();
            ActiveAgentPool pool = mgr.getActiveAgentPool();
            pool.setPort(port);
        }

        private int port;
    }

    //----------------------------------------------------------------------------

    private static class AgentPoolTimeoutCommand extends Command {
        static String getName() {
            return "agentPoolTimeout";
        }

        AgentPoolTimeoutCommand(ListIterator argIter) throws Fault {
            super(getName());

            if (!argIter.hasNext())
                throw new Fault(i18n, "cmgr.missingArg.err");

            try {
                timeout = Integer.parseInt(nextArg(argIter));
            }
            catch (NumberFormatException e) {
                throw new Command.Fault(i18n, "cmgr.badNumber.err");
            }
        }

        public void run(CommandContext ctx) {
            AgentManager mgr = AgentManager.access();
            ActiveAgentPool pool = mgr.getActiveAgentPool();
            pool.setTimeout(timeout * 1000);
        }

        private int timeout;
    }

    //----------------------------------------------------------------------------

    private static class StartAgentPoolCommand extends Command {
        static String getName() {
            return "startAgentPool";
        }

        StartAgentPoolCommand(ListIterator argIter) throws Fault {
            super(getName());

            while (argIter.hasNext()) {
                String arg = nextArg(argIter);
                if (arg.equalsIgnoreCase("-" + AgentPoolPortCommand.getName())) {
                    portSubcommand = new AgentPoolPortCommand(argIter);
                    addArgs(portSubcommand);
                }
                else if (arg.equalsIgnoreCase("-" + AgentPoolTimeoutCommand.getName())) {
                    timeoutSubcommand = new AgentPoolTimeoutCommand(argIter);
                    addArgs(timeoutSubcommand);
                }
                else {
                    putbackArg(argIter);
                    break;
                }
            }
        }

        public void run(CommandContext ctx) throws Fault {
            if (portSubcommand != null)
                portSubcommand.run(ctx);

            if (timeoutSubcommand != null)
                timeoutSubcommand.run(ctx);

            try {
                AgentManager mgr = AgentManager.access();
                ActiveAgentPool pool = mgr.getActiveAgentPool();
                pool.setListening(true);
            }
            catch (IOException e) {
                AgentManager mgr = AgentManager.access();
                ActiveAgentPool pool = mgr.getActiveAgentPool();

                // this line does not function correctly on locales
                // other than English, it prduces garbage as the
                // thousands seperator
                //Integer p = new Integer(pool.getPort());
                // warning: line below is not really i18n compliant
                String p = Integer.toString(pool.getPort());
                throw new Fault(i18n, "cmgr.listenOn.err", new Object[] {p,e});
            }
        }

        private void addArgs(Command c) {
            String[] args = c.getArgs();
            for (int i = 1; i < args.length; i++)
                addArg(args[i]);
        }

        private AgentPoolPortCommand portSubcommand;
        private AgentPoolTimeoutCommand timeoutSubcommand;
    }

    //----------------------------------------------------------------------------

    private static class ShowMonitorCommand extends Command {
        static String getName() {
            return "monitorAgent";
        }

        ShowMonitorCommand() {
            super(getName());
        }

        public int getDesktopMode() {
            return DESKTOP_REQUIRED_DTMODE;
        }

        public void run(CommandContext ctx) {
            Desktop d = ctx.getDesktop();
            AgentMonitorToolManager tm = (AgentMonitorToolManager) (d.getToolManager(AgentMonitorToolManager.class));
            tm.startTool();
        }
    }
}
