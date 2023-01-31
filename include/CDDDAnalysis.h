#ifndef CONTROL_DEP_DATA_DEP_ANALYSIS_H
#define CONTROL_DEP_DATA_DEP_ANALYSIS_H

#include "CommonLLVM.h"
#include "CDG.h"


using namespace llvm;

namespace llvm {

class ControlDependentDataDependencyAnalysis {
public:
  explicit ControlDependentDataDependencyAnalysis(Function &F,
                                                  ControlDependenceGraph &CDG,
                                                  Instruction *interIterDep);
  ~ControlDependentDataDependencyAnalysis();

  /// Return true if the analysis concluded that the data dependencies
  /// are control dependent.
  bool isControlDependent() { 
    return controlDependencySourceBlock != nullptr; 
  }

  /// Return the source block of the control dependency.
  BasicBlock *getControlDependencySource() {
    return controlDependencySourceBlock;
  }

private:
  BasicBlock *controlDependencySourceBlock = nullptr;
};

} // end namespace llvm

#endif
