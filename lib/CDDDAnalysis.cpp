
#include "CDDDAnalysis.h"
#include "CDG.h"
#include "CommonLLVM.h"

using namespace llvm;

namespace llvm {

/// Given {BB} return all instructions used in {BB} but defined in other blocks.
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

/// Given {BB} return all instructions defined in {BB} and used in other blocks.
SmallVector<Instruction *>
ControlDependentDataDependencyAnalysis::getOutgoingDefs(Function &F,
                                                        BasicBlock *BB) {
  SmallVector<Instruction *> result;

  for (auto &I : *BB) {
    for (auto User : I.users()) {
      if (auto UserI = dyn_cast<Instruction>(User)) {
        if (UserI->getParent() != BB) {
          result.push_back(&I);
          break;
        }
      }
    }
  }

  return result;
}

} // end namespace llvm
