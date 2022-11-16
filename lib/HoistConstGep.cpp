#include "StoreqUtils.h"

#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/DependenceAnalysis.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/Transforms/Utils/ScalarEvolutionExpander.h"

#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/InitializePasses.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/Demangle/Demangle.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/JSON.h"
#include "llvm/Support/raw_ostream.h"

#include <algorithm>
#include <cassert>
#include <regex>
#include <string>
#include <utility>

using namespace llvm;

namespace storeq {

/// If a GEP instruction has all constant operators, then move it to the function entry.
bool hoistGepPass(Function &F, FunctionAnalysisManager &AM) {
  auto &DT = AM.getResult<DominatorTreeAnalysis>(F);

  auto entryBB = &F.getEntryBlock();
  
  SmallVector<GetElementPtrInst *> gepsToHoist;
  for (auto &BB : F) {
    for (auto &I : BB) {
      if (auto gep = dyn_cast<GetElementPtrInst>(&I)) {
        if (gep->hasAllConstantIndices() && gep->getParent() != entryBB) 
          gepsToHoist.push_back(gep);
      }
    }
  }
  bool wasChanged = false;

  for (auto gep : gepsToHoist) {
    if (DT.dominates(dyn_cast<Instruction>(gep->getOperand(0))->getParent(), entryBB)) {
      wasChanged = true;
      gep->moveBefore(entryBB->getTerminator());
    }
  }

  return wasChanged;
}

struct HoistContGep : PassInfoMixin<HoistContGep> {
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    bool wasChanged = false;
    if (F.getCallingConv() == CallingConv::SPIR_FUNC) 
      wasChanged = hoistGepPass(F, AM);

    return wasChanged ? PreservedAnalyses::none() : PreservedAnalyses::all();
  }

  // Without isRequired returning true, this pass will be skipped for functions
  // decorated with the optnone LLVM attribute. Note that clang -O0 decorates
  // all functions with optnone.
  static bool isRequired() { return true; }

  void getAnalysisUsage(AnalysisUsage &AU) const {
    AU.addRequiredID(DominatorTreeAnalysis::ID());
  }
};


//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getHoistContGepPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "HoistContGep", LLVM_VERSION_STRING, [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback([](StringRef Name, FunctionPassManager &FPM,
                                                  ArrayRef<PassBuilder::PipelineElement>) {
              if (Name == "hoist-const-gep") {
                FPM.addPass(HoistContGep());
                return true;
              }
              return false;
            });
          }};
}

// This is the core interface for pass plugins. It guarantees that 'opt' will find the pass.
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return getHoistContGepPluginInfo();
}

} // end namespace storeq