/// This file is the analysis driver. It uses DataHazardAnalysis and
/// CDDDAnalysis to identify memory instructions, basic blocks, and whole loops
/// which can benefit from dynamic scheduling.

#include "CommonLLVM.h"
#include "AnalysisReportSchema.h"
#include "DynamicLoopFusionAnalysis.h"

using namespace llvm;


namespace llvm {

using MemoryRequest = DynamicLoopFusionAnalysis::MemoryRequest;

/// Given a basic block, return the name of the kernel that will contain it
/// after our transformations.
std::string getKernelName(BasicBlock *BB, const SmallVector<PEInfo> &peArray) {
  // If the BB is decoupled into a block PE, then return its name.
  for (auto &peInfo : peArray) {
    if (peInfo.peType == BLOCK && peInfo.basicBlock == BB)
      return peInfo.peKernelName;
  }
  // Otherwise, if the BB is part of a loop PE, then return its name.
  for (auto &peInfo : peArray) {
    if (peInfo.peType == LOOP && peInfo.loop->contains(BB))
      return peInfo.peKernelName;
  }
  // Otherwise, the BB will not be decoupled. Return the original kernel name.
  return demangle(std::string(BB->getParent()->getName()));
}

int getCommonLoopDepth(Loop *L1, Loop *L2) {
  if (L1 == L2) 
    return L1->getLoopDepth() - 1;
  else if (L1->getLoopDepth() == 0 || L2->getLoopDepth() == 0)
    return -1;

  if (L1->getLoopDepth() > L2->getLoopDepth()) 
    return getCommonLoopDepth(L1->getParentLoop(), L2);
  else
    return getCommonLoopDepth(L1, L2->getParentLoop());

  return -1;
}

/// Return the direction of the A --depends-on--> B dependency. Return
/// ENUM_DIR::BACK if A comes first in program order, else return FORWARD.
DEP_DIR getDependencyDir(Instruction *A, Instruction *B, LoopInfo &LI) {
  auto LA = LI.getLoopFor(A->getParent());
  auto LB = LI.getLoopFor(B->getParent());

  if (LA == LB) {
    if (A->getParent() == B->getParent()) {
      return getIndexIntoParent(A) < getIndexIntoParent(B) ? BACK : FORWARD;
    }
    
    for (auto BB : LA->blocks()) {
      if (BB == A->getParent()) {
        return BACK;
      } else if (BB == B->getParent()) {
        return FORWARD;
      }
    }
  }

  for (auto L : LI.getLoopsInPreorder()) {
    if (L == LA) {
      return BACK;
    } else if (L == LB) {
      return FORWARD;
    }
  }

  return FORWARD;
}

SmallVector<MemoryDependencyInfo, 4>
getMemoryDependencyInfo(DynamicLoopFusionAnalysis &DLFA, LoopInfo &LI) {
  MapVector<int, SmallVector<MemoryRequest *>> LoadsForMem =
      DLFA.getCluesteredLoadRequests();
  MapVector<int, SmallVector<MemoryRequest *>> StoresForMem =
      DLFA.getCluesteredStoreRequests();

  // init
  MapVector<int, MemoryDependencyInfo> DepInfoForMem;
  for (auto kv : StoresForMem) {
    DepInfoForMem[kv.first] = MemoryDependencyInfo{
        kv.first,
        int(LoadsForMem[kv.first].size()),
        int(StoresForMem[kv.first].size()),
    };
  }

  for (auto &[Id, LdRequests] : LoadsForMem) {
    for (auto &LdReq : LdRequests) {
      auto LdLoop = LI.getLoopFor(LdReq->memOp->getParent());
      DepInfoForMem[Id].maxLoopDepth = std::max(DepInfoForMem[Id].maxLoopDepth, 
                                                LdReq->loopDepth + 1);

      DepInfoForMem[Id].loadLoopDepth.push_back(LdReq->loopDepth);
      DepInfoForMem[Id].loadIsMaxIterNeeded.push_back(LdReq->isMaxIterNeeded);

      SmallVector<bool> StoreInSameLoop;
      SmallVector<int> StoreCommonLoopDepth;
      SmallVector<DEP_DIR> StoreDepDir;
      for (auto &StReq : StoresForMem[Id]) {
        auto StLoop = LI.getLoopFor(StReq->memOp->getParent());
        StoreInSameLoop.push_back(LdLoop == StLoop);
        StoreCommonLoopDepth.push_back(getCommonLoopDepth(LdLoop, StLoop));
        StoreDepDir.push_back(getDependencyDir(LdReq->memOp, StReq->memOp, LI));
      }
      DepInfoForMem[Id].loadStoreInSameLoop.push_back(StoreInSameLoop);
      DepInfoForMem[Id].loadStoreCommonLoopDepth.push_back(StoreCommonLoopDepth);
      DepInfoForMem[Id].loadStoreDepDir.push_back(StoreDepDir);
    }
  }

  for (auto &[Id, StRequests] : StoresForMem) {
    for (auto &StReq : StRequests) {
      auto StReqLoop = LI.getLoopFor(StReq->memOp->getParent());
      DepInfoForMem[Id].maxLoopDepth = std::max(DepInfoForMem[Id].maxLoopDepth, 
                                                StReq->loopDepth + 1);

      DepInfoForMem[Id].storeLoopDepth.push_back(StReq->loopDepth);
      DepInfoForMem[Id].storeIsMaxIterNeeded.push_back(StReq->isMaxIterNeeded);

      SmallVector<bool> StoreInSameLoop;
      SmallVector<int> StoreCommonLoopDepth;
      SmallVector<DEP_DIR> StoreDepDir;
      for (auto &OtherStReq : StoresForMem[Id]) {
        auto OtherStLoop = LI.getLoopFor(OtherStReq->memOp->getParent());
        StoreInSameLoop.push_back(StReqLoop == OtherStLoop);
        StoreCommonLoopDepth.push_back(getCommonLoopDepth(StReqLoop, OtherStLoop));
        StoreDepDir.push_back(getDependencyDir(StReq->memOp, OtherStReq->memOp, LI));
      }
      DepInfoForMem[Id].storeStoreInSameLoop.push_back(StoreInSameLoop);
      DepInfoForMem[Id].storeStoreCommonLoopDepth.push_back(StoreCommonLoopDepth);
      DepInfoForMem[Id].storeStoreDepDir.push_back(StoreDepDir);
    }
  }

  SmallVector<MemoryDependencyInfo, 4> Res;
  for (auto &kv : DepInfoForMem)
    Res.push_back(kv.second);
  
  return Res;
}

struct DynamicLoopFusionAnalysisPrinter
    : PassInfoMixin<DynamicLoopFusionAnalysisPrinter> {

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    if (F.getCallingConv() == CallingConv::SPIR_KERNEL) {
      auto &LI = AM.getResult<LoopAnalysis>(F);
      auto &SE = AM.getResult<ScalarEvolutionAnalysis>(F);
      auto *DLFA = new DynamicLoopFusionAnalysis(LI, SE);

      auto DepInfos = getMemoryDependencyInfo(*DLFA, LI);
      for (auto Info : DepInfos) {
        Info.print(errs());
      }


      // outs() << formatv("{0:2}", json::Value(std::move(reportJson))) << "\n";
    }

    return PreservedAnalyses::all();
  }

  // Without isRequired returning true, this pass will be skipped for functions
  // decorated with the optnone LLVM attribute. Note that clang -O0 decorates
  // all functions with optnone.
  static bool isRequired() { return true; }

  void getAnalysisUsage(AnalysisUsage &AU) const {
    AU.addRequiredID(LoopAnalysis::ID());
    AU.addRequiredID(ScalarEvolutionAnalysis::ID());
    AU.addRequiredID(DominatorTreeAnalysis::ID());
    AU.addRequiredID(PostDominatorTreeAnalysis::ID());
    AU.addRequiredID(DependenceAnalysis::ID());
  }
};

//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getDynamicLoopFusionAnalysisPrinterPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "DynamicLoopFusionAnalysisPrinter",
          LLVM_VERSION_STRING, [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "dynamic-loop-fusion-analysis") {
                    FPM.addPass(DynamicLoopFusionAnalysisPrinter());
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
  return getDynamicLoopFusionAnalysisPrinterPluginInfo();
}

} // end namespace llvm
