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
  SharkBuilder(llvm::Module* module);

  // The module we build our functions into
 private:
  llvm::Module* _module;

 protected:
  llvm::Module* module() const
  {
    return _module;
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

  llvm::LoadInst* CreateArrayLoad(llvm::Value* arrayoop,
                                  BasicType    basic_type,
                                  ByteSize     base_offset,
                                  llvm::Value* index,
                                  const char*  name = "")
  {
    llvm::Value* offset = index;

    if (type2aelembytes[basic_type] != 1)
      offset = CreateShl(
        offset,
        LLVMValue::jint_constant(exact_log2(type2aelembytes[basic_type])));
    offset = CreateAdd(LLVMValue::jint_constant(in_bytes(base_offset)),offset);

    const llvm::Type *array_type = SharkType::to_arrayType(basic_type);
    return CreateLoad(
      CreateIntToPtr(
        CreateAdd(CreatePtrToInt(arrayoop, SharkType::intptr_type()), offset),
        llvm::PointerType::getUnqual(array_type)),
      name);
  }

  llvm::StoreInst* CreateArrayStore(llvm::Value* arrayoop,
                                    BasicType    basic_type,
                                    ByteSize     base_offset,
                                    llvm::Value* index,
                                    llvm::Value* value)
  {
    llvm::Value* offset = index;

    if (type2aelembytes[basic_type] != 1)
      offset = CreateShl(
        offset,
        LLVMValue::jint_constant(exact_log2(type2aelembytes[basic_type])));
    offset = CreateAdd(LLVMValue::jint_constant(in_bytes(base_offset)),offset);

    const llvm::Type *array_type = SharkType::to_arrayType(basic_type);
    return CreateStore(
      value,
      CreateIntToPtr(
        CreateAdd(CreatePtrToInt(arrayoop, SharkType::intptr_type()), offset),
        llvm::PointerType::getUnqual(array_type)));
  }

  llvm::LoadInst* CreateArrayLoad(llvm::Value* arrayoop,
                                  BasicType    basic_type,
                                  llvm::Value* index,
                                  const char*  name = "")
  {
    return CreateArrayLoad(
      arrayoop, basic_type,
      in_ByteSize(arrayOopDesc::base_offset_in_bytes(basic_type)),
      index, name);
  }

  llvm::StoreInst* CreateArrayStore(llvm::Value* arrayoop,
                                    BasicType    basic_type,
                                    llvm::Value* index,
                                    llvm::Value* value)
  {
    return CreateArrayStore(
      arrayoop, basic_type,
      in_ByteSize(arrayOopDesc::base_offset_in_bytes(basic_type)),
      index, value);
  }

  // Helper for making pointers
 private:
  llvm::Constant* make_pointer(intptr_t addr, const llvm::Type* type) const
  {
    return llvm::ConstantExpr::getIntToPtr(
      LLVMValue::intptr_constant(addr),
      llvm::PointerType::getUnqual(type));
  }

  // External functions (and intrinsics)
 private:
  llvm::Constant* _dump_fn;
  llvm::Constant* _llvm_memset_fn;  
  llvm::Constant* _report_should_not_reach_here_fn;
  llvm::Constant* _trace_bytecode_fn;
  llvm::Constant* _report_unimplemented_fn;

  void set_dump_fn(llvm::Constant* dump_fn)
  {
    _dump_fn = dump_fn;
  }
  void set_llvm_memset_fn(llvm::Constant* llvm_memset_fn)
  {
    _llvm_memset_fn = llvm_memset_fn;
  }
  void set_report_should_not_reach_here_fn(llvm::Constant* report_snrh_fn)
  {
    _report_should_not_reach_here_fn = report_snrh_fn;
  }
  void set_trace_bytecode_fn(llvm::Constant* trace_bytecode_fn)
  {
    _trace_bytecode_fn = trace_bytecode_fn;
  }
  void set_report_unimplemented_fn(llvm::Constant* report_unimplemented_fn)
  {
    _report_unimplemented_fn = report_unimplemented_fn;
  }

  void init_external_functions();

  static void dump(const char *name, intptr_t value);
  static void trace_bytecode(int bci, const char* bc);

 protected:
  llvm::Constant* dump_fn() const
  {
    return _dump_fn;
  }
  llvm::Constant* llvm_memset_fn() const
  {
    return _llvm_memset_fn;
  }
  llvm::Constant* report_should_not_reach_here_fn() const
  {
    return _report_should_not_reach_here_fn;
  }
  llvm::Constant* trace_bytecode_fn() const
  {
    return _trace_bytecode_fn;
  }
  llvm::Constant* report_unimplemented_fn() const
  {
    return _report_unimplemented_fn;
  }

 public:
  llvm::CallInst* CreateDump(llvm::Value* value);
  llvm::CallInst* CreateMemset(llvm::Value* dst,
                               llvm::Value* value,
                               llvm::Value* len,
                               llvm::Value* align);
  llvm::CallInst* CreateShouldNotReachHere(const char* file, int line);  
  llvm::CallInst* CreateTraceBytecode(int bci, Bytecodes::Code bc);
  llvm::CallInst* CreateUnimplemented(const char* file, int line);  

  // Memory barriers
 public:
  void CreateUpdateBarrierSet(BarrierSet*  bs,
                              llvm::Value* field,
                              llvm::Value* new_value)
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
};
