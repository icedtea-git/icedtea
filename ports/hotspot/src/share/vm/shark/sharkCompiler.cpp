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

using namespace llvm;

SharkCompiler::SharkCompiler()
  : AbstractCompiler(),
    _builder()
{
  SharkType::initialize();
  SharkRuntime::initialize(builder());
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

  ResourceMark rm;
  const char *name = methodname(target);

#ifndef PRODUCT
  // Skip methods if requested
  static uintx methods_seen = 0;
  methods_seen++;
  if (methods_seen < SharkStartAt) {
    env->record_method_not_compilable("methods_seen < SharkStartAt");
    return;
  }
  else if (methods_seen > SharkStopAfter) {
    while (true)
      sleep(1);
  }
#endif // !PRODUCT

  if (SharkOnlyCompile != NULL) {
    if (strcmp(SharkOnlyCompile, name)) {
      env->record_method_not_compilable("does not match SharkOnlyCompile");
      return;
    }
  }

  // Do the typeflow analysis
  ciTypeFlow *flow = target->get_flow_analysis();
  if (env->failing())
    return;
  if (SharkPrintTypeflowOf != NULL) {
    if (!strcmp(SharkPrintTypeflowOf, name))
      flow->print_on(tty);
  }

  // Create the recorders
  Arena arena;
  env->set_oop_recorder(new OopRecorder(&arena));
  OopMapSet oopmaps;
  env->set_debug_info(new DebugInformationRecorder(env->oop_recorder()));
  env->debug_info()->set_oopmaps(&oopmaps);
  env->set_dependencies(new Dependencies(env));

  // Create the CodeBuffer and MacroAssembler
  BufferBlob *bb = BufferBlob::create("shark_temp", 256 * K);
  CodeBuffer cb(bb->instructions_begin(), bb->instructions_size());
  cb.initialize_oop_recorder(env->oop_recorder());
  MacroAssembler *masm = new MacroAssembler(&cb);

  // Compile the method into the CodeBuffer
  ciBytecodeStream iter(target);
  SharkFunction function(builder(), name, flow, &iter, masm);

  // Install the method into the VM
  CodeOffsets offsets;
  offsets.set_value(CodeOffsets::Deopt, 0);
  offsets.set_value(CodeOffsets::Exceptions, 0);
  offsets.set_value(CodeOffsets::Verified_Entry,
                    target->is_static() ? 0 : wordSize);

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

  // Free the BufferBlob
  BufferBlob::free(bb);
}

const char* SharkCompiler::methodname(const ciMethod* target)
{
  const char *klassname = target->holder()->name()->as_utf8();
  const char *methodname = target->name()->as_utf8();

  char *buf = NEW_RESOURCE_ARRAY(
    char, strlen(klassname) + 2 + strlen(methodname) + 1);

  char *dst = buf;
  for (const char *c = klassname; *c; c++) {
    if (*c == '/')
      *(dst++) = '.';
    else
      *(dst++) = *c;
  }
  *(dst++) = ':';
  *(dst++) = ':';
  for (const char *c = methodname; *c; c++) {
    *(dst++) = *c;
  }
  *(dst++) = '\0';
  return buf;
}
