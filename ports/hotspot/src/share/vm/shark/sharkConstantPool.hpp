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

class SharkConstantPool : public StackObj {
 public:
  SharkConstantPool(SharkTopLevelBlock* block)
    : _block(block),
      _constants_method(NULL),
      _tags_constants(NULL),
      _cache_constants(NULL) {}

 private:
  SharkTopLevelBlock* _block;

 private:
  SharkTopLevelBlock* block() const
  {
    return _block;
  }
  SharkBuilder* builder() const
  {
    return block()->builder();
  }
  llvm::Value* method() const
  {
    return block()->method();
  }

 private:
  llvm::Value* _constants;        // The constant pool, a constantPoolOop
  llvm::Value* _constants_method; // The method _constants was loaded from

  llvm::Value* _tags;             // The tags array, a typeArrayOop
  llvm::Value* _tags_constants;   // The constantPoolOop _tags is in

  llvm::Value* _cache;            // The cache, a constantPoolCacheOop
  llvm::Value* _cache_constants;  // The constantPoolCacheOop _cache is in

 private:
  llvm::Value* constants();
  llvm::Value* tags();
  llvm::Value* cache();

 public:
  llvm::Value* object_at(int which);
  llvm::Value* tag_at(int which);
  llvm::Value* cache_entry_at(int which);
  llvm::Value* java_mirror();
};
