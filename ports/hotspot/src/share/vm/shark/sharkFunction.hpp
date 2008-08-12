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
                ciTypeFlow*       flow,
                ciBytecodeStream* iter,
                CodeBuffer*       cb,
                OopMapSet*        oopmaps)
    : _builder(builder),
      _flow(flow),
      _iter(iter),
      _cb(cb),
      _oopmaps(oopmaps)
  { initialize(); }

 private:
  void initialize();

 private:
  SharkBuilder*     _builder;
  ciTypeFlow*       _flow;
  ciBytecodeStream* _iter;
  CodeBuffer*       _cb;
  OopMapSet*        _oopmaps;
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
  ciTypeFlow* flow() const
  {
    return _flow;
  }
  ciBytecodeStream* iter() const
  {
    return _iter;
  }
  CodeBuffer* cb() const
  {
    return _cb;
  }
  OopMapSet* oopmaps() const
  {
    return _oopmaps;
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

 public:
  ciEnv* env() const
  {
    return flow()->env();
  }
  bool failing() const
  {
    return env()->failing();
  }
  void record_method_not_compilable(const char* reason) const
  {
    return env()->record_method_not_compilable(reason);
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
  llvm::BasicBlock* CreateBlock(const char* name = "")
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
  llvm::Value*   _pc_slot;
  llvm::Value*   _method_slot;
  llvm::Value*   _locals_slots;
  SharkMonitor** _monitors;
  llvm::Value*   _stack_slots;

 public:
  llvm::Value* pc_slot() const
  {
    return _pc_slot;
  }  
  llvm::Value* method_slot() const
  {
    return _method_slot;
  }  
  llvm::Value* locals_slots() const
  {
    return _locals_slots;
  }  
  SharkMonitor* monitor(int index) const
  {
    return _monitors[index];
  }
  llvm::Value* stack_slots() const
  {
    return _stack_slots;
  }

 private:
  llvm::Value* CreateBuildFrame();

  // OopMap support
 public:
  int code_offset() const
  {
    int offset = masm()->offset();
    masm()->emit_zero_byte(); // keeps PCs unique
    return offset;
  }
  void add_gc_map(int offset, OopMap* oopmap)
  {
    oopmaps()->add_gc_map(offset, oopmap);
  }

 private:
  int _oopmap_frame_size;
  int _stack_slots_offset;
  int _monitors_slots_offset;
  int _method_slot_offset;
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
  int method_slot_offset() const
  {
    return _method_slot_offset;
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
  llvm::LoadInst* CreateLoadVMResult() const
  {
    return builder()->CreateValueOfStructEntry(
      thread(), JavaThread::vm_result_offset(),
      SharkType::jobject_type(),
      "vm_result");
  }
};
