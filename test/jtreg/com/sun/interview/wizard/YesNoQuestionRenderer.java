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

import com.sun.interview.Question;
import com.sun.interview.YesNoQuestion;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.ButtonGroup;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JRadioButton;


public class YesNoQuestionRenderer implements QuestionRenderer{

    public JComponent getQuestionRendererComponent(Question qq, ActionListener listener) {
        final YesNoQuestion q = (YesNoQuestion)qq;
        final ActionListener editedListener = listener;

        String[] displayChoices = q.getDisplayChoices();
        String[] choices = q.getChoices();


        boolean allowUnset = (choices[0] == null);
        String v = q.getValue();

        ActionListener l = new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    q.setValue(e.getActionCommand());
                    fireEditedEvent(e.getSource(), editedListener);
                }
            };

        final JPanel btnPanel = new JPanel(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.anchor = GridBagConstraints.FIRST_LINE_START;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        btnPanel.setName("chc.btns");
        btnPanel.setFocusable(false);

        ButtonGroup bg = new ButtonGroup();

        int realNumber = 0;
        for (int i = (allowUnset ? 1 : 0); i < choices.length; i++) {
            String choice = choices[i];
            String displayChoice = displayChoices[i];
            JRadioButton rb = new JRadioButton(displayChoice, (v == choice));
            rb.setName("chc.btn." + choices[i]);

            if (realNumber < 10)
                rb.setMnemonic('0' + realNumber);

            rb.setToolTipText(i18n.getString("chc.btn.tip"));
            rb.getAccessibleContext().setAccessibleName(rb.getName());
            rb.getAccessibleContext().setAccessibleDescription(rb.getToolTipText());
            rb.setActionCommand(choice);
            rb.addActionListener(l);
            bg.add(rb);
            //gbc.gridx = realNumber % 2;
            gbc.gridy = realNumber;
            btnPanel.add(rb, gbc);
            realNumber++;
        }

        JPanel result = new JPanel(new GridBagLayout());
        result.setName("chc");
        result.setFocusable(false);

        JLabel label = new JLabel(i18n.getString("chc.btns.lbl"));

        GridBagConstraints c = new GridBagConstraints();
        c.anchor = GridBagConstraints.NORTHWEST;
        c.gridwidth = GridBagConstraints.REMAINDER;
        c.weightx = 1;
        result.add(label, c);

        c.insets.top = 10;
        c.anchor = GridBagConstraints.NORTH;

        result.add(btnPanel, c);

        return result;
    }

    public String getInvalidValueMessage(Question q) {
        return null;
    }

    private void fireEditedEvent(Object src, ActionListener l) {
        ActionEvent e = new ActionEvent(src,
                                        ActionEvent.ACTION_PERFORMED,
                                        EDITED);
        l.actionPerformed(e);
    }

    private static final I18NResourceBundle i18n = I18NResourceBundle.getDefaultBundle();

}
