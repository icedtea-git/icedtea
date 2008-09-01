/*
 * Copyright 2003-2007 Sun Microsystems, Inc.  All Rights Reserved.
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

class ZeroEntry {
 public:
  ZeroEntry()
  {
    ShouldNotCallThis();
  }

 public:
  typedef void (*method_entry_t)(methodOop method, intptr_t base_pc, TRAPS);

 private:
  method_entry_t _entry_point;

 public:
  method_entry_t entry_point() const
  {
    return _entry_point;
  }
  void set_entry_point(method_entry_t entry_point)
  {
    _entry_point = entry_point;
  }

 public:
  void invoke(methodOop method, TRAPS) const
  {
    entry_point()(method, (intptr_t) this, THREAD);
  }

 public:
  static ByteSize entry_point_offset()
  {
    return byte_offset_of(ZeroEntry, _entry_point);
  }
};
