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

/// Assummes that the first n spir_func calls in F are pipe calls.
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

void transformIdxKernel(Function &F, FunctionAnalysisManager &AM, const int seqNum,
                        const int numStores, const Value *addr,  Instruction *memInst, bool isLoad) {
  CallInst *pipeWriteCall = getNthPipeCall(F, seqNum);
  assert(pipeWriteCall && "No pipe call in this function\n");

  // {idx, tag} struct store instructions.
  SmallVector<StoreInst *, 2> storesToStruct;
  Value *idxPipeWriteArg = pipeWriteCall->getArgOperand(0);
  // Get first store instruction for each GEP value.
  for (auto user : idxPipeWriteArg->users()) {
    if (auto gep = dyn_cast<GetElementPtrInst>(user)) {
      for (auto userGEP : gep->users()) {
        if (auto stInstr = dyn_cast<StoreInst>(userGEP)) {
          storesToStruct.emplace_back(stInstr);
          break;
        }
      }
    }
  }

  const GetElementPtrInst *idxGEP = dyn_cast<GetElementPtrInst>(addr);
  Value *idxVal = idxGEP->getOperand(1);

  // Move pipe call and {idx, tag} struct strores into the correct place.
  pipeWriteCall->moveBefore(memInst);
  for (auto &st : storesToStruct)
    st->moveBefore(pipeWriteCall);

  auto tagSctructStore = storesToStruct[0];
  auto idxSctructStore = storesToStruct[1];

  // Add correct operands to struct stores.
  // Add idx.
  auto idxTypeForPipe = idxSctructStore->getOperand(0)->getType();
  auto idxCasted = TruncInst::CreateTruncOrBitCast(idxVal, idxTypeForPipe, "",
                                                   dyn_cast<Instruction>(idxSctructStore));
  idxSctructStore->setOperand(0, idxCasted);

  // Add tag.
  // TODO: generalise tag generation
  PHINode *baseTagPhi;
  auto tagTypeForPipe = tagSctructStore->getOperand(0)->getType();
  for (auto &phi : memInst->getParent()->phis()) {
    baseTagPhi = dyn_cast<PHINode>(phi.clone());
    baseTagPhi->insertAfter(&phi);
    break;
  }
  IRBuilder<> Builder(pipeWriteCall->getParent()->getTerminator());
  Value *baseTagNext = Builder.CreateAdd(baseTagPhi, ConstantInt::get(tagTypeForPipe, 1));
  baseTagPhi->setIncomingValue(1, baseTagNext);

  // Calculate tag based on numer of stores in scope.
  Builder.SetInsertPoint(tagSctructStore);
  auto baseTagTimesNumStores = Builder.CreateMul(baseTagPhi, ConstantInt::get(tagTypeForPipe, numStores));
  Value *finalTag = isLoad 
                    ? baseTagTimesNumStores
                    : Builder.CreateAdd(baseTagTimesNumStores, ConstantInt::get(tagTypeForPipe, seqNum+1));
  tagSctructStore->setOperand(0, finalTag);

  deleteInstrAfter(pipeWriteCall);
}

void transformMainKernel(Function &F, FunctionAnalysisManager &AM, json::Object &report,
                         SmallVector<const Value *> &storeAddrs,
                         SmallVector<const Value *> &loadAddrs,
                         SmallVector<Instruction *> &storeInstrs,
                         SmallVector<Instruction *> &loadInstrs) {
  // Get pipe calls.
  SmallVector<CallInst*> loadPipeReadCalls(loadInstrs.size());
  SmallVector<CallInst*> storePipeWriteCalls(storeInstrs.size());
  CallInst* endSignalPipeWriteCall = getNthPipeCall(F, storeInstrs.size() + loadInstrs.size());
  Value* storeReqValPtr = endSignalPipeWriteCall->getOperand(0);

  for (size_t i=0; i<loadInstrs.size(); ++i) 
    loadPipeReadCalls[i] = getNthPipeCall(F, i);
  for (size_t i=0; i<storeInstrs.size(); ++i) 
    storePipeWriteCalls[i] = getNthPipeCall(F, i + loadInstrs.size());

  // Replace load instructions with calls to pipe::read
  for (size_t iLoad=0; iLoad<loadInstrs.size(); ++iLoad) {
    loadPipeReadCalls[iLoad]->moveBefore(loadInstrs[iLoad]);
    Value* loadVal = dyn_cast<Value>(loadInstrs[iLoad]);
    loadVal->replaceAllUsesWith(loadPipeReadCalls[iLoad]);
  }

  // Replace store instructions with calls to pipe::write
  for (size_t iStore=0; iStore<storeInstrs.size(); ++iStore) {
    storePipeWriteCalls[iStore]->moveAfter(storeInstrs[iStore]);
    storeInstrs[iStore]->setOperand(1, storePipeWriteCalls[iStore]->getOperand(0));
    // Increment num_store_req value for every store_val_pipe write call.
    IRBuilder<> IR(storeInstrs[iStore]);
    LoadInst *loadReqInstr = IR.CreateLoad(Type::getInt32Ty(IR.getContext()), storeReqValPtr);
    Value *incReqVal = IR.CreateAdd(IR.getInt32(1), loadReqInstr);
    IR.CreateStore(incReqVal, storeReqValPtr);
  }
  
  // The end signal pipe should be called before every fn exit.
  for (auto &BB : F) {
    for (auto &I : BB) {
      if (isa<ReturnInst>(I))
        endSignalPipeWriteCall->clone()->insertBefore(&I);
    }
  }
  endSignalPipeWriteCall->eraseFromParent();
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
          int iLoad = std::stoi(load_matches[1]);
          transformIdxKernel(F, AM, iLoad, loadInstrs.size(), loadAddrs[iLoad], loadInstrs[iLoad], true);
        } else if (store_matches.size() > 1) {
          int iStore = std::stoi(store_matches[1]);
          transformIdxKernel(F, AM, iStore, storeInstrs.size(), storeAddrs[iStore], storeInstrs[iStore], false);
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
