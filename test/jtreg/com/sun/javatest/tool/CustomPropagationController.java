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
package com.sun.javatest.tool;

import com.sun.javatest.InterviewParameters;
import com.sun.javatest.InterviewPropagator;

import java.util.Properties;

/**
 * This class provides a way to extend default behaviour of tempate propagation process
 */
public class CustomPropagationController {

    /**
     * Invoked when a template propagation event occurs.
     * @param evt  - kind of event. For example EventType.Start or EventType.Finish
     * @param interview - current InterviewParameters.
     * @param templateData - loaded atcual template's values. Can be null
     */
    public void notify(EventType evt, InterviewParameters interview, Properties templateData) {}

    /**
     * Returns the question's text. This text is used in the template propagation dialog only
     * @param key - the question key
     * @param defaultText
     * @return question text
     */
    public String getQuestionText(String key, String defaultText) {
        return defaultText;
    }

    /**
     * Invoked before propagation process.
     * Provides possibility for custom preprocessing
     * of interview data based on template data
     * If the interview data or state was changed the method must return true
     * @param templateData - template data in key-value form
     * @param interview
     * @return true if the interview was changed
     */
    public boolean preprocessData(Properties templateData, InterviewParameters interview) {
        return false;
    }


    /**
     * Request that the harness reload the test suite structure from the
     * test suite.  If called on the GUI event thread, it will start a new
     * thread before executing the operation, to avoid blocking the GUI.
     * It is recommended that the caller use a different thread and probably
     * show the user a "Please wait" message until this method returns.
     * This method can be invoked between
     * EventType.Start and EventType.Finish notifications
     */
    public void refreshTests() {
        if (refresher != null && ip != null) {
            refresher.refreshTestTree(ip);
        }
    }

    public void setRefresher(InterviewPropagator.TestRefresher refresher) {
        this.refresher = refresher;
    }

    public void setInterview(InterviewParameters ip) {
        this.ip = ip;
    }


    /**
     * EventType.Start - propagation starting event
     * EventType.TemplateLoaded - external template data loaded and accessible
     * EventType.Finish - propagation finishing event
     */
    public static class EventType {
        private int code;
        private EventType(int i) {
            code = i;
        }
        public final static EventType Start = new EventType(0);
        public final static EventType TemplateLoaded = new EventType(1);
        public final static EventType Finish = new EventType(2);
    }

    private InterviewPropagator.TestRefresher refresher;
    private InterviewParameters ip;

}
