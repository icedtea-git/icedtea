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

import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.util.Map;
import javax.swing.Box;
import javax.swing.ButtonGroup;
import javax.swing.JCheckBox;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JTextArea;

import com.sun.javatest.tool.Preferences;
import com.sun.javatest.tool.UIFactory;

class PrefsPane extends Preferences.Pane
{
    PrefsPane() {
        uif = new UIFactory(this);
        initGUI();
    }

    public Preferences.Pane[] getChildPanes() {
        if (configEditorPane == null)
            configEditorPane = new ConfigEditorPane();
        if (childPanes == null)
            childPanes = new Preferences.Pane[] { configEditorPane };
        return childPanes;
    }

    public String getText() {
        return uif.getI18NString("ep.title");
    }

    public void load(Map m) {
        super.load(m);
        String p = (String) (m.get(ExecTool.TOOLBAR_PREF));
        toolBarChk.setSelected(p == null || p.equals("true"));
        p = (String) (m.get(ExecTool.FILTER_WARN_PREF));
        filterWarnChk.setSelected(p == null || p.equals("true"));
    }

    public void save(Map m) {
        super.save(m);
        m.put(ExecTool.TOOLBAR_PREF, String.valueOf(toolBarChk.isSelected()));
        m.put(ExecTool.FILTER_WARN_PREF, String.valueOf(filterWarnChk.isSelected()));
    }

    private void initGUI() {
        setLayout(new GridBagLayout());

        GridBagConstraints c = new GridBagConstraints();
        c.fill = GridBagConstraints.HORIZONTAL;
        c.gridwidth = GridBagConstraints.REMAINDER;
        c.weightx = 1;

        add(createToolBarPanel(), c);
        add(createFilterPanel(), c);

        c.weighty = 1;
        add(Box.createVerticalGlue(), c);

    }

    private JPanel createToolBarPanel() {
        JPanel p = uif.createPanel("exec.prefs", new GridBagLayout(), false);
        GridBagConstraints c = new GridBagConstraints();
        c.anchor = GridBagConstraints.WEST;
        c.weightx = 1;
        p.setBorder(uif.createTitledBorder("ep.toolbar"));
        toolBarChk = uif.createCheckBox("ep.toolbar", true);
        // override default a11y name
        uif.setAccessibleName(toolBarChk, "ep.toolbar");
        p.add(toolBarChk, c);
        return p;
    }

    private JPanel createFilterPanel() {
        // could have setting to ask user what the default filter to use is
        JPanel p = uif.createPanel("exec.prefs.filter", new GridBagLayout(), false);
        GridBagConstraints c = new GridBagConstraints();
        c.anchor = GridBagConstraints.WEST;
        c.weightx = 1;
        p.setBorder(uif.createTitledBorder("ep.filt"));
        filterWarnChk= uif.createCheckBox("ep.filt", true);
        // override default a11y name
        uif.setAccessibleName(filterWarnChk, "ep.filt");
        p.add(filterWarnChk, c);
        return p;
    }

    private UIFactory uif;
    private JCheckBox toolBarChk;
    private JCheckBox filterWarnChk;
    private ConfigEditorPane configEditorPane;
    private Preferences.Pane[] childPanes;

    private class ConfigEditorPane extends Preferences.Pane {
        ConfigEditorPane() {
            initGUI();
        }

        public String getText() {
            return uif.getI18NString("ep.ce.title");
        }

        public void load(Map m) {
            String vp = (String) (m.get(ConfigEditor.VIEW_PREF));
            if (vp != null && vp.equals(CE_View.STD))
                stdBtn.setSelected(true);
            else
                fullBtn.setSelected(true);
            String mp = (String) (m.get(ConfigEditor.MORE_INFO_PREF));
            moreInfoChk.setSelected(mp == null || mp.equals("true"));
        }

        public void save(Map m) {
            m.put(ConfigEditor.VIEW_PREF, (stdBtn.isSelected() ? CE_View.STD : CE_View.FULL));
            m.put(ConfigEditor.MORE_INFO_PREF, String.valueOf(moreInfoChk.isSelected()));
        }

        private void initGUI() {
            setLayout(new GridBagLayout());

            GridBagConstraints c = new GridBagConstraints();
            c.fill = GridBagConstraints.HORIZONTAL;
            c.gridwidth = GridBagConstraints.REMAINDER;
            c.weightx = 1;

            add(createDefaultViewPanel(), c);

            c.weighty = 1;
            add(Box.createVerticalGlue(), c);
        }

        private JPanel createDefaultViewPanel() {
            GridBagConstraints c = new GridBagConstraints();
            c.anchor = GridBagConstraints.WEST;
            c.gridwidth = GridBagConstraints.REMAINDER;
            c.insets.left = 10;
            c.weightx = 1;
            c.weighty = 0;

            JPanel p = new JPanel(new GridBagLayout());

            JTextArea infoTa = uif.createMessageArea("ep.ce.info");
            infoTa.setOpaque(false);
            add(infoTa, c);

            p.setBorder(uif.createTitledBorder("ep.ce.defView"));
            ButtonGroup grp = new ButtonGroup();
            fullBtn = uif.createRadioButton("ep.ce.defView.full", grp);
            p.add(fullBtn, c);
            stdBtn = uif.createRadioButton("ep.ce.defView.std", grp);
            p.add(stdBtn, c);
            moreInfoChk = uif.createCheckBox("ep.ce.moreInfo", true);
            // override default a11y name
            uif.setAccessibleName(moreInfoChk, "ep.ce.moreInfo");

            c.insets.top = 10;
            p.add(moreInfoChk, c);
            return p;
        }

        private JRadioButton fullBtn;
        private JRadioButton stdBtn;
        private JCheckBox moreInfoChk;
    }
}
