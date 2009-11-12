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

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.TableColumn;

import com.sun.interview.ChoiceArrayQuestion;
import com.sun.interview.Question;
import java.awt.Component;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.util.HashMap;
import javax.swing.event.TableModelEvent;
import javax.swing.table.TableCellRenderer;
import javax.swing.table.TableModel;

class ChoiceArrayQuestionRenderer
        implements QuestionRenderer
{
    /**
     * Returns component for this question. Selects if it should be a pane with buttons, or
     * table, using information about available space. This information sets in
     * setPreferredPaneSize() method.
     * For each question we save its last layout and try to use it, if we cant do better.
     * We don't allow for renderer to switch from table to buttons panel and back for
     * the same question
     */
    public JComponent getQuestionRendererComponent(Question qq, ActionListener listener) {
        final ChoiceArrayQuestion q = (ChoiceArrayQuestion)qq;
            return createChoiceTable(q, listener);
    }

    public String getInvalidValueMessage(Question q) {
        return null;
    }

    private JComponent createChoiceTable(final ChoiceArrayQuestion q,
                                        final ActionListener editedListener) {
        final String[] displayChoices = q.getDisplayChoices();
        final boolean[] values = q.getValue();

        class TestTableModel extends AbstractTableModel {
                public Class getColumnClass(int c) {
                    return (c == 0 ? Boolean.class : String.class);
                }

                public int getColumnCount() {
                    return 2;
                }

                public int getRowCount() {
                    return displayChoices.length;
                }

                public Object getValueAt(int r, int c) {
                    return (c == 0 ? (Object) (new Boolean(values[r])) : displayChoices[r]);
                }

                public void setValueAt(Object o, int r, int c) {
                    if (c == 0) {
                        values[r] = ((Boolean) o).booleanValue();
                        q.setValue(values);
                        fireEditedEvent(this, editedListener);
                    }
                }

                public boolean isCellEditable(int r, int c) {
                    return (c == 0 ? true : false);
                }
            };

        final TestTableModel tm = new TestTableModel();

        final JTable tbl = new JTable(tm);
        tbl.setPreferredScrollableViewportSize(new Dimension(DOTS_PER_INCH, DOTS_PER_INCH));
        tbl.setShowHorizontalLines(false);
        tbl.setShowVerticalLines(false);
        tbl.setTableHeader(null);
        tbl.setAutoResizeMode(JTable.AUTO_RESIZE_OFF);
        tbl.setRowSelectionAllowed(false);
        tbl.setColumnSelectionAllowed(false);
        tbl.setToolTipText(i18n.getString("chcArr.tbl.tip"));

        tbl.addKeyListener(new KeyAdapter() {
           public void keyPressed(KeyEvent e) {
               if((e.getModifiersEx() & e.CTRL_DOWN_MASK) != 0 && e.getKeyCode() == e.VK_A) {
                   boolean allSelected = true;
                   for(int i = 0; i < tm.getRowCount(); i++) {
                       if(tm.getValueAt(i, 0).equals(new Boolean(false))) {
                           allSelected = false;
                           break;
                       }

                   }
                   for(int i = 0; i < tm.getRowCount(); i++) {
                       tm.setValueAt(new Boolean(!allSelected), i, 0);
                       TableModelEvent ev = new TableModelEvent(tm, i, i,
                                            TableModelEvent.ALL_COLUMNS,
                                            TableModelEvent.UPDATE);
                       tm.fireTableChanged(ev);
                   }
               }

           }
        });

        TableColumn col0 = tbl.getColumnModel().getColumn(0);
        col0.setPreferredWidth(24);
        col0.setMaxWidth(24);
        col0.setResizable(false);

        TableColumn col1 = tbl.getColumnModel().getColumn(1);
        col1.setPreferredWidth(getColumnWidth(tbl, 1) + 20);

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
//        tbl.setRowMargin(6);
//        int gapWidth = 20;
//        int gapHeight = 10;
//        tbl.setIntercellSpacing(new Dimension(gapWidth, gapHeight));
//        Dimension d = tbl.getIntercellSpacing();
//        for(int i = 0; i < tbl.getRowCount(); i++)
//            for(int j = 0; j < tbl.getColumnCount(); j++) {
//                ((JComponent)tbl.getCellRenderer(i, j)).setBorder(BorderFactory.createEmptyBorder(2, 2, 2, 2));
//
//            }

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




    private void fireEditedEvent(Object src, ActionListener l) {
        ActionEvent e = new ActionEvent(src,
                                        ActionEvent.ACTION_PERFORMED,
                                        EDITED);
        l.actionPerformed(e);
    }

    private HashMap layoutHistory = new HashMap();

    private static final I18NResourceBundle i18n = I18NResourceBundle.getDefaultBundle();
    private static final int DOTS_PER_INCH = Toolkit.getDefaultToolkit().getScreenResolution();

}
