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

  // A frame represents a physical stack frame (an activation).  Frames
  // can be C or Java frames, and the Java frames can be interpreted or
  // compiled.  In contrast, vframes represent source-level activations,
  // so that one physical frame can correspond to multiple source level
  // frames because of inlining.  A frame is comprised of {pc, sp}

#ifdef PPC
 public:
  enum {
    pc_return_offset = 0
  };

#endif // PPC
 public:
  // Constructors
  frame(intptr_t* sp);
  frame(intptr_t* sp, address pc);

  // accessors for the instance variables
  intptr_t* fp() const
  {
#ifdef PPC
    return sp();
#else
    Unimplemented();
#endif
  }

#ifdef CC_INTERP
  inline interpreterState get_interpreterState() const;
#endif // CC_INTERP

#ifdef PPC
 private:
  static int _call_wrapper_offset;

 public:
  static int call_wrapper_offset()
  {
    assert(_call_wrapper_offset != 0, "call_wrapper_offset not set");
    return _call_wrapper_offset;
  }

  static void set_call_wrapper_offset(int offset)
  {
    assert(_call_wrapper_offset == 0, "call_wrapper_offset already set");
    _call_wrapper_offset = offset;
  }
#endif // PPC
