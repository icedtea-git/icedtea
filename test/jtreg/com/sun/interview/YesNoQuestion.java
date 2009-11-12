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
package com.sun.interview;

import java.util.ResourceBundle;

/**
 * A {@link Question question} to which the response is yes or no.
 */
public abstract class YesNoQuestion extends ChoiceQuestion
{
    /**
     * A value for an affirmative response.
     */
    public static final String YES = "Yes";

    /**
     * A value for a negative response.
     */
    public static final String NO = "No";

    /**
     * Create a question with a nominated tag.
     * If this constructor is used, the choices must be supplied separately.
     * @param interview The interview containing this question.
     * @param tag A unique tag to identify this specific question.
     */
    protected YesNoQuestion(Interview interview, String tag) {
        this(interview, tag, null);
    }

    /**
     * Create a question with a nominated tag.
     * If this constructor is used, the choices must be supplied separately.
     * @param interview The interview containing this question.
     * @param tag A unique tag to identify this specific question.
     * @param defaultValue A default value for the question; this must be one of
     * <em>null</em>, YES, or NO.
     */
    protected YesNoQuestion(Interview interview, String tag, String defaultValue) {
        super(interview, tag);

        if (defaultValue != null && !defaultValue.equals(YES) && !defaultValue.equals(NO))
            throw new IllegalArgumentException();

        String[] choices = new String[] { null, YES, NO };
        String[] displayChoices = new String[] { null,
                                                 i18n.getString("yn.yes"),
                                                 i18n.getString("yn.no") };

        setChoices(choices, displayChoices);

        if (defaultValue != getDefaultValue()) {
            setDefaultValue(defaultValue);
            setValue(defaultValue);
        }
    }

    /**
     * Set the set of legal responses for this question. This is
     * not permitted, since the choices are fixed as YES and NO.
     * @param choices The set possible responses for this question.
     * @throws IllegalArgumentException always.
     */
    protected final void setChoices(String[] choices) {
        throw new IllegalArgumentException("Cannot set choices for YesNoQuestion");
    }

    private static final ResourceBundle i18n = Interview.i18n;
}
