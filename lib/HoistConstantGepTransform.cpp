#include "llvm/IR/Dominators.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"

#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

#include "llvm/ADT/SmallVector.h"


using namespace llvm;

namespace llvm {

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
    auto gepOp0 = gep->getOperand(0);
    if (isa<Instruction>(gepOp0) &&
        DT.dominates(dyn_cast<Instruction>(gepOp0)->getParent(), entryBB)) {
      wasChanged = true;
      gep->moveBefore(entryBB->getTerminator());
    }
  }

  return wasChanged;
}

/// If a GEP instruction has all constant operators, then move it to the
/// function entry.
/// 
/// SYCL/OpenCL kernel arguments are represented as a structure of pointers
/// in LLVM IR. So a kernel argument ends up as a GetElementPointer with 
/// contant offsets into that structure. 
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
  return {LLVM_PLUGIN_API_VERSION, "HoistContGep", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "hoist-const-gep") {
                    FPM.addPass(HoistContGep());
                    return true;
                  }
                  return false;
                });
          }};
}

// This is the core interface for pass plugins. It guarantees that 'opt' will
// find the pass.
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getHoistContGepPluginInfo();
}

} // end namespace llvm