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
public:
  // A CFG edge.
  using PoisonLocation = std::pair<BasicBlock *, BasicBlock *>;
  using InstructionSet = SetVector<Instruction *, SmallVector<Instruction *>>;
  
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

  /// Return the block at which {I} is speculated, or nullptr if not speculated.
  BasicBlock *getSpeculationBlock(Instruction *I) {
    for (auto &[specBB, allocStack] : requestMap) {
      if (allocStack.contains(I))
        return specBB;
    }

    return nullptr;
  }

  MapVector<Instruction *, SetVector<PoisonLocation>> getPoisonLocations() {
    return poisonLocations;
  }
  SetVector<PoisonLocation> getPoisonLocations(Instruction *I) {
    return poisonLocations[I];
  }

private:
  /// For testing, to turn off decoupling and speculation.
  bool aguDecouplingOff;

  /// Memory load and store instructions for each base address that is part
  /// of a data hazard.
  SmallVector<SmallVector<Instruction *>> hazardInstrs;
  
  /// Addresses that have hazard instructions.
  SmallVector<Instruction *> baseAddresses;

  /// For each base address, a bool indicating if the address generation
  /// instructions can be decoupled from F.
  SmallVector<bool> decouplingDecisions;

  /// For each base address, a bool indicating if any of the LSQ address
  /// allocations need to be speculated in order to achieve decoupling.
  SmallVector<bool> speculationDecisions;

  /// Map of memory ops to the ctrl dep source block that causes a control
  /// dependency loss of decoupling. If multiple such blocks exists, then the
  /// key is the one that dominates all the rest. (Called "L" in paper).
  MapVector<BasicBlock *, InstructionSet> requestMap;

  // Any needed poison read/writes for each LSQ.
  // SmallVector<SmallVector<PoisonInfo>> poisonInfo;
  MapVector<Instruction *, SetVector<PoisonLocation>> poisonLocations;
  
  /// For each base address, a bool indicating if the target memory is on-chip.
  SmallVector<bool> isOnChip;

  /// For each base address, a size of the memory if it is on-chip, 0 otherwise.
  SmallVector<int> memorySizes;

  /// For each base address, an ideal size for the store alloc queue in an LSQ.
  SmallVector<int> storeQueueSizes;

  /// Collect all loads that will be routed through an LSQ.
  SmallVector<Instruction *> getAllLoads();
  /// Return the src of a data dependency that causes a loss-of-decoupling
  /// for the memOp
  Instruction* getLodDataDependencySrc(Instruction *I);
  /// Return control dependency src blocks that cause a loss-of-decoupling.
  SmallVector<BasicBlock *> getLodCtrlDepSources(BasicBlock *BB, LoopInfo &LI,
                                                 ControlDependenceGraph &CDG);
  SmallVector<BasicBlock *> getLodCtrlDepSources(Instruction *I, LoopInfo &LI,
                                                 ControlDependenceGraph &CDG) {
    return getLodCtrlDepSources(I->getParent(), LI, CDG);
  }

  void calculateDecoupling();
  void calculateSpeculation(ControlDependenceGraph &CDG, LoopInfo &LI);
  void hoistSpeculativeRequests(Function &F, ControlDependenceGraph &CDG,
                                LoopInfo &LI);
  void calculatePoisonBlocks(Function &F, DominatorTree &DT, LoopInfo &LI);

};

} // end namespace llvm

#endif
