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

class SharkBlock;
class SharkMonitor;

class SharkFunction : public StackObj {
 public:
  SharkFunction(SharkBuilder*     builder,
                const char*       name,
                ciTypeFlow*       flow,
                ciBytecodeStream* iter,
                MacroAssembler*   masm)
    : _builder(builder),
      _name(name),
      _flow(flow),
      _iter(iter),
      _masm(masm)
  { initialize(); }

 private:
  void initialize();

 private:
  SharkBuilder*     _builder;
  const char*       _name;
  ciTypeFlow*       _flow;
  ciBytecodeStream* _iter;
  MacroAssembler*   _masm;
  llvm::Function*   _function;
  SharkBlock**      _blocks;
  llvm::Value*      _base_pc;
  llvm::Value*      _thread;
  int               _monitor_count;

 public:  
  SharkBuilder* builder() const
  {
    return _builder;
  }
  const char* name() const
  {
    return _name;
  }
  ciTypeFlow* flow() const
  {
    return _flow;
  }
  ciBytecodeStream* iter() const
  {
    return _iter;
  }
  MacroAssembler* masm() const
  {
    return _masm;
  }
  llvm::Function* function() const
  {
    return _function;
  }
  SharkBlock* block(int i) const
  {
    return _blocks[i];
  }
  llvm::Value* base_pc() const
  {
    return _base_pc;
  }
  llvm::Value* thread() const
  {
    return _thread;
  }
  int monitor_count() const
  {
    return _monitor_count;
  }

 public:
  int arg_size() const
  {
    return target()->arg_size();
  }
  int block_count() const
  {
    return flow()->block_count();
  }
  DebugInformationRecorder *debug_info() const
  {
    return env()->debug_info();
  }
  ciEnv* env() const
  {
    return flow()->env();
  }
  int max_locals() const
  {
    return flow()->max_locals();
  }
  int max_stack() const
  {
    return flow()->max_stack();
  }
  ciMethod* target() const
  {
    return flow()->method();
  }

  // Block management
 private:
  llvm::BasicBlock* _block_insertion_point;

  void set_block_insertion_point(llvm::BasicBlock* block_insertion_point)
  {
    _block_insertion_point = block_insertion_point;
  }
  llvm::BasicBlock* block_insertion_point() const
  {
    return _block_insertion_point;
  }

 public:
  llvm::BasicBlock* CreateBlock(const char* name = "") const
  {
    return llvm::BasicBlock::Create(name, function(), block_insertion_point());
  }

  // Stack management
 private:
  llvm::Value* _zero_stack_base;
  llvm::Value* _zero_stack_pointer_addr;
  llvm::Value* _zero_frame_pointer_addr;

 private:
  llvm::Value* zero_stack_base() const 
  {
    return _zero_stack_base;
  }
  llvm::Value* zero_stack_pointer_addr() const 
  {
    return _zero_stack_pointer_addr;
  }
  llvm::Value* zero_frame_pointer_addr() const 
  {
    return _zero_frame_pointer_addr;
  }

 private:
  void CreateInitZeroStack();

 public:
  llvm::LoadInst* CreateLoadZeroStackPointer(const char *name = "")
  {
    return builder()->CreateLoad(zero_stack_pointer_addr(), name);
  }
  llvm::StoreInst* CreateStoreZeroStackPointer(llvm::Value* value)
  {
    return builder()->CreateStore(value, zero_stack_pointer_addr());
  }

 private:
  llvm::LoadInst* CreateLoadZeroFramePointer(const char *name = "")
  {
    return builder()->CreateLoad(zero_frame_pointer_addr(), name);
  }
  llvm::StoreInst* CreateStoreZeroFramePointer(llvm::Value* value)
  {
    return builder()->CreateStore(value, zero_frame_pointer_addr());
  }

 private:
  void CreateStackOverflowCheck(llvm::Value* sp);

 public:
  void CreatePushFrame(llvm::Value* fp);
  llvm::Value* CreatePopFrame(int result_slots);

  // Frame management
 private:
  llvm::Value* _frame;

 public:
  llvm::Value* CreateAddressOfFrameEntry(int               offset,
                                         const llvm::Type* type = NULL,
                                         const char*       name = "") const;
 public:
  llvm::Value* exception_slot() const
  {
    return CreateAddressOfFrameEntry(
      exception_slot_offset(),
      SharkType::oop_type(),
      "exception_slot");
  }
  llvm::Value* monitors_slots() const
  {
    return CreateAddressOfFrameEntry(
      monitors_slots_offset(),
      llvm::ArrayType::get(SharkType::monitor_type(), monitor_count()),
      "monitors");
  }

 public:
  SharkMonitor* monitor(int index) const
  {
    return monitor(LLVMValue::jint_constant(index));
  }
  SharkMonitor* monitor(llvm::Value* index) const;

 private:
  llvm::Value* CreateBuildFrame();

  // OopMap support
 public:
  // Every time a new, distinct pc is required, an extra byte is
  // emitted into the codebuffer
  int code_offset() const
  {
    int offset = masm()->offset();
    masm()->advance(1); // keeps PCs unique
    return offset;
  }

 private:
  int _oopmap_frame_size;
  int _stack_slots_offset;
  int _monitors_slots_offset;
  int _exception_slot_offset;
  int _method_slot_offset;
  int _pc_slot_offset;
  int _locals_slots_offset;

 public:
  int oopmap_frame_size() const
  {
    return _oopmap_frame_size;
  }
  int stack_slots_offset() const
  {
    return _stack_slots_offset;
  }
  int monitors_slots_offset() const
  {
    return _monitors_slots_offset;
  }
  int exception_slot_offset() const
  {
    return _exception_slot_offset;
  }
  int method_slot_offset() const
  {
    return _method_slot_offset;
  }
  int pc_slot_offset() const
  {
    return _pc_slot_offset;
  }
  int locals_slots_offset() const
  {
    return _locals_slots_offset;
  }

  // VM interface
 private:
  llvm::StoreInst* CreateStoreLastJavaSP(llvm::Value* value) const
  {
    return builder()->CreateStore(
      value,
      builder()->CreateAddressOfStructEntry(
        thread(), JavaThread::last_Java_sp_offset(),
        llvm::PointerType::getUnqual(SharkType::intptr_type()),
        "last_Java_sp_addr"));
  }

 public:
  void set_last_Java_frame()
  {
    CreateStoreLastJavaSP(CreateLoadZeroFramePointer());
  }
  void reset_last_Java_frame()
  {
    CreateStoreLastJavaSP(LLVMValue::intptr_constant(0));
  }

 public:
  llvm::LoadInst* CreateGetVMResult() const
  {
    llvm::Value *addr = builder()->CreateAddressOfStructEntry(
      thread(), JavaThread::vm_result_offset(),
      llvm::PointerType::getUnqual(SharkType::jobject_type()),
      "vm_result_addr");
    llvm::LoadInst *result = builder()->CreateLoad(addr, "vm_result");
    builder()->CreateStore(LLVMValue::null(), addr);
    return result;
  }

 public:
  llvm::Value* pending_exception_address() const
  {
    return builder()->CreateAddressOfStructEntry(
      thread(), Thread::pending_exception_offset(),
      llvm::PointerType::getUnqual(SharkType::jobject_type()),
      "pending_exception_addr");
  }
  llvm::LoadInst* CreateGetPendingException() const
  {
    llvm::Value *addr = pending_exception_address();
    llvm::LoadInst *result = builder()->CreateLoad(addr, "pending_exception");
    builder()->CreateStore(LLVMValue::null(), addr);
    return result;
  }
};
