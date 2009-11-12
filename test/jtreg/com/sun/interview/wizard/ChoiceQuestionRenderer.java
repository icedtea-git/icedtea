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

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JPanel;
import java.awt.Dimension;

import com.sun.interview.ChoiceQuestion;
import com.sun.interview.Question;
import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Toolkit;
import java.util.HashMap;
import javax.swing.AbstractCellEditor;
import javax.swing.ButtonGroup;
import javax.swing.CellEditor;
import javax.swing.JRadioButton;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.TableCellEditor;
import javax.swing.table.TableCellRenderer;
import javax.swing.table.TableColumn;
import javax.swing.table.TableModel;

class ChoiceQuestionRenderer
        implements QuestionRenderer
{
    /**
     * Returns component for this question. Selects if it should be a pane with buttons, or
     * drop-down list, using information about available space. This information sets in
     * setPreferredPaneSize() method.
     * For each question we save its last layout and try to use it, if we cant do better.
     * We don't allow for renderer to switch from list to buttons panel and back for
     * the same question
     */
    public JComponent getQuestionRendererComponent(Question qq, ActionListener listener) {
        final ChoiceQuestion q = (ChoiceQuestion)qq;
        return createChoiceTable(q, listener);
    }

    public String getInvalidValueMessage(Question q) {
        return null;
    }


    private JComponent createChoiceTable(final ChoiceQuestion q,
                                        final ActionListener editedListener) {
        final String[] displayChoices = q.getDisplayChoices();
        final String[] values = q.getChoices();
        final int starts_from = values[0] == null ? 1 : 0;

        class TestTableModel extends AbstractTableModel {
            public Class getColumnClass(int c) {
                return String.class;
            }

            public int getColumnCount() {
                return 1;
            }

            public int getRowCount() {
                return displayChoices.length - starts_from;
            }

            public Object getValueAt(int r, int c) {
                return values[r + starts_from];
            }

            public void setValueAt(Object o, int r, int c) {
                if (c == 0) {
                    q.setValue(values[r + starts_from]);
                    fireEditedEvent(this, editedListener);
                }
            }

            public boolean isCellEditable(int r, int c) {
                return true;
            }
        };

        final TestTableModel tm = new TestTableModel();
        final JTable tbl = new JTable(tm);


        final JRadioButton[] rb = new JRadioButton[displayChoices.length - starts_from];
        final ButtonGroup bg = new ButtonGroup();

        String v = q.getValue();
        for(int i = 0; i < rb.length; i++) {

            rb[i] = new JRadioButton(displayChoices[i + starts_from],
                                                (values[i + starts_from] == v));
            rb[i].setActionCommand(values[i + starts_from]);

            rb[i].setName("chc.btn." + values[i + starts_from]);
            if (i < 10)
                rb[i].setMnemonic('0' + i);

            rb[i].setToolTipText(i18n.getString("chc.btn.tip"));
            rb[i].getAccessibleContext().setAccessibleName(rb[i].getName());
            rb[i].getAccessibleContext().setAccessibleDescription(rb[i].getToolTipText());

            rb[i].setBackground(tbl.getBackground());

            rb[i].setFocusPainted(false);
            bg.add(rb[i]);

            rb[i].addActionListener(new ActionListener() {
               public void actionPerformed(ActionEvent e) {
                   CellEditor editor = tbl.getCellEditor();
                   if (editor != null) {
                       editor.stopCellEditing();
                   }
               }
            });
        }





        class TestTableRenderer implements TableCellRenderer {

            public Component getTableCellRendererComponent(JTable table, Object value,
                    boolean isSelected, boolean hasFocus, int row, int column) {

                return rb[row];
            }
        };

        final TestTableRenderer r = new TestTableRenderer();

        class TestTableEditor extends AbstractCellEditor implements TableCellEditor {
            public Object getCellEditorValue() {
                return null;
            }
            public Component getTableCellEditorComponent(JTable table, Object value,
                    boolean isSelected, int row, int column) {

                rb[row].setSelected(true);
                return rb[row];
            }
        };

        tbl.setPreferredScrollableViewportSize(new Dimension(DOTS_PER_INCH, DOTS_PER_INCH));
        tbl.setShowHorizontalLines(false);
        tbl.setShowVerticalLines(false);
        tbl.setTableHeader(null);

        tbl.setAutoResizeMode(JTable.AUTO_RESIZE_OFF);
        tbl.setRowSelectionAllowed(false);
        tbl.setColumnSelectionAllowed(false);
        tbl.setToolTipText(i18n.getString("chcArr.tbl.tip"));

        TableColumn col0 = tbl.getColumnModel().getColumn(0);
        col0.setCellRenderer(r);
        col0.setCellEditor(new TestTableEditor());

        col0.setPreferredWidth(getColumnWidth(tbl, 0) + 20);

        final JScrollPane sp = new JScrollPane(tbl);
        sp.setName("chcArr.sp");
        sp.getViewport().setBackground(tbl.getBackground());


        JLabel lbl = new JLabel(i18n.getString("chcArr.tbl.lbl"));
        lbl.setName("chcArr.tbl.lbl");
        lbl.setDisplayedMnemonic(i18n.getString("chcArr.tbl.mne").charAt(0));
        lbl.setToolTipText(i18n.getString("chcArr.tbl.tip"));
        lbl.setLabelFor(sp);

        JPanel result = new JPanel(new BorderLayout());

        tbl.setRowHeight(22);


        result.add(lbl, BorderLayout.NORTH);
        result.add(sp, BorderLayout.CENTER);

        return result;
    }


    private int getColumnWidth(JTable table, int colIndex) {
        int width = -1;

        TableModel model = table.getModel();
        int rowCount = model.getRowCount();
        TableColumn col = table.getColumnModel().getColumn(colIndex);

        for(int i = 0; i < rowCount; i++) {
            TableCellRenderer r = table.getCellRenderer(i, colIndex);
            Component c = r.getTableCellRendererComponent(table,
                model.getValueAt(i, colIndex),
                false, false, i, colIndex);
            width = Math.max(width, c.getPreferredSize().width);
        }

        return width;
    }

    private static final int DOTS_PER_INCH = Toolkit.getDefaultToolkit().getScreenResolution();

    private void fireEditedEvent(Object src, ActionListener l) {
        ActionEvent e = new ActionEvent(src,
                                        ActionEvent.ACTION_PERFORMED,
                                        EDITED);
        l.actionPerformed(e);
    }

    private JPanel btnPanel;

    private static final I18NResourceBundle i18n = I18NResourceBundle.getDefaultBundle();

    private HashMap layoutHistory = new HashMap();

}
