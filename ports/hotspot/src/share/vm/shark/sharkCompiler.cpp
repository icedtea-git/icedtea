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

#include "incls/_precompiled.incl"
#include "incls/_sharkCompiler.cpp.incl"

#include <fnmatch.h>

using namespace llvm;

SharkCompiler::SharkCompiler()
  : AbstractCompiler()
{
  // Create a module to build our functions into
  _module = new Module("shark");

  // Create the builder to build our functions
  _builder = new SharkBuilder(this);
  
  // Create the JIT
  ModuleProvider *module_provider = new ExistingModuleProvider(module());
  _memory_manager = new SharkMemoryManager();
  _execution_engine = ExecutionEngine::createJIT(
#if SHARK_LLVM_VERSION >= 26
   /*
    * LLVM 26 introduced a more fine-grained control to set the optimization
    * level when creating the LLVM JIT.
    * The optimization level are now specified with a enum instead of a bool.
    * CodeGenOpt::None = bool true; a fast JIT with reduced optimization.
    * CodeGenOpt::Default = bool false; a non-fast JIT with optimization.
    * CodeGenOpt::Aggressive = a new non-fast JIT with best optimization.
    */
    module_provider, NULL, memory_manager(), CodeGenOpt::Default);
#else
    module_provider, NULL, memory_manager(), false);
#endif

  // Initialize Shark components that need it
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
    if (!strcmp(SharkPrintTypeflowOf, name) ||
	!strcmp(SharkPrintTypeflowOf, "*"))
      flow->print_on(tty);
  }

  // Create the recorders
  Arena arena;
  env->set_oop_recorder(new OopRecorder(&arena));
  OopMapSet oopmaps;
  env->set_debug_info(new DebugInformationRecorder(env->oop_recorder()));
  env->debug_info()->set_oopmaps(&oopmaps);
  env->set_dependencies(new Dependencies(env));

  // Create the code buffer and hook it into the builder
  SharkCodeBuffer cb(env->oop_recorder());
  builder()->set_code_buffer(&cb);

  // Compile the method
  ciBytecodeStream iter(target);
  SharkFunction function(this, name, flow, &iter);

  // Unhook the code buffer
  builder()->set_code_buffer(NULL);  

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
                       cb.cb(),
                       0,
                       &oopmaps,
                       &handler_table,
                       &inc_table,
                       this,
                       env->comp_level(),
                       false,
                       false);
}


ZeroEntry::method_entry_t SharkCompiler::compile(const char* name,
                                                 Function*   function)
{
  // Dump the generated code, if requested
#ifndef PRODUCT
#ifdef X86
  if (SharkPrintAsmOf != NULL) {
    std::vector<const char*> args;
    args.push_back(""); // program name
    if (!fnmatch(SharkPrintAsmOf, name, 0))
      args.push_back("-debug-only=x86-emitter");
    else
      args.push_back("-debug-only=none");
    args.push_back(0);  // terminator
    cl::ParseCommandLineOptions(args.size() - 1, (char **) &args[0]);
  }
#endif // X86
#endif // !PRODUCT

  // Compile to native code
  return (ZeroEntry::method_entry_t)
    execution_engine()->getPointerToFunction(function);
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
