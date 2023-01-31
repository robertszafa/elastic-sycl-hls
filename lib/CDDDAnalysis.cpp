
#include "CommonLLVM.h"
#include "CDDDAnalysis.h"
#include "CDG.h"

using namespace llvm;

namespace llvm {

void ControlDependentDataDependencyAnalysis::calculateControlDependencySource(
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
      controlDependencySourceBlock = dyn_cast<BlockCDGNode>(N)->getBasicBlock();
      return;
    }
  }
}

template <typename T>
T *getDDGNodeForInstruction(DataDependenceGraph &DDG, Instruction *I) {
  for (auto &N : DDG) {
    if (!isa<T>(N))
      continue;

    SmallVector<Instruction *> Is;
    N->collectInstructions([](auto v) { return isa<Instruction>(v); }, Is);

    if (llvm::find(Is, I) != Is.end())
      return dyn_cast<T>(N);
  }

  return nullptr;
}

void ControlDependentDataDependencyAnalysis::calculateInOutDependencies(
    Function &F, DataDependenceGraph &DDG, DominatorTree &DT, Instruction *I) {
  // errs() << DDG << "\n****************************\n";
  auto piBlock = getDDGNodeForInstruction<PiBlockDDGNode>(DDG, I);
  assert(piBlock && "No DDG pi block node for the instruction.");

  errs() << *piBlock << "\n";

  // TODO: Given the DDG piblock, calculate the required in and out pipes.
  //       Dominance info is probably not needed if we have the DDG.
}

} // end namespace llvm
