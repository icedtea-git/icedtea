/*
 * Copyright 2002-2005 Sun Microsystems, Inc.  All Rights Reserved.
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
#include "incls/_register_definitions_@@cpu@@.cpp.incl"

REGISTER_DEFINITION(Register, noreg);

#ifdef PPC
REGISTER_DEFINITION(Register, r0);
REGISTER_DEFINITION(Register, r1);
REGISTER_DEFINITION(Register, r2);
REGISTER_DEFINITION(Register, r3);
REGISTER_DEFINITION(Register, r4);
REGISTER_DEFINITION(Register, r5);
REGISTER_DEFINITION(Register, r6);
REGISTER_DEFINITION(Register, r7);
REGISTER_DEFINITION(Register, r8);
REGISTER_DEFINITION(Register, r9);
REGISTER_DEFINITION(Register, r10);
REGISTER_DEFINITION(Register, r11);
REGISTER_DEFINITION(Register, r12);
REGISTER_DEFINITION(Register, r13);
REGISTER_DEFINITION(Register, r14);
REGISTER_DEFINITION(Register, r15);
REGISTER_DEFINITION(Register, r16);
REGISTER_DEFINITION(Register, r17);
REGISTER_DEFINITION(Register, r18);
REGISTER_DEFINITION(Register, r19);
REGISTER_DEFINITION(Register, r20);
REGISTER_DEFINITION(Register, r21);
REGISTER_DEFINITION(Register, r22);
REGISTER_DEFINITION(Register, r23);
REGISTER_DEFINITION(Register, r24);
REGISTER_DEFINITION(Register, r25);
REGISTER_DEFINITION(Register, r26);
REGISTER_DEFINITION(Register, r27);
REGISTER_DEFINITION(Register, r28);
REGISTER_DEFINITION(Register, r29);
REGISTER_DEFINITION(Register, r30);
REGISTER_DEFINITION(Register, r31);

REGISTER_DEFINITION(FloatRegister, f0);
REGISTER_DEFINITION(FloatRegister, f1);
REGISTER_DEFINITION(FloatRegister, f2);
REGISTER_DEFINITION(FloatRegister, f3);
REGISTER_DEFINITION(FloatRegister, f4);
REGISTER_DEFINITION(FloatRegister, f5);
REGISTER_DEFINITION(FloatRegister, f6);
REGISTER_DEFINITION(FloatRegister, f7);
REGISTER_DEFINITION(FloatRegister, f8);
REGISTER_DEFINITION(FloatRegister, f9);
REGISTER_DEFINITION(FloatRegister, f10);
REGISTER_DEFINITION(FloatRegister, f11);
REGISTER_DEFINITION(FloatRegister, f12);
REGISTER_DEFINITION(FloatRegister, f13);
REGISTER_DEFINITION(FloatRegister, f14);
REGISTER_DEFINITION(FloatRegister, f15);
REGISTER_DEFINITION(FloatRegister, f16);
REGISTER_DEFINITION(FloatRegister, f17);
REGISTER_DEFINITION(FloatRegister, f18);
REGISTER_DEFINITION(FloatRegister, f19);
REGISTER_DEFINITION(FloatRegister, f20);
REGISTER_DEFINITION(FloatRegister, f21);
REGISTER_DEFINITION(FloatRegister, f22);
REGISTER_DEFINITION(FloatRegister, f23);
REGISTER_DEFINITION(FloatRegister, f24);
REGISTER_DEFINITION(FloatRegister, f25);
REGISTER_DEFINITION(FloatRegister, f26);
REGISTER_DEFINITION(FloatRegister, f27);
REGISTER_DEFINITION(FloatRegister, f28);
REGISTER_DEFINITION(FloatRegister, f29);
REGISTER_DEFINITION(FloatRegister, f30);
REGISTER_DEFINITION(FloatRegister, f31);

REGISTER_DEFINITION(ConditionRegister, cr0);
REGISTER_DEFINITION(ConditionRegister, cr1);
REGISTER_DEFINITION(ConditionRegister, cr2);
REGISTER_DEFINITION(ConditionRegister, cr3);
REGISTER_DEFINITION(ConditionRegister, cr4);
REGISTER_DEFINITION(ConditionRegister, cr5);
REGISTER_DEFINITION(ConditionRegister, cr6);
REGISTER_DEFINITION(ConditionRegister, cr7);

REGISTER_DEFINITION(SpecialPurposeRegister, xer);
REGISTER_DEFINITION(SpecialPurposeRegister, lr);
REGISTER_DEFINITION(SpecialPurposeRegister, ctr);

// Non-volatile registers used by the interpreter
REGISTER_DEFINITION(Register, Rmethod);
REGISTER_DEFINITION(Register, Rlocals);
REGISTER_DEFINITION(Register, Rthread);
#endif // PPC
