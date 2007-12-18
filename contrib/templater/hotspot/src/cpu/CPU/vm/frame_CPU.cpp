/*
 * Copyright 2003-2007 Sun Microsystems, Inc.  All Rights Reserved.
 * Copyright 2007 Red Hat, Inc.
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
#include "incls/_frame_@@cpu@@.cpp.incl"

#ifdef PPC
int frame::_call_wrapper_offset = 0;
#endif // PPC

#ifdef ASSERT
void RegisterMap::check_location_valid()
{
  Unimplemented();
}
#endif

bool frame::is_interpreted_frame() const
{
#ifdef PPC
  return Interpreter::contains(pc());
#else
  Unimplemented();
#endif // PPC
}

frame frame::sender_for_entry_frame(RegisterMap *map) const
{
#ifdef PPC
  assert(map != NULL, "map must be set");
  // Java frame called from C; skip all C frames and return top C
  // frame of that chunk as the sender
  JavaFrameAnchor* jfa = entry_frame_call_wrapper()->anchor();
  assert(!entry_frame_is_first(), "next Java fp must be non zero");
  assert(jfa->last_Java_sp() > sp(), "must be above this frame on stack");  
  map->clear(); 
  assert(map->include_argument_oops(), "should be set by clear");
  return frame(jfa->last_Java_sp(), jfa->last_Java_pc());
#else
  Unimplemented();
#endif // PPC
}

frame frame::sender_for_interpreter_frame(RegisterMap *map) const
{
#ifdef PPC
  return frame(sender_sp());
#else
  Unimplemented();
#endif // PPC
}

frame frame::sender(RegisterMap* map) const
{
  // Default is not to follow arguments; the various
  // sender_for_xxx methods update this accordingly.
  map->set_include_argument_oops(false);

  if (is_entry_frame())
    return sender_for_entry_frame(map);

  if (is_interpreted_frame())
    return sender_for_interpreter_frame(map);

  Unimplemented();
}

#ifdef CC_INTERP
BasicObjectLock* frame::interpreter_frame_monitor_begin() const
{
  return get_interpreterState()->monitor_base();
}

BasicObjectLock* frame::interpreter_frame_monitor_end() const
{
  return (BasicObjectLock*) get_interpreterState()->stack_base();
}
#endif // CC_INTERP

void frame::patch_pc(Thread* thread, address pc)
{
  Unimplemented();
}

bool frame::safe_for_sender(JavaThread *thread)
{
  Unimplemented();
}

void frame::pd_gc_epilog()
{
#ifndef PPC
  Unimplemented();
#endif
}

bool frame::is_interpreted_frame_valid() const
{
  Unimplemented();
}

BasicType frame::interpreter_frame_result(oop* oop_result,
					  jvalue* value_result)
{
  Unimplemented();
}

int frame::frame_size() const
{
  Unimplemented();
}

intptr_t* frame::interpreter_frame_tos_at(jint offset) const
{
  int index = (Interpreter::expr_offset_in_bytes(offset) / wordSize);
  return &interpreter_frame_tos_address()[index];
}
