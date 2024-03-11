/*
这两行是预处理指令，用于防止头文件SplitBasicBlock.h被重复包含。
如果没有定义宏LLVM_OBFUSCATION_SPLITBASICBLOCK_H，就定义它，确保头文件的内容只被包含一次，避免重复定义的编译错误。
*/
#ifndef CFL_LLVM_PASS1
#define CFL_LLVM_PASS1

#include "llvm/IR/PassManager.h"

namespace llvm {
class CflLLVMPass : public PassInfoMixin<CflLLVMPass> {
public:
  PreservedAnalyses run(Module &M,  ModuleAnalysisManager &AM);
  static bool isRequired() { return true; }
};
} // namespace llvm

#endif /* CFL_LLVM_PASS1 */
