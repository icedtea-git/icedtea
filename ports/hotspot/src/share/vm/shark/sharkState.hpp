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

class SharkBlock;

class SharkState : public ResourceObj {
 protected:
  SharkState(SharkBlock* block)
    : _block(block)          { initialize(NULL); }
  SharkState(const SharkState* state)
    : _block(state->block()) { initialize(state); }

 private:
  void initialize(const SharkState* state);

 private:
  SharkBlock* _block;

 public:
  SharkBlock *block() const
  {
    return _block;
  }

 protected:
  inline SharkBuilder* builder() const;
  inline SharkFunction* function() const;

 public:
  inline int max_locals() const;
  inline int max_stack() const;

  // The values we are tracking
 private:
  llvm::Value* _method;
  SharkValue** _locals;
  SharkValue** _stack;
  SharkValue** _sp;

  // Method
 public:
  llvm::Value** method_addr()
  {
    return &_method;
  }
  llvm::Value* method() const
  {
    return _method;
  }
  void set_method(llvm::Value* method)
  {
    _method = method;
  }

  // Local variables
 public:
  SharkValue** local_addr(int index) const
  {
    assert(index >= 0 && index < max_locals(), "bad local variable index");
    return &_locals[index];
  }
  SharkValue* local(int index) const
  {
    return *local_addr(index);
  }
  void set_local(int index, SharkValue* value)
  {
    *local_addr(index) = value;
  }

  // Expression stack
 public:
  SharkValue** stack_addr(int slot) const
  {
    assert(slot >= 0 && slot < stack_depth(), "bad stack slot");
    return &_sp[-(slot + 1)];
  }
  SharkValue* stack(int slot) const
  {
    return *stack_addr(slot);
  }
 protected:
  void set_stack(int slot, SharkValue* value)
  {
    *stack_addr(slot) = value;
  }
 public:
  int stack_depth() const
  {
    return _sp - _stack;
  }
  void push(SharkValue* value)
  {
    assert(stack_depth() < max_stack(), "stack overrun");
    *(_sp++) = value;
  }
  SharkValue* pop()
  {
    assert(stack_depth() > 0, "stack underrun");
    return *(--_sp);
  }
  void pop(int slots)
  {
    assert(stack_depth() >= slots, "stack underrun");
    _sp -= slots;
  }  
  inline int stack_depth_at_entry() const;
};

class SharkEntryState : public SharkState {
 public:
  SharkEntryState(llvm::Value* method, SharkBlock* start_block)
    : SharkState(start_block) { initialize(method); }

 private:
  void initialize(llvm::Value* method);
};

class SharkPHIState : public SharkState {
 public:
  SharkPHIState(SharkBlock* block)
    : SharkState(block) { initialize(); }

 private:
  void initialize();

 public:
  void add_incoming(SharkState* incoming_state);
};

class SharkTrackingState : public SharkState {
 public:
  SharkTrackingState(const SharkState* state)
    : SharkState(state)
  {
    set_method(state->method());
    NOT_PRODUCT(set_has_stack_frame(true));
  }

  // Cache and decache
 public:
  inline void decache_for_Java_call(ciMethod* callee);
  inline void cache_after_Java_call(ciMethod* callee);
  inline void decache_for_VM_call();
  inline void cache_after_VM_call();
  inline void decache_for_trap();

  // Copy and merge
 public:
  SharkTrackingState* copy() const
  {
    return new SharkTrackingState(this);
  }
  void merge(SharkState*       other,
             llvm::BasicBlock* other_block,
             llvm::BasicBlock* this_block);

  // Inlining
#ifndef PRODUCT
 private:
  bool _has_stack_frame;

 protected:
  bool has_stack_frame() const
  {
    return _has_stack_frame;
  }
  void set_has_stack_frame(bool has_stack_frame)
  {
    _has_stack_frame = has_stack_frame;
  }
#endif // PRODUCT

 public:
  void enter_inlined_section() PRODUCT_RETURN;
  void leave_inlined_section() PRODUCT_RETURN;
};
