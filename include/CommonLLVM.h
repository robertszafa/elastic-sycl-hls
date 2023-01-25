//===----------------------------------------------------------------------===//
//
// This file defines functions commonly used during LLVM passes.
//
//===----------------------------------------------------------------------===//

#ifndef COMMON_LLVM_H
#define COMMON_LLVM_H

#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include <llvm/IR/Instructions.h>

using namespace llvm;

// This file is included in several other filer.
// Use an anonymous namespace.
namespace {

auto isaLoad = [](auto i) { return isa<LoadInst>(i); };
auto isaStore = [](auto i) { return isa<StoreInst>(i); };

[[maybe_unused]] SmallVector<Instruction *>
getLoads(const SmallVector<Instruction *> &memInstr) {
  SmallVector<Instruction *> loads;
  for (auto &I : memInstr)
    if (isaLoad(I))
      loads.push_back(I);
  return loads;
}

[[maybe_unused]] SmallVector<Instruction *>
getStores(const SmallVector<Instruction *> &memInstr) {
  SmallVector<Instruction *> stores;
  for (auto &I : memInstr)
    if (isaStore(I))
      stores.push_back(I);
  return stores;
}

/// Given Function {F}, return all Functions that call {F}.
[[maybe_unused]] SmallVector<Function *> getCallerFunctions(Module *M,
                                                            Function &F) {
  // The expected case is one caller.
  SmallVector<Function *, 1> callers;
  auto &functionList = M->getFunctionList();
  for (auto &function : functionList) {
    for (auto &bb : function) {
      for (auto &instruction : bb) {
        if (CallInst *callInst = dyn_cast<CallInst>(&instruction)) {
          if (Function *calledFunction = callInst->getCalledFunction()) {
            if (calledFunction->getName() == F.getName()) {
              callers.push_back(&function);
            }
          }
        }
      }
    }
  }

  return callers;
}

} // namespace

#endif
