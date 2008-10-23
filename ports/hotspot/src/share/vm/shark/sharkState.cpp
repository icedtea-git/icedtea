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
#include "incls/_sharkState.cpp.incl"

using namespace llvm;

void SharkState::initialize(const SharkState *state)
{
  _locals = NEW_RESOURCE_ARRAY(SharkValue*, max_locals());
  _stack  = NEW_RESOURCE_ARRAY(SharkValue*, max_stack());

  if (state) {
    memcpy(_locals, state->_locals, max_locals() * sizeof(SharkValue *));
    memcpy(_stack,  state->_stack,  max_stack()  * sizeof(SharkValue *));
    _sp = _stack + state->stack_depth();
  }
  else {
    _sp = _stack;

    NOT_PRODUCT(memset(_locals, 23, max_locals() * sizeof(SharkValue *)));
    NOT_PRODUCT(memset(_stack,  23, max_stack()  * sizeof(SharkValue *)));
  }
}

void SharkEntryState::initialize(Value* method)
{
  char name[18];

  // Method
  set_method(method);

  // Local variables
  for (int i = 0; i < max_locals(); i++) {
    ciType *type = block()->local_type_at_entry(i);

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
            name));
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
  assert(!stack_depth_at_entry(), "entry block shouldn't have stack");
}

void SharkPHIState::initialize()
{
  BasicBlock *saved_insert_point = builder()->GetInsertBlock();
  builder()->SetInsertPoint(block()->entry_block());
  char name[18];

  // Method
  set_method(builder()->CreatePHI(SharkType::methodOop_type(), "method"));

  // Local variables
  for (int i = 0; i < max_locals(); i++) {
    ciType *type = block()->local_type_at_entry(i);
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
  for (int i = 0; i < stack_depth_at_entry(); i++) {
    ciType *type = block()->stack_type_at_entry(i);
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
  assert(stack_depth_at_entry() == incoming_state->stack_depth(), "should be");
  for (int i = 0; i < stack_depth_at_entry(); i++) {
    assert((stack(i) == NULL) == (incoming_state->stack(i) == NULL), "oops");
    if (stack(i))
      stack(i)->addIncoming(incoming_state->stack(i), predecessor);
  }
}

void SharkTrackingState::merge(SharkState* other,
                               BasicBlock* other_block,
                               BasicBlock* this_block)
{
  PHINode *phi;
  char name[18];

  // Method
  Value *this_method = this->method();
  Value *other_method = other->method();
  if (this_method != other_method) {
    phi = builder()->CreatePHI(SharkType::methodOop_type(), "method");
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
    if (this_value == other_value)
      continue;

    ciType *this_type = this_value->type();
    assert(this_type == other_value->type(), "should be");

    snprintf(name, sizeof(name), "local_%d_", i);
    phi = builder()->CreatePHI(SharkType::to_stackType(this_type), name);
    phi->addIncoming(this_value->generic_value(), this_block);
    phi->addIncoming(other_value->generic_value(), other_block);
    set_local(i, SharkValue::create_generic(this_type, phi));
  }

  // Expression stack
  assert(this->stack_depth() == other->stack_depth(), "should be");
  for (int i = 0; i < stack_depth(); i++) {
    SharkValue *this_value = this->stack(i);
    SharkValue *other_value = other->stack(i);
    assert((this_value == NULL) == (other_value == NULL), "should be");
    if (this_value == other_value)
      continue;

    ciType *this_type = this_value->type();
    assert(this_type == other_value->type(), "should be");

    snprintf(name, sizeof(name), "stack_%d_", i);
    phi = builder()->CreatePHI(SharkType::to_stackType(this_type), name);
    phi->addIncoming(this_value->generic_value(), this_block);
    phi->addIncoming(other_value->generic_value(), other_block);
    set_stack(i, SharkValue::create_generic(this_type, phi));
  }
}
