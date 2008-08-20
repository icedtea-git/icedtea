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

class SharkRuntime : public AllStatic {
 public:
  static void initialize(SharkBuilder* builder);

  // VM calls
 private:
  static llvm::Constant* _newarray;
  static llvm::Constant* _new_instance;
  static llvm::Constant* _resolve_get_put;
  static llvm::Constant* _resolve_invoke;

 public:
  static llvm::Constant* newarray()
  {
    return _newarray;
  }
  static llvm::Constant* new_instance()
  {
    return _new_instance;
  }
  static llvm::Constant* resolve_get_put()
  {
    return _resolve_get_put;
  }
  static llvm::Constant* resolve_invoke()
  {
    return _resolve_invoke;
  }

 private:
  static void newarray_C(JavaThread* thread, BasicType type, int size);
  static void new_instance_C(JavaThread* thread, klassOop klass);
  static void resolve_get_put_C(JavaThread*             thread,
                                ConstantPoolCacheEntry* entry,
                                int                     bci,
                                Bytecodes::Code         bytecode);
  static void resolve_invoke_C(JavaThread*             thread,
                               ConstantPoolCacheEntry* entry,
                               int                     bci,
                               Bytecodes::Code         bytecode);

  // Helpers for VM calls
 private:
  static const SharkFrame* last_frame(JavaThread *thread)
  {
    return thread->last_frame().zero_sharkframe();
  }
  static methodOop method(JavaThread *thread)
  {
    return last_frame(thread)->method();
  }
  static address bcp(JavaThread *thread, int bci)
  {
    return method(thread)->code_base() + bci;
  }
  static int two_byte_index(JavaThread *thread, int bci)
  {
    return Bytes::get_Java_u2(bcp(thread, bci) + 1);
  }
  static intptr_t tos_at(JavaThread *thread, int offset)
  {
    return *(thread->zero_stack()->sp() + offset);
  }  

  // Non-VM calls
 private:
  static llvm::Constant* _dump;
  static llvm::Constant* _is_subtype_of;
  static llvm::Constant* _should_not_reach_here;
  static llvm::Constant* _unimplemented;

 public:
  static llvm::Constant* dump()
  {
    return _dump;
  }
  static llvm::Constant* is_subtype_of()
  {
    return _is_subtype_of;
  }
  static llvm::Constant* should_not_reach_here()
  {
    return _should_not_reach_here;
  }
  static llvm::Constant* unimplemented()
  {
    return _unimplemented;
  }

 private:
  static void dump_C(const char *name, intptr_t value);
  static bool is_subtype_of_C(klassOop check_klass, klassOop object_klass); 
};
