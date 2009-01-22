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

class SharkEntry : public ZeroEntry {
 private:
  llvm::Function* _llvm_function;

 public:
  llvm::Function* llvm_function() const
  {
    return _llvm_function;
  }
  void set_llvm_function(llvm::Function* llvm_function)
  {
    _llvm_function = llvm_function;
  }

 public:
  static ByteSize llvm_function_offset()
  {
    return byte_offset_of(SharkEntry, _llvm_function);
  }

 public:
  void print_statistics(const char* name) const PRODUCT_RETURN;

#ifndef PRODUCT
 private:
  address code_start() const
  {
    return start;
  }
  address code_limit() const
  {
    return limit;
  }
  void print_pd_statistics(address start, address limit) const;

  address start, limit;

public:
  void setBounds(unsigned char *FunctionStart, unsigned char *FunctionEnd)
  {
    start = (address)FunctionStart;
    limit = (address)FunctionEnd;
  }

#endif // !PRODUCT
};
