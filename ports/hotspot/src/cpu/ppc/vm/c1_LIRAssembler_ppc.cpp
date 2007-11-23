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
#include "incls/_c1_LIRAssembler_ppc.cpp.incl"

void LIR_Assembler::emit_opTypeCheck(LIR_OpTypeCheck* op)
{
  Unimplemented();
}

void LIR_Assembler::align_call(LIR_Code code)
{
  Unimplemented();
}

void LIR_Assembler::osr_entry()
{
  Unimplemented();
}

void LIR_Assembler::const2mem(LIR_Opr src, LIR_Opr dest, BasicType type, CodeEmitInfo* info )
{
  Unimplemented();
}

void LIR_Assembler::emit_alloc_array(LIR_OpAllocArray* op)
{
  Unimplemented();
}

void LIR_Assembler::prefetchw(LIR_Opr src)
{
  Unimplemented();
}

void LIR_Assembler::mem2reg(LIR_Opr src, LIR_Opr dest, BasicType type, LIR_PatchCode patch_code, CodeEmitInfo* info, bool /* unaligned */)
{
  Unimplemented();
}

void LIR_Assembler::volatile_move_op(LIR_Opr src, LIR_Opr dest, BasicType type, CodeEmitInfo* info)
{
  Unimplemented();
}

void LIR_Assembler::emit_compare_and_swap(LIR_OpCompareAndSwap* op)
{
  Unimplemented();
}

void LIR_Assembler::reg2mem(LIR_Opr src, LIR_Opr dest, BasicType type, LIR_PatchCode patch_code, CodeEmitInfo* info, bool pop_fpu_stack, bool /* unaligned */)
{
  Unimplemented();
}

void LIR_Assembler::shift_op(LIR_Code code, LIR_Opr left, LIR_Opr count, LIR_Opr dest, LIR_Opr tmp)
{
  Unimplemented();
}

int LIR_Assembler::safepoint_poll(LIR_Opr tmp, CodeEmitInfo* info)
{
  Unimplemented();
}

void LIR_Assembler::set_24bit_FPU()
{
  Unimplemented();
}

void LIR_Assembler::const2stack(LIR_Opr src, LIR_Opr dest)
{
  Unimplemented();
}

void LIR_Assembler::membar()
{
  Unimplemented();
}

int LIR_Assembler::initial_frame_size_in_bytes()
{
  Unimplemented();
}

void LIR_Assembler::emit_alloc_obj(LIR_OpAllocObj* op)
{
  Unimplemented();
}

void LIR_Assembler::membar_release()
{
  Unimplemented();
}

void LIR_Assembler::prefetchr(LIR_Opr src)
{
  Unimplemented();
}

void LIR_Assembler::membar_acquire()
{
  Unimplemented();
}

void LIR_Assembler::emit_exception_handler()
{
  Unimplemented();
}

void LIR_Assembler::negate(LIR_Opr left, LIR_Opr dest)
{
  Unimplemented();
}

void LIR_Assembler::stack2stack(LIR_Opr src, LIR_Opr dest, BasicType type)
{
  Unimplemented();
}

void LIR_Assembler::throw_op(LIR_Opr exceptionPC, LIR_Opr exceptionOop, CodeEmitInfo* info, bool unwind)
{
  Unimplemented();
}

void LIR_Assembler::pop(LIR_Opr opr)
{
  Unimplemented();
}

void LIR_Assembler::stack2reg(LIR_Opr src, LIR_Opr dest, BasicType type)
{
  Unimplemented();
}

void LIR_Assembler::push(LIR_Opr opr)
{
  Unimplemented();
}

void LIR_Assembler::fxch(int i)
{
  Unimplemented();
}

void LIR_Assembler::reg2reg(LIR_Opr src, LIR_Opr dest)
{
  Unimplemented();
}

LIR_Opr LIR_Assembler::osrBufferPointer()
{
  Unimplemented();
}

void LIR_Assembler::leal(LIR_Opr addr, LIR_Opr dest)
{
  Unimplemented();
}

void LIR_Assembler::emit_opBranch(LIR_OpBranch* op)
{
  Unimplemented();
}

void LIR_Assembler::comp_fl2i(LIR_Code code, LIR_Opr left, LIR_Opr right, LIR_Opr dst, LIR_Op2* op)
{
  Unimplemented();
}

void LIR_Assembler::align_backward_branch_target()
{
  Unimplemented();
}

void LIR_Assembler::reset_FPU()
{
  Unimplemented();
}

void LIR_Assembler::emit_op3(LIR_Op3* op)
{
  Unimplemented();
}

void LIR_Assembler::vtable_call(int vtable_offset, CodeEmitInfo* info)
{
  Unimplemented();
}

void LIR_Assembler::cmove(LIR_Condition condition, LIR_Opr opr1, LIR_Opr opr2, LIR_Opr result)
{
  Unimplemented();
}

void LIR_Assembler::emit_profile_call(LIR_OpProfileCall* op)
{
  Unimplemented();
}

void LIR_Assembler::const2reg(LIR_Opr src, LIR_Opr dest, LIR_PatchCode patch_code, CodeEmitInfo* info)
{
  Unimplemented();
}

void LIR_Assembler::monitor_address(int monitor_no, LIR_Opr dst)
{
  Unimplemented();
}

void LIR_Assembler::comp_op(LIR_Condition condition, LIR_Opr opr1, LIR_Opr opr2, LIR_Op2* op)
{
  Unimplemented();
}

void LIR_Assembler::rt_call(LIR_Opr result, address dest, const LIR_OprList* args, LIR_Opr tmp, CodeEmitInfo* info)
{
  Unimplemented();
}

void LIR_Assembler::peephole(LIR_List*)
{
  Unimplemented();
}

void LIR_Assembler::fld(int i)
{
  Unimplemented();
}

void LIR_Assembler::fpop()
{
  Unimplemented();
}

void LIR_Assembler::emit_lock(LIR_OpLock* op)
{
  Unimplemented();
}

void LIR_Assembler::reg2stack(LIR_Opr src, LIR_Opr dest, BasicType type, bool pop_fpu_stack)
{
  Unimplemented();
}

void LIR_Assembler::get_thread(LIR_Opr result_reg)
{
  Unimplemented();
}

void LIR_Assembler::emit_delay(LIR_OpDelay*)
{
  Unimplemented();
}

void LIR_Assembler::call(address entry, relocInfo::relocType rtype, CodeEmitInfo* info)
{
  Unimplemented();
}

LIR_Opr LIR_Assembler::receiverOpr()
{
  Unimplemented();
}

void LIR_Assembler::emit_static_call_stub()
{
  Unimplemented();
}

void LIR_Assembler::emit_deopt_handler()
{
  Unimplemented();
}

void LIR_Assembler::arith_op(LIR_Code code, LIR_Opr left, LIR_Opr right, LIR_Opr dest, CodeEmitInfo* info, bool pop_fpu_stack)
{
  Unimplemented();
}

void LIR_Assembler::intrinsic_op(LIR_Code code, LIR_Opr value, LIR_Opr unused, LIR_Opr dest, LIR_Op* op)
{
  Unimplemented();
}

void LIR_Assembler::ffree(int i)
{
  Unimplemented();
}

void LIR_Assembler::shift_op(LIR_Code code, LIR_Opr left, jint count, LIR_Opr dest)
{
  Unimplemented();
}

void LIR_Assembler::emit_opConvert(LIR_OpConvert* op)
{
  Unimplemented();
}

void LIR_Assembler::ic_call(address entry, CodeEmitInfo* info)
{
  Unimplemented();
}

void LIR_Assembler::breakpoint()
{
  Unimplemented();
}

void LIR_Assembler::emit_arraycopy(LIR_OpArrayCopy* op)
{
  Unimplemented();
}

void LIR_Assembler::return_op(LIR_Opr result)
{
  Unimplemented();
}

void LIR_Assembler::logic_op(LIR_Code code, LIR_Opr left, LIR_Opr right, LIR_Opr dst)
{
  Unimplemented();
}

int LIR_Assembler::check_icache()
{
  Unimplemented();
}
