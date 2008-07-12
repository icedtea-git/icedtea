/*
 * Copyright 2000-2007 Sun Microsystems, Inc.  All Rights Reserved.
 * Copyright 2008 Red Hat, Inc.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
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
 *
 */

//
// Defines all global flags used by the shark compiler.
//

#define SHARK_FLAGS(develop, develop_pd, product, product_pd, diagnostic, notproduct) \
                                                                              \
  product(intx, MaxNodeLimit, 65000,                                          \
          "Maximum number of nodes")                                          \
                                                                              \
  /* compiler debugging */                                                    \
  notproduct(uintx, SharkStartAt, 0,                                          \
             "First method to consider when using Shark")                     \
                                                                              \
  notproduct(uintx, SharkStopAfter, max_uintx,                                \
             "Last method to consider when using Shark")                      \
                                                                              \
  notproduct(uintx, SharkDumpModuleAfter, max_uintx,                          \
             "Dump the LLVM bitcode after compiling this number of methods")  \
                                                                              \
  notproduct(uintx, SharkPrintTypeflowAfter, max_uintx,                       \
             "Print the typeflow after compiling this number of methods")     \
                                                                              \
  notproduct(bool, SharkTraceBytecodes, false,                                \
             "Trace bytecode compilation")                                    \

SHARK_FLAGS(DECLARE_DEVELOPER_FLAG, DECLARE_PD_DEVELOPER_FLAG, DECLARE_PRODUCT_FLAG, DECLARE_PD_PRODUCT_FLAG, DECLARE_DIAGNOSTIC_FLAG, DECLARE_NOTPRODUCT_FLAG)
