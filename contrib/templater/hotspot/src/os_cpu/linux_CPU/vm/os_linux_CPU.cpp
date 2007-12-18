/*
 * Copyright 2003-2007 Sun Microsystems, Inc.  All Rights Reserved.
 * Copyright 2007 Red Hat, Inc.
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

// do not include precompiled header file
#include "incls/_os_linux_@@cpu@@.cpp.incl"

address os::current_stack_pointer()
{
#ifdef PPC
  register address *r1 __asm__ ("r1");
  return *r1;
#else
  Unimplemented();
#endif // PPC
}

frame os::get_sender_for_C_frame(frame* fr)
{
#ifdef PPC
  return frame(fr->sender_sp());
#else
  Unimplemented();
#endif // PPC
}

frame os::current_frame()
{
  frame myframe((intptr_t*) os::current_stack_pointer());
  return os::get_sender_for_C_frame(&myframe);
}

char* os::non_memory_address_word()
{
  // Must never look like an address returned by reserve_memory,
  // even in its subfields (as defined by the CPU immediate fields,
  // if the CPU splits constants across multiple instructions).
#ifdef PPC
  return (char*) -1;
#else
  Unimplemented();
#endif // PPC
}

void os::initialize_thread()
{
  // Nothing to do.
}

address os::Linux::ucontext_get_pc(ucontext_t* uc)
{
  Unimplemented();
}

ExtendedPC os::fetch_frame_from_context(void* ucVoid,
                                        intptr_t** ret_sp,
                                        intptr_t** ret_fp) {
  Unimplemented();
}

frame os::fetch_frame_from_context(void* ucVoid)
{
  Unimplemented();
}

julong os::allocatable_physical_memory(julong size)
{
  Unimplemented();
}

extern "C" int 
JVM_handle_linux_signal(int sig,
                        siginfo_t* info,
                        void* ucVoid,
                        int abort_if_unrecognized)
{
  ucontext_t* uc = (ucontext_t*) ucVoid;

  Thread* t = ThreadLocalStorage::get_thread_slow();

  SignalHandlerMark shm(t);

  // Note: it's not uncommon that JNI code uses signal/sigset to
  // install then restore certain signal handler (e.g. to temporarily
  // block SIGPIPE, or have a SIGILL handler when detecting CPU
  // type). When that happens, JVM_handle_linux_signal() might be
  // invoked with junk info/ucVoid. To avoid unnecessary crash when
  // libjsig is not preloaded, try handle signals that do not require
  // siginfo/ucontext first.

  if (sig == SIGPIPE || sig == SIGXFSZ) {
    // allow chained handler to go first
    if (os::Linux::chained_handler(sig, info, ucVoid)) {
      return true;
    } else {
      if (PrintMiscellaneous && (WizardMode || Verbose)) {
        char buf[64];
        warning("Ignoring %s - see bugs 4229104 or 646499219", 
                os::exception_name(sig, buf, sizeof(buf)));
      }
      return true;
    }
  }
  
  JavaThread* thread = NULL;
  VMThread* vmthread = NULL;
  if (os::Linux::signal_handlers_are_installed) {
    if (t != NULL ){
      if(t->is_Java_thread()) {
        thread = (JavaThread*)t;
      }
      else if(t->is_VM_thread()){
        vmthread = (VMThread *)t;
      }
    }
  }

  if (info != NULL && thread != NULL) {
    // Check to see if we caught the safepoint code in the process
    // of write protecting the memory serialization page.  It write
    // enables the page immediately after protecting it so we can
    // just return to retry the write.
    if (sig == SIGSEGV &&
        os::is_memory_serialize_page(thread, (address) info->si_addr)) {
      // Block current thread until permission is restored.
      os::block_on_serialize_page_trap();
      return true;
    }
  }

  const char *fmt = "caught unhandled signal %d";
  char buf[64];

  sprintf(buf, fmt, sig);
  fatal(buf);
}

void os::Linux::init_thread_fpu_state(void)
{
  // Nothing to do
}

int os::Linux::get_fpu_control_word()
{
  Unimplemented();
}

void os::Linux::set_fpu_control_word(int fpu)
{
  Unimplemented();
}

///////////////////////////////////////////////////////////////////////////////
// thread stack

size_t os::Linux::min_stack_allowed = 64 * K;

bool os::Linux::supports_variable_stack_size()
{
  return true;
}

size_t os::Linux::default_stack_size(os::ThreadType thr_type)
{
  // default stack size (compiler thread needs larger stack)
#ifdef _LP64
  size_t s = (thr_type == os::compiler_thread ? 4 * M : 1 * M);
#else
  size_t s = (thr_type == os::compiler_thread ? 2 * M : 512 * K);
#endif // _LP64
  return s;
}

size_t os::Linux::default_guard_size(os::ThreadType thr_type)
{
  // Creating guard page is very expensive. Java thread has HotSpot
  // guard page, only enable glibc guard page for non-Java threads.
  return (thr_type == java_thread ? 0 : page_size());
}

// Java thread:
//
//   Low memory addresses
//    +------------------------+
//    |                        |\  JavaThread created by VM does not have glibc
//    |    glibc guard page    | - guard, attached Java thread usually has
//    |                        |/  1 page glibc guard.
// P1 +------------------------+ Thread::stack_base() - Thread::stack_size()
//    |                        |\
//    |  HotSpot Guard Pages   | - red and yellow pages
//    |                        |/
//    +------------------------+ JavaThread::stack_yellow_zone_base()
//    |                        |\
//    |      Normal Stack      | -
//    |                        |/
// P2 +------------------------+ Thread::stack_base()
//
// Non-Java thread:
//
//   Low memory addresses
//    +------------------------+
//    |                        |\
//    |  glibc guard page      | - usually 1 page
//    |                        |/
// P1 +------------------------+ Thread::stack_base() - Thread::stack_size()
//    |                        |\
//    |      Normal Stack      | -
//    |                        |/
// P2 +------------------------+ Thread::stack_base()
//
// ** P1 (aka bottom) and size ( P2 = P1 - size) are the address and stack size returned from
//    pthread_attr_getstack()

static void current_stack_region(address* bottom, size_t* size) {
  if (os::Linux::is_initial_thread()) {
     // initial thread needs special handling because pthread_getattr_np()
     // may return bogus value.
     *bottom = os::Linux::initial_thread_stack_bottom();
     *size = os::Linux::initial_thread_stack_size();
  } else {
     pthread_attr_t attr;

     int rslt = pthread_getattr_np(pthread_self(), &attr);

     // JVM needs to know exact stack location, abort if it fails
     if (rslt != 0) {
       if (rslt == ENOMEM) {
         vm_exit_out_of_memory(0, "pthread_getattr_np");
       } else {
         fatal1("pthread_getattr_np failed with errno = %d", rslt);
       }
     }

     if (pthread_attr_getstack(&attr, (void **)bottom, size) != 0 ) {
         fatal("Can not locate current stack attributes!");
     }

     pthread_attr_destroy(&attr);

  }
  assert(os::current_stack_pointer() >= *bottom &&
         os::current_stack_pointer() < *bottom + *size, "just checking");
}

address os::current_stack_base()
{
  address bottom;
  size_t size;
  current_stack_region(&bottom, &size);
  return bottom + size;
}

size_t os::current_stack_size()
{
  // stack size includes normal stack and HotSpot guard pages
  address bottom;
  size_t size;
  current_stack_region(&bottom, &size);
  return size;
}

/////////////////////////////////////////////////////////////////////////////
// helper functions for fatal error handler

void os::print_context(outputStream* st, void* context)
{
  Unimplemented();
}

/////////////////////////////////////////////////////////////////////////////
// Stubs for things that should be in linux_@@cpu@@.s

extern "C" {

#ifdef PPC
#ifdef PPC64
  void _Copy_conjoint_jints_atomic(jint* from, jint* to, size_t count)
  {
    Unimplemented();
  }
#endif // PPC64

#ifdef PPC32
  void _Copy_conjoint_jlongs_atomic(jlong* from, jlong* to, size_t count)
  {
#ifdef XXX_EVIL_EVIL_EVIL
    _Copy_conjoint_jints_atomic((jint *) from, (jint *) to, count * 2);
#else
    Unimplemented();
#endif // XXX_EVIL_EVIL_EVIL
  }
#endif // PPC32
#endif // PPC

  void _Copy_conjoint_jshorts_atomic(jshort* from, jshort* to, size_t count)
  {
    Unimplemented();
  }
  void _Copy_arrayof_conjoint_bytes(HeapWord* from, HeapWord* to, size_t count)
  {
    Unimplemented();
  }
  void _Copy_arrayof_conjoint_jshorts(HeapWord* from, HeapWord* to,
				      size_t count) {
    Unimplemented();
  }
  void _Copy_arrayof_conjoint_jints(HeapWord* from, HeapWord* to, size_t count)
  {
    Unimplemented();
  }
  void _Copy_arrayof_conjoint_jlongs(HeapWord* from, HeapWord* to,
				     size_t count) {
    Unimplemented();
  }

  // Implementations of atomic operations not supported by processors.
  //  -- http://gcc.gnu.org/onlinedocs/gcc-4.2.1/gcc/Atomic-Builtins.html
#ifndef _LP64
  long long unsigned int __sync_val_compare_and_swap_8(
    volatile void *ptr,
    long long unsigned int oldval,
    long long unsigned int newval)
  {
    Unimplemented();
  }
#endif // !_LP64
}
