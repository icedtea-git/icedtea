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
package com.sun.javatest.mrep;

import java.io.File;
/**
 *
 *
 * This a short description of test which can be used
 * to resolve conflicts and prepare summary.
 *
 * The constructors are made package local to avoid
 * creating instance of this class outside mrep.
 *
 */
class TestResultDescr {

        private File file;
        private String status;
        private long time;
        private int id;

        TestResultDescr(File file, String status, int id, int time) {
                this.file = file;
                this.status = status;
                this.time = time;
                this.id = id;
        }
        TestResultDescr(String status, int id, long time) {
                this.file = null;
                this.status = status;
                this.time = time;
                this.id = id;
        }
        public File getFile() {
                return file;
        }
        public String getStatus() {
                return status;
        }
        public long getTime() {
                return time;
        }
        int getID() {
                return id;
        }
        public void setID(int id) {
                this.id = id;
        }
        public void setFile(File file) {
                this.file = file;
        }

}
