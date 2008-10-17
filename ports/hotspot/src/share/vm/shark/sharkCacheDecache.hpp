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

// Class hierarchy:
// - SharkStateScanner
//   - SharkCacherDecacher
//     - SharkDecacher
//       - SharkJavaCallDecacher
//       - SharkVMCallDecacher
//       - SharkTrapDecacher
//     - SharkCacher
//       - SharkJavaCallCacher
//       - SharkVMCallCacher

class SharkCacherDecacher : public SharkStateScanner {
 protected:
  SharkCacherDecacher(SharkFunction* function, int bci)
    : SharkStateScanner(function), _bci(bci) {}

 protected:
  SharkBuilder* builder() const
  {
    return function()->builder();
  }  

 private:
  int _bci;
  
 protected:
  int bci() const
  {
    return _bci;
  }

  // Helper
 protected:
  static int adjusted_offset(SharkValue* value, int offset)
  {
    if (value->is_two_word())
      offset--;
    return offset;
  }
};

class SharkDecacher : public SharkCacherDecacher {
 protected:
  SharkDecacher(SharkFunction* function, int bci)
    : SharkCacherDecacher(function, bci) {}

 private:
  DebugInformationRecorder* debug_info() const
  {
    return function()->debug_info();
  }

 private:
  int                           _pc_offset;
  OopMap*                       _oopmap;
  GrowableArray<ScopeValue*>*   _exparray;
  GrowableArray<MonitorValue*>* _monarray;
  GrowableArray<ScopeValue*>*   _locarray;

 private:
  int pc_offset() const
  {
    return _pc_offset;
  }
  OopMap* oopmap() const
  {
    return _oopmap;
  }
  GrowableArray<ScopeValue*>* exparray() const
  {
    return _exparray;
  }
  GrowableArray<MonitorValue*>* monarray() const
  {
    return _monarray;
  }
  GrowableArray<ScopeValue*>* locarray() const
  {
    return _locarray;
  }

  // Callbacks
 protected:
  void start_frame();

  void start_stack(int num_slots, int max_slots);
  void process_stack_slot(int index, SharkValue** value, int offset);

  void start_monitors(int num_monitors);
  void process_monitor(int index, int offset);

  void process_exception_slot(int offset);
  void process_method_slot(llvm::Value** value, int offset);
  void process_pc_slot(int offset);
  
  void start_locals(int num_locals);
  void process_local_slot(int index, SharkValue** value, int offset);

  void end_frame();

  // oopmap and debuginfo helpers
 private:
  static int oopmap_slot_munge(int x)
  {
    return x << (LogBytesPerWord - LogBytesPerInt);
  }
  static VMReg slot2reg(int offset)
  {
    return VMRegImpl::stack2reg(oopmap_slot_munge(offset));
  }
  static Location slot2loc(int offset, Location::Type type)
  {
    return Location::new_stk_loc(type, offset * wordSize);
  }
  static LocationValue* slot2lv(int offset, Location::Type type)
  {
    return new LocationValue(slot2loc(offset, type));
  }

  // Stack slot helpers
 protected:
  virtual bool stack_slot_needs_write(int index, SharkValue* value) = 0;
  virtual bool stack_slot_needs_oopmap(int index, SharkValue* value) = 0;
  virtual bool stack_slot_needs_debuginfo(int index, SharkValue* value) = 0;

  static Location::Type stack_location_type(int index, SharkValue** addr)
  {
    if (addr[0] && addr[0]->is_jobject())
      return Location::oop;
    return Location::normal;
  }

  // Local slot helpers
 protected:
  virtual bool local_slot_needs_write(int index, SharkValue* value) = 0;
  virtual bool local_slot_needs_oopmap(int index, SharkValue* value) = 0;
  virtual bool local_slot_needs_debuginfo(int index, SharkValue* value) = 0;

  static Location::Type local_location_type(int index, SharkValue** addr)
  {
    if (addr[0] && addr[0]->is_jobject())
      return Location::oop;
    if (addr[0])
      return Location::normal;
    if (index > 0 && addr[-1] && addr[-1]->is_two_word())
      return Location::normal;
    return Location::invalid;
  }
};

class SharkJavaCallDecacher : public SharkDecacher {
 public:
  SharkJavaCallDecacher(SharkFunction* function, int bci, ciMethod* callee)
    : SharkDecacher(function, bci), _callee(callee) {}

 private:
  ciMethod* _callee;

 protected:
  ciMethod* callee() const
  {
    return _callee;
  }

  // Stack slot helpers
 protected:
  bool stack_slot_needs_write(int index, SharkValue* value)
  {
    return value && (index < callee()->arg_size() || value->is_jobject());
  }
  bool stack_slot_needs_oopmap(int index, SharkValue* value)
  {
    return value && value->is_jobject() && index >= callee()->arg_size();
  }
  bool stack_slot_needs_debuginfo(int index, SharkValue* value)
  {
    return index >= callee()->arg_size();
  }

  // Local slot helpers
 protected:
  bool local_slot_needs_write(int index, SharkValue* value)
  {
    return value && value->is_jobject();
  }
  bool local_slot_needs_oopmap(int index, SharkValue* value)
  {
    return value && value->is_jobject();
  }
  bool local_slot_needs_debuginfo(int index, SharkValue* value)
  {
    return true;
  }
};

class SharkVMCallDecacher : public SharkDecacher {
 public:
  SharkVMCallDecacher(SharkFunction* function, int bci)
    : SharkDecacher(function, bci) {}

  // Stack slot helpers
 protected:
  bool stack_slot_needs_write(int index, SharkValue* value)
  {
    return value && value->is_jobject();
  }
  bool stack_slot_needs_oopmap(int index, SharkValue* value)
  {
    return value && value->is_jobject();
  }
  bool stack_slot_needs_debuginfo(int index, SharkValue* value)
  {
    return true;
  }

  // Local slot helpers
 protected:
  bool local_slot_needs_write(int index, SharkValue* value)
  {
    return value && value->is_jobject();
  }
  bool local_slot_needs_oopmap(int index, SharkValue* value)
  {
    return value && value->is_jobject();
  }
  bool local_slot_needs_debuginfo(int index, SharkValue* value)
  {
    return true;
  }
};

class SharkTrapDecacher : public SharkDecacher {
 public:
  SharkTrapDecacher(SharkFunction* function, int bci)
    : SharkDecacher(function, bci) {}

  // Stack slot helpers
 protected:
  bool stack_slot_needs_write(int index, SharkValue* value)
  {
    return value != NULL;
  }
  bool stack_slot_needs_oopmap(int index, SharkValue* value)
  {
    return value && value->is_jobject();
  }
  bool stack_slot_needs_debuginfo(int index, SharkValue* value)
  {
    return true;
  }

  // Local slot helpers
 protected:
  bool local_slot_needs_write(int index, SharkValue* value)
  {
    return value != NULL;
  }
  bool local_slot_needs_oopmap(int index, SharkValue* value)
  {
    return value && value->is_jobject();
  }
  bool local_slot_needs_debuginfo(int index, SharkValue* value)
  {
    return true;
  }
};

class SharkCacher : public SharkCacherDecacher {
 protected:
  SharkCacher(SharkFunction* function, int bci)
    : SharkCacherDecacher(function, bci) {}

  // Callbacks
 protected:
  void process_stack_slot(int index, SharkValue** value, int offset);

  void process_method_slot(llvm::Value** value, int offset);

  void process_local_slot(int index, SharkValue** value, int offset);

  // Stack slot helper
 protected:
  virtual bool stack_slot_needs_read(int index, SharkValue* value) = 0;

  // Local slot helper
 protected:
  bool local_slot_needs_read(int index, SharkValue* value)
  {
    return value && value->is_jobject();
  }
};

class SharkJavaCallCacher : public SharkCacher {
 public:
  SharkJavaCallCacher(SharkFunction* function, int bci, ciMethod* callee)
    : SharkCacher(function, bci), _callee(callee) {}

 private:
  ciMethod* _callee;

 protected:
  ciMethod* callee() const
  {
    return _callee;
  }

  // Stack slot helper
 protected:
  bool stack_slot_needs_read(int index, SharkValue* value)
  {
    return value && (index < callee()->return_type()->size() ||
                     value->is_jobject());
  }
};

class SharkVMCallCacher : public SharkCacher {
 public:
  SharkVMCallCacher(SharkFunction* function, int bci)
    : SharkCacher(function, bci) {}

  // Stack slot helper
 protected:
  bool stack_slot_needs_read(int index, SharkValue* value)
  {
    return value && value->is_jobject();
  }
};
