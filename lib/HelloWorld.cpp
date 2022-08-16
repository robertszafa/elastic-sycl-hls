//=============================================================================
// FILE:
//    HelloWorld.cpp
//
// DESCRIPTION:
//    Visits all functions in a module, prints their names and the number of
//    arguments via stderr. Strictly speaking, this is an analysis pass (i.e.
//    the functions are not modified). However, in order to keep things simple
//    there's no 'print' method here (every analysis pass should implement it).
//
// USAGE:
//    1. Legacy PM
//      opt -load libHelloWorld.so -legacy-hello-world -disable-output `\`
//        <input-llvm-file>
//    2. New PM
//      opt -load-pass-plugin=libHelloWorld.so -passes="hello-world" `\`
//        -disable-output <input-llvm-file>
//
//
// License: MIT
//=============================================================================

// #include "HelloWorld.h"

#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/IR/CallingConv.h"
#include "llvm/ADT/SmallVector.h"

// #include "llvm/Analysis/DependenceAnalysis.h"
#include "llvm/Analysis/AssumptionCache.h"
#include "llvm/Analysis/BlockFrequencyInfo.h"
#include "llvm/Analysis/GlobalsModRef.h"
#include "llvm/Analysis/LazyBlockFrequencyInfo.h"
#include "llvm/Analysis/LoopAccessAnalysis.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/ProfileSummaryInfo.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/ScalarEvolutionExpressions.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/InitializePasses.h"
#include "llvm/Pass.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Utils.h"
#include "llvm/Transforms/Utils/LoopSimplify.h"
#include "llvm/Transforms/Utils/LoopVersioning.h"
#include "llvm/Transforms/Utils/ScalarEvolutionExpander.h"
#include "llvm/Transforms/Utils/SizeOpts.h"

// #include "llvm/module_split.h"
// llvm::module_split

using namespace llvm;

//-----------------------------------------------------------------------------
// HelloWorld implementation
//-----------------------------------------------------------------------------
// No need to expose the internals of the pass to the outside world - keep
// everything in an anonymous namespace.
// namespace {

// This method implements what the pass does
void visitor(Function &F, FunctionAnalysisManager &AM) {
  auto &LI = AM.getResult<LoopAnalysis>(F);
  auto &SE = AM.getResult<ScalarEvolutionAnalysis>(F);
  auto &TTI = AM.getResult<TargetIRAnalysis>(F);
  auto &DT = AM.getResult<DominatorTreeAnalysis>(F);
  auto &TLI = AM.getResult<TargetLibraryAnalysis>(F);
  auto &AA = AM.getResult<AAManager>(F);
  auto &AC = AM.getResult<AssumptionAnalysis>(F);
  auto &LAM = AM.getResult<LoopAnalysisManagerFunctionProxy>(F).getManager();

  LoopStandardAnalysisResults AR = {AA,  AC,  DT,      LI,      SE,
                                    TLI, TTI, nullptr, nullptr, nullptr};

  for (Loop *TopLevelLoop : LI) {
    for (Loop *L : depth_first(TopLevelLoop)) {
      auto &LAI = LAM.getResult<LoopAccessAnalysis>(*L, AR);
      const auto *Deps = LAI.getDepChecker().getDependences();

      errs() << "loads: " << LAI.getNumLoads() << "\n";
      errs() << LAI.getReport()->getMsg() << "\n";
      // if (Deps) {
      //     errs() << "Dependence "
      //           << Deps[0][0].Type
      //           << ", num_stores"
      //           << LAI.getNumStores()
      //           << ", num_loaded"
      //           << LAI.getNumLoads();
      // }
          //       << "\n";
      // if (Deps) {
      //   for (const auto &Dep : *Deps) {
      //     errs() << "Dependence in\n";
      //     Instruction *Source = Dep.getSource(LAI);
      //     Instruction *Destination = Dep.getDestination(LAI);
      //     errs() << int(Dep.Type) << ", ";

      //   }
      // }
    }
  }
  errs() << "\n ";


  errs() << "F.getName(): " << F.getName() << "\n";
  errs() << "number of arguments: " << F.arg_size() << "\n\n";
}

struct HelloWorld : PassInfoMixin<HelloWorld> {
  // Main entry point, takes IR unit to run the pass on (&F) and the
  // corresponding pass manager (to be queried if need be)
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    // if (F.getCallingConv() == CallingConv::SPIR_FUNC && F.getLinkage() == llvm::GlobalValue::InternalLinkage)
      visitor(F, AM);

    return PreservedAnalyses::all();
  }

  // Without isRequired returning true, this pass will be skipped for functions
  // decorated with the optnone LLVM attribute. Note that clang -O0 decorates
  // all functions with optnone.
  static bool isRequired() { return true; }

  void getAnalysisUsage(AnalysisUsage &AU) const {
    AU.addRequiredID(LoopAnalysis::ID());
    AU.addRequiredID(ScalarEvolutionAnalysis::ID());
    AU.addRequiredID(TargetIRAnalysis::ID());
    AU.addRequiredID(DominatorTreeAnalysis::ID());
    AU.addRequiredID(TargetLibraryAnalysis::ID());
    AU.addRequiredID(AAManager::ID());
    AU.addRequiredID(AssumptionAnalysis::ID());
    AU.addRequiredID(LoopAccessAnalysis::ID());
  }

};

// } // namespace

//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getHelloWorldPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "HelloWorld", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "hello-world") {
                    FPM.addPass(HelloWorld());
                    return true;
                  }
                  return false;
                });
          }};
}

// This is the core interface for pass plugins. It guarantees that 'opt' will
// be able to recognize HelloWorld when added to the pass pipeline on the
// command line, i.e. via '-passes=hello-world'
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getHelloWorldPluginInfo();
}
