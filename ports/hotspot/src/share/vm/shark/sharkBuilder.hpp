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

class SharkBuilder : public llvm::IRBuilder {
 public:
  SharkBuilder();

 private:
  llvm::Module                 _module;
  llvm::ExistingModuleProvider _module_provider;
  llvm::ExecutionEngine*       _execution_engine;

 public:
  llvm::Module* module()
  {
    return &_module;
  }
  llvm::ExecutionEngine* execution_engine() const
  {
    return _execution_engine;
  }

  // Function creation
 public:
  llvm::Function *CreateFunction();

  // Helpers for accessing structures and arrays
 public:
  llvm::Value* CreateAddressOfStructEntry(llvm::Value* base,
                                          ByteSize offset,
                                          const llvm::Type* type,
                                          const char *name = "")
  {
    return CreateBitCast(CreateStructGEP(base, in_bytes(offset)), type, name);
  }

  llvm::LoadInst* CreateValueOfStructEntry(llvm::Value* base,
                                           ByteSize offset,
                                           const llvm::Type* type,
                                           const char *name = "")
  {
    return CreateLoad(
      CreateAddressOfStructEntry(
        base, offset, llvm::PointerType::getUnqual(type)),
      name);
  }

  llvm::LoadInst* CreateArrayLength(llvm::Value* array)
  {
    return CreateValueOfStructEntry(
      array, in_ByteSize(arrayOopDesc::length_offset_in_bytes()),
      SharkType::jint_type(), "length");
  }

  llvm::Value* CreateArrayAddress(llvm::Value*      arrayoop,
                                  const llvm::Type* element_type,
                                  int               element_bytes,
                                  ByteSize          base_offset,
                                  llvm::Value*      index,
                                  const char*       name = "")
  {
    llvm::Value* offset = index;
    if (element_bytes != 1)
      offset = CreateShl(
        offset,
        LLVMValue::jint_constant(exact_log2(element_bytes)));
    offset = CreateAdd(
      LLVMValue::jint_constant(in_bytes(base_offset)), offset);

    return CreateIntToPtr(
      CreateAdd(CreatePtrToInt(arrayoop, SharkType::intptr_type()), offset),
      llvm::PointerType::getUnqual(element_type),
      name);
  }

  llvm::Value* CreateArrayAddress(llvm::Value* arrayoop,
                                  BasicType    basic_type,
                                  ByteSize     base_offset,
                                  llvm::Value* index,
                                  const char*  name = "")
  {
    return CreateArrayAddress(
      arrayoop,
      SharkType::to_arrayType(basic_type),
      type2aelembytes[basic_type],
      base_offset, index, name);
  }

  llvm::Value* CreateArrayAddress(llvm::Value* arrayoop,
                                  BasicType    basic_type,
                                  llvm::Value* index,
                                  const char*  name = "")
  {
    return CreateArrayAddress(
      arrayoop, basic_type,
      in_ByteSize(arrayOopDesc::base_offset_in_bytes(basic_type)),
      index, name);
  }

  // Helper for making function pointers
 public:
  llvm::Constant* make_function(intptr_t                  addr,
                                const llvm::FunctionType* sig,
                                const char*               name);

  // Helper for making pointers
 public:
  llvm::Constant* make_pointer(intptr_t addr, const llvm::Type* type) const
  {
    return llvm::ConstantExpr::getIntToPtr(
      LLVMValue::intptr_constant(addr),
      llvm::PointerType::getUnqual(type));
  }

  // External functions (and intrinsics)
 private:
  llvm::Constant* _llvm_cmpxchg_ptr_fn;  
  llvm::Constant* _llvm_memory_barrier_fn;
  llvm::Constant* _llvm_memset_fn;  

  void set_llvm_cmpxchg_ptr_fn(llvm::Constant* llvm_cmpxchg_ptr_fn)
  {
    _llvm_cmpxchg_ptr_fn = llvm_cmpxchg_ptr_fn;
  }
  void set_llvm_memory_barrier_fn(llvm::Constant* llvm_memory_barrier_fn)
  {
    _llvm_memory_barrier_fn = llvm_memory_barrier_fn;
  }
  void set_llvm_memset_fn(llvm::Constant* llvm_memset_fn)
  {
    _llvm_memset_fn = llvm_memset_fn;
  }

  void init_external_functions();

 protected:
  llvm::Constant* llvm_cmpxchg_ptr_fn() const
  {
    return _llvm_cmpxchg_ptr_fn;
  }
  llvm::Constant* llvm_memory_barrier_fn() const
  {
    return _llvm_memory_barrier_fn;
  }
  llvm::Constant* llvm_memset_fn() const
  {
    return _llvm_memset_fn;
  }

 public:
  llvm::CallInst* CreateDump(llvm::Value* value);
  llvm::CallInst* CreateMemset(llvm::Value* dst,
                               llvm::Value* value,
                               llvm::Value* len,
                               llvm::Value* align);
  llvm::CallInst* CreateCmpxchgPtr(llvm::Value* exchange_value,
                                   llvm::Value* dst,
                                   llvm::Value* compare_value);
  llvm::CallInst* CreateShouldNotReachHere(const char* file, int line);  
  llvm::CallInst* CreateUnimplemented(const char* file, int line);  

  // HotSpot memory barriers
 public:
  void CreateUpdateBarrierSet(BarrierSet* bs, llvm::Value* field)
  {
    if (bs->kind() != BarrierSet::CardTableModRef) {
      Unimplemented();
    }

    CreateStore(
      LLVMValue::jbyte_constant(CardTableModRefBS::dirty_card),
      CreateIntToPtr(
        CreateAdd(
          LLVMValue::intptr_constant(
            (intptr_t) ((CardTableModRefBS *) bs)->byte_map_base),
          CreateLShr(
            CreatePtrToInt(field, SharkType::intptr_type()),
            LLVMValue::intptr_constant(CardTableModRefBS::card_shift))),
        llvm::PointerType::getUnqual(SharkType::jbyte_type())));
  }

  // Hardware memory barriers
 public:
  enum BarrierFlags {
    BARRIER_LOADLOAD   = 1,
    BARRIER_LOADSTORE  = 2,
    BARRIER_STORELOAD  = 4,
    BARRIER_STORESTORE = 8
  };

 public:
  llvm::CallInst* CreateMemoryBarrier(BarrierFlags flags);

  // Alignment
 public:
  llvm::Value* CreateAlign(llvm::Value* x, intptr_t s, const char* name = "")
  {
    assert(is_power_of_2(s), "should be");
    return CreateAnd(
      CreateAdd(x, LLVMValue::intptr_constant(s - 1)),
      LLVMValue::intptr_constant(~(s - 1)),
      name);
  }
};
