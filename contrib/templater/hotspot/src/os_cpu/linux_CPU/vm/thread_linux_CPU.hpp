/*
 * Copyright 2000-2007 Sun Microsystems, Inc.  All Rights Reserved.
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

private:
  void pd_initialize()
  {
    _anchor.clear();
  }

  frame pd_last_frame()
  {
    assert(has_last_Java_frame(), "must have last_Java_sp() when suspended");
    if (_anchor.last_Java_pc() != NULL) {
      return frame(_anchor.last_Java_sp(), _anchor.last_Java_pc());
    } else {
      // This will pick up pc from sp
      return frame(_anchor.last_Java_sp());
    }
  }

public:
  void set_base_of_stack_pointer(intptr_t* base_sp)
  {
#ifndef PPC
    Unimplemented();
#endif // PPC
  }

  void record_base_of_stack_pointer()
  {
#ifndef PPC
    Unimplemented();
#endif // PPC
  }

  bool pd_get_top_frame_for_signal_handler(frame* fr_addr,
                                           void* ucontext,
                                           bool isInJava)
  {
    Unimplemented();
  }

@@(cpu == "ia64" and """\
  static bool register_stack_overflow()
  {
    Unimplemented();
  }

  static void enable_register_stack_guard()
  {
    Unimplemented();
  }

  static void disable_register_stack_guard()
  {
    Unimplemented();
  }"""
   or """\
  // These routines are only used on cpu architectures that
  // have separate register stacks (Itanium).
  static bool register_stack_overflow() { return false; }
  static void enable_register_stack_guard() {}
  static void disable_register_stack_guard() {}""")@@
