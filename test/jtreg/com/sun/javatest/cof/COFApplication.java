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
package com.sun.javatest.cof;

import java.util.LinkedHashMap;

public class COFApplication extends COFItem {

        static LinkedHashMap xmlAttributes;

        static LinkedHashMap xmlElements;

        static String xmlTagName;
        static {
                xmlTagName = "application";
                xmlAttributes = new LinkedHashMap();
                xmlAttributes.put("id", "id");
                xmlAttributes.put("environmentid", "environmentid");
                xmlAttributes.put("swentityid", "swentityid");
        }

        protected String id = "app:0";

        protected String environmentid="env:0";

        protected String swentityid = "swentity:0";

        /**
         * Gets the value of the id property.
         *
         * @return possible object is {@link String }
         *
         */
        public String getId() {
                return id;
        }

        LinkedHashMap getItemAttributes() {
                return xmlAttributes;
        }

        LinkedHashMap getItemElements() {
                return xmlElements;
        }

        String getItemTagName() {
                return xmlTagName;
        }

        public void setId(String value) {
                this.id = value;
        }

        public String getEnvironmentid() {
                return environmentid;
        }

        public void setEnvironmentid(String environmentid) {
                this.environmentid = environmentid;
        }

        public String getSwentityid() {
                return swentityid;
        }

        public void setSwentityid(String swentityid) {
                this.swentityid = swentityid;
        }

        /**
         * Sets the value of the name property.
         *
         * @param value
         *            allowed object is {@link String }
         *
         */

}
