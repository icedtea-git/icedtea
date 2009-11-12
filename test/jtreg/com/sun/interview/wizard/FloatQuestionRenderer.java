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
import java.text.NumberFormat;
import java.util.Hashtable;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSlider;
import com.sun.interview.FloatQuestion;
import com.sun.interview.Question;

class FloatQuestionRenderer
    implements QuestionRenderer
{
    public JComponent getQuestionRendererComponent(Question qq, ActionListener listener) {
        FloatQuestion q = (FloatQuestion) qq;

        lwb = q.getLowerBound();
        upb = q.getUpperBound();
        resolution = q.getResolution();
        value = q.getValue();
        range = upb - lwb;
        suggs = q.getSuggestions();

        //if (range > 100)
            return createTextField(q, listener);
        //else
            //return createSlider(q, listener);
    }

    public String getInvalidValueMessage(Question q) {
        return null;
    }

    private JPanel createTextField(final FloatQuestion q, ActionListener listener) {
        int w = 1;
        while (range >= 10) {
            range /= 10;
            w++;
        }
        if (lwb < 0)
            w++;
        // add in room for fractions
        w += 5;

        w = Math.min(w, 20);

        String[] strSuggs;
        if (suggs == null)
            strSuggs = null;
        else {
            strSuggs = new String[suggs.length];
            for (int i = 0; i < suggs.length; i++)
                strSuggs[i] = String.valueOf(suggs[i]);
        }

        final JButton resetBtn;

        final float defVal = q.getDefaultValue();
        if (Float.isNaN(defVal))
            resetBtn = null;
        else {
            resetBtn = new JButton(i18n.getString("flt.reset.btn"));
            resetBtn.setName("flt.reset.btn");
            resetBtn.setMnemonic(i18n.getString("flt.reset.mne").charAt(0));
            resetBtn.setToolTipText(i18n.getString("flt.reset.tip"));
        }

        final TypeInPanel p =  new TypeInPanel("flt.field",
                                               q,
                                               w,
                                               strSuggs,
                                               resetBtn,
                                               listener);

        if (resetBtn != null) {
            resetBtn.addActionListener(new ActionListener() {
                    public void actionPerformed(ActionEvent e) {
                        NumberFormat fmt = NumberFormat.getNumberInstance();  // will be locale-specific
                        p.setValue(fmt.format(new Double(defVal)));
                    }
                });
        }

        return p;
    }

    private JPanel createSlider(final FloatQuestion q, ActionListener listener) {
        JPanel panel = new JPanel(new GridBagLayout());
        panel.setName("flt");
        panel.setFocusable(false);

        GridBagConstraints c = new GridBagConstraints();

        JLabel label = new JLabel(i18n.getString("flt.sldr.lbl"));
        label.setName("flt.sldr.lbl");
        label.setDisplayedMnemonic(i18n.getString("flt.sldr.mne").charAt(0));
        label.setToolTipText(i18n.getString("flt.sldr.tip"));
        panel.add(label, c);

        /*
          final JSlider slider = new JSlider(lwb, upb, value);
          slider.setMajorTickSpacing((upb - lwb)/2);
          slider.setMinorTickSpacing(1);
          slider.setSnapToTicks(true);
          slider.setPaintTicks(true);
          float startHint = q.getLabelStartHint();
          float incHint = q.getLabelIncrementHint();
          if (incHint != 0)
          slider.setLabelTable(slider.createStandardLabels(incHint, startHint));
        */
        int iMax = (int)((upb - lwb) / resolution);
        int iVal = (int)((value - lwb) / resolution);
        final JSlider slider = new JSlider(0, iMax, iVal);
        slider.setName("flt");
        slider.getAccessibleContext().setAccessibleName(slider.getName());
        slider.getAccessibleContext().setAccessibleDescription(slider.getToolTipText());
        slider.setMajorTickSpacing(iMax / 2);
        slider.setMinorTickSpacing(Math.max((int)(1/resolution), 1));
        slider.setSnapToTicks(true);
        slider.setPaintTicks(true);
        float startHint = q.getLabelStartHint();
        float incHint = q.getLabelIncrementHint();
        int iStartHint = (int)((startHint - lwb)/ resolution);
        int iIncHint = (int)(incHint / resolution);
        Hashtable labels = new Hashtable();
        for (int i = iStartHint; i < iMax; i += iIncHint) {
            float f = lwb + i*resolution;
            String s;
            if (f == ((int)f))
                s = String.valueOf((int)f);
            else
                s = String.valueOf(f);
            labels.put(new Integer(i), new JLabel(s));
        }
        slider.setLabelTable(labels);
        slider.setPaintLabels(true);
        //slider.registerKeyboardAction(enterListener, enterKey, JComponent.WHEN_FOCUSED);

        c.fill = GridBagConstraints.HORIZONTAL;
        c.weightx = 1;
        panel.add(slider, c);

        Runnable valueSaver = new Runnable() {
                public void run() {
                    int i = slider.getValue();
                    q.setValue(lwb + i * resolution);
                }
            };


        panel.putClientProperty(VALUE_SAVER, valueSaver);
        return panel;
    }

    private float lwb;
    private float upb;
    private float resolution;
    private float value;
    private float range;
    private float[] suggs;


    private static final I18NResourceBundle i18n = I18NResourceBundle.getDefaultBundle();
}
