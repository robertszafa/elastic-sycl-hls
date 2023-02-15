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
  explicit ControlDependentDataDependencyAnalysis(Function &F,
                                                  ControlDependenceGraph &CDG,
                                                  LoopInfo &LI,
                                                  Instruction *bottleneckI) {
    if (auto ctrlDepSrc = CDG.getControlDependencySource(bottleneckI)) {
      ctrlDepSrcBlock = ctrlDepSrc;

      if (LI.getLoopFor(ctrlDepSrc) ==
          LI.getLoopFor(bottleneckI->getParent())) {
        ctrlDepInsideLoopFlag = true;
        dependenciesIn = getIncomingUses(F, bottleneckI->getParent());
        dependenciesOut = getOutgoingDefs(F, bottleneckI->getParent());
      }
    }
  }

  ~ControlDependentDataDependencyAnalysis();

  /// Return true if the analysis concluded that the data dependency
  /// is control dependent on a basic block from the same loop.
  bool isCtrlDepInsideLoop() { return ctrlDepInsideLoopFlag; }

  /// Return the source block of the control dependency.
  BasicBlock *getCtrlDepSrcBlock() { return ctrlDepSrcBlock; }

  /// Return values used in the basic block of the dependency,
  /// but which are defined outside of it.
  SmallVector<Instruction *> getDependenciesIn() { return dependenciesIn; }

  /// Return values defined in the basic block of the dependency,
  /// but which are used outside of it.
  SmallVector<Instruction *> getDependenciesOut() { return dependenciesOut; }

private:
  bool ctrlDepInsideLoopFlag = false;

  BasicBlock *ctrlDepSrcBlock = nullptr;

  SmallVector<Instruction *> dependenciesIn;
  SmallVector<Instruction *> dependenciesOut;

  SmallVector<Instruction *> getIncomingUses(Function &F, BasicBlock *BB);
  SmallVector<Instruction *> getOutgoingDefs(Function &F, BasicBlock *BB);
};

} // end namespace llvm

#endif
