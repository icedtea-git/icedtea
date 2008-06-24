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
#include "incls/_sharkBuilder.cpp.incl"

using namespace llvm;

SharkBuilder::SharkBuilder(Module* module)
    : IRBuilder(),
      _module(module)
{
  init_external_functions();
}

void SharkBuilder::init_external_functions()
{
  std::vector<const Type*> params;
  params.push_back(SharkType::intptr_type());
  params.push_back(SharkType::jint_type());
  FunctionType *type = FunctionType::get(Type::VoidTy, params, false);
  Constant *func = make_pointer((intptr_t) report_should_not_reach_here, type);
#ifndef PRODUCT
  // Use a trampoline to make dumped code more readable
  Function *trampoline =
    (Function *) module()->getOrInsertFunction("should_not_reach_here", type);
  Function::arg_iterator ai = trampoline->arg_begin();
  Value *file = ai++;
  Value *line = ai++;
  SetInsertPoint(BasicBlock::Create("", trampoline));
  CreateCall2(func, file, line);
  CreateRetVoid();
  func = trampoline;
#endif // !PRODUCT
  set_report_should_not_reach_here_fn(func);

  // XXX copy and paste -- refactor this!
  func = make_pointer((intptr_t) report_unimplemented, type);
#ifndef PRODUCT
  // Use a trampoline to make dumped code more readable
  trampoline =
    (Function *) module()->getOrInsertFunction("unimplemented", type);
  ai = trampoline->arg_begin();
  file = ai++;
  line = ai++;
  SetInsertPoint(BasicBlock::Create("", trampoline));
  CreateCall2(func, file, line);
  CreateRetVoid();
  func = trampoline;
#endif // !PRODUCT
  set_report_unimplemented_fn(func);

  // XXX copy and paste -- refactor this!
  params.clear();
  params.push_back(SharkType::jint_type());
  params.push_back(SharkType::intptr_type());
  func = make_pointer((intptr_t) trace_bytecode, type);
#ifndef PRODUCT
  // Use a trampoline to make dumped code more readable
  trampoline =
    (Function *) module()->getOrInsertFunction("trace_bytecode", type);
  ai = trampoline->arg_begin();
  file = ai++;
  line = ai++;
  SetInsertPoint(BasicBlock::Create("", trampoline));
  CreateCall2(func, file, line);
  CreateRetVoid();
  func = trampoline;
#endif // !PRODUCT
  set_trace_bytecode_fn(func);

  params.clear();
  params.push_back(SharkType::intptr_type());
  params.push_back(SharkType::jbyte_type());
  params.push_back(SharkType::jint_type());
  params.push_back(SharkType::jint_type());
  type = FunctionType::get(Type::VoidTy, params, false);
  set_llvm_memset_fn(module()->getOrInsertFunction("llvm.memset.i32", type));

  params.clear();
  params.push_back(SharkType::intptr_type());
  params.push_back(SharkType::intptr_type());
  type = FunctionType::get(Type::VoidTy, params, false);
  func = make_pointer((intptr_t) dump, type);
#ifndef PRODUCT
  // Use a trampoline to make dumped code more readable
  trampoline =
    (Function *) module()->getOrInsertFunction("print_value", type);
  ai = trampoline->arg_begin();
  file = ai++;
  line = ai++;
  SetInsertPoint(BasicBlock::Create("", trampoline));
  CreateCall2(func, file, line);
  CreateRetVoid();
  func = trampoline;
#endif // !PRODUCT
  set_dump_fn(func);
}

Function *SharkBuilder::CreateFunction()
{
  Function *function = Function::Create(
      SharkType::method_entry_type(),
      GlobalVariable::InternalLinkage,
      "func");
  module()->getFunctionList().push_back(function);
  return function;
}

CallInst* SharkBuilder::CreateDump(llvm::Value* value)
{
  Constant *const_name;
  if (value->hasName())
    const_name = ConstantArray::get(value->getName());
  else
    const_name = ConstantArray::get("unnamed_value");

  Value *name = CreatePtrToInt(
    CreateStructGEP(
      new GlobalVariable(
        const_name->getType(),
        true, GlobalValue::InternalLinkage,
        const_name, "dump", module()),
      0),
    SharkType::intptr_type());

  if (isa<PointerType>(value->getType()))
    value = CreatePtrToInt(value, SharkType::intptr_type());
  else if (value->getType()->isInteger())
    value = CreateIntCast(value, SharkType::intptr_type(), false);
  else
    Unimplemented();

  Value *args[] = {name, value};
  return CreateCall2(dump_fn(), name, value);
}

void SharkBuilder::dump(const char *name, intptr_t value)
{
  oop valueOop = (oop) value;
  tty->print("%s = ", name);
  if (valueOop->is_oop(true))
    valueOop->print_on(tty);
  else if (value >= ' ' && value <= '~')
    tty->print("'%c' (%d)", value, value);
  else
    tty->print("%d", value);
  tty->print_cr("");
}

CallInst* SharkBuilder::CreateMemset(Value* dst,
                                     Value* value,
                                     Value* len,
                                     Value* align)
{
  return CreateCall4(llvm_memset_fn(), dst, value, len, align);
}

CallInst* SharkBuilder::CreateUnimplemented(const char* file, int line)
{
  return CreateCall2(
    report_unimplemented_fn(), 
    LLVMValue::intptr_constant((intptr_t) file),
    LLVMValue::jint_constant(line));
}

CallInst* SharkBuilder::CreateShouldNotReachHere(const char* file, int line)
{
  return CreateCall2(
    report_should_not_reach_here_fn(),
    LLVMValue::intptr_constant((intptr_t) file),
    LLVMValue::jint_constant(line));
}

void SharkBuilder::trace_bytecode(int bci, const char* bc)
{
  tty->print_cr("%3d: %s", bci, bc);  
}

CallInst* SharkBuilder::CreateTraceBytecode(int bci, Bytecodes::Code bc)
{
  return CreateCall2(
    trace_bytecode_fn(),
    LLVMValue::jint_constant(bci),
    LLVMValue::intptr_constant((intptr_t) Bytecodes::name(bc)));
}
