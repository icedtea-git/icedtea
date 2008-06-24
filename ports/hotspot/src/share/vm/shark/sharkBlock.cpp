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

#include "incls/_precompiled.incl"
#include "incls/_sharkBlock.cpp.incl"
#include "ciArrayKlass.hpp" // XXX fuck you makeDeps

using namespace llvm;

void SharkBlock::initialize()
{
  char name[28];
  snprintf(name, sizeof(name),
           "bci_%d%s",
           ciblock()->start(),
           ciblock()->is_private_copy() ? "_private_copy" : "");
  _entry_block = function()->CreateBlock(name);

  // Cached values
  _constant_pool_check       = NULL;
  _constant_pool_tags_check  = NULL;
  _constant_pool_cache_check = NULL;
}

void SharkBlock::parse()
{
  SharkValue *a, *b, *c, *d;
  int i;

  builder()->SetInsertPoint(entry_block());

  if (never_entered()) {
    NOT_PRODUCT(warning("skipping unentered block"));
    builder()->CreateShouldNotReachHere(__FILE__, __LINE__);
    builder()->CreateUnreachable();
    return;
  }

  if (ciblock()->has_trap()) {
    builder()->CreateUnimplemented(__FILE__, __LINE__);
    builder()->CreateUnreachable();
    return;
  }

  iter()->reset_to_bci(start());
  while (iter()->next() != ciBytecodeStream::EOBC() && bci() < limit()) {
    NOT_PRODUCT(a = b = c = d = NULL);    

    if (SharkTraceBytecodes)
      tty->print_cr("%4d: %s", bci(), Bytecodes::name(bc()));
    
    if(UseLoopSafepoints) {
      int len;

      switch (bc()) {
      case Bytecodes::_goto:
      case Bytecodes::_goto_w:
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

      case Bytecodes::_tableswitch:
        len = iter()->get_int_table(2) - iter()->get_int_table(1) + 1;
        for (i = 0; i < len + 3; i++) {
          if (i != 1 && i != 2) {
            if (iter()->get_dest_table(i) <= bci()) {
              add_safepoint();
              break;
            }
          }
        }
        break;
      }
    }

    switch (bc()) {
    case Bytecodes::_nop:
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

    case Bytecodes::_bipush:
      push(SharkValue::jint_constant(iter()->get_byte()));
      break;
    case Bytecodes::_sipush:
      push(SharkValue::jint_constant(iter()->get_short()));
      break;

    case Bytecodes::_aconst_null:
      push(SharkValue::null());
      break;

    case Bytecodes::_ldc:
    case Bytecodes::_ldc_w:
    case Bytecodes::_ldc2_w:
      do_ldc();
      break;

    case Bytecodes::_iload_0:
    case Bytecodes::_fload_0:
    case Bytecodes::_aload_0:
      push(local(0));
      break;
    case Bytecodes::_iload_1:
    case Bytecodes::_fload_1:
    case Bytecodes::_aload_1:
      push(local(1));
      break;
    case Bytecodes::_iload_2:
    case Bytecodes::_fload_2:
    case Bytecodes::_aload_2:
      push(local(2));
      break;
    case Bytecodes::_iload_3:
    case Bytecodes::_fload_3:
    case Bytecodes::_aload_3:
      push(local(3));
      break;
    case Bytecodes::_iload:
    case Bytecodes::_fload:
    case Bytecodes::_aload:
      push(local(iter()->get_index()));
      break;

    case Bytecodes::_istore_0:
    case Bytecodes::_fstore_0:
    case Bytecodes::_astore_0:
      set_local(0, pop());
      break;
    case Bytecodes::_istore_1:
    case Bytecodes::_fstore_1:
    case Bytecodes::_astore_1:
      set_local(1, pop());
      break;
    case Bytecodes::_istore_2:
    case Bytecodes::_fstore_2:
    case Bytecodes::_astore_2:
      set_local(2, pop());
      break;
    case Bytecodes::_istore_3:
    case Bytecodes::_fstore_3:
    case Bytecodes::_astore_3:
      set_local(3, pop());
      break;
    case Bytecodes::_istore:
    case Bytecodes::_fstore:
    case Bytecodes::_astore:
      set_local(iter()->get_index(), pop());
      break;

    case Bytecodes::_pop:
      assert(stack(0)->is_one_word(), "should be");
      pop();
      break;
    case Bytecodes::_dup:
      assert(stack(0)->is_one_word(), "should be");
      a = pop();
      push(a);
      push(a);
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

    case Bytecodes::_imul:
      b = pop();
      a = pop();
      push(SharkValue::create_jint(
        builder()->CreateMul(a->jint_value(), b->jint_value())));
      break;
    case Bytecodes::_iadd:
      b = pop();
      a = pop();
      push(SharkValue::create_jint(
        builder()->CreateAdd(a->jint_value(), b->jint_value())));
      break;
    case Bytecodes::_ineg:
      a = pop();      
      push(SharkValue::create_jint(
        builder()->CreateNeg(a->jint_value())));
      break;
    case Bytecodes::_isub:
      b = pop();
      a = pop();
      push(SharkValue::create_jint(
        builder()->CreateSub(a->jint_value(), b->jint_value())));
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
    case Bytecodes::_ishl:
      b = pop();
      a = pop();
      push(SharkValue::create_jint(
        builder()->CreateShl(a->jint_value(), b->jint_value())));
      break;
    case Bytecodes::_ishr:
      b = pop();
      a = pop();
      push(SharkValue::create_jint(
        builder()->CreateAShr(a->jint_value(), b->jint_value())));
      break;
    case Bytecodes::_iushr:
      b = pop();
      a = pop();
      push(SharkValue::create_jint(
        builder()->CreateLShr(a->jint_value(), b->jint_value())));
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

    case Bytecodes::_iinc:
      i = iter()->get_index();
      set_local(
        i,
        SharkValue::create_jint(
          builder()->CreateAdd(
            LLVMValue::jint_constant(iter()->get_iinc_con()),
            local(i)->jint_value())));
      break;
      
    case Bytecodes::_return:
      do_return(T_VOID);
      break;
    case Bytecodes::_ireturn:
      do_return(T_INT);
      break;
    case Bytecodes::_freturn:
      do_return(T_FLOAT);
      break;
    case Bytecodes::_areturn:
      do_return(T_OBJECT);
      break;

    case Bytecodes::_athrow:
      builder()->CreateUnimplemented(__FILE__, __LINE__);
      builder()->CreateUnreachable();
      break;

    case Bytecodes::_goto:
    case Bytecodes::_goto_w:
      builder()->CreateBr(successor(ciTypeFlow::GOTO_TARGET)->entry_block());
      break;

    case Bytecodes::_ifnull:
      do_if(ICmpInst::ICMP_EQ, SharkValue::null(), pop());
      break;
    case Bytecodes::_ifnonnull:
      do_if(ICmpInst::ICMP_NE, SharkValue::null(), pop());
      break;
    case Bytecodes::_if_acmpeq:
      do_if(ICmpInst::ICMP_EQ, pop(), pop());
      break;
    case Bytecodes::_if_acmpne:
      do_if(ICmpInst::ICMP_NE, pop(), pop());
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
      do_if(ICmpInst::ICMP_EQ, pop(), pop());
      break;
    case Bytecodes::_if_icmpne:
      do_if(ICmpInst::ICMP_NE, pop(), pop());
      break;
    case Bytecodes::_if_icmplt:
      do_if(ICmpInst::ICMP_SLT, pop(), pop());
      break;
    case Bytecodes::_if_icmple:
      do_if(ICmpInst::ICMP_SLE, pop(), pop());
      break;
    case Bytecodes::_if_icmpgt:
      do_if(ICmpInst::ICMP_SGT, pop(), pop());
      break;
    case Bytecodes::_if_icmpge:
      do_if(ICmpInst::ICMP_SGE, pop(), pop());
      break;

    case Bytecodes::_tableswitch:
      do_tableswitch();
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

    default:
      tty->print_cr("Unhandled bytecode %s", Bytecodes::name(bc()));
      ShouldNotReachHere();
    }
  }

  if (falls_through()) {
    builder()->CreateBr(successor(ciTypeFlow::FALL_THROUGH)->entry_block());
  }

  for (int i = 0; i < num_successors(); i++)
    successor(i)->add_incoming(current_state());
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

Value *SharkBlock::constant_pool()
{
  Value *m = method();
  if (m != _constant_pool_check) {
    _constant_pool_value = builder()->CreateValueOfStructEntry(
      m, methodOopDesc::constants_offset(),
      SharkType::oop_type(), "constants");
    _constant_pool_check = m;
  }
  return _constant_pool_value;
}

Value *SharkBlock::constant_pool_tags()
{
  Value *cp = constant_pool();
  if (cp != _constant_pool_tags_check) {
    _constant_pool_tags_value = builder()->CreateValueOfStructEntry(
      cp, in_ByteSize(constantPoolOopDesc::tags_offset_in_bytes()),
      SharkType::oop_type(), "tags");
    _constant_pool_tags_check = cp;
  }
  return _constant_pool_tags_value;
}

Value *SharkBlock::constant_pool_cache()
{
  Value *cp = constant_pool();
  if (cp != _constant_pool_cache_check) {
    _constant_pool_cache_value = builder()->CreateValueOfStructEntry(
      cp, in_ByteSize(constantPoolOopDesc::cache_offset_in_bytes()),
      SharkType::oop_type(), "cache");
    _constant_pool_cache_check = cp;
  }
  return _constant_pool_cache_value;
}

Value *SharkBlock::constant_pool_tag_at(int which)
{
  return builder()->CreateArrayLoad(
    constant_pool_tags(), T_BYTE, LLVMValue::jint_constant(which));
}

Value *SharkBlock::constant_pool_object_at(int which)
{
  return builder()->CreateArrayLoad(
    constant_pool(),
    T_OBJECT, in_ByteSize(sizeof(constantPoolOopDesc)),
    LLVMValue::jint_constant(which));
}

Value *SharkBlock::constant_pool_cache_entry_at(int which)
{
  Value *entry = builder()->CreateIntToPtr(
    builder()->CreateAdd(
      builder()->CreatePtrToInt(
        constant_pool_cache(), SharkType::intptr_type()),
      LLVMValue::intptr_constant(
        in_bytes(constantPoolCacheOopDesc::base_offset()) +
        which * sizeof(ConstantPoolCacheEntry))),
    SharkType::cpCacheEntry_type());

#ifdef ASSERT
  // Check the entry is resolved
  int shift;
  switch (ConstantPoolCacheEntry::bytecode_number(bc())) {
  case 1:
    shift = 16;
    break;
  case 2:
    shift = 24;
    break;
  default:
    ShouldNotReachHere();
  }

  Value *opcode = builder()->CreateAnd(
    builder()->CreateLShr(
      builder()->CreateValueOfStructEntry(
        entry, ConstantPoolCacheEntry::indices_offset(),
        SharkType::intptr_type()),
      LLVMValue::jint_constant(shift)),
    LLVMValue::intptr_constant(0xff));

  BasicBlock *not_resolved = function()->CreateBlock("not_resolved");
  BasicBlock *resolved     = function()->CreateBlock("resolved");

  builder()->CreateCondBr(
    builder()->CreateICmpEQ(opcode, LLVMValue::intptr_constant(bc())),
    resolved, not_resolved);

  builder()->SetInsertPoint(not_resolved);  
  builder()->CreateShouldNotReachHere(__FILE__, __LINE__);
  builder()->CreateUnreachable();

  builder()->SetInsertPoint(resolved);
#endif // ASSERT

  return entry;
}

void SharkBlock::check_null(SharkValue *object)
{
  if (object->null_checked())
    return;

  BasicBlock *null     = function()->CreateBlock("null");
  BasicBlock *not_null = function()->CreateBlock("not_null");

  builder()->CreateCondBr(
    builder()->CreateICmpEQ(object->jobject_value(), LLVMValue::null()),
    null, not_null);

  builder()->SetInsertPoint(null);
  builder()->CreateUnimplemented(__FILE__, __LINE__);
  builder()->CreateUnreachable();

  builder()->SetInsertPoint(not_null);

  object->set_null_checked(true);
}

void SharkBlock::check_bounds(SharkValue* array, SharkValue* index)
{
  BasicBlock *out_of_bounds = function()->CreateBlock("out_of_bounds");
  BasicBlock *in_bounds     = function()->CreateBlock("in_bounds");

  Value *length = builder()->CreateValueOfStructEntry(
    array->jarray_value(), in_ByteSize(arrayOopDesc::length_offset_in_bytes()),
    SharkType::jint_type(), "length");

  builder()->CreateCondBr(
    builder()->CreateICmpSLT(index->jint_value(), length),
    in_bounds, out_of_bounds);

  builder()->SetInsertPoint(out_of_bounds);
  builder()->CreateUnimplemented(__FILE__, __LINE__);
  builder()->CreateUnreachable();

  builder()->SetInsertPoint(in_bounds);
}

void SharkBlock::check_pending_exception()
{
  BasicBlock *exception    = function()->CreateBlock("exception");
  BasicBlock *no_exception = function()->CreateBlock("no_exception");

  Value *pending_exception = builder()->CreateValueOfStructEntry(
    thread(), Thread::pending_exception_offset(),
    SharkType::jobject_type(), "pending_exception");

  builder()->CreateCondBr(
    builder()->CreateICmpEQ(pending_exception, LLVMValue::null()),
    no_exception, exception);

  builder()->SetInsertPoint(exception);
  builder()->CreateUnimplemented(__FILE__, __LINE__);
  builder()->CreateUnreachable();

  builder()->SetInsertPoint(no_exception);
}

void SharkBlock::add_safepoint()
{
  BasicBlock *do_safepoint = function()->CreateBlock("do_safepoint");
  BasicBlock *safepointed  = function()->CreateBlock("safepointed");

  Value *state = builder()->CreateLoad(
    builder()->CreateIntToPtr(
      LLVMValue::intptr_constant(
        (intptr_t) SafepointSynchronize::address_of_state()),
      PointerType::getUnqual(SharkType::jint_type())),
    "state");

  builder()->CreateCondBr(
    builder()->CreateICmpEQ(
      state,
      LLVMValue::jint_constant(SafepointSynchronize::_synchronizing)),
    do_safepoint, safepointed);

  builder()->SetInsertPoint(do_safepoint);
  // XXX decache_state
  // XXX call whatever
  // XXX cache_state
  // XXX  THEN MERGE (need phis...)
  // XXX (if it's call_VM then call_VM should do the decache/cache)
  // XXX shouldn't ever need a fixup_after_potential_safepoint
  // XXX since anything that might safepoint needs a decache/cache
  builder()->CreateUnimplemented(__FILE__, __LINE__);
  builder()->CreateUnreachable();

  builder()->SetInsertPoint(safepointed);
}

void SharkBlock::do_ldc()
{
  SharkValue *value = SharkValue::from_ciConstant(iter()->get_constant());
  if (value == NULL) {
    Unimplemented() // XXX it's an oop :(
  }
  push(value);
}

void SharkBlock::do_aload(BasicType basic_type)
{
  SharkValue *index = pop();
  SharkValue *array = pop();

  assert(array->type()->is_array_klass(), "should be");
  ciType *element_type = ((ciArrayKlass *) array->type())->element_type();
  assert(element_type->basic_type() == basic_type, "type mismatch");

  check_null(array);
  check_bounds(array, index);

  Value *value = builder()->CreateArrayLoad(
    array->jarray_value(), basic_type, index->jint_value());

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
  assert(element_type->basic_type() == basic_type, "type mismatch");

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

  default:
    tty->print_cr("Unhandled type %s", type2name(basic_type));
    ShouldNotReachHere();
  }

  const Type *array_type = SharkType::to_arrayType(basic_type);
  if (value->getType() != array_type)
    value = builder()->CreateIntCast(value, array_type, basic_type != T_CHAR);

  builder()->CreateArrayStore(
    array->jarray_value(), basic_type,
    index->jint_value(), value);
}

void SharkBlock::do_field_access(bool is_get, bool is_field)
{
  bool will_link;
  ciField *field = iter()->get_field(will_link);
  assert(will_link, "typeflow responsibility");

  // Check the bytecode matches the field
  if (is_field == field->is_static())
    Unimplemented();

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
      Value *cache = constant_pool_cache_entry_at(iter()->get_field_index());
  
      object = builder()->CreateValueOfStructEntry(
       cache, ConstantPoolCacheEntry::f1_offset(),
       SharkType::jobject_type(),
       "object");
    }
    Value *addr = builder()->CreateAddressOfStructEntry(
      object, in_ByteSize(field->offset_in_bytes()),
      PointerType::getUnqual(SharkType::to_stackType(field->type())),
      "field");
  
    // Do the access
    if (is_get) {
      value = SharkValue::create_generic(
        field->type(), builder()->CreateLoad(addr, "value"));
    }
    else {
      builder()->CreateStore(value->generic_value(), addr);
      
      if (!field->type()->is_primitive_type())
        Unimplemented(); // XXX builder()->CreateUpdateBarrierSet(
                         //       oopDesc::bs(), addr, value);
      if (field->is_volatile())
        Unimplemented(); // XXX __sync_synchronize
    }
  }

  // Push the value onto the stack where necessary
  if (is_get)
    push(value);
}

void SharkBlock::do_return(BasicType basic_type)
{
  add_safepoint();

  Value *result_addr = function()->CreatePopFrame(type2size[basic_type]);
  if (basic_type != T_VOID) {
    SharkValue *result = pop();

    assert(result->basic_type() == basic_type ||
           result->basic_type() == T_ARRAY && basic_type == T_OBJECT,
           "type mismatch");

    builder()->CreateStore(
      result->generic_value(),
      builder()->CreateIntToPtr(
        result_addr,
        PointerType::getUnqual(SharkType::to_stackType(basic_type))));
  }
  assert(stack_depth() == 0, "should be");

  builder()->CreateRetVoid();
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

void SharkBlock::do_tableswitch()
{
  int low  = iter()->get_int_table(1);
  int high = iter()->get_int_table(2);
  int len  = high - low + 1;

  SwitchInst *switchinst = builder()->CreateSwitch(
    pop()->jint_value(),
    successor(ciTypeFlow::SWITCH_DEFAULT)->entry_block(),
    len);

  for (int i = 0; i < len; i++) {
    switchinst->addCase(
      LLVMValue::jint_constant(i + low),
      bci_successor(iter()->get_dest_table(i + 3))->entry_block());
  }
}

void SharkBlock::do_call()
{
  bool will_link;
  ciMethod *method = iter()->get_method(will_link);
  assert(will_link, "typeflow responsibility");

  // Find the receiver in the stack
  SharkValue *receiver = NULL;
  if (bc() != Bytecodes::_invokestatic) {
    receiver = stack(method->arg_size() - 1);
    check_null(receiver);
  }

  // Find the method we are calling
  Value *callee = NULL;
  if (bc() == Bytecodes::_invokeinterface) {
    Unimplemented();
  }
  else {
    Value *cache = constant_pool_cache_entry_at(iter()->get_method_index());

    if (bc() == Bytecodes::_invokevirtual) {
      int mask = 1 << ConstantPoolCacheEntry::vfinalMethod;
      if (method->flags().as_int() & mask == mask) {
        callee = builder()->CreateValueOfStructEntry(
          cache, ConstantPoolCacheEntry::f2_offset(),
          SharkType::methodOop_type(),
          "callee");
      }
      else {
        Unimplemented();
      }
    }
    else {
      callee = builder()->CreateValueOfStructEntry(
        cache, ConstantPoolCacheEntry::f1_offset(),
        SharkType::methodOop_type(),
        "callee");
    }
  }

  Value *entry_point = builder()->CreateValueOfStructEntry(
    callee, methodOopDesc::from_interpreted_offset(), // XXX hacky
    PointerType::getUnqual(SharkType::method_entry_type()),
    "entry_point");

  // Make the call
  current_state()->decache(method);
  builder()->CreateCall2(entry_point, callee, thread());
  current_state()->cache(method);

  // Check for pending exceptions
  check_pending_exception();
}

void SharkBlock::do_instance_check()
{
  BasicBlock *not_null       = function()->CreateBlock("not_null");
  BasicBlock *resolve        = function()->CreateBlock("resolve");
  BasicBlock *resolved       = function()->CreateBlock("resolved");
  BasicBlock *subclass_check = function()->CreateBlock("subclass_check");
  BasicBlock *failure        = function()->CreateBlock("failure");
  BasicBlock *success        = function()->CreateBlock("success");

  SharkValue *sharkobject = pop();
  Value *object = sharkobject->jobject_value();

  // Null objects aren't instances of anything
  builder()->CreateCondBr(
    builder()->CreateICmpEQ(object, LLVMValue::null()),
    bc() == Bytecodes::_checkcast ? success : failure, not_null);
  builder()->SetInsertPoint(not_null);

  // Get the class we're checking against
  Value *tag = constant_pool_tag_at(iter()->get_klass_index());
  builder()->CreateCondBr(
    builder()->CreateOr(
      builder()->CreateICmpEQ(
        tag, LLVMValue::jbyte_constant(JVM_CONSTANT_UnresolvedClass)),
      builder()->CreateICmpEQ(
        tag, LLVMValue::jbyte_constant(JVM_CONSTANT_UnresolvedClassInError))),
    resolve, resolved);

  // If the class is unresolved we must resolve it
  builder()->SetInsertPoint(resolve);
  builder()->CreateUnimplemented(__FILE__, __LINE__);
  builder()->CreateBr(resolved);

  builder()->SetInsertPoint(resolved);
  Value *klass = constant_pool_object_at(iter()->get_klass_index());

  // Get the class of the object being tested
  Value *objKlass = builder()->CreateValueOfStructEntry(
    object, in_ByteSize(oopDesc::klass_offset_in_bytes()),
    SharkType::jobject_type(),
    "objKlass");

  // Check if it's the same class
  builder()->CreateCondBr(
    builder()->CreateICmpEQ(klass, objKlass),
    success, subclass_check);

  // Check if it's a subclass
  builder()->SetInsertPoint(subclass_check);
  builder()->CreateUnimplemented(__FILE__, __LINE__);
  builder()->CreateUnreachable();

  // Handle the result
  if (bc() == Bytecodes::_checkcast) {
    builder()->SetInsertPoint(failure);
    builder()->CreateUnimplemented(__FILE__, __LINE__);
    builder()->CreateUnreachable();

    builder()->SetInsertPoint(success);
    push(sharkobject);
  }
  else if (bc() == Bytecodes::_instanceof) {
    BasicBlock *done = function()->CreateBlock("done");

    builder()->SetInsertPoint(success);
    builder()->CreateBr(done);

    builder()->SetInsertPoint(failure);
    builder()->CreateBr(done);

    builder()->SetInsertPoint(done);
    PHINode *result = builder()->CreatePHI(SharkType::jint_type(), "result");
    result->addIncoming(LLVMValue::jint_constant(1), success);
    result->addIncoming(LLVMValue::jint_constant(0), failure);

    push(SharkValue::create_jint(result));
  }
  else {
    tty->print_cr("Unhandled bytecode %s", Bytecodes::name(bc()));
    ShouldNotReachHere();
  }
}
