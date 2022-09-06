// #include "StoreQueueTransform.h"

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

  // Get all memory loads and stores.
  SmallVector<const Value *> storeAddrs;
  SmallVector<const Value *> loadAddrs;
  SmallVector<Instruction *> storeInstrs;
  SmallVector<Instruction *> loadInstrs;
  bool isAnyRAW = false;
  for (Loop *TopLevelLoop : LI) {
    for (Loop *L : depth_first(TopLevelLoop)) {
      auto &LAI = LAM.getResult<LoopAccessAnalysis>(*L, AR);

      auto depChecker = LAI.getDepChecker();
      const auto allDeps = depChecker.getDependences();
      errs() << "isSafeForVectorization(): " << depChecker.isSafeForVectorization() << "\n";
      if (depChecker.isSafeForVectorization())
        continue;

      errs() << "All deps:\n";
      for (const auto dep : *allDeps) {
        Instruction* srcI = dep.getSource(LAI);
        Instruction* dstI = dep.getDestination(LAI);
        assert(isa<LoadInst>(srcI) && "Source of dependence is not a load\n");
        assert(isa<StoreInst>(dstI) && "Destination of dependence is not a store\n");

        LoadInst *li = dyn_cast<LoadInst>(srcI);
        StoreInst *si = dyn_cast<StoreInst>(dstI);
        const SCEV *ldPointerSCEV = SE.getSCEV(li->getPointerOperand());
        const SCEV *stPointerSCEV = SE.getSCEV(si->getPointerOperand());

        (si->getPointerOperand())->print(errs());
        errs() << "\ngetname: " << (si->getPointerOperand())->getName();
        errs() << "\ngetOperand: " << (si->getPointerOperand())->getNameOrAsOperand();
        errs() << "\n";
        
        if (SE.isLoopInvariant(ldPointerSCEV, L) && SE.hasComputableLoopEvolution(ldPointerSCEV, L) &&
            SE.isLoopInvariant(stPointerSCEV, L) && SE.hasComputableLoopEvolution(stPointerSCEV, L)) {
          errs() << "Ignoring dep because is invariant or SCEV-computable\n";
          continue;
        }
        loadAddrs.push_back(li->getPointerOperand());
        loadInstrs.push_back(li);
        storeInstrs.push_back(si);
        storeAddrs.push_back(si->getPointerOperand());

        isAnyRAW = true;

        errs() << "\nldPointer\n";
        errs() << "SCEV formulation of pointer: ";
        ldPointerSCEV->print(errs());
        errs() << "\n";
        li->getPointerOperand()->print(errs());
        errs() << "\nstPointer\n";
        errs() << "SCEV formulation of pointer: ";
        stPointerSCEV->print(errs());
        errs() << "\n";
        si->getPointerOperand()->print(errs());
        errs() << "\n";
      }
    }
  }

  if (isAnyRAW) {
    errs() << demangle(std::string(F.getName())) << " has dependency\n";
    errs() << "\n\n--Analysis:\n";

    Module* mod = F.getParent();
    auto &functionList = mod->getFunctionList();
    for (auto &function : functionList) {
      for (auto &bb : function) {
        for (auto &instruction : bb) {
          if (CallInst *callInst = dyn_cast<CallInst>(&instruction)) {
            if (Function *calledFunction = callInst->getCalledFunction()) {
              if (calledFunction->getName() == F.getName()) {
                errs() << "Kernel: " << demangle(std::string(function.getName())) << "\n";
                errs() << "Num Copies: " << storeInstrs.size() + loadInstrs.size() << "\n";
                errs() << "Num Loads: " << loadInstrs.size() << "\n";
                errs() << "Num Stores: " << storeInstrs.size() << "\n";
                errs() << "Array Line: " << storeInstrs[0]->getDebugLoc().getLine() << "\n";
                errs() << "Array Column: " << storeInstrs[0]->getDebugLoc()->getColumn() << "\n";

                // The array type is not used since it's more robus to get the type from array_name:
                //     using val_type = std::remove_reference<decltype( *{array_name} )>::type;
                errs() << "Array Type: "; 
                dyn_cast<StoreInst>(storeInstrs[0])->getPointerOperand()->getType()->print(errs());
                errs() << "\n"; 
              }
            }
          }
        }
      }
    }
  }

}

struct StoreQueueTransform : PassInfoMixin<StoreQueueTransform> {
  const std::string loopRAWReportFilename = "loop-raw-report.txt";



  // Main entry point, takes IR unit to run the pass on (&F) and the
  // corresponding pass manager (to be queried if need be)
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    if (F.getCallingConv() == CallingConv::SPIR_FUNC) {// &&
        // F.getLinkage() == llvm::GlobalValue::InternalLinkage) {
      visitor(F, AM);
    }

    return PreservedAnalyses::all();
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
