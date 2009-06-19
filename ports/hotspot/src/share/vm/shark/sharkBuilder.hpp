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

class SharkBuilder : public llvm::IRBuilder<> {
 public:
  SharkBuilder(SharkCompiler* compiler);

 private:
  SharkCompiler* _compiler;

 public:
  SharkCompiler* compiler() const
  {
    return _compiler;
  }

 private:
  llvm::Module* module() const
  {
    return compiler()->module();
  }
  llvm::ExecutionEngine* execution_engine() const
  {
    return compiler()->execution_engine();
  }

  // Helpers for creating basic blocks
  // NB don't use unless SharkFunction::CreateBlock is unavailable
 public:
  llvm::BasicBlock* GetBlockInsertionPoint() const
  {
    llvm::BasicBlock *cur = GetInsertBlock();

    // BasicBlock::Create takes an insertBefore argument, so
    // we need to find the block _after_ the current block
    llvm::Function::iterator iter = cur->getParent()->begin();
    llvm::Function::iterator end  = cur->getParent()->end();
    while (iter != end) {
      iter++;
      if (&*iter == cur) {
        iter++;
        break;
      }
    }

    if (iter == end)
      return NULL;
    else
      return iter;
  }
  llvm::BasicBlock* CreateBlock(llvm::BasicBlock* ip, const char* name="") const
  {
    return llvm::BasicBlock::Create(name, GetInsertBlock()->getParent(), ip);
  }
  
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
    llvm::Value* offset = CreateIntCast(index, SharkType::intptr_type(), false);
    if (element_bytes != 1)
      offset = CreateShl(
        offset,
        LLVMValue::intptr_constant(exact_log2(element_bytes)));
    offset = CreateAdd(
      LLVMValue::intptr_constant(in_bytes(base_offset)), offset);

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
      type2aelembytes(basic_type),
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

  llvm::Constant* pointer_constant(const void *ptr)
  {
    // Create a pointer constant that points at PTR.  We do this by
    // creating a GlobalVariable mapped at PTR.  This is a workaround
    // for http://www.llvm.org/bugs/show_bug.cgi?id=2920

    using namespace llvm;

    // This might be useful but it returns a const pointer that can't
    // be used for anything.  Go figure...
//     {
//       const GlobalValue *value
// 	= execution_engine()->getGlobalValueAtAddress(const_cast<void*>(ptr));
//       if (value)
// 	return ConstantExpr::getPtrToInt(value, SharkType::intptr_type());
//     }

    char name[128];
    snprintf(name, sizeof name - 1, "pointer_constant_%p", ptr);

    GlobalVariable *value = new GlobalVariable(SharkType::intptr_type(),
      false, GlobalValue::ExternalLinkage,
      NULL, name, module());
    execution_engine()->addGlobalMapping(value, const_cast<void*>(ptr));

    return ConstantExpr::getPtrToInt(value, SharkType::intptr_type());
  }

  // Helper for making pointers
 public:
  llvm::Constant* make_pointer(intptr_t addr, const llvm::Type* type)
  {
    return llvm::ConstantExpr::getIntToPtr(
      LLVMValue::intptr_constant(addr),
      llvm::PointerType::getUnqual(type));
  }

  // External functions (and intrinsics)
 private:
  llvm::Constant* _llvm_cmpxchg_int_fn;
  llvm::Constant* _llvm_cmpxchg_ptr_fn;  
  llvm::Constant* _llvm_memory_barrier_fn;
  llvm::Constant* _llvm_memset_fn;  
  llvm::Constant* _llvm_sin_fn;
  llvm::Constant* _llvm_cos_fn;
  llvm::Constant* _llvm_sqrt_fn;
  llvm::Constant* _llvm_log_fn;
  llvm::Constant* _llvm_log10_fn;
  llvm::Constant* _llvm_pow_fn;
  llvm::Constant* _llvm_exp_fn;

  void set_llvm_cmpxchg_int_fn(llvm::Constant* llvm_cmpxchg_int_fn)
  {
    _llvm_cmpxchg_int_fn = llvm_cmpxchg_int_fn;
  }
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
  void set_llvm_sin_fn(llvm::Constant* llvm_sin_fn)
  {
    _llvm_sin_fn = llvm_sin_fn;
  }
  void set_llvm_cos_fn(llvm::Constant* llvm_cos_fn)
  {
    _llvm_cos_fn = llvm_cos_fn;
  }
  void set_llvm_sqrt_fn(llvm::Constant* llvm_sqrt_fn)
  {
    _llvm_sqrt_fn = llvm_sqrt_fn;
  }
  void set_llvm_log_fn(llvm::Constant* llvm_log_fn)
  {
    _llvm_log_fn = llvm_log_fn;
  }
  void set_llvm_log10_fn(llvm::Constant* llvm_log10_fn)
  {
    _llvm_log10_fn = llvm_log10_fn;
  }
  void set_llvm_pow_fn(llvm::Constant* llvm_pow_fn)
  {
    _llvm_pow_fn = llvm_pow_fn;
  }
  void set_llvm_exp_fn(llvm::Constant* llvm_exp_fn)
  {
    _llvm_exp_fn = llvm_exp_fn;
  }

  void init_external_functions();

 protected:
  llvm::Constant* llvm_cmpxchg_int_fn() const
  {
    return _llvm_cmpxchg_int_fn;
  }
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
  llvm::Constant* llvm_sin_fn() const
  {
    return _llvm_sin_fn;
  }
  llvm::Constant* llvm_cos_fn() const
  {
    return _llvm_cos_fn;
  }
  llvm::Constant* llvm_sqrt_fn() const
  {
    return _llvm_sqrt_fn;
  }
  llvm::Constant* llvm_log_fn() const
  {
    return _llvm_log_fn;
  }
  llvm::Constant* llvm_log10_fn() const
  {
    return _llvm_log10_fn;
  }
  llvm::Constant* llvm_pow_fn() const
  {
    return _llvm_pow_fn;
  }
  llvm::Constant* llvm_exp_fn() const
  {
    return _llvm_exp_fn;
  }

 public:
  llvm::CallInst* CreateDump(llvm::Value* value);
  llvm::CallInst* CreateMemset(llvm::Value* dst,
                               llvm::Value* value,
                               llvm::Value* len,
                               llvm::Value* align);
  llvm::CallInst* CreateCmpxchgInt(llvm::Value* exchange_value,
                                    llvm::Value* dst,
                                    llvm::Value* compare_value);
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
          pointer_constant(((CardTableModRefBS *) bs)->byte_map_base),
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

  // CodeBuffer interface
 private:
  SharkCodeBuffer* _code_buffer;

 public:
  SharkCodeBuffer* code_buffer() const
  {
    return _code_buffer;
  }
  void set_code_buffer(SharkCodeBuffer* code_buffer)
  {
    _code_buffer = code_buffer;
  }

 public:
  llvm::Value* code_buffer_address(int offset)
  {
    return CreateAdd(
      code_buffer()->base_pc(), LLVMValue::intptr_constant(offset));
  }

 public:
  llvm::Value* CreateInlineOop(ciObject* object, const char* name = "")
  {
    return CreateLoad(
      CreateIntToPtr(
        code_buffer_address(code_buffer()->inline_oop(object)),
        llvm::PointerType::getUnqual(SharkType::jobject_type())),
      name);
  }
};
