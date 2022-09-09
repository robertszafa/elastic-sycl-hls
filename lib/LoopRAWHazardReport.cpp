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

using namespace llvm;

namespace storeq {

json::Object generateReport(Function &F, SmallVector<const Value *> &storeAddrs,
                            SmallVector<const Value *> &loadAddrs,
                            SmallVector<Instruction *> &storeInstrs,
                            SmallVector<Instruction *> &loadInstrs) {
  json::Object report;
  auto callers = getCallerFunctions(F.getParent(), F);
  // A spir_func lambda is called only once from one kernel.
  if (callers.size() == 1) {
    report["kernel_class_name"] = demangle(std::string(callers[0]->getName()));
    report["spir_func_name"] = demangle(std::string(F.getName()));
    report["num_copies"] = storeInstrs.size() + loadInstrs.size();
    report["num_loads"] = loadInstrs.size();
    report["num_stores"] = storeInstrs.size();
    report["array_line"] = storeInstrs[0]->getDebugLoc().getLine();
    report["array_column"] = storeInstrs[0]->getDebugLoc()->getColumn();

    std::string typeStr;
    llvm::raw_string_ostream rso(typeStr);
    storeInstrs[0]->getOperand(0)->getType()->print(rso);
    report["val_type"] = rso.str();
  }

  return report;
}

// This method implements what the pass does
void visitor(Function &F, FunctionAnalysisManager &AM) {
  // Get all memory loads and stores that form a RAW hazard dependence.
  SmallVector<const Value *> storeAddrs;
  SmallVector<const Value *> loadAddrs;
  SmallVector<Instruction *> storeInstrs;
  SmallVector<Instruction *> loadInstrs;
  getDepMemOps(F, AM, storeAddrs, loadAddrs, storeInstrs, loadInstrs);
  bool isAnyRAW = storeInstrs.size() > 0;

  if (isAnyRAW) {
    json::Object report = generateReport(F, storeAddrs, loadAddrs, storeInstrs, loadInstrs);
    outs() << formatv("{0:2}", json::Value(std::move(report))) << "\n"; 
  }
}

struct LoopRAWHazardReport : PassInfoMixin<LoopRAWHazardReport> {

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    // F.getLinkage() == llvm::GlobalValue::InternalLinkage) 
    if (F.getCallingConv() == CallingConv::SPIR_FUNC) {
      visitor(F, AM);
    }

    return PreservedAnalyses::all();
  }

  // Without isRequired returning true, this pass will be skipped for functions
  // decorated with the optnone LLVM attribute. Note that clang -O0 decorates
  // all functions with optnone.
  static bool isRequired() { return true; }

  void getAnalysisUsage(AnalysisUsage &AU) const {
    // AU.addRequiredID(DependenceAnalysis::ID());

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