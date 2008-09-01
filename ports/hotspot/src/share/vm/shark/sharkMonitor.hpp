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

class SharkMonitor : public ResourceObj {
 public:
  SharkMonitor(const SharkFunction* function, llvm::Value* monitor)
    : _function(function), _monitor(monitor)
  { initialize(); }

 private:
  void initialize();

 private:
  const SharkFunction* _function;
  llvm::Value*         _monitor;
  llvm::Value*         _object_addr;
  llvm::Value*         _displaced_header_addr;

 private:
  const SharkFunction* function() const
  {
    return _function;
  }
  llvm::Value* monitor() const
  {
    return _monitor;
  }
  llvm::Value* object_addr() const
  {
    return _object_addr;
  }
  llvm::Value* displaced_header_addr() const
  {
    return _displaced_header_addr;
  }

 public:
  SharkBuilder* builder() const
  {
    return function()->builder();
  }

 public:
  llvm::Value* object() const
  {
    builder()->CreateLoad(object_addr());
  }
  void set_object(llvm::Value* object) const
  {
    builder()->CreateStore(object, object_addr());
  }
  llvm::Value* displaced_header() const
  {
    builder()->CreateLoad(displaced_header_addr());
  }
  void set_displaced_header(llvm::Value* displaced_header) const
  {
    builder()->CreateStore(displaced_header, displaced_header_addr());
  }

 public:
  void mark_free() const
  {
    set_object(LLVMValue::null());
  }

 public:
  void acquire(llvm::Value* lockee) const;
  void release() const;
};
