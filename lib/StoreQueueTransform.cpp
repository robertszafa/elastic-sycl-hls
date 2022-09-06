// #include "StoreQueueTransform.h"
#include <cassert>
#include <fstream>
#include <sstream>

#include "llvm/Demangle/Demangle.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"

#include "llvm/Analysis/AssumptionCache.h"
#include "llvm/Analysis/BlockFrequencyInfo.h"
#include "llvm/Analysis/DependenceAnalysis.h"
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
#include "llvm/Analysis/ValueTracking.h"
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

#include "llvm/Support/JSON.h"

using namespace llvm;


// This method implements what the pass does
void visitor(Function &F, FunctionAnalysisManager &AM) {
  return;
}

/// This function can be used to identrify spir functions that need to be transformed.
/// This is done by detecting annotations.
std::set<Function *> getAnnotatedFunctions(Module *M) {
  std::set<Function *> annotFuncs;
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
          annotFuncs.insert(FUNC);
          // errs() << "Found annotated function " << FUNC->getName()<<"\n";
        }
      }
    }
  }

  return annotFuncs;
}

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
  std::set<Function*> annotFuncs;
  json::Object report;

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    // Module* M = F.getParent();

    if (report.empty()) {
      report = *parseJsonReport(loopRAWReportFilename).getAsObject();
    }
    
    if (F.getCallingConv() == CallingConv::SPIR_FUNC) {
        // F.getLinkage() == llvm::GlobalValue::InternalLinkage) {
      errs() << "num_loads: " << report["num_loads"] << "\n";
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
