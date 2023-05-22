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
  explicit ControlDependentDataDependencyAnalysis(
      LoopInfo &LI, DataDependenceGraph &DDG, ControlDependenceGraph &CDG) {
    // Go over all SCCs in the DDG.
    for (auto nodeDDG : DDG) {
      if (auto loopSCC = dyn_cast<PiBlockDDGNode>(nodeDDG)) {
        reset();

        getSCCPaths(*loopSCC);
        DDGNodesToInstructions();
        calculateAllPathII();
        calculateRegionsToDecouple(LI, CDG);
        calculateInOutDependencies();
      }
    }
  }

  ~ControlDependentDataDependencyAnalysis();

  /// Return true if the analysis concluded that there is at least one block
  /// worth decoupling.
  bool hasAnyControlDependency() { return !blocksToDecouple.empty(); }

  /// Return a set of BBs which were deemed worth decoupling.
  SetVector<BasicBlock *> getBlocksToDecouple() { return blocksToDecouple; }

  SetVector<Instruction *> getInstructionsToDecouple(BasicBlock *BB) {
    return instrToDecoupleInBB[BB];
  }

  SetVector<Instruction *> getInputDependencies(BasicBlock *BB) {
    return inputDependencies[BB];
  }

  SetVector<Instruction *> getOutputDependencies(BasicBlock *BB) {
    return outputDependencies[BB];
  }

private:
  SetVector<BasicBlock *> blocksToDecouple;
  MapVector<BasicBlock *, SetVector<Instruction *>> inputDependencies; 
  MapVector<BasicBlock *, SetVector<Instruction *>> outputDependencies;
  MapVector<BasicBlock *, SetVector<Instruction *>> instrToDecoupleInBB;

  SmallVector<Instruction *> criticalPath;
  SmallVector<SmallVector<SimpleDDGNode *>> sccPaths;
  SmallVector<SmallVector<Instruction *>> allSCCInstructionPaths;
  SmallVector<int> sccIIs;
  int maxII = 0;

  void reset() {
    criticalPath.clear();
    sccPaths.clear();
    allSCCInstructionPaths.clear();
    sccIIs.clear();
    maxII = 0;
  }

  void getSCCPaths(PiBlockDDGNode &SCC);
  void DDGNodesToInstructions();
  void calculateAllPathII();
  void calculateRegionsToDecouple(LoopInfo &LI, ControlDependenceGraph &CDG);
  void calculateInOutDependencies();

  int calculatePathII(SmallVector<Instruction *> &SCC);
};

} // end namespace llvm

#endif
