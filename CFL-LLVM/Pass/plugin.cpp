#include "cfl-llvm-pass1.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

using namespace llvm;
void add_CFL_LLVM_PASS(ModulePassManager &MPM, llvm::PassBuilder::OptimizationLevel Level) {
    if (Level == llvm::PassBuilder::OptimizationLevel::O0 || Level == llvm::PassBuilder::OptimizationLevel::O1 || Level == llvm::PassBuilder::OptimizationLevel::O2 || Level == llvm::PassBuilder::OptimizationLevel::O3) {
        MPM.addPass(CflLLVMPass());
    }
}

/*
LLVM 的 Pipeline 是代码按特定顺序组织的一系列的优化与转换流程
即针对不同的优化等级 采取不同的 Pass 流来处理 IR 文件
*/
llvm::PassPluginLibraryInfo getCFLPluginInfo() {
  return {
      LLVM_PLUGIN_API_VERSION, "CFL-LLVM-PASS", LLVM_VERSION_STRING,
      [](PassBuilder &PB) {
  /// This extension point allows adding optimization once at the start of the
  /// pipeline
        PB.registerPipelineStartEPCallback([](llvm::ModulePassManager &MPM,
                                              llvm::PassBuilder::OptimizationLevel Level) {
          MPM.addPass(CflLLVMPass());

        });

      }};
}
/*
定义一个 LLVM Pass 插件的入口点函数。这个函数会在编译时被链接到 LLVM 工具中，除非通过宏 LLVM_OBFUSCATION_LINK_INTO_TOOLS 明确指定不链接
*/
#ifndef LLVM_OBFUSCATION_LINK_INTO_TOOLS
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getCFLPluginInfo();
}
#endif
