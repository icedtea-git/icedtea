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
    current_state()->decache();
    builder()->CreateDump(LLVMValue::jint_constant(start()));
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

    case Bytecodes::_arraylength:
      do_arraylength();
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

    case Bytecodes::_new:
      do_new();
      break;

    case Bytecodes::_monitorenter:
      do_monitorenter();
      break;
    case Bytecodes::_monitorexit:
      do_monitorexit();
      break;

    default:
      {
        const char *code = Bytecodes::name(bc());
        int buflen = 19 + strlen(code) + 1;
        char *buf = (char *) function()->env()->arena()->Amalloc(buflen);
        snprintf(buf, buflen, "Unhandled bytecode %s", code);
        function()->record_method_not_compilable(buf);
        return;
      }
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

  Value *length = builder()->CreateArrayLength(array->jarray_value());
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

CallInst* SharkBlock::call_vm_base(Constant* callee,
                                   Value**   args_start,
                                   Value**   args_end)
{
  // Decache the state
  current_state()->decache();

  // Set up the Java frame anchor  
  function()->set_last_Java_frame();

  // Make the call
  CallInst *result = builder()->CreateCall(callee, args_start, args_end);

  // Clear the frame anchor
  function()->reset_last_Java_frame();

  // Recache the state
  current_state()->cache();

  // Check for pending exceptions  
  check_pending_exception();

  return result;
}

void SharkBlock::do_ldc()
{
  SharkValue *value = SharkValue::from_ciConstant(iter()->get_constant());
  if (value == NULL) {
    SharkConstantPool constants(this);

    BasicBlock *string     = function()->CreateBlock("string");
    BasicBlock *klass      = function()->CreateBlock("klass");
    BasicBlock *unresolved = function()->CreateBlock("unresolved");
    BasicBlock *unknown    = function()->CreateBlock("unknown");
    BasicBlock *done       = function()->CreateBlock("done");
    
    SwitchInst *switchinst = builder()->CreateSwitch(
      constants.tag_at(iter()->get_constant_index()),
      unknown, 5);

    switchinst->addCase(
      LLVMValue::jbyte_constant(JVM_CONSTANT_String), string);
    switchinst->addCase(
      LLVMValue::jbyte_constant(JVM_CONSTANT_Class), klass);
    switchinst->addCase(
      LLVMValue::jbyte_constant(JVM_CONSTANT_UnresolvedString), unresolved);
    switchinst->addCase(
      LLVMValue::jbyte_constant(JVM_CONSTANT_UnresolvedClass), unresolved);
    switchinst->addCase(
      LLVMValue::jbyte_constant(JVM_CONSTANT_UnresolvedClassInError),
      unresolved);

    builder()->SetInsertPoint(string);
    Value *string_value = constants.object_at(iter()->get_constant_index());
    builder()->CreateBr(done);

    builder()->SetInsertPoint(klass);
    builder()->CreateUnimplemented(__FILE__, __LINE__);
    Value *klass_value = LLVMValue::null();
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
    phi->addIncoming(string_value, string);
    phi->addIncoming(klass_value, klass);
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
  assert(element_type->basic_type() == basic_type, "type mismatch");

  check_null(array);
  check_bounds(array, index);

  Value *value = builder()->CreateArrayLoad(
    array->jarray_value(), basic_type, index->jint_value());

  const Type *stack_type = SharkType::to_stackType(basic_type);
  if (value->getType() != stack_type)
    value = builder()->CreateIntCast(value, stack_type, basic_type != T_CHAR);

  switch (basic_type) {
  case T_BOOLEAN:
  case T_BYTE:
  case T_CHAR:
  case T_SHORT:
  case T_INT:
    push(SharkValue::create_jint(value));
    break;

  case T_OBJECT:
    push(SharkValue::create_jobject(value));
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

  case T_LONG:
    value = svalue->jlong_value();
    break;

  case T_OBJECT:
    value = svalue->jobject_value();
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
      "field");
  
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

void SharkBlock::do_return(BasicType basic_type)
{
  add_safepoint();

  if (target()->is_synchronized())
    monitor(0)->release();

  Value *result_addr = function()->CreatePopFrame(type2size[basic_type]);
  if (basic_type != T_VOID) {
    SharkValue *result = pop();

#ifdef ASSERT
    switch (result->basic_type()) {
    case T_BOOLEAN:
    case T_BYTE:
    case T_CHAR:
    case T_SHORT:
      assert(basic_type == T_INT, "type mismatch");
      break;

    case T_ARRAY:
      assert(basic_type == T_OBJECT, "type mismatch");
      break;

    default:
      assert(result->basic_type() == basic_type, "type mismatch");
    }
#endif // ASSERT

    builder()->CreateStore(
      result->generic_value(),
      builder()->CreateIntToPtr(
        result_addr,
        PointerType::getUnqual(SharkType::to_stackType(basic_type))));
  }

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
    int shark_slot = 0, java_slot = 0;
    while (java_slot < method->arg_size() - 1)
      java_slot += stack(shark_slot++)->type()->size();
    receiver = stack(shark_slot);
    check_null(receiver);
  }

  // Find the method we are calling
  Value *callee = NULL;
  SharkConstantPool constants(this);
  Value *cache = constants.cache_entry_at(iter()->get_method_index());

  if (bc() == Bytecodes::_invokevirtual) {
    BasicBlock *final     = function()->CreateBlock("final");
    BasicBlock *not_final = function()->CreateBlock("not_final");
    BasicBlock *invoke    = function()->CreateBlock("invoke");

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
    builder()->CreateBr(invoke);

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

    Value *nonfinal_callee = builder()->CreateArrayLoad(
      klass,
      SharkType::methodOop_type(),
      vtableEntry::size() * wordSize,
      in_ByteSize(instanceKlass::vtable_start_offset() * wordSize),
      index,
      "nonfinal_callee");
    builder()->CreateBr(invoke);

    builder()->SetInsertPoint(invoke);
    callee = builder()->CreatePHI(SharkType::methodOop_type(), "callee");
    ((PHINode *) callee)->addIncoming(final_callee, final);
    ((PHINode *) callee)->addIncoming(nonfinal_callee, not_final);
  }
  else if (bc() == Bytecodes::_invokeinterface) {
    BasicBlock *hacky     = function()->CreateBlock("hacky");
    BasicBlock *normal    = function()->CreateBlock("normal");
    BasicBlock *loop      = function()->CreateBlock("loop");
    BasicBlock *got_null  = function()->CreateBlock("got_null");
    BasicBlock *not_null  = function()->CreateBlock("not_null");
    BasicBlock *next      = function()->CreateBlock("next");
    BasicBlock *got_entry = function()->CreateBlock("got_entry");
    BasicBlock *invoke    = function()->CreateBlock("invoke");

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
    builder()->CreateUnimplemented(__FILE__, __LINE__);
    Value *hacky_callee =
      ConstantPointerNull::get(SharkType::methodOop_type());
    builder()->CreateBr(invoke);

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

    bool needs_aligning = HeapWordsPerLong > 1;
    const char *itable_start_name = "itable_start";
    Value *itable_start = builder()->CreateAdd(
      vtable_start,
      builder()->CreateShl(
        vtable_length,
        LLVMValue::jint_constant(exact_log2(vtableEntry::size() * wordSize))),
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
              LLVMValue::jint_constant(
                exact_log2(itableMethodEntry::size() * wordSize)))),
          LLVMValue::intptr_constant(
            itableMethodEntry::method_offset_in_bytes())),
        PointerType::getUnqual(SharkType::methodOop_type())),
      "normal_callee");
    builder()->CreateBr(invoke);

    builder()->SetInsertPoint(invoke);
    callee = builder()->CreatePHI(SharkType::methodOop_type(), "callee");
    ((PHINode *) callee)->addIncoming(hacky_callee, hacky);
    ((PHINode *) callee)->addIncoming(normal_callee, got_entry);    
  }
  else {
    callee = builder()->CreateValueOfStructEntry(
      cache, ConstantPoolCacheEntry::f1_offset(),
      SharkType::methodOop_type(),
      "callee");
  }

  Value *entry_point = builder()->CreateValueOfStructEntry(
    callee, methodOopDesc::from_interpreted_offset(), // XXX hacky
    PointerType::getUnqual(SharkType::interpreter_entry_type()),
    "entry_point");

  // Decache for the call
  current_state()->decache(method);

  // Make the call
  BasicBlock *interpreter_call = function()->CreateBlock("interpreter_call");
  BasicBlock *shark_call       = function()->CreateBlock("shark_call");
  BasicBlock *post_call        = function()->CreateBlock("post_call");

  Value *entry_point_or_sharkmethod = builder()->CreateValueOfStructEntry(
    callee, methodOopDesc::from_interpreted_offset(),
    SharkType::intptr_type(),
    "entry_point_or_sharkmethod");

  builder()->CreateCondBr(
    builder()->CreateICmpEQ(
      builder()->CreateAnd(
        entry_point_or_sharkmethod,
        LLVMValue::intptr_constant(SharkMethod::marker())),
      LLVMValue::intptr_constant(0)),
    interpreter_call, shark_call);

  // Interpreter calls
  builder()->SetInsertPoint(interpreter_call);

  Value *interpreter_entry_point = builder()->CreateIntToPtr(
    entry_point_or_sharkmethod,
    PointerType::getUnqual(SharkType::interpreter_entry_type()),
    "interpreter_entry_point");

  builder()->CreateCall2(interpreter_entry_point, callee, thread());
  builder()->CreateBr(post_call);

  // Shark calls
  builder()->SetInsertPoint(shark_call);

  Value *base_pc = builder()->CreateAnd(
    entry_point_or_sharkmethod,
    LLVMValue::intptr_constant(~SharkMethod::marker()),
    "base_pc");

  Value *shark_entry_point = builder()->CreateLoad(
    builder()->CreateIntToPtr(
      builder()->CreateAdd(
        base_pc,
        LLVMValue::intptr_constant(
          in_bytes(SharkMethod::entry_point_offset()))),
      PointerType::getUnqual(
        PointerType::getUnqual(SharkType::shark_entry_type()))),
    "shark_entry_point");

  builder()->CreateCall3(shark_entry_point, callee, base_pc, thread());
  builder()->CreateBr(post_call);

  // Recache state
  builder()->SetInsertPoint(post_call); 
  current_state()->cache(method);

  // Check for pending exceptions
  check_pending_exception();
}

void SharkBlock::do_instance_check()
{
  BasicBlock *not_null      = function()->CreateBlock("not_null");
  BasicBlock *resolve       = function()->CreateBlock("resolve");
  BasicBlock *resolved      = function()->CreateBlock("resolved");
  BasicBlock *subtype_check = function()->CreateBlock("subtype_check");
  BasicBlock *failure       = function()->CreateBlock("failure");
  BasicBlock *success       = function()->CreateBlock("success");

  SharkValue *sharkobject = pop();
  Value *object = sharkobject->jobject_value();

  // Null objects aren't instances of anything
  builder()->CreateCondBr(
    builder()->CreateICmpEQ(object, LLVMValue::null()),
    bc() == Bytecodes::_checkcast ? success : failure, not_null);
  builder()->SetInsertPoint(not_null);

  // Get the class we're checking against
  SharkConstantPool constants(this);
  Value *tag = constants.tag_at(iter()->get_klass_index());
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
  builder()->CreateUnreachable(); // XXX builder()->CreateBr(resolved);

  builder()->SetInsertPoint(resolved);
  Value *check_klass = constants.object_at(iter()->get_klass_index());

  // Get the class of the object being tested
  Value *object_klass = builder()->CreateValueOfStructEntry(
    object, in_ByteSize(oopDesc::klass_offset_in_bytes()),
    SharkType::jobject_type(),
    "object_klass");

  // Perform the check
  builder()->CreateCondBr(
    builder()->CreateICmpEQ(check_klass, object_klass),
    success, subtype_check);
  
  builder()->SetInsertPoint(subtype_check);
  builder()->CreateCondBr(
    builder()->CreateICmpNE(
      builder()->CreateCall2(
        SharkRuntime::is_subtype_of(), check_klass, object_klass),
      LLVMValue::jbyte_constant(0)),
    success, failure);

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

void SharkBlock::do_new()
{
  bool will_link;
  ciInstanceKlass* klass = iter()->get_klass(will_link)->as_instance_klass();
  assert(will_link, "typeflow responsibility");

  BasicBlock *got_tlab            = NULL;
  BasicBlock *heap_alloc          = NULL;
  BasicBlock *retry               = NULL;
  BasicBlock *got_heap            = NULL;
  BasicBlock *initialize          = NULL;
  BasicBlock *slow_alloc_and_init = NULL;
  BasicBlock *got_slow            = NULL;
  BasicBlock *push_object         = NULL;

  SharkState *fast_state = NULL;
  
  Value *tlab_object = NULL;
  Value *heap_object = NULL;
  Value *fast_object = NULL;
  Value *slow_object = NULL;
  Value *object      = NULL;

  // The fast path
  if (!Klass::layout_helper_needs_slow_path(klass->layout_helper())) {
    if (UseTLAB) {
      got_tlab          = function()->CreateBlock("got_tlab");
      heap_alloc        = function()->CreateBlock("heap_alloc");
    }
    retry               = function()->CreateBlock("retry");
    got_heap            = function()->CreateBlock("got_heap");
    initialize          = function()->CreateBlock("initialize");
    slow_alloc_and_init = function()->CreateBlock("slow_alloc_and_init");
    push_object         = function()->CreateBlock("push_object");

    size_t size_in_bytes = klass->size_helper() << LogHeapWordSize;

    // Thread local allocation
    if (UseTLAB) {
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
      
      builder()->SetInsertPoint(heap_alloc);
    }

    // Heap allocation
    Value *top_addr = builder()->CreateIntToPtr(
      LLVMValue::intptr_constant((intptr_t) Universe::heap()->top_addr()),
      PointerType::getUnqual(SharkType::intptr_type()),
      "top_addr");

    Value *end = builder()->CreateLoad(
      builder()->CreateIntToPtr(
        LLVMValue::intptr_constant((intptr_t) Universe::heap()->end_addr()),
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
    SharkConstantPool constants(this);
    Value *rtklass = constants.object_at(iter()->get_klass_index());
    builder()->CreateStore(rtklass, klass_addr);

    builder()->CreateBr(push_object);
    builder()->SetInsertPoint(slow_alloc_and_init);
    fast_state = current_state()->copy();
  }

  // The slow path
  SharkConstantPool constants(this);
  call_vm(
    SharkRuntime::new_instance(),
    constants.object_at(iter()->get_klass_index()));
  slow_object = function()->CreateLoadVMResult();
  got_slow = builder()->GetInsertBlock();

  // Push the object
  if (push_object) {
    builder()->CreateBr(push_object);
    builder()->SetInsertPoint(push_object);
  }
  if (fast_object) {
    PHINode *phi = builder()->CreatePHI(SharkType::jobject_type(), "object");
    phi->addIncoming(fast_object, initialize);
    phi->addIncoming(slow_object, got_slow);
    object = phi;
    current_state()->merge(fast_state, initialize, got_slow);
  }
  else {
    object = slow_object;
  }

  push(SharkValue::create_jobject(object));
}

void SharkBlock::do_monitorenter()
{
  SharkValue *lockee = pop();
  builder()->CreateUnimplemented(__FILE__, __LINE__);
}

void SharkBlock::do_monitorexit()
{
  SharkValue *lockee = pop();
  builder()->CreateUnimplemented(__FILE__, __LINE__);
}
