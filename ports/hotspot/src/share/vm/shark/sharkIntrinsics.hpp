/*
 * Copyright 1999-2007 Sun Microsystems, Inc.  All Rights Reserved.
 * Copyright 2009 Red Hat, Inc.
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

class SharkIntrinsics : public AllStatic {
 public:
  static bool is_intrinsic(ciMethod* target);
  static void inline_intrinsic(ciMethod*    target,
                               SharkState*  state,
                               llvm::Value* thread);

 private:
  static void do_Math_minmax(SharkState* state, llvm::ICmpInst::Predicate p);
  static void do_Math_1to1(SharkState* state, llvm::Constant* function);
  static void do_Math_2to1(SharkState* state, llvm::Constant* function);
  static void do_Object_getClass(SharkState* state);
  static void do_System_currentTimeMillis(SharkState* state);
  static void do_Thread_currentThread(SharkState* state, llvm::Value* thread);
  static void do_Unsafe_compareAndSwapInt(SharkState* state);
};
