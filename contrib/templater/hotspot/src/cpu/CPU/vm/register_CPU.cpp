/*
 * Copyright 2000-2007 Sun Microsystems, Inc.  All Rights Reserved.
 * Copyright 2007 Red Hat, Inc.
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

#include "incls/_precompiled.incl"
#include "incls/_register_@@cpu@@.cpp.incl"

const int ConcreteRegisterImpl::max_gpr = RegisterImpl::number_of_registers;
const int ConcreteRegisterImpl::max_fpr =
  ConcreteRegisterImpl::max_gpr + FloatRegisterImpl::number_of_registers;

const char* RegisterImpl::name() const {
#ifdef PPC
  const char* names[number_of_registers] = {
    "r0",  "r1",  "r2",  "r3",  "r4",  "r5",  "r6",  "r7",
    "r8",  "r9",  "r10", "r11", "r12", "r13", "r14", "r15",
    "r16", "r17", "r18", "r19", "r20", "r21", "r22", "r23", 
    "r24", "r25", "r26", "r27", "r28", "r29", "r30", "r31"
  };
  return is_valid() ? names[encoding()] : "noreg";
#else
  Unimplemented();
#endif // PPC
}

const char* FloatRegisterImpl::name() const {
#ifdef PPC
  const char* names[number_of_registers] = {
    "f0",  "f1",  "f2",  "f3",  "f4",  "f5",  "f6",  "f7",
    "f8",  "f9",  "f10", "f11", "f12", "f13", "f14", "f15",
    "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", 
    "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31"
  };
  return is_valid() ? names[encoding()] : "noreg";
#else
  Unimplemented();
#endif // PPC
}
