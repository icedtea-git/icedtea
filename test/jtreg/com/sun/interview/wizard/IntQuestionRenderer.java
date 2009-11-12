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

import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.text.NumberFormat;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.KeyStroke;
import com.sun.interview.IntQuestion;
import com.sun.interview.Question;

class IntQuestionRenderer
    implements QuestionRenderer
{
    public JComponent getQuestionRendererComponent(Question qq, ActionListener listener) {
        IntQuestion q = (IntQuestion) qq;

        lwb = q.getLowerBound();
        upb = q.getUpperBound();
        range = (long)upb - (long)lwb;
        suggs = q.getSuggestions();

        if (range > 10 || suggs != null)
            return createTextField(q, listener);
        else
            return createSlider(q, listener);
    }

    public String getInvalidValueMessage(Question q) {
        return null;
    }

    private JPanel createTextField(final IntQuestion q, ActionListener listener) {
        int w = 1;
        while (range >= 10) {
            range /= 10;
            w++;
        }
        if (lwb < 0)
            w++;

        String[] strSuggs;
        if (suggs == null)
            strSuggs = null;
        else {
            strSuggs = new String[suggs.length];
            for (int i = 0; i < suggs.length; i++)
                strSuggs[i] = String.valueOf(suggs[i]);
        }

        final JButton resetBtn;

        final int defVal = q.getDefaultValue();
        if (defVal == Integer.MIN_VALUE)
            resetBtn = null;
        else {
            resetBtn = new JButton(i18n.getString("int.reset.btn"));
            resetBtn.setName("int.reset.btn");
            resetBtn.setMnemonic(i18n.getString("int.reset.mne").charAt(0));
            resetBtn.setToolTipText(i18n.getString("int.reset.tip"));
        }

        final TypeInPanel p =  new TypeInPanel("int.field",
                                               q,
                                               w,
                                               strSuggs,
                                               resetBtn,
                                               listener);

        if (resetBtn != null) {
            resetBtn.addActionListener(new ActionListener() {
                    public void actionPerformed(ActionEvent e) {
                        NumberFormat fmt = NumberFormat.getNumberInstance();  // will be locale-specific
                        p.setValue(fmt.format(new Integer(defVal)));
                    }
                });
        }

        return p;

    }

    private JPanel createSlider(final IntQuestion q, ActionListener listener) {
        JPanel panel = new JPanel(new GridBagLayout());
        panel.setName("int");
        panel.setFocusable(false);

        GridBagConstraints c = new GridBagConstraints();

        JLabel label = new JLabel(i18n.getString("int.sldr.lbl"));
        label.setName("int.sldr.lbl");
        label.setDisplayedMnemonic(i18n.getString("int.sldr.mne").charAt(0));
        label.setToolTipText(i18n.getString("int.sldr.tip"));
        panel.add(label, c);

        int value = (q.isValid() ? q.getValue() : lwb);
        final JSlider slider = new JSlider(lwb, upb, value);
        slider.setName("int.sldr");
        slider.setMajorTickSpacing((upb - lwb)/2);
        slider.setMinorTickSpacing(1);
        slider.setSnapToTicks(true);
        slider.setPaintTicks(true);
        int startHint = q.getLabelStartHint();
        int incHint = q.getLabelIncrementHint();
        if (incHint != 0)
            slider.setLabelTable(slider.createStandardLabels(incHint, startHint));
        slider.setPaintLabels(true);
        //slider.registerKeyboardAction(enterListener, enterKey, JComponent.WHEN_FOCUSED);
        label.setLabelFor(slider);

        c.fill = GridBagConstraints.HORIZONTAL;
        c.weightx = 1;
        panel.add(slider, c);

        Runnable valueSaver = new Runnable() {
                public void run() {
                    q.setValue(slider.getValue());
                }
            };

        panel.putClientProperty(VALUE_SAVER, valueSaver);

        return panel;
    }

    private int lwb;
    private int upb;
    private long range;
    private int[] suggs;

    private static final KeyStroke enterKey = KeyStroke.getKeyStroke(KeyEvent.VK_ENTER, 0);
    private static final I18NResourceBundle i18n = I18NResourceBundle.getDefaultBundle();
}
