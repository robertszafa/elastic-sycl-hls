#ifndef STOREQ_UTILS_H
#define STOREQ_UTILS_H

#include "llvm/IR/Constants.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"

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

/// Collect all load and store instruction, and the their address values, that have a RAW
/// inter-iteration dependence whose scalar evolution is not computable.
void getDepMemOps(Function &F, FunctionAnalysisManager &AM, SmallVector<const Value *> &storeAddrs,
                  SmallVector<const Value *> &loadAddrs, SmallVector<Instruction *> &storeInstrs,
                  SmallVector<Instruction *> &loadInstrs) {
  auto &LI = AM.getResult<LoopAnalysis>(F);
  auto &SE = AM.getResult<ScalarEvolutionAnalysis>(F);
  auto &TTI = AM.getResult<TargetIRAnalysis>(F);
  auto &DT = AM.getResult<DominatorTreeAnalysis>(F);
  auto &TLI = AM.getResult<TargetLibraryAnalysis>(F);
  auto &AA = AM.getResult<AAManager>(F);
  auto &AC = AM.getResult<AssumptionAnalysis>(F);
  // auto &DA = AM.getResult<DependenceAnalysis>(F);
  auto &LAM = AM.getResult<LoopAnalysisManagerFunctionProxy>(F).getManager();
  LoopStandardAnalysisResults AR = {AA, AC, DT, LI, SE, TLI, TTI, nullptr, nullptr, nullptr};

  for (Loop *TopLevelLoop : LI) {
    for (Loop *L : depth_first(TopLevelLoop)) {
      auto &LAI = LAM.getResult<LoopAccessAnalysis>(*L, AR);

      auto depChecker = LAI.getDepChecker();
      const auto allDeps = depChecker.getDependences();
      if (depChecker.isSafeForVectorization())
        continue;

      for (const auto dep : *allDeps) {
        Instruction *srcI = dep.getSource(LAI);
        Instruction *dstI = dep.getDestination(LAI);
        assert(isa<LoadInst>(srcI) && "Source of dependence is not a load\n");
        assert(isa<StoreInst>(dstI) && "Destination of dependence is not a store\n");

        LoadInst *li = dyn_cast<LoadInst>(srcI);
        StoreInst *si = dyn_cast<StoreInst>(dstI);
        const SCEV *ldPointerSCEV = SE.getSCEV(li->getPointerOperand());
        const SCEV *stPointerSCEV = SE.getSCEV(si->getPointerOperand());

        if (SE.isLoopInvariant(ldPointerSCEV, L) &&
            SE.hasComputableLoopEvolution(ldPointerSCEV, L) &&
            SE.isLoopInvariant(stPointerSCEV, L) &&
            SE.hasComputableLoopEvolution(stPointerSCEV, L)) {
          continue;
        }
        loadAddrs.push_back(li->getPointerOperand());
        loadInstrs.push_back(li);
        storeInstrs.push_back(si);
        storeAddrs.push_back(si->getPointerOperand());
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

/// This function can be used to identrify spir functions that need to be transformed.
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
          // errs() << "Found annotated function " << FUNC->getName()<<"\n";
        }
      }
    }
  }

  return annotFuncs;
}


} // end namespace storeq

#endif