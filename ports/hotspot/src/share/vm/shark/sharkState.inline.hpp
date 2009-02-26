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

inline SharkBuilder* SharkState::builder() const
{
  return block()->builder();
}  
  
inline SharkFunction* SharkState::function() const
{
  return block()->function();
}  
  
inline int SharkState::max_locals() const
{
  return block()->max_locals();
}

inline int SharkState::max_stack() const
{
  return block()->max_stack();
}

inline int SharkState::stack_depth_at_entry() const
{
  return block()->stack_depth_at_entry();
}

inline void SharkTrackingState::decache_for_Java_call(ciMethod* callee)
{
  assert(has_stack_frame(), "should do");
  SharkJavaCallDecacher(function(), block()->bci(), callee).scan(this);
  pop(callee->arg_size());
}

inline void SharkTrackingState::cache_after_Java_call(ciMethod* callee)
{
  assert(has_stack_frame(), "should do");
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

inline void SharkTrackingState::decache_for_VM_call()
{
  assert(has_stack_frame(), "should do");
  SharkVMCallDecacher(function(), block()->bci()).scan(this);
}

inline void SharkTrackingState::cache_after_VM_call()
{
  assert(has_stack_frame(), "should do");
  SharkVMCallCacher(function(), block()->bci()).scan(this);
}

inline void SharkTrackingState::decache_for_trap()
{
  assert(has_stack_frame(), "should do");
  SharkTrapDecacher(function(), block()->bci()).scan(this);
}
