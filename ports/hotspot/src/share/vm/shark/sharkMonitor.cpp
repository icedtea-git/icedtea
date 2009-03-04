/*
 * Copyright 1999-2007 Sun Microsystems, Inc.  All Rights Reserved.
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

#include "incls/_precompiled.incl"
#include "incls/_sharkMonitor.cpp.incl"

using namespace llvm;

void SharkMonitor::initialize()
{
  _object_addr = builder()->CreateAddressOfStructEntry(
    monitor(), in_ByteSize(BasicObjectLock::obj_offset_in_bytes()),
    PointerType::getUnqual(SharkType::oop_type()),
    "object_addr");

  _displaced_header_addr = builder()->CreateAddressOfStructEntry(
    monitor(), in_ByteSize(
      BasicObjectLock::lock_offset_in_bytes() +
      BasicLock::displaced_header_offset_in_bytes()),
    PointerType::getUnqual(SharkType::intptr_type()),
    "displaced_header_addr");
}

void SharkMonitor::acquire(SharkTopLevelBlock* block, Value *lockee) const
{
  BasicBlock *try_recursive = function()->CreateBlock("try_recursive");
  BasicBlock *got_recursive = function()->CreateBlock("got_recursive");
  BasicBlock *not_recursive = function()->CreateBlock("not_recursive");
  BasicBlock *acquired_fast = function()->CreateBlock("acquired_fast");
  BasicBlock *lock_acquired = function()->CreateBlock("lock_acquired");

  set_object(lockee);

  Value *lock = builder()->CreatePtrToInt(
    displaced_header_addr(), SharkType::intptr_type());

  Value *mark_addr = builder()->CreateAddressOfStructEntry(
    lockee, in_ByteSize(oopDesc::mark_offset_in_bytes()),
    PointerType::getUnqual(SharkType::intptr_type()),
    "mark_addr");

  Value *mark = builder()->CreateLoad(mark_addr, "mark");
  Value *disp = builder()->CreateOr(
    mark, LLVMValue::intptr_constant(markOopDesc::unlocked_value), "disp");
  set_displaced_header(disp);

  // Try a simple lock
  Value *check = builder()->CreateCmpxchgPtr(lock, mark_addr, disp);
  builder()->CreateCondBr(
    builder()->CreateICmpEQ(disp, check),
    acquired_fast, try_recursive);

  // Locking failed, but maybe this thread already owns it
  builder()->SetInsertPoint(try_recursive);
  Value *addr = builder()->CreateAnd(
    disp,
    LLVMValue::intptr_constant(~markOopDesc::lock_mask_in_place));

  // NB we use the entire stack, but JavaThread::is_lock_owned()
  // uses a more limited range.  I don't think it hurts though...
  Value *stack_limit = builder()->CreateValueOfStructEntry(
    function()->thread(), Thread::stack_base_offset(),
    SharkType::intptr_type(),
    "stack_limit");

  assert(sizeof(size_t) == sizeof(intptr_t), "should be");
  Value *stack_size = builder()->CreateValueOfStructEntry(
    function()->thread(), Thread::stack_size_offset(),
    SharkType::intptr_type(),
    "stack_size");

  Value *stack_start =
    builder()->CreateSub(stack_limit, stack_size, "stack_start");

  builder()->CreateCondBr(
    builder()->CreateAnd(
      builder()->CreateICmpUGE(addr, stack_start),
      builder()->CreateICmpULT(addr, stack_limit)),
    got_recursive, not_recursive);
                         
  builder()->SetInsertPoint(got_recursive);
  set_displaced_header(LLVMValue::intptr_constant(0));
  builder()->CreateBr(acquired_fast);

  // Create an edge for the state merge
  builder()->SetInsertPoint(acquired_fast);
  SharkState *fast_state = block->current_state()->copy();
  builder()->CreateBr(lock_acquired);

  // It's not a recursive case so we need to drop into the runtime
  builder()->SetInsertPoint(not_recursive);
  block->call_vm_nocheck(SharkRuntime::monitorenter(), monitor());
  BasicBlock *acquired_slow = builder()->GetInsertBlock();
  builder()->CreateBr(lock_acquired);  

  // All done
  builder()->SetInsertPoint(lock_acquired);
  block->current_state()->merge(fast_state, acquired_fast, acquired_slow);
}

void SharkMonitor::release(SharkTopLevelBlock* block) const
{
  BasicBlock *not_recursive = function()->CreateBlock("not_recursive");
  BasicBlock *released_fast = function()->CreateBlock("released_fast");
  BasicBlock *slow_path     = function()->CreateBlock("slow_path");
  BasicBlock *lock_released = function()->CreateBlock("lock_released");

  Value *disp = displaced_header();
  Value *lockee = object();
  set_object(LLVMValue::null());

  // If it is recursive then we're already done
  builder()->CreateCondBr(
    builder()->CreateICmpEQ(disp, LLVMValue::intptr_constant(0)),
    released_fast, not_recursive);

  // Try a simple unlock
  builder()->SetInsertPoint(not_recursive);

  Value *lock = builder()->CreatePtrToInt(
    displaced_header_addr(), SharkType::intptr_type());

  Value *mark_addr = builder()->CreateAddressOfStructEntry(
    lockee, in_ByteSize(oopDesc::mark_offset_in_bytes()),
    PointerType::getUnqual(SharkType::intptr_type()),
    "mark_addr");

  Value *check = builder()->CreateCmpxchgPtr(disp, mark_addr, lock);
  builder()->CreateCondBr(
    builder()->CreateICmpEQ(lock, check),
    released_fast, slow_path);

  // Create an edge for the state merge
  builder()->SetInsertPoint(released_fast);
  SharkState *fast_state = block->current_state()->copy();
  builder()->CreateBr(lock_released);  

  // Need to drop into the runtime to release this one
  builder()->SetInsertPoint(slow_path);
  set_object(lockee);
  block->call_vm_nocheck(SharkRuntime::monitorexit(), monitor());
  BasicBlock *released_slow = builder()->GetInsertBlock();
  builder()->CreateBr(lock_released);  

  // All done
  builder()->SetInsertPoint(lock_released);
  block->current_state()->merge(fast_state, released_fast, released_slow);
}
