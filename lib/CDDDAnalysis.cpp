
#include "CommonLLVM.h"
#include "CDDDAnalysis.h"
#include "CDG.h"

using namespace llvm;

namespace llvm {

ControlDependentDataDependencyAnalysis::ControlDependentDataDependencyAnalysis(
    Function &F, ControlDependenceGraph &CDG, Instruction *interIterDep) {
  // If there is an edge from the CDG root to the parent of {interIterDep},
  // then {interIterDep} is not control dependent.
  auto cdgNode = CDG.getBlockNode(interIterDep->getParent());
  if (CDG.getRoot()->hasEdgeTo(*cdgNode))
    return;

  // interIterDep is control dependent on some block. Find out the source block.
  for (auto &N : CDG) {
    // We already checked the root.
    if (isa<RootCDGNode>(N))
      continue;

    SmallVector<CDGEdge *, 2> edgesToNode;
    N->findEdgesTo(*cdgNode, edgesToNode);
    if (edgesToNode.size() > 0) {
      this->controlDependencySourceBlock =
          dyn_cast<BlockCDGNode>(N)->getBasicBlock();
      break;
    }
  }
}

} // end namespace llvm
