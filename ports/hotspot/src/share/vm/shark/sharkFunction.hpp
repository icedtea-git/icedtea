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

class SharkFunction : public StackObj {
 public:
  SharkFunction(SharkBuilder*     builder,
                ciTypeFlow*       flow,
                ciBytecodeStream* iter)
    : _builder(builder), _flow(flow), _iter(iter), _start_block(NULL)
  { initialize(); }

 private:
  void initialize();

 private:
  SharkBuilder*     _builder;
  ciTypeFlow*       _flow;
  ciBytecodeStream* _iter;
  llvm::Function*   _function;
  SharkBlock**      _blocks;
  SharkBlock*       _start_block;
  llvm::Value*      _thread;

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
  llvm::Function* function() const
  {
    return _function;
  }
  SharkBlock* block(int i) const
  {
    return _blocks[i];
  }
  SharkBlock* start_block() const
  {
    return _start_block;
  }
  llvm::Value* thread() const
  {
    return _thread;
  }

 public:
  int arg_size() const
  {
    return flow()->method()->arg_size();
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

  void CreateStackOverflowCheck(llvm::Value* sp);

 public:
  void CreatePushFrame(llvm::Value* fp);
  llvm::Value* CreatePopFrame(int result_slots);
  
  // Frame management
 private:
  llvm::Value* _method_slot;
  llvm::Value* _locals_slots;
  llvm::Value* _stack_slots;

 public:
  llvm::Value* method_slot() const
  {
    return _method_slot;
  }  
  llvm::Value* locals_slots() const
  {
    return _locals_slots;
  }  
  llvm::Value* stack_slots() const
  {
    return _stack_slots;
  }

 private:
  llvm::Value* CreateBuildFrame();

  // Debugging
#ifndef PRODUCT
 private:
  bool _debug;
  
 public:
  bool debug() const
  {
    return _debug;
  }
#endif // PRODUCT
};
