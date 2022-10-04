#ifndef STOREQ_UTILS_H
#define STOREQ_UTILS_H

#include "llvm/IR/Constants.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include <llvm/IR/BasicBlock.h>

#include "llvm/Analysis/CFG.h"
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

#include <fstream>
#include <sstream>
#include <string>

using namespace llvm;

namespace storeq {

using DepPairT = std::pair<Instruction *, Instruction *>;

/// Collect all load and store instruction, and the their address values, that have a RAW
/// inter-iteration dependence whose scalar evolution is not computable.
void getDepMemOps(Function &F, FunctionAnalysisManager &AM, SmallVector<Instruction *> &loads,
                  SmallVector<Instruction *> &stores, SmallVector<DepPairT> &depPairs) {
  auto &LI = AM.getResult<LoopAnalysis>(F);
  auto &SE = AM.getResult<ScalarEvolutionAnalysis>(F);
  auto &TTI = AM.getResult<TargetIRAnalysis>(F);
  auto &DT = AM.getResult<DominatorTreeAnalysis>(F);
  auto &TLI = AM.getResult<TargetLibraryAnalysis>(F);
  auto &AA = AM.getResult<AAManager>(F);
  auto &AC = AM.getResult<AssumptionAnalysis>(F);
  auto &DA = AM.getResult<DependenceAnalysis>(F);
  auto &LAM = AM.getResult<LoopAnalysisManagerFunctionProxy>(F).getManager();
  LoopStandardAnalysisResults AR = {AA, AC, DT, LI, SE, TLI, TTI, nullptr, nullptr, nullptr};

  for (Loop *TopLevelLoop : LI) {
    for (Loop *L : depth_first(TopLevelLoop)) {
      auto &LAI = LAM.getResult<LoopAccessAnalysis>(*L, AR);
      auto depChecker = LAI.getDepChecker();
      
      if (LAI.canVectorizeMemory())
        continue;

      auto &memInstrs = depChecker.getMemoryInstructions();

      for (auto &I0 : memInstrs) {
        for (auto &I1 : memInstrs) {
          // Capture only pairs where load depends on store (inter-iteration RAW hazards).
          if (I0 == I1 || !DA.depends(I0, I1, false) || !isa<LoadInst>(I0) || !isa<StoreInst>(I1))
            continue;

          auto li = dyn_cast<LoadInst>(I0);
          auto si = dyn_cast<StoreInst>(I1);
          auto liPointer = li->getPointerOperand();
          auto siPointer = si->getPointerOperand();

          // If both load and store pointers have a computable scalar evolution, then ignore. 
          if (SE.hasComputableLoopEvolution(SE.getSCEV(liPointer), L) && 
              SE.hasComputableLoopEvolution(SE.getSCEV(siPointer), L)) {
            continue;
          }

          auto depPair = DepPairT{li, si};

          if (std::count(loads.begin(), loads.end(), li) == 0) 
            loads.push_back(li);
          if (std::count(stores.begin(), stores.end(), si) == 0) 
            stores.push_back(si);
          if (std::count(depPairs.begin(), depPairs.end(), depPair) == 0) 
            depPairs.push_back(depPair);
        }
      }
    }
  }
}

/// Given a BasicBlock, return the inner-most loop that it belongs to.
/// Return nullptr if it doesn't belong to any loop.
Loop *getBBLoop(LoopInfo &LI, BasicBlock *BB) {
  Loop *res = nullptr;

  for (Loop *TopLevelLoop : LI) {
    for (Loop *L : depth_first(TopLevelLoop)) {
      if (L->contains(BB))
        res = L;
    }
  }

  return res;
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

void deleteInstruction(Instruction *inst) {
  inst->dropAllReferences();
  inst->replaceAllUsesWith(UndefValue::get(inst->getType()));
  inst->eraseFromParent();
}

/// Given a vector of stores, cluster them into equivalence sets, where equivalence is defined
/// as writing to the same address.
SmallVector<SmallVector<Instruction *>> 
getEquivalentStores(SmallVector<Instruction *> &stores) {
  SmallVector<SmallVector<Instruction *>> storeEquivalenceClasses;

  int num_classes = 0;
  for (auto &si : stores) {
    auto siAddrI = dyn_cast<Instruction>(dyn_cast<StoreInst>(si)->getPointerOperand());
    bool foundClass = false;

    // Compare si->pointerOperand to pointer operand from the eq classes.
    // If there is a match, add it to the matching class.
    for (auto &eqClass : storeEquivalenceClasses) {
      auto eqClassI = dyn_cast<StoreInst>(eqClass[0]);
      auto eqClassAddrI = dyn_cast<Instruction>(eqClassI->getPointerOperand());

      // isIdenticalTo would only match when the valueIDs are the same.
      if (eqClassAddrI->isSameOperationAs(siAddrI)) {
        eqClass.emplace_back(si);
        foundClass = true;
        break;
      }
    }

    // If there is no match, create new class and add si to it.
    if (!foundClass) {
      storeEquivalenceClasses.emplace_back(SmallVector<Instruction *>());
      storeEquivalenceClasses[num_classes].emplace_back(stores[0]);
      num_classes++;
    }
  }

  return storeEquivalenceClasses;
}

/// Given two basic blocks (with same pre/successors), each containing a store instruction writing
/// to the same address, convert the 2 stores into one store in the successor -- if-conversion.
void doIfConversionForStore(SmallVector<Instruction *> stores) {
  if (stores.size() < 2) return;

  // Create a phi = (si0.operand, si1.operand, ...)
  IRBuilder<> Builder(stores[0]->getParent()->getSingleSuccessor()->getFirstNonPHI());
  auto valPhi = Builder.CreatePHI(stores[0]->getOperand(0)->getType(), stores.size());
  
  for (auto &si : stores) 
    valPhi->addIncoming(si->getOperand(0), si->getParent());

  // Move store 0 to succ and set val to the phi.
  stores[0]->moveAfter(valPhi);
  stores[0]->setOperand(0, valPhi);
  // Delete rest of stores.
  for (size_t i=1; i<stores.size(); ++i)
    deleteInstruction(stores[i]);
}

/// Turn:
///   if (..)
///     hist[i] = calc(x)
///   else
///     hist[i] = calc(y)
///
/// Into:
///   if (..)
///     calc(x)
///   else
///     calc(y)
///   hist[i] = phi(x, y)
bool ifConversionForStores(Function &F, FunctionAnalysisManager &AM) {
  bool isTransformed = false;

  // Get all memory loads and stores that form a RAW hazard dependence.
  SmallVector<Instruction *> loads;
  SmallVector<Instruction *> stores;
  SmallVector<DepPairT> depPairs;

  getDepMemOps(F, AM, loads, stores, depPairs);
  if (stores.size() == 0) return isTransformed;

  // Each class holds stores with the same address expression
  SmallVector<SmallVector<Instruction *>> eqStoreClasses = getEquivalentStores(stores);

  // Now check if there is a path from the Loop header to Loop latch that doesn't go through any
  // of the stores. If that's the case, then there exists an execution path without the store,
  // and we cannot do the if-conversion.
  auto &LI = AM.getResult<LoopAnalysis>(F);
  auto &DT = AM.getResult<DominatorTreeAnalysis>(F);
  for (auto &eqClass : eqStoreClasses) {
    if (eqClass.size() > 32) {
      errs() << "Warning: Exceeded num equivalent stores in ifConversionForStores() (TODO: split)\n";
      continue;
    }

    // Put stores into a set, expected by the CFG analysis API (with max capacity of 32).
    SmallPtrSet<BasicBlock *, 32> exclusionSet;
    for (auto &si : eqClass)
      exclusionSet.insert(si->getParent());

    auto *L = getBBLoop(LI, eqClass[0]->getParent());
    if (!isPotentiallyReachable(L->getHeader(), L->getLoopLatch(), &exclusionSet, &DT, &LI)) {
      // No header->latch path without store in eqClass. Do ifConversion, invalidate analysis'
      // (since we're chaning the IR), and call ifConversionForStores() recursively with changed IR.
      doIfConversionForStore(eqClass);
      isTransformed = true;
      AM.invalidate(F, PreservedAnalyses::none());
      ifConversionForStores(F, AM);
      break;
    }
  }

  return isTransformed;
}


} // end namespace storeq

#endif