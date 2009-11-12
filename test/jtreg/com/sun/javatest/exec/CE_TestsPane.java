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
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashSet;
import java.util.Vector;
import java.util.Set;
import javax.swing.ButtonGroup;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import com.sun.javatest.InterviewParameters;
import com.sun.javatest.TestResultTable;
import com.sun.javatest.WorkDirectory;
import com.sun.javatest.Parameters.TestsParameters;
import com.sun.javatest.Parameters.MutableTestsParameters;
import com.sun.javatest.tool.FileChooser;
import com.sun.javatest.tool.TestTreeSelectionPane;
import com.sun.javatest.tool.UIFactory;

/**
 * Standard values view, initial tests selection panel.
 */
class CE_TestsPane extends CE_StdPane
{
    CE_TestsPane(UIFactory uif, InterviewParameters config) {
        super(uif, config, "tests");

        updateConfig();
        initGUI();
    }

    boolean isOKToClose() {
        if (mutableTestsParameters == null)
            return true;

        if (selectTestsBtn.isSelected() && testsField.isSelectionEmpty()) {
            uif.showError("ce.tests.noTests");
            return false;
        }

        return true;
    }

    void updateConfig() {
        testsParameters = config.getTestsParameters();
        if (testsParameters instanceof MutableTestsParameters)
            mutableTestsParameters = ((MutableTestsParameters) (testsParameters));
        else
            mutableTestsParameters = null;
    }

    void load() {
        updateConfig();

        if (mutableTestsParameters != null) {
            int tm = mutableTestsParameters.getTestsMode();
            if (tm == MutableTestsParameters.ALL_TESTS)
                allTestsBtn.setSelected(true);
            else
                selectTestsBtn.setSelected(true);

            testsField.setSelection(mutableTestsParameters.getSpecifiedTests());
            testsField.setEnabled(selectTestsBtn.isSelected());
        }
        else {
            mutableTestsParameters = null;

            String[] tests = testsParameters.getTests();
            if (tests == null || tests.length == 0) {
                allTestsBtn.setSelected(true);
                testsField.clear();
            }
            else {
                selectTestsBtn.setSelected(true);
                testsField.setSelection(tests);
            }
            allTestsBtn.setEnabled(false);
            selectTestsBtn.setEnabled(false);
            testsField.setEnabled(false);
        }
    }

    void save() {
        if (mutableTestsParameters != null) {
            if (allTestsBtn.isSelected())
                mutableTestsParameters.setTestsMode(MutableTestsParameters.ALL_TESTS);
            else if (selectTestsBtn.isSelected())
                mutableTestsParameters.setTestsMode(MutableTestsParameters.SPECIFIED_TESTS);
            mutableTestsParameters.setSpecifiedTests(testsField.getSelection());
        }
    }

    private void initGUI() {
        JPanel p = uif.createPanel("ce.tests", new GridBagLayout(), false);

        GridBagConstraints c = new GridBagConstraints();
        c.gridwidth = GridBagConstraints.REMAINDER;
        c.fill = GridBagConstraints.HORIZONTAL;
        c.weightx = 1;

        btnGrp = new ButtonGroup();
        allTestsBtn = uif.createRadioButton("ce.tests.all", btnGrp);
        p.add(allTestsBtn, c);


        selectTestsBtn = uif.createRadioButton("ce.tests.select", btnGrp);
        selectTestsBtn.addChangeListener(new ChangeListener() {
            public void stateChanged(ChangeEvent e) {
                boolean s = selectTestsBtn.isSelected();
                testsField.setEnabled(s);
                loadBtn.setEnabled(s);
            }
        });
        c.gridheight = 2;
        c.gridwidth = 1;
        c.weightx = 0;
        p.add(selectTestsBtn, c);

        testsField = new TestTreeSelectionPane(config.getWorkDirectory().getTestResultTable());
        testsField.setEnabled(selectTestsBtn.isSelected());
        c.fill = GridBagConstraints.BOTH;
        c.gridheight = 1;
        c.gridwidth = GridBagConstraints.REMAINDER;
        c.weightx = 1;
        c.weighty = 1;
        p.add(testsField, c);

        loadBtn = uif.createButton("ce.tests.load", new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    selectFromFile();
                }
            });
        loadBtn.setEnabled(selectTestsBtn.isSelected());
        c.anchor = GridBagConstraints.EAST;
        c.fill = GridBagConstraints.NONE;
        c.insets.top = 5;
        c.weighty = 1;
        c.weighty = 0;
        p.add(loadBtn, c);

        addBody(p);
    }

    private void selectFromFile() {
        if (chooser == null) {
            chooser = new FileChooser();
        }

        int rc = chooser.showDialog(this, chooser.getApproveButtonText());
        if (rc != JFileChooser.APPROVE_OPTION)
            return;

        WorkDirectory wd = config.getWorkDirectory();
        TestResultTable trt = wd.getTestResultTable();

        File file = chooser.getSelectedFile();
        Vector paths = new Vector();
        Vector badPaths = new Vector();
        Set seen = new HashSet();
        try {
            BufferedReader in = new BufferedReader(new FileReader(file));
            String line;
            while ((line = in.readLine()) != null) {
                line = line.trim();
                if (line.length() == 0 || line.startsWith("#"))
                    continue;
                int sp = line.indexOf(' ');
                String path = (sp == -1 ? line : line.substring(0, sp));
                if (!seen.contains(path)) {
                    if (trt.validatePath(path))
                        paths.add(path);
                    else
                        badPaths.add(path);
                    seen.add(path);
                }
            }
            in.close();
        }
        catch (FileNotFoundException e) {
            uif.showError("ce.tests.cantFindFile", file.toString());
            return;
        }
        catch (IOException e) {
            uif.showError("ce.tests.cantReadFile", new Object[] { file, e.toString() });
            return;
        }

        final int MAX_BAD_PATHS = 10;

        if (badPaths.size() > 0) {
            if (badPaths.size() == 1)
                uif.showError("ce.tests.badPath", badPaths.get(0));
            else {
                StringBuffer sb = new StringBuffer();
                for (int i = 0; i < Math.min(badPaths.size(), MAX_BAD_PATHS); i++) {
                    if (sb.length() > 0)
                        sb.append('\n');
                    sb.append(badPaths.get(i));
                }
                boolean more = badPaths.size() > MAX_BAD_PATHS;
                uif.showError("ce.tests.badPaths",
                              new Object[] { sb.toString(), new Integer(more ? 1 : 0) });
            }
        }

        testsField.setSelection((String[]) (paths.toArray(new String[paths.size()])));
    }

    private TestsParameters testsParameters;
    private MutableTestsParameters mutableTestsParameters;
    private ButtonGroup btnGrp;
    private JRadioButton allTestsBtn;
    private JRadioButton selectTestsBtn;
    private TestTreeSelectionPane testsField;
    private JButton loadBtn;
    private FileChooser chooser;
}
