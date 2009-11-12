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
import java.awt.Component;
import java.awt.Dimension;
import java.awt.FontMetrics;
import java.awt.GridBagConstraints;
import java.awt.Graphics;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionListener;
import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;
import javax.swing.JTextField;
import javax.swing.event.AncestorEvent;
import javax.swing.event.AncestorListener;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import com.sun.javatest.InterviewParameters;
import com.sun.javatest.tool.ToolDialog;
import com.sun.javatest.tool.UIFactory;

class CE_StdView extends CE_View
{
    CE_StdView(InterviewParameters config,
               UIFactory uif, ActionListener l) {
        super(config, uif, l);
        initGUI();
    }

    public Dimension getPreferredSize() {
        Insets tabInsets = tabs.getInsets();
        int w = (tabInsets == null ? 0 : tabInsets.left + tabInsets.right);
        Graphics g = tabs.getGraphics();
        if (g != null) {
            FontMetrics fm = g.getFontMetrics();
            for (int i = 0; i < tabs.getTabCount(); i++)
                w += 15 + fm.stringWidth(tabs.getTitleAt(i));
        }
        Dimension d = super.getPreferredSize();
        d.width = Math.max(d.width, w);
        return d;
    }

    public void setVisible(boolean b) {
        if (b)
            load();
        super.setVisible(b);
    }

    /**
     * Basic realisation overriden to update all panes
     */
    public void setParentToolDialog(ToolDialog d) {
        super.setParentToolDialog(d);
        for (CE_StdPane pane : panes) {
            pane.setParentToolDialog(d);
        }
    }


    void showTab(int id) {
        Class c;
        switch (id) {
        case TESTS_PANE:
            c = CE_TestsPane.class;
            break;
        case EXCLUDE_LIST_PANE:
            c = CE_ExcludeListPane.class;
            break;
        case KEYWORDS_PANE:
            c = CE_KeywordsPane.class;
            break;
        case PRIOR_STATUS_PANE:
            c = CE_PriorStatusPane.class;
            break;
        case ENVIRONMENT_PANE:
            c = CE_EnvironmentPane.class;
            break;
        case EXECUTION_PANE:
        case CONCURRENCY_PANE:
        case TIMEOUT_FACTOR_PANE:
            c = CE_ExecutionPane.class;
            break;
        default:
            throw new IllegalArgumentException();
        }

        for (int i = 0; i < tabs.getTabCount(); i++) {
            Component tab = tabs.getComponentAt(i);
            if (c.isAssignableFrom(tab.getClass())) {
                tabs.setSelectedIndex(i);
                return;
            }
        }
    }

    static final int TESTS_PANE = 0;
    static final int EXCLUDE_LIST_PANE = 1;
    static final int KEYWORDS_PANE = 2;
    static final int PRIOR_STATUS_PANE = 3;
    static final int ENVIRONMENT_PANE = 4;
    static final int EXECUTION_PANE = 5;
    static final int CONCURRENCY_PANE = 6;  // note displayed in EXECUTION_PANE
    static final int TIMEOUT_FACTOR_PANE = 7; // note displayed in EXECUTION_PANE

    boolean isOKToClose() {
        CE_StdPane currPane = (CE_StdPane) (tabs.getSelectedComponent());
        if (currPane == null)
            return true;
        return currPane.isOKToClose();
    }

    void load() {
        for (int i = 0; i < panes.length; i++)
            panes[i].load();
    }

    void save() {
        for (int i = 0; i < panes.length; i++)
            panes[i].save();
    }

    void setCheckExcludeListListener(ActionListener l) {
        for (int i = 0; i < panes.length; i++) {
            CE_StdPane pane = panes[i];
            if (pane instanceof CE_ExcludeListPane)
                ((CE_ExcludeListPane) pane).setCheckExcludeListListener(l);
        }
    }

    private void initGUI() {
        setName(STD);

        setLayout(new BorderLayout());
        initBody();
        initButtons();
    }

    private void initBody() {
        CE_KeywordsPane kp = new CE_KeywordsPane(uif, config);
        kp.setParentToolDialog(toolDialog);
        panes = new CE_StdPane[] {
            new CE_TestsPane(uif, config),
            new CE_ExcludeListPane(uif, config),
            kp,
            new CE_PriorStatusPane(uif, config),
            new CE_EnvironmentPane(uif, config),
            new CE_ExecutionPane(uif, config)
        };

        tabs = new JTabbedPane() {
                public void setSelectedIndex(int index) {
                    if (index == getSelectedIndex())
                        return;

                    CE_StdPane p = (CE_StdPane) (getSelectedComponent());
                    if (p != null && !p.isOKToClose())
                        return;

                    super.setSelectedIndex(index);
                }
            };
        tabs.setName("tabs");
        uif.setAccessibleName(tabs, "ce.tabs");
        uif.setToolTip(tabs, "ce.tabs");

        // contentious design issue here:
        // should we disable the tabs for disabled panes, or completely hide them,
        // or choose on a pane by pane basis
        for (int i = 0; i < panes.length; i++) {
            if (panes[i].isEnabled())
                uif.addTab(tabs, "ce." + panes[i].getName(), panes[i]);
        }

        tabs.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
        tabs.addChangeListener(localListener);

        add(tabs, BorderLayout.CENTER);

        addAncestorListener(localListener);
    }

    private void initButtons() {
        JPanel btnPanel = uif.createPanel("ce.std.btns",
                                    new GridBagLayout(),
                                    false);
        GridBagConstraints c = new GridBagConstraints();
        c.insets.top = 5;
        c.insets.bottom = 11;  // value from JL&F Guidelines
        c.insets.right = 5;    // value from JL&F Guidelines
        c.insets.left = 11;

        // Message Area, grow to fit
        c.fill = GridBagConstraints.HORIZONTAL;
        c.weightx = 1;
        msgField = uif.createOutputField("ce.msgs");
        msgField.setBorder(null);
        msgField.setEnabled(false);
        //completeMsg = uif.getI18NString("ce.msgs.complete");
        //incompleteMsg = uif.getI18NString("ce.msgs.incomplete");
        btnPanel.add(msgField, c);

        c.weightx = 0;
        c.insets.left = 0;
        c.insets.right = 11;    // value from JL&F Guidelines
        JButton doneBtn = uif.createButton("ce.done", listener, DONE);
        btnPanel.add(doneBtn, c);

        add(btnPanel, BorderLayout.SOUTH);
    }

    private JTabbedPane tabs;
    private CE_StdPane[] panes;
    private JTextField msgField;

    private Listener localListener = new Listener();

    private class Listener
        implements AncestorListener, ChangeListener
    {
        // ---------- from AncestorListener -----------

        public void ancestorAdded(AncestorEvent e) {
            updateCSHAndInfo();
        }

        public void ancestorMoved(AncestorEvent e) { }

        public void ancestorRemoved(AncestorEvent e) {
        }

        // ---------- from ChangeListener -----------

        public void stateChanged(ChangeEvent e) {
            updateCSHAndInfo();
        }

        // --------------------------------------------

        private void updateCSHAndInfo() {
            CE_StdPane p = (CE_StdPane) (tabs.getSelectedComponent());
        }
    };
}
