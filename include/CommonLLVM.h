//===----------------------------------------------------------------------===//
//
// This file defines functions commonly used during LLVM passes,
// and common LLVM includes.
//
//===----------------------------------------------------------------------===//

#ifndef COMMON_LLVM_H
#define COMMON_LLVM_H

// These are the LLVM analysis passes used.
#include "llvm/Analysis/DependenceAnalysis.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/Analysis/ScalarEvolution.h"

// This includes Instruction, Function, Value and other common IR. 
#include "llvm/IR/IRBuilder.h"

#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/Support/JSON.h"
#include "llvm/Demangle/Demangle.h"


using namespace llvm;

// This file is included in several other files.
// Use an anonymous namespace to avoid multiple same definitions.
namespace {

/// Lambdas for easier use in range based algorithms.
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

/// Return the index of {child} inside of the default traverse of {parent}. 
/// Returns -1 if not found.
template <typename T1, typename T2> 
[[maybe_unused]] int getIndexOfChild(T1 *Parent, T2 *Child) {
  int idx = -1;
  for (auto &ch : *Parent) {
    idx++;
    if (&ch == Child)
      break;
  }
  
  return idx;
}

/// Return the Child at {idx} in {Parent}. Return a nullptr if out of bounds.
template <typename T1, typename T2> 
[[maybe_unused]] T2 *getChildWithIndex(T1 *Parent, int idx) {
  int i = 0;
  for (auto &ch : *Parent) {
    if (i == idx)
      return &ch;
    i++;
  }
  
  return nullptr;
}



} // namespace

#endif
