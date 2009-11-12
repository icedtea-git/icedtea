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
package com.sun.interview.wizard;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.KeyboardFocusManager;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.HashMap;

import javax.accessibility.AccessibleContext;

import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTable;
import javax.swing.JTextArea;
import javax.swing.table.TableCellRenderer;
import javax.swing.table.DefaultTableModel;
import javax.swing.table.TableColumn;
import javax.swing.table.TableModel;

import com.sun.interview.PropertiesQuestion;
import com.sun.interview.Question;
import java.util.Iterator;
import java.util.Set;
import javax.swing.CellEditor;
import javax.swing.event.AncestorEvent;
import javax.swing.event.AncestorListener;


class PropertiesQuestionRenderer implements QuestionRenderer {
    public JComponent getQuestionRendererComponent(Question qq, ActionListener listener) {
        question = (PropertiesQuestion)qq;

        tables = new HashMap();

        JPanel panel = new JPanel(new GridBagLayout());
        panel.setName("properties");
        panel.setFocusable(false);

        if (question.getValue() == null) {
            showEmptyQuestion(panel);
            return panel;
        }

        String[] headers = {question.getKeyHeaderName(),
                            question.getValueHeaderName()};


        // add table(s)
        addGroup(null, panel, question, headers, listener);

        // note that empty groups are not returned by the next call
        String[] groups = question.getGroups();
        if (groups != null)
            for (int i = 0; i < groups.length; i++) {
                addGroup(groups[i], panel, question, headers, listener);
            }   // for

        if (panel.getComponentCount() == 0) {
            showEmptyQuestion(panel);
        }

        valueSaver = new Runnable() {
                public void run() {
                    Set keys = tables.keySet();
                    Iterator iter = keys.iterator();
                    while(iter.hasNext()) {
                        JTable table = (JTable)tables.get((String)iter.next());
                        CellEditor editor = table.getCellEditor();
                        if(editor != null) {
                            editor.stopCellEditing();
                        }
                    }
                }
            };

        panel.putClientProperty(VALUE_SAVER, valueSaver);


        // This inserted to handle programmaticaly fired events
        // when user click 'X' button in ConfigEditor
        panel.addAncestorListener(new AncestorListener() {
            public void ancestorAdded(AncestorEvent e) {

            }
            public void ancestorMoved(AncestorEvent e) {

            }
            public void ancestorRemoved(AncestorEvent e) {
                if (valueSaver != null) {
                    valueSaver.run();
                }
            }
        });


        return panel;
    }

    public String getInvalidValueMessage(Question q) {
        return null;
    }

    private void showEmptyQuestion(JPanel panel) {
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.anchor = GridBagConstraints.PAGE_START;
        gbc.fill = GridBagConstraints.BOTH;
        gbc.weightx = 2.0;

        // component to hold the message, with lots of 508 adjustments
        JTextArea txt = new JTextArea(i18n.getString("props.empty.txt"));
        txt.setOpaque(false);
        txt.setEditable(false);
        txt.setLineWrap(true);
        txt.setWrapStyleWord(true);
        txt.setFocusTraversalKeys(KeyboardFocusManager.FORWARD_TRAVERSAL_KEYS, null);
        txt.setFocusTraversalKeys(KeyboardFocusManager.BACKWARD_TRAVERSAL_KEYS, null);
        AccessibleContext ac = txt.getAccessibleContext();
        ac.setAccessibleName(i18n.getString("props.message.name"));
        ac.setAccessibleDescription(i18n.getString("props.message.desc"));

        panel.add(txt, gbc);
    }

    // TABLE BUILDING
    /**
     * @param headers Just an optimzation right now.
     */
    private void addGroup(String group, JPanel panel, PropertiesQuestion q,
                          String[] headers, ActionListener listener) {
        TableModel model = new PropTableModel(headers, group, q, listener);
        // don't show empty tables
        if (model.getRowCount() == 0)
            return;

        GridBagConstraints c = new GridBagConstraints();
        c.gridx = 0;
        c.gridy = tables.size() * 4;
        c.anchor = GridBagConstraints.PAGE_START;
        c.weightx = 1.0;
        c.fill = GridBagConstraints.BOTH;

        if (c.gridy > 0) {
            Component box = Box.createVerticalStrut(40);
            box.setFocusable(false);
            c.weighty = 1.0;
            panel.add(box, c);
            c.weighty = 0.0;
        }

        // null group is for ungrouped properties
        if (group != null) {
            JLabel label = new JLabel(q.getGroupDisplayName(group));
            label.setName(q.getGroupDisplayName(group));
            //label.setDisplayedMnemonic(i18n.getString("int.sldr.mne").charAt(0));
            //label.setToolTipText(i18n.getString("int.sldr.tip"));
            panel.add(label, c);
        }
        /*
        else {
            Component box = Box.createVerticalStrut(1);
            box.setFocusable(false);
            panel.add(box, c);
        }
        */

        if (renderer == null)
            renderer = new RenderingUtilities.PropCellRenderer(q);

        JTable table = new PropJTable(model);
        table.setBorder(BorderFactory.createEtchedBorder());
        table.setRowSelectionAllowed(false);
        table.setColumnSelectionAllowed(false);
        table.getTableHeader().setReorderingAllowed(false);

        // setup key column
        TableColumn tc = table.getColumnModel().getColumn(0);
        tc.setCellRenderer(renderer);
        tc.setResizable(true);

        // setup value column
        tc = table.getColumnModel().getColumn(1);
        tc.setCellEditor(
            new RenderingUtilities.PCE(question));
        tc.setCellRenderer(renderer);
        tc.setResizable(true);

        c.gridy++;
        //panel.add(new JScrollPane(table), c);
        panel.add(table.getTableHeader(), c);

        c.gridy++;
        panel.add(table, c);

        tables.put(group, table);
    }

    // UTILITY

    private void fireEditedEvent(Object src, ActionListener l) {
        ActionEvent e = new ActionEvent(src,
                                        ActionEvent.ACTION_PERFORMED,
                                        EDITED);
        l.actionPerformed(e);
    }

    private class PropJTable extends JTable {
        PropJTable(TableModel model) {
            super(model);
            setIntercellSpacing(new Dimension(4,4));
            setRowHeight((int)(getRowHeight() * 1.5));
        }

        public boolean isCellEditable(int row, int column) {
            if (column == 0)
                return false;

            if ( question.isReadOnlyValue((String)(getValueAt(row, 0))) )
                return false;

            return true;
        }
    }

    private class PropTableModel extends DefaultTableModel {
        PropTableModel(String[] headers, String group, PropertiesQuestion q,
                       ActionListener listener) {
            super();
            this.q = q;
            editedListener = listener;

            setColumnCount(2);

            String[][] d = q.getGroup(group);

            if (d != null) {
                ArrayList rm = null;
                for (int i = 0; i < d.length; i++) {
                    if (!q.isEntryVisible(d[i][0])) {
                        if (rm == null)
                            rm = new ArrayList();
                        else { }
                        rm.add(d[i][0]);
                    }
                    else {
                        // this entry is visible
                    }
                }   // for

                // remove items from d
                if (rm != null) {
                    String[][] d2 = new String[d.length-rm.size()][2];
                    int pos = 0;
                    for (int i = 0; i < d.length; i++) {
                        if (rm.contains(d[i][0]))
                            continue;
                        else {
                            d2[pos][0] = d[i][0];
                            d2[pos][1] = d[i][1];
                            pos++;
                        }
                    // assert: pos == d2.length
                    }   // loop should fill d2.length!

                    d = d2;
                }
                setDataVector(d, headers);
            }

            /* old code which doesn't support invisibility
            if (d != null)
                setDataVector(d, headers);
            */
        }

            /*
         String getColumnName(int column) {
             if (column > headers.length - 1)
                 return super.getColumnName();
             else
                 return headers[column];
         }

        String[] headers;
         */

        public void setValueAt(Object o, int row, int col) {
            super.setValueAt(o, row, col);

            if (col == 1) {
                String key = (String)(getValueAt(row, 0));
                q.updateProperty(key, (String)o);
                fireEditedEvent(this, editedListener);
                fireTableCellUpdated(row, 0);
                fireTableCellUpdated(row, 1);
            }
        }

        private PropertiesQuestion q;
        private ActionListener editedListener;
    }

    private Runnable valueSaver;
    private HashMap tables;
    private TableCellRenderer renderer;
    private PropertiesQuestion question;

    private static final I18NResourceBundle i18n = I18NResourceBundle.getDefaultBundle();
}
