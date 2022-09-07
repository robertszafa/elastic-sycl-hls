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

void transformLoadKernel(Function &F, FunctionAnalysisManager &AM, const int loadNum,
                         SmallVector<const Value *> &storeAddrs,
                         SmallVector<const Value *> &loadAddrs,
                         SmallVector<Instruction *> &storeInstrs,
                         SmallVector<Instruction *> &loadInstrs) {

  bool isAnyRAW = storeInstrs.size() > 0;
  errs() << "Visited " << demangle(std::string(F.getName())) << "\n";
  errs() << "isAnyRAW " << isAnyRAW << "\n";
  return;
}

void transformStoreKernel(Function &F, FunctionAnalysisManager &AM, const int storeNum,
                          SmallVector<const Value *> &storeAddrs,
                          SmallVector<const Value *> &loadAddrs,
                          SmallVector<Instruction *> &storeInstrs,
                          SmallVector<Instruction *> &loadInstrs) {
  errs() << "Visited " << demangle(std::string(F.getName())) << "\n";
  return;
}

void transformMainKernel(Function &F, FunctionAnalysisManager &AM, json::Object &report,
                         SmallVector<const Value *> &storeAddrs,
                         SmallVector<const Value *> &loadAddrs,
                         SmallVector<Instruction *> &storeInstrs,
                         SmallVector<Instruction *> &loadInstrs) {
  errs() << "Visited " << demangle(std::string(F.getName())) << "\n";
  return;
}

/// Given json file name, return llvm::json::Value
json::Value parseJsonReport(const std::string fname) {
  std::ifstream t(fname);
  std::stringstream buffer;
  buffer << t.rdbuf();

  auto Json = json::parse(llvm::StringRef(buffer.str()));
  assert(Json && "Error parsing json loop-raw-report");

  if (Json)
    return *Json;
  return json::Value(nullptr);
}

struct StoreQueueTransform : PassInfoMixin<StoreQueueTransform> {
  const std::string loopRAWReportFilename = "loop-raw-report.json";
  SmallVector<Function *> annotFuncs;
  json::Object report;
  std::regex load_regex{"_load_(\\d+)", std::regex_constants::ECMAScript};
  std::regex store_regex{"_store_(\\d+)", std::regex_constants::ECMAScript};

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    Module *M = F.getParent();

    // Read in report once.
    if (report.empty()) {
      report = *parseJsonReport(loopRAWReportFilename).getAsObject();
    }

    auto callers = getCallerFunctions(M, F);
    if (callers.size() != 1)
      return PreservedAnalyses::all();

    std::string mainKernelName = std::string(report["kernel_class_name"].getAsString().getValue());
    std::string thisKernelName = demangle(std::string(callers[0]->getName()));

    if (F.getCallingConv() == CallingConv::SPIR_FUNC) {
      // The names of kernels that we need to transform are guaranteed to begin with mainKernelName.
      if (std::equal(mainKernelName.begin(), mainKernelName.end(), thisKernelName.begin())) {
        std::smatch load_matches, store_matches;
        std::regex_search(thisKernelName, load_matches, load_regex);
        std::regex_search(thisKernelName, store_matches, store_regex);

        SmallVector<const Value *> storeAddrs;
        SmallVector<const Value *> loadAddrs;
        SmallVector<Instruction *> storeInstrs;
        SmallVector<Instruction *> loadInstrs;
        getDepMemOps(F, AM, storeAddrs, loadAddrs, storeInstrs, loadInstrs);

        if (load_matches.size() > 1) {
          int loadNum = std::stoi(load_matches[1]);
          transformLoadKernel(F, AM, loadNum, storeAddrs, loadAddrs, storeInstrs, loadInstrs);
        } else if (store_matches.size() > 1) {
          int storeNum = std::stoi(store_matches[1]);
          transformStoreKernel(F, AM, storeNum, storeAddrs, loadAddrs, storeInstrs, loadInstrs);
        } else {
          transformMainKernel(F, AM, report, storeAddrs, loadAddrs, storeInstrs, loadInstrs);
        }
      }
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
llvm::PassPluginLibraryInfo getStoreQueueTransformPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "StoreQueueTransform", LLVM_VERSION_STRING, [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback([](StringRef Name, FunctionPassManager &FPM,
                                                  ArrayRef<PassBuilder::PipelineElement>) {
              if (Name == "stq-transform") {
                FPM.addPass(StoreQueueTransform());
                return true;
              }
              return false;
            });
          }};
}

// This is the core interface for pass plugins. It guarantees that 'opt' will
// be able to recognize the pass via '-passes=stq-insert'
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return getStoreQueueTransformPluginInfo();
}

} // end namespace storeq