#ifndef RAW_DATA_HAZARD_ANALYSIS_H
#define RAW_DATA_HAZARD_ANALYSIS_H

#include "CommonLLVM.h"
#include "CDG.h"
#include "llvm/Analysis/DDG.h"

using namespace llvm;

namespace llvm {

/// Data Hazard Analysis driver class.
/// Collect all memory instruction, that have a RAW inter-iteration 
/// memory dependence whose scalar evolution is not computable.
class DataHazardAnalysis  {
  // A CFG edge.
  using PoisonLocation = std::pair<BasicBlock *, BasicBlock *>;

public:
  explicit DataHazardAnalysis(Function &F, LoopInfo &LI, ScalarEvolution &SE,
                              DominatorTree &DT, PostDominatorTree &PDT,
                              DataDependenceGraph &DDG,
                              ControlDependenceGraph &CDG);
  ~DataHazardAnalysis();

  /// Return all memory instructions for each base address in the function
  /// which has at least one data hazard.
  SmallVector<SmallVector<Instruction *>> getHazardInstructions() {
    return hazardInstrs;
  }
  SmallVector<Instruction *> getBaseAddresses() { return baseAddresses; }
  SmallVector<bool> getDecoupligDecisions() { return decouplingDecisions; }
  SmallVector<bool> getIsOnChip() { return isOnChip; }
  SmallVector<int> getMemorySizes() { return memorySizes; }
  SmallVector<int> getStoreQueueSizes() { return storeQueueSizes; }
  SmallVector<bool> getSpeculationDecisions() { return speculationDecisions; }

  MapVector<BasicBlock *, SmallVector<Instruction *>> getSpeculationStack() {
    return speculationStack;
  }

  MapVector<Instruction *, SmallVector<PoisonLocation>> getPoisonLocations() {
    return poisonLocations;
  }

  SmallVector<PoisonLocation> getPoisonLocations(Instruction *I) {
    return poisonLocations[I];
  }

  /// Return the block at which {I} is speculated, or nullptr if not speculated.
  BasicBlock *getSpeculationBlock(Instruction *I) {
    for (auto [specBB, allocStack] : speculationStack) {
      if (llvm::is_contained(allocStack, I))
        return specBB;
    }

    return nullptr;
  }

  /// Collect all loads that will be routed through an LSQ.
  SmallVector<Instruction *> getAllLoads();

  /// Return the src of a data dependency that causes a loss-of-decoupling
  /// for the memOp
  Instruction* getLodDataDependencySrc(Instruction *I);
  
  /// Return the src of a control dependency that causes a loss-of-decoupling
  /// for the memOp
  BasicBlock *getLodControlDependencySrc(BasicBlock *BB, LoopInfo &LI,
                                         ControlDependenceGraph &CDG);
  BasicBlock *getLodControlDependencySrc(Instruction *memOp, LoopInfo &LI,
                                         ControlDependenceGraph &CDG) {
    return getLodControlDependencySrc(memOp->getParent(), LI, CDG);
  }

private:
  /// For testing, to turn off decoupling and speculation.
  bool decouplingEnabled;

  /// Memory load and store instructions for each base address that is part
  /// of a data hazard.
  SmallVector<SmallVector<Instruction *>> hazardInstrs;
  
  SmallVector<Instruction *> baseAddresses;

  /// For each base address, a bool indicating if the address generation
  /// instructions can be decoupled from F.
  SmallVector<bool> decouplingDecisions;

  /// For each base address, a bool indicating if any of the LSQ address
  /// allocations need to be speculated in order to achieve decoupling.
  SmallVector<bool> speculationDecisions;

  /// For each memory operation in each base address, a pointer to a basic block
  /// that is the source of a special control-dependency (nullptr if doesn't
  /// exist).
  SmallVector<SmallVector<BasicBlock *>> specialCtrlDepSrsBlocks;

  MapVector<BasicBlock *, SmallVector<Instruction *>> speculationStack;

  // Any needed poison read/writes for each LSQ.
  // SmallVector<SmallVector<PoisonInfo>> poisonInfo;
  MapVector<Instruction *, SmallVector<PoisonLocation>> poisonLocations;

  /// For each base address, a bool indicating if the target memory is on-chip.
  SmallVector<bool> isOnChip;

  /// For each base address, a size of the memory if it is on-chip, 0 otherwise.
  SmallVector<int> memorySizes;

  /// For each base address, an ideal size for the store alloc queue in an LSQ.
  SmallVector<int> storeQueueSizes;

  void calculateDecoupling(ControlDependenceGraph &CDG, LoopInfo &LI);
  
  void calculatePoisonBlocks(DominatorTree &DT, LoopInfo &LI);
};

} // end namespace llvm

#endif
