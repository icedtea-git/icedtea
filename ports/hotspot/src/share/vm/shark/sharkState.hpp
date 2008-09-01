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

class SharkState : public ResourceObj {
  friend class SharkEntryState;
  friend class SharkPHIState;
  friend class SharkTrackingState;

 protected:
  SharkState(SharkBlock* block)
    : _block(block) { initialize(); }

 private:
  void initialize();

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
 protected:
  llvm::Value* _method;
  SharkValue** _locals;
  SharkValue** _stack;
  SharkValue** _sp;

  // Method
 public:
  llvm::Value* method() const
  {
    return _method;
  }

  // Local variables
 public:
  SharkValue* local(int index) const
  {
    assert(index >= 0 && index < max_locals(), "bad local variable index");
    return _locals[index];
  }

  // Expression stack
 public:
  SharkValue* stack(int slot) const
  {
    assert(slot >= 0 && slot < stack_depth(), "bad stack slot");
    return _sp[-(slot + 1)];
  }
  void set_stack(int slot, SharkValue* value)
  {
    assert(slot >= 0 && slot < stack_depth(), "bad stack slot");
    _sp[-(slot + 1)] = value;
  }
  int stack_depth() const
  {
    return _sp - _stack;
  }
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

#ifdef ASSERT
 private:
  int _stack_depth_at_entry;
#endif // ASSERT

 public:
  void add_incoming(SharkState* incoming_state);
};

class SharkTrackingState : public SharkState {
 public:
  SharkTrackingState(const SharkState* initial_state)
    : SharkState(initial_state->block()) { initialize(initial_state); }

 private:
  void initialize(const SharkState* initial_state);

  // Method
 public:
  void set_method(llvm::Value* method)
  {
    _method = method;
  }

  // Local variables
 public:
  void set_local(int index, SharkValue* value)
  {
    assert(index >= 0 && index < max_locals(), "bad local variable index");
    _locals[index] = value;
  }

  // Expression stack
 public:
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
  void set_stack(int slot, SharkValue* value)
  {
    assert(slot >= 0 && slot < stack_depth(), "bad stack slot");
    _sp[-(slot + 1)] = value;
  }
  int stack_depth_in_slots() const;

  // Cache and decache
 public:
  void cache(ciMethod *callee = NULL);
  void decache(ciMethod *callee = NULL);

  // Decache helpers
 private:
  static int oopmap_slot_munge(int x)
  {
    return x << (LogBytesPerWord - LogBytesPerInt);
  }
  static VMReg slot2reg(int offset)
  {
    return VMRegImpl::stack2reg(oopmap_slot_munge(offset));
  }

  // Copy and merge
 public:
  SharkTrackingState* copy() const
  {
    return new SharkTrackingState(this);
  }
  void merge(SharkState*       other,
             llvm::BasicBlock* other_block,
             llvm::BasicBlock* this_block);
};
