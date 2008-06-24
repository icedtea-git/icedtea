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

void SharkState::initialize()
{
  _locals = NEW_RESOURCE_ARRAY(SharkValue*, max_locals());
  _stack  = NEW_RESOURCE_ARRAY(SharkValue*, max_stack());
}

void SharkEntryState::initialize()
{
  char name[18];

  // Method
  _method = function()->function()->arg_begin();

  // Local variables
  for (int i = 0; i < max_locals(); i++) {
    SharkValue *value = NULL;
    ciType *type = block()->local_type_at_entry(i);
    if (type->basic_type() != T_CONFLICT) {
      if (i < function()->arg_size()) {
        snprintf(name, sizeof(name), "local_%d_", i);
        value = SharkValue::create_generic(
          type,
          builder()->CreateLoad(
            builder()->CreateBitCast(
              builder()->CreateStructGEP(
                function()->locals_slots(), max_locals() - 1 - i),
              PointerType::getUnqual(SharkType::to_stackType(type))),
            name));
      }
      else {
        Unimplemented();
      }
    }
    _locals[i] = value;
  }

  // Expression stack
  assert(!block()->stack_depth_at_entry(), "entry block shouldn't have stack");
  memset(_stack, 0, sizeof(SharkValue*) * max_stack());
  _sp = _stack;
}

void SharkPHIState::initialize()
{
  BasicBlock *saved_insert_point = builder()->GetInsertBlock();
  builder()->SetInsertPoint(block()->entry_block());
  char name[18];

  // Method
  _method = builder()->CreatePHI(SharkType::methodOop_type(), "method");

  // Local variables
  for (int i = 0; i < max_locals(); i++) {
    SharkValue *value = NULL;
    ciType *type = block()->local_type_at_entry(i);
    if (type->basic_type() != T_CONFLICT) {
      snprintf(name, sizeof(name), "local_%d_", i);
      value = SharkValue::create_generic(
        type, builder()->CreatePHI(SharkType::to_stackType(type), name));
    }
    _locals[i] = value;    
  }

  // Expression stack
  for (int i = 0; i < max_stack(); i++) {
    SharkValue *value = NULL;
    if (i < block()->stack_depth_at_entry()) {
      ciType *type = block()->stack_type_at_entry(i);
      snprintf(name, sizeof(name), "stack_%d_", i);
      value = SharkValue::create_generic(
        type, builder()->CreatePHI(SharkType::to_stackType(type), name));
    }
    _stack[i] = value;
  }
  _sp = _stack + block()->stack_depth_at_entry();

  builder()->SetInsertPoint(saved_insert_point);
}

void SharkPHIState::add_incoming(SharkState* incoming_state)
{
  BasicBlock *predecessor = builder()->GetInsertBlock();
    
  // Method
  ((PHINode *) method())->addIncoming(incoming_state->method(), predecessor);

  // Local variables
  for (int i = 0; i < max_locals(); i++) {
    if (local(i) != NULL) {
      ((PHINode *) local(i)->generic_value())->addIncoming(
        incoming_state->local(i)->generic_value(), predecessor);
    }
  }

  // Expression stack
  assert(block()->stack_depth_at_entry() == incoming_state->stack_depth(),
         "should be");
  for (int i = 0; i < block()->stack_depth_at_entry(); i++) {
    ((PHINode *) stack(i)->generic_value())->addIncoming(
      incoming_state->stack(i)->generic_value(),
      predecessor);
  }
}

void SharkTrackingState::initialize(const SharkState *initial_state)
{
  _method = initial_state->_method;
  memcpy(_locals, initial_state->_locals, sizeof(SharkValue*) * max_locals());
  memcpy(_stack,  initial_state->_stack,  sizeof(SharkValue*) * max_stack());
  _sp = _stack + initial_state->stack_depth();
}

int SharkTrackingState::stack_depth_in_slots() const
{
  int depth = 0;
  for (SharkValue **v = _stack; v < _sp; v++)
    depth += type2size[(*v)->basic_type()];
  return depth;
}

void SharkTrackingState::decache(ciMethod *callee)
{
  // Decache expression stack slots as necessary
  int stack_slots = stack_depth_in_slots();
  for (int i = 0, j = 0; i < stack_slots; i++) {
    SharkValue *value;
    bool write;

    if (callee && i < callee->arg_size()) {
      value = pop();
      write = true;
    }
    else {
      value = stack(j++);
      write = value->is_jobject();
    }

    if (write) {
      builder()->CreateStore(
        value->generic_value(),
        builder()->CreateBitCast(
          builder()->CreateStructGEP(
            function()->stack_slots(),
            i + max_stack() - stack_slots),
          PointerType::getUnqual(SharkType::to_stackType(value->type()))));
    }

    if (value->is_two_word())
      i++;
  }

  // Decache the method pointer
  builder()->CreateStore(method(), function()->method_slot());

  // Decache local variables as necesary
  for (int i = max_locals() - 1; i >= 0; i--) {
    SharkValue *value = local(i);
    if (value && value->is_jobject()) {
      builder()->CreateStore(
        value->generic_value(),
        builder()->CreateBitCast(
          builder()->CreateStructGEP(
            function()->locals_slots(),
            max_locals() - 1 - i),
          PointerType::getUnqual(
            SharkType::to_stackType(value->type()))));
    }
  }

  // If we're decaching for a call then trim back the stack
  if (callee && stack_slots != max_stack()) {
    function()->CreateStoreZeroStackPointer(
      builder()->CreatePtrToInt(
        builder()->CreateStructGEP(
          function()->stack_slots(), max_stack() - stack_slots),
        SharkType::intptr_type()));
  }
}

void SharkTrackingState::cache(ciMethod *callee)
{
  // If we're caching after a call then push a dummy result to set up the stack
  int result_size = callee ? callee->return_type()->size() : 0;
  if (result_size) {
    push(SharkValue::create_generic(callee->return_type(), NULL));
  }

  // Cache expression stack slots as necessary
  int stack_slots = stack_depth_in_slots();
  for (int i = 0, j = 0; i < stack_slots; i++, j++) {
    SharkValue *value = stack(j);
    bool read;

    if (result_size && i == 0) {
      read = true;
    }
    else {
      read = value->is_jobject();
    }

    if (read) {
      set_stack(
        j,
        SharkValue::create_generic(
          value->type(),
          builder()->CreateLoad(
            builder()->CreateBitCast(
              builder()->CreateStructGEP(
                function()->stack_slots(),
                i + max_stack() - stack_slots),
              PointerType::getUnqual(
                SharkType::to_stackType(value->basic_type()))))));
    }

    if (value->is_two_word())
      i++;
  }

  // Cache the method pointer
  set_method(builder()->CreateLoad(function()->method_slot()));

  // Cache local variables as necesary
  for (int i = max_locals() - 1; i >= 0; i--) {
    SharkValue *value = local(i);
    if (value && value->is_jobject()) {
      set_local(
        i,
        SharkValue::create_generic(
          value->type(),
          builder()->CreateLoad(
            builder()->CreateBitCast(
              builder()->CreateStructGEP(
                function()->locals_slots(),
                max_locals() - 1 - i),
              PointerType::getUnqual(
                SharkType::to_stackType(value->type()))))));
    }
  }

  // If we're caching after a call then restore the stack pointer
  if (callee && stack_slots != max_stack()) {
    function()->CreateStoreZeroStackPointer(
      builder()->CreatePtrToInt(
        builder()->CreateStructGEP(function()->stack_slots(), 0),
        SharkType::intptr_type()));
  }
}
