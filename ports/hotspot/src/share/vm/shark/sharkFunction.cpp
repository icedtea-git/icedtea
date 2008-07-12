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
#include "incls/_sharkFunction.cpp.incl"

using namespace llvm;

void SharkFunction::initialize()
{
#ifndef PRODUCT
  _debug = false;
#endif // PRODUCT

  // Create the function
  _function = builder()->CreateFunction();
  set_block_insertion_point(NULL);

  // Initialize the blocks
  _blocks = NEW_RESOURCE_ARRAY(SharkBlock*, flow()->block_count());
  for (int i = 0; i < block_count(); i++) {
    _blocks[i] = new SharkBlock(this, flow()->pre_order_at(i));
    if (block(i)->start() == 0) {
      assert(start_block() == NULL, "multiple start blocks");
      _start_block = block(i);
    }
  }
  start_block()->add_predecessor(NULL);
  for (int i = 0; i < block_count(); i++) {
    for (int j = 0; j < block(i)->num_successors(); j++) {
      block(i)->successor(j)->add_predecessor(block(i));
    }
  }

  // Get our arguments
  Function::arg_iterator ai = function()->arg_begin();
  Argument *method = ai++;
  _thread = ai++;

  method->setName("method");
  _thread->setName("thread");

  // Create the method preamble
  set_block_insertion_point(&function()->front());
  builder()->SetInsertPoint(CreateBlock());
  CreateInitZeroStack();
  CreatePushFrame(CreateBuildFrame());

  // Transition into the method proper
  start_block()->add_incoming(new SharkEntryState(start_block()));
  builder()->CreateBr(start_block()->entry_block());

  // Parse the blocks
  for (int i = 0; i < block_count(); i++) {
    if (i + 1 < block_count())
      set_block_insertion_point(block(i + 1)->entry_block());
    else
      set_block_insertion_point(NULL);

    block(i)->parse();
    if (failing())
      return;
  }
}

void SharkFunction::CreateInitZeroStack()
{
  Value *zero_stack = builder()->CreateAddressOfStructEntry(
    thread(), JavaThread::zero_stack_offset(),
    SharkType::zeroStack_type(),
    "zero_stack");

  _zero_stack_base = builder()->CreateValueOfStructEntry(
    zero_stack, ZeroStack::base_offset(),
    SharkType::intptr_type(),
    "zero_stack_base");

  _zero_stack_pointer_addr = builder()->CreateAddressOfStructEntry(
    zero_stack, ZeroStack::sp_offset(),
    PointerType::getUnqual(SharkType::intptr_type()),
    "zero_stack_pointer_addr");

  _zero_frame_pointer_addr = builder()->CreateAddressOfStructEntry(
    thread(), JavaThread::top_zero_frame_offset(),
    PointerType::getUnqual(SharkType::intptr_type()),
    "zero_frame_pointer_addr");
}

void SharkFunction::CreateStackOverflowCheck(Value *sp)
{
  BasicBlock *overflow = CreateBlock("stack_overflow");
  BasicBlock *no_overflow = CreateBlock("no_overflow");
  
  builder()->CreateCondBr(
    builder()->CreateICmpULT(sp, zero_stack_base()),
    overflow, no_overflow);

  builder()->SetInsertPoint(overflow);
  builder()->CreateUnimplemented(__FILE__, __LINE__);
  builder()->CreateUnreachable();

  builder()->SetInsertPoint(no_overflow);  
}

void SharkFunction::CreatePushFrame(Value *fp)
{
  builder()->CreateStore(CreateLoadZeroFramePointer(), fp);
  CreateStoreZeroFramePointer(
    builder()->CreatePtrToInt(fp, SharkType::intptr_type()));
}

Value* SharkFunction::CreatePopFrame(int result_slots)
{
  assert(result_slots >= 0 && result_slots <= 2, "should be");

  int locals_to_pop = max_locals() - result_slots;

  Value *fp = CreateLoadZeroFramePointer();
  Value *sp = builder()->CreateAdd(
    fp,
    LLVMValue::jint_constant((1 + locals_to_pop) * wordSize));

  CreateStoreZeroStackPointer(sp);
  CreateStoreZeroFramePointer(
    builder()->CreateLoad(
      builder()->CreateIntToPtr(
        fp, PointerType::getUnqual(SharkType::intptr_type()))));

#ifdef _LP64
  if (result_slots == 2)
    return builder()->CreateAdd(sp, LLVMValue::jint_constant(wordSize));
#endif // _LP64
  return sp;
}

Value* SharkFunction::CreateBuildFrame()
{
  if (flow()->method()->is_synchronized() || flow()->method()->uses_monitors())
    Unimplemented();

  int locals_words = max_locals();
  int extra_locals = locals_words - arg_size();
  int header_words = SharkFrame::header_words;
  int stack_words  = max_stack();
  int frame_words  = header_words + stack_words;
  
  // Update the stack pointer
  Value *zero_stack_pointer = builder()->CreateSub(
    CreateLoadZeroStackPointer(),
    LLVMValue::intptr_constant((frame_words + extra_locals) * wordSize));
  CreateStackOverflowCheck(zero_stack_pointer);
  CreateStoreZeroStackPointer(zero_stack_pointer);

  // Create the frame
  Value *frame = builder()->CreateIntToPtr(
    zero_stack_pointer,
    PointerType::getUnqual(
      ArrayType::get(SharkType::intptr_type(), frame_words + locals_words)),
    "frame");
  int offset = 0;

  // Java stack
  _stack_slots = builder()->CreateBitCast(
    builder()->CreateStructGEP(frame, offset),
    PointerType::getUnqual(
      ArrayType::get(SharkType::intptr_type(), stack_words)),
    "stack_slots");
  offset += stack_words;

  // Method pointer
  _method_slot = builder()->CreateBitCast(
    builder()->CreateStructGEP(frame, offset++),
    PointerType::getUnqual(SharkType::methodOop_type()),
    "method_slot");

  // Frame header
  builder()->CreateStore(
    LLVMValue::intptr_constant(ZeroFrame::SHARK_FRAME),
    builder()->CreateStructGEP(frame, offset++));
  Value *fp = builder()->CreateStructGEP(frame, offset++, "fp");

  // Local variables
  _locals_slots = builder()->CreateBitCast(
    builder()->CreateStructGEP(frame, offset),
    PointerType::getUnqual(
      ArrayType::get(SharkType::intptr_type(), locals_words)),
    "locals_slots");
  offset += locals_words;

  assert(offset == frame_words + locals_words, "should do");
  return fp;
}
