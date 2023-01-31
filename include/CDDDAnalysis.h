#ifndef CONTROL_DEP_DATA_DEP_ANALYSIS_H
#define CONTROL_DEP_DATA_DEP_ANALYSIS_H

#include "CommonLLVM.h"
#include "CDG.h"


using namespace llvm;

namespace llvm {

class ControlDependentDataDependencyAnalysis {
public:
  explicit ControlDependentDataDependencyAnalysis(Function &F,
                                                  ControlDependenceGraph &CDG);
  ~ControlDependentDataDependencyAnalysis();
};

} // end namespace llvm

#endif
