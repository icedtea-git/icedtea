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
#include "incls/_sharkTopLevelBlock.cpp.incl"

using namespace llvm;

void SharkTopLevelBlock::scan_for_traps()
{
  // If typeflow found a trap then don't scan past it
  int limit_bci = ciblock()->has_trap() ? ciblock()->trap_bci() : limit();

  // Scan the bytecode for traps that are always hit
  iter()->reset_to_bci(start());
  while (iter()->next_bci() < limit_bci) {
    iter()->next();

    ciField *field;
    ciMethod *method;
    ciInstanceKlass *klass;
    bool will_link;
    bool is_field;

    int index = -1;

    switch (bc()) {
    case Bytecodes::_ldc:
    case Bytecodes::_ldc_w:
      if (!SharkConstant::for_ldc(iter())->is_loaded()) {
        set_trap(
          Deoptimization::make_trap_request(
            Deoptimization::Reason_uninitialized,
            Deoptimization::Action_reinterpret), bci());
        return;
      }
      break;
      
    case Bytecodes::_getfield:
    case Bytecodes::_getstatic:
    case Bytecodes::_putfield:
    case Bytecodes::_putstatic:
      field = iter()->get_field(will_link);
      assert(will_link, "typeflow responsibility");
      is_field = (bc() == Bytecodes::_getfield || bc() == Bytecodes::_putfield);

      // If the bytecode does not match the field then bail out to
      // the interpreter to throw an IncompatibleClassChangeError
      if (is_field == field->is_static()) {
        set_trap(
          Deoptimization::make_trap_request(
            Deoptimization::Reason_unhandled,
            Deoptimization::Action_none), bci());
        return;
      }
      break;

    case Bytecodes::_invokevirtual:
      method = iter()->get_method(will_link);
      assert(will_link, "typeflow responsibility");

      // If this is a non-final invokevirtual then we need to
      // check that its holder is linked, because its vtable
      // won't have been set up otherwise.
      if (!method->is_final_method() && !method->holder()->is_linked()) {
        set_trap(
          Deoptimization::make_trap_request(
            Deoptimization::Reason_uninitialized,
            Deoptimization::Action_reinterpret), bci());
          return;
      }
      break;

    case Bytecodes::_invokeinterface:
      method = iter()->get_method(will_link);
      assert(will_link, "typeflow responsibility");

      // Continue to the check
      index = iter()->get_method_index();
      break;

    case Bytecodes::_new:
      klass = iter()->get_klass(will_link)->as_instance_klass();
      assert(will_link, "typeflow responsibility");

      // Bail out if the class is unloaded
      if (iter()->is_unresolved_klass() || !klass->is_initialized()) {
        set_trap(
          Deoptimization::make_trap_request(
            Deoptimization::Reason_uninitialized,
            Deoptimization::Action_reinterpret), bci());
        return;
      }

      // Bail out if the class cannot be instantiated
      if (klass->is_abstract() || klass->is_interface() ||
          klass->name() == ciSymbol::java_lang_Class()) {
        set_trap(
          Deoptimization::make_trap_request(
            Deoptimization::Reason_unhandled,
            Deoptimization::Action_reinterpret), bci());
        return;
      }
      break;
    }

    // If we found a constant pool access on this bytecode then check it
    if (index != -1) {
      if (!target()->holder()->is_cache_entry_resolved(
             Bytes::swap_u2(index), bc())) {
        set_trap(
          Deoptimization::make_trap_request(
            Deoptimization::Reason_uninitialized,
            Deoptimization::Action_reinterpret), bci());
        return;
      }
    }
  }
  
  // Trap if typeflow trapped (and we didn't before)
  if (ciblock()->has_trap()) {
    set_trap(
      Deoptimization::make_trap_request(
        Deoptimization::Reason_unloaded,
        Deoptimization::Action_reinterpret,
        ciblock()->trap_index()), ciblock()->trap_bci());
    return;
  }
}

SharkState* SharkTopLevelBlock::entry_state()
{
  if (_entry_state == NULL) {
    assert(needs_phis(), "should do");
    _entry_state = new SharkPHIState(this);
  }
  return _entry_state;
}

void SharkTopLevelBlock::add_incoming(SharkState* incoming_state)
{
  if (needs_phis()) {
    ((SharkPHIState *) entry_state())->add_incoming(incoming_state);
  }
  else if (_entry_state == NULL) {
    _entry_state = incoming_state;
  }
  else {
    assert(entry_state()->equal_to(incoming_state), "should be");
  }
}

void SharkTopLevelBlock::enter(SharkTopLevelBlock* predecessor,
                               bool is_exception)
{
  // This block requires phis:
  //  - if it is entered more than once
  //  - if it is an exception handler, because in which
  //    case we assume it's entered more than once.
  //  - if the predecessor will be compiled after this
  //    block, in which case we can't simple propagate
  //    the state forward.
  if (!needs_phis() &&
      (entered() ||
       is_exception ||
       (predecessor && predecessor->index() >= index())))
    _needs_phis = true;

  // Recurse into the tree
  if (!entered()) {
    _entered = true;

    scan_for_traps();
    if (!has_trap()) {
      for (int i = 0; i < num_successors(); i++) {
        successor(i)->enter(this, false);
      }
    }
    for (int i = 0; i < num_exceptions(); i++) {
      exception(i)->enter(this, true);
    }
  }
}
  
void SharkTopLevelBlock::initialize()
{
  char name[28];
  snprintf(name, sizeof(name),
           "bci_%d%s",
           start(), is_backedge_copy() ? "_backedge_copy" : "");
  _entry_block = function()->CreateBlock(name);
}

void SharkTopLevelBlock::emit_IR()
{
  builder()->SetInsertPoint(entry_block());

  // Parse the bytecode
  parse_bytecode(start(), limit());

  // If this block falls through to the next then it won't have been
  // terminated by a bytecode and we have to add the branch ourselves
  if (falls_through() && !has_trap())
    do_branch(ciTypeFlow::FALL_THROUGH);
}

SharkTopLevelBlock* SharkTopLevelBlock::bci_successor(int bci) const
{
  // XXX now with Linear Search Technology (tm)
  for (int i = 0; i < num_successors(); i++) {
    ciTypeFlow::Block *successor = ciblock()->successors()->at(i);
    if (successor->start() == bci)
      return function()->block(successor->pre_order());
  }
  ShouldNotReachHere();
}

void SharkTopLevelBlock::do_zero_check(SharkValue *value)
{
  if (value->is_phi() && value->as_phi()->all_incomers_zero_checked()) {
    function()->add_deferred_zero_check(this, value);
  }
  else {
    BasicBlock *continue_block = function()->CreateBlock("not_zero");
    SharkState *saved_state = current_state();
    set_current_state(saved_state->copy());
    zero_check_value(value, continue_block);
    builder()->SetInsertPoint(continue_block);
    set_current_state(saved_state);
  }

  value->set_zero_checked(true);
}

void SharkTopLevelBlock::do_deferred_zero_check(SharkValue* value,
                                                int         bci,
                                                SharkState* saved_state,
                                                BasicBlock* continue_block)
{
  if (value->as_phi()->all_incomers_zero_checked()) {
    builder()->CreateBr(continue_block);
  }
  else {
    iter()->force_bci(start());
    set_current_state(saved_state);  
    zero_check_value(value, continue_block);
  }
}

void SharkTopLevelBlock::zero_check_value(SharkValue* value,
                                          BasicBlock* continue_block)
{
  BasicBlock *zero_block = builder()->CreateBlock(continue_block, "zero");

  Value *a, *b;
  switch (value->basic_type()) {
  case T_BYTE:
  case T_CHAR:
  case T_SHORT:
  case T_INT:
    a = value->jint_value();
    b = LLVMValue::jint_constant(0);
    break;
  case T_LONG:
    a = value->jlong_value();
    b = LLVMValue::jlong_constant(0);
    break;
  case T_OBJECT:
  case T_ARRAY:
    a = value->jobject_value();
    b = LLVMValue::LLVMValue::null();
    break;
  default:
    tty->print_cr("Unhandled type %s", type2name(value->basic_type()));
    ShouldNotReachHere();
  }

  builder()->CreateCondBr(
    builder()->CreateICmpNE(a, b), continue_block, zero_block);

  builder()->SetInsertPoint(zero_block);
  if (value->is_jobject()) {
    call_vm(
      SharkRuntime::throw_NullPointerException(),
      builder()->pointer_constant(__FILE__),
      LLVMValue::jint_constant(__LINE__),
      EX_CHECK_NONE);
  }
  else {
    builder()->CreateUnimplemented(__FILE__, __LINE__);
  } 
  handle_exception(function()->CreateGetPendingException(), EX_CHECK_FULL);
}

void SharkTopLevelBlock::check_bounds(SharkValue* array, SharkValue* index)
{
  BasicBlock *out_of_bounds = function()->CreateBlock("out_of_bounds");
  BasicBlock *in_bounds     = function()->CreateBlock("in_bounds");

  Value *length = builder()->CreateArrayLength(array->jarray_value());
  // we use an unsigned comparison to catch negative values
  builder()->CreateCondBr(
    builder()->CreateICmpULT(index->jint_value(), length),
    in_bounds, out_of_bounds);

  builder()->SetInsertPoint(out_of_bounds);
  SharkState *saved_state = current_state()->copy();
  call_vm(
    SharkRuntime::throw_ArrayIndexOutOfBoundsException(),
    builder()->pointer_constant(__FILE__),
    LLVMValue::jint_constant(__LINE__),
    index->jint_value(),
    EX_CHECK_NONE);
  handle_exception(function()->CreateGetPendingException(), EX_CHECK_FULL);
  set_current_state(saved_state);  

  builder()->SetInsertPoint(in_bounds);
}

void SharkTopLevelBlock::check_pending_exception(int action)
{
  assert(action & EAM_CHECK, "should be");

  BasicBlock *exception    = function()->CreateBlock("exception");
  BasicBlock *no_exception = function()->CreateBlock("no_exception");

  Value *pending_exception_addr = function()->pending_exception_address();
  Value *pending_exception = builder()->CreateLoad(
    pending_exception_addr, "pending_exception");

  builder()->CreateCondBr(
    builder()->CreateICmpEQ(pending_exception, LLVMValue::null()),
    no_exception, exception);

  builder()->SetInsertPoint(exception);
  builder()->CreateStore(LLVMValue::null(), pending_exception_addr);
  SharkState *saved_state = current_state()->copy();
  if (action & EAM_MONITOR_FUDGE) {
    // The top monitor is marked live, but the exception was thrown
    // while setting it up so we need to mark it dead before we enter
    // any exception handlers as they will not expect it to be there.
    set_num_monitors(num_monitors() - 1);
    action ^= EAM_MONITOR_FUDGE;
  }
  handle_exception(pending_exception, action); 
  set_current_state(saved_state);

  builder()->SetInsertPoint(no_exception);
}

void SharkTopLevelBlock::handle_exception(Value* exception, int action)
{
  if (action & EAM_HANDLE && num_exceptions() != 0) {
    // Clear the stack and push the exception onto it.
    // We do this now to protect it across the VM call
    // we may be about to make.
    while (xstack_depth())
      pop();
    push(SharkValue::create_jobject(exception, true));

    int *indexes = NEW_RESOURCE_ARRAY(int, num_exceptions());
    bool has_catch_all = false;

    ciExceptionHandlerStream eh_iter(target(), bci());
    for (int i = 0; i < num_exceptions(); i++, eh_iter.next()) {
      ciExceptionHandler* handler = eh_iter.handler();

      if (handler->is_catch_all()) {
        assert(i == num_exceptions() - 1, "catch-all should be last");
        has_catch_all = true;
      }
      else {
        indexes[i] = handler->catch_klass_index();
      }
    }

    int num_options = num_exceptions();
    if (has_catch_all)
      num_options--;

    // Drop into the runtime if there are non-catch-all options
    if (num_options > 0) {
      Value *options = builder()->CreateAlloca(
        ArrayType::get(SharkType::jint_type(), num_options),
        LLVMValue::jint_constant(1));

      for (int i = 0; i < num_options; i++)
        builder()->CreateStore(
          LLVMValue::jint_constant(indexes[i]),
          builder()->CreateStructGEP(options, i));

      Value *index = call_vm(
        SharkRuntime::find_exception_handler(),
        builder()->CreateStructGEP(options, 0),
        LLVMValue::jint_constant(num_options),
        EX_CHECK_NO_CATCH);

      // Jump to the exception handler, if found
      BasicBlock *no_handler = function()->CreateBlock("no_handler");
      SwitchInst *switchinst = builder()->CreateSwitch(
        index, no_handler, num_options);

      for (int i = 0; i < num_options; i++) {
        SharkTopLevelBlock* handler = this->exception(i);

        switchinst->addCase(
          LLVMValue::jint_constant(i),
          handler->entry_block());

        handler->add_incoming(current_state());
      }

      builder()->SetInsertPoint(no_handler);
    }

    // No specific handler exists, but maybe there's a catch-all
    if (has_catch_all) {
      SharkTopLevelBlock* handler = this->exception(num_options);

      builder()->CreateBr(handler->entry_block());
      handler->add_incoming(current_state());
      return;
    }
  }

  // No exception handler was found; unwind and return
  handle_return(T_VOID, exception);
}

void SharkTopLevelBlock::maybe_add_safepoint()
{
  if (current_state()->has_safepointed())
    return;

  BasicBlock *orig_block = builder()->GetInsertBlock();
  SharkState *orig_state = current_state()->copy();

  BasicBlock *do_safepoint = function()->CreateBlock("do_safepoint");
  BasicBlock *safepointed  = function()->CreateBlock("safepointed");

  Value *state = builder()->CreateLoad(
    builder()->CreateIntToPtr(
      builder()->pointer_constant(SafepointSynchronize::address_of_state()),
      PointerType::getUnqual(SharkType::jint_type())),
    "state");

  builder()->CreateCondBr(
    builder()->CreateICmpEQ(
      state,
      LLVMValue::jint_constant(SafepointSynchronize::_synchronizing)),
    do_safepoint, safepointed);

  builder()->SetInsertPoint(do_safepoint);
  call_vm(SharkRuntime::safepoint(), EX_CHECK_FULL);
  BasicBlock *safepointed_block = builder()->GetInsertBlock();  
  builder()->CreateBr(safepointed);

  builder()->SetInsertPoint(safepointed);
  current_state()->merge(orig_state, orig_block, safepointed_block);

  current_state()->set_has_safepointed(true);
}

void SharkTopLevelBlock::maybe_add_backedge_safepoint()
{
  if (current_state()->has_safepointed())
    return;

  for (int i = 0; i < num_successors(); i++) {
    if (successor(i)->can_reach(this)) {
      maybe_add_safepoint();
      break;
    }
  }
}

bool SharkTopLevelBlock::can_reach(SharkTopLevelBlock* other)
{
  for (int i = 0; i < function()->block_count(); i++)
    function()->block(i)->_can_reach_visited = false;

  return can_reach_helper(other);
}

bool SharkTopLevelBlock::can_reach_helper(SharkTopLevelBlock* other)
{
  if (this == other)
    return true;

  if (_can_reach_visited)
    return false;
  _can_reach_visited = true;
  
  if (!has_trap()) {
    for (int i = 0; i < num_successors(); i++) {
      if (successor(i)->can_reach_helper(other))
        return true;
    }
  }

  for (int i = 0; i < num_exceptions(); i++) {
    if (exception(i)->can_reach_helper(other))
      return true;
  }

  return false;
}

void SharkTopLevelBlock::do_trap(int trap_request)
{
  current_state()->decache_for_trap();
  builder()->CreateCall2(
    SharkRuntime::uncommon_trap(),
    thread(),
    LLVMValue::jint_constant(trap_request));
  builder()->CreateRetVoid();
}

void SharkTopLevelBlock::call_register_finalizer(Value *receiver)
{
  BasicBlock *orig_block = builder()->GetInsertBlock();
  SharkState *orig_state = current_state()->copy();

  BasicBlock *do_call = function()->CreateBlock("has_finalizer");
  BasicBlock *done    = function()->CreateBlock("done");

  Value *klass = builder()->CreateValueOfStructEntry(
    receiver,
    in_ByteSize(oopDesc::klass_offset_in_bytes()),
    SharkType::jobject_type(),
    "klass");
  
  Value *klass_part = builder()->CreateAddressOfStructEntry(
    klass,
    in_ByteSize(klassOopDesc::klass_part_offset_in_bytes()),
    SharkType::klass_type(),
    "klass_part");

  Value *access_flags = builder()->CreateValueOfStructEntry(
    klass_part,
    in_ByteSize(Klass::access_flags_offset_in_bytes()),
    SharkType::jint_type(),
    "access_flags");

  builder()->CreateCondBr(
    builder()->CreateICmpNE(
      builder()->CreateAnd(
        access_flags,
        LLVMValue::jint_constant(JVM_ACC_HAS_FINALIZER)),
      LLVMValue::jint_constant(0)),
    do_call, done);

  builder()->SetInsertPoint(do_call);
  call_vm(SharkRuntime::register_finalizer(), receiver, EX_CHECK_FULL);
  BasicBlock *branch_block = builder()->GetInsertBlock();  
  builder()->CreateBr(done);

  builder()->SetInsertPoint(done);
  current_state()->merge(orig_state, orig_block, branch_block);
}

void SharkTopLevelBlock::handle_return(BasicType type, Value* exception)
{
  assert (exception == NULL || type == T_VOID, "exception OR result, please");

  if (num_monitors()) {
    // Protect our exception across possible monitor release decaches
    if (exception)
      set_oop_tmp(exception);

    // We don't need to check for exceptions thrown here.  If
    // we're returning a value then we just carry on as normal:
    // the caller will see the pending exception and handle it.
    // If we're returning with an exception then that exception
    // takes priority and the release_lock one will be ignored.
    while (num_monitors())
      release_lock(EX_CHECK_NONE);

    // Reload the exception we're throwing
    if (exception)
      exception = get_oop_tmp();
  }

  if (exception) {
    builder()->CreateStore(exception, function()->pending_exception_address());
  }

  Value *result_addr = function()->CreatePopFrame(type2size[type]);
  if (type != T_VOID) {
    builder()->CreateStore(
      pop_result(type)->generic_value(),
      builder()->CreateIntToPtr(
        result_addr,
        PointerType::getUnqual(SharkType::to_stackType(type))));
  }

  builder()->CreateRetVoid();
}

void SharkTopLevelBlock::do_arraylength()
{
  SharkValue *array = pop();
  check_null(array);
  Value *length = builder()->CreateArrayLength(array->jarray_value());
  push(SharkValue::create_jint(length, false));
}

void SharkTopLevelBlock::do_aload(BasicType basic_type)
{
  SharkValue *index = pop();
  SharkValue *array = pop();

  assert(array->type()->is_array_klass(), "should be");
  ciType *element_type = ((ciArrayKlass *) array->type())->element_type();
  assert((element_type->basic_type() == T_BOOLEAN && basic_type == T_BYTE) ||
         (element_type->basic_type() == T_ARRAY && basic_type == T_OBJECT) ||
         (element_type->basic_type() == basic_type), "type mismatch");

  check_null(array);
  check_bounds(array, index);

  Value *value = builder()->CreateLoad(
    builder()->CreateArrayAddress(
      array->jarray_value(), basic_type, index->jint_value()));

  const Type *stack_type = SharkType::to_stackType(basic_type);
  if (value->getType() != stack_type)
    value = builder()->CreateIntCast(value, stack_type, basic_type != T_CHAR);

  switch (basic_type) {
  case T_BYTE:
  case T_CHAR:
  case T_SHORT:
  case T_INT:
    push(SharkValue::create_jint(value, false));
    break;

  case T_LONG:
    push(SharkValue::create_jlong(value, false));
    break;

  case T_FLOAT:
    push(SharkValue::create_jfloat(value));
    break;

  case T_DOUBLE:
    push(SharkValue::create_jdouble(value));
    break;
    
  case T_OBJECT:
    push(SharkValue::create_generic(element_type, value, false));
    break;

  default:
    tty->print_cr("Unhandled type %s", type2name(basic_type));
    ShouldNotReachHere();
  }
}

void SharkTopLevelBlock::do_astore(BasicType basic_type)
{
  SharkValue *svalue = pop();
  SharkValue *index  = pop();
  SharkValue *array  = pop();

  assert(array->type()->is_array_klass(), "should be");
  ciType *element_type = ((ciArrayKlass *) array->type())->element_type();
  assert((element_type->basic_type() == T_BOOLEAN && basic_type == T_BYTE) ||
         (element_type->basic_type() == T_ARRAY && basic_type == T_OBJECT) ||
         (element_type->basic_type() == basic_type), "type mismatch");

  check_null(array);
  check_bounds(array, index);

  Value *value;
  switch (basic_type) {
  case T_BYTE:
  case T_CHAR:
  case T_SHORT:
  case T_INT:
    value = svalue->jint_value();
    break;

  case T_LONG:
    value = svalue->jlong_value();
    break;

  case T_FLOAT:
    value = svalue->jfloat_value();
    break;

  case T_DOUBLE:
    value = svalue->jdouble_value();
    break;

  case T_OBJECT:
    value = svalue->jobject_value();
    // XXX assignability check
    break;

  default:
    tty->print_cr("Unhandled type %s", type2name(basic_type));
    ShouldNotReachHere();
  }

  const Type *array_type = SharkType::to_arrayType(basic_type);
  if (value->getType() != array_type)
    value = builder()->CreateIntCast(value, array_type, basic_type != T_CHAR);

  Value *addr = builder()->CreateArrayAddress(
    array->jarray_value(), basic_type, index->jint_value(), "addr");

  builder()->CreateStore(value, addr);

  if (!element_type->is_primitive_type())
    builder()->CreateUpdateBarrierSet(oopDesc::bs(), addr);
}

void SharkTopLevelBlock::do_return(BasicType type)
{
  if (target()->intrinsic_id() == vmIntrinsics::_Object_init)
    call_register_finalizer(local(0)->jobject_value());
  maybe_add_safepoint();
  handle_return(type, NULL);
}

void SharkTopLevelBlock::do_athrow()
{
  SharkValue *exception = pop();
  check_null(exception);
  handle_exception(exception->jobject_value(), EX_CHECK_FULL);
}

void SharkTopLevelBlock::do_goto()
{
  do_branch(ciTypeFlow::GOTO_TARGET);
}

void SharkTopLevelBlock::do_jsr()
{
  push(SharkValue::address_constant(iter()->next_bci()));
  do_branch(ciTypeFlow::GOTO_TARGET);
}

void SharkTopLevelBlock::do_ret()
{
  assert(local(iter()->get_index())->address_value() ==
         successor(ciTypeFlow::GOTO_TARGET)->start(), "should be");
  do_branch(ciTypeFlow::GOTO_TARGET);
}

// All propagation of state from one block to the next (via
// dest->add_incoming) is handled by these methods:
//   do_branch
//   do_if_helper
//   do_switch
//   handle_exception

void SharkTopLevelBlock::do_branch(int successor_index)
{
  SharkTopLevelBlock *dest = successor(successor_index);
  builder()->CreateBr(dest->entry_block());
  dest->add_incoming(current_state());
}

void SharkTopLevelBlock::do_if(ICmpInst::Predicate p,
                               SharkValue*         b,
                               SharkValue*         a)
{
  Value *llvm_a, *llvm_b;
  if (a->is_jobject()) {
    llvm_a = a->intptr_value(builder());
    llvm_b = b->intptr_value(builder());
  }
  else {
    llvm_a = a->jint_value();
    llvm_b = b->jint_value();
  }
  do_if_helper(p, llvm_b, llvm_a, current_state(), current_state());
}

void SharkTopLevelBlock::do_if_helper(ICmpInst::Predicate p,
                                      Value*              b,
                                      Value*              a,
                                      SharkState*         if_taken_state,
                                      SharkState*         not_taken_state)
{
  SharkTopLevelBlock *if_taken  = successor(ciTypeFlow::IF_TAKEN);
  SharkTopLevelBlock *not_taken = successor(ciTypeFlow::IF_NOT_TAKEN);

  builder()->CreateCondBr(
    builder()->CreateICmp(p, a, b),
    if_taken->entry_block(), not_taken->entry_block());

  if_taken->add_incoming(if_taken_state);
  not_taken->add_incoming(not_taken_state);
}

void SharkTopLevelBlock::do_switch()
{
  int len = switch_table_length();

  SharkTopLevelBlock *dest_block = successor(ciTypeFlow::SWITCH_DEFAULT);
  SwitchInst *switchinst = builder()->CreateSwitch(
    pop()->jint_value(), dest_block->entry_block(), len);
  dest_block->add_incoming(current_state());

  for (int i = 0; i < len; i++) {
    int dest_bci = switch_dest(i);
    if (dest_bci != switch_default_dest()) {
      dest_block = bci_successor(dest_bci);
      switchinst->addCase(
        LLVMValue::jint_constant(switch_key(i)),
        dest_block->entry_block());
      dest_block->add_incoming(current_state());      
    }
  }
}

// Figure out what type of call this is.
//  - Direct calls are where the callee is fixed.
//  - Interface and Virtual calls require lookup at runtime.
// NB some invokevirtuals can be resolved to direct calls.
SharkTopLevelBlock::CallType SharkTopLevelBlock::get_call_type(ciMethod* method)
{
  if (bc() == Bytecodes::_invokeinterface)
    return CALL_INTERFACE;
  else if (bc() == Bytecodes::_invokevirtual && !method->is_final_method())
    return CALL_VIRTUAL;
  else
    return CALL_DIRECT;
}

Value *SharkTopLevelBlock::get_callee(CallType    call_type,
                                      ciMethod*   method,
                                      SharkValue* receiver)
{
  switch (call_type) {
  case CALL_DIRECT:
    return get_direct_callee(method);
  case CALL_VIRTUAL:
    return get_virtual_callee(receiver, method);
  case CALL_INTERFACE:
    return get_interface_callee(receiver);
  default:
    ShouldNotReachHere();
  } 
}

// Direct calls can be made when the callee is fixed.
// invokestatic and invokespecial are always direct;
// invokevirtual is direct in some circumstances.
Value *SharkTopLevelBlock::get_direct_callee(ciMethod* method)
{
  return builder()->CreateBitCast(
    builder()->CreateInlineOop(method),
    SharkType::methodOop_type(),
    "callee");
}

// Non-direct virtual calls are handled here
Value *SharkTopLevelBlock::get_virtual_callee(SharkValue* receiver,
                                              ciMethod*   method)
{
  Value *klass = builder()->CreateValueOfStructEntry(
    receiver->jobject_value(),
    in_ByteSize(oopDesc::klass_offset_in_bytes()),
    SharkType::jobject_type(),
    "klass");

  return builder()->CreateLoad(
    builder()->CreateArrayAddress(
      klass,
      SharkType::methodOop_type(),
      vtableEntry::size() * wordSize,
      in_ByteSize(instanceKlass::vtable_start_offset() * wordSize),
      LLVMValue::intptr_constant(method->vtable_index())),
    "callee");
}

// Interpreter-style virtual call lookup
Value* SharkTopLevelBlock::get_virtual_callee(Value *cache,
                                              SharkValue *receiver)
{
  BasicBlock *final      = function()->CreateBlock("final");
  BasicBlock *not_final  = function()->CreateBlock("not_final");
  BasicBlock *got_callee = function()->CreateBlock("got_callee");

  Value *flags = builder()->CreateValueOfStructEntry(
    cache, ConstantPoolCacheEntry::flags_offset(),
    SharkType::intptr_type(),
    "flags");

  const int mask = 1 << ConstantPoolCacheEntry::vfinalMethod;
  builder()->CreateCondBr(
    builder()->CreateICmpNE(
      builder()->CreateAnd(flags, LLVMValue::intptr_constant(mask)),
      LLVMValue::intptr_constant(0)),
    final, not_final);

  // For final methods f2 is the actual address of the method
  builder()->SetInsertPoint(final);
  Value *final_callee = builder()->CreateValueOfStructEntry(
    cache, ConstantPoolCacheEntry::f2_offset(),
    SharkType::methodOop_type(),
    "final_callee");
  builder()->CreateBr(got_callee);

  // For non-final methods f2 is the index into the vtable
  builder()->SetInsertPoint(not_final);
  Value *klass = builder()->CreateValueOfStructEntry(
    receiver->jobject_value(),
    in_ByteSize(oopDesc::klass_offset_in_bytes()),
    SharkType::jobject_type(),
    "klass");

  Value *index = builder()->CreateValueOfStructEntry(
    cache, ConstantPoolCacheEntry::f2_offset(),
    SharkType::intptr_type(),
    "index");

  Value *nonfinal_callee = builder()->CreateLoad(
    builder()->CreateArrayAddress(
      klass,
      SharkType::methodOop_type(),
      vtableEntry::size() * wordSize,
      in_ByteSize(instanceKlass::vtable_start_offset() * wordSize),
      index),
    "nonfinal_callee");
  builder()->CreateBr(got_callee);

  builder()->SetInsertPoint(got_callee);
  PHINode *callee = builder()->CreatePHI(
    SharkType::methodOop_type(), "callee");
  callee->addIncoming(final_callee, final);
  callee->addIncoming(nonfinal_callee, not_final);

  return callee;
}

// Interpreter-style interface call lookup
Value* SharkTopLevelBlock::get_interface_callee(SharkValue *receiver)
{
  SharkConstantPool constants(this);
  Value *cache = constants.cache_entry_at(iter()->get_method_index());

  BasicBlock *hacky      = function()->CreateBlock("hacky");
  BasicBlock *normal     = function()->CreateBlock("normal");
  BasicBlock *loop       = function()->CreateBlock("loop");
  BasicBlock *got_null   = function()->CreateBlock("got_null");
  BasicBlock *not_null   = function()->CreateBlock("not_null");
  BasicBlock *next       = function()->CreateBlock("next");
  BasicBlock *got_entry  = function()->CreateBlock("got_entry");
  BasicBlock *got_callee = function()->CreateBlock("got_callee");

  Value *flags = builder()->CreateValueOfStructEntry(
    cache, ConstantPoolCacheEntry::flags_offset(),
    SharkType::intptr_type(),
    "flags");

  const int mask = 1 << ConstantPoolCacheEntry::methodInterface;
  builder()->CreateCondBr(
    builder()->CreateICmpNE(
      builder()->CreateAnd(flags, LLVMValue::intptr_constant(mask)),
      LLVMValue::intptr_constant(0)),
    hacky, normal);

  // Workaround for the case where we encounter an invokeinterface,
  // but should really have an invokevirtual since the resolved
  // method is a virtual method in java.lang.Object. This is a
  // corner case in the spec but is presumably legal, and while
  // javac does not generate this code there's no reason it could
  // not be produced by a compliant java compiler.  See
  // cpCacheOop.cpp for more details.
  builder()->SetInsertPoint(hacky);
  Value *hacky_callee = get_virtual_callee(cache, receiver);
  BasicBlock *got_hacky = builder()->GetInsertBlock();
  builder()->CreateBr(got_callee);

  // Locate the receiver's itable
  builder()->SetInsertPoint(normal);
  Value *object_klass = builder()->CreateValueOfStructEntry(
    receiver->jobject_value(), in_ByteSize(oopDesc::klass_offset_in_bytes()),
    SharkType::jobject_type(),
    "object_klass");

  Value *vtable_start = builder()->CreateAdd(
    builder()->CreatePtrToInt(object_klass, SharkType::intptr_type()),
    LLVMValue::intptr_constant(
      instanceKlass::vtable_start_offset() * HeapWordSize),
    "vtable_start");

  Value *vtable_length = builder()->CreateValueOfStructEntry(
    object_klass,
    in_ByteSize(instanceKlass::vtable_length_offset() * HeapWordSize),
    SharkType::jint_type(),
    "vtable_length");
  vtable_length =
    builder()->CreateIntCast(vtable_length, SharkType::intptr_type(), false);

  bool needs_aligning = HeapWordsPerLong > 1;
  const char *itable_start_name = "itable_start";
  Value *itable_start = builder()->CreateAdd(
    vtable_start,
    builder()->CreateShl(
      vtable_length,
      LLVMValue::intptr_constant(exact_log2(vtableEntry::size() * wordSize))),
    needs_aligning ? "" : itable_start_name);
  if (needs_aligning)
    itable_start = builder()->CreateAlign(
      itable_start, BytesPerLong, itable_start_name);

  // Locate this interface's entry in the table
  Value *iklass = builder()->CreateValueOfStructEntry(
    cache, ConstantPoolCacheEntry::f1_offset(),
    SharkType::jobject_type(),
    "iklass");

  builder()->CreateBr(loop);
  builder()->SetInsertPoint(loop);
  PHINode *itable_entry_addr = builder()->CreatePHI(
    SharkType::intptr_type(), "itable_entry_addr");
  itable_entry_addr->addIncoming(itable_start, normal);

  Value *itable_entry = builder()->CreateIntToPtr(
    itable_entry_addr, SharkType::itableOffsetEntry_type(), "itable_entry");

  Value *itable_iklass = builder()->CreateValueOfStructEntry(
    itable_entry,
    in_ByteSize(itableOffsetEntry::interface_offset_in_bytes()),
    SharkType::jobject_type(),
    "itable_iklass");

  builder()->CreateCondBr(
    builder()->CreateICmpEQ(itable_iklass, LLVMValue::null()),
    got_null, not_null);

  // A null entry means that the class doesn't implement the
  // interface, and wasn't the same as the class checked when
  // the interface was resolved.
  builder()->SetInsertPoint(got_null);
  builder()->CreateUnimplemented(__FILE__, __LINE__);
  builder()->CreateUnreachable();
                          
  builder()->SetInsertPoint(not_null);
  builder()->CreateCondBr(
    builder()->CreateICmpEQ(itable_iklass, iklass),
    got_entry, next);

  builder()->SetInsertPoint(next);
  Value *next_entry = builder()->CreateAdd(
    itable_entry_addr,
    LLVMValue::intptr_constant(itableOffsetEntry::size() * wordSize));
  builder()->CreateBr(loop);
  itable_entry_addr->addIncoming(next_entry, next);

  // Locate the method pointer
  builder()->SetInsertPoint(got_entry);
  Value *offset = builder()->CreateValueOfStructEntry(
    itable_entry,
    in_ByteSize(itableOffsetEntry::offset_offset_in_bytes()),
    SharkType::jint_type(),
    "offset");
  offset =
    builder()->CreateIntCast(offset, SharkType::intptr_type(), false);

  Value *index = builder()->CreateValueOfStructEntry(
    cache, ConstantPoolCacheEntry::f2_offset(),
    SharkType::intptr_type(),
    "index");

  Value *normal_callee = builder()->CreateLoad(
    builder()->CreateIntToPtr(
      builder()->CreateAdd(
        builder()->CreateAdd(
          builder()->CreateAdd(
            builder()->CreatePtrToInt(
              object_klass, SharkType::intptr_type()),
            offset),
          builder()->CreateShl(
            index,
            LLVMValue::intptr_constant(
              exact_log2(itableMethodEntry::size() * wordSize)))),
        LLVMValue::intptr_constant(
          itableMethodEntry::method_offset_in_bytes())),
      PointerType::getUnqual(SharkType::methodOop_type())),
    "normal_callee");
  BasicBlock *got_normal = builder()->GetInsertBlock();
  builder()->CreateBr(got_callee);

  builder()->SetInsertPoint(got_callee);
  PHINode *callee = builder()->CreatePHI(
    SharkType::methodOop_type(), "callee");
  callee->addIncoming(hacky_callee, got_hacky);
  callee->addIncoming(normal_callee, got_normal);

  return callee;
} 

void SharkTopLevelBlock::do_call()
{
  bool will_link;
  ciMethod *method = iter()->get_method(will_link);
  assert(will_link, "typeflow responsibility");

  // Figure out what type of call this is
  CallType call_type = get_call_type(method);

  // Find the receiver in the stack.  We do this before
  // trying to inline because the inliner can only use
  // zero-checked values, not being able to perform the
  // check itself.
  SharkValue *receiver = NULL;
  if (bc() != Bytecodes::_invokestatic) {
    receiver = xstack(method->arg_size() - 1);
    check_null(receiver);
  }

  // Try to inline the call
  if (call_type == CALL_DIRECT) {
    if (SharkInliner::attempt_inline(method, current_state(), thread()))
      return;
  }

  // Find the method we are calling
  Value *callee = get_callee(call_type, method, receiver);

  // Load the SharkEntry from the callee
  Value *base_pc = builder()->CreateValueOfStructEntry(
    callee, methodOopDesc::from_interpreted_offset(),
    SharkType::intptr_type(),
    "base_pc");

  // Load the entry point from the SharkEntry
  Value *entry_point = builder()->CreateLoad(
    builder()->CreateIntToPtr(
      builder()->CreateAdd(
        base_pc,
        LLVMValue::intptr_constant(in_bytes(ZeroEntry::entry_point_offset()))),
      PointerType::getUnqual(
        PointerType::getUnqual(SharkType::entry_point_type()))),
    "entry_point");

  // Make the call
  current_state()->decache_for_Java_call(method);
  builder()->CreateCall3(entry_point, callee, base_pc, thread());
  current_state()->cache_after_Java_call(method);

  // Check for pending exceptions
  check_pending_exception(EX_CHECK_FULL);

  // Mark that a safepoint check has occurred
  current_state()->set_has_safepointed(true);
}

bool SharkTopLevelBlock::static_subtype_check(ciKlass* check_klass,
                                              ciKlass* object_klass)
{
  // If the class we're checking against is java.lang.Object
  // then this is a no brainer.  Apparently this can happen
  // in reflective code...
  if (check_klass == function()->env()->Object_klass())
    return true;

  // Perform a subtype check.  NB in opto's code for this
  // (GraphKit::static_subtype_check) it says that static
  // interface types cannot be trusted, and if opto can't
  // trust them then I assume we can't either.
  if (!object_klass->is_interface()) {
    if (object_klass == check_klass)
      return true;

    if (object_klass->is_loaded() && check_klass->is_loaded()) {
      if (object_klass->is_subtype_of(check_klass))
        return true;
    }
  }

  return false;
}
  
void SharkTopLevelBlock::do_instance_check()
{
  // Get the class we're checking against
  bool will_link;
  ciKlass *check_klass = iter()->get_klass(will_link);

  // Get the class of the object we're checking
  ciKlass *object_klass = xstack(0)->type()->as_klass();

  // Can we optimize this check away?
  if (static_subtype_check(check_klass, object_klass)) {
    if (bc() == Bytecodes::_instanceof) {
      pop();
      push(SharkValue::jint_constant(1));
    }
    return;
  }

  // Need to check this one at runtime
  if (will_link)
    do_full_instance_check(check_klass);
  else
    do_trapping_instance_check(check_klass);
}
                
bool SharkTopLevelBlock::maybe_do_instanceof_if()
{
  // Get the class we're checking against
  bool will_link;
  ciKlass *check_klass = iter()->get_klass(will_link);

  // If the class is unloaded then the instanceof
  // cannot possibly succeed.
  if (!will_link)
    return false;

  // Keep a copy of the object we're checking
  SharkValue *old_object = xstack(0);
  
  // Get the class of the object we're checking
  ciKlass *object_klass = old_object->type()->as_klass();

  // If the instanceof can be optimized away at compile time
  // then any subsequent checkcasts will be too so we handle
  // it normally.
  if (static_subtype_check(check_klass, object_klass))
    return false;

  // Perform the instance check
  do_full_instance_check(check_klass);
  Value *result = pop()->jint_value();

  // Create the casted object
  SharkValue *new_object = SharkValue::create_generic(
    check_klass, old_object->jobject_value(), old_object->zero_checked());

  // Create two copies of the current state, one with the
  // original object and one with all instances of the
  // original object replaced with the new, casted object.
  SharkState *new_state = current_state();
  SharkState *old_state = new_state->copy();
  new_state->replace_all(old_object, new_object);

  // Perform the check-and-branch
  switch (iter()->next_bc()) {
  case Bytecodes::_ifeq:
    // branch if not an instance
    do_if_helper(
      ICmpInst::ICMP_EQ,
      LLVMValue::jint_constant(0), result,
      old_state, new_state);
    break;

  case Bytecodes::_ifne:
    // branch if an instance
    do_if_helper(
      ICmpInst::ICMP_NE,
      LLVMValue::jint_constant(0), result,
      new_state, old_state);
    break;

  default:
    ShouldNotReachHere();
  }

  return true;
}

void SharkTopLevelBlock::do_full_instance_check(ciKlass* klass)
{ 
  BasicBlock *not_null      = function()->CreateBlock("not_null");
  BasicBlock *subtype_check = function()->CreateBlock("subtype_check");
  BasicBlock *is_instance   = function()->CreateBlock("is_instance");
  BasicBlock *not_instance  = function()->CreateBlock("not_instance");
  BasicBlock *merge1        = function()->CreateBlock("merge1");
  BasicBlock *merge2        = function()->CreateBlock("merge2");

  enum InstanceCheckStates {
    IC_IS_NULL,
    IC_IS_INSTANCE,
    IC_NOT_INSTANCE,
  };

  // Pop the object off the stack
  Value *object = pop()->jobject_value();
  
  // Null objects aren't instances of anything
  builder()->CreateCondBr(
    builder()->CreateICmpEQ(object, LLVMValue::null()),
    merge2, not_null);
  BasicBlock *null_block = builder()->GetInsertBlock();

  // Get the class we're checking against
  builder()->SetInsertPoint(not_null);
  Value *check_klass = builder()->CreateInlineOop(klass);

  // Get the class of the object being tested
  Value *object_klass = builder()->CreateValueOfStructEntry(
    object, in_ByteSize(oopDesc::klass_offset_in_bytes()),
    SharkType::jobject_type(),
    "object_klass");

  // Perform the check
  builder()->CreateCondBr(
    builder()->CreateICmpEQ(check_klass, object_klass),
    is_instance, subtype_check);

  builder()->SetInsertPoint(subtype_check);
  builder()->CreateCondBr(
    builder()->CreateICmpNE(
      builder()->CreateCall2(
        SharkRuntime::is_subtype_of(), check_klass, object_klass),
      LLVMValue::jbyte_constant(0)),
    is_instance, not_instance);

  builder()->SetInsertPoint(is_instance);
  builder()->CreateBr(merge1);

  builder()->SetInsertPoint(not_instance);
  builder()->CreateBr(merge1);

  // First merge
  builder()->SetInsertPoint(merge1);
  PHINode *nonnull_result = builder()->CreatePHI(
    SharkType::jint_type(), "nonnull_result");
  nonnull_result->addIncoming(
    LLVMValue::jint_constant(IC_IS_INSTANCE), is_instance);
  nonnull_result->addIncoming(
    LLVMValue::jint_constant(IC_NOT_INSTANCE), not_instance);
  BasicBlock *nonnull_block = builder()->GetInsertBlock();
  builder()->CreateBr(merge2);
  
  // Second merge
  builder()->SetInsertPoint(merge2);
  PHINode *result = builder()->CreatePHI(
    SharkType::jint_type(), "result");
  result->addIncoming(LLVMValue::jint_constant(IC_IS_NULL), null_block);
  result->addIncoming(nonnull_result, nonnull_block);

  // Handle the result
  if (bc() == Bytecodes::_checkcast) {
    BasicBlock *failure = function()->CreateBlock("failure");
    BasicBlock *success = function()->CreateBlock("success");

    builder()->CreateCondBr(
      builder()->CreateICmpNE(
        result, LLVMValue::jint_constant(IC_NOT_INSTANCE)),
      success, failure);

    builder()->SetInsertPoint(failure);
    builder()->CreateUnimplemented(__FILE__, __LINE__);
    builder()->CreateUnreachable();

    builder()->SetInsertPoint(success);
    push(SharkValue::create_generic(klass, object, false));
  }
  else {
    push(
      SharkValue::create_jint(
        builder()->CreateIntCast(
          builder()->CreateICmpEQ(
            result, LLVMValue::jint_constant(IC_IS_INSTANCE)),
          SharkType::jint_type(), false), false));
  }
}

void SharkTopLevelBlock::do_trapping_instance_check(ciKlass* klass)
{
  BasicBlock *not_null = function()->CreateBlock("not_null");
  BasicBlock *is_null  = function()->CreateBlock("null");

  // Leave the object on the stack so it's there if we trap
  builder()->CreateCondBr(
    builder()->CreateICmpEQ(xstack(0)->jobject_value(), LLVMValue::null()),
    is_null, not_null);
  SharkState *saved_state = current_state()->copy();

  // If it's not null then we need to trap
  builder()->SetInsertPoint(not_null);
  set_current_state(saved_state->copy());
  do_trap(
    Deoptimization::make_trap_request(
      Deoptimization::Reason_uninitialized,
      Deoptimization::Action_reinterpret));

  // If it's null then we're ok
  builder()->SetInsertPoint(is_null);
  set_current_state(saved_state);
  if (bc() == Bytecodes::_checkcast) {
    push(SharkValue::create_generic(klass, pop()->jobject_value(), false));    
  }
  else {
    pop();
    push(SharkValue::jint_constant(0));
  }
}

void SharkTopLevelBlock::do_new()
{
  bool will_link;
  ciInstanceKlass* klass = iter()->get_klass(will_link)->as_instance_klass();
  assert(will_link, "typeflow responsibility");

  BasicBlock *got_tlab            = NULL;
  BasicBlock *heap_alloc          = NULL;
  BasicBlock *retry               = NULL;
  BasicBlock *got_heap            = NULL;
  BasicBlock *initialize          = NULL;
  BasicBlock *got_fast            = NULL;
  BasicBlock *slow_alloc_and_init = NULL;
  BasicBlock *got_slow            = NULL;
  BasicBlock *push_object         = NULL;

  SharkState *fast_state = NULL;
  
  Value *tlab_object = NULL;
  Value *heap_object = NULL;
  Value *fast_object = NULL;
  Value *slow_object = NULL;
  Value *object      = NULL;

  // The fast path
  if (!Klass::layout_helper_needs_slow_path(klass->layout_helper())) {
    if (UseTLAB) {
      got_tlab          = function()->CreateBlock("got_tlab");
      heap_alloc        = function()->CreateBlock("heap_alloc");
    }
    retry               = function()->CreateBlock("retry");
    got_heap            = function()->CreateBlock("got_heap");
    initialize          = function()->CreateBlock("initialize");
    slow_alloc_and_init = function()->CreateBlock("slow_alloc_and_init");
    push_object         = function()->CreateBlock("push_object");

    size_t size_in_bytes = klass->size_helper() << LogHeapWordSize;

    // Thread local allocation
    if (UseTLAB) {
      Value *top_addr = builder()->CreateAddressOfStructEntry(
        thread(), Thread::tlab_top_offset(),
        PointerType::getUnqual(SharkType::intptr_type()),
        "top_addr");

      Value *end = builder()->CreateValueOfStructEntry(
        thread(), Thread::tlab_end_offset(),
        SharkType::intptr_type(),
        "end");

      Value *old_top = builder()->CreateLoad(top_addr, "old_top");
      Value *new_top = builder()->CreateAdd(
        old_top, LLVMValue::intptr_constant(size_in_bytes));

      builder()->CreateCondBr(
        builder()->CreateICmpULE(new_top, end),
        got_tlab, heap_alloc);

      builder()->SetInsertPoint(got_tlab);
      tlab_object = builder()->CreateIntToPtr(
        old_top, SharkType::jobject_type(), "tlab_object");

      builder()->CreateStore(new_top, top_addr);
      builder()->CreateBr(initialize);

      builder()->SetInsertPoint(heap_alloc);
    }

    // Heap allocation
    Value *top_addr = builder()->CreateIntToPtr(
	builder()->pointer_constant(Universe::heap()->top_addr()),
      PointerType::getUnqual(SharkType::intptr_type()),
      "top_addr");

    Value *end = builder()->CreateLoad(
      builder()->CreateIntToPtr(
        builder()->pointer_constant(Universe::heap()->end_addr()),
        PointerType::getUnqual(SharkType::intptr_type())),
      "end");

    builder()->CreateBr(retry);
    builder()->SetInsertPoint(retry);

    Value *old_top = builder()->CreateLoad(top_addr, "top");
    Value *new_top = builder()->CreateAdd(
      old_top, LLVMValue::intptr_constant(size_in_bytes));

    builder()->CreateCondBr(
      builder()->CreateICmpULE(new_top, end),
      got_heap, slow_alloc_and_init);

    builder()->SetInsertPoint(got_heap);
    heap_object = builder()->CreateIntToPtr(
      old_top, SharkType::jobject_type(), "heap_object");

    Value *check = builder()->CreateCmpxchgPtr(new_top, top_addr, old_top);
    builder()->CreateCondBr(
      builder()->CreateICmpEQ(old_top, check),
      initialize, retry);

    // Initialize the object
    builder()->SetInsertPoint(initialize);
    if (tlab_object) {
      PHINode *phi = builder()->CreatePHI(
        SharkType::jobject_type(), "fast_object");
      phi->addIncoming(tlab_object, got_tlab);
      phi->addIncoming(heap_object, got_heap);
      fast_object = phi;
    }
    else {
      fast_object = heap_object;
    }

    builder()->CreateMemset(
      builder()->CreateBitCast(
        fast_object, PointerType::getUnqual(SharkType::jbyte_type())),
      LLVMValue::jbyte_constant(0),
      LLVMValue::jint_constant(size_in_bytes),
      LLVMValue::jint_constant(HeapWordSize));

    Value *mark_addr = builder()->CreateAddressOfStructEntry(
      fast_object, in_ByteSize(oopDesc::mark_offset_in_bytes()),
      PointerType::getUnqual(SharkType::intptr_type()),
      "mark_addr");

    Value *klass_addr = builder()->CreateAddressOfStructEntry(
      fast_object, in_ByteSize(oopDesc::klass_offset_in_bytes()),
      PointerType::getUnqual(SharkType::jobject_type()),
      "klass_addr");

    // Set the mark
    intptr_t mark;
    if (UseBiasedLocking) {
      Unimplemented();
    }
    else {
      mark = (intptr_t) markOopDesc::prototype();
    }
    builder()->CreateStore(LLVMValue::intptr_constant(mark), mark_addr);

    // Set the class
    Value *rtklass = builder()->CreateInlineOop(klass);
    builder()->CreateStore(rtklass, klass_addr);
    got_fast = builder()->GetInsertBlock();

    builder()->CreateBr(push_object);
    builder()->SetInsertPoint(slow_alloc_and_init);
    fast_state = current_state()->copy();
  }

  // The slow path
  call_vm(
    SharkRuntime::new_instance(),
    LLVMValue::jint_constant(iter()->get_klass_index()),
    EX_CHECK_FULL);
  slow_object = function()->CreateGetVMResult();
  got_slow = builder()->GetInsertBlock();

  // Push the object
  if (push_object) {
    builder()->CreateBr(push_object);
    builder()->SetInsertPoint(push_object);
  }
  if (fast_object) {
    PHINode *phi = builder()->CreatePHI(SharkType::jobject_type(), "object");
    phi->addIncoming(fast_object, got_fast);
    phi->addIncoming(slow_object, got_slow);
    object = phi;
    current_state()->merge(fast_state, got_fast, got_slow);
  }
  else {
    object = slow_object;
  }

  push(SharkValue::create_jobject(object, true));
}

void SharkTopLevelBlock::do_newarray()
{
  BasicType type = (BasicType) iter()->get_index();

  call_vm(
    SharkRuntime::newarray(),
    LLVMValue::jint_constant(type),
    pop()->jint_value(),
    EX_CHECK_FULL);

  push(SharkValue::create_generic(
    ciArrayKlass::make(ciType::make(type)),
    function()->CreateGetVMResult(),
    true));
}

void SharkTopLevelBlock::do_anewarray()
{
  bool will_link;
  ciKlass *klass = iter()->get_klass(will_link);
  assert(will_link, "typeflow responsibility");

  ciObjArrayKlass *array_klass = ciObjArrayKlass::make(klass);
  if (!array_klass->is_loaded()) {
    Unimplemented();
  }

  call_vm(
    SharkRuntime::anewarray(),
    LLVMValue::jint_constant(iter()->get_klass_index()),
    pop()->jint_value(),
    EX_CHECK_FULL);

  push(SharkValue::create_generic(
    array_klass, function()->CreateGetVMResult(), true));
}

void SharkTopLevelBlock::do_multianewarray()
{
  bool will_link;
  ciArrayKlass *array_klass = iter()->get_klass(will_link)->as_array_klass();
  assert(will_link, "typeflow responsibility");

  // The dimensions are stack values, so we use their slots for the
  // dimensions array.  Note that we are storing them in the reverse
  // of normal stack order.
  int ndims = iter()->get_dimensions();

  Value *dimensions = function()->CreateAddressOfFrameEntry(
    function()->stack_slots_offset() + max_stack() - xstack_depth(),
    ArrayType::get(SharkType::jint_type(), ndims),
    "dimensions");

  for (int i = 0; i < ndims; i++) {
    builder()->CreateStore(
      xstack(ndims - 1 - i)->jint_value(),
      builder()->CreateStructGEP(dimensions, i));
  }

  call_vm(
    SharkRuntime::multianewarray(),
    LLVMValue::jint_constant(iter()->get_klass_index()),
    LLVMValue::jint_constant(ndims),
    builder()->CreateStructGEP(dimensions, 0),
    EX_CHECK_FULL);

  // Now we can pop the dimensions off the stack
  for (int i = 0; i < ndims; i++)
    pop();

  push(SharkValue::create_generic(
    array_klass, function()->CreateGetVMResult(), true));
}

void SharkTopLevelBlock::acquire_method_lock()
{
  Value *lockee;
  if (target()->is_static())
    lockee = builder()->CreateInlineOop(target()->holder()->java_mirror());
  else
    lockee = local(0)->jobject_value();

  iter()->force_bci(start()); // for the decache in acquire_lock
  acquire_lock(lockee, EX_CHECK_NO_CATCH);
}

void SharkTopLevelBlock::do_monitorenter()
{
  SharkValue *lockee = pop();
  check_null(lockee);
  acquire_lock(lockee->jobject_value(), EX_CHECK_FULL);
}

void SharkTopLevelBlock::do_monitorexit()
{
  pop(); // don't need this (monitors are block structured)
  release_lock(EX_CHECK_FULL);
}

void SharkTopLevelBlock::acquire_lock(Value *lockee, int exception_action)
{
  BasicBlock *try_recursive = function()->CreateBlock("try_recursive");
  BasicBlock *got_recursive = function()->CreateBlock("got_recursive");
  BasicBlock *not_recursive = function()->CreateBlock("not_recursive");
  BasicBlock *acquired_fast = function()->CreateBlock("acquired_fast");
  BasicBlock *lock_acquired = function()->CreateBlock("lock_acquired");

  int monitor = num_monitors();
  Value *monitor_addr        = function()->monitor_addr(monitor);
  Value *monitor_object_addr = function()->monitor_object_addr(monitor);
  Value *monitor_header_addr = function()->monitor_header_addr(monitor);

  // Store the object and mark the slot as live
  builder()->CreateStore(lockee, monitor_object_addr);
  set_num_monitors(monitor + 1);

  // Try a simple lock
  Value *mark_addr = builder()->CreateAddressOfStructEntry(
    lockee, in_ByteSize(oopDesc::mark_offset_in_bytes()),
    PointerType::getUnqual(SharkType::intptr_type()),
    "mark_addr");

  Value *mark = builder()->CreateLoad(mark_addr, "mark");
  Value *disp = builder()->CreateOr(
    mark, LLVMValue::intptr_constant(markOopDesc::unlocked_value), "disp");
  builder()->CreateStore(disp, monitor_header_addr);

  Value *lock = builder()->CreatePtrToInt(
    monitor_header_addr, SharkType::intptr_type());
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
  builder()->CreateStore(LLVMValue::intptr_constant(0), monitor_header_addr);
  builder()->CreateBr(acquired_fast);

  // Create an edge for the state merge
  builder()->SetInsertPoint(acquired_fast);
  SharkState *fast_state = current_state()->copy();
  builder()->CreateBr(lock_acquired);

  // It's not a recursive case so we need to drop into the runtime
  builder()->SetInsertPoint(not_recursive);
  call_vm(
    SharkRuntime::monitorenter(), monitor_addr,
    exception_action | EAM_MONITOR_FUDGE);
  BasicBlock *acquired_slow = builder()->GetInsertBlock();
  builder()->CreateBr(lock_acquired);  

  // All done
  builder()->SetInsertPoint(lock_acquired);
  current_state()->merge(fast_state, acquired_fast, acquired_slow);
}

void SharkTopLevelBlock::release_lock(int exception_action)
{
  BasicBlock *not_recursive = function()->CreateBlock("not_recursive");
  BasicBlock *released_fast = function()->CreateBlock("released_fast");
  BasicBlock *slow_path     = function()->CreateBlock("slow_path");
  BasicBlock *lock_released = function()->CreateBlock("lock_released");

  int monitor = num_monitors() - 1;
  Value *monitor_addr        = function()->monitor_addr(monitor);
  Value *monitor_object_addr = function()->monitor_object_addr(monitor);
  Value *monitor_header_addr = function()->monitor_header_addr(monitor);

  // If it is recursive then we're already done
  Value *disp = builder()->CreateLoad(monitor_header_addr);
  builder()->CreateCondBr(
    builder()->CreateICmpEQ(disp, LLVMValue::intptr_constant(0)),
    released_fast, not_recursive);

  // Try a simple unlock
  builder()->SetInsertPoint(not_recursive);

  Value *lock = builder()->CreatePtrToInt(
    monitor_header_addr, SharkType::intptr_type());

  Value *lockee = builder()->CreateLoad(monitor_object_addr);

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
  SharkState *fast_state = current_state()->copy();
  builder()->CreateBr(lock_released);  

  // Need to drop into the runtime to release this one
  builder()->SetInsertPoint(slow_path);
  call_vm(SharkRuntime::monitorexit(), monitor_addr, exception_action);
  BasicBlock *released_slow = builder()->GetInsertBlock();
  builder()->CreateBr(lock_released);  

  // All done
  builder()->SetInsertPoint(lock_released);
  current_state()->merge(fast_state, released_fast, released_slow);

  // The object slot is now dead
  set_num_monitors(monitor);
}
