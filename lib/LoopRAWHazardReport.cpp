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
    report["num_copies"] = 1 + loads.size(); // 1 store idx kernel
    report["num_loads"] = loads.size();
    report["num_stores"] = stores.size();
    report["array_line"] = stores[0]->getDebugLoc().getLine();
    report["array_column"] = stores[0]->getDebugLoc()->getColumn();

    std::string typeStr;
    llvm::raw_string_ostream rso(typeStr);
    stores[0]->getOperand(0)->getType()->print(rso);
    report["val_type"] = rso.str();
  }

  return report;
}

void analyseRAW(Function &F, FunctionAnalysisManager &AM) {
  // Get all memory loads and stores that form a RAW hazard dependence.
  SmallVector<Instruction *> loads;
  SmallVector<Instruction *> stores;
  // DenseMap<const SCEV *, SmallVector<Instruction *>> memInstrs;

  getMemInstrsWithRAW(F, AM, loads, stores);
  bool isAnyRAW = stores.size() > 0;

  if (isAnyRAW) {
    json::Object report = generateReport(F, loads, stores);
    
    // Check if the store address generation can be split from the compute into a different kernel.
    bool canSplitStores = canSplitAddressGenFromCompute(F, AM, loads, stores);
    report["split_stores"] = int(canSplitStores);

    outs() << formatv("{0:2}", json::Value(std::move(report))) << "\n"; 

    // Degub prints.
    dbgs() << "dbg: collected the following offending instructions\ndbg: Stores " << stores.size() << ":\n";
    for (auto &si : stores) {
      si->print(dbgs());
      dbgs() << "\n";
    }
    dbgs() << "dbg: Loads " << loads.size() << ":\n";
    for (auto &li : loads) {
      li->print(dbgs());
      dbgs() << "\n";
    }
    dbgs() << "dbg: canSplitStores " << canSplitStores << "\n";
  } 
  else {
    errs() << "Warning: Report not generated - no RAW hazards.\n";  
  }
}

struct LoopRAWHazardReport : PassInfoMixin<LoopRAWHazardReport> {

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    bool invalidateAnalysis = false;

    if (F.getCallingConv() == CallingConv::SPIR_FUNC) {
      invalidateAnalysis = ifConversionForStores(F, AM);
      dbgs() << "\ndgb: ifConversionForStores " << invalidateAnalysis << "\n";

      invalidateAnalysis |= hoistLoadsOutOfBranches(F, AM);
      dbgs() << "dgb: movedLoadsToFront " << invalidateAnalysis << "\n";

      // TODO: recalculate analysis
      analyseRAW(F, AM);
    }

    return invalidateAnalysis ? PreservedAnalyses::none() : PreservedAnalyses::all();
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