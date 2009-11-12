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
import java.awt.event.ActionListener;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JPanel;
import com.sun.interview.StringListQuestion;
import com.sun.interview.Question;

class StringListQuestionRenderer
    implements QuestionRenderer
{
    public JComponent getQuestionRendererComponent(Question qq, ActionListener listener) {
        final StringListQuestion q = (StringListQuestion) qq;

        JPanel panel = new JPanel(new BorderLayout());
        panel.setName("slst");
        panel.setFocusable(false);

        JLabel label = new JLabel(i18n.getString("slst.lbl"));
        label.setName("slst.lbl");
        label.setDisplayedMnemonic(i18n.getString("slst.mne").charAt(0));
        label.setToolTipText(i18n.getString("slst.tip"));
        panel.add(label, BorderLayout.NORTH);

        final EditableList list = new EditableList("slst", q.getValue());
        list.setDuplicatesAllowed(q.isDuplicatesAllowed());
        list.addListDataListener(new ActionListDataListener(panel,
                                                            listener,
                                                            QuestionRenderer.EDITED));
        label.setLabelFor(list);

        panel.add(list, BorderLayout.CENTER);

        Runnable valueSaver = new Runnable() {
            public void run() {
                q.setValue((String[]) list.getItems(String.class));
            }
        };

        panel.putClientProperty(VALUE_SAVER, valueSaver);

        return panel;
    }

    public String getInvalidValueMessage(Question q) {
        return null;
    }

    private static final I18NResourceBundle i18n = I18NResourceBundle.getDefaultBundle();
}
