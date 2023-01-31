#ifndef CONTROL_DEP_DATA_DEP_ANALYSIS_H
#define CONTROL_DEP_DATA_DEP_ANALYSIS_H

#include "CommonLLVM.h"
#include "CDG.h"
#include "llvm/Analysis/DDG.h"

using namespace llvm;

namespace llvm {

class ControlDependentDataDependencyAnalysis {
public:
  explicit ControlDependentDataDependencyAnalysis(Function &F,
                                                  ControlDependenceGraph &CDG,
                                                  DataDependenceGraph &DDG,
                                                  DominatorTree &DT,
                                                  Instruction *interIterDep) {
    calculateControlDependencySource(F, CDG, interIterDep);
    if (isControlDependent()) 
      calculateInOutDependencies(F, DDG, DT, interIterDep);
  }

  ~ControlDependentDataDependencyAnalysis();

  /// Return true if the analysis concluded that the data dependencies
  /// are control dependent.
  bool isControlDependent() { 
    return controlDependencySourceBlock != nullptr; 
  }

  /// Return the source block of the control dependency.
  BasicBlock *getControlDependencySourceBlock() {
    return controlDependencySourceBlock;
  }

private:
  BasicBlock *controlDependencySourceBlock = nullptr;

  SmallVector<Instruction *> dependenciesIn;
  SmallVector<Instruction *> dependenciesOut;

  void calculateControlDependencySource(Function &F,
                                        ControlDependenceGraph &CDG,
                                        Instruction *interIterDep);

  void calculateInOutDependencies(Function &F, DataDependenceGraph &DDG,
                                  DominatorTree &DT, Instruction *I);
};

} // end namespace llvm

#endif
