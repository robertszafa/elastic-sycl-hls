
#include "CDDDAnalysis.h"
#include "CDG.h"
#include "CommonLLVM.h"

using namespace llvm;

namespace llvm {

BasicBlock *ControlDependentDataDependencyAnalysis::getControlDependencySource(
    Function &F, ControlDependenceGraph &CDG, Instruction *interIterDep) {
  // If there is an edge from the CDG root to the parent of {interIterDep},
  // then {interIterDep} is not control dependent.
  auto cdgNode = CDG.getBlockNode(interIterDep->getParent());
  if (CDG.getRoot()->hasEdgeTo(*cdgNode))
    return nullptr;

  // interIterDep is control dependent on some block. Find out the source block.
  for (auto &N : CDG) {
    // We already checked the root.
    if (isa<RootCDGNode>(N))
      continue;

    SmallVector<CDGEdge *, 2> edgesToNode;
    N->findEdgesTo(*cdgNode, edgesToNode);
    if (edgesToNode.size() > 0)
      return dyn_cast<BlockCDGNode>(N)->getBasicBlock();
  }

  return nullptr;
}

SmallVector<Instruction *>
ControlDependentDataDependencyAnalysis::getIncomingUses(Function &F,
                                                        BasicBlock *BB) {
  SmallVector<Instruction *> result;

  auto isUserInBB = [BB](auto User) {
    if (auto UserI = dyn_cast<Instruction>(User))
      return UserI->getParent() == BB;
    return false;
  };

  for (auto &otherBB : F) {
    for (auto &I : otherBB) {
      if (&otherBB != BB && llvm::any_of(I.users(), isUserInBB))
        result.push_back(&I);
    }
  }

  return result;
}

SmallVector<Instruction *>
ControlDependentDataDependencyAnalysis::getOutgoingDefs(Function &F,
                                                        BasicBlock *BB) {
  SmallVector<Instruction *> result;

  for (auto &I : *BB) {
    for (auto User : I.users()) {
      if (auto UserI = dyn_cast<Instruction>(User)) {
        if (UserI->getParent() != BB) {
          result.push_back(UserI);
        }
      }
    }
  }

  return result;
}

} // end namespace llvm
