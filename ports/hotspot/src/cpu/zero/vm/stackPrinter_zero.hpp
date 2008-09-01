/*
 * Copyright 2003-2007 Sun Microsystems, Inc.  All Rights Reserved.
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

class ZeroStackPrinter {
 private:
  outputStream* _st;
  char*         _buf;
  int           _buflen;

 public:
  ZeroStackPrinter(outputStream *st, char *buf, int buflen)
    : _st(st), _buf(buf), _buflen(buflen) {}

  void print(JavaThread *thread)
  {
    intptr_t *lo_addr = thread->zero_stack()->sp();
    if (!lo_addr) {
      _st->print(" stack not set up");
      return;
    }

    intptr_t *hi_addr = (intptr_t *) thread->top_zero_frame();
    if (!hi_addr) {
      _st->print("no frames pushed"); 
      return;
    }
    assert(hi_addr >= lo_addr, "corrupted stack");

    bool top_frame = true;
    while (hi_addr) {
      if (!top_frame)
        _st->cr();      
      ZeroFrame *frame = (ZeroFrame *) hi_addr;
      for (intptr_t *addr = lo_addr; addr <= hi_addr; addr++)
        print_word(frame, addr, top_frame);
      lo_addr = hi_addr + 1;
      hi_addr = *(intptr_t **) hi_addr;
      top_frame = false;
    }
  }

 private:
  void print_word(ZeroFrame *frame, intptr_t *addr, bool top_frame)
  {
    const char *field = NULL;
    const char *value = NULL;

    int word = (intptr_t *) frame - addr;
    switch (word) {
    case ZeroFrame::next_frame_off:
      field = "next_frame";
      break;
    case ZeroFrame::frame_type_off:
      field = "frame_type";
      switch (*addr) {
      case ZeroFrame::ENTRY_FRAME:
        value = "ENTRY_FRAME";
        break;
      case ZeroFrame::INTERPRETER_FRAME:
        value = "INTERPRETER_FRAME";
        break;
      case ZeroFrame::SHARK_FRAME:
        value = "SHARK_FRAME";
        break;
      }
      break;
    }

    if (!field) {
      if (frame->is_entry_frame()) {
        if (word == EntryFrame::call_wrapper_off) {
          field = "call_wrapper";
        }
        else {
          snprintf(_buf, _buflen, "local[%d]", word - 3);
          field = _buf;
        }
      }
      if (frame->is_interpreter_frame()) {
        interpreterState istate =
          ((InterpreterFrame *) frame)->interpreter_state();
        intptr_t *monitor_base = (intptr_t *) istate->monitor_base();
        if (addr >= (intptr_t *) istate) {
          field = istate->name_of_field_at_address((address) addr);
          if (field) {
            if (!strcmp(field, "_method")) {
              value = istate->method()->name_and_sig_as_C_string(_buf,_buflen);
              field = "istate->_method";
            }
            else {
              snprintf(_buf, _buflen, "%sistate->%s",
                       field[strlen(field) - 1] == ')' ? "(": "", field);
              field = _buf;
            }
          }
          else if (addr == (intptr_t *) istate) {
            field = "(vtable for istate)";
          }
        }
        else if (addr >= istate->stack_base() && addr < monitor_base) {
          int monitor_size = frame::interpreter_frame_monitor_size();
          int last_index =
            (monitor_base - istate->stack_base()) / monitor_size - 1;
          int index =
            last_index - (addr - istate->stack_base()) / monitor_size;
          intptr_t monitor = (intptr_t) (istate->monitor_base() - 1 - index);
          intptr_t offset = (intptr_t) addr - monitor;
          
          if (offset == BasicObjectLock::obj_offset_in_bytes()) {
            snprintf(_buf, _buflen, "monitor[%d]->_obj", index);
            field = _buf;
          }
          else if (offset ==  BasicObjectLock::lock_offset_in_bytes()) {
            snprintf(_buf, _buflen, "monitor[%d]->_lock", index);
            field = _buf;
          }
        }
        else if (addr < istate->stack_base()) {
          if (istate->method()->is_native()) {
            address hA = istate->method()->signature_handler();
            if (hA != NULL) {
              if (hA != (address) InterpreterRuntime::slow_signature_handler) {
                InterpreterRuntime::SignatureHandler *handler =
                  InterpreterRuntime::SignatureHandler::from_handlerAddr(hA);

                intptr_t *params =
                  istate->stack_base() - handler->argument_count();

                if (addr >= params) {
                  int param = addr - params;
                  const char *desc = "";
                  if (param == 0)
                    desc = " (JNIEnv)";
                  else if (param == 1) {
                    if (istate->method()->is_static())
                      desc = " (mirror)";
                    else
                      desc = " (this)";
                  }
                  snprintf(_buf, _buflen, "parameter[%d]%s", param, desc);
                  field = _buf;
                }
                else {
                  for (int i = 0; i < handler->argument_count(); i++) {
                    if (params[i] == (intptr_t) addr) {
                      snprintf(_buf, _buflen, "unboxed parameter[%d]", i);
                      field = _buf;
                      break;
                    }
                  } 
                }
              }
            }
          }
          else {
            snprintf(_buf, _buflen, "%s[%d]",
                     top_frame ? "stack_word" : "local",
                     istate->stack_base() - addr - 1);
            field = _buf;
          }
        }
      }
      if (frame->is_shark_frame()) {
        if (word == SharkFrame::pc_off) {
          field = "pc";
        }
        else if (word == SharkFrame::unextended_sp_off) {
          field = "unextended_sp";
        }
        else if (word == SharkFrame::method_off) {
          field = "method";
          methodOop method = ((SharkFrame *) frame)->method();
          if (method->is_oop())
            value = method->name_and_sig_as_C_string(_buf, _buflen);
        }
        else {
          SharkFrame *sf = (SharkFrame *) frame;
          intptr_t *monitor_base =
            (intptr_t *) frame - SharkFrame::header_words + 1;
          intptr_t *stack_base =
            sf->unextended_sp() + sf->method()->max_stack();

          if (addr >= stack_base && addr < monitor_base) {
            int monitor_size = frame::interpreter_frame_monitor_size();
            int last_index = (monitor_base - stack_base) / monitor_size - 1;
            int index = last_index - (addr - stack_base) / monitor_size;
            intptr_t monitor =
              (intptr_t) ((BasicObjectLock *) monitor_base - 1 - index);
            intptr_t offset = (intptr_t) addr - monitor;

            if (offset == BasicObjectLock::obj_offset_in_bytes()) {
              snprintf(_buf, _buflen, "monitor[%d]->_obj", index);
              field = _buf;
            }
            else if (offset ==  BasicObjectLock::lock_offset_in_bytes()) {
              snprintf(_buf, _buflen, "monitor[%d]->_lock", index);
              field = _buf;
            }
          }
          else {
            snprintf(_buf, _buflen, "%s[%d]",
                     top_frame ? "stack_word" : "local",
                     stack_base - addr - 1);
            field = _buf;
          }
        }
      }
    }
      
    if (!field) {
      snprintf(_buf, _buflen, "word[%d]", word);
      field = _buf;
    }
    _st->print(" %p: %-21s = ", addr, field);
    if (value)
      _st->print_cr("%s", value);
    else
      _st->print_cr(PTR_FORMAT, *addr);    
  }
};
