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

SharkState::SharkState(const SharkState* state)
  : _block(state->block()),
    _function(state->function()),
    _method(state->method())
{
  initialize(state);
}

SharkState::SharkState(SharkBlock*    block,
                       SharkFunction* function,
                       llvm::Value*   method)
  : _block(block),
    _function(function),
    _method(method)
{
  initialize(NULL);
}

SharkState::SharkState(const SharkState* state)
  : _block(state->block()),
    _function(state->function()),
    _method(state->method())
{
  initialize(state);
}

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
void SharkState::merge(SharkState* other,
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

    push(SharkValue::create_generic(type, NULL));
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
