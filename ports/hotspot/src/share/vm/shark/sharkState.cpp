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

void SharkEntryState::initialize(Value* method)
{
  char name[18];

  // Method
  _method = method;

  // Local variables
  for (int i = 0; i < max_locals(); i++) {
    SharkValue *value = NULL;
    ciType *type = block()->local_type_at_entry(i);
    switch (type->basic_type()) {
    case ciTypeFlow::StateVector::T_BOTTOM:
    case ciTypeFlow::StateVector::T_LONG2:
    case ciTypeFlow::StateVector::T_DOUBLE2:
      break;

    case ciTypeFlow::StateVector::T_TOP:
      Unimplemented();
      break;

    case ciTypeFlow::StateVector::T_NULL:
      ShouldNotReachHere();
      break;
      
    default:
      if (i < function()->arg_size()) {
        snprintf(name, sizeof(name), "local_%d_", i);
        value = SharkValue::create_generic(
          type,
          builder()->CreateLoad(
            builder()->CreateBitCast(
              builder()->CreateStructGEP(
                function()->locals_slots(),
                max_locals() - type->size() - i),
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
    switch (type->basic_type()) {
    case ciTypeFlow::StateVector::T_BOTTOM:
    case ciTypeFlow::StateVector::T_LONG2:
    case ciTypeFlow::StateVector::T_DOUBLE2:
      break;

    case ciTypeFlow::StateVector::T_TOP:
      Unimplemented();
      break;

    case ciTypeFlow::StateVector::T_NULL:
      // XXX we could do all kinds of clever stuff here
      type = ciType::make(T_OBJECT); // XXX what about T_ARRAY?
      // fall through

    default:
      snprintf(name, sizeof(name), "local_%d_", i);
      value = SharkValue::create_generic(
        type, builder()->CreatePHI(SharkType::to_stackType(type), name));
    }
    _locals[i] = value;    
  }

  // Expression stack
  _sp = _stack;
  for (int i = 0; i < max_stack(); i++) {
    if (i < block()->stack_depth_at_entry()) {
      ciType *type = block()->stack_type_at_entry(i);
      switch (type->basic_type()) {
      case ciTypeFlow::StateVector::T_TOP:
      case ciTypeFlow::StateVector::T_BOTTOM:
      case ciTypeFlow::StateVector::T_NULL:
        Unimplemented();
        break;

      case ciTypeFlow::StateVector::T_LONG2:
      case ciTypeFlow::StateVector::T_DOUBLE2:
        break;

      default:
        snprintf(name, sizeof(name), "stack_%d_", i);
        *(_sp++) = SharkValue::create_generic(
          type, builder()->CreatePHI(SharkType::to_stackType(type), name));
      }
    }
  }
#ifdef ASSERT
  _stack_depth_at_entry = stack_depth();
#endif // ASSERT

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
  assert(_stack_depth_at_entry == incoming_state->stack_depth(), "should be");
  for (int i = 0; i < incoming_state->stack_depth(); i++) {
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
  // Create the OopMap
  OopMap *oopmap = new OopMap(
    oopmap_slot_munge(function()->oopmap_frame_size()),
    oopmap_slot_munge(function()->arg_size()));

  // Decache expression stack slots as necessary
  int stack_slots = stack_depth_in_slots();
  for (int i = 0, j = 0; i < stack_slots; i++) {
    SharkValue *value;
    bool write;
    bool record;
    int dst;

    if (callee && i < callee->arg_size()) {
      value = pop();
      write = true;
      record = false;
    }
    else {
      value = stack(j++);
      write = record = value->is_jobject();
    }

    if (write) {
      dst = i + max_stack() - stack_slots;
      builder()->CreateStore(
        value->generic_value(),
        builder()->CreateBitCast(
          builder()->CreateStructGEP(function()->stack_slots(), dst),
          PointerType::getUnqual(SharkType::to_stackType(value->type()))));
    }

    if (record)
      oopmap->set_oop(slot2reg(function()->stack_slots_offset() + dst));

    if (value->is_two_word())
      i++;
  }

  // Record any monitors
  for (int i = 0; i < function()->monitor_count(); i++) {
    oopmap->set_oop(
      slot2reg(
        function()->monitors_slots_offset() +
        i * frame::interpreter_frame_monitor_size() +
        (BasicObjectLock::obj_offset_in_bytes() >> LogBytesPerWord)));
  }

  // Decache the method pointer
  builder()->CreateStore(method(), function()->method_slot());
  oopmap->set_oop(slot2reg(function()->method_slot_offset()));

  // Decache the PC
  int offset = function()->code_offset();
  builder()->CreateStore(
    builder()->CreateAdd(
      function()->base_pc(), LLVMValue::intptr_constant(offset)),
    function()->pc_slot());

  // Decache local variables as necesary
  for (int i = max_locals() - 1; i >= 0; i--) {
    SharkValue *value = local(i);
    if (value && value->is_jobject()) {
      int dst = max_locals() - 1 - i;
      
      builder()->CreateStore(
        value->generic_value(),
        builder()->CreateBitCast(
          builder()->CreateStructGEP(function()->locals_slots(), dst),
          PointerType::getUnqual(
            SharkType::to_stackType(value->type()))));

      oopmap->set_oop(slot2reg(function()->locals_slots_offset() + dst));
    }
  }

  // Trim back the stack if necessary
  if (stack_slots != max_stack()) {
    function()->CreateStoreZeroStackPointer(
      builder()->CreatePtrToInt(
        builder()->CreateStructGEP(
          function()->stack_slots(), max_stack() - stack_slots),
        SharkType::intptr_type()));
  }

  // Install the OopMap
  function()->add_gc_map(offset, oopmap);
}

void SharkTrackingState::cache(ciMethod *callee)
{
  // If we're caching after a call then push a dummy result to set up the stack
  int result_size = callee ? callee->return_type()->size() : 0;
  if (result_size) {
    ciType *result_type;
    switch (callee->return_type()->basic_type()) {
    case T_BOOLEAN:
    case T_BYTE:
    case T_CHAR:
    case T_SHORT:
      result_type = ciType::make(T_INT);
      break;

    default:
      result_type = callee->return_type();
    }
    push(SharkValue::create_generic(result_type, NULL));
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
      int src = i + max_stack() - stack_slots;
      set_stack(
        j,
        SharkValue::create_generic(
          value->type(),
          builder()->CreateLoad(
            builder()->CreateBitCast(
              builder()->CreateStructGEP(function()->stack_slots(), src),
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
      int src = max_locals() - 1 - i;
      set_local(
        i,
        SharkValue::create_generic(
          value->type(),
          builder()->CreateLoad(
            builder()->CreateBitCast(
              builder()->CreateStructGEP(function()->locals_slots(), src),
              PointerType::getUnqual(
                SharkType::to_stackType(value->type()))))));
    }
  }

  // Restore the stack pointer if necessary
  if (stack_slots != max_stack()) {
    function()->CreateStoreZeroStackPointer(
      builder()->CreatePtrToInt(
        builder()->CreateStructGEP(function()->stack_slots(), 0),
        SharkType::intptr_type()));
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
    _method = phi;
  }

  // Local variables
  assert(this->max_locals() == other->max_locals(), "should be");
  for (int i = 0; i < max_locals(); i++) {
    SharkValue *this_value = this->local(i);
    SharkValue *other_value = other->local(i);
    if (this_value == other_value)
      continue;
    assert(this_value != NULL && other_value != NULL, "shouldn't be");

    ciType *this_type = this_value->type();
    assert(this_type == other_value->type(), "should be");

    snprintf(name, sizeof(name), "local_%d_", i);
    phi = builder()->CreatePHI(SharkType::to_stackType(this_type), name);
    phi->addIncoming(this_value->generic_value(), this_block);
    phi->addIncoming(other_value->generic_value(), other_block);
    _locals[i] = SharkValue::create_generic(this_type, phi);
  }

  // Expression stack
  assert(this->stack_depth() == other->stack_depth(), "should be");
  for (int i = 0; i < stack_depth(); i++) {
    SharkValue *this_value = this->stack(i);
    SharkValue *other_value = other->stack(i);
    assert(this_value != NULL && other_value != NULL, "shouldn't be");
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
