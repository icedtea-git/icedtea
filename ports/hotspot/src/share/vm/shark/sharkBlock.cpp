/*
 * Copyright 1999-2007 Sun Microsystems, Inc.  All Rights Reserved.
 * Copyright 2008, 2009 Red Hat, Inc.
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

#include "incls/_precompiled.incl"
#include "incls/_sharkBlock.cpp.incl"
#include "ciArrayKlass.hpp" // XXX fuck you makeDeps
#include "ciObjArrayKlass.hpp" // XXX likewise

using namespace llvm;

void SharkBlock::enter(SharkBlock* predecessor, bool is_exception)
{
  // This block requires phis:
  //  - if it is entered more than once
  //  - if it is an exception handler, because in which
  //    case we assume it's entered more than once.
  //  - if the predecessor will be compiled after this
  //    block, in which case we can't simple propagate
  //    the state forward.
  if (!needs_phis() &&
      (entered() ||
       is_exception ||
       (predecessor && predecessor->index() >= index())))
    _needs_phis = true;

  // Recurse into the tree
  if (!entered()) {
    _entered = true;

    if (!has_trap()) {
      for (int i = 0; i < num_successors(); i++) {
        successor(i)->enter(this, false);
      }
      for (int i = 0; i < num_exceptions(); i++) {
        exception(i)->enter(this, true);
      }
    }
  }
}
  
void SharkBlock::initialize()
{
  char name[28];
  snprintf(name, sizeof(name),
           "bci_%d%s",
           start(), is_private_copy() ? "_private_copy" : "");
  _entry_block = function()->CreateBlock(name);
}

void SharkBlock::acquire_method_lock()
{
  Value *object;
  if (target()->is_static()) {
    SharkConstantPool constants(this);
    object = constants.java_mirror();
  }
  else {
    object = local(0)->jobject_value();
  }
  iter()->force_bci(start()); // for the decache
  function()->monitor(0)->acquire(this, object);
  check_pending_exception(false);
}

void SharkBlock::release_method_lock()
{
  function()->monitor(0)->release(this);

  // We neither need nor want to check for pending exceptions here.
  // This method is only called by handle_return, which copes with
  // them implicitly:
  //  - if a value is being returned then we just carry on as normal;
  //    the caller will see the pending exception and handle it.
  //  - if an exception is being thrown then that exception takes
  //    priority and ours will be ignored.
}

void SharkBlock::parse()
{
  SharkValue *a, *b, *c, *d;
  int i;

  builder()->SetInsertPoint(entry_block());

  if (has_trap()) {
    iter()->force_bci(start());

    current_state()->decache_for_trap();
    builder()->CreateCall2(
      SharkRuntime::uncommon_trap(),
      thread(),
      LLVMValue::jint_constant(trap_index()));
    builder()->CreateRetVoid();
    return;
  }

  iter()->reset_to_bci(start());
  bool successors_done = false;
  while (iter()->next() != ciBytecodeStream::EOBC() && bci() < limit()) {
    NOT_PRODUCT(a = b = c = d = NULL);    

    if (TraceBytecodes) {
      Value *tos, *tos2;
      SharkBytecodeTracer::decode(builder(), current_state(), &tos, &tos2);
      call_vm(
        SharkRuntime::trace_bytecode(),
        LLVMValue::jint_constant(bci()),
        tos, tos2);
    }
    
    if (SharkTraceBytecodes)
      tty->print_cr("%4d: %s", bci(), Bytecodes::name(bc()));
    
    if(UseLoopSafepoints) {
      int len;

      // XXX if a lcmp is followed by an if_?? then C2 maybe-inserts
      // the safepoint before the lcmp rather than before the if.
      // Maybe we should do this too.  See parse2.cpp for details.

      switch (bc()) {
      case Bytecodes::_goto:
      case Bytecodes::_ifnull:
      case Bytecodes::_ifnonnull:
      case Bytecodes::_if_acmpeq:
      case Bytecodes::_if_acmpne:
      case Bytecodes::_ifeq:
      case Bytecodes::_ifne:
      case Bytecodes::_iflt:
      case Bytecodes::_ifle:
      case Bytecodes::_ifgt:
      case Bytecodes::_ifge:
      case Bytecodes::_if_icmpeq:
      case Bytecodes::_if_icmpne:
      case Bytecodes::_if_icmplt:
      case Bytecodes::_if_icmple:
      case Bytecodes::_if_icmpgt:
      case Bytecodes::_if_icmpge:
        if (iter()->get_dest() <= bci())
          add_safepoint();
        break;

      case Bytecodes::_goto_w:
        if (iter()->get_far_dest() <= bci())
          add_safepoint();
        break;

      case Bytecodes::_tableswitch:
      case Bytecodes::_lookupswitch:
        if (switch_default_dest() <= bci()) {
          add_safepoint();
          break;
        }
        len = switch_table_length();
        for (i = 0; i < len; i++) {
          if (switch_dest(i) <= bci()) {
            add_safepoint();
            break;
          }
        }
        break;
      }
    }

    switch (bc()) {
    case Bytecodes::_nop:
      break;

    case Bytecodes::_aconst_null:
      push(SharkValue::null());
      break;

    case Bytecodes::_iconst_m1:
      push(SharkValue::jint_constant(-1));
      break;
    case Bytecodes::_iconst_0:
      push(SharkValue::jint_constant(0));
      break;
    case Bytecodes::_iconst_1:
      push(SharkValue::jint_constant(1));
      break;
    case Bytecodes::_iconst_2:
      push(SharkValue::jint_constant(2));
      break;
    case Bytecodes::_iconst_3:
      push(SharkValue::jint_constant(3));
      break;
    case Bytecodes::_iconst_4:
      push(SharkValue::jint_constant(4));
      break;
    case Bytecodes::_iconst_5:
      push(SharkValue::jint_constant(5));
      break;

    case Bytecodes::_lconst_0:
      push(SharkValue::jlong_constant(0));
      break;
    case Bytecodes::_lconst_1:
      push(SharkValue::jlong_constant(1));
      break;

    case Bytecodes::_fconst_0:
      push(SharkValue::jfloat_constant(0));
      break;
    case Bytecodes::_fconst_1:
      push(SharkValue::jfloat_constant(1));
      break;
    case Bytecodes::_fconst_2:
      push(SharkValue::jfloat_constant(2));
      break;

    case Bytecodes::_dconst_0:
      push(SharkValue::jdouble_constant(0));
      break;
    case Bytecodes::_dconst_1:
      push(SharkValue::jdouble_constant(1));
      break;

    case Bytecodes::_bipush:
      push(SharkValue::jint_constant(iter()->get_byte()));
      break;
    case Bytecodes::_sipush:
      push(SharkValue::jint_constant(iter()->get_short()));
      break;

    case Bytecodes::_ldc:
    case Bytecodes::_ldc_w:
    case Bytecodes::_ldc2_w:
      do_ldc();
      break;

    case Bytecodes::_iload_0:
    case Bytecodes::_lload_0:
    case Bytecodes::_fload_0:
    case Bytecodes::_dload_0:
    case Bytecodes::_aload_0:
      push(local(0));
      break;
    case Bytecodes::_iload_1:
    case Bytecodes::_lload_1:
    case Bytecodes::_fload_1:
    case Bytecodes::_dload_1:
    case Bytecodes::_aload_1:
      push(local(1));
      break;
    case Bytecodes::_iload_2:
    case Bytecodes::_lload_2:
    case Bytecodes::_fload_2:
    case Bytecodes::_dload_2:
    case Bytecodes::_aload_2:
      push(local(2));
      break;
    case Bytecodes::_iload_3:
    case Bytecodes::_lload_3:
    case Bytecodes::_fload_3:
    case Bytecodes::_dload_3:
    case Bytecodes::_aload_3:
      push(local(3));
      break;
    case Bytecodes::_iload:
    case Bytecodes::_lload:
    case Bytecodes::_fload:
    case Bytecodes::_dload:
    case Bytecodes::_aload:
      push(local(iter()->get_index()));
      break;

    case Bytecodes::_baload:
      do_aload(T_BYTE);
      break;
    case Bytecodes::_caload:
      do_aload(T_CHAR);
      break;
    case Bytecodes::_saload:
      do_aload(T_SHORT);
      break;
    case Bytecodes::_iaload:
      do_aload(T_INT);
      break;
    case Bytecodes::_laload:
      do_aload(T_LONG);
      break;
    case Bytecodes::_faload:
      do_aload(T_FLOAT);
      break;
    case Bytecodes::_daload:
      do_aload(T_DOUBLE);
      break;
    case Bytecodes::_aaload:
      do_aload(T_OBJECT);
      break;

    case Bytecodes::_istore_0:
    case Bytecodes::_lstore_0:
    case Bytecodes::_fstore_0:
    case Bytecodes::_dstore_0:
    case Bytecodes::_astore_0:
      set_local(0, pop());
      break;
    case Bytecodes::_istore_1:
    case Bytecodes::_lstore_1:
    case Bytecodes::_fstore_1:
    case Bytecodes::_dstore_1:
    case Bytecodes::_astore_1:
      set_local(1, pop());
      break;
    case Bytecodes::_istore_2:
    case Bytecodes::_lstore_2:
    case Bytecodes::_fstore_2:
    case Bytecodes::_dstore_2:
    case Bytecodes::_astore_2:
      set_local(2, pop());
      break;
    case Bytecodes::_istore_3:
    case Bytecodes::_lstore_3:
    case Bytecodes::_fstore_3:
    case Bytecodes::_dstore_3:
    case Bytecodes::_astore_3:
      set_local(3, pop());
      break;
    case Bytecodes::_istore:
    case Bytecodes::_lstore:
    case Bytecodes::_fstore:
    case Bytecodes::_dstore:
    case Bytecodes::_astore:
      set_local(iter()->get_index(), pop());
      break;

    case Bytecodes::_bastore:
      do_astore(T_BYTE);
      break;
    case Bytecodes::_castore:
      do_astore(T_CHAR);
      break;
    case Bytecodes::_sastore:
      do_astore(T_SHORT);
      break;
    case Bytecodes::_iastore:
      do_astore(T_INT);
      break;
    case Bytecodes::_lastore:
      do_astore(T_LONG);
      break;
    case Bytecodes::_fastore:
      do_astore(T_FLOAT);
      break;
    case Bytecodes::_dastore:
      do_astore(T_DOUBLE);
      break;
    case Bytecodes::_aastore:
      do_astore(T_OBJECT);
      break;

    case Bytecodes::_pop:
      xpop();
      break;
    case Bytecodes::_pop2:
      xpop();
      xpop();
      break;
    case Bytecodes::_swap: 
      a = xpop();
      b = xpop();
      xpush(a);
      xpush(b);
      break;  
    case Bytecodes::_dup:
      a = xpop();
      xpush(a);
      xpush(a);
      break;
    case Bytecodes::_dup_x1: 
      a = xpop();
      b = xpop();
      xpush(a);
      xpush(b);
      xpush(a);
      break;
    case Bytecodes::_dup_x2: 
      a = xpop();
      b = xpop();
      c = xpop();
      xpush(a);
      xpush(c);
      xpush(b);
      xpush(a);
      break;
    case Bytecodes::_dup2: 
      a = xpop();
      b = xpop();
      xpush(b);
      xpush(a);
      xpush(b);
      xpush(a);
      break;
    case Bytecodes::_dup2_x1:
      a = xpop();
      b = xpop();
      c = xpop();
      xpush(b);
      xpush(a);
      xpush(c);
      xpush(b);
      xpush(a);
      break;
    case Bytecodes::_dup2_x2:
      a = xpop();
      b = xpop();
      c = xpop();
      d = xpop();
      xpush(b);
      xpush(a);
      xpush(d);
      xpush(c);
      xpush(b);
      xpush(a);
      break;

    case Bytecodes::_arraylength:
      do_arraylength();
      break;

    case Bytecodes::_getfield:
      do_getfield();
      break;
    case Bytecodes::_getstatic:
      do_getstatic();
      break;
    case Bytecodes::_putfield:
      do_putfield();
      break;
    case Bytecodes::_putstatic:
      do_putstatic();
      break;

    case Bytecodes::_iadd:
      b = pop();
      a = pop();
      push(SharkValue::create_jint(
        builder()->CreateAdd(a->jint_value(), b->jint_value())));
      break;
    case Bytecodes::_isub:
      b = pop();
      a = pop();
      push(SharkValue::create_jint(
        builder()->CreateSub(a->jint_value(), b->jint_value())));
      break;
    case Bytecodes::_imul:
      b = pop();
      a = pop();
      push(SharkValue::create_jint(
        builder()->CreateMul(a->jint_value(), b->jint_value())));
      break;
    case Bytecodes::_idiv:
      do_idiv();
      break;
    case Bytecodes::_irem:
      do_irem();
      break;
    case Bytecodes::_ineg:
      a = pop();      
      push(SharkValue::create_jint(
        builder()->CreateNeg(a->jint_value())));
      break;
    case Bytecodes::_ishl:
      b = pop();
      a = pop();
      push(SharkValue::create_jint(
        builder()->CreateShl(
          a->jint_value(),
          builder()->CreateAnd(
            b->jint_value(), LLVMValue::jint_constant(0x1f)))));
      break;
    case Bytecodes::_ishr:
      b = pop();
      a = pop();
      push(SharkValue::create_jint(
        builder()->CreateAShr(
          a->jint_value(),
          builder()->CreateAnd(
            b->jint_value(), LLVMValue::jint_constant(0x1f)))));
      break;
    case Bytecodes::_iushr:
      b = pop();
      a = pop();
      push(SharkValue::create_jint(
        builder()->CreateLShr(
          a->jint_value(),
          builder()->CreateAnd(
            b->jint_value(), LLVMValue::jint_constant(0x1f)))));
      break;
    case Bytecodes::_iand:
      b = pop();
      a = pop();
      push(SharkValue::create_jint(
        builder()->CreateAnd(a->jint_value(), b->jint_value())));
      break;
    case Bytecodes::_ior:
      b = pop();
      a = pop();
      push(SharkValue::create_jint(
        builder()->CreateOr(a->jint_value(), b->jint_value())));
      break;
    case Bytecodes::_ixor:
      b = pop();
      a = pop();
      push(SharkValue::create_jint(
        builder()->CreateXor(a->jint_value(), b->jint_value())));
      break;

    case Bytecodes::_ladd:
      b = pop();
      a = pop();
      push(SharkValue::create_jlong(
        builder()->CreateAdd(a->jlong_value(), b->jlong_value())));
      break;
    case Bytecodes::_lsub:
      b = pop();
      a = pop();
      push(SharkValue::create_jlong(
        builder()->CreateSub(a->jlong_value(), b->jlong_value())));
      break;
    case Bytecodes::_lmul:
      b = pop();
      a = pop();
      push(SharkValue::create_jlong(
        builder()->CreateMul(a->jlong_value(), b->jlong_value())));
      break;
    case Bytecodes::_ldiv:
      do_ldiv();
      break;
    case Bytecodes::_lrem:
      do_lrem();
      break;
    case Bytecodes::_lneg:
      a = pop();      
      push(SharkValue::create_jlong(
        builder()->CreateNeg(a->jlong_value())));
      break;
    case Bytecodes::_lshl:
      b = pop();
      a = pop();
      push(SharkValue::create_jlong(
        builder()->CreateShl(
          a->jlong_value(),
          builder()->CreateIntCast(
            builder()->CreateAnd(
              b->jint_value(), LLVMValue::jint_constant(0x3f)),
            SharkType::jlong_type(), true))));
      break;
    case Bytecodes::_lshr:
      b = pop();
      a = pop();
      push(SharkValue::create_jlong(
        builder()->CreateAShr(
          a->jlong_value(),
          builder()->CreateIntCast(
            builder()->CreateAnd(
              b->jint_value(), LLVMValue::jint_constant(0x3f)),
            SharkType::jlong_type(), true))));
      break;
    case Bytecodes::_lushr:
      b = pop();
      a = pop();
      push(SharkValue::create_jlong(
        builder()->CreateLShr(
          a->jlong_value(),
          builder()->CreateIntCast(
            builder()->CreateAnd(
              b->jint_value(), LLVMValue::jint_constant(0x3f)),
            SharkType::jlong_type(), true))));
      break;
    case Bytecodes::_land:
      b = pop();
      a = pop();
      push(SharkValue::create_jlong(
        builder()->CreateAnd(a->jlong_value(), b->jlong_value())));
      break;
    case Bytecodes::_lor:
      b = pop();
      a = pop();
      push(SharkValue::create_jlong(
        builder()->CreateOr(a->jlong_value(), b->jlong_value())));
      break;
    case Bytecodes::_lxor:
      b = pop();
      a = pop();
      push(SharkValue::create_jlong(
        builder()->CreateXor(a->jlong_value(), b->jlong_value())));
      break;

    case Bytecodes::_fadd:
      b = pop();
      a = pop();
      push(SharkValue::create_jfloat(
        builder()->CreateAdd(a->jfloat_value(), b->jfloat_value())));
      break;
    case Bytecodes::_fsub:
      b = pop();
      a = pop();
      push(SharkValue::create_jfloat(
        builder()->CreateSub(a->jfloat_value(), b->jfloat_value())));
      break;
    case Bytecodes::_fmul:
      b = pop();
      a = pop();
      push(SharkValue::create_jfloat(
        builder()->CreateMul(a->jfloat_value(), b->jfloat_value())));
      break;
    case Bytecodes::_fdiv:
      b = pop();
      a = pop();      
      push(SharkValue::create_jfloat(
        builder()->CreateFDiv(a->jfloat_value(), b->jfloat_value())));
      break;
    case Bytecodes::_frem:
      b = pop();
      a = pop();      
      push(SharkValue::create_jfloat(
        builder()->CreateFRem(a->jfloat_value(), b->jfloat_value())));
      break;
    case Bytecodes::_fneg:
      a = pop();      
      push(SharkValue::create_jfloat(
        builder()->CreateNeg(a->jfloat_value())));
      break;

    case Bytecodes::_dadd:
      b = pop();
      a = pop();
      push(SharkValue::create_jdouble(
        builder()->CreateAdd(a->jdouble_value(), b->jdouble_value())));
      break;
    case Bytecodes::_dsub:
      b = pop();
      a = pop();
      push(SharkValue::create_jdouble(
        builder()->CreateSub(a->jdouble_value(), b->jdouble_value())));
      break;
    case Bytecodes::_dmul:
      b = pop();
      a = pop();
      push(SharkValue::create_jdouble(
        builder()->CreateMul(a->jdouble_value(), b->jdouble_value())));
      break;
    case Bytecodes::_ddiv:
      b = pop();
      a = pop();      
      push(SharkValue::create_jdouble(
        builder()->CreateFDiv(a->jdouble_value(), b->jdouble_value())));
      break;
    case Bytecodes::_drem:
      b = pop();
      a = pop();      
      push(SharkValue::create_jdouble(
        builder()->CreateFRem(a->jdouble_value(), b->jdouble_value())));
      break;
    case Bytecodes::_dneg:
      a = pop();      
      push(SharkValue::create_jdouble(
        builder()->CreateNeg(a->jdouble_value())));
      break;

    case Bytecodes::_iinc:
      i = iter()->get_index();
      set_local(
        i,
        SharkValue::create_jint(
          builder()->CreateAdd(
            LLVMValue::jint_constant(iter()->get_iinc_con()),
            local(i)->jint_value())));
      break;

    case Bytecodes::_lcmp:
      do_lcmp();
      break;

    case Bytecodes::_fcmpl:
      do_fcmp(false, false);
      break;
    case Bytecodes::_fcmpg:
      do_fcmp(false, true);
      break;
    case Bytecodes::_dcmpl:
      do_fcmp(true, false);
      break;
    case Bytecodes::_dcmpg:
      do_fcmp(true, true);
      break;

    case Bytecodes::_i2l:
      push(SharkValue::create_jlong(
        builder()->CreateIntCast(
          pop()->jint_value(), SharkType::jlong_type(), true)));
      break;
    case Bytecodes::_i2f:
      push(SharkValue::create_jfloat(
        builder()->CreateSIToFP(
          pop()->jint_value(), SharkType::jfloat_type())));
      break;
    case Bytecodes::_i2d:
      push(SharkValue::create_jdouble(
        builder()->CreateSIToFP(
          pop()->jint_value(), SharkType::jdouble_type())));
      break;

    case Bytecodes::_l2i:
      push(SharkValue::create_jint(
        builder()->CreateIntCast(
          pop()->jlong_value(), SharkType::jint_type(), true)));
      break;
    case Bytecodes::_l2f:
      push(SharkValue::create_jfloat(
        builder()->CreateSIToFP(
          pop()->jlong_value(), SharkType::jfloat_type())));
      break;
    case Bytecodes::_l2d:
      push(SharkValue::create_jdouble(
        builder()->CreateSIToFP(
          pop()->jlong_value(), SharkType::jdouble_type())));
      break;

    case Bytecodes::_f2i:
      push(SharkValue::create_jint(
        call_vm_leaf(SharkRuntime::f2i(), pop()->jfloat_value())));
      break;
    case Bytecodes::_f2l:
      push(SharkValue::create_jlong(
        call_vm_leaf(SharkRuntime::f2l(), pop()->jfloat_value())));
      break;
    case Bytecodes::_f2d:
      push(SharkValue::create_jdouble(
        builder()->CreateFPExt(
          pop()->jfloat_value(), SharkType::jdouble_type())));
      break;

    case Bytecodes::_d2i:
      push(SharkValue::create_jint(
        call_vm_leaf(SharkRuntime::d2i(), pop()->jdouble_value())));
      break;
    case Bytecodes::_d2l:
      push(SharkValue::create_jlong(
        call_vm_leaf(SharkRuntime::d2l(), pop()->jdouble_value())));
      break;
    case Bytecodes::_d2f:
      push(SharkValue::create_jfloat(
        builder()->CreateFPTrunc(
          pop()->jdouble_value(), SharkType::jfloat_type())));
      break;

    case Bytecodes::_i2b:
      push(SharkValue::create_jint(
        builder()->CreateAShr(
          builder()->CreateShl(
            pop()->jint_value(),
            LLVMValue::jint_constant(24)),
          LLVMValue::jint_constant(24))));
      break;
    case Bytecodes::_i2c:
      push(SharkValue::create_jint(
        builder()->CreateAnd(
            pop()->jint_value(),
            LLVMValue::jint_constant(0xffff))));
      break;
    case Bytecodes::_i2s:
      push(SharkValue::create_jint(
        builder()->CreateAShr(
          builder()->CreateShl(
            pop()->jint_value(),
            LLVMValue::jint_constant(16)),
          LLVMValue::jint_constant(16))));
      break;

    case Bytecodes::_return:
      do_return(T_VOID);
      break;
    case Bytecodes::_ireturn:
      do_return(T_INT);
      break;
    case Bytecodes::_lreturn:
      do_return(T_LONG);
      break;
    case Bytecodes::_freturn:
      do_return(T_FLOAT);
      break;
    case Bytecodes::_dreturn:
      do_return(T_DOUBLE);
      break;
    case Bytecodes::_areturn:
      do_return(T_OBJECT);
      break;

    case Bytecodes::_athrow:
      do_athrow();
      break;

    case Bytecodes::_goto:
    case Bytecodes::_goto_w:
      builder()->CreateBr(successor(ciTypeFlow::GOTO_TARGET)->entry_block());
      break;

    case Bytecodes::_jsr:
    case Bytecodes::_jsr_w:
      push(SharkValue::create_returnAddress(iter()->next_bci()));
      builder()->CreateBr(successor(ciTypeFlow::GOTO_TARGET)->entry_block());
      break;

    case Bytecodes::_ret:
      assert(local(iter()->get_index())->returnAddress_value() ==
             successor(ciTypeFlow::GOTO_TARGET)->start(), "should be");
      builder()->CreateBr(successor(ciTypeFlow::GOTO_TARGET)->entry_block());
      break;

    case Bytecodes::_ifnull:
      do_if(ICmpInst::ICMP_EQ, SharkValue::null(), pop());
      break;
    case Bytecodes::_ifnonnull:
      do_if(ICmpInst::ICMP_NE, SharkValue::null(), pop());
      break;
    case Bytecodes::_if_acmpeq:
      b = pop(); a = pop();
      do_if(ICmpInst::ICMP_EQ, b, a);
      break;
    case Bytecodes::_if_acmpne:
      b = pop(); a = pop();
      do_if(ICmpInst::ICMP_NE, b, a);
      break;
    case Bytecodes::_ifeq:
      do_if(ICmpInst::ICMP_EQ, SharkValue::jint_constant(0), pop());
      break;
    case Bytecodes::_ifne:
      do_if(ICmpInst::ICMP_NE, SharkValue::jint_constant(0), pop());
      break;
    case Bytecodes::_iflt:
      do_if(ICmpInst::ICMP_SLT, SharkValue::jint_constant(0), pop());
      break;
    case Bytecodes::_ifle:
      do_if(ICmpInst::ICMP_SLE, SharkValue::jint_constant(0), pop());
      break;
    case Bytecodes::_ifgt:
      do_if(ICmpInst::ICMP_SGT, SharkValue::jint_constant(0), pop());
      break;
    case Bytecodes::_ifge:
      do_if(ICmpInst::ICMP_SGE, SharkValue::jint_constant(0), pop());
      break;
    case Bytecodes::_if_icmpeq:
      b = pop(); a = pop();
      do_if(ICmpInst::ICMP_EQ, b, a);
      break;
    case Bytecodes::_if_icmpne:
      b = pop(); a = pop();
      do_if(ICmpInst::ICMP_NE, b, a);
      break;
    case Bytecodes::_if_icmplt:
      b = pop(); a = pop();
      do_if(ICmpInst::ICMP_SLT, b, a);
      break;
    case Bytecodes::_if_icmple:
      b = pop(); a = pop();
      do_if(ICmpInst::ICMP_SLE, b, a);
      break;
    case Bytecodes::_if_icmpgt:
      b = pop(); a = pop();
      do_if(ICmpInst::ICMP_SGT, b, a);
      break;
    case Bytecodes::_if_icmpge:
      b = pop(); a = pop();
      do_if(ICmpInst::ICMP_SGE, b, a);
      break;

    case Bytecodes::_tableswitch:
    case Bytecodes::_lookupswitch:
      do_switch();
      successors_done = true;
      break;

    case Bytecodes::_invokestatic:
    case Bytecodes::_invokespecial:
    case Bytecodes::_invokevirtual:
    case Bytecodes::_invokeinterface:
      do_call();
      break;

    case Bytecodes::_checkcast:
    case Bytecodes::_instanceof:
      do_instance_check();
      break;

    case Bytecodes::_new:
      do_new();
      break;
    case Bytecodes::_newarray:
      do_newarray();
      break;
    case Bytecodes::_anewarray:
      do_anewarray();
      break;
    case Bytecodes::_multianewarray:
      do_multianewarray();
      break;

    case Bytecodes::_monitorenter:
      do_monitorenter();
      break;
    case Bytecodes::_monitorexit:
      do_monitorexit();
      break;

    default:
      ShouldNotReachHere();
    }
  }

  if (falls_through()) {
    builder()->CreateBr(successor(ciTypeFlow::FALL_THROUGH)->entry_block());
  }

  if (!successors_done) {
    for (int i = 0; i < num_successors(); i++)
      successor(i)->add_incoming(current_state());
  }
}

SharkBlock* SharkBlock::bci_successor(int bci) const
{
  // XXX now with Linear Search Technology (tm)
  for (int i = 0; i < num_successors(); i++) {
    ciTypeFlow::Block *successor = ciblock()->successors()->at(i);
    if (successor->start() == bci)
      return function()->block(successor->pre_order());
  }
  ShouldNotReachHere();
}

void SharkBlock::check_zero(SharkValue *value)
{
  if (value->zero_checked())
    return;

  BasicBlock *zero     = function()->CreateBlock("zero");
  BasicBlock *not_zero = function()->CreateBlock("not_zero");

  Value *a, *b;
  switch (value->basic_type()) {
  case T_BYTE:
  case T_CHAR:
  case T_SHORT:
  case T_INT:
    a = value->jint_value();
    b = LLVMValue::jint_constant(0);
    break;
  case T_LONG:
    a = value->jlong_value();
    b = LLVMValue::jlong_constant(0);
    break;
  case T_OBJECT:
  case T_ARRAY:
    a = value->jobject_value();
    b = LLVMValue::LLVMValue::null();
    break;
  default:
    tty->print_cr("Unhandled type %s", type2name(value->basic_type()));
    ShouldNotReachHere();
  }

  builder()->CreateCondBr(builder()->CreateICmpNE(a, b), not_zero, zero);

  builder()->SetInsertPoint(zero);
  SharkTrackingState *saved_state = current_state()->copy();
  if (value->is_jobject()) {
    call_vm_nocheck(
      SharkRuntime::throw_NullPointerException(),
      builder()->pointer_constant(__FILE__),
      LLVMValue::jint_constant(__LINE__));
  }
  else {
    builder()->CreateUnimplemented(__FILE__, __LINE__);
  } 
  handle_exception(function()->CreateGetPendingException());
  set_current_state(saved_state);  

  builder()->SetInsertPoint(not_zero);

  value->set_zero_checked(true);
}

void SharkBlock::check_bounds(SharkValue* array, SharkValue* index)
{
  BasicBlock *out_of_bounds = function()->CreateBlock("out_of_bounds");
  BasicBlock *in_bounds     = function()->CreateBlock("in_bounds");

  Value *length = builder()->CreateArrayLength(array->jarray_value());
  // we use an unsigned comparison to catch negative values
  builder()->CreateCondBr(
    builder()->CreateICmpULT(index->jint_value(), length),
    in_bounds, out_of_bounds);

  builder()->SetInsertPoint(out_of_bounds);
  SharkTrackingState *saved_state = current_state()->copy();
  call_vm_nocheck(
    SharkRuntime::throw_ArrayIndexOutOfBoundsException(),
    builder()->pointer_constant(__FILE__),
    LLVMValue::jint_constant(__LINE__),
    index->jint_value());
  handle_exception(function()->CreateGetPendingException());
  set_current_state(saved_state);  

  builder()->SetInsertPoint(in_bounds);
}

void SharkBlock::check_pending_exception(bool attempt_catch)
{
  BasicBlock *exception    = function()->CreateBlock("exception");
  BasicBlock *no_exception = function()->CreateBlock("no_exception");

  Value *pending_exception_addr = function()->pending_exception_address();
  Value *pending_exception = builder()->CreateLoad(
    pending_exception_addr, "pending_exception");

  builder()->CreateCondBr(
    builder()->CreateICmpEQ(pending_exception, LLVMValue::null()),
    no_exception, exception);

  builder()->SetInsertPoint(exception);
  builder()->CreateStore(LLVMValue::null(), pending_exception_addr);
  SharkTrackingState *saved_state = current_state()->copy();
  handle_exception(pending_exception, attempt_catch);
  set_current_state(saved_state);

  builder()->SetInsertPoint(no_exception);
}

void SharkBlock::handle_exception(Value* exception, bool attempt_catch)
{
  if (attempt_catch && num_exceptions() != 0) {
    // Clear the stack and push the exception onto it.
    // We do this now to protect it across the VM call
    // we may be about to make.
    while (xstack_depth())
      pop();
    push(SharkValue::create_jobject(exception));

    int *indexes = NEW_RESOURCE_ARRAY(int, num_exceptions());
    bool has_catch_all = false;

    ciExceptionHandlerStream eh_iter(target(), bci());
    for (int i = 0; i < num_exceptions(); i++, eh_iter.next()) {
      ciExceptionHandler* handler = eh_iter.handler();

      if (handler->is_catch_all()) {
        assert(i == num_exceptions() - 1, "catch-all should be last");
        has_catch_all = true;
      }
      else {
        indexes[i] = handler->catch_klass_index();
      }
    }

    int num_options = num_exceptions();
    if (has_catch_all)
      num_options--;

    // Drop into the runtime if there are non-catch-all options
    if (num_options > 0) {
      Value *options = builder()->CreateAlloca(
        ArrayType::get(SharkType::jint_type(), num_options),
        LLVMValue::jint_constant(1));

      for (int i = 0; i < num_options; i++)
        builder()->CreateStore(
          LLVMValue::jint_constant(indexes[i]),
          builder()->CreateStructGEP(options, i));

      Value *index = call_vm_nocheck(
        SharkRuntime::find_exception_handler(),
        builder()->CreateStructGEP(options, 0),
        LLVMValue::jint_constant(num_options));
      check_pending_exception(false);

      // Jump to the exception handler, if found
      BasicBlock *no_handler = function()->CreateBlock("no_handler");
      SwitchInst *switchinst = builder()->CreateSwitch(
        index, no_handler, num_options);

      for (int i = 0; i < num_options; i++) {
        SharkBlock* handler = this->exception(i);

        switchinst->addCase(
          LLVMValue::jint_constant(i),
          handler->entry_block());

        handler->add_incoming(current_state());
      }

      builder()->SetInsertPoint(no_handler);
    }

    // No specific handler exists, but maybe there's a catch-all
    if (has_catch_all) {
      SharkBlock* handler = this->exception(num_options);

      builder()->CreateBr(handler->entry_block());
      handler->add_incoming(current_state());
      return;
    }
  }

  // No exception handler was found; unwind and return
  handle_return(T_VOID, exception);
}

void SharkBlock::add_safepoint()
{
  BasicBlock *orig_block = builder()->GetInsertBlock();
  SharkState *orig_state = current_state()->copy();

  BasicBlock *do_safepoint = function()->CreateBlock("do_safepoint");
  BasicBlock *safepointed  = function()->CreateBlock("safepointed");

  Value *state = builder()->CreateLoad(
    builder()->CreateIntToPtr(
      builder()->pointer_constant(SafepointSynchronize::address_of_state()),
      PointerType::getUnqual(SharkType::jint_type())),
    "state");

  builder()->CreateCondBr(
    builder()->CreateICmpEQ(
      state,
      LLVMValue::jint_constant(SafepointSynchronize::_synchronizing)),
    do_safepoint, safepointed);

  builder()->SetInsertPoint(do_safepoint);
  call_vm(SharkRuntime::safepoint());
  BasicBlock *safepointed_block = builder()->GetInsertBlock();  
  builder()->CreateBr(safepointed);

  builder()->SetInsertPoint(safepointed);
  current_state()->merge(orig_state, orig_block, safepointed_block);
}

void SharkBlock::call_register_finalizer(Value *receiver)
{
  BasicBlock *orig_block = builder()->GetInsertBlock();
  SharkState *orig_state = current_state()->copy();

  BasicBlock *do_call = function()->CreateBlock("has_finalizer");
  BasicBlock *done    = function()->CreateBlock("done");

  Value *klass = builder()->CreateValueOfStructEntry(
    receiver,
    in_ByteSize(oopDesc::klass_offset_in_bytes()),
    SharkType::jobject_type(),
    "klass");
  
  Value *klass_part = builder()->CreateAddressOfStructEntry(
    klass,
    in_ByteSize(klassOopDesc::klass_part_offset_in_bytes()),
    SharkType::klass_type(),
    "klass_part");

  Value *access_flags = builder()->CreateValueOfStructEntry(
    klass_part,
    in_ByteSize(Klass::access_flags_offset_in_bytes()),
    SharkType::jint_type(),
    "access_flags");

  builder()->CreateCondBr(
    builder()->CreateICmpNE(
      builder()->CreateAnd(
        access_flags,
        LLVMValue::jint_constant(JVM_ACC_HAS_FINALIZER)),
      LLVMValue::jint_constant(0)),
    do_call, done);

  builder()->SetInsertPoint(do_call);
  call_vm(SharkRuntime::register_finalizer(), receiver);
  BasicBlock *branch_block = builder()->GetInsertBlock();  
  builder()->CreateBr(done);

  builder()->SetInsertPoint(done);
  current_state()->merge(orig_state, orig_block, branch_block);
}

void SharkBlock::handle_return(BasicType type, Value* exception)
{
  assert (exception == NULL || type == T_VOID, "exception OR result, please");

  if (exception)
    builder()->CreateStore(exception, function()->exception_slot());

  release_locked_monitors();
  if (target()->is_synchronized())
    release_method_lock();
  
  if (exception) {
    exception = builder()->CreateLoad(function()->exception_slot());
    builder()->CreateStore(exception, function()->pending_exception_address());
  }

  Value *result_addr = function()->CreatePopFrame(type2size[type]);
  if (type != T_VOID) {
    SharkValue *result = pop();

#ifdef ASSERT
    switch (result->basic_type()) {
    case T_BOOLEAN:
    case T_BYTE:
    case T_CHAR:
    case T_SHORT:
      assert(type == T_INT, "type mismatch");
      break;

    case T_ARRAY:
      assert(type == T_OBJECT, "type mismatch");
      break;

    default:
      assert(result->basic_type() == type, "type mismatch");
    }
#endif // ASSERT

    builder()->CreateStore(
      result->generic_value(),
      builder()->CreateIntToPtr(
        result_addr,
        PointerType::getUnqual(SharkType::to_stackType(type))));
  }

  builder()->CreateRetVoid();
}

void SharkBlock::release_locked_monitors()
{
  int base = target()->is_synchronized();
  for (int i = function()->monitor_count() - 1; i >= base; i--) {
    BasicBlock *locked   = function()->CreateBlock("locked");
    BasicBlock *unlocked = function()->CreateBlock("unlocked");

    Value *object = function()->monitor(i)->object();
    builder()->CreateCondBr(
      builder()->CreateICmpNE(object, LLVMValue::null()),
      locked, unlocked);
    
    builder()->SetInsertPoint(locked);
    builder()->CreateUnimplemented(__FILE__, __LINE__);
    builder()->CreateUnreachable();

    builder()->SetInsertPoint(unlocked);
  }
}

void SharkBlock::do_ldc()
{
  SharkValue *value = SharkValue::from_ciConstant(iter()->get_constant());
  if (value == NULL) {
    SharkConstantPool constants(this);

    BasicBlock *resolved   = function()->CreateBlock("resolved");
    BasicBlock *resolved_class = function()->CreateBlock("resolved_class");
    BasicBlock *unresolved = function()->CreateBlock("unresolved");
    BasicBlock *unknown    = function()->CreateBlock("unknown");
    BasicBlock *done       = function()->CreateBlock("done");

    SwitchInst *switchinst = builder()->CreateSwitch(
      constants.tag_at(iter()->get_constant_index()),
      unknown, 5);

    switchinst->addCase(
      LLVMValue::jbyte_constant(JVM_CONSTANT_String), resolved);
    switchinst->addCase(
      LLVMValue::jbyte_constant(JVM_CONSTANT_Class), resolved_class);
    switchinst->addCase(
      LLVMValue::jbyte_constant(JVM_CONSTANT_UnresolvedString), unresolved);
    switchinst->addCase(
      LLVMValue::jbyte_constant(JVM_CONSTANT_UnresolvedClass), unresolved);
    switchinst->addCase(
      LLVMValue::jbyte_constant(JVM_CONSTANT_UnresolvedClassInError),
      unresolved);

    builder()->SetInsertPoint(resolved);
    Value *resolved_value = constants.object_at(iter()->get_constant_index());
    builder()->CreateBr(done);

    builder()->SetInsertPoint(resolved_class);
    Value *resolved_class_value
      = constants.object_at(iter()->get_constant_index());
    resolved_class_value
      = builder()->CreatePtrToInt(resolved_class_value,
                                  SharkType::intptr_type());
    resolved_class_value
      = (builder()->CreateAdd
         (resolved_class_value,
          (LLVMValue::intptr_constant
           (klassOopDesc::klass_part_offset_in_bytes()
            + Klass::java_mirror_offset_in_bytes()))));
    resolved_class_value
      // FIXME: We need a memory barrier before this load.
      = (builder()->CreateLoad
         (builder()->CreateIntToPtr
          (resolved_class_value,
           PointerType::getUnqual(SharkType::jobject_type()))));
    builder()->CreateBr(done);

    builder()->SetInsertPoint(unresolved);
    builder()->CreateUnimplemented(__FILE__, __LINE__);
    Value *unresolved_value = LLVMValue::null();
    builder()->CreateBr(done);

    builder()->SetInsertPoint(unknown);
    builder()->CreateShouldNotReachHere(__FILE__, __LINE__);
    builder()->CreateUnreachable();

    builder()->SetInsertPoint(done);
    PHINode *phi = builder()->CreatePHI(SharkType::jobject_type(), "constant");
    phi->addIncoming(resolved_value, resolved);
    phi->addIncoming(resolved_class_value, resolved_class);
    phi->addIncoming(unresolved_value, unresolved);
    value = SharkValue::create_jobject(phi);
  }
  push(value);
}

void SharkBlock::do_arraylength()
{
  SharkValue *array = pop();
  check_null(array);
  Value *length = builder()->CreateArrayLength(array->jarray_value());
  push(SharkValue::create_jint(length));
}

void SharkBlock::do_aload(BasicType basic_type)
{
  SharkValue *index = pop();
  SharkValue *array = pop();

  assert(array->type()->is_array_klass(), "should be");
  ciType *element_type = ((ciArrayKlass *) array->type())->element_type();
  assert((element_type->basic_type() == T_BOOLEAN && basic_type == T_BYTE) ||
         (element_type->basic_type() == T_ARRAY && basic_type == T_OBJECT) ||
         (element_type->basic_type() == basic_type), "type mismatch");

  check_null(array);
  check_bounds(array, index);

  Value *value = builder()->CreateLoad(
    builder()->CreateArrayAddress(
      array->jarray_value(), basic_type, index->jint_value()));

  const Type *stack_type = SharkType::to_stackType(basic_type);
  if (value->getType() != stack_type)
    value = builder()->CreateIntCast(value, stack_type, basic_type != T_CHAR);

  switch (basic_type) {
  case T_BYTE:
  case T_CHAR:
  case T_SHORT:
  case T_INT:
    push(SharkValue::create_jint(value));
    break;

  case T_LONG:
    push(SharkValue::create_jlong(value));
    break;

  case T_FLOAT:
    push(SharkValue::create_jfloat(value));
    break;

  case T_DOUBLE:
    push(SharkValue::create_jdouble(value));
    break;
    
  case T_OBJECT:
    push(SharkValue::create_generic(element_type, value));
    break;

  default:
    tty->print_cr("Unhandled type %s", type2name(basic_type));
    ShouldNotReachHere();
  }
}

void SharkBlock::do_astore(BasicType basic_type)
{
  SharkValue *svalue = pop();
  SharkValue *index  = pop();
  SharkValue *array  = pop();

  assert(array->type()->is_array_klass(), "should be");
  ciType *element_type = ((ciArrayKlass *) array->type())->element_type();
  assert((element_type->basic_type() == T_BOOLEAN && basic_type == T_BYTE) ||
         (element_type->basic_type() == T_ARRAY && basic_type == T_OBJECT) ||
         (element_type->basic_type() == basic_type), "type mismatch");

  check_null(array);
  check_bounds(array, index);

  Value *value;
  switch (basic_type) {
  case T_BYTE:
  case T_CHAR:
  case T_SHORT:
  case T_INT:
    value = svalue->jint_value();
    break;

  case T_LONG:
    value = svalue->jlong_value();
    break;

  case T_FLOAT:
    value = svalue->jfloat_value();
    break;

  case T_DOUBLE:
    value = svalue->jdouble_value();
    break;

  case T_OBJECT:
    value = svalue->jobject_value();
    // XXX assignability check
    break;

  default:
    tty->print_cr("Unhandled type %s", type2name(basic_type));
    ShouldNotReachHere();
  }

  const Type *array_type = SharkType::to_arrayType(basic_type);
  if (value->getType() != array_type)
    value = builder()->CreateIntCast(value, array_type, basic_type != T_CHAR);

  Value *addr = builder()->CreateArrayAddress(
    array->jarray_value(), basic_type, index->jint_value(), "addr");

  builder()->CreateStore(value, addr);

  if (!element_type->is_primitive_type())
    builder()->CreateUpdateBarrierSet(oopDesc::bs(), addr);
}

void SharkBlock::do_div_or_rem(bool is_long, bool is_rem)
{
  SharkValue *sb = pop();
  SharkValue *sa = pop();

  check_divide_by_zero(sb);

  Value *a, *b, *p, *q;
  if (is_long) {
    a = sa->jlong_value();
    b = sb->jlong_value();
    p = LLVMValue::jlong_constant(0x8000000000000000LL);
    q = LLVMValue::jlong_constant(-1);
  }
  else {
    a = sa->jint_value();
    b = sb->jint_value();
    p = LLVMValue::jint_constant(0x80000000);
    q = LLVMValue::jint_constant(-1);
  }

  BasicBlock *special_case = function()->CreateBlock("special_case");
  BasicBlock *general_case = function()->CreateBlock("general_case");
  BasicBlock *done         = function()->CreateBlock("done");

  builder()->CreateCondBr(
    builder()->CreateAnd(
      builder()->CreateICmpEQ(a, p),
      builder()->CreateICmpEQ(b, q)),
    special_case, general_case);
  
  builder()->SetInsertPoint(special_case);
  Value *special_result;
  if (is_rem) {
    if (is_long)
      special_result = LLVMValue::jlong_constant(0);
    else
      special_result = LLVMValue::jint_constant(0);
  }
  else {
    special_result = a;
  }
  builder()->CreateBr(done);

  builder()->SetInsertPoint(general_case);
  Value *general_result;
  if (is_rem)
    general_result = builder()->CreateSRem(a, b);
  else
    general_result = builder()->CreateSDiv(a, b);
  builder()->CreateBr(done);

  builder()->SetInsertPoint(done);
  PHINode *result;
  if (is_long)
    result = builder()->CreatePHI(SharkType::jlong_type(), "result");
  else
    result = builder()->CreatePHI(SharkType::jint_type(), "result");
  result->addIncoming(special_result, special_case);
  result->addIncoming(general_result, general_case);

  if (is_long)
    push(SharkValue::create_jlong(result));
  else
    push(SharkValue::create_jint(result));
}

void SharkBlock::do_field_access(bool is_get, bool is_field, ciField* field)
{
  if (field == NULL) {
    bool will_link;
    field = iter()->get_field(will_link);
    assert(will_link, "typeflow responsibility");

    // Check the bytecode matches the field
    if (is_field == field->is_static())
      Unimplemented();
  }

  // Pop the value off the stack where necessary
  SharkValue *value = NULL;
  if (!is_get)
    value = pop();

  // Find the object we're accessing, if necessary
  Value *object = NULL;
  if (is_field) {
    SharkValue *value = pop();
    check_null(value);
    object = value->generic_value();
  }
  if (is_get && field->is_constant()) {
    value = SharkValue::from_ciConstant(field->constant_value());
  }
  if (!is_get || value == NULL) {
    if (!is_field) {
      SharkConstantPool constants(this);
      Value *cache = constants.cache_entry_at(iter()->get_field_index());
  
      object = builder()->CreateValueOfStructEntry(
       cache, ConstantPoolCacheEntry::f1_offset(),
       SharkType::jobject_type(),
       "object");
    }

    BasicType   basic_type = field->type()->basic_type();
    const Type *stack_type = SharkType::to_stackType(basic_type);
    const Type *field_type = SharkType::to_arrayType(basic_type);
    
    Value *addr = builder()->CreateAddressOfStructEntry(
      object, in_ByteSize(field->offset_in_bytes()),
      PointerType::getUnqual(field_type),
      "addr");
  
    // Do the access
    if (is_get) {
      Value *field_value = builder()->CreateLoad(addr);

      if (field_type != stack_type)
      field_value = builder()->CreateIntCast(
        field_value, stack_type, basic_type != T_CHAR);

      value = SharkValue::create_generic(field->type(), field_value);
    }
    else {
      Value *field_value = value->generic_value();

      if (field_type != stack_type)
        field_value = builder()->CreateIntCast(
          field_value, field_type, basic_type != T_CHAR);

      builder()->CreateStore(field_value, addr);
      
      if (!field->type()->is_primitive_type())
        builder()->CreateUpdateBarrierSet(oopDesc::bs(), addr);

      if (field->is_volatile())
        builder()->CreateMemoryBarrier(SharkBuilder::BARRIER_STORELOAD);
    }
  }

  // Push the value onto the stack where necessary
  if (is_get)
    push(value);
}

void SharkBlock::do_lcmp()
{
  Value *b = pop()->jlong_value();
  Value *a = pop()->jlong_value();

  BasicBlock *ne   = function()->CreateBlock("lcmp_ne");
  BasicBlock *lt   = function()->CreateBlock("lcmp_lt");
  BasicBlock *gt   = function()->CreateBlock("lcmp_gt");
  BasicBlock *done = function()->CreateBlock("done");

  BasicBlock *eq = builder()->GetInsertBlock();
  builder()->CreateCondBr(builder()->CreateICmpEQ(a, b), done, ne);

  builder()->SetInsertPoint(ne);
  builder()->CreateCondBr(builder()->CreateICmpSLT(a, b), lt, gt);

  builder()->SetInsertPoint(lt);
  builder()->CreateBr(done);

  builder()->SetInsertPoint(gt);
  builder()->CreateBr(done);

  builder()->SetInsertPoint(done);
  PHINode *result = builder()->CreatePHI(SharkType::jint_type(), "result");
  result->addIncoming(LLVMValue::jint_constant(-1), lt);
  result->addIncoming(LLVMValue::jint_constant(0),  eq);
  result->addIncoming(LLVMValue::jint_constant(1),  gt);

  push(SharkValue::create_jint(result));
}

void SharkBlock::do_fcmp(bool is_double, bool unordered_is_greater)
{
  Value *a, *b;
  if (is_double) {
    b = pop()->jdouble_value();
    a = pop()->jdouble_value();
  }
  else {
    b = pop()->jfloat_value();
    a = pop()->jfloat_value();
  }

  BasicBlock *ordered = function()->CreateBlock("ordered");
  BasicBlock *ge      = function()->CreateBlock("fcmp_ge");
  BasicBlock *lt      = function()->CreateBlock("fcmp_lt");
  BasicBlock *eq      = function()->CreateBlock("fcmp_eq");
  BasicBlock *gt      = function()->CreateBlock("fcmp_gt");
  BasicBlock *done    = function()->CreateBlock("done");

  builder()->CreateCondBr(
    builder()->CreateFCmpUNO(a, b),
    unordered_is_greater ? gt : lt, ordered);

  builder()->SetInsertPoint(ordered);
  builder()->CreateCondBr(builder()->CreateFCmpULT(a, b), lt, ge);

  builder()->SetInsertPoint(ge);
  builder()->CreateCondBr(builder()->CreateFCmpUGT(a, b), gt, eq);

  builder()->SetInsertPoint(lt);
  builder()->CreateBr(done);

  builder()->SetInsertPoint(gt);
  builder()->CreateBr(done);

  builder()->SetInsertPoint(eq);
  builder()->CreateBr(done);

  builder()->SetInsertPoint(done);
  PHINode *result = builder()->CreatePHI(SharkType::jint_type(), "result");
  result->addIncoming(LLVMValue::jint_constant(-1), lt);
  result->addIncoming(LLVMValue::jint_constant(0),  eq);
  result->addIncoming(LLVMValue::jint_constant(1),  gt);

  push(SharkValue::create_jint(result));
}

void SharkBlock::do_if(ICmpInst::Predicate p, SharkValue *b, SharkValue *a)
{
  Value *llvm_a, *llvm_b;
  if (a->is_jobject()) {
    llvm_a = a->intptr_value(builder());
    llvm_b = b->intptr_value(builder());
  }
  else {
    llvm_a = a->jint_value();
    llvm_b = b->jint_value();
  }
                          
  builder()->CreateCondBr(
    builder()->CreateICmp(p, llvm_a, llvm_b),
    successor(ciTypeFlow::IF_TAKEN)->entry_block(),
    successor(ciTypeFlow::IF_NOT_TAKEN)->entry_block());
}

int SharkBlock::switch_default_dest()
{
  return iter()->get_dest_table(0);
}

int SharkBlock::switch_table_length()
{
  switch(bc()) {
  case Bytecodes::_tableswitch:
    return iter()->get_int_table(2) - iter()->get_int_table(1) + 1;

  case Bytecodes::_lookupswitch:
    return iter()->get_int_table(1);

  default:
    ShouldNotReachHere();
  } 
}

int SharkBlock::switch_key(int i)
{
  switch(bc()) {
  case Bytecodes::_tableswitch:
    return iter()->get_int_table(1) + i;

  case Bytecodes::_lookupswitch:
    return iter()->get_int_table(2 + 2 * i);

  default:
    ShouldNotReachHere();
  } 
}

int SharkBlock::switch_dest(int i)
{
  switch(bc()) {
  case Bytecodes::_tableswitch:
    return iter()->get_dest_table(i + 3);

  case Bytecodes::_lookupswitch:
    return iter()->get_dest_table(2 + 2 * i + 1);

  default:
    ShouldNotReachHere();
  } 
}

void SharkBlock::do_switch()
{
  int len = switch_table_length();

  SharkBlock *dest_block = successor(ciTypeFlow::SWITCH_DEFAULT);
  SwitchInst *switchinst = builder()->CreateSwitch(
    pop()->jint_value(), dest_block->entry_block(), len);
  dest_block->add_incoming(current_state());

  for (int i = 0; i < len; i++) {
    int dest_bci = switch_dest(i);
    if (dest_bci != switch_default_dest()) {
      dest_block = bci_successor(dest_bci);
      switchinst->addCase(
        LLVMValue::jint_constant(switch_key(i)),
        dest_block->entry_block());
      dest_block->add_incoming(current_state());      
    }
  }
}

// Figure out what type of call this is.
//  - Direct calls are where the callee is fixed.
//  - Interface and Virtual calls require lookup at runtime.
// NB some invokevirtuals can be resolved to direct calls.
SharkBlock::CallType SharkBlock::get_call_type(ciMethod* method)
{
  if (bc() == Bytecodes::_invokeinterface)
    return CALL_INTERFACE;
  else if (bc() == Bytecodes::_invokevirtual && !method->is_final_method())
    return CALL_VIRTUAL;
  else
    return CALL_DIRECT;
}

Value *SharkBlock::get_callee(CallType    call_type,
                              ciMethod*   method,
                              SharkValue* receiver)
{
  switch (call_type) {
  case CALL_DIRECT:
    return get_direct_callee(method);
  case CALL_VIRTUAL:
    return get_virtual_callee(receiver, method);
  case CALL_INTERFACE:
    return get_interface_callee(receiver);
  default:
    ShouldNotReachHere();
  } 
}

// Direct calls can be made when the callee is fixed.
// invokestatic and invokespecial are always direct;
// invokevirtual is direct in some circumstances.
Value *SharkBlock::get_direct_callee(ciMethod* method)
{
  SharkConstantPool constants(this);
  Value *cache = constants.cache_entry_at(iter()->get_method_index());
  return builder()->CreateValueOfStructEntry(
    cache,
    bc() == Bytecodes::_invokevirtual ?
      ConstantPoolCacheEntry::f2_offset() :
      ConstantPoolCacheEntry::f1_offset(),
    SharkType::methodOop_type(),
    "callee");
}

// Non-direct virtual calls are handled here
Value *SharkBlock::get_virtual_callee(SharkValue *receiver, ciMethod* method)
{
  Value *klass = builder()->CreateValueOfStructEntry(
    receiver->jobject_value(),
    in_ByteSize(oopDesc::klass_offset_in_bytes()),
    SharkType::jobject_type(),
    "klass");

  Value *index;
  if (!method->holder()->is_linked()) {
    // Yuck, we have to do this one slow :(
    // XXX should we trap on this?
    NOT_PRODUCT(warning("unresolved invokevirtual in %s", function()->name()));
    SharkConstantPool constants(this);
    Value *cache = constants.cache_entry_at(iter()->get_method_index());
    index = builder()->CreateValueOfStructEntry(
      cache, ConstantPoolCacheEntry::f2_offset(),
      SharkType::intptr_type(),
      "index");
  }
  else {
    index = LLVMValue::intptr_constant(method->vtable_index());
  }

  return builder()->CreateLoad(
    builder()->CreateArrayAddress(
      klass,
      SharkType::methodOop_type(),
      vtableEntry::size() * wordSize,
      in_ByteSize(instanceKlass::vtable_start_offset() * wordSize),
      index),
    "callee");
}

// Interpreter-style virtual call lookup
Value* SharkBlock::get_virtual_callee(Value *cache, SharkValue *receiver)
{
  BasicBlock *final      = function()->CreateBlock("final");
  BasicBlock *not_final  = function()->CreateBlock("not_final");
  BasicBlock *got_callee = function()->CreateBlock("got_callee");

  Value *flags = builder()->CreateValueOfStructEntry(
    cache, ConstantPoolCacheEntry::flags_offset(),
    SharkType::intptr_type(),
    "flags");

  const int mask = 1 << ConstantPoolCacheEntry::vfinalMethod;
  builder()->CreateCondBr(
    builder()->CreateICmpNE(
      builder()->CreateAnd(flags, LLVMValue::intptr_constant(mask)),
      LLVMValue::intptr_constant(0)),
    final, not_final);

  // For final methods f2 is the actual address of the method
  builder()->SetInsertPoint(final);
  Value *final_callee = builder()->CreateValueOfStructEntry(
    cache, ConstantPoolCacheEntry::f2_offset(),
    SharkType::methodOop_type(),
    "final_callee");
  builder()->CreateBr(got_callee);

  // For non-final methods f2 is the index into the vtable
  builder()->SetInsertPoint(not_final);
  Value *klass = builder()->CreateValueOfStructEntry(
    receiver->jobject_value(),
    in_ByteSize(oopDesc::klass_offset_in_bytes()),
    SharkType::jobject_type(),
    "klass");

  Value *index = builder()->CreateValueOfStructEntry(
    cache, ConstantPoolCacheEntry::f2_offset(),
    SharkType::intptr_type(),
    "index");

  Value *nonfinal_callee = builder()->CreateLoad(
    builder()->CreateArrayAddress(
      klass,
      SharkType::methodOop_type(),
      vtableEntry::size() * wordSize,
      in_ByteSize(instanceKlass::vtable_start_offset() * wordSize),
      index),
    "nonfinal_callee");
  builder()->CreateBr(got_callee);

  builder()->SetInsertPoint(got_callee);
  PHINode *callee = builder()->CreatePHI(
    SharkType::methodOop_type(), "callee");
  callee->addIncoming(final_callee, final);
  callee->addIncoming(nonfinal_callee, not_final);

  return callee;
}

// Interpreter-style interface call lookup
Value* SharkBlock::get_interface_callee(SharkValue *receiver)
{
  SharkConstantPool constants(this);
  Value *cache = constants.cache_entry_at(iter()->get_method_index());

  BasicBlock *hacky      = function()->CreateBlock("hacky");
  BasicBlock *normal     = function()->CreateBlock("normal");
  BasicBlock *loop       = function()->CreateBlock("loop");
  BasicBlock *got_null   = function()->CreateBlock("got_null");
  BasicBlock *not_null   = function()->CreateBlock("not_null");
  BasicBlock *next       = function()->CreateBlock("next");
  BasicBlock *got_entry  = function()->CreateBlock("got_entry");
  BasicBlock *got_callee = function()->CreateBlock("got_callee");

  Value *flags = builder()->CreateValueOfStructEntry(
    cache, ConstantPoolCacheEntry::flags_offset(),
    SharkType::intptr_type(),
    "flags");

  const int mask = 1 << ConstantPoolCacheEntry::methodInterface;
  builder()->CreateCondBr(
    builder()->CreateICmpNE(
      builder()->CreateAnd(flags, LLVMValue::intptr_constant(mask)),
      LLVMValue::intptr_constant(0)),
    hacky, normal);

  // Workaround for the case where we encounter an invokeinterface,
  // but should really have an invokevirtual since the resolved
  // method is a virtual method in java.lang.Object. This is a
  // corner case in the spec but is presumably legal, and while
  // javac does not generate this code there's no reason it could
  // not be produced by a compliant java compiler.  See
  // cpCacheOop.cpp for more details.
  builder()->SetInsertPoint(hacky);
  Value *hacky_callee = get_virtual_callee(cache, receiver);
  BasicBlock *got_hacky = builder()->GetInsertBlock();
  builder()->CreateBr(got_callee);

  // Locate the receiver's itable
  builder()->SetInsertPoint(normal);
  Value *object_klass = builder()->CreateValueOfStructEntry(
    receiver->jobject_value(), in_ByteSize(oopDesc::klass_offset_in_bytes()),
    SharkType::jobject_type(),
    "object_klass");

  Value *vtable_start = builder()->CreateAdd(
    builder()->CreatePtrToInt(object_klass, SharkType::intptr_type()),
    LLVMValue::intptr_constant(
      instanceKlass::vtable_start_offset() * HeapWordSize),
    "vtable_start");

  Value *vtable_length = builder()->CreateValueOfStructEntry(
    object_klass,
    in_ByteSize(instanceKlass::vtable_length_offset() * HeapWordSize),
    SharkType::jint_type(),
    "vtable_length");
  vtable_length =
    builder()->CreateIntCast(vtable_length, SharkType::intptr_type(), false);

  bool needs_aligning = HeapWordsPerLong > 1;
  const char *itable_start_name = "itable_start";
  Value *itable_start = builder()->CreateAdd(
    vtable_start,
    builder()->CreateShl(
      vtable_length,
      LLVMValue::intptr_constant(exact_log2(vtableEntry::size() * wordSize))),
    needs_aligning ? "" : itable_start_name);
  if (needs_aligning)
    itable_start = builder()->CreateAlign(
      itable_start, BytesPerLong, itable_start_name);

  // Locate this interface's entry in the table
  Value *iklass = builder()->CreateValueOfStructEntry(
    cache, ConstantPoolCacheEntry::f1_offset(),
    SharkType::jobject_type(),
    "iklass");

  builder()->CreateBr(loop);
  builder()->SetInsertPoint(loop);
  PHINode *itable_entry_addr = builder()->CreatePHI(
    SharkType::intptr_type(), "itable_entry_addr");
  itable_entry_addr->addIncoming(itable_start, normal);

  Value *itable_entry = builder()->CreateIntToPtr(
    itable_entry_addr, SharkType::itableOffsetEntry_type(), "itable_entry");

  Value *itable_iklass = builder()->CreateValueOfStructEntry(
    itable_entry,
    in_ByteSize(itableOffsetEntry::interface_offset_in_bytes()),
    SharkType::jobject_type(),
    "itable_iklass");

  builder()->CreateCondBr(
    builder()->CreateICmpEQ(itable_iklass, LLVMValue::null()),
    got_null, not_null);

  // A null entry means that the class doesn't implement the
  // interface, and wasn't the same as the class checked when
  // the interface was resolved.
  builder()->SetInsertPoint(got_null);
  builder()->CreateUnimplemented(__FILE__, __LINE__);
  builder()->CreateUnreachable();
                          
  builder()->SetInsertPoint(not_null);
  builder()->CreateCondBr(
    builder()->CreateICmpEQ(itable_iklass, iklass),
    got_entry, next);

  builder()->SetInsertPoint(next);
  Value *next_entry = builder()->CreateAdd(
    itable_entry_addr,
    LLVMValue::intptr_constant(itableOffsetEntry::size() * wordSize));
  builder()->CreateBr(loop);
  itable_entry_addr->addIncoming(next_entry, next);

  // Locate the method pointer
  builder()->SetInsertPoint(got_entry);
  Value *offset = builder()->CreateValueOfStructEntry(
    itable_entry,
    in_ByteSize(itableOffsetEntry::offset_offset_in_bytes()),
    SharkType::jint_type(),
    "offset");
  offset =
    builder()->CreateIntCast(offset, SharkType::intptr_type(), false);

  Value *index = builder()->CreateValueOfStructEntry(
    cache, ConstantPoolCacheEntry::f2_offset(),
    SharkType::intptr_type(),
    "index");

  Value *normal_callee = builder()->CreateLoad(
    builder()->CreateIntToPtr(
      builder()->CreateAdd(
        builder()->CreateAdd(
          builder()->CreateAdd(
            builder()->CreatePtrToInt(
              object_klass, SharkType::intptr_type()),
            offset),
          builder()->CreateShl(
            index,
            LLVMValue::intptr_constant(
              exact_log2(itableMethodEntry::size() * wordSize)))),
        LLVMValue::intptr_constant(
          itableMethodEntry::method_offset_in_bytes())),
      PointerType::getUnqual(SharkType::methodOop_type())),
    "normal_callee");
  BasicBlock *got_normal = builder()->GetInsertBlock();
  builder()->CreateBr(got_callee);

  builder()->SetInsertPoint(got_callee);
  PHINode *callee = builder()->CreatePHI(
    SharkType::methodOop_type(), "callee");
  callee->addIncoming(hacky_callee, got_hacky);
  callee->addIncoming(normal_callee, got_normal);

  return callee;
} 

bool SharkBlock::maybe_inline_call(ciMethod *method)
{
  // Quick checks to allow us to bail out fast.  We can't inline
  // native methods, there's no point inlining abstract ones, and
  // monitors aren't allowed because the inlined section has no
  // frame to put them in.
  if (method->is_native() ||
      method->is_abstract() ||
      method->is_synchronized() ||
      method->has_monitor_bytecodes())
    return false;

  // Inlining empty methods is trivial
  if (method->is_empty_method()) {
    int desired_stack_depth = xstack_depth() - method->arg_size();
    while (xstack_depth() > desired_stack_depth)
      pop();

    return true;
  } 
  
  // We need to scan the bytecode to do any more, so we bail out
  // now if the method is too big
  if (method->code_size() > 5)
    return false;

  // If the holder isn't linked then there isn't a lot we can do
  if (!method->holder()->is_linked())
    return false;
  
  // Inspect the method's code to see if we can inline it.  We
  // don't use method->is_accessor() because that only spots
  // some getfields, whereas we can inline *all* getfields, all
  // putfields, and some getstatics too.
  address code = method->code();
  switch (method->code_size()) {
  case 4:
    // getstatic and putstatic will try to look up the receiver
    // from the holder's constant pool, which we can't do.  Some
    // getstatics, however, resolve to constants, and those we
    // can do.  So we try...
    if (code[0] == Bytecodes::_getstatic)
      return maybe_inline_accessor(method, false);
    break;

  case 5:
    if (code[0] == Bytecodes::_aload_0 &&
        (code[1] == Bytecodes::_getfield ||
         code[1] == Bytecodes::_putfield))
      return maybe_inline_accessor(method, true);
    break;
  }
  return false;
}

bool SharkBlock::maybe_inline_accessor(ciMethod *method, bool is_field)
{
  if (method->arg_size() != (is_field ? 1 : 0)) {
    NOT_PRODUCT(warning("wacky accessor in %s", function()->name()));
    return false;
  }

  ciBytecodeStream iter(method);
  Bytecodes::Code bc;

  if (is_field) {
    bc = iter.next();
    assert(bc == Bytecodes::_aload_0, "eh?");
  }

  bool is_get;
  bc = iter.next();
  if (is_field) {
    assert(bc == Bytecodes::_getfield || bc == Bytecodes::_putfield, "eh?");
    is_get = bc == Bytecodes::_getfield;
  }
  else {
    assert(bc == Bytecodes::_getstatic, "eh?");
    is_get = true;
  }

  bool will_link;
  ciField *field = iter.get_field(will_link);
  if (!will_link)
    return false;

  // We can only inline getstatic if the field resolves to a
  // non-oop constant.
  if (!is_field) {
    if (!field->is_constant())
      return false;

    if (SharkValue::from_ciConstant(field->constant_value()) == NULL)
      return false;
  }

  // We mustn't inline if the resolved field is the wrong type,
  // because the thrown exception would appear to come from the
  // wrong method.
  if (is_field == field->is_static())
    return false;
  
  bc = iter.next();
  if (is_get) {
    switch (bc) {
    case Bytecodes::_ireturn:
    case Bytecodes::_lreturn:
    case Bytecodes::_freturn:
    case Bytecodes::_dreturn:
    case Bytecodes::_areturn:
      break;

    default:
      NOT_PRODUCT(warning("wacky accessor in %s", function()->name()));
      return false;
    }
  }
  else {
    if (bc != Bytecodes::_return) {
      NOT_PRODUCT(warning("wacky accessor in %s", function()->name()));
      return false;
    }
  }

  bc = iter.next();
  assert(bc == ciBytecodeStream::EOBC(), "eh?");

  // For field accesses we need to null check the receiver before
  // entering the inlined section.  For the majority of accessors
  // this has already been done (in do_call) but if the accessor
  // was invoked by invokestatic (eg synthetic accessors) then it
  // may not have been checked and do_field_access will try to do
  // it and fail.
  if (is_field) {
    if (!xstack(0)->zero_checked())
      return false;
  }
  
  current_state()->enter_inlined_section();
  do_field_access(is_get, is_field, field);
  current_state()->leave_inlined_section();
  return true;
}

void SharkBlock::do_call()
{
  bool will_link;
  ciMethod *method = iter()->get_method(will_link);
  assert(will_link, "typeflow responsibility");

  // Figure out what type of call this is
  CallType call_type = get_call_type(method);

  // Find the receiver in the stack.  This has to happen
  // before we try to inline, because nothing in the inlined
  // code can decache (which check_null needs to do for the
  // VM call to throw the NullPointerException).  Once we've
  // checked, the repeated null check elimination stuff does
  // the work for us.
  SharkValue *receiver = NULL;
  if (bc() != Bytecodes::_invokestatic) {
    receiver = xstack(method->arg_size() - 1);
    check_null(receiver);
  }

  // Try to inline the call
  if (call_type == CALL_DIRECT) {
    if (maybe_inline_call(method))
      return;
  }

  // Find the method we are calling
  Value *callee = get_callee(call_type, method, receiver);

  // Load the SharkEntry from the callee
  Value *base_pc = builder()->CreateValueOfStructEntry(
    callee, methodOopDesc::from_interpreted_offset(),
    SharkType::intptr_type(),
    "base_pc");

  // Load the entry point from the SharkEntry
  Value *entry_point = builder()->CreateLoad(
    builder()->CreateIntToPtr(
      builder()->CreateAdd(
        base_pc,
        LLVMValue::intptr_constant(in_bytes(ZeroEntry::entry_point_offset()))),
      PointerType::getUnqual(
        PointerType::getUnqual(SharkType::entry_point_type()))),
    "entry_point");

  // Make the call
  current_state()->decache_for_Java_call(method);
  builder()->CreateCall3(entry_point, callee, base_pc, thread());
  current_state()->cache_after_Java_call(method);

  // Check for pending exceptions
  check_pending_exception();
}

void SharkBlock::do_instance_check()
{
  // Leave the object on the stack until after all the VM calls
  assert(xstack(0)->is_jobject(), "should be");
  
  ciKlass *klass = NULL;
  if (bc() == Bytecodes::_checkcast) {
    bool will_link;
    klass = iter()->get_klass(will_link);
    if (!will_link) {
      // XXX why is this not typeflow's responsibility?
      NOT_PRODUCT(warning("unresolved checkcast in %s", function()->name()));
      klass = (ciKlass *) xstack(0)->type();
    }
  }

  BasicBlock *not_null      = function()->CreateBlock("not_null");
  BasicBlock *fast_path     = function()->CreateBlock("fast_path");
  BasicBlock *slow_path     = function()->CreateBlock("slow_path");
  BasicBlock *got_klass     = function()->CreateBlock("got_klass");
  BasicBlock *subtype_check = function()->CreateBlock("subtype_check");
  BasicBlock *is_instance   = function()->CreateBlock("is_instance");
  BasicBlock *not_instance  = function()->CreateBlock("not_instance");
  BasicBlock *merge1        = function()->CreateBlock("merge1");
  BasicBlock *merge2        = function()->CreateBlock("merge2");

  enum InstanceCheckStates {
    IC_IS_NULL,
    IC_IS_INSTANCE,
    IC_NOT_INSTANCE,
  };

  // Null objects aren't instances of anything
  builder()->CreateCondBr(
    builder()->CreateICmpEQ(xstack(0)->jobject_value(), LLVMValue::null()),
    merge2, not_null);
  BasicBlock *null_block = builder()->GetInsertBlock();
  SharkState *null_state = current_state()->copy();

  // Get the class we're checking against
  builder()->SetInsertPoint(not_null);
  SharkConstantPool constants(this);
  Value *tag = constants.tag_at(iter()->get_klass_index());
  builder()->CreateCondBr(
    builder()->CreateOr(
      builder()->CreateICmpEQ(
        tag, LLVMValue::jbyte_constant(JVM_CONSTANT_UnresolvedClass)),
      builder()->CreateICmpEQ(
        tag, LLVMValue::jbyte_constant(JVM_CONSTANT_UnresolvedClassInError))),
    slow_path, fast_path);

  // The fast path
  builder()->SetInsertPoint(fast_path);
  BasicBlock *fast_block = builder()->GetInsertBlock();
  SharkState *fast_state = current_state()->copy();
  Value *fast_klass = constants.object_at(iter()->get_klass_index());
  builder()->CreateBr(got_klass);

  // The slow path
  builder()->SetInsertPoint(slow_path);
  call_vm(
    SharkRuntime::resolve_klass(),
    LLVMValue::jint_constant(iter()->get_klass_index()));
  Value *slow_klass = function()->CreateGetVMResult();
  BasicBlock *slow_block = builder()->GetInsertBlock();  
  builder()->CreateBr(got_klass);

  // We have the class to test against
  builder()->SetInsertPoint(got_klass);
  current_state()->merge(fast_state, fast_block, slow_block);
  PHINode *check_klass = builder()->CreatePHI(
    SharkType::jobject_type(), "check_klass");
  check_klass->addIncoming(fast_klass, fast_block);
  check_klass->addIncoming(slow_klass, slow_block);

  // Get the class of the object being tested
  Value *object_klass = builder()->CreateValueOfStructEntry(
    xstack(0)->jobject_value(), in_ByteSize(oopDesc::klass_offset_in_bytes()),
    SharkType::jobject_type(),
    "object_klass");

  // Perform the check
  builder()->CreateCondBr(
    builder()->CreateICmpEQ(check_klass, object_klass),
    is_instance, subtype_check);

  builder()->SetInsertPoint(subtype_check);
  builder()->CreateCondBr(
    builder()->CreateICmpNE(
      builder()->CreateCall2(
        SharkRuntime::is_subtype_of(), check_klass, object_klass),
      LLVMValue::jbyte_constant(0)),
    is_instance, not_instance);

  builder()->SetInsertPoint(is_instance);
  builder()->CreateBr(merge1);

  builder()->SetInsertPoint(not_instance);
  builder()->CreateBr(merge1);

  // First merge
  builder()->SetInsertPoint(merge1);
  PHINode *nonnull_result = builder()->CreatePHI(
    SharkType::jint_type(), "nonnull_result");
  nonnull_result->addIncoming(
    LLVMValue::jint_constant(IC_IS_INSTANCE), is_instance);
  nonnull_result->addIncoming(
    LLVMValue::jint_constant(IC_NOT_INSTANCE), not_instance);
  BasicBlock *nonnull_block = builder()->GetInsertBlock();
  builder()->CreateBr(merge2);
  
  // Second merge
  builder()->SetInsertPoint(merge2);
  current_state()->merge(null_state, null_block, nonnull_block);
  PHINode *result = builder()->CreatePHI(
    SharkType::jint_type(), "result");
  result->addIncoming(LLVMValue::jint_constant(IC_IS_NULL), null_block);
  result->addIncoming(nonnull_result, nonnull_block);

  // We can finally pop the object!
  Value *object = pop()->jobject_value();

  // Handle the result
  if (bc() == Bytecodes::_checkcast) {
    BasicBlock *failure = function()->CreateBlock("failure");
    BasicBlock *success = function()->CreateBlock("success");

    builder()->CreateCondBr(
      builder()->CreateICmpNE(
        result, LLVMValue::jint_constant(IC_NOT_INSTANCE)),
      success, failure);

    builder()->SetInsertPoint(failure);
    builder()->CreateUnimplemented(__FILE__, __LINE__);
    builder()->CreateUnreachable();

    builder()->SetInsertPoint(success);
    push(SharkValue::create_generic(klass, object));
  }
  else {
    push(
      SharkValue::create_jint(
        builder()->CreateIntCast(
          builder()->CreateICmpEQ(
            result, LLVMValue::jint_constant(IC_IS_INSTANCE)),
          SharkType::jint_type(), false)));
  }
}

void SharkBlock::do_new()
{
  bool will_link;
  ciInstanceKlass* klass = iter()->get_klass(will_link)->as_instance_klass();
  assert(will_link, "typeflow responsibility");

  BasicBlock *tlab_alloc          = NULL;
  BasicBlock *got_tlab            = NULL;
  BasicBlock *heap_alloc          = NULL;
  BasicBlock *retry               = NULL;
  BasicBlock *got_heap            = NULL;
  BasicBlock *initialize          = NULL;
  BasicBlock *got_fast            = NULL;
  BasicBlock *slow_alloc_and_init = NULL;
  BasicBlock *got_slow            = NULL;
  BasicBlock *push_object         = NULL;

  SharkState *fast_state = NULL;
  
  Value *tlab_object = NULL;
  Value *heap_object = NULL;
  Value *fast_object = NULL;
  Value *slow_object = NULL;
  Value *object      = NULL;

  SharkConstantPool constants(this);

  // The fast path
  if (!Klass::layout_helper_needs_slow_path(klass->layout_helper())) {
    if (UseTLAB) {
      tlab_alloc        = function()->CreateBlock("tlab_alloc");
      got_tlab          = function()->CreateBlock("got_tlab");
    }
    heap_alloc          = function()->CreateBlock("heap_alloc");
    retry               = function()->CreateBlock("retry");
    got_heap            = function()->CreateBlock("got_heap");
    initialize          = function()->CreateBlock("initialize");
    slow_alloc_and_init = function()->CreateBlock("slow_alloc_and_init");
    push_object         = function()->CreateBlock("push_object");

    builder()->CreateCondBr(
      builder()->CreateICmpEQ(
        constants.tag_at(iter()->get_klass_index()),
        LLVMValue::jbyte_constant(JVM_CONSTANT_Class)),
      UseTLAB ? tlab_alloc : heap_alloc, slow_alloc_and_init);
    
    size_t size_in_bytes = klass->size_helper() << LogHeapWordSize;

    // Thread local allocation
    if (UseTLAB) {
      builder()->SetInsertPoint(tlab_alloc);

      Value *top_addr = builder()->CreateAddressOfStructEntry(
        thread(), Thread::tlab_top_offset(),
        PointerType::getUnqual(SharkType::intptr_type()),
        "top_addr");

      Value *end = builder()->CreateValueOfStructEntry(
        thread(), Thread::tlab_end_offset(),
        SharkType::intptr_type(),
        "end");

      Value *old_top = builder()->CreateLoad(top_addr, "old_top");
      Value *new_top = builder()->CreateAdd(
        old_top, LLVMValue::intptr_constant(size_in_bytes));

      builder()->CreateCondBr(
        builder()->CreateICmpULE(new_top, end),
        got_tlab, heap_alloc);

      builder()->SetInsertPoint(got_tlab);
      tlab_object = builder()->CreateIntToPtr(
        old_top, SharkType::jobject_type(), "tlab_object");

      builder()->CreateStore(new_top, top_addr);
      builder()->CreateBr(initialize);
    }

    // Heap allocation
    builder()->SetInsertPoint(heap_alloc);

    Value *top_addr = builder()->CreateIntToPtr(
	builder()->pointer_constant(Universe::heap()->top_addr()),
      PointerType::getUnqual(SharkType::intptr_type()),
      "top_addr");

    Value *end = builder()->CreateLoad(
      builder()->CreateIntToPtr(
        builder()->pointer_constant(Universe::heap()->end_addr()),
        PointerType::getUnqual(SharkType::intptr_type())),
      "end");

    builder()->CreateBr(retry);
    builder()->SetInsertPoint(retry);

    Value *old_top = builder()->CreateLoad(top_addr, "top");
    Value *new_top = builder()->CreateAdd(
      old_top, LLVMValue::intptr_constant(size_in_bytes));

    builder()->CreateCondBr(
      builder()->CreateICmpULE(new_top, end),
      got_heap, slow_alloc_and_init);

    builder()->SetInsertPoint(got_heap);
    heap_object = builder()->CreateIntToPtr(
      old_top, SharkType::jobject_type(), "heap_object");

    Value *check = builder()->CreateCmpxchgPtr(new_top, top_addr, old_top);
    builder()->CreateCondBr(
      builder()->CreateICmpEQ(old_top, check),
      initialize, retry);

    // Initialize the object
    builder()->SetInsertPoint(initialize);
    if (tlab_object) {
      PHINode *phi = builder()->CreatePHI(
        SharkType::jobject_type(), "fast_object");
      phi->addIncoming(tlab_object, got_tlab);
      phi->addIncoming(heap_object, got_heap);
      fast_object = phi;
    }
    else {
      fast_object = heap_object;
    }

    builder()->CreateMemset(
      builder()->CreateBitCast(
        fast_object, PointerType::getUnqual(SharkType::jbyte_type())),
      LLVMValue::jbyte_constant(0),
      LLVMValue::jint_constant(size_in_bytes),
      LLVMValue::jint_constant(HeapWordSize));

    Value *mark_addr = builder()->CreateAddressOfStructEntry(
      fast_object, in_ByteSize(oopDesc::mark_offset_in_bytes()),
      PointerType::getUnqual(SharkType::intptr_type()),
      "mark_addr");

    Value *klass_addr = builder()->CreateAddressOfStructEntry(
      fast_object, in_ByteSize(oopDesc::klass_offset_in_bytes()),
      PointerType::getUnqual(SharkType::jobject_type()),
      "klass_addr");

    // Set the mark
    intptr_t mark;
    if (UseBiasedLocking) {
      Unimplemented();
    }
    else {
      mark = (intptr_t) markOopDesc::prototype();
    }
    builder()->CreateStore(LLVMValue::intptr_constant(mark), mark_addr);

    // Set the class
    Value *rtklass = constants.object_at(iter()->get_klass_index());
    builder()->CreateStore(rtklass, klass_addr);
    got_fast = builder()->GetInsertBlock();

    builder()->CreateBr(push_object);
    builder()->SetInsertPoint(slow_alloc_and_init);
    fast_state = current_state()->copy();
  }

  // The slow path
  call_vm(
    SharkRuntime::new_instance(),
    LLVMValue::jint_constant(iter()->get_klass_index()));
  slow_object = function()->CreateGetVMResult();
  got_slow = builder()->GetInsertBlock();

  // Push the object
  if (push_object) {
    builder()->CreateBr(push_object);
    builder()->SetInsertPoint(push_object);
  }
  if (fast_object) {
    PHINode *phi = builder()->CreatePHI(SharkType::jobject_type(), "object");
    phi->addIncoming(fast_object, got_fast);
    phi->addIncoming(slow_object, got_slow);
    object = phi;
    current_state()->merge(fast_state, got_fast, got_slow);
  }
  else {
    object = slow_object;
  }

  SharkValue *result = SharkValue::create_jobject(object);
  result->set_zero_checked(true);
  push(result);
}

void SharkBlock::do_newarray()
{
  BasicType type = (BasicType) iter()->get_index();

  call_vm(
    SharkRuntime::newarray(),
    LLVMValue::jint_constant(type),
    pop()->jint_value());

  SharkValue *result = SharkValue::create_generic(
    ciArrayKlass::make(ciType::make(type)),
    function()->CreateGetVMResult());
  result->set_zero_checked(true);
  push(result);
}

void SharkBlock::do_anewarray()
{
  bool will_link;
  ciKlass *klass = iter()->get_klass(will_link);
  assert(will_link, "typeflow responsibility");

  ciObjArrayKlass *array_klass = ciObjArrayKlass::make(klass);
  if (!array_klass->is_loaded()) {
    Unimplemented();
  }

  call_vm(
    SharkRuntime::anewarray(),
    LLVMValue::jint_constant(iter()->get_klass_index()),
    pop()->jint_value());

  SharkValue *result = SharkValue::create_generic(
    array_klass, function()->CreateGetVMResult());
  result->set_zero_checked(true);
  push(result);
}

void SharkBlock::do_multianewarray()
{
  bool will_link;
  ciArrayKlass *array_klass = iter()->get_klass(will_link)->as_array_klass();
  assert(will_link, "typeflow responsibility");

  // The dimensions are stack values, so we use their slots for the
  // dimensions array.  Note that we are storing them in the reverse
  // of normal stack order.
  int ndims = iter()->get_dimensions();

  Value *dimensions = function()->CreateAddressOfFrameEntry(
    function()->stack_slots_offset() + max_stack() - xstack_depth(),
    ArrayType::get(SharkType::jint_type(), ndims),
    "dimensions");

  for (int i = 0; i < ndims; i++) {
    builder()->CreateStore(
      xstack(ndims - 1 - i)->jint_value(),
      builder()->CreateStructGEP(dimensions, i));
  }

  call_vm(
    SharkRuntime::multianewarray(),
    LLVMValue::jint_constant(iter()->get_klass_index()),
    LLVMValue::jint_constant(ndims),
    builder()->CreateStructGEP(dimensions, 0));

  // Now we can pop the dimensions off the stack
  for (int i = 0; i < ndims; i++)
    pop();

  SharkValue *result = SharkValue::create_generic(
    array_klass, function()->CreateGetVMResult());
  result->set_zero_checked(true);
  push(result);
}

void SharkBlock::do_monitorenter()
{
  SharkValue *lockee = pop();
  check_null(lockee);
  Value *object = lockee->jobject_value();

  // Find a free monitor, or one already allocated for this object
  BasicBlock *loop_top    = function()->CreateBlock("loop_top");
  BasicBlock *loop_iter   = function()->CreateBlock("loop_iter");
  BasicBlock *loop_check  = function()->CreateBlock("loop_check");
  BasicBlock *no_monitor  = function()->CreateBlock("no_monitor");
  BasicBlock *got_monitor = function()->CreateBlock("got_monitor");

  BasicBlock *entry_block = builder()->GetInsertBlock();
  builder()->CreateBr(loop_check);

  builder()->SetInsertPoint(loop_check);
  PHINode *index = builder()->CreatePHI(SharkType::jint_type(), "index");
  index->addIncoming(
    LLVMValue::jint_constant(function()->monitor_count() - 1), entry_block);
  builder()->CreateCondBr(
    builder()->CreateICmpUGE(index, LLVMValue::jint_constant(0)),
    loop_top, no_monitor);

  builder()->SetInsertPoint(loop_top);
  SharkMonitor* monitor = function()->monitor(index);
  Value *smo = monitor->object();
  builder()->CreateCondBr(
    builder()->CreateOr(
      builder()->CreateICmpEQ(smo, LLVMValue::null()),
      builder()->CreateICmpEQ(smo, object)),
    got_monitor, loop_iter);

  builder()->SetInsertPoint(loop_iter);
  index->addIncoming(
    builder()->CreateSub(index, LLVMValue::jint_constant(1)), loop_iter);
  builder()->CreateBr(loop_check);

  builder()->SetInsertPoint(no_monitor);
  builder()->CreateShouldNotReachHere(__FILE__, __LINE__);
  builder()->CreateUnreachable();

  // Acquire the lock
  builder()->SetInsertPoint(got_monitor);
  monitor->acquire(this, object);
  check_pending_exception();
}

void SharkBlock::do_monitorexit()
{
  SharkValue *lockee = pop();
  // The monitorexit can't throw an NPE because the verifier checks
  // that the monitor operations are block structured before we
  // compile.
  // check_null(lockee);
  Value *object = lockee->jobject_value();

  // Find the monitor associated with this object
  BasicBlock *loop_top    = function()->CreateBlock("loop_top");
  BasicBlock *loop_iter   = function()->CreateBlock("loop_iter");
  BasicBlock *loop_check  = function()->CreateBlock("loop_check");
  BasicBlock *no_monitor  = function()->CreateBlock("no_monitor");
  BasicBlock *got_monitor = function()->CreateBlock("got_monitor");

  BasicBlock *entry_block = builder()->GetInsertBlock();
  builder()->CreateBr(loop_check);

  builder()->SetInsertPoint(loop_check);
  PHINode *index = builder()->CreatePHI(SharkType::jint_type(), "index");
  index->addIncoming(
    LLVMValue::jint_constant(function()->monitor_count() - 1), entry_block);
  builder()->CreateCondBr(
    builder()->CreateICmpUGE(index, LLVMValue::jint_constant(0)),
    loop_top, no_monitor);

  builder()->SetInsertPoint(loop_top);
  SharkMonitor* monitor = function()->monitor(index);
  Value *smo = monitor->object();
  builder()->CreateCondBr(
    builder()->CreateICmpEQ(smo, object),
    got_monitor, loop_iter);

  builder()->SetInsertPoint(loop_iter);
  index->addIncoming(
    builder()->CreateSub(index, LLVMValue::jint_constant(1)), loop_iter);
  builder()->CreateBr(loop_check);

  builder()->SetInsertPoint(no_monitor);
  builder()->CreateShouldNotReachHere(__FILE__, __LINE__);
  builder()->CreateUnreachable();

  // Release the lock
  builder()->SetInsertPoint(got_monitor);
  monitor->release(this);
  // The monitorexit can't throw an NPE because the verifier checks
  // that the monitor operations are block structured before we
  // compile.
  // check_pending_exception();
}
