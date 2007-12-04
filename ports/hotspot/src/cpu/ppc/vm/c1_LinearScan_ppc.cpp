/*
 * Copyright 2005 Sun Microsystems, Inc.  All Rights Reserved.
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
#include "incls/_c1_LinearScan_ppc.cpp.incl"

void LinearScan::allocate_fpu_stack()
{
  Unimplemented();
}

bool LinearScan::is_processed_reg_num(int reg_num)
{
#ifdef XXX_EVIL_EVIL_EVIL
  return true;
#else
  Unimplemented();
#endif
}

int LinearScan::num_physical_regs(BasicType type)
{
#ifdef PPC32
  if (type == T_LONG) {
#ifdef XXX_EVIL_EVIL_EVIL
    // XXX how to enforce "alignment"?
    return 2;
#else
    Unimplemented();
#endif // XXX_EVIL_EVIL_EVIL
  }
#endif // PPC32
  return 1;
}

bool LinearScan::requires_adjacent_regs(BasicType type)
{
  return type == T_LONG;
}

bool LinearScan::is_caller_save(int assigned_reg)
{
  Unimplemented();
}

void LinearScan::pd_add_temps(LIR_Op* op)
{
  // No special case behaviours yet
}

bool LinearScanWalker::pd_init_regs_for_alloc(Interval* cur)
{
#ifdef XXX_EVIL_EVIL_EVIL
  // XXX how to stop it from using r13?
  return false;
#else
  Unimplemented();
#endif
}
