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
  static llvm::Constant* _find_exception_handler;
  static llvm::Constant* _monitorenter;
  static llvm::Constant* _monitorexit;
  static llvm::Constant* _new_instance;
  static llvm::Constant* _newarray;
  static llvm::Constant* _anewarray;
  static llvm::Constant* _multianewarray;
  static llvm::Constant* _resolve_get_put;
  static llvm::Constant* _resolve_invoke;
  static llvm::Constant* _resolve_klass;
  static llvm::Constant* _safepoint;
  static llvm::Constant* _throw_ArrayIndexOutOfBoundsException;
  static llvm::Constant* _throw_NullPointerException;
  static llvm::Constant* _trace_bytecode;

 public:
  static llvm::Constant* find_exception_handler()
  {
    return _find_exception_handler;
  }
  static llvm::Constant* monitorenter()
  {
    return _monitorenter;
  }
  static llvm::Constant* monitorexit()
  {
    return _monitorexit;
  }
  static llvm::Constant* new_instance()
  {
    return _new_instance;
  }
  static llvm::Constant* newarray()
  {
    return _newarray;
  }
  static llvm::Constant* anewarray()
  {
    return _anewarray;
  }
  static llvm::Constant* multianewarray()
  {
    return _multianewarray;
  }
  static llvm::Constant* resolve_get_put()
  {
    return _resolve_get_put;
  }
  static llvm::Constant* resolve_invoke()
  {
    return _resolve_invoke;
  }
  static llvm::Constant* resolve_klass()
  {
    return _resolve_klass;
  }
  static llvm::Constant* safepoint()
  {
    return _safepoint;
  }
  static llvm::Constant* throw_ArrayIndexOutOfBoundsException()
  {
    return _throw_ArrayIndexOutOfBoundsException;
  }
  static llvm::Constant* throw_NullPointerException()
  {
    return _throw_NullPointerException;
  }  
  static llvm::Constant* trace_bytecode()
  {
    return _trace_bytecode;
  }

 private:
  static int find_exception_handler_C(JavaThread* thread,
                                      int*        indexes,
                                      int         num_indexes);

  static void monitorenter_C(JavaThread* thread, BasicObjectLock* lock);
  static void monitorexit_C(JavaThread* thread, BasicObjectLock* lock);

  static void new_instance_C(JavaThread* thread, int index);
  static void newarray_C(JavaThread* thread, BasicType type, int size);
  static void anewarray_C(JavaThread* thread, int index, int size);
  static void multianewarray_C(JavaThread* thread,
                               int         index,
                               int         ndims,
                               int*        dims);

  static void resolve_get_put_C(JavaThread*             thread,
                                ConstantPoolCacheEntry* entry,
                                int                     bci,
                                Bytecodes::Code         bytecode);
  static void resolve_invoke_C(JavaThread*             thread,
                               ConstantPoolCacheEntry* entry,
                               int                     bci,
                               Bytecodes::Code         bytecode);
  static void resolve_klass_C(JavaThread* thread, int index);
  static void throw_ArrayIndexOutOfBoundsException_C(JavaThread* thread,
                                                     const char* file,
                                                     int         line,
                                                     int         index);
  static void throw_NullPointerException_C(JavaThread* thread,
                                           const char* file,
                                           int         line);
  static void trace_bytecode_C(JavaThread* thread,
                               int         bci,
                               intptr_t    tos,
                               intptr_t    tos2);

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

  // Leaf calls
 private:
  static llvm::Constant* _f2i;
  static llvm::Constant* _f2l;
  static llvm::Constant* _d2i;
  static llvm::Constant* _d2l;

 public:
  static llvm::Constant* f2i()
  {
    return _f2i;
  }  
  static llvm::Constant* f2l()
  {
    return _f2l;
  }  
  static llvm::Constant* d2i()
  {
    return _d2i;
  }  
  static llvm::Constant* d2l()
  {
    return _d2l;
  }  
  
  // Non-VM calls
 private:
  static llvm::Constant* _dump;
  static llvm::Constant* _is_subtype_of;
  static llvm::Constant* _should_not_reach_here;
  static llvm::Constant* _unimplemented;
  static llvm::Constant* _uncommon_trap;

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
  static llvm::Constant* uncommon_trap()
  {
    return _uncommon_trap;
  }

 private:
  static void dump_C(const char *name, intptr_t value);
  static bool is_subtype_of_C(klassOop check_klass, klassOop object_klass); 
  static void uncommon_trap_C(JavaThread* thread, int index);
};
