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
    for (auto NodeDDG : DDG) {
      if (auto LoopSCC = dyn_cast<PiBlockDDGNode>(NodeDDG)) {
        auto AllSccPaths = getSCCPaths(*LoopSCC);
        auto AllSCCInstructionPaths = DDGNodesToInstructions(AllSccPaths);
        
        // Populate blocksToDecouple, in/outDependencies, instrToDecoupleInBB.
        // Use sets since there could be repetitions across SCCs.
        collectBlocksToDecouple(LI, CDG, AllSCCInstructionPaths);
        collectBlocksInOutDependencies();
      }
    }

    collectLoopsToDecouple(LI, CDG);
  }

  ~ControlDependentDataDependencyAnalysis();

  /// Return a set of BBs marked for decoupling.
  SetVector<BasicBlock *> getBlocksToDecouple() { return blocksToDecouple; }

  /// Given a decoupled BB, return its instructions that should be decoupled.
  SetVector<Instruction *> getInstructionsToDecouple(BasicBlock *BB) {
    return instrToDecoupleInBB[BB];
  }

  /// Given a decoupled BB, return instructions from other BBs that it is using.
  SetVector<Instruction *> getInputDependencies(BasicBlock *BB) {
    return inputDependencies[BB];
  }

  /// Given a decoupled BB, return instructions that it defines and that are
  /// used by other BBs.
  SetVector<Instruction *> getOutputDependencies(BasicBlock *BB) {
    return outputDependencies[BB];
  }

private:
  SetVector<BasicBlock *> blocksToDecouple;
  MapVector<BasicBlock *, SetVector<Instruction *>> inputDependencies; 
  MapVector<BasicBlock *, SetVector<Instruction *>> outputDependencies;
  MapVector<BasicBlock *, SetVector<Instruction *>> instrToDecoupleInBB;

  SmallVector<BasicBlock *> headersOfLoopsToDecouple;

  SmallVector<SmallVector<SimpleDDGNode *>> getSCCPaths(PiBlockDDGNode &SCC);
  SmallVector<SmallVector<Instruction *>> DDGNodesToInstructions(
      SmallVector<SmallVector<SimpleDDGNode *>> &AllSccPaths);

  void collectBlocksToDecouple(
      LoopInfo &LI, ControlDependenceGraph &CDG,
      SmallVector<SmallVector<Instruction *>> &AllSCCInstructionPaths);
  void collectBlocksInOutDependencies();
  void collectLoopsToDecouple(LoopInfo &LI, ControlDependenceGraph &CDG);
};

} // end namespace llvm

#endif
