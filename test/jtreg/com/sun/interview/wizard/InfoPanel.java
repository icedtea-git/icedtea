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
import javax.accessibility.AccessibleContext;
import javax.swing.JComponent;
import javax.swing.event.AncestorEvent;
import javax.swing.event.AncestorListener;

import com.sun.interview.ErrorQuestion;
import com.sun.interview.Interview;
import com.sun.interview.Question;

class InfoPanel extends JComponent
{
    public InfoPanel(Interview interview) {
        this.interview = interview;
        setName("info");
        setLayout(new BorderLayout());
        addAncestorListener(new Listener());
    }

    public Dimension getPreferredSize() {
        Toolkit tk = Toolkit.getDefaultToolkit();
        Dimension d = new Dimension(PREFERRED_WIDTH*tk.getScreenResolution(),
                                    PREFERRED_HEIGHT*tk.getScreenResolution());
        return d;
    }

    private Interview interview;
    private Listener listener = new Listener();

    private static final I18NResourceBundle i18n = I18NResourceBundle.getDefaultBundle();

    private static final int PREFERRED_WIDTH = 4; // inches
    private static final int PREFERRED_HEIGHT = 3; // inches

    private class Listener implements AncestorListener, Interview.Observer
    {
        // ---------- from AncestorListener -----------

        public void ancestorAdded(AncestorEvent e) {
            interview.addObserver(this);
            currentQuestionChanged(interview.getCurrentQuestion());
        }

        public void ancestorMoved(AncestorEvent e) { }

        public void ancestorRemoved(AncestorEvent e) {
            interview.removeObserver(this);
        }

        //----- from Interview.Observer -----------

        public void pathUpdated() { }

        public void currentQuestionChanged(Question q) {
            if (!(q instanceof ErrorQuestion))
                ;
        }
    }
}
