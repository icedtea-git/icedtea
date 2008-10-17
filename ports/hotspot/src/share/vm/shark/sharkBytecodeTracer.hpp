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

class SharkBytecodeTracer : public AllStatic {
 public:
  static void decode(SharkBuilder*     builder,
                     const SharkState* state,
                     llvm::Value**     tos,
                     llvm::Value**     tos2);
 public:
  static const intptr_t EMPTY_SLOT =
    NOT_LP64(0x23232323) LP64_ONLY(0x2323232323232323);

  static const intptr_t UNDECODABLE_SLOT =
    NOT_LP64(0xdeadbabe) LP64_ONLY(0xdeadbabedeadbabe);

 private:
  static void decode_one_word(SharkBuilder*     builder,
                              const SharkState* state,
                              int               index,
                              llvm::Value**     dst);
  static void decode_two_word(SharkBuilder*     builder,
                              const SharkState* state,
                              int               index,
                              llvm::Value**     dst,
                              llvm::Value**     dst2);
};
