/*
 * Copyright 1999-2006 Sun Microsystems, Inc.  All Rights Reserved.
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
#include "incls/_c1_FrameMap_ppc.cpp.incl"

LIR_Opr FrameMap::map_to_opr(BasicType type, VMRegPair* reg, bool)
{
  LIR_Opr opr = LIR_OprFact::illegalOpr;
  VMReg r_1 = reg->first();
  VMReg r_2 = reg->second();
  if (r_1->is_stack()) {
    Unimplemented();
  }
  else if (r_1->is_Register()) {
    Register reg = r_1->as_Register();
    if (r_2->is_Register()) {
      Register reg2 = r_2->as_Register();
      opr = as_long_opr(reg2, reg);
    } else if (type == T_OBJECT) {
      opr = as_oop_opr(reg);
    } else {
      opr = as_opr(reg);
    }
  }
  else if (r_1->is_FloatRegister()) {
    Unimplemented();
  }
  else {
    ShouldNotReachHere();
  }
  return opr;
}

LIR_Opr FrameMap::gpr_opr[];
LIR_Opr FrameMap::gpr_oop_opr[];
LIR_Opr FrameMap::fpr_opr[];

LIR_Opr FrameMap::_caller_save_cpu_regs[] = { 0, };
LIR_Opr FrameMap::_caller_save_fpu_regs[] = { 0, };

void FrameMap::init()
{
  if (_init_done)
    return;

  for (int i = 0; i < nof_cpu_regs; i++) {
    map_register(i, as_Register(i));
    gpr_opr[i] = LIR_OprFact::single_cpu(i);
    gpr_oop_opr[i] = LIR_OprFact::single_cpu_oop(i);
  }

  for (int i = 0, j = 0; j < nof_caller_save_cpu_regs; i++) {
    if (i != 1 && i != 2)
      _caller_save_cpu_regs[j++] = LIR_OprFact::single_cpu(i);
  }

  for (int i = 0; i < nof_caller_save_fpu_regs; i++) {
    _caller_save_fpu_regs[i] = LIR_OprFact::single_fpu(i);
  }

  _init_done = true;
}

bool FrameMap::validate_frame()
{
#ifdef XXX_EVIL_EVIL_EVIL
  return true;
#endif
}

VMReg FrameMap::fpu_regname(int n)
{
  Unimplemented();
}

LIR_Opr FrameMap::stack_pointer()
{
  Unimplemented();
}

bool FrameMap::is_caller_save_register(LIR_Opr reg)
{
  Unimplemented();
}
