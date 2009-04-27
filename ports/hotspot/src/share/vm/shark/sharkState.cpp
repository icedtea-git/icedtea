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
#include "incls/_sharkState.cpp.incl"

using namespace llvm;

SharkState::SharkState(SharkBlock*    block,
                       SharkFunction* function,
                       llvm::Value*   method)
  : _block(block),
    _function(function),
    _method(method)
{
  initialize(NULL);
}

SharkState::SharkState(SharkBlock* block, const SharkState* state)
  : _block(block),
    _function(state->function()),
    _method(state->method())
{
  initialize(state);
}

void SharkState::initialize(const SharkState *state)
{
  _locals = NEW_RESOURCE_ARRAY(SharkValue*, max_locals());
  _stack  = NEW_RESOURCE_ARRAY(SharkValue*, max_stack());

  NOT_PRODUCT(memset(_locals, 23, max_locals() * sizeof(SharkValue *)));
  NOT_PRODUCT(memset(_stack,  23, max_stack()  * sizeof(SharkValue *)));
  _sp = _stack;

  if (state) {
    for (int i = 0; i < max_locals(); i++) {
      SharkValue *value = state->local(i);
      if (value)
        value = value->clone();
      set_local(i, value);
    }

    for (int i = state->stack_depth() - 1; i >= 0; i--) {
      SharkValue *value = state->stack(i);
      if (value)
        value = value->clone();
      push(value);
    }
  } 
}

bool SharkState::equal_to(SharkState *other)
{
  if (block() != other->block())
    return false;

  if (function() != other->function())
    return false;

  if (method() != other->method())
    return false;

  if (max_locals() != other->max_locals())
    return false;

  if (stack_depth() != other->stack_depth())
    return false;

  for (int i = 0; i < max_locals(); i++) {
    SharkValue *value = local(i);
    SharkValue *other_value = other->local(i);

    if (value == NULL) {
      if (other_value != NULL)
        return false;
    }
    else {
      if (other_value == NULL)
        return false;

      if (!value->equal_to(other_value))
        return false;
    }
  }

  for (int i = 0; i < stack_depth(); i++) {
    SharkValue *value = stack(i);
    SharkValue *other_value = other->stack(i);

    if (value == NULL) {
      if (other_value != NULL)
        return false;
    }
    else {
      if (other_value == NULL)
        return false;

      if (!value->equal_to(other_value))
        return false;
    }
  }

  return true;
}

void SharkState::merge(SharkState* other,
                       BasicBlock* other_block,
                       BasicBlock* this_block)
{
  // Method
  Value *this_method = this->method();
  Value *other_method = other->method();
  if (this_method != other_method) {
    PHINode *phi = builder()->CreatePHI(SharkType::methodOop_type(), "method");
    phi->addIncoming(this_method, this_block);
    phi->addIncoming(other_method, other_block);
    set_method(phi);
  }

  // Local variables
  assert(this->max_locals() == other->max_locals(), "should be");
  for (int i = 0; i < max_locals(); i++) {
    SharkValue *this_value = this->local(i);
    SharkValue *other_value = other->local(i);
    assert((this_value == NULL) == (other_value == NULL), "should be");
    if (this_value != NULL) {
      char name[18];
      snprintf(name, sizeof(name), "local_%d_", i);
      set_local(i, this_value->merge(
        builder(), other_value, other_block, this_block, name));
    }
  }

  // Expression stack
  assert(this->stack_depth() == other->stack_depth(), "should be");
  for (int i = 0; i < stack_depth(); i++) {
    SharkValue *this_value = this->stack(i);
    SharkValue *other_value = other->stack(i);
    assert((this_value == NULL) == (other_value == NULL), "should be");
    if (this_value != NULL) {
      char name[18];
      snprintf(name, sizeof(name), "stack_%d_", i);
      set_stack(i, this_value->merge(
        builder(), other_value, other_block, this_block, name));
    }
  }
}

void SharkState::decache_for_Java_call(ciMethod* callee)
{
  assert(function() && method(), "you cannot decache here");
  SharkJavaCallDecacher(function(), block()->bci(), callee).scan(this);
  pop(callee->arg_size());
}

void SharkState::cache_after_Java_call(ciMethod* callee)
{
  assert(function() && method(), "you cannot cache here");
  if (callee->return_type()->size()) {
    ciType *type;
    switch (callee->return_type()->basic_type()) {
    case T_BOOLEAN:
    case T_BYTE:
    case T_CHAR:
    case T_SHORT:
      type = ciType::make(T_INT);
      break;

    default:
      type = callee->return_type();
    }

    push(SharkValue::create_generic(type, NULL, false));
    if (type->is_two_word())
      push(NULL);
  }
  SharkJavaCallCacher(function(), block()->bci(), callee).scan(this);
}

void SharkState::decache_for_VM_call()
{
  assert(function() && method(), "you cannot decache here");
  SharkVMCallDecacher(function(), block()->bci()).scan(this);
}

void SharkState::cache_after_VM_call()
{
  assert(function() && method(), "you cannot cache here");
  SharkVMCallCacher(function(), block()->bci()).scan(this);
}

void SharkState::decache_for_trap()
{
  assert(function() && method(), "you cannot decache here");
  SharkTrapDecacher(function(), block()->bci()).scan(this);
}

SharkEntryState::SharkEntryState(SharkTopLevelBlock* block, Value* method)
  : SharkState(block, block->function(), method)
{
  char name[18];

  // Local variables
  for (int i = 0; i < max_locals(); i++) {
    ciType *type = block->local_type_at_entry(i);

    SharkValue *value = NULL;
    switch (type->basic_type()) {
    case T_INT:
    case T_LONG:
    case T_FLOAT:
    case T_DOUBLE:
    case T_OBJECT:
    case T_ARRAY:
      if (i < function()->arg_size()) {
        snprintf(name, sizeof(name), "local_%d_", i);
        value = SharkValue::create_generic(
          type,
          builder()->CreateLoad(
            function()->CreateAddressOfFrameEntry(
              function()->locals_slots_offset()
              + max_locals() - type->size() - i,
              SharkType::to_stackType(type)),
            name),
          i == 0 && !function()->target()->is_static());
      }
      else {
        Unimplemented();
      }
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
  assert(!block->stack_depth_at_entry(), "entry block shouldn't have stack");
}

SharkPHIState::SharkPHIState(SharkTopLevelBlock* block)
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
      value = SharkValue::create_phi(
        type, builder()->CreatePHI(SharkType::to_stackType(type), name));
      break;

    case T_ADDRESS:
      value = SharkValue::address_constant(type->as_return_address()->bci());
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
      value = SharkValue::create_phi(
        type, builder()->CreatePHI(SharkType::to_stackType(type), name));
      break;

    case T_ADDRESS:
      value = SharkValue::address_constant(type->as_return_address()->bci());
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

void SharkPHIState::add_incoming(SharkState* incoming_state)
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
