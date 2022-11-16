#ifndef STOREQ_UTILS_H
#define STOREQ_UTILS_H

#include "llvm/IR/Constants.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/SetVector.h>
#include <llvm/IR/BasicBlock.h>

#include "llvm/Analysis/CFG.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/Analysis/AssumptionCache.h"
#include "llvm/Analysis/DependenceAnalysis.h"
#include "llvm/Analysis/GlobalsModRef.h"
#include "llvm/Analysis/LoopAccessAnalysis.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/Transforms/Utils/ScalarEvolutionExpander.h"

#include "llvm/ADT/DepthFirstIterator.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Demangle/Demangle.h"
#include "llvm/Support/Casting.h"
#include <llvm/IR/Constant.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/Type.h>
#include <llvm/Support/raw_ostream.h>

#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

using namespace llvm;

namespace storeq {

/// This function has the same functionality as ScalarEvolution.getPointerBase, but it returns 
/// the Instruction representing the pointer base, not a SCEV.
Instruction *getPointerBase(Value *pointerOperand) {
  if (auto gep = dyn_cast<GetElementPtrInst>(pointerOperand)) {
    if (gep->getPointerOperand() && !gep->hasAllConstantIndices())
      return getPointerBase(dyn_cast<Instruction>(gep->getPointerOperand()));
  }

  return dyn_cast<Instruction>(pointerOperand);
}

/// Return true if the {addr2InstMap} has a key equivalent to {instr}.
bool isEquivalentInMap(const DenseMap<Instruction *, SetVector<Instruction *>> &addr2InstMap,
                       const Instruction *instr) {
  for (const auto &kv : addr2InstMap) {
    if (kv.getFirst()->isIdenticalTo(instr)) 
      return true;
  }

  return false;
}

/// Return values for the {instr} key in {addr2InstMap}.
/// Return nullptr if {addr2InstMap} doesn't have the {instr} key.
SetVector<Instruction *> *getEquivalentInMap(DenseMap<Instruction *, 
                                             SetVector<Instruction *>> &addr2InstMap,
                                             const Instruction *instr) {
  for (auto &kv : addr2InstMap) {
    if (kv.getFirst()->isIdenticalTo(instr)) 
      return &kv.getSecond();
  }

  return nullptr;
}

/// Insert {instr} into the set associated with the {basePointer} key in {addr2InstMap}.
/// If {addr2InstMap} doesn't have the {basePointer} key, then create it and insert there.
void insertInMap(DenseMap<Instruction *, SetVector<Instruction *>> &addr2InstMap,
                 Instruction *basePointer, Instruction *instr) {
  for (const auto &kv : addr2InstMap) {
    if (kv.getFirst()->isIdenticalTo(basePointer)) {
      addr2InstMap[kv.getFirst()].insert(instr);
      return;
    }
  }
  
  addr2InstMap[basePointer].insert(instr);
}

/// Collect all load and store instruction, and the their address values, that have a RAW
/// inter-iteration dependence whose scalar evolution is not computable.
void getMemInstrsWithRAW(Function &F, FunctionAnalysisManager &AM, SmallVector<Instruction *> &loads,
                         SmallVector<Instruction *> &stores) {
  auto &LI = AM.getResult<LoopAnalysis>(F);
  auto &SE = AM.getResult<ScalarEvolutionAnalysis>(F);

  // Collect all base addresses that are stored to with an SE uncomputable index inside a loop.
  DenseMap<Instruction *, SetVector<Instruction *>> addr2InstMap;

  for (Loop *TopLevelLoop : LI) {
    for (Loop *L : depth_first(TopLevelLoop)) {
      for (auto &BB : L->getBlocks()) {
        for (auto &I : *BB) {
          if (auto si = dyn_cast<StoreInst>(&I)) {
            auto siPointerSE = SE.getSCEV(si->getPointerOperand());
            auto isInterestingRAW = !SE.hasComputableLoopEvolution(siPointerSE, L) &&
                                    !SE.containsAddRecurrence(siPointerSE);
            if (isInterestingRAW) {
              auto siPointerBase = getPointerBase(si->getPointerOperand());
              insertInMap(addr2InstMap, siPointerBase, &I);
            }
          }
        }
      }
    }
  }

  // Collect loads seperately.
  for (Loop *TopLevelLoop : LI) {
    for (Loop *L : depth_first(TopLevelLoop)) {
      for (auto &BB : L->getBlocks()) {
        for (auto &I : *BB) {
          if (auto li = dyn_cast<LoadInst>(&I)) {
            auto pointerBaseInstr = getPointerBase(li->getPointerOperand());

            // We are only intersted in loads where there is 
            // a store to the the same base address.
            if (auto bucketToInsert = getEquivalentInMap(addr2InstMap, pointerBaseInstr)) {
              bucketToInsert->insert(&I);
            }
          }
        }
      }
    }
  }

  // TODO: deal with stores/loads to multiple base adrresses (see SE.getUsedLoops(SCEV, Loops[])).
  for (auto &kv : addr2InstMap) {
    // If a given base address has only a store, then there will be no RAW hazards.
    bool anyLoads = std::any_of(kv.getSecond().begin(), kv.getSecond().end(),
                                [](Instruction *i) { return isa<LoadInst>(i); });
    bool anyStores = std::any_of(kv.getSecond().begin(), kv.getSecond().end(),
                                 [](Instruction *i) { return isa<StoreInst>(i); });
    if (anyLoads && anyStores) {
      for (auto &I : kv.getSecond()) {
        if (isa<LoadInst>(I)) 
          loads.push_back(I);
        else if (isa<StoreInst>(I))
          stores.push_back(I);
      }
    }
  }

}

/// Given Function {F}, return all Functions that call {F}.
SmallVector<Function *> getCallerFunctions(Module *M, Function &F) {
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

/// This function can be used to identify spir functions that need to be transformed.
/// This is done by detecting annotations.
SmallVector<Function *> getAnnotatedFunctions(Module *M) {
  SmallVector<Function *> annotFuncs;
  const std::string AnnotationString("load_num_0");

  for (Module::global_iterator I = M->global_begin(), E = M->global_end(); I != E; ++I) {
    if (I->getName() == "llvm.global.annotations") {
      ConstantArray *CA = dyn_cast<ConstantArray>(I->getOperand(0));
      for (auto OI = CA->op_begin(); OI != CA->op_end(); ++OI) {
        ConstantStruct *CS = dyn_cast<ConstantStruct>(OI->get());
        Function *FUNC = dyn_cast<Function>(CS->getOperand(0)->getOperand(0));
        GlobalVariable *AnnotationGL = dyn_cast<GlobalVariable>(CS->getOperand(1)->getOperand(0));
        StringRef annotation =
            dyn_cast<ConstantDataArray>(AnnotationGL->getInitializer())->getAsCString();
        if (annotation.compare(AnnotationString) == 0) {
          annotFuncs.push_back(FUNC);
        }
      }
    }
  }

  return annotFuncs;
}

} // end namespace storeq

#endif