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
#include "incls/_sharkCacheDecache.cpp.incl"

using namespace llvm;

void SharkDecacher::start_frame()
{
  // Start recording the debug information
  _pc_offset = function()->code_offset();
  _oopmap = new OopMap(
    oopmap_slot_munge(function()->oopmap_frame_size()),
    oopmap_slot_munge(function()->arg_size()));
  debug_info()->add_safepoint(pc_offset(), oopmap());
}

void SharkDecacher::start_stack(int num_slots, int max_slots)
{
  // Create the array we'll record our stack slots in
  _exparray = new GrowableArray<ScopeValue*>(num_slots);

  // Set the stack pointer
  function()->CreateStoreZeroStackPointer(
    builder()->CreatePtrToInt(
      function()->CreateAddressOfFrameEntry(
        function()->stack_slots_offset() + max_slots - num_slots),
      SharkType::intptr_type()));
}

void SharkDecacher::process_stack_slot(int          index,
                                       SharkValue** addr,
                                       int          offset)
{
  SharkValue *value = *addr;

  // Write the value to the frame if necessary
  if (stack_slot_needs_write(index, value)) {
    builder()->CreateStore(
      value->generic_value(),
      function()->CreateAddressOfFrameEntry(
        adjusted_offset(value, offset),
        SharkType::to_stackType(value->basic_type())));
  }

  // Record the value in the oopmap if necessary
  if (stack_slot_needs_oopmap(index, value)) {
    oopmap()->set_oop(slot2reg(offset));
  }

  // Record the value in the debuginfo if necessary
  if (stack_slot_needs_debuginfo(index, value)) {
    exparray()->append(slot2lv(offset, stack_location_type(index, addr)));
  }
}

void SharkDecacher::start_monitors(int num_monitors)
{
  // Create the array we'll record our monitors in
  _monarray = new GrowableArray<MonitorValue*>(num_monitors);
}

void SharkDecacher::process_monitor(int index, int box_offset)
{
  int obj_offset =
    box_offset + (BasicObjectLock::obj_offset_in_bytes() >> LogBytesPerWord);

  oopmap()->set_oop(slot2reg(obj_offset));

  monarray()->append(new MonitorValue(
    slot2lv (obj_offset, Location::oop),
    slot2loc(box_offset, Location::normal)));
}

void SharkDecacher::process_exception_slot(int offset)
{
  // Record the exception slot
  oopmap()->set_oop(slot2reg(offset));
}

void SharkDecacher::process_method_slot(llvm::Value** value, int offset)
{
  // Decache the method pointer
  builder()->CreateStore(
    *value,
    function()->CreateAddressOfFrameEntry(
      offset,
      SharkType::methodOop_type()));
  oopmap()->set_oop(slot2reg(offset));  
}

void SharkDecacher::process_pc_slot(int offset)
{
  // Record the PC
  builder()->CreateStore(
    builder()->CreateAdd(
      function()->base_pc(), LLVMValue::intptr_constant(pc_offset())),
    function()->CreateAddressOfFrameEntry(offset));
}
  
void SharkDecacher::start_locals(int num_locals)
{
  // Create the array we'll record our local variables in
  _locarray = new GrowableArray<ScopeValue*>(num_locals);}

void SharkDecacher::process_local_slot(int          index,
                                       SharkValue** addr,
                                       int          offset)
{
  SharkValue *value = *addr;

  // Write the value to the frame if necessary
  if (local_slot_needs_write(index, value)) {
    builder()->CreateStore(
      value->generic_value(),
      function()->CreateAddressOfFrameEntry(
        adjusted_offset(value, offset),
        SharkType::to_stackType(value->basic_type())));
  }

  // Record the value in the oopmap if necessary
  if (local_slot_needs_oopmap(index, value)) {
    oopmap()->set_oop(slot2reg(offset));
  }

  // Record the value in the debuginfo if necessary
  if (local_slot_needs_debuginfo(index, value)) {
    locarray()->append(slot2lv(offset, local_location_type(index, addr)));
  }
}

void SharkDecacher::end_frame()
{
  // Record the scope
  debug_info()->describe_scope(
    pc_offset(),
    function()->target(),
    bci(),
    debug_info()->create_scope_values(locarray()),
    debug_info()->create_scope_values(exparray()),
    debug_info()->create_monitor_values(monarray()));

  // Finish recording the debug information
  debug_info()->end_safepoint(pc_offset());
}

void SharkCacher::process_stack_slot(int          index,
                                     SharkValue** addr,
                                     int          offset)
{
  SharkValue *value = *addr;

  // Read the value from the frame if necessary
  if (stack_slot_needs_read(index, value)) {
    *addr = SharkValue::create_generic(
      value->type(),
      builder()->CreateLoad(
        function()->CreateAddressOfFrameEntry(
          adjusted_offset(value, offset),
          SharkType::to_stackType(value->basic_type()))),
      value->zero_checked());
  }
}

void SharkCacher::process_method_slot(llvm::Value** value, int offset)
{
  // Cache the method pointer
  *value = builder()->CreateLoad(
    function()->CreateAddressOfFrameEntry(
      offset,
      SharkType::methodOop_type()));
}

void SharkCacher::process_local_slot(int          index,
                                     SharkValue** addr,
                                     int          offset)
{
  SharkValue *value = *addr;

  // Read the value from the frame if necessary
  if (local_slot_needs_read(index, value)) {
    *addr = SharkValue::create_generic(
      value->type(),
      builder()->CreateLoad(
        function()->CreateAddressOfFrameEntry(
          adjusted_offset(value, offset),
          SharkType::to_stackType(value->basic_type()))),
      value->zero_checked());
  }
}
