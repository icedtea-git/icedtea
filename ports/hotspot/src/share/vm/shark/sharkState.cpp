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
      value = SharkValue::create_returnAddress(block()->jsr_ret_bci());
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
      value = SharkValue::create_returnAddress(block()->jsr_ret_bci());
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

#if 0
void SharkTrackingState::decache_for(CDReason reason, ciMethod *callee)
{
  // Start recording the debug information
  OopMap *oopmap = new OopMap(
    oopmap_slot_munge(function()->oopmap_frame_size()),
    oopmap_slot_munge(function()->arg_size()));
  int offset = function()->code_offset();
  debug_info()->add_safepoint(offset, oopmap);

  // Decache expression stack slots as necessary
  GrowableArray<ScopeValue*> *exparray =
    new GrowableArray<ScopeValue*>(stack_depth());

  for (int i = stack_depth() - 1; i >= 0; i--) {
    SharkValue *value = stack(i);

    Location::Type type = Location::normal;
    if (value && value->is_jobject())
      type = Location::oop;

    int            dst          = i + max_stack() - stack_depth();
    int            oiwfusp      = function()->stack_slots_offset() + dst;
    VMReg          dst_as_vmreg = slot2reg(oiwfusp);
    LocationValue* dst_as_lv    = slot2lv(oiwfusp, type);

    bool write  = false;
    bool record = false;

    if (reason == TRAP) {
      write  = value != NULL;
      record = true;
    }
    else if (value) {
      if (reason == JAVA_CALL && i < callee->arg_size()) {
        write  = true;
        record = false;
      }
      else {
        write  = value->is_jobject();
        record = true;
      }
    }

    if (write) {
      if (value->is_two_word()) {
        assert(i > 0, "should be");
        assert(stack(i - 1) == NULL, "should be");
        dst--;
      }
      
      builder()->CreateStore(
        value->generic_value(),
        builder()->CreateBitCast(
          builder()->CreateStructGEP(function()->stack_slots(), dst),
          PointerType::getUnqual(SharkType::to_stackType(
            value->basic_type()))));

      if (record)
        oopmap->set_oop(dst_as_vmreg);
    }

    if (record)
      exparray->append(dst_as_lv);
  }

  // If we're decaching for a call then pop the arguments
  int trim_slots = max_stack() - stack_depth();
  if (reason == JAVA_CALL)
    pop(callee->arg_size());

  // Record any monitors
  GrowableArray<MonitorValue*> *monarray =
    new GrowableArray<MonitorValue*>(function()->monitor_count());

  for (int i = 0; i < function()->monitor_count(); i++) {
    int box_oiwfusp =
      function()->monitors_slots_offset() +
      i * frame::interpreter_frame_monitor_size();

    int obj_oiwfusp =
      box_oiwfusp +
      (BasicObjectLock::obj_offset_in_bytes() >> LogBytesPerWord);
    
    oopmap->set_oop(slot2reg(obj_oiwfusp));

    monarray->append(new MonitorValue(
      slot2lv (obj_oiwfusp, Location::oop),
      slot2loc(box_oiwfusp, Location::normal)));
  }

  // Record the exception slot
  oopmap->set_oop(slot2reg(function()->exception_slot_offset()));

  // Decache the method pointer
  builder()->CreateStore(method(), function()->method_slot());
  oopmap->set_oop(slot2reg(function()->method_slot_offset()));

  // Decache the PC
  builder()->CreateStore(
    builder()->CreateAdd(
      function()->base_pc(), LLVMValue::intptr_constant(offset)),
    function()->pc_slot());

  // Decache local variables as necesary
  GrowableArray<ScopeValue*> *locarray =
    new GrowableArray<ScopeValue*>(max_locals());

  for (int i = 0; i < max_locals(); i++) {
    SharkValue *value = local(i);

    Location::Type type = Location::invalid;
    if (value) {
      if (value->is_jobject())
        type = Location::oop;
      else
        type = Location::normal;
    }
    else if (i > 0) {
      SharkValue *prev = local(i - 1);
      if (prev && prev->is_two_word())
        type = Location::normal;
    }

    int            dst          = max_locals() - 1 - i;
    int            oiwfusp      = function()->locals_slots_offset() + dst;
    VMReg          dst_as_vmreg = slot2reg(oiwfusp);
    LocationValue* dst_as_lv    = slot2lv(oiwfusp, type);

    if (value) {
      if (value->is_two_word()) {
        assert(i + 1 < max_locals(), "should be");
        assert(local(i + 1) == NULL, "should be");
        dst--;
      }

      if (reason == TRAP || value->is_jobject()) {
        builder()->CreateStore(
          value->generic_value(),
          builder()->CreateBitCast(
            builder()->CreateStructGEP(function()->locals_slots(), dst),
            PointerType::getUnqual(
              SharkType::to_stackType(value->basic_type()))));
      }
  
      if (value->is_jobject())
        oopmap->set_oop(dst_as_vmreg);
    }
    locarray->append(dst_as_lv);
  }

  // Trim back the stack if necessary
  if (trim_slots) {
    function()->CreateStoreZeroStackPointer(
      builder()->CreatePtrToInt(
        builder()->CreateStructGEP(
          function()->stack_slots(), trim_slots),
        SharkType::intptr_type()));
  }

  // Record the scope and end the block of debug information
  DebugToken *locvals = debug_info()->create_scope_values(locarray);
  DebugToken *expvals = debug_info()->create_scope_values(exparray);
  DebugToken *monvals = debug_info()->create_monitor_values(monarray);
  debug_info()->describe_scope(
    offset, block()->target(), block()->bci(), locvals, expvals, monvals);
  debug_info()->end_safepoint(offset);
}

void SharkTrackingState::cache_after(CDReason reason, ciMethod *callee)
{
  assert(reason != TRAP, "shouldn't be");

  // If we're caching after a call then push a dummy result to set up the stack
  int result_size = reason == JAVA_CALL ? callee->return_type()->size() : 0;
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
    if (result_size == 2)
      push(NULL);
  }

  // Cache expression stack slots as necessary
  for (int i = 0; i < stack_depth(); i++) {
    SharkValue *value = stack(i);
    if (value == NULL) {
      set_stack(i, NULL);
      continue;
    }

    bool read;

    if (i < result_size) {
      read = true;
    }
    else {
      read = value->is_jobject();
    }

    if (read) {
      int src = i + max_stack() - stack_depth();
      if (value->is_two_word()) {
        assert(i > 0, "should be");
        assert(stack(i - 1) == NULL, "should be");
        src--;
      }
      
      set_stack(
        i,
        SharkValue::create_generic(
          value->type(),
          builder()->CreateLoad(
            builder()->CreateBitCast(
              builder()->CreateStructGEP(function()->stack_slots(), src),
              PointerType::getUnqual(
                SharkType::to_stackType(value->basic_type()))))));
    }
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
  if (stack_depth() != max_stack()) {
    function()->CreateStoreZeroStackPointer(
      builder()->CreatePtrToInt(
        builder()->CreateStructGEP(function()->stack_slots(), 0),
        SharkType::intptr_type()));
  }
}
#endif // 0

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
