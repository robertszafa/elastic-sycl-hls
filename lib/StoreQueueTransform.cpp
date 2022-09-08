#include "StoreqUtils.h"

#include "llvm/Analysis/AssumptionCache.h"
#include "llvm/Analysis/DependenceAnalysis.h"
#include "llvm/Analysis/GlobalsModRef.h"
#include "llvm/Analysis/LoopAccessAnalysis.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Operator.h"
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

CallInst* getNthPipeCall(Function &F, const int n) {
  int callsSoFar = 0;
  for (auto &bb : F) {
    for (auto &instruction : bb) {
      if (CallInst *callInst = dyn_cast<CallInst>(&instruction)) {
        if (Function *calledFunction = callInst->getCalledFunction()) {
          if (calledFunction->getCallingConv() == CallingConv::SPIR_FUNC && callsSoFar == n) {
            return callInst;
          }
          else if (calledFunction->getCallingConv() == CallingConv::SPIR_FUNC) {
            callsSoFar++;
          }
        }
      }
    }
  }

  return nullptr;
}

/// Delete any side-effect instructions and their users that come after {InstToDeleteAfter}.
/// (Uses of deleteded Values are replaced with undefs).
void deleteInstrAfter(Instruction* InstToDeleteAfter) {
  SmallVector<Instruction *> instToDelete;
  bool afterPipe = false;
  for (Instruction &inst : InstToDeleteAfter->getParent()->getInstList()) {
    if (afterPipe && (isa<StoreInst>(&inst) || isa<LoadInst>(&inst))) {
      instToDelete.emplace_back(&inst);
    } else if (InstToDeleteAfter->isSameOperationAs(&inst)) {
      afterPipe = true;
    }
  }
  for (Instruction *inst : instToDelete) {
    inst->dropAllReferences();
    inst->replaceAllUsesWith(UndefValue::get(inst->getType()));
    inst->eraseFromParent();
  }
}

void transformLoadKernel(Function &F, FunctionAnalysisManager &AM, const int loadNum, const int numStores,
                         SmallVector<const Value *> &loadAddrs,
                         SmallVector<Instruction *> &loadInstrs) {
  const GetElementPtrInst *gepInst = dyn_cast<GetElementPtrInst>(loadAddrs[0]);
  Value *idxVal = gepInst->getOperand(1);
  CallInst *pipeWriteCall = getNthPipeCall(F, loadNum);
  assert(pipeWriteCall && "No pipe call in this function\n");

  // {idx, tag} struct store instructions.
  SmallVector<StoreInst *, 2> idxTagStores;
  Value *pipeArgPtr = pipeWriteCall->getArgOperand(0);
  // Get first store instrs into the gep values.
  for (auto user : pipeArgPtr->users()) {
    if (auto gep = dyn_cast<GetElementPtrInst>(user)) {
      for (auto userGEP : gep->users()) {
        if (auto stInstr = dyn_cast<StoreInst>(userGEP)) {
          idxTagStores.emplace_back(stInstr);
          break;
        }
      }
    }
  }

  // Move pipe call and {idx, tag} struct strores into place.
  pipeWriteCall->moveBefore(loadInstrs[loadNum]);
  for (auto &st : idxTagStores)
    st->moveBefore(pipeWriteCall);

  // Add correct operands to struct stores.
  auto idxTypeForPipe = idxTagStores[0]->getOperand(0)->getType();
  auto idxCasted = TruncInst::CreateTruncOrBitCast(idxVal, idxTypeForPipe, "",
                                                   dyn_cast<Instruction>(idxTagStores[0]));
  idxTagStores[0]->setOperand(0, idxCasted);

  // Create phi for tag base induction variable.
  // TODO: generalise tag generation
  PHINode *tagPhi;
  auto tagTypeForPipe = idxTagStores[1]->getOperand(0)->getType();
  for (auto &phi : loadInstrs[0]->getParent()->phis()) {
    tagPhi = dyn_cast<PHINode>(phi.clone());
    tagPhi->insertAfter(&phi);
    break;
  }
  IRBuilder<> Builder(pipeWriteCall->getParent()->getTerminator());
  Value *NextVar = Builder.CreateAdd(tagPhi, ConstantInt::get(tagTypeForPipe, 1), "tagBaseNext");
  tagPhi->setIncomingValue(1, NextVar);

  // Calculate tag based on numer of stores in scope.
  Builder.SetInsertPoint(idxTagStores[1]);
  Value *finalTag = Builder.CreateMul(tagPhi, ConstantInt::get(tagTypeForPipe, numStores), "tag");
  idxTagStores[1]->setOperand(0, finalTag);

  deleteInstrAfter(pipeWriteCall);
}

void transformStoreKernel(Function &F, FunctionAnalysisManager &AM, const int storeNum,
                          SmallVector<const Value *> &storeAddrs,
                          SmallVector<Instruction *> &storeInstrs) {
  // errs() << "Visited " << demangle(std::string(F.getName())) << "\n";
}

void transformMainKernel(Function &F, FunctionAnalysisManager &AM, json::Object &report,
                         SmallVector<const Value *> &storeAddrs,
                         SmallVector<const Value *> &loadAddrs,
                         SmallVector<Instruction *> &storeInstrs,
                         SmallVector<Instruction *> &loadInstrs) {
  // errs() << "Visited " << demangle(std::string(F.getName())) << "\n";
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
          transformLoadKernel(F, AM, loadNum, storeInstrs.size(), loadAddrs, loadInstrs);
        } else if (store_matches.size() > 1) {
          int storeNum = std::stoi(store_matches[1]);
          transformStoreKernel(F, AM, storeNum, storeAddrs, storeInstrs);
        } else {
          transformMainKernel(F, AM, report, storeAddrs, loadAddrs, storeInstrs, loadInstrs);
        }
      }
    }

    return PreservedAnalyses::none();
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