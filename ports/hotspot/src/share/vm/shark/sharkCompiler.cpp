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

#include "incls/_precompiled.incl"
#include "incls/_sharkCompiler.cpp.incl"
#ifndef PRODUCT
#include <fstream>
#include <ostream>
#include <llvm/Bitcode/ReaderWriter.h>
#endif // !PRODUCT

using namespace llvm;

SharkCompiler::SharkCompiler()
  : AbstractCompiler(),
    _module(name()),
    _builder(&_module),
    _module_provider(&_module),
    _execution_engine(ExecutionEngine::create(&_module_provider))
{
  SharkType::initialize();
  mark_initialized();
}

void SharkCompiler::initialize()
{
  ShouldNotCallThis();
}

void SharkCompiler::compile_method(ciEnv* env, ciMethod* target, int entry_bci)
{
  assert(is_initialized(), "should be");
  assert(entry_bci == InvocationEntryBci, "OSR is not supported");

#ifndef PRODUCT
  static uintx methods_seen = 0;
  methods_seen++;
  if (methods_seen < SharkStartAt || methods_seen > SharkStopAfter) {
    env->record_method_not_compilable("outside SharkStartAt/SharkStopAfter");
    return;
  }
#endif // !PRODUCT

  ResourceMark rm;

  // Do the typeflow analysis
  ciTypeFlow *flow = target->get_flow_analysis();
  if (env->failing())
    return;
#ifndef PRODUCT
  if (methods_seen == SharkPrintTypeflowAfter) {
    flow->print_on(tty);
  }
#endif // PRODUCT

  // Generate the IR
  ciBytecodeStream iter(target);
  SharkFunction function(builder(), flow, &iter);
  if (env->failing())
    return;
#ifndef PRODUCT
  if (methods_seen == SharkDumpModuleAfter) {
    tty->print_cr("%3d   Dumping module to hotspot.bc", methods_seen);
    std::ostream *out = new std::ofstream(
      "hotspot.bc", std::ios::out | std::ios::trunc | std::ios::binary);
    WriteBitcodeToFile(&_module, *out);
    delete out;
  }
#endif // !PRODUCT

  // Compile and install the method
  install_method(env, target, entry_bci, function.function());
}

void SharkCompiler::install_method(ciEnv*    env,
                                   ciMethod* target,
                                   int       entry_bci,
                                   Function* function)
{
  // Pretty much everything in this method is junk to stop
  // ciEnv::register_method() from failing assertions.

  OopRecorder oop_recorder(env->arena());
  env->set_oop_recorder(&oop_recorder);

  DebugInformationRecorder debug_info(&oop_recorder);
  env->set_debug_info(&debug_info);

  OopMapSet oopmaps;
  debug_info.set_oopmaps(&oopmaps);

  Dependencies deps(env);  
  env->set_dependencies(&deps);

  CodeOffsets offsets;
  offsets.set_value(CodeOffsets::Deopt, 0);
  offsets.set_value(CodeOffsets::Exceptions, 0);
  offsets.set_value(CodeOffsets::Verified_Entry,
                    target->is_static() ? 0 : wordSize);

  assert(CodeEntryAlignment > (int)(sizeof(intptr_t) * 2), "buffer too small");
  unsigned char buf[CodeEntryAlignment * 2];
  intptr_t *data =
    (intptr_t *) align_size_up((intptr_t) buf, CodeEntryAlignment);
  CodeBuffer cb((address) data, CodeEntryAlignment);
  cb.initialize_oop_recorder(&oop_recorder);
  *(data++) = (intptr_t) execution_engine()->getPointerToFunction(function);
  *(data++) = (intptr_t) function;
  cb.set_code_end((address) data);

  ExceptionHandlerTable handler_table;
  ImplicitExceptionTable inc_table;
  
  env->register_method(target,
                       entry_bci,
                       &offsets,
                       0,
                       &cb,
                       0,
                       &oopmaps,
                       &handler_table,
                       &inc_table,
                       this,
                       env->comp_level(),
                       false,
                       false);
}
