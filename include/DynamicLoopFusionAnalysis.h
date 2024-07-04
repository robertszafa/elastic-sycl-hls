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

  enum MemoryRequestType  { protectedMem, simpleMem };
  enum DecoupledLoopType  { compute, agu, memory };

  struct MemoryRequest {
    int memoryId;
    int loopId;
    int reqId;

    MemoryRequestType type;

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

    /// Generate a string pipe name associated with this memory request.
    std::string getPipeName(DecoupledLoopType loopType = compute);
    /// Collect pipeCall instrs in F associated with this pipeName.
    void collectPipeCalls(Function &F, DecoupledLoopType loopType);
    /// Collect any stores to the fields in the request.
    void collectStoresToRequestStruct(Function &F, DecoupledLoopType loopType);
  };

  struct DecoupledLoopInfo {
    int id;
    DecoupledLoopType type;

    // Index 0 has outermost loop, index size-1 has innermost loop.
    SmallVector<Loop *> loops;

    SmallVector<MemoryRequest, 4> loads;
    SmallVector<MemoryRequest, 4> stores;
    // SmallVector<LoopDependency> inputDeps;
    // SmallVector<LoopDependency> outputDeps;

    std::string kernelName;

    Loop *inner() const { return loops.back(); }
    Loop *outer() const { return loops.front(); }
  };

  enum DepDir {
    BACK,    // First load, then store in program order.
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
    SmallVector<SmallVector<bool>> loadStoreInSameThread;
    SmallVector<SmallVector<int>> loadStoreCommonLoopDepth;
    SmallVector<SmallVector<DepDir>> loadStoreDepDir;

    SmallVector<int> storeLoopDepth;
    SmallVector<SmallVector<bool>> storeIsMaxIterNeeded;
    SmallVector<SmallVector<bool>> storeStoreInSameLoop;
    SmallVector<SmallVector<int>> storeStoreCommonLoopDepth;
    SmallVector<SmallVector<DepDir>> storeStoreDepDir;
  };

  explicit DynamicLoopFusionAnalysis(LoopInfo &LI, ScalarEvolution &SE,
                                     const std::string &fName) {
    this->fName = fName;

    collectComputeLoops(LI);
    collectBasePointersToProtect(LI);
    collectProtectedMemoryRequests(LI);
    checkIsMaxIterNeeded(LI, SE);
    collectProtectedMemoryInfo(LI);
    collectAguLoops();
    collectSimpleMemoryLoops(LI);
  }

  ~DynamicLoopFusionAnalysis();

  auto getAguLoops() { return AguLoops; }
  auto getComputeLoops() { return ComputeLoops; }
  auto getSimpleMemoryLoops() { return SimpleMemoryLoops; }
  auto getBasePointersToProtect() { return BasePtrsToProtect; }
  auto getProtectedMemoryInfo() { return ProtectedMemoryInfo; }

  auto getDecoupledLoopsWithType(DecoupledLoopType loopType) {
    if (loopType == DecoupledLoopType::agu)
      return getAguLoops();
    else if (loopType == DecoupledLoopType::memory)
      return getSimpleMemoryLoops();
    else
      return getComputeLoops();
  }

private:
  /// The function name of the analyzed kernel. This is used as the basis to
  /// generate kernel names of the decoupled loops.
  std::string fName;

  // Loops to decouple. All loops between parent and child loop get decoupled.
  SmallVector<DecoupledLoopInfo, 4> ComputeLoops;
  // Not every decoupled loop needs an associated AGU.
  SmallVector<DecoupledLoopInfo, 4> AguLoops;
  MapVector<int, MemoryDependencyInfo> ProtectedMemoryInfo;
  SetVector<Instruction *> BasePtrsToProtect;

  SmallVector<DecoupledLoopInfo, 4> SimpleMemoryLoops;

  void collectComputeLoops(LoopInfo &LI);
  void collectBasePointersToProtect(LoopInfo &LI);
  void collectProtectedMemoryRequests(LoopInfo &LI);
  void checkIsMaxIterNeeded(LoopInfo &LI, ScalarEvolution &SE);
  void collectProtectedMemoryInfo(LoopInfo &LI);
  void collectAguLoops();
  void collectSimpleMemoryLoops(LoopInfo &LI);
  // void collectLoopIO(LoopInfo &LI, ControlDependenceGraph &CDG);
};

} // end namespace llvm

#endif
