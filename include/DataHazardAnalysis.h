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
  DataHazardAnalysis() = default;
  DataHazardAnalysis(DataHazardAnalysis &G) = delete;
  explicit DataHazardAnalysis(Function &F, LoopInfo &LI, ScalarEvolution &SE,
                              DominatorTree &DT);
  ~DataHazardAnalysis();

  /// Return all memory instructions for each base address in the function
  /// which has at least one data hazard.
  SmallVector<SmallVector<Instruction *>> getResult() {
    return clusteredInstructions;
  }

private:
  SmallVector<SmallVector<Instruction *>> clusteredInstructions;
 
};


} // end namespace llvm

#endif
