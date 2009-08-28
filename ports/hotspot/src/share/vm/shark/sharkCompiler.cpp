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
#if SHARK_LLVM_VERSION >= 26
  // Make LLVM safe for multithreading.  We only make LLVM calls from
  // the compiler thread, but if LLVM leaves stubs to be rewritten on
  // execution then it's possible for Java threads to be making LLVM
  // calls at the same time we are.
  if (!llvm_start_multithreaded()) 
    warning("llvm_start_multithreaded() failed");
#endif

  // Create a module to build our functions into
#if SHARK_LLVM_VERSION >= 26
  // LLVM 2.6 and later requires passing a LLVMContext during module
  // creation. The LLVM API getGlobalContext() returns a LLVMContext that
  // can be used safely as long as the shark compiler stays single threaded
  // and only uses one module.
  _module = new Module("shark", getGlobalContext());
#else
  _module = new Module("shark");
#endif

#if SHARK_LLVM_VERSION >= 26
  // If we have a native target, initialize it to ensure it is linked in and
  // usable by the JIT.
  InitializeNativeTarget();
#endif
  
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
  mark_initialized();
}

void SharkCompiler::initialize()
{
  ShouldNotCallThis();
}

void SharkCompiler::compile_method(ciEnv* env, ciMethod* target, int entry_bci)
{
  assert(is_initialized(), "should be");

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
    if (fnmatch(SharkOnlyCompile, name, 0)) {
      env->record_method_not_compilable("does not match SharkOnlyCompile");
      return;
    }
  }

  // Do the typeflow analysis
  ciTypeFlow *flow;
  if (entry_bci == InvocationEntryBci)
    flow = target->get_flow_analysis();
  else
    flow = target->get_osr_flow_analysis(entry_bci);
  if (env->failing())
    return;
  if (SharkPrintTypeflowOf != NULL) {
    if (!fnmatch(SharkPrintTypeflowOf, name, 0))
      flow->print_on(tty);
  }

  // Create the recorders
  Arena arena;
  env->set_oop_recorder(new OopRecorder(&arena));
  OopMapSet oopmaps;
  env->set_debug_info(new DebugInformationRecorder(env->oop_recorder()));
  env->debug_info()->set_oopmaps(&oopmaps);
  env->set_dependencies(new Dependencies(env));

  // Create the code buffer and builder
  SharkCodeBuffer cb(env->oop_recorder());
  SharkBuilder builder(module(), &cb);

  // Emit the entry point
  SharkEntry *entry = (SharkEntry *) cb.malloc(sizeof(SharkEntry));
  
  // Build the LLVM IR for the method
  Function *function = SharkFunction::build(this, env, &builder, flow, name);
  if (SharkPrintBitcodeOf != NULL) {
    if (!fnmatch(SharkPrintBitcodeOf, name, 0))
      function->dump();
  }
  entry->set_function(function);

  // Compile to native code
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
  memory_manager()->set_entry_for_function(function, entry);
  module()->getFunctionList().push_back(function);
  entry->set_entry_point(
    (ZeroEntry::method_entry_t)
      execution_engine()->getPointerToFunction(function));
  address code_start = entry->code_start();
  address code_limit = entry->code_limit();

  // Register generated code for profiling, etc
  if (JvmtiExport::should_post_dynamic_code_generated())
    JvmtiExport::post_dynamic_code_generated(name, code_start, code_limit);
  
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

  // Print statistics, if requested
  if (SharkTraceInstalls) {
    tty->print_cr(
      " [%p-%p): %s (%d bytes code)",
      code_start, code_limit, name, code_limit - code_start);
  }
}

void SharkCompiler::free_compiled_method(address code)
{
  Function *function = ((SharkEntry *) code)->function();
  execution_engine()->freeMachineCodeForFunction(function);
  function->eraseFromParent();
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
