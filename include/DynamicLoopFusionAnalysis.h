#ifndef DYNAMIC_LOOP_FUSION_ANALYSIS_H
#define DYNAMIC_LOOP_FUSION_ANALYSIS_H

#include "CommonLLVM.h"
#include "CDG.h"
#include "llvm/Analysis/DDG.h"

using namespace llvm;

namespace llvm {

class DynamicLoopFusionAnalysis {

public:
  // struct LoopDependency {
  //   Loop *from;
  //   Loop *to;
  //   Instruction *dep;
  // };

  struct MemoryRequest {
    int memoryId;

    Instruction *memOp;
    Instruction *basePtr;

    int loopDepth;
    // Index 0 has outermost loop, index size-1 has innermost loop.
    SmallVector<Loop *> loopNest;
    SmallVector<bool> isMaxIterNeeded;
  };

  struct DecoupledLoopInfo {
    int id;

    // Index 0 has outermost loop, index size-1 has innermost loop.
    SmallVector<Loop *> loops;
    
    SmallVector<MemoryRequest> loads;
    SmallVector<MemoryRequest> stores;
    // SmallVector<LoopDependency> inputDeps;
    // SmallVector<LoopDependency> outputDeps;

    Loop *inner() const { return loops[loops.size() - 1]; }
    Loop *outer() const { return loops[0]; }
  };

  enum DepDir {
    BACK, // First load, then store in program order.
    FORWARD, // First store, then load in program order.
  };
  struct MemoryDependencyInfo {
    int id;

    int numLoads;
    int numStores;
    int maxLoopDepth;

    std::string cType = "";

    SmallVector<int> loadLoopDepth;
    SmallVector<SmallVector<bool>> loadIsMaxIterNeeded;
    SmallVector<SmallVector<bool>> loadStoreInSameLoop;
    SmallVector<SmallVector<int>> loadStoreCommonLoopDepth;
    SmallVector<SmallVector<DepDir>> loadStoreDepDir;

    SmallVector<int> storeLoopDepth;
    SmallVector<SmallVector<bool>> storeIsMaxIterNeeded;
    SmallVector<SmallVector<bool>> storeStoreInSameLoop;
    SmallVector<SmallVector<int>> storeStoreCommonLoopDepth;
    SmallVector<SmallVector<DepDir>> storeStoreDepDir;

    MapVector<Instruction *, int> loadReqIds;
    MapVector<Instruction *, int> storeReqIds;
  };

  explicit DynamicLoopFusionAnalysis(LoopInfo &LI, ScalarEvolution &SE) {
    collectLoopsToDecouple(LI);
    collectMemoriesToProtect(LI);
    collectAGUs(LI);
    checkIsMaxIterNeeded(LI, SE);
    collectMemoryDepInfo(LI);

    for (auto decoupleInfo : loopsToDecouple) {
      errs() << "\nDecoupling loop span [" 
             << decoupleInfo.inner()->getHeader()->getNameOrAsOperand()  << " -- "
             << decoupleInfo.outer()->getHeader()->getNameOrAsOperand() << "]\n";
      
      errs() << "Loads:\n";
      for (auto Ld : decoupleInfo.loads) {
        errs() << "\n";
        Ld.memOp->print(errs());
        errs() << "\n";
        errs() << "  memId = " << Ld.memoryId << "\n";
        errs() << "  isMaxIterNeeded: [";
        for (auto isNeeded : Ld.isMaxIterNeeded) {
          errs() << isNeeded << ", ";
        }
        errs() << "]\n";
      }
    }

  }

  ~DynamicLoopFusionAnalysis();

  auto getLoopsToDecouple() { return loopsToDecouple; }
  auto getAgusToDecouple() { return agusToDecouple; }
  auto getMemoriesToProtect() { return basePtrsToProtect; }
  auto getMemoryDependencyInfo() { return memDepInfo; }

private:
  // Loops to decouple. All loops between parent and child loop get decoupled.
  SmallVector<DecoupledLoopInfo, 4> loopsToDecouple;
  // Not every decoupled loop needs an associated AGU.
  SmallVector<DecoupledLoopInfo, 4> agusToDecouple;
  MapVector<int, MemoryDependencyInfo> memDepInfo;
  SetVector<Instruction *> basePtrsToProtect;

  void collectLoopsToDecouple(LoopInfo &LI);
  void collectMemoriesToProtect(LoopInfo &LI);
  void collectAGUs(LoopInfo &LI);
  void checkIsMaxIterNeeded(LoopInfo &LI, ScalarEvolution &SE);
  void collectMemoryDepInfo(LoopInfo &LI);
  // void collectLoopIO(LoopInfo &LI, ControlDependenceGraph &CDG);
};

} // end namespace llvm

#endif
