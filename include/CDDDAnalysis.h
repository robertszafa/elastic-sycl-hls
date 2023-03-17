#ifndef CONTROL_DEP_DATA_DEP_ANALYSIS_H
#define CONTROL_DEP_DATA_DEP_ANALYSIS_H

#include "CommonLLVM.h"
#include "CDG.h"
#include "llvm/Analysis/DDG.h"

using namespace llvm;

namespace llvm {

/// Calculates whether a {bottleneckI} instruction, which is a source of an
/// inter-iteration data dependency, is control dependent on a basic block from
/// the same loop. If yes, collect values which are used (dependencies in) and
/// defined (dependencies out) inside the {bottleneckI} basic block.
class ControlDependentDataDependencyAnalysis {
public:
  using BlockList = SetVector<BasicBlock *, SmallVector<BasicBlock *>>;

  explicit ControlDependentDataDependencyAnalysis(PiBlockDDGNode &loopSCC,
                                                  LoopInfo &LI,
                                                  ControlDependenceGraph &CDG) {
    getSCCPaths(loopSCC);
    nodesToInstructions();
    calculateAllPathII();
    calculateBlocksToDecouple(LI, CDG);
    calculateIncomingUses();
    calculateOutgoingDefs();
  }

  ~ControlDependentDataDependencyAnalysis();

  /// Return true if the analysis concluded that there is at least one block
  /// worth decoupling.
  bool hasAnyControlDependency() { return !blocksToDecouple.empty(); }

  /// Return a set of BBs which were deemed worth decoupling.
  BlockList getBlocksToDecouple() { return blocksToDecouple; }

  SmallVector<SmallVector<Instruction *>> getIncomingUses() {
    return incomingUses;
  }

  SmallVector<SmallVector<Instruction *>> getOutgoingDefs() {
    return outgoingDefs;
  }

  void print(raw_ostream &out, int id=0);

private:
  BlockList blocksToDecouple;
  SmallVector<SmallVector<Instruction *>> incomingUses; 
  SmallVector<SmallVector<Instruction *>> outgoingDefs;
  SmallVector<Instruction *> criticalPath;
  int maxII = 0;

  SmallVector<SmallVector<SimpleDDGNode *>> sccPaths;
  SmallVector<SmallVector<Instruction *>> sccInstructionPaths;
  SmallVector<int> sccIIs;

  void getSCCPaths(PiBlockDDGNode &SCC);
  void nodesToInstructions();
  void calculateAllPathII();
  void calculateBlocksToDecouple(LoopInfo &LI, ControlDependenceGraph &CDG);
  void calculateIncomingUses();
  void calculateOutgoingDefs();

  int calculatePathII(SmallVector<Instruction *> &SCC);
};

} // end namespace llvm

#endif
