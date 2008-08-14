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

  const char *method_klass = NULL;
  const char *method_name  = NULL;
  
#ifndef PRODUCT
  method_klass = klassname(target);
  method_name  = methodname(target);

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
    if (!method_matches(SharkOnlyCompile, method_klass, method_name)) {
      env->record_method_not_compilable("does not match SharkOnlyCompile");
      return;
    }
  }

  // Do the typeflow analysis
  ciTypeFlow *flow = target->get_flow_analysis();
  if (env->failing())
    return;
  if (SharkPrintTypeflowOf != NULL) {
    if (method_matches(SharkPrintTypeflowOf, method_klass, method_name))
      flow->print_on(tty);
  }

  // Create the CodeBuffer and OopMapSet
  BufferBlob *bb = BufferBlob::create(
    "shark_temp", sizeof(SharkMethod) + target->code_size());
  CodeBuffer cb(bb->instructions_begin(), bb->instructions_size());
  OopMapSet oopmaps;

  // Compile the method
  ciBytecodeStream iter(target);
  SharkFunction function(builder(), flow, &iter, &cb, &oopmaps);
  if (env->failing())
    return;
  if (SharkPrintBitcodeOf != NULL) {
    if (method_matches(SharkPrintBitcodeOf, method_klass, method_name))
      function.function()->dump();
  }
  if (SharkTraceInstalls) {
    uint32_t *start = NULL;
    uint32_t *limit = NULL;
#ifdef PPC
    start = *(uint32_t **) bb->instructions_begin();
    limit = start;
    while (*limit)
      limit++;
#else
    Unimplemented();
#endif // PPC

    tty->print_cr(
      "Installing method %s::%s at [%p-%p)",
      method_klass, method_name, start, limit);
  }

  // Install the method
  install_method(env, target, entry_bci, &cb, &oopmaps);
}

void SharkCompiler::install_method(ciEnv*      env,
                                   ciMethod*   target,
                                   int         entry_bci,
                                   CodeBuffer* cb,
                                   OopMapSet*  oopmaps)
{
  // Pretty much everything in this method is junk to stop
  // ciEnv::register_method() from failing assertions.

  OopRecorder oop_recorder(env->arena());
  env->set_oop_recorder(&oop_recorder);

  DebugInformationRecorder debug_info(&oop_recorder);
  debug_info.set_oopmaps(oopmaps);
  env->set_debug_info(&debug_info);

  Dependencies deps(env);  
  env->set_dependencies(&deps);

  CodeOffsets offsets;
  offsets.set_value(CodeOffsets::Deopt, 0);
  offsets.set_value(CodeOffsets::Exceptions, 0);
  offsets.set_value(CodeOffsets::Verified_Entry,
                    target->is_static() ? 0 : wordSize);

  cb->initialize_oop_recorder(&oop_recorder);

  ExceptionHandlerTable handler_table;
  ImplicitExceptionTable inc_table;
  
  env->register_method(target,
                       entry_bci,
                       &offsets,
                       0,
                       cb,
                       0,
                       oopmaps,
                       &handler_table,
                       &inc_table,
                       this,
                       env->comp_level(),
                       false,
                       false);
}

#ifndef PRODUCT
const char* SharkCompiler::klassname(const ciMethod* target)
{
  const char *name = target->holder()->name()->as_utf8();
  char *buf = NEW_RESOURCE_ARRAY(char, strlen(name) + 1);
  strcpy(buf, name);
  for (char *c = buf; *c; c++) {
    if (*c == '/')
      *c = '.';
  }
  return buf;
}

const char* SharkCompiler::methodname(const ciMethod* target)
{
  return target->name()->as_utf8();
}

bool SharkCompiler::method_matches(const char* pattern,
                                   const char* klassname,
                                   const char* methodname)
{
  if (pattern[0] == '*') {
    pattern++;
  }
  else {
    int len = strlen(klassname);
    if (strncmp(pattern, klassname, len))
      return false;
    pattern += len;
  }

  if (pattern[0] != ':' && pattern[1] != ':')
    return false;
  pattern += 2;

  if (pattern[0] == '*') {
    pattern++;
  }
  else {
    int len = strlen(methodname);
    if (strncmp(pattern, methodname, len))
      return false;
    pattern += len;
  }

  return pattern[0] == '\0';
}
#endif // !PRODUCT
