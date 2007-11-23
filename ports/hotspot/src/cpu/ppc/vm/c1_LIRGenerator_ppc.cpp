/*
 * Copyright 2005-2006 Sun Microsystems, Inc.  All Rights Reserved.
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
#include "incls/_c1_LIRGenerator_ppc.cpp.incl"

void LIRGenerator::get_Object_unsafe(LIR_Opr dst, LIR_Opr src, LIR_Opr offset,
                                     BasicType type, bool is_volatile)
{
  Unimplemented();
}

LIR_Address* LIRGenerator::generate_address(LIR_Opr base, LIR_Opr index,
                                            int shift, int disp, BasicType type)
{
  Unimplemented();
}

void LIRGenerator::do_BlockBegin(BlockBegin* x)
{
  Unimplemented();
}

LIR_Address* LIRGenerator::emit_array_address(LIR_Opr array_opr, LIR_Opr index_opr,
                                              BasicType type, bool needs_card_mark)
{
  Unimplemented();
}

void LIRGenerator::cmp_reg_mem(LIR_Condition condition, LIR_Opr reg, LIR_Opr base, LIR_Opr disp, BasicType type, CodeEmitInfo* info)
{
  Unimplemented();
}

void LIRGenerator::increment_counter(LIR_Address* addr, int step)
{
  Unimplemented();
}

void LIRGenerator::put_Object_unsafe(LIR_Opr src, LIR_Opr offset, LIR_Opr data,
                                     BasicType type, bool is_volatile)
{
  Unimplemented();
}

void LIRGenerator::do_InstanceOf(InstanceOf* x)
{
  Unimplemented();
}

void LIRGenerator::increment_counter(address counter, int step)
{
  Unimplemented();
}

void LIRGenerator::do_CheckCast(CheckCast* x)
{
  Unimplemented();
}

bool LIRGenerator::can_store_as_constant(Value v, BasicType type) const
{
  Unimplemented();
}

LIR_Opr LIRGenerator::result_register_for(ValueType* type, bool callee)
{
  Unimplemented();
}

LIR_Opr LIRGenerator::exceptionPcOpr()
{
  Unimplemented();
}

LIR_Opr LIRGenerator::safepoint_poll_register()
{
  Unimplemented();
}

bool LIRGenerator::strength_reduce_multiply(LIR_Opr left, int c, LIR_Opr result, LIR_Opr tmp)
{
  Unimplemented();
}

void LIRGenerator::do_ArrayCopy(Intrinsic* x)
{
  Unimplemented();
}

void LIRGenerator::do_NewInstance(NewInstance* x)
{
  Unimplemented();
}

void LIRGenerator::do_NewMultiArray(NewMultiArray* x)
{
  Unimplemented();
}

void LIRGenerator::cmp_reg_mem(LIR_Condition condition, LIR_Opr reg, LIR_Opr base, int disp, BasicType type, CodeEmitInfo* info)
{
  Unimplemented();
}

void LIRGenerator::volatile_field_store(LIR_Opr value, LIR_Address* address,
                                        CodeEmitInfo* info)
{
  Unimplemented();
}

LIR_Opr LIRGenerator::rlock_byte(BasicType type)
{
  Unimplemented();
}

LIR_Opr LIRGenerator::exceptionOopOpr()
{
  Unimplemented();
}

void LIRGenerator::do_NegateOp(NegateOp* x)
{
  Unimplemented();
}

void LIRGenerator::volatile_field_load(LIR_Address* address, LIR_Opr result,
                                       CodeEmitInfo* info)
{
  Unimplemented();
}

bool LIRGenerator::can_inline_as_constant(Value v) const
{
  Unimplemented();
}

void LIRGenerator::cmp_mem_int(LIR_Condition condition, LIR_Opr base, int disp, int c, CodeEmitInfo* info)
{
  Unimplemented();
}

void LIRGenerator::do_StoreIndexed(StoreIndexed* x)
{
  Unimplemented();
}

void LIRGenerator::do_LogicOp(LogicOp* x)
{
  Unimplemented();
}

LIR_Opr LIRGenerator::getThreadPointer()
{
  Unimplemented();
}

void LIRGenerator::do_AttemptUpdate(Intrinsic* x)
{
  Unimplemented();
}

void LIRGenerator::do_MonitorExit(MonitorExit* x)
{
  Unimplemented();
}

void LIRGenerator::do_MathIntrinsic(Intrinsic* x)
{
  Unimplemented();
}

void LIRGenerator::do_ShiftOp(ShiftOp* x)
{
  Unimplemented();
}

void LIRItem::load_byte_item()
{
  Unimplemented();
}

bool LIRGenerator::can_inline_as_constant(LIR_Const* c) const
{
  Unimplemented();
}

void LIRItem::load_nonconstant()
{
  Unimplemented();
}

void LIRGenerator::do_NewTypeArray(NewTypeArray* x)
{
  Unimplemented();
}

void LIRGenerator::trace_block_entry(BlockBegin* block)
{
  Unimplemented();
}

void LIRGenerator::do_NewObjectArray(NewObjectArray* x)
{
  Unimplemented();
}

void LIRGenerator::do_ArithmeticOp(ArithmeticOp* x)
{
  Unimplemented();
}

LIR_Opr LIRGenerator::getThreadTemp()
{
  Unimplemented();
}

void LIRGenerator::do_MonitorEnter(MonitorEnter* x)
{
  Unimplemented();
}

void LIRGenerator::do_Convert(Convert* x)
{
  Unimplemented();
}

LIR_Opr LIRGenerator::syncTempOpr()
{
  Unimplemented();
}

void LIRGenerator::do_If(If* x)
{
  Unimplemented();
}

void LIRGenerator::do_CompareOp(CompareOp* x)
{
  Unimplemented();
}

void LIRGenerator::do_CompareAndSwap(Intrinsic* x, ValueType* type)
{
  Unimplemented();
}
