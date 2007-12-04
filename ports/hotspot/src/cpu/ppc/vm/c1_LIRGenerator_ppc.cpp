/*
 * Copyright 2005-2006 Sun Microsystems, Inc.  All Rights Reserved.
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

#include "incls/_precompiled.incl"
#include "incls/_c1_LIRGenerator_ppc.cpp.incl"

#ifdef ASSERT
#define __ gen()->lir(__FILE__, __LINE__)->
#else
#define __ gen()->lir()->
#endif

void LIRItem::load_byte_item()
{
  // byte loads use same registers as other loads
  load_item();
}

void LIRItem::load_nonconstant()
{
  LIR_Opr r = value()->operand();
  if (r->is_constant()) {
    // XXX sparc has some extra stuff here
    _result = r;
  } else {
    load_item();
  }
}

//--------------------------------------------------------------
//               LIRGenerator
//--------------------------------------------------------------

LIR_Opr LIRGenerator::exceptionOopOpr()
{
  Unimplemented();
}

LIR_Opr LIRGenerator::exceptionPcOpr()
{
  Unimplemented();
}

LIR_Opr LIRGenerator::syncTempOpr()
{
  Unimplemented();
}

LIR_Opr LIRGenerator::getThreadTemp()
{
  Unimplemented();
}

LIR_Opr LIRGenerator::result_register_for(ValueType* type, bool callee)
{
  LIR_Opr opr;

  switch (type->tag()) {
  case intTag:
    opr = FrameMap::gpr_opr[3];
    break;

  case objectTag:
    opr = FrameMap::gpr_oop_opr[3];
    break;

  case longTag:
    Unimplemented();
    break;

  case floatTag:
    Unimplemented();
    break;

  case doubleTag:
    Unimplemented();
    break;
    
  default:
    ShouldNotReachHere();
  }

  assert(opr->type_field() == as_OprType(as_BasicType(type)), "type mismatch");
  return opr;
}

LIR_Opr LIRGenerator::rlock_byte(BasicType type)
{
  Unimplemented();
}


//--------- loading items into registers --------------------------------

bool LIRGenerator::can_store_as_constant(Value v, BasicType type) const
{
  return false;
}

bool LIRGenerator::can_inline_as_constant(Value v) const
{
  if (v->type()->as_IntConstant() != NULL) {
    return Assembler::is_simm16(v->type()->as_IntConstant()->value());
  }
  return false;
}

bool LIRGenerator::can_inline_as_constant(LIR_Const* c) const
{
  if (c->type() == T_INT) {
    return Assembler::is_simm16(c->as_jint());
  }
  return false;
}

LIR_Opr LIRGenerator::safepoint_poll_register()
{
  Unimplemented();
}

LIR_Address* LIRGenerator::generate_address(LIR_Opr base, LIR_Opr index,
                                            int shift, int disp,
                                            BasicType type) {
  assert(base->is_register(), "must be");

  // accumulate fixed displacements
  if (index->is_constant()) {
    disp += index->as_constant_ptr()->as_jint() << shift;
    index = LIR_OprFact::illegalOpr;
  }

  if (index->is_register()) {
    // apply the shift and accumulate the displacement
    if (shift > 0) {
      LIR_Opr tmp = new_register(T_INT);
      __ shift_left(index, shift, tmp);
      index = tmp;
    }
    if (disp != 0) {
      LIR_Opr tmp = new_register(T_INT);
      if (Assembler::is_simm16(disp)) {
        __ add(index, LIR_OprFact::intConst(disp), tmp);
        index = tmp;
      }
      else {
        __ move(LIR_OprFact::intConst(disp), tmp);
        __ add(tmp, index, tmp);
        index = tmp;
      }
      disp = 0;
    }
  }
  else if (disp != 0 && !Assembler::is_simm16(disp)) {
    // need to load the displacement into a register
    index = new_register(T_INT);
    __ move(LIR_OprFact::intConst(disp), index);
    disp = 0;
  }

  // at this point we either have base + index or base + displacement
  if (disp == 0) {
    return new LIR_Address(base, index, type);
  }
  else {
    assert(Assembler::is_simm16(disp), "must be");
    return new LIR_Address(base, disp, type);
  }
}

LIR_Address* LIRGenerator::emit_array_address(LIR_Opr array_opr,
                                              LIR_Opr index_opr,
                                              BasicType type,
                                              bool needs_card_mark) {
  int elem_size = type2aelembytes[type];
  int shift = exact_log2(elem_size);

  LIR_Opr base_opr;
  int offset = arrayOopDesc::base_offset_in_bytes(type);

  if (index_opr->is_constant()) {
    Unimplemented();
  }
  else {
#ifdef PPC64
    if (index_opr->type() == T_INT) {
      LIR_Opr tmp = new_register(T_LONG);
      __ convert(Bytecodes::_i2l, index_opr, tmp);
      index_opr = tmp;
    }
#endif

    base_opr = new_pointer_register();
    assert (index_opr->is_register(), "Must be register");
    if (shift > 0) {
      __ shift_left(index_opr, shift, base_opr);
      __ add(base_opr, array_opr, base_opr);
    } else {
      __ add(index_opr, array_opr, base_opr);
    }
  }

  if (needs_card_mark) {
    Unimplemented();
  }
  else {
    return new LIR_Address(base_opr, offset, type);
  }
}

void LIRGenerator::increment_counter(address counter, int step)
{
  Unimplemented();
}

void LIRGenerator::increment_counter(LIR_Address* addr, int step)
{
  Unimplemented();
}

void LIRGenerator::cmp_mem_int(LIR_Condition condition, LIR_Opr base, int disp,
                               int c, CodeEmitInfo* info)
{
  Unimplemented();
}

void LIRGenerator::cmp_reg_mem(LIR_Condition condition, LIR_Opr reg,
                               LIR_Opr base, int disp, BasicType type,
                               CodeEmitInfo* info)
{
  __ unimplemented (__FILE__, __LINE__);
}

void LIRGenerator::cmp_reg_mem(LIR_Condition condition, LIR_Opr reg,
                               LIR_Opr base, LIR_Opr disp, BasicType type,
                               CodeEmitInfo* info)
{
  Unimplemented();
}

bool LIRGenerator::strength_reduce_multiply(LIR_Opr left, int c,
                                            LIR_Opr result, LIR_Opr tmp) {
  assert(left != result, "should be different registers");
  if (is_power_of_2(c + 1)) {
    __ shift_left(left, log2_intptr(c + 1), result);
    __ sub(result, left, result);
    return true;
  } else if (is_power_of_2(c - 1)) {
    __ shift_left(left, log2_intptr(c - 1), result);
    __ add(result, left, result);
    return true;
  }
  return false;
}

//----------------------------------------------------------------------
//             visitor functions
//----------------------------------------------------------------------

void LIRGenerator::do_StoreIndexed(StoreIndexed* x)
{
  Unimplemented();
}

void LIRGenerator::do_MonitorEnter(MonitorEnter* x)
{
  Unimplemented();
}

void LIRGenerator::do_MonitorExit(MonitorExit* x)
{
  Unimplemented();
}

// _ineg, _lneg, _fneg, _dneg
void LIRGenerator::do_NegateOp(NegateOp* x)
{
  Unimplemented();
}

// for  _fadd, _fmul, _fsub, _fdiv, _frem
//      _dadd, _dmul, _dsub, _ddiv, _drem
void LIRGenerator::do_ArithmeticOp_FPU(ArithmeticOp* x)
{
  Unimplemented();
}
  
// for  _ladd, _lmul, _lsub, _ldiv, _lrem
void LIRGenerator::do_ArithmeticOp_Long(ArithmeticOp* x)
{
  Unimplemented();
}

// for: _iadd, _imul, _isub, _idiv, _irem
void LIRGenerator::do_ArithmeticOp_Int(ArithmeticOp* x)
{
  LIRItem left(x->x(), this);
  LIRItem right(x->y(), this);
  // missing test if instr is commutative and if we should swap
  right.load_nonconstant();
  assert(right.is_constant() || right.is_register(), "wrong state of right");
  left.load_item();
  rlock_result(x);
  if (x->op() == Bytecodes::_idiv || x->op() == Bytecodes::_irem) {
    Unimplemented();
  }
  else {
    // XXX x86 uses shift for some multiplies here
    arithmetic_op_int(x->op(), x->operand(), left.result(), right.result(),
                      LIR_OprFact::illegalOpr);
  }
}

void LIRGenerator::do_ArithmeticOp(ArithmeticOp* x)
{
  ValueTag tag = x->type()->tag();
  assert(x->x()->type()->tag() == tag &&
         x->y()->type()->tag() == tag, "wrong parameters");

  switch (tag) {
  case floatTag:
  case doubleTag:
    do_ArithmeticOp_FPU(x);
    break;

  case longTag:
    do_ArithmeticOp_Long(x);
    break;

  case intTag:
    do_ArithmeticOp_Int(x);
    break;

  default:
    ShouldNotReachHere();    
  }
}

// _ishl, _lshl, _ishr, _lshr, _iushr, _lushr
void LIRGenerator::do_ShiftOp(ShiftOp* x)
{
  Unimplemented();
}

// _iand, _land, _ior, _lor, _ixor, _lxor
void LIRGenerator::do_LogicOp(LogicOp* x)
{
  Unimplemented();
}

// _lcmp, _fcmpl, _fcmpg, _dcmpl, _dcmpg
void LIRGenerator::do_CompareOp(CompareOp* x)
{
  Unimplemented();
}

void LIRGenerator::do_AttemptUpdate(Intrinsic* x)
{
  Unimplemented();
}

void LIRGenerator::do_CompareAndSwap(Intrinsic* x, ValueType* type)
{
  Unimplemented();
}

void LIRGenerator::do_MathIntrinsic(Intrinsic* x)
{
  Unimplemented();
}

void LIRGenerator::do_ArrayCopy(Intrinsic* x)
{
  Unimplemented();
}

// _i2l, _i2f, _i2d, _l2i, _l2f, _l2d,
// _f2i, _f2l, _f2d, _d2i, _d2l, _d2f,
// _i2b, _i2c, _i2s
void LIRGenerator::do_Convert(Convert* x)
{
  Unimplemented();
}

void LIRGenerator::do_NewInstance(NewInstance* x)
{
  if (PrintNotLoaded && !x->klass()->is_loaded()) {
    tty->print_cr("   ###class not loaded at new bci %d", x->bci());
  }
  CodeEmitInfo* info = state_for(x, x->state());
  const LIR_Opr reg = result_register_for(x->type());
  LIR_Opr klass_reg = new_register(objectType);
  new_instance(reg, x->klass(),
               LIR_OprFact::illegalOpr,
               LIR_OprFact::illegalOpr,
               LIR_OprFact::illegalOpr,
               LIR_OprFact::illegalOpr, klass_reg, info);
  LIR_Opr result = rlock_result(x);
  __ move(reg, result);
}

void LIRGenerator::do_NewTypeArray(NewTypeArray* x)
{
  Unimplemented();
}

void LIRGenerator::do_NewObjectArray(NewObjectArray* x)
{
  Unimplemented();
}

void LIRGenerator::do_NewMultiArray(NewMultiArray* x)
{
  Unimplemented();
}

void LIRGenerator::do_BlockBegin(BlockBegin* x)
{
  __ unimplemented (__FILE__, __LINE__);
}

void LIRGenerator::do_CheckCast(CheckCast* x)
{
  Unimplemented();
}

void LIRGenerator::do_InstanceOf(InstanceOf* x)
{
  Unimplemented();
}

void LIRGenerator::do_If(If* x)
{
  __ unimplemented(__FILE__, __LINE__);
}

LIR_Opr LIRGenerator::getThreadPointer()
{
  Unimplemented();
}

void LIRGenerator::trace_block_entry(BlockBegin* block)
{
  Unimplemented();
}

void LIRGenerator::volatile_field_store(LIR_Opr value, LIR_Address* address,
                                        CodeEmitInfo* info)
{
  Unimplemented();
}

void LIRGenerator::volatile_field_load(LIR_Address* address, LIR_Opr result,
                                       CodeEmitInfo* info)
{
  Unimplemented();
}

void LIRGenerator::put_Object_unsafe(LIR_Opr src, LIR_Opr offset, LIR_Opr data,
                                     BasicType type, bool is_volatile)
{
  Unimplemented();
}

void LIRGenerator::get_Object_unsafe(LIR_Opr dst, LIR_Opr src, LIR_Opr offset,
                                     BasicType type, bool is_volatile)
{
  Unimplemented();
}
