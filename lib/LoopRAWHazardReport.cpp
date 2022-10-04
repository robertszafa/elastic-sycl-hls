#include "StoreqUtils.h"

#include "llvm/Analysis/AssumptionCache.h"
#include "llvm/Analysis/DependenceAnalysis.h"
#include "llvm/Analysis/GlobalsModRef.h"
#include "llvm/Analysis/LoopAccessAnalysis.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/ScalarEvolution.h"
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
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/JSON.h"

#include <algorithm>
#include <cassert>
#include <regex>
#include <string>
#include <utility>

using namespace llvm;

namespace storeq {

json::Object generateReport(Function &F, SmallVector<Instruction *> &loads,
                            SmallVector<Instruction *> &stores, SmallVector<DepPairT> &depPairs) {
  json::Object report;
  auto callers = getCallerFunctions(F.getParent(), F);
  // A spir_func lambda is called only once from one kernel.
  if (callers.size() == 1) {
    report["kernel_class_name"] = demangle(std::string(callers[0]->getName()));
    report["spir_func_name"] = demangle(std::string(F.getName()));
    report["num_copies"] = stores.size() + loads.size();
    report["num_loads"] = loads.size();
    report["num_stores"] = stores.size();
    report["num_dep_pairs"] = depPairs.size();
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
  SmallVector<DepPairT> depPairs;

  getDepMemOps(F, AM, loads, stores, depPairs);
  bool isAnyRAW = stores.size() > 0;

  if (isAnyRAW) {
    json::Object report = generateReport(F, loads, stores, depPairs);
    outs() << formatv("{0:2}", json::Value(std::move(report))) << "\n"; 
  } 
  else {
    errs() << "No RAW hazards.\n";  
  }
}

struct LoopRAWHazardReport : PassInfoMixin<LoopRAWHazardReport> {

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    bool preserveAnalysis = true;

    if (F.getCallingConv() == CallingConv::SPIR_FUNC) {
      preserveAnalysis = ifConversionForStores(F, AM);
      analyseRAW(F, AM);
    }

    return preserveAnalysis ? PreservedAnalyses::all() : PreservedAnalyses::none();
  }

  // Without isRequired returning true, this pass will be skipped for functions
  // decorated with the optnone LLVM attribute. Note that clang -O0 decorates
  // all functions with optnone.
  static bool isRequired() { return true; }

  void getAnalysisUsage(AnalysisUsage &AU) const {
    AU.addRequiredID(DependenceAnalysis::ID());
    AU.addRequiredID(LoopAccessAnalysis::ID());
    AU.addRequiredID(LoopAnalysis::ID());
    AU.addRequiredID(ScalarEvolutionAnalysis::ID());
    AU.addRequiredID(TargetIRAnalysis::ID());
    AU.addRequiredID(DominatorTreeAnalysis::ID());
    AU.addRequiredID(TargetLibraryAnalysis::ID());
    AU.addRequiredID(AAManager::ID());
    AU.addRequiredID(AssumptionAnalysis::ID());
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