#define CFL_LLVM_PASS

#include "../include/config.h"
#include "../include/types.h"
#include "../include/debug.h"
#include "./cfl-llvm-pass1.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "llvm/Support/CommandLine.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/Debug.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Pass.h"
using namespace llvm;

/*
定义一个 LLVM Pass 插件的入口点函数。这个函数会在编译时被链接到 LLVM 工具中，除非通过宏 LLVM_OBFUSCATION_LINK_INTO_TOOLS 明确指定不链接
*/
// cl::opt<bool> CFLEnabled("cfl", cl::init(false),
//                          cl::desc("Enable CFL LLVM PASS"));

PreservedAnalyses CflLLVMPass::run(Module &M, ModuleAnalysisManager &AM){
    LLVMContext &C = M.getContext();
    IntegerType *Int8Ty  = IntegerType::getInt8Ty(C);
    IntegerType *Int32Ty = IntegerType::getInt32Ty(C);

    /* Show a banner */
    char be_quiet = 0;
#ifdef MESSAGES_TO_STDOUT
    if (isatty(1) && !getenv("CFL_QUIET")) {

        PT(cCYA "cfl-llvm-pass " cBRI VERSION cRST " by <1539412714@qq.com>\n");

    } else be_quiet = 1;
#else
    if (isatty(2) && !getenv("CFL_QUIET")) {

  PT(cCYA "cfl-llvm-pass " cBRI VERSION cRST " by <1539412714@qq.com>\n");

} else be_quiet = 1;
#endif
    //若正确设置了 CFL_INST_RATIO 则以其为标准 若设置错误则即 若没设置则取 100
    char* inst_ratio_str = getenv("CFL_INST_RATIO");
    unsigned int inst_ratio = 100;
    if (inst_ratio_str) {
        if (sscanf(inst_ratio_str, "%u", &inst_ratio) != 1 || !inst_ratio ||
            inst_ratio > 100)
            FATAL("Bad value of CFL_INST_RATIO (must be between 1 and 100)");

    }

    /* Get globals for the SHM region and the previous location. Note that
      __afl_prev_loc is thread-local. */
    //地址空间编号（在这个例子中是 0），这个编号指定了生成的指针类型应该位于哪个地址空间。
    GlobalVariable *CFLMapPtr =
            new GlobalVariable(M, PointerType::get(Int8Ty, 0), false,
                               GlobalValue::ExternalLinkage, 0, "__cfl_area_ptr");
    /*
    GlobalValue::ExternalLinkage 使得这个变量可以被模块外部代码引用。
    GlobalVariable::GeneralDynamicTLSModel 指定了线程局部存储（TLS）的模型，这里使用的是通用动态模型，适用于大多数情况。
    */
    GlobalVariable *CFLPrevLoc = new GlobalVariable(
            M, Int32Ty, false, GlobalValue::ExternalLinkage, 0, "__cfl_prev_loc",
            0, GlobalVariable::GeneralDynamicTLSModel, 0, false);

    /* Instrument all the things! */

    int inst_blocks = 0;

    for (auto &F : M){
        for (auto &BB : F) {
            //找到基本块（BasicBlock）中的首个安全插入点，即第一个非 PHI（φ）节点的指令
            BasicBlock::iterator IP = BB.getFirstInsertionPt();
            IRBuilder<> IRB(&(*IP));

            if (CFL_R(100) >= inst_ratio) continue;

            /* Make up cur_loc */
            //生成一个随机的位置 cur_loc，这个位置是在 AFL 的覆盖率位图（通常称为 coverage bitmap）中的一个索引
            unsigned int cur_loc = CFL_R(MAP_SIZE);

            ConstantInt *CurLoc = ConstantInt::get(Int32Ty, cur_loc);

            /* Load prev_loc */

            LoadInst *PrevLoc = IRB.CreateLoad(CFLPrevLoc);
            PrevLoc->setMetadata(M.getMDKindID("nosanitize"), MDNode::get(C, None));
            //零拓展 保证是 32 位
            Value *PrevLocCasted = IRB.CreateZExt(PrevLoc, IRB.getInt32Ty());

            /* Load SHM pointer */

            LoadInst *MapPtr = IRB.CreateLoad(CFLMapPtr);
            //setMetadata 设定了一个元数据标记 "nosanitize"，意思是告诉编译器不对这次加载操作进行内存安全检查。
            MapPtr->setMetadata(M.getMDKindID("nosanitize"), MDNode::get(C, None));
            //创建一个指针 指向 MpaPtr 加上 PreLocCasted 和 CurLoc 的异或结果
            Value *MapPtrIdx =
                    IRB.CreateGEP(MapPtr, IRB.CreateXor(PrevLocCasted, CurLoc));

            /* Update bitmap */
            //这里应该是取指针 用指针定位相应位置的值 并在值上 +1 标识已经被覆盖
            LoadInst *Counter = IRB.CreateLoad(MapPtrIdx);
            Counter->setMetadata(M.getMDKindID("nosanitize"), MDNode::get(C, None));
            Value *Incr = IRB.CreateAdd(Counter, ConstantInt::get(Int8Ty, 1));
            IRB.CreateStore(Incr, MapPtrIdx)
                    ->setMetadata(M.getMDKindID("nosanitize"), MDNode::get(C, None));

            /* Set prev_loc to cur_loc >> 1 */
            //将 cur_loc 右移一位后的值存为 prev_loc，为下一个基本块执行时使用。这是一种简单的哈希操作，用于改变路径标识，减少不同路径间的冲突
            StoreInst *Store =
                    IRB.CreateStore(ConstantInt::get(Int32Ty, cur_loc >> 1), CFLPrevLoc);
            Store->setMetadata(M.getMDKindID("nosanitize"), MDNode::get(C, None));

            inst_blocks++;

        }
    }
    /* Say something nice. */
    //输出些插桩的样例
    if (!be_quiet) {

        if (!inst_blocks) WARNF("No instrumentation targets found.");
        else OKF("Instrumented %u locations (%s mode, ratio %u%%).",
                 inst_blocks, getenv("CFL_HARDEN") ? "hardened" :
                              ((getenv("CFL_USE_ASAN") || getenv("CFL_USE_MSAN")) ?
                               "ASAN/MSAN" : "non-hardened"), inst_ratio);

    }
    return PreservedAnalyses::none();

};