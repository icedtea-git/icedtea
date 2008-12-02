/*
 * Copyright 2003-2007 Sun Microsystems, Inc.  All Rights Reserved.
 * Copyright 2007, 2008 Red Hat, Inc.
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
#include "incls/_frame_zero.cpp.incl"

#ifdef ASSERT
void RegisterMap::check_location_valid()
{
  Unimplemented();
}
#endif

bool frame::is_interpreted_frame() const
{
  return zeroframe()->is_interpreter_frame();
}

bool frame::is_deoptimizer_frame() const
{
  return zeroframe()->is_deoptimizer_frame();
}

frame frame::sender_for_entry_frame(RegisterMap *map) const
{
  assert(map != NULL, "map must be set");
  assert(!entry_frame_is_first(), "next Java fp must be non zero");
  assert(entry_frame_call_wrapper()->anchor()->last_Java_sp() == sender_sp(),
         "sender should be next Java frame");
  map->clear();
  assert(map->include_argument_oops(), "should be set by clear");
  return frame(sender_sp());
}

frame frame::sender_for_interpreter_frame(RegisterMap *map) const
{
  return frame(sender_sp());
}

frame frame::sender_for_compiled_frame(RegisterMap *map) const
{
  return frame(sender_sp());
}

frame frame::sender_for_deoptimizer_frame(RegisterMap *map) const
{
  return frame(sender_sp());
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

  assert(_cb == CodeCache::find_blob(pc()),"Must be the same");
  if (_cb != NULL) {
    return sender_for_compiled_frame(map);
  }

  if (is_deoptimizer_frame())
    return sender_for_deoptimizer_frame(map);

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
#ifdef SHARK
  // We borrow this call to set the thread pointer in the interpreter
  // state; the hook to set up deoptimized frames isn't supplied it.
  assert(pc == NULL, "should be");
  get_interpreterState()->set_thread((JavaThread *) thread);
#else
  Unimplemented();
#endif // SHARK
}

bool frame::safe_for_sender(JavaThread *thread)
{
  Unimplemented();
}

void frame::pd_gc_epilog()
{
}

bool frame::is_interpreted_frame_valid(JavaThread *thread) const
{
  Unimplemented();
}

BasicType frame::interpreter_frame_result(oop* oop_result,
                                          jvalue* value_result)
{
  assert(is_interpreted_frame(), "interpreted frame expected");
  methodOop method = interpreter_frame_method();
  BasicType type = method->result_type();
  intptr_t* tos_addr = (intptr_t *) interpreter_frame_tos_address();
  oop obj;

  switch (type) {
  case T_VOID:
    break;
  case T_BOOLEAN:
    value_result->z = *(jboolean *) tos_addr;
    break;
  case T_BYTE:
    value_result->b = *(jbyte *) tos_addr;
    break;
  case T_CHAR:
    value_result->c = *(jchar *) tos_addr;
    break;
  case T_SHORT:
    value_result->s = *(jshort *) tos_addr;
    break;
  case T_INT:
    value_result->i = *(jint *) tos_addr;
    break;
  case T_LONG:
    value_result->j = *(jlong *) tos_addr;
    break;
  case T_FLOAT:
    value_result->f = *(jfloat *) tos_addr;
    break;
  case T_DOUBLE:
    value_result->d = *(jdouble *) tos_addr;
    break;

  case T_OBJECT: 
  case T_ARRAY:
    if (method->is_native()) {
      obj = get_interpreterState()->oop_temp();
    }
    else {
      oop* obj_p = (oop *) tos_addr;
      obj = (obj_p == NULL) ? (oop) NULL : *obj_p;
    }
    assert(obj == NULL || Universe::heap()->is_in(obj), "sanity check");
    *oop_result = obj;
    break;

  default:
    ShouldNotReachHere();
  }

  return type;
}

int frame::frame_size() const
{
#ifdef PRODUCT
  ShouldNotCallThis();
#else
  return 0; // make javaVFrame::print_value work
#endif // PRODUCT
}

intptr_t* frame::interpreter_frame_tos_at(jint offset) const
{
  int index = (Interpreter::expr_offset_in_bytes(offset) / wordSize);
  return &interpreter_frame_tos_address()[index];
}
