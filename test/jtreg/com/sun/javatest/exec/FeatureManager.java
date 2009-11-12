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
package com.sun.javatest.exec;


/**
 * This class represents default feature manager.
 * It can be extended to set another behaviour of featuers in JT Harness.
 * The method isEnabled may be overriden to change behaviour.
 */
public class FeatureManager {
    public FeatureManager() {
        featureToggles = new boolean[FEATURE_COUNT];
        featureToggles[TEMPLATE_USAGE] = true;
        featureToggles[TEMPLATE_CREATION] = true;
        featureToggles[SHOW_TEMPLATE_UPDATE] = true;
        featureToggles[WD_WITHOUT_TEMPLATE] = true;
        featureToggles[TEMPLATE_LOADING] = true;
    }

    /**
     * Can someone load any template they want, even if
     * WD_WITHOUT_TEMPLATE is enabled?
     */
    public static final int TEMPLATE_LOADING = 0;

    /**
     * Can templates be used?
     */
    public static final int TEMPLATE_USAGE = 1;

    /**
     * Can templates be created?
     */
    public static final int TEMPLATE_CREATION = 2;

    /**
     * Show "check for template update" menu.
     */
    public static final int SHOW_TEMPLATE_UPDATE = 3;

    /**
     * Can this test suite be opened more than once within a harness?
     * False allows any number of instances of the test suite to be opened.
     */
    public static final int SINGLE_TEST_MANAGER = 4;

    /**
     * Ability to support only work directories with templates attaached.
     */
    public static final int WD_WITHOUT_TEMPLATE = 5;

    private static int FEATURE_COUNT = 6;

    /**
     * @param feature one of TEMPLATE_USAGE, TEMPLATE_CREATION,
     *              AUTOPROPAGATE, SINGLE_TEST_MANAGER
     * @return true if this feature enabled, false otherwise
     */
    public boolean isEnabled(int feature) {
        return featureToggles[feature];
    }

    protected boolean[] featureToggles;
}
