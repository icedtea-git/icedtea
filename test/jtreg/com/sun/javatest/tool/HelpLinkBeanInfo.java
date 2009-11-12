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

import java.beans.SimpleBeanInfo;
import java.beans.PropertyDescriptor;
import com.sun.javatest.JavaTestError;

/**
 * Bean info for {@link HelpLink}.
 */
public class HelpLinkBeanInfo extends SimpleBeanInfo
{
    /**
     * Get property descriptors for properties of HelpLink objects.
     * Four properties are defined: target, text, textFont and textColor.
     * @return property descriptors for properties of HelpLink objects
     * @see HelpLink
     */
    public PropertyDescriptor[] getPropertyDescriptors() {
        try {
            PropertyDescriptor[] pds = {
                new PropertyDescriptor("target",     HelpLink.class),
                new PropertyDescriptor("text",       HelpLink.class),
                new PropertyDescriptor("textFont",   HelpLink.class),
                new PropertyDescriptor("textColor",  HelpLink.class)
            };
            return pds;
        }
        catch (Exception e) {
            JavaTestError.unexpectedException(e);
            return null;
        }
    }
}
