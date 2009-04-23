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
#include "incls/_sharkBuilder.cpp.incl"

using namespace llvm;

#ifdef ARM
/*
 * ARM lacks atomic operation implementation in LLVM
 * http://llvm.org/bugs/show_bug.cgi?id=3877
 * 
 * These two functions zero_cmpxchg_int_fn and zero_cmpxchg_ptr_fn
 * are defined so that they can be inserted into llvm as a workaround to
 * make shark reroute all atomic calls back to the implementation in zero. 
 * The actual insertion are done in SharkBuilder::init_external_functions().
 */

extern "C" {
  jint zero_cmpxchg_int_fn(volatile jint *ptr,
                           jint          *oldval,
                           jint          *newval)
  {
    return Atomic::cmpxchg(*newval,
                           ptr,
                           *oldval);
  }

  intptr_t* zero_cmpxchg_ptr_fn(volatile void* ptr,
                                intptr_t*      oldval,
                                intptr_t*      newval)
  { 
    return (intptr_t *) Atomic::cmpxchg_ptr((void *) newval,
                                                     ptr,
                                            (void *) oldval);
  }
};
#endif

SharkBuilder::SharkBuilder(SharkCompiler* compiler)
  : IRBuilder<>(),
    _compiler(compiler)
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
  params.push_back(PointerType::getUnqual(SharkType::jint_type()));
  params.push_back(SharkType::jint_type());
  params.push_back(SharkType::jint_type());
  type = FunctionType::get(SharkType::jint_type(), params, false);
  set_llvm_cmpxchg_int_fn(
#ifdef ARM
    make_function(
      (intptr_t) zero_cmpxchg_int_fn,
      type,
      "zero_cmpxchg_int_fn"));
#else
    module()->getOrInsertFunction("llvm.atomic.cmp.swap.i32", type));
#endif

  params.clear();
  params.push_back(PointerType::getUnqual(SharkType::intptr_type()));
  params.push_back(SharkType::intptr_type());
  params.push_back(SharkType::intptr_type());
  type = FunctionType::get(SharkType::intptr_type(), params, false);
  set_llvm_cmpxchg_ptr_fn(
#ifdef ARM
    make_function(
      (intptr_t) zero_cmpxchg_ptr_fn,
      type,
      "zero_cmpxchg_ptr_fn"));
#else
    module()->getOrInsertFunction(
      "llvm.atomic.cmp.swap.i" LP64_ONLY("64") NOT_LP64("32"), type));
#endif

  params.clear();
  for (int i = 0; i < 5; i++)
    params.push_back(Type::Int1Ty);
  type = FunctionType::get(Type::VoidTy, params, false);
  set_llvm_memory_barrier_fn(
    module()->getOrInsertFunction("llvm.memory.barrier", type));

  params.clear();
  params.push_back(SharkType::jdouble_type());
  type = FunctionType::get(SharkType::jdouble_type(), params, false);
  set_llvm_sin_fn  (module()->getOrInsertFunction("llvm.sin.f64",   type));
  set_llvm_cos_fn  (module()->getOrInsertFunction("llvm.cos.f64",   type));
  set_llvm_sqrt_fn (module()->getOrInsertFunction("llvm.sqrt.f64",  type));
  set_llvm_log_fn  (module()->getOrInsertFunction("llvm.log.f64",   type));
  set_llvm_log10_fn(module()->getOrInsertFunction("llvm.log10.f64", type));
  set_llvm_exp_fn  (module()->getOrInsertFunction("llvm.exp.f64",   type));  

  params.clear();
  params.push_back(SharkType::jdouble_type());
  params.push_back(SharkType::jdouble_type());
  type = FunctionType::get(SharkType::jdouble_type(), params, false);
  set_llvm_pow_fn(module()->getOrInsertFunction("llvm.pow.f64", type));  
}

Function *SharkBuilder::CreateFunction(const char *name)
{
  Function *function = Function::Create(
      SharkType::entry_point_type(),
      GlobalVariable::InternalLinkage,
      name);
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

CallInst* SharkBuilder::CreateCmpxchgInt(Value* exchange_value,
                                         Value* dst,
                                         Value* compare_value)
{
  return CreateCall3(
    llvm_cmpxchg_int_fn(), dst, compare_value, exchange_value);
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
    pointer_constant(file),
    LLVMValue::jint_constant(line));
}

CallInst* SharkBuilder::CreateShouldNotReachHere(const char* file, int line)
{
  return CreateCall2(
    SharkRuntime::should_not_reach_here(),
    pointer_constant(file),
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
