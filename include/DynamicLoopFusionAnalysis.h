#ifndef DYNAMIC_LOOP_FUSION_ANALYSIS_H
#define DYNAMIC_LOOP_FUSION_ANALYSIS_H

#include "CommonLLVM.h"
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
    int loopId;
    int reqId;

    Instruction *memOp;
    Instruction *basePtr;

    int loopDepth;
    // Index 0 has outermost loop, index size-1 has innermost loop.
    SmallVector<Loop *> loopNest;
    SmallVector<bool> isMaxIterNeeded;

    int numLoadsInMemoryId;
    int numStoresInMemoryId;
    int maxLoopDepthInMemoryId;

    SmallVector<CallInst *> pipeCalls;
    StoreInst *addrReqStore;
    SmallVector<StoreInst *> schedReqStore;
    SmallVector<StoreInst *> isMaxIterReqStore;
    SmallVector<StoreInst *> isPosDepDistReqStore;
  };

  struct DecoupledLoopInfo {
    int id;

    // Index 0 has outermost loop, index size-1 has innermost loop.
    SmallVector<Loop *> loops;
    
    SmallVector<MemoryRequest, 4> loads;
    SmallVector<MemoryRequest, 4> stores;
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
  };

  explicit DynamicLoopFusionAnalysis(LoopInfo &LI, ScalarEvolution &SE) {
    collectLoopsToDecouple(LI);
    collectBasePointersToProtect(LI);
    collectMemoryRequests(LI);
    checkIsMaxIterNeeded(LI, SE);
    collectMemoryDepInfo(LI);

    collectAGUs();
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
  void collectBasePointersToProtect(LoopInfo &LI);
  void collectMemoryRequests(LoopInfo &LI);
  void checkIsMaxIterNeeded(LoopInfo &LI, ScalarEvolution &SE);
  void collectMemoryDepInfo(LoopInfo &LI);
  void collectAGUs();
  // void collectLoopIO(LoopInfo &LI, ControlDependenceGraph &CDG);
};

} // end namespace llvm

#endif
