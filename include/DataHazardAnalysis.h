#ifndef RAW_DATA_HAZARD_ANALYSIS_H
#define RAW_DATA_HAZARD_ANALYSIS_H

#include "CommonLLVM.h"


using namespace llvm;

namespace llvm {

/// Data Hazard Analysis driver class.
/// Collect all memory instruction, that have a RAW inter-iteration 
/// memory dependence whose scalar evolution is not computable.
class DataHazardAnalysis  {
public:
  explicit DataHazardAnalysis(Function &F, LoopInfo &LI, ScalarEvolution &SE,
                              DominatorTree &DT);
  ~DataHazardAnalysis();

  /// Return all memory instructions for each base address in the function
  /// which has at least one data hazard.
  SmallVector<SmallVector<Instruction *>> getResult() { return hazardInstrs; }

  SmallVector<bool> getDecoupligDecisions() { return decouplingDecisions; }

private:
  /// Memory load and store instructions for each base address that is part
  /// of a data hazard.
  SmallVector<SmallVector<Instruction *>> hazardInstrs;

  /// For each base address, a bool indicating if the address generation
  /// instructions can be decoupled from F.
  SmallVector<bool> decouplingDecisions;
};

} // end namespace llvm

#endif
