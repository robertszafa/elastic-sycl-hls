#ifndef STOREQ_UTILS_H
#define STOREQ_UTILS_H

#include "llvm/IR/Constants.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include <llvm/ADT/DenseMap.h>
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

Instruction *getPointerBase(Instruction *pointerOperand) {
  if (auto gep = dyn_cast<GetElementPtrInst>(pointerOperand)) 
    return getPointerBase(dyn_cast<Instruction>(gep->getPointerOperand()));

  return pointerOperand;
}

/// Collect all load and store instruction, and the their address values, that have a RAW
/// inter-iteration dependence whose scalar evolution is not computable.
void getMemInstrsWithRAW(Function &F, FunctionAnalysisManager &AM, SmallVector<Instruction *> &loads,
                         SmallVector<Instruction *> &stores) {
  auto &LI = AM.getResult<LoopAnalysis>(F);
  auto &SE = AM.getResult<ScalarEvolutionAnalysis>(F);

  // Collect all base addresses that are stored to with an SE uncomputable index inside a loop.
  DenseMap<const SCEV *, SmallVector<Instruction *>> memInstrs;
  for (Loop *TopLevelLoop : LI) {
    for (Loop *L : depth_first(TopLevelLoop)) {
      for (auto &BB : L->getBlocks()) {
        for (auto &I : *BB) {
          if (auto si = dyn_cast<StoreInst>(&I)) {
            auto siPointerSE = SE.getSCEV(si->getPointerOperand());
            if (!SE.hasComputableLoopEvolution(siPointerSE, L)) {
              auto siPointerBaseSE = SE.getPointerBase(siPointerSE);
              if (std::find(memInstrs[siPointerBaseSE].begin(), memInstrs[siPointerBaseSE].end(),
                            &I) == memInstrs[siPointerBaseSE].end()) {
                memInstrs[siPointerBaseSE].emplace_back(&I);
              }
            }
          }
        }
      }
    }
  }

  // Collect loads seperately.
  // TODO: check if loads and stores are part of the same loop?
  // TODO: check if multiple loads are part of the same BB, and generate a single kernel for them?
  for (Loop *TopLevelLoop : LI) {
    for (Loop *L : depth_first(TopLevelLoop)) {
      for (auto &BB : L->getBlocks()) {
        for (auto &I : *BB) {
          if (auto li = dyn_cast<LoadInst>(&I)) {
            auto liPointerSE = SE.getSCEV(li->getPointerOperand());
            auto liPointerBaseSE = SE.getPointerBase(liPointerSE);
            // Add if there is an offending store to the same base address.
            if (memInstrs.find(liPointerBaseSE) != memInstrs.end()) {
              // And if not added before.
              if (std::find(memInstrs[liPointerBaseSE].begin(), memInstrs[liPointerBaseSE].end(),
                            &I) == memInstrs[liPointerBaseSE].end()) {
                memInstrs[liPointerBaseSE].emplace_back(&I);
              }
            }
          }
        }
      }
    }
  }

  // TODO: deal with stores/loads to multiple base adrresses (see SE.getUsedLoops(SCEV, Loops[])).
  for (auto &kv : memInstrs) {
    for (auto &I : kv.getSecond()) {
      if (isa<LoadInst>(I)) 
        loads.push_back(I);
      else if (isa<StoreInst>(I))
        stores.push_back(I);
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