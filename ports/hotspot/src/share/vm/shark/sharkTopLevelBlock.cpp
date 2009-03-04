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
#include "ciArrayKlass.hpp" // XXX fuck you makeDeps
#include "ciObjArrayKlass.hpp" // XXX likewise

using namespace llvm;

class SharkPHIState : public SharkState {
 public:
  SharkPHIState(SharkTopLevelBlock* block)
    : SharkState(block, block->function())
  {
    BasicBlock *saved_insert_point = builder()->GetInsertBlock();
    builder()->SetInsertPoint(block->entry_block());
    char name[18];
  
    // Method
    set_method(builder()->CreatePHI(SharkType::methodOop_type(), "method"));
  
    // Local variables
    for (int i = 0; i < max_locals(); i++) {
      ciType *type = block->local_type_at_entry(i);
      if (type->basic_type() == (BasicType) ciTypeFlow::StateVector::T_NULL) {
        // XXX we could do all kinds of clever stuff here
        type = ciType::make(T_OBJECT); // XXX what about T_ARRAY?
      }
  
      SharkValue *value = NULL;
      switch (type->basic_type()) {
      case T_INT:
      case T_LONG:
      case T_FLOAT:
      case T_DOUBLE:
      case T_OBJECT:
      case T_ARRAY:
        snprintf(name, sizeof(name), "local_%d_", i);
        value = SharkValue::create_generic(
          type, builder()->CreatePHI(SharkType::to_stackType(type), name));
        break;
  
      case T_ADDRESS:
        value = SharkValue::create_returnAddress(
          type->as_return_address()->bci());
        break;
  
      case ciTypeFlow::StateVector::T_BOTTOM:
        break;
  
      case ciTypeFlow::StateVector::T_LONG2:
      case ciTypeFlow::StateVector::T_DOUBLE2:
        break;
  
      default:
        ShouldNotReachHere();
      }
      set_local(i, value);
    }
  
    // Expression stack
    for (int i = 0; i < block->stack_depth_at_entry(); i++) {
      ciType *type = block->stack_type_at_entry(i);
      if (type->basic_type() == (BasicType) ciTypeFlow::StateVector::T_NULL) {
        // XXX we could do all kinds of clever stuff here
        type = ciType::make(T_OBJECT); // XXX what about T_ARRAY?
      }
  
      SharkValue *value = NULL;
      switch (type->basic_type()) {
      case T_INT:
      case T_LONG:
      case T_FLOAT:
      case T_DOUBLE:
      case T_OBJECT:
      case T_ARRAY:
        snprintf(name, sizeof(name), "stack_%d_", i);
        value = SharkValue::create_generic(
          type, builder()->CreatePHI(SharkType::to_stackType(type), name));
        break;
  
      case T_ADDRESS:
        value = SharkValue::create_returnAddress(
          type->as_return_address()->bci());
        break;
  
      case ciTypeFlow::StateVector::T_LONG2:
      case ciTypeFlow::StateVector::T_DOUBLE2:
        break;
  
      default:
        ShouldNotReachHere();
      }
      push(value);
    }
  
    builder()->SetInsertPoint(saved_insert_point);    
  }

 public:
  void add_incoming(SharkState* incoming_state)
  {
    BasicBlock *predecessor = builder()->GetInsertBlock();
      
    // Method
    ((PHINode *) method())->addIncoming(incoming_state->method(), predecessor);
  
    // Local variables
    for (int i = 0; i < max_locals(); i++) {
      if (local(i) != NULL)
        local(i)->addIncoming(incoming_state->local(i), predecessor);
    }
  
    // Expression stack
    int stack_depth = ((SharkTopLevelBlock *) block())->stack_depth_at_entry();
    assert(stack_depth == incoming_state->stack_depth(), "should be");
    for (int i = 0; i < stack_depth; i++) {
      assert((stack(i) == NULL) == (incoming_state->stack(i) == NULL), "oops");
      if (stack(i))
        stack(i)->addIncoming(incoming_state->stack(i), predecessor);
    }    
  }
};

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
  else if (_entry_state != incoming_state) {
    assert(_entry_state == NULL, "should be");
    _entry_state = incoming_state;
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

    if (!has_trap()) {
      for (int i = 0; i < num_successors(); i++) {
        successor(i)->enter(this, false);
      }
      for (int i = 0; i < num_exceptions(); i++) {
        exception(i)->enter(this, true);
      }
    }
  }
}
  
void SharkTopLevelBlock::initialize()
{
  char name[28];
  snprintf(name, sizeof(name),
           "bci_%d%s",
           start(), is_private_copy() ? "_private_copy" : "");
  _entry_block = function()->CreateBlock(name);
}

void SharkTopLevelBlock::acquire_method_lock()
{
  Value *object;
  if (target()->is_static()) {
    SharkConstantPool constants(this);
    object = constants.java_mirror();
  }
  else {
    object = local(0)->jobject_value();
  }
  iter()->force_bci(start()); // for the decache
  function()->monitor(0)->acquire(this, object);
  check_pending_exception(false);
}

void SharkTopLevelBlock::release_method_lock()
{
  function()->monitor(0)->release(this);

  // We neither need nor want to check for pending exceptions here.
  // This method is only called by handle_return, which copes with
  // them implicitly:
  //  - if a value is being returned then we just carry on as normal;
  //    the caller will see the pending exception and handle it.
  //  - if an exception is being thrown then that exception takes
  //    priority and ours will be ignored.
}

void SharkTopLevelBlock::emit_IR()
{
  builder()->SetInsertPoint(entry_block());

  // Handle traps
  if (has_trap()) {
    iter()->force_bci(start());

    current_state()->decache_for_trap();
    builder()->CreateCall2(
      SharkRuntime::uncommon_trap(),
      thread(),
      LLVMValue::jint_constant(trap_index()));
    builder()->CreateRetVoid();
    return;
  }

  // Parse the bytecode
  parse_bytecode(start(), limit());

  // If this block falls through to the next then it won't have been
  // terminated by a bytecode and we have to add the branch ourselves
  if (falls_through()) {
    builder()->CreateBr(successor(ciTypeFlow::FALL_THROUGH)->entry_block());
  }

  // Process the successor states if not already done
  switch (bc()) {
  case Bytecodes::_tableswitch:
  case Bytecodes::_lookupswitch:
    // done by do_switch()
    break;

  default:
    for (int i = 0; i < num_successors(); i++)
      successor(i)->add_incoming(current_state());
  }
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
  BasicBlock *zero     = function()->CreateBlock("zero");
  BasicBlock *not_zero = function()->CreateBlock("not_zero");

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

  builder()->CreateCondBr(builder()->CreateICmpNE(a, b), not_zero, zero);

  builder()->SetInsertPoint(zero);
  SharkState *saved_state = current_state()->copy();
  if (value->is_jobject()) {
    call_vm_nocheck(
      SharkRuntime::throw_NullPointerException(),
      builder()->pointer_constant(__FILE__),
      LLVMValue::jint_constant(__LINE__));
  }
  else {
    builder()->CreateUnimplemented(__FILE__, __LINE__);
  } 
  handle_exception(function()->CreateGetPendingException());
  set_current_state(saved_state);  

  builder()->SetInsertPoint(not_zero);

  value->set_zero_checked(true);
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
  call_vm_nocheck(
    SharkRuntime::throw_ArrayIndexOutOfBoundsException(),
    builder()->pointer_constant(__FILE__),
    LLVMValue::jint_constant(__LINE__),
    index->jint_value());
  handle_exception(function()->CreateGetPendingException());
  set_current_state(saved_state);  

  builder()->SetInsertPoint(in_bounds);
}

void SharkTopLevelBlock::check_pending_exception(bool attempt_catch)
{
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
  handle_exception(pending_exception, attempt_catch);
  set_current_state(saved_state);

  builder()->SetInsertPoint(no_exception);
}

void SharkTopLevelBlock::handle_exception(Value* exception, bool attempt_catch)
{
  if (attempt_catch && num_exceptions() != 0) {
    // Clear the stack and push the exception onto it.
    // We do this now to protect it across the VM call
    // we may be about to make.
    while (xstack_depth())
      pop();
    push(SharkValue::create_jobject(exception));

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

      Value *index = call_vm_nocheck(
        SharkRuntime::find_exception_handler(),
        builder()->CreateStructGEP(options, 0),
        LLVMValue::jint_constant(num_options));
      check_pending_exception(false);

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

void SharkTopLevelBlock::add_safepoint()
{
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
  call_vm(SharkRuntime::safepoint());
  BasicBlock *safepointed_block = builder()->GetInsertBlock();  
  builder()->CreateBr(safepointed);

  builder()->SetInsertPoint(safepointed);
  current_state()->merge(orig_state, orig_block, safepointed_block);
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
  call_vm(SharkRuntime::register_finalizer(), receiver);
  BasicBlock *branch_block = builder()->GetInsertBlock();  
  builder()->CreateBr(done);

  builder()->SetInsertPoint(done);
  current_state()->merge(orig_state, orig_block, branch_block);
}

void SharkTopLevelBlock::handle_return(BasicType type, Value* exception)
{
  assert (exception == NULL || type == T_VOID, "exception OR result, please");

  if (exception)
    builder()->CreateStore(exception, function()->exception_slot());

  release_locked_monitors();
  if (target()->is_synchronized())
    release_method_lock();
  
  if (exception) {
    exception = builder()->CreateLoad(function()->exception_slot());
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

void SharkTopLevelBlock::release_locked_monitors()
{
  int base = target()->is_synchronized();
  for (int i = function()->monitor_count() - 1; i >= base; i--) {
    BasicBlock *locked   = function()->CreateBlock("locked");
    BasicBlock *unlocked = function()->CreateBlock("unlocked");

    Value *object = function()->monitor(i)->object();
    builder()->CreateCondBr(
      builder()->CreateICmpNE(object, LLVMValue::null()),
      locked, unlocked);
    
    builder()->SetInsertPoint(locked);
    builder()->CreateUnimplemented(__FILE__, __LINE__);
    builder()->CreateUnreachable();

    builder()->SetInsertPoint(unlocked);
  }
}

Value *SharkTopLevelBlock::lookup_for_ldc()
{
  SharkConstantPool constants(this);

  BasicBlock *resolved       = function()->CreateBlock("resolved");
  BasicBlock *resolved_class = function()->CreateBlock("resolved_class");
  BasicBlock *unresolved     = function()->CreateBlock("unresolved");
  BasicBlock *unknown        = function()->CreateBlock("unknown");
  BasicBlock *done           = function()->CreateBlock("done");

  SwitchInst *switchinst = builder()->CreateSwitch(
    constants.tag_at(iter()->get_constant_index()),
    unknown, 5);

  switchinst->addCase(
    LLVMValue::jbyte_constant(JVM_CONSTANT_String), resolved);
  switchinst->addCase(
    LLVMValue::jbyte_constant(JVM_CONSTANT_Class), resolved_class);
  switchinst->addCase(
    LLVMValue::jbyte_constant(JVM_CONSTANT_UnresolvedString), unresolved);
  switchinst->addCase(
    LLVMValue::jbyte_constant(JVM_CONSTANT_UnresolvedClass), unresolved);
  switchinst->addCase(
    LLVMValue::jbyte_constant(JVM_CONSTANT_UnresolvedClassInError),
    unresolved);

  builder()->SetInsertPoint(resolved);
  Value *resolved_value = constants.object_at(iter()->get_constant_index());
  builder()->CreateBr(done);

  builder()->SetInsertPoint(resolved_class);
  Value *resolved_class_value
    = constants.object_at(iter()->get_constant_index());
  resolved_class_value
    = builder()->CreatePtrToInt(resolved_class_value,
                                SharkType::intptr_type());
  resolved_class_value
    = (builder()->CreateAdd
       (resolved_class_value,
        (LLVMValue::intptr_constant
         (klassOopDesc::klass_part_offset_in_bytes()
          + Klass::java_mirror_offset_in_bytes()))));
  // XXX FIXME: We need a memory barrier before this load.
  resolved_class_value
    = (builder()->CreateLoad
       (builder()->CreateIntToPtr
        (resolved_class_value,
         PointerType::getUnqual(SharkType::jobject_type()))));
  builder()->CreateBr(done);

  builder()->SetInsertPoint(unresolved);
  builder()->CreateUnimplemented(__FILE__, __LINE__);
  Value *unresolved_value = LLVMValue::null();
  builder()->CreateBr(done);

  builder()->SetInsertPoint(unknown);
  builder()->CreateShouldNotReachHere(__FILE__, __LINE__);
  builder()->CreateUnreachable();

  builder()->SetInsertPoint(done);
  PHINode *phi = builder()->CreatePHI(SharkType::jobject_type(), "constant");
  phi->addIncoming(resolved_value, resolved);
  phi->addIncoming(resolved_class_value, resolved_class);
  phi->addIncoming(unresolved_value, unresolved);
  return phi;
}

Value* SharkTopLevelBlock::lookup_for_field_access()
{
  SharkConstantPool constants(this);
  Value *cache = constants.cache_entry_at(iter()->get_field_index());

  return builder()->CreateValueOfStructEntry(
   cache, ConstantPoolCacheEntry::f1_offset(),
   SharkType::jobject_type(),
   "object");
}

void SharkTopLevelBlock::do_arraylength()
{
  SharkValue *array = pop();
  check_null(array);
  Value *length = builder()->CreateArrayLength(array->jarray_value());
  push(SharkValue::create_jint(length));
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
    push(SharkValue::create_jint(value));
    break;

  case T_LONG:
    push(SharkValue::create_jlong(value));
    break;

  case T_FLOAT:
    push(SharkValue::create_jfloat(value));
    break;

  case T_DOUBLE:
    push(SharkValue::create_jdouble(value));
    break;
    
  case T_OBJECT:
    push(SharkValue::create_generic(element_type, value));
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
  else
    add_safepoint();
  handle_return(type, NULL);
}

void SharkTopLevelBlock::do_athrow()
{
  SharkValue *exception = pop();
  check_null(exception);
  handle_exception(exception->jobject_value());
}

void SharkTopLevelBlock::do_goto()
{
  builder()->CreateBr(successor(ciTypeFlow::GOTO_TARGET)->entry_block());
}

void SharkTopLevelBlock::do_jsr()
{
  push(SharkValue::create_returnAddress(iter()->next_bci()));
  builder()->CreateBr(successor(ciTypeFlow::GOTO_TARGET)->entry_block());
}

void SharkTopLevelBlock::do_ret()
{
  assert(local(iter()->get_index())->returnAddress_value() ==
         successor(ciTypeFlow::GOTO_TARGET)->start(), "should be");
  builder()->CreateBr(successor(ciTypeFlow::GOTO_TARGET)->entry_block());
}
 
void SharkTopLevelBlock::do_if(ICmpInst::Predicate p, SharkValue *b, SharkValue *a)
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
                          
  builder()->CreateCondBr(
    builder()->CreateICmp(p, llvm_a, llvm_b),
    successor(ciTypeFlow::IF_TAKEN)->entry_block(),
    successor(ciTypeFlow::IF_NOT_TAKEN)->entry_block());
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
  SharkConstantPool constants(this);
  Value *cache = constants.cache_entry_at(iter()->get_method_index());
  return builder()->CreateValueOfStructEntry(
    cache,
    bc() == Bytecodes::_invokevirtual ?
      ConstantPoolCacheEntry::f2_offset() :
      ConstantPoolCacheEntry::f1_offset(),
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

  Value *index;
  if (!method->holder()->is_linked()) {
    // Yuck, we have to do this one slow :(
    // XXX should we trap on this?
    NOT_PRODUCT(warning("unresolved invokevirtual in %s", function()->name()));
    SharkConstantPool constants(this);
    Value *cache = constants.cache_entry_at(iter()->get_method_index());
    index = builder()->CreateValueOfStructEntry(
      cache, ConstantPoolCacheEntry::f2_offset(),
      SharkType::intptr_type(),
      "index");
  }
  else {
    index = LLVMValue::intptr_constant(method->vtable_index());
  }

  return builder()->CreateLoad(
    builder()->CreateArrayAddress(
      klass,
      SharkType::methodOop_type(),
      vtableEntry::size() * wordSize,
      in_ByteSize(instanceKlass::vtable_start_offset() * wordSize),
      index),
    "callee");
}

// Interpreter-style virtual call lookup
Value* SharkTopLevelBlock::get_virtual_callee(Value *cache, SharkValue *receiver)
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
    if (SharkInliner::attempt_inline(method, current_state()))
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
  check_pending_exception();
}

void SharkTopLevelBlock::do_instance_check()
{
  // Leave the object on the stack until after all the VM calls
  assert(xstack(0)->is_jobject(), "should be");
  
  ciKlass *klass = NULL;
  if (bc() == Bytecodes::_checkcast) {
    bool will_link;
    klass = iter()->get_klass(will_link);
    if (!will_link) {
      // XXX why is this not typeflow's responsibility?
      NOT_PRODUCT(warning("unresolved checkcast in %s", function()->name()));
      klass = (ciKlass *) xstack(0)->type();
    }
  }

  BasicBlock *not_null      = function()->CreateBlock("not_null");
  BasicBlock *fast_path     = function()->CreateBlock("fast_path");
  BasicBlock *slow_path     = function()->CreateBlock("slow_path");
  BasicBlock *got_klass     = function()->CreateBlock("got_klass");
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

  // Null objects aren't instances of anything
  builder()->CreateCondBr(
    builder()->CreateICmpEQ(xstack(0)->jobject_value(), LLVMValue::null()),
    merge2, not_null);
  BasicBlock *null_block = builder()->GetInsertBlock();
  SharkState *null_state = current_state()->copy();

  // Get the class we're checking against
  builder()->SetInsertPoint(not_null);
  SharkConstantPool constants(this);
  Value *tag = constants.tag_at(iter()->get_klass_index());
  builder()->CreateCondBr(
    builder()->CreateOr(
      builder()->CreateICmpEQ(
        tag, LLVMValue::jbyte_constant(JVM_CONSTANT_UnresolvedClass)),
      builder()->CreateICmpEQ(
        tag, LLVMValue::jbyte_constant(JVM_CONSTANT_UnresolvedClassInError))),
    slow_path, fast_path);

  // The fast path
  builder()->SetInsertPoint(fast_path);
  BasicBlock *fast_block = builder()->GetInsertBlock();
  SharkState *fast_state = current_state()->copy();
  Value *fast_klass = constants.object_at(iter()->get_klass_index());
  builder()->CreateBr(got_klass);

  // The slow path
  builder()->SetInsertPoint(slow_path);
  call_vm(
    SharkRuntime::resolve_klass(),
    LLVMValue::jint_constant(iter()->get_klass_index()));
  Value *slow_klass = function()->CreateGetVMResult();
  BasicBlock *slow_block = builder()->GetInsertBlock();  
  builder()->CreateBr(got_klass);

  // We have the class to test against
  builder()->SetInsertPoint(got_klass);
  current_state()->merge(fast_state, fast_block, slow_block);
  PHINode *check_klass = builder()->CreatePHI(
    SharkType::jobject_type(), "check_klass");
  check_klass->addIncoming(fast_klass, fast_block);
  check_klass->addIncoming(slow_klass, slow_block);

  // Get the class of the object being tested
  Value *object_klass = builder()->CreateValueOfStructEntry(
    xstack(0)->jobject_value(), in_ByteSize(oopDesc::klass_offset_in_bytes()),
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
  current_state()->merge(null_state, null_block, nonnull_block);
  PHINode *result = builder()->CreatePHI(
    SharkType::jint_type(), "result");
  result->addIncoming(LLVMValue::jint_constant(IC_IS_NULL), null_block);
  result->addIncoming(nonnull_result, nonnull_block);

  // We can finally pop the object!
  Value *object = pop()->jobject_value();

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
    push(SharkValue::create_generic(klass, object));
  }
  else {
    push(
      SharkValue::create_jint(
        builder()->CreateIntCast(
          builder()->CreateICmpEQ(
            result, LLVMValue::jint_constant(IC_IS_INSTANCE)),
          SharkType::jint_type(), false)));
  }
}

void SharkTopLevelBlock::do_new()
{
  bool will_link;
  ciInstanceKlass* klass = iter()->get_klass(will_link)->as_instance_klass();
  assert(will_link, "typeflow responsibility");

  BasicBlock *tlab_alloc          = NULL;
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

  SharkConstantPool constants(this);

  // The fast path
  if (!Klass::layout_helper_needs_slow_path(klass->layout_helper())) {
    if (UseTLAB) {
      tlab_alloc        = function()->CreateBlock("tlab_alloc");
      got_tlab          = function()->CreateBlock("got_tlab");
    }
    heap_alloc          = function()->CreateBlock("heap_alloc");
    retry               = function()->CreateBlock("retry");
    got_heap            = function()->CreateBlock("got_heap");
    initialize          = function()->CreateBlock("initialize");
    slow_alloc_and_init = function()->CreateBlock("slow_alloc_and_init");
    push_object         = function()->CreateBlock("push_object");

    builder()->CreateCondBr(
      builder()->CreateICmpEQ(
        constants.tag_at(iter()->get_klass_index()),
        LLVMValue::jbyte_constant(JVM_CONSTANT_Class)),
      UseTLAB ? tlab_alloc : heap_alloc, slow_alloc_and_init);
    
    size_t size_in_bytes = klass->size_helper() << LogHeapWordSize;

    // Thread local allocation
    if (UseTLAB) {
      builder()->SetInsertPoint(tlab_alloc);

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
    }

    // Heap allocation
    builder()->SetInsertPoint(heap_alloc);

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
    Value *rtklass = constants.object_at(iter()->get_klass_index());
    builder()->CreateStore(rtklass, klass_addr);
    got_fast = builder()->GetInsertBlock();

    builder()->CreateBr(push_object);
    builder()->SetInsertPoint(slow_alloc_and_init);
    fast_state = current_state()->copy();
  }

  // The slow path
  call_vm(
    SharkRuntime::new_instance(),
    LLVMValue::jint_constant(iter()->get_klass_index()));
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

  SharkValue *result = SharkValue::create_jobject(object);
  result->set_zero_checked(true);
  push(result);
}

void SharkTopLevelBlock::do_newarray()
{
  BasicType type = (BasicType) iter()->get_index();

  call_vm(
    SharkRuntime::newarray(),
    LLVMValue::jint_constant(type),
    pop()->jint_value());

  SharkValue *result = SharkValue::create_generic(
    ciArrayKlass::make(ciType::make(type)),
    function()->CreateGetVMResult());
  result->set_zero_checked(true);
  push(result);
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
    pop()->jint_value());

  SharkValue *result = SharkValue::create_generic(
    array_klass, function()->CreateGetVMResult());
  result->set_zero_checked(true);
  push(result);
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
    builder()->CreateStructGEP(dimensions, 0));

  // Now we can pop the dimensions off the stack
  for (int i = 0; i < ndims; i++)
    pop();

  SharkValue *result = SharkValue::create_generic(
    array_klass, function()->CreateGetVMResult());
  result->set_zero_checked(true);
  push(result);
}

void SharkTopLevelBlock::do_monitorenter()
{
  SharkValue *lockee = pop();
  check_null(lockee);
  Value *object = lockee->jobject_value();

  // Find a free monitor, or one already allocated for this object
  BasicBlock *loop_top    = function()->CreateBlock("loop_top");
  BasicBlock *loop_iter   = function()->CreateBlock("loop_iter");
  BasicBlock *loop_check  = function()->CreateBlock("loop_check");
  BasicBlock *no_monitor  = function()->CreateBlock("no_monitor");
  BasicBlock *got_monitor = function()->CreateBlock("got_monitor");

  BasicBlock *entry_block = builder()->GetInsertBlock();
  builder()->CreateBr(loop_check);

  builder()->SetInsertPoint(loop_check);
  PHINode *index = builder()->CreatePHI(SharkType::jint_type(), "index");
  index->addIncoming(
    LLVMValue::jint_constant(function()->monitor_count() - 1), entry_block);
  builder()->CreateCondBr(
    builder()->CreateICmpUGE(index, LLVMValue::jint_constant(0)),
    loop_top, no_monitor);

  builder()->SetInsertPoint(loop_top);
  SharkMonitor* monitor = function()->monitor(index);
  Value *smo = monitor->object();
  builder()->CreateCondBr(
    builder()->CreateOr(
      builder()->CreateICmpEQ(smo, LLVMValue::null()),
      builder()->CreateICmpEQ(smo, object)),
    got_monitor, loop_iter);

  builder()->SetInsertPoint(loop_iter);
  index->addIncoming(
    builder()->CreateSub(index, LLVMValue::jint_constant(1)), loop_iter);
  builder()->CreateBr(loop_check);

  builder()->SetInsertPoint(no_monitor);
  builder()->CreateShouldNotReachHere(__FILE__, __LINE__);
  builder()->CreateUnreachable();

  // Acquire the lock
  builder()->SetInsertPoint(got_monitor);
  monitor->acquire(this, object);
  check_pending_exception();
}

void SharkTopLevelBlock::do_monitorexit()
{
  SharkValue *lockee = pop();
  // The monitorexit can't throw an NPE because the verifier checks
  // that the monitor operations are block structured before we
  // compile.
  // check_null(lockee);
  Value *object = lockee->jobject_value();

  // Find the monitor associated with this object
  BasicBlock *loop_top    = function()->CreateBlock("loop_top");
  BasicBlock *loop_iter   = function()->CreateBlock("loop_iter");
  BasicBlock *loop_check  = function()->CreateBlock("loop_check");
  BasicBlock *no_monitor  = function()->CreateBlock("no_monitor");
  BasicBlock *got_monitor = function()->CreateBlock("got_monitor");

  BasicBlock *entry_block = builder()->GetInsertBlock();
  builder()->CreateBr(loop_check);

  builder()->SetInsertPoint(loop_check);
  PHINode *index = builder()->CreatePHI(SharkType::jint_type(), "index");
  index->addIncoming(
    LLVMValue::jint_constant(function()->monitor_count() - 1), entry_block);
  builder()->CreateCondBr(
    builder()->CreateICmpUGE(index, LLVMValue::jint_constant(0)),
    loop_top, no_monitor);

  builder()->SetInsertPoint(loop_top);
  SharkMonitor* monitor = function()->monitor(index);
  Value *smo = monitor->object();
  builder()->CreateCondBr(
    builder()->CreateICmpEQ(smo, object),
    got_monitor, loop_iter);

  builder()->SetInsertPoint(loop_iter);
  index->addIncoming(
    builder()->CreateSub(index, LLVMValue::jint_constant(1)), loop_iter);
  builder()->CreateBr(loop_check);

  builder()->SetInsertPoint(no_monitor);
  builder()->CreateShouldNotReachHere(__FILE__, __LINE__);
  builder()->CreateUnreachable();

  // Release the lock
  builder()->SetInsertPoint(got_monitor);
  monitor->release(this);
  // The monitorexit can't throw an NPE because the verifier checks
  // that the monitor operations are block structured before we
  // compile.
  // check_pending_exception();
}
