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
package com.sun.javatest.tool;

import java.awt.Component;
import java.awt.Container;
import java.awt.Window;

import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.net.URL;
import java.text.DateFormat;
import java.util.Arrays;
import java.util.Iterator;
import java.util.ListIterator;
import java.util.TreeMap;
import java.util.Vector;

import javax.swing.JFrame;
import javax.swing.JTextField;

import com.sun.javatest.Harness;
import com.sun.javatest.ProductInfo;
import com.sun.javatest.util.ExitCount;
import com.sun.javatest.util.HelpTree;
import com.sun.javatest.util.I18NResourceBundle;
import com.sun.javatest.util.WrapWriter;

/**
 * A manager for command line help.
 */
public class HelpManager extends CommandManager
{
    /**
     * Create a HelpManager to manage the command line help
     * for a set of command managers.
     * The command managers should be set with setCommandManagers.
     */
    public HelpManager() {
    }

    /**
     * Create a HelpManager to manage the command line help
     * for a set of command managers.
     * @param commandManagers the command managers for which
     * to give command line help
     */
    public HelpManager(CommandManager[] commandManagers) {
        setCommandManagers(commandManagers);
    }

    public HelpTree.Node getHelp() {
        String[] helpOptions = {
            "help",
            "onlineHelp",
            "query1",
            "query2",
            "usage",
            "version"
        };
        return new HelpTree.Node(i18n, "help.cmd.opts", helpOptions);
    }

    /**
     * Parse a command (and any arguments it might take).
     * @param cmd the command to be parsed
     * @param argIter an iterator from which to get any arguments that
     * might be required by the option
     * @param ctx a context object to use while parsing the command
     * @return true if the command is recognized and successfully parsed,
     * and false otherwise
     */
    public boolean parseCommand(String cmd, ListIterator argIter, CommandContext ctx) {
        if (cmd.equalsIgnoreCase("help")
            || cmd.equalsIgnoreCase("usage")
            || cmd.equalsIgnoreCase("?")) {
            Vector v = new Vector();
            while (argIter.hasNext())
                v.add(((String) (argIter.next())).toLowerCase());
            commandLineHelpFlag = true;
            commandLineHelpQuery = (String[]) (v.toArray(new String[v.size()]));
            return true;
        }

        if (cmd.equalsIgnoreCase("onlineHelp") || cmd.equalsIgnoreCase("userGuide")) {
            StringBuffer sb = new StringBuffer();
            while (argIter.hasNext()) {
                if (sb.length() > 0)
                    sb.append(' ');
                sb.append(argIter.next());
            }
            onlineHelpFlag = true;
            onlineHelpQuery = sb.toString();
            return true;
        }

        if (cmd.equalsIgnoreCase("version")) {
            versionFlag = true;
            return true;
        }

        return false;
    }

    /**
     * Set the command managers for which to generate command line help.
     * @param commandManagers the command managers for which to generate command line help
     */
    public void setCommandManagers(CommandManager[] commandManagers) {
        this.commandManagers = commandManagers;
    }

    boolean isInfoRequired() {
        return (versionFlag || commandLineHelpFlag || onlineHelpFlag);
    }

    void showRequiredInfo(PrintWriter out, CommandContext ctx) {
        if (versionFlag)
            showVersion(out);

        if (commandLineHelpFlag)
            showCommandLineHelp(out);
    }

    /**
     * Print out info about the options accepted by the command line decoder.
     * @param out A stream to which to write the information.
     */
    void showCommandLineHelp(PrintWriter out) {
        HelpTree commandHelpTree = new HelpTree();

        Integer nodeIndent = Integer.getInteger("javatest.help.nodeIndent");
        if (nodeIndent != null)
            commandHelpTree.setNodeIndent(nodeIndent.intValue());

        Integer descIndent = Integer.getInteger("javatest.help.descIndent");
        if (descIndent != null)
            commandHelpTree.setDescriptionIndent(descIndent.intValue());

        // sort the command manager help nodes according to their name
        TreeMap tm = new TreeMap();
        for (int i = 0; i < commandManagers.length; i++) {
            HelpTree.Node n = commandManagers[i].getHelp();
            tm.put(n.getName(), n);
        }

        for (Iterator iter = tm.values().iterator(); iter.hasNext(); )
            commandHelpTree.addNode((HelpTree.Node) (iter.next()));

        // now add file types
        String[] fileTypes = {
            "ts",
            "wd",
            "jti"
        };
        HelpTree.Node filesNode = new HelpTree.Node(i18n, "help.cmd.files", fileTypes);
        commandHelpTree.addNode(filesNode);

        // now add syntax info
        String[] syntaxTypes = {
            "opts",
            "string",
            "atfile",
            "readfile",
            "encode"
        };
        HelpTree.Node syntaxNode = new HelpTree.Node(i18n, "help.cmd.syntax", syntaxTypes);
        commandHelpTree.addNode(syntaxNode);

        String progName =
            System.getProperty("program", "java " + Main.class.getName());

        try {
            WrapWriter ww = new WrapWriter(out);

            if (commandLineHelpQuery == null || commandLineHelpQuery.length == 0) {
                // no keywords given
                ww.write(i18n.getString("help.cmd.proto", progName));
                ww.write("\n\n");
                ww.write(i18n.getString("help.cmd.introHead"));
                ww.write('\n');
                commandHelpTree.writeSummary(ww);
            }
            else if (Arrays.asList(commandLineHelpQuery).contains("all")) {
                // -help all
                ww.write(i18n.getString("help.cmd.proto", progName));
                ww.write("\n\n");
                ww.write(i18n.getString("help.cmd.fullHead"));
                ww.write('\n');
                commandHelpTree.write(ww);
            }
            else {
                HelpTree.Selection s = commandHelpTree.find(commandLineHelpQuery);
                if (s != null)
                    commandHelpTree.write(ww, s);
                else {
                    ww.write(i18n.getString("help.cmd.noEntriesFound"));
                    ww.write("\n\n");
                    ww.write(i18n.getString("help.cmd.summaryHead"));
                    ww.write('\n');
                    commandHelpTree.writeSummary(ww);
                }
            }

            ww.write('\n');
            ww.write(i18n.getString("help.cmd.tail"));
            ww.write("\n\n");
            ww.write(i18n.getString("help.copyright.txt"));
            ww.write("\n\n");

            ww.flush();
        }
        catch (IOException e) {
            // should not happen, from PrintWriter
        }

    }

    /**
     * Show version information for JT Harness.
     * @param out the stream to which to write the information
     */
    void showVersion(PrintWriter out) {
        File classDir = Harness.getClassDir();
        String classDirPath =
            (classDir == null ? i18n.getString("help.version.unknown") : classDir.getPath());
        DateFormat df = DateFormat.getDateInstance(DateFormat.LONG);

        Object[] versionArgs = {
            /*product*/ ProductInfo.getName(),
            /*version*/ ProductInfo.getVersion(),
            /*milestone*/ ProductInfo.getMilestone(),
            /*build*/ ProductInfo.getBuildNumber(),
            /*Installed in*/ classDirPath,
            /*Running on platform version*/ System.getProperty("java.version"),
            /*from*/ System.getProperty("java.home"),
            /*Built with*/ ProductInfo.getBuildJavaVersion(),
            /*Built on*/ df.format(ProductInfo.getBuildDate())
        };

        out.println(i18n.getString("help.version.txt", versionArgs));
        out.println(i18n.getString("help.copyright.txt"));
    }

    private CommandManager[] commandManagers;
    private boolean commandLineHelpFlag;
    private String[] commandLineHelpQuery;
    private boolean onlineHelpFlag;
    private String onlineHelpQuery;
    private boolean versionFlag;

    private static I18NResourceBundle i18n = I18NResourceBundle.getBundleForClass(HelpManager.class);
}
