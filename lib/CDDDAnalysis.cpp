
#include "CDDDAnalysis.h"
#include "CDG.h"
#include "CommonLLVM.h"

using namespace llvm;

namespace llvm {

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
