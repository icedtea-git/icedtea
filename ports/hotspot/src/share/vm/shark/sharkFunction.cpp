/*
 * Copyright 1999-2007 Sun Microsystems, Inc.  All Rights Reserved.
 * Copyright 2008, 2009 Red Hat, Inc.
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

#include <fnmatch.h>

using namespace llvm;

void SharkFunction::initialize()
{
  // Emit the entry point
  SharkEntry *entry = (SharkEntry *) masm()->pc();
  masm()->advance(sizeof(SharkEntry));

  // Create the function
  _function = builder()->CreateFunction(name());
  entry->set_llvm_function(function());
#ifndef PRODUCT
  // FIXME: there should be a mutex when updating sharkEntry in case
  // there are multiple compilation threads.
  builder()->sharkEntry[function()] = entry;
#endif // !PRODUCT

  // Create the list of blocks
  set_block_insertion_point(NULL);
  _blocks = NEW_RESOURCE_ARRAY(SharkBlock*, flow()->block_count());
  for (int i = 0; i < block_count(); i++)
    {
      ciTypeFlow::Block *b = flow()->pre_order_at(i);
      // Work around a bug in pre_order_at() that does not return the
      // correct pre-ordering.  If pre_order_at() were correct this
      // line could simply be:
      // _blocks[i] = new SharkBlock(this, b);
      _blocks[b->pre_order()] = new SharkBlock(this, b);
    }
  // Walk the tree from the start block to determine which
  // blocks are entered and which blocks require phis
  SharkBlock *start_block = block(0);
  assert(start_block->start() == 0, "blocks out of order");
  start_block->enter();

  // Initialize all entered blocks
  for (int i = 0; i < block_count(); i++) {
    if (block(i)->entered())
      block(i)->initialize();
  }

  // Initialize the monitors
  _monitor_count = 0;  
  if (target()->is_synchronized() || target()->uses_monitors()) {
    for (int i = 0; i < block_count(); i++)
      _monitor_count = MAX2(
        _monitor_count, block(i)->ciblock()->monitor_count());
  }
  
  // Get our arguments
  Function::arg_iterator ai = function()->arg_begin();
  Argument *method = ai++;
  method->setName("method");
  _base_pc = ai++;
  _base_pc->setName("base_pc");
  _thread = ai++;
  _thread->setName("thread");

  // Create the method preamble
  set_block_insertion_point(&function()->front());
  builder()->SetInsertPoint(CreateBlock());
  CreateInitZeroStack();
  CreatePushFrame(CreateBuildFrame());
  NOT_PRODUCT(builder()->CreateStore(
    method,
    CreateAddressOfFrameEntry(
      method_slot_offset(),
      SharkType::methodOop_type(),
      "method_slot")));

  // Lock if necessary
  SharkState *entry_state = new SharkEntryState(method, start_block);
  if (target()->is_synchronized()) {
    SharkBlock *locker = new SharkBlock(this, start_block->ciblock());
    locker->add_incoming(entry_state);

    set_block_insertion_point(start_block->entry_block());
    locker->acquire_method_lock();

    entry_state = locker->current_state();
  }

  // Transition into the method proper
  start_block->add_incoming(entry_state);
  builder()->CreateBr(start_block->entry_block());

  // Parse the blocks
  for (int i = 0; i < block_count(); i++) {
    if (!block(i)->entered())
      continue;

    if (i + 1 < block_count())
      set_block_insertion_point(block(i + 1)->entry_block());
    else
      set_block_insertion_point(NULL);

    block(i)->parse();
  }

  // Dump the bitcode, if requested
  if (SharkPrintBitcodeOf != NULL) {
    if (!fnmatch(SharkPrintBitcodeOf, name(), 0))
      function()->dump();
  }

  if (SharkPrintAsmOf != NULL) {
#if defined (__x86_64) || defined (__i386)
      std::vector<const char*> Args;
      Args.push_back(""); // program name
      if (!fnmatch(SharkPrintAsmOf, name(), 0))
	// Oh, yuck.  The LLVM name for this debugging dump is
	// target-specific.
	Args.push_back("-debug-only=" "x86-emitter");
      else
	Args.push_back("-debug-only=" "none");
      Args.push_back(0);  // Null terminator.
      cl::ParseCommandLineOptions(Args.size()-1, (char**)&Args[0]);
#endif
  }

  // Compile to native code
  void *code = builder()->execution_engine()->getPointerToFunction(function());

  // Register generated code for profiling, etc
  if (JvmtiExport::should_post_dynamic_code_generated()) {
    JvmtiExport::post_dynamic_code_generated
      (name(), entry->code_start(), entry->code_limit());
  }

  entry->set_entry_point((ZeroEntry::method_entry_t) code);
  if (SharkTraceInstalls)
    entry->print_statistics(name());
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
    LLVMValue::intptr_constant((1 + locals_to_pop) * wordSize));

  CreateStoreZeroStackPointer(sp);
  CreateStoreZeroFramePointer(
    builder()->CreateLoad(
      builder()->CreateIntToPtr(
        fp, PointerType::getUnqual(SharkType::intptr_type()))));

  return sp;
}

Value* SharkFunction::CreateBuildFrame()
{
  int locals_words  = max_locals();
  int extra_locals  = locals_words - arg_size();
  int header_words  = SharkFrame::header_words;
  int monitor_words = monitor_count()*frame::interpreter_frame_monitor_size();
  int stack_words   = max_stack();
  int frame_words   = header_words + monitor_words + stack_words;

  _oopmap_frame_size = frame_words + extra_locals;

  // Update the stack pointer
  Value *zero_stack_pointer = builder()->CreateSub(
    CreateLoadZeroStackPointer(),
    LLVMValue::intptr_constant((frame_words + extra_locals) * wordSize));
  CreateStackOverflowCheck(zero_stack_pointer);
  NOT_PRODUCT(CreateStoreZeroStackPointer(zero_stack_pointer));

  // Create the frame
  _frame = builder()->CreateIntToPtr(
    zero_stack_pointer,
    PointerType::getUnqual(
      ArrayType::get(SharkType::intptr_type(), frame_words + locals_words)),
    "frame");
  int offset = 0;

  // Expression stack
  _stack_slots_offset = offset;
  offset += stack_words;

  // Monitors
  if (monitor_count()) {
    _monitors_slots_offset = offset; 

    for (int i = 0; i < monitor_count(); i++) {
      if (i != 0 || !target()->is_synchronized())
        monitor(i)->mark_free();
    }
  }
  offset += monitor_words;

  // Exception pointer
  _exception_slot_offset = offset++;
  builder()->CreateStore(LLVMValue::null(), exception_slot());

  // Method pointer
  _method_slot_offset = offset++;

  // Unextended SP
  builder()->CreateStore(
    zero_stack_pointer,
    CreateAddressOfFrameEntry(offset++));

  // PC
  _pc_slot_offset = offset++;

  // Frame header
  builder()->CreateStore(
    LLVMValue::intptr_constant(ZeroFrame::SHARK_FRAME),
    CreateAddressOfFrameEntry(offset++));
  Value *fp = CreateAddressOfFrameEntry(offset++);

  // Local variables
  _locals_slots_offset = offset;  
  offset += locals_words;

  assert(offset == frame_words + locals_words, "should do");
  return fp;
}

Value* SharkFunction::CreateAddressOfFrameEntry(int               offset,
                                                const llvm::Type* type,
                                                const char*       name) const
{
  bool needs_cast = type && type != SharkType::intptr_type();

  Value* result = builder()->CreateStructGEP(
    _frame, offset, needs_cast ? "" : name);

  if (needs_cast) {
    result = builder()->CreateBitCast(
      result, PointerType::getUnqual(type), name);
  }
  return result;
}

SharkMonitor* SharkFunction::monitor(Value *index) const
{
  Value *indexes[] = {
    LLVMValue::jint_constant(0),
    builder()->CreateSub(
      LLVMValue::jint_constant(monitor_count() - 1), index),
  };
  return new SharkMonitor(
    this,
    builder()->CreateGEP(monitors_slots(), indexes, indexes + 2));
}
