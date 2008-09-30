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

SharkBuilder::SharkBuilder()
  : IRBuilder(),
      _module("shark"),
      _module_provider(module()),
      _execution_engine(ExecutionEngine::create(&_module_provider))
{
  init_external_functions();
}

Constant* SharkBuilder::make_function(intptr_t            addr,
                                      const FunctionType* sig,
                                      const char*         name)
{
  Constant *func = make_pointer(addr, sig);

#ifndef PRODUCT
  ResourceMark rm;

  // Use a trampoline to make dumped code more readable
  Function *trampoline = (Function *) module()->getOrInsertFunction(name, sig);
  SetInsertPoint(BasicBlock::Create("", trampoline));

  Value **args = NEW_RESOURCE_ARRAY(Value*, trampoline->arg_size());
  Function::arg_iterator ai = trampoline->arg_begin();
  for (unsigned i = 0; i < trampoline->arg_size(); i++)
    args[i] = ai++;

  Value *result = CreateCall(func, args, args + trampoline->arg_size());
  if (sig->getReturnType() == Type::VoidTy)
    CreateRetVoid();
  else
    CreateRet(result);

  func = trampoline;
#endif // !PRODUCT

  return func;
}

void SharkBuilder::init_external_functions()
{
  std::vector<const Type*> params;
  params.push_back(PointerType::getUnqual(SharkType::jbyte_type()));
  params.push_back(SharkType::jbyte_type());
  params.push_back(SharkType::jint_type());
  params.push_back(SharkType::jint_type());
  FunctionType *type = FunctionType::get(Type::VoidTy, params, false);
  set_llvm_memset_fn(module()->getOrInsertFunction("llvm.memset.i32", type));

  params.clear();
  params.push_back(PointerType::getUnqual(SharkType::intptr_type()));
  params.push_back(SharkType::intptr_type());
  params.push_back(SharkType::intptr_type());
  type = FunctionType::get(SharkType::intptr_type(), params, false);
  set_llvm_cmpxchg_ptr_fn(
    module()->getOrInsertFunction(
      "llvm.atomic.cmp.swap.i" LP64_ONLY("64") NOT_LP64("32"), type));

  params.clear();
  for (int i = 0; i < 5; i++)
    params.push_back(Type::Int1Ty);
  type = FunctionType::get(Type::VoidTy, params, false);
  set_llvm_memory_barrier_fn(
    module()->getOrInsertFunction("llvm.memory.barrier", type));
}

Function *SharkBuilder::CreateFunction()
{
  Function *function = Function::Create(
      SharkType::entry_point_type(),
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
  return CreateCall2(SharkRuntime::dump(), name, value);
}

CallInst* SharkBuilder::CreateCmpxchgPtr(Value* exchange_value,
                                         Value* dst,
                                         Value* compare_value)
{
  return CreateCall3(
    llvm_cmpxchg_ptr_fn(), dst, compare_value, exchange_value);
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
    SharkRuntime::unimplemented(),
    LLVMValue::intptr_constant((intptr_t) file),
    LLVMValue::jint_constant(line));
}

CallInst* SharkBuilder::CreateShouldNotReachHere(const char* file, int line)
{
  return CreateCall2(
    SharkRuntime::should_not_reach_here(),
    LLVMValue::intptr_constant((intptr_t) file),
    LLVMValue::jint_constant(line));
}

CallInst *SharkBuilder::CreateMemoryBarrier(BarrierFlags flags)
{
  Value *args[] = {
    ConstantInt::get(Type::Int1Ty, (flags & BARRIER_LOADLOAD) ? 1 : 0),
    ConstantInt::get(Type::Int1Ty, (flags & BARRIER_LOADSTORE) ? 1 : 0),
    ConstantInt::get(Type::Int1Ty, (flags & BARRIER_STORELOAD) ? 1 : 0),
    ConstantInt::get(Type::Int1Ty, (flags & BARRIER_STORESTORE) ? 1 : 0),
    ConstantInt::get(Type::Int1Ty, 0)};
  return CreateCall(llvm_memory_barrier_fn(), args, args + 5);
}
