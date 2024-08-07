#ifndef CONTROL_DEP_DATA_DEP_ANALYSIS_H
#define CONTROL_DEP_DATA_DEP_ANALYSIS_H

#include "CommonLLVM.h"
#include "CDG.h"
#include "llvm/Analysis/DDG.h"

using namespace llvm;

namespace llvm {

/// Collects basic blocks and loops that can benefit from being decoupled into
/// their own modulo-schedling instance. If decoupled, collects values which are
/// used (dependencies in) and defined (dependencies out) inside the decoupled
/// BB or loop. 
class ControlDependentDataDependencyAnalysis {
public:
  explicit ControlDependentDataDependencyAnalysis(
      LoopInfo &LI, DataDependenceGraph &DDG, ControlDependenceGraph &CDG) {
    bool peDecouplingOff = std::getenv("NO_PE_DECOUPLING") && 
                           strcmp(std::getenv("NO_PE_DECOUPLING"), "1") == 0;
    if (peDecouplingOff)
      return;
          
    // Go over all SCCs in the DDG.
    for (auto NodeDDG : DDG) {
      if (auto LoopSCC = dyn_cast<PiBlockDDGNode>(NodeDDG)) {
        auto AllSccPaths = getSCCPaths(*LoopSCC);
        auto AllSCCInstructionPaths = DDGNodesToInstructions(AllSccPaths);
        
        // Populate blocksToDecouple, in/outDependencies, instrToDecoupleInBB.
        // Use sets since there could be repetitions across SCCs.
        collectBlocksToDecouple(LI, CDG, AllSCCInstructionPaths);
        collectBlockInOutDependencies();
      }
    }
    
    // Check if some dependency communication can be moved before/after loops.
    collectDependenciesToHoist(LI);

    collectLoopsToDecouple(LI, CDG);
    collectLoopInOutDependencies();
  }

  ~ControlDependentDataDependencyAnalysis();

  /// Return a set of BBs marked for decoupling.
  SetVector<BasicBlock *> getBlocksToDecouple() { return blocksToDecouple; }

  /// Given a decoupled BB, return the PHINode at the start of the recurrence
  PHINode *getRecurrenceStart(BasicBlock *BB) { return recurrenceStarts[BB]; }
  /// Given a decoupled BB, return the PHINode at the end of the recurrence
  PHINode *getRecurrenceEnd(BasicBlock *BB) { return recurrenceEnds[BB]; }

  SmallVector<Loop *> getLoopsToDecouple() { return loopsToDecouple; }

  /// Given a decoupled BB, return its instructions that should be decoupled.
  SetVector<Instruction *> getInstructionsToDecouple(BasicBlock *BB) {
    return instrToDecoupleInBB[BB];
  }

  /// Given a decoupled BB, return in/out dependencies that can be communicated
  /// once before/after a loop without affecting correctness.
  SetVector<Instruction *> getInstructionsToHoist(BasicBlock *BB) {
    return instrToHoistInBB[BB];
  }

  /// Given a decoupled BB, return instructions from other BBs that it is using.
  SetVector<Instruction *> getBlockInputDependencies(BasicBlock *BB) {
    return blockInputDependencies[BB];
  }

  /// Given a decoupled BB, return instructions that it defines and that are
  /// used by other BBs.
  SetVector<Instruction *> getBlockOutputDependencies(BasicBlock *BB) {
    return blockOutputDependencies[BB];
  }

  /// Given a decoupled loop, return instructions defined outse the loop but
  /// used inside it.
  SetVector<Instruction *> getLoopInputDependencies(Loop *L) {
    return loopInputDependencies[L];
  }

  /// Given a decoupled loop, return instructions that it defines and that are
  /// used outside the loop.
  SetVector<Instruction *> getLoopOutputDependencies(Loop *L) {
    return loopOutputDependencies[L];
  }


private:
  SetVector<BasicBlock *> blocksToDecouple;
  MapVector<BasicBlock *, PHINode *> recurrenceStarts;
  MapVector<BasicBlock *, PHINode *> recurrenceEnds;
  MapVector<BasicBlock *, SetVector<Instruction *>> blockInputDependencies; 
  MapVector<BasicBlock *, SetVector<Instruction *>> blockOutputDependencies;
  MapVector<BasicBlock *, SetVector<Instruction *>> instrToDecoupleInBB;
  MapVector<BasicBlock *, SetVector<Instruction *>> instrToHoistInBB;

  SmallVector<Loop *> loopsToDecouple;
  MapVector<Loop *, SetVector<Instruction *>> loopInputDependencies; 
  MapVector<Loop *, SetVector<Instruction *>> loopOutputDependencies;

  SmallVector<SmallVector<SimpleDDGNode *>> getSCCPaths(PiBlockDDGNode &SCC);
  SmallVector<SmallVector<Instruction *>> DDGNodesToInstructions(
      SmallVector<SmallVector<SimpleDDGNode *>> &AllSccPaths);

  void collectBlocksToDecouple(
      LoopInfo &LI, ControlDependenceGraph &CDG,
      SmallVector<SmallVector<Instruction *>> &AllSCCInstructionPaths);
  void collectBlockInOutDependencies();
  void collectDependenciesToHoist(LoopInfo &LI);

  void collectLoopsToDecouple(LoopInfo &LI, ControlDependenceGraph &CDG);
  void collectLoopInOutDependencies();
};

} // end namespace llvm

#endif
