#include "StoreqUtils.h"

#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/DependenceAnalysis.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/Transforms/Utils/ScalarEvolutionExpander.h"

#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/InitializePasses.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/Demangle/Demangle.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/JSON.h"
#include "llvm/Support/raw_ostream.h"

#include <algorithm>
#include <cassert>
#include <regex>
#include <string>
#include <utility>

using namespace llvm;

namespace storeq {

json::Object generateReport(Function &F, SmallVector<Instruction *> &loads,
                            SmallVector<Instruction *> &stores) {
  json::Object report;
  auto callers = getCallerFunctions(F.getParent(), F);
  // A spir_func lambda is called only once from one kernel.
  if (callers.size() == 1) {
    report["kernel_class_name"] = demangle(std::string(callers[0]->getName()));
    report["spir_func_name"] = demangle(std::string(F.getName()));
    report["num_loads"] = loads.size();
    report["num_stores"] = stores.size();
    report["array_line"] = stores[0]->getDebugLoc().getLine();
    report["array_column"] = stores[0]->getDebugLoc()->getColumn();
  }

  return report;
}

// Returns true if inst0 is control dependent (may_occur && !must_occur), 
// where the control dependency depends on {inst1}.
//
// The motivation behind this check is to see, if the generation of store addresses can be hosited
// into it's own kernel, without using the base address of the {st} instruction. 
// 
// Go over all users of {onI} recursively:
//  - if we hit a branch:
//      - if the store is in any but not all of the BB dominated by the branch, return true
// Record pairs into {checkedPairs} to prune the recursive search.
bool isInst0ConditionalOnInst1(DominatorTree &DT, Instruction *inst0, Instruction *inst1, 
                               SmallVector<SmallVector<Instruction *, 2>> &checkedPairs) {
  SmallVector<Instruction *, 2> thisPair(2);
  thisPair[0] = inst0;
  thisPair[1] = inst1;
  if (std::find(checkedPairs.begin(), checkedPairs.end(), thisPair) != checkedPairs.end())
    return false;
  
  checkedPairs.emplace_back(thisPair);

  for (auto user : inst1->users()) {
    if (!isa<Instruction>(user))
      continue;

    auto userI = dyn_cast<Instruction>(user);
    if (auto brI = dyn_cast<BranchInst>(userI)) {
      bool atLeastOneDominatesStore = false;
      bool allDominateStore = true;
      for (auto succ : brI->successors()) {
        allDominateStore &= DT.dominates(succ, inst0->getParent());
        atLeastOneDominatesStore |= DT.dominates(succ, inst0->getParent());
      }

      if (atLeastOneDominatesStore && !allDominateStore) 
        return true;
    }

    return isInst0ConditionalOnInst1(DT, inst0, userI, checkedPairs);
  }

  return false;
}

bool canSplitAddressGenFromCompute(Function &F, FunctionAnalysisManager &AM, 
                            SmallVector<Instruction *> &loads,
                            SmallVector<Instruction *> &stores) {
  auto &DT = AM.getResult<DominatorTreeAnalysis>(F);

  bool isAnyStoreControlDepOnBaseAddr = false;
  for (auto si : stores) {
    for (auto li : loads) {
      SmallVector<SmallVector<Instruction *, 2>> checkedPairs;
      isAnyStoreControlDepOnBaseAddr |= isInst0ConditionalOnInst1(DT, si, li, checkedPairs);
    }
  }

  return !isAnyStoreControlDepOnBaseAddr;
}

void analyseRAW(Function &F, FunctionAnalysisManager &AM) {
  // Get all memory loads and stores that form a RAW hazard dependence.
  SmallVector<Instruction *> loads;
  SmallVector<Instruction *> stores;
  getMemInstrsWithRAW(F, AM, loads, stores);

  if (stores.size() > 0) {
    json::Object report = generateReport(F, loads, stores);
    
    // Check if the store address generation can be split from the compute into a different kernel.
    bool canSplitStores = canSplitAddressGenFromCompute(F, AM, loads, stores);
    // report["split_stores"] = int(canSplitStores);
    report["split_stores"] = int(0);

    outs() << formatv("{0:2}", json::Value(std::move(report))) << "\n"; 

    // Degub prints.
    dbgs() << "---- DEBUG ----\nCollected instructions\nStores " << stores.size() << ":\n";
    for (auto &si : stores) {
      si->print(dbgs());
      dbgs() << "\n";
    }
    dbgs() << "\nLoads " << loads.size() << ":\n";
    for (auto &li : loads) {
      li->print(dbgs());
      dbgs() << "\n";
    }
    dbgs() << "\nDecoupled address gen: " << canSplitStores << "\n---- DEBUG ----\n\n";
  } 
  else {
    errs() << "Warning: Report not generated - no RAW hazards.\n";  
  }
}

struct LoopRAWHazardReport : PassInfoMixin<LoopRAWHazardReport> {

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    if (F.getCallingConv() == CallingConv::SPIR_FUNC) 
      analyseRAW(F, AM);

    return PreservedAnalyses::all();
  }

  // Without isRequired returning true, this pass will be skipped for functions
  // decorated with the optnone LLVM attribute. Note that clang -O0 decorates
  // all functions with optnone.
  static bool isRequired() { return true; }

  void getAnalysisUsage(AnalysisUsage &AU) const {
    AU.addRequiredID(LoopAnalysis::ID());
    AU.addRequiredID(ScalarEvolutionAnalysis::ID());
    AU.addRequiredID(DominatorTreeAnalysis::ID());
    AU.addRequiredID(PostDominatorTreeAnalysis::ID());
    AU.addRequiredID(DependenceAnalysis::ID());
  }
};


//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getLoopRAWHazardReportPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "LoopRAWHazardReport", LLVM_VERSION_STRING, [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback([](StringRef Name, FunctionPassManager &FPM,
                                                  ArrayRef<PassBuilder::PipelineElement>) {
              if (Name == "loop-raw-report") {
                FPM.addPass(LoopRAWHazardReport());
                return true;
              }
              return false;
            });
          }};
}

// This is the core interface for pass plugins. It guarantees that 'opt' will find the pass.
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return getLoopRAWHazardReportPluginInfo();
}

} // end namespace storeq