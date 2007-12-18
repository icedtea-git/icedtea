/*
 * Copyright 2003-2005 Sun Microsystems, Inc.  All Rights Reserved.
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

  // This file holds the platform specific parts of the StubRoutines
  // definition. See stubRoutines.hpp for a description on how to
  // extend it.

 public:
  static bool returns_to_call_stub(address return_pc)
  {
#ifdef PPC
    return return_pc == _call_stub_return_address;
#else
    Unimplemented();
#endif // PPC
  }

  enum platform_dependent_constants 
  {
    code_size1 =  4 * K, // The assembler will fail with a guarantee
    code_size2 = 12 * K  // if these are too small.  Simply increase
  };                     // them if that happens.

#ifdef PPC
 private:
  static int _call_stub_base_size;

 public:
  static int call_stub_base_size()
  {
    assert(_call_stub_base_size != 0, "call_stub_base_size not set");
    return _call_stub_base_size;
  }

  static void set_call_stub_base_size(int size)
  {
    assert(_call_stub_base_size == 0, "call_stub_base_size already set");
    _call_stub_base_size = size;
  }
#endif // PPC
