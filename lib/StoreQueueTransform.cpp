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
#include <llvm/IR/Function.h>
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
void deleteSideEffectInstrAfter(Instruction* InstToDeleteAfter) {
  SmallVector<Instruction *> instToDelete;
  bool isAfterCutoff = false;
  for (Instruction &inst : InstToDeleteAfter->getParent()->getInstList()) {
    if (isAfterCutoff && (isa<StoreInst>(&inst) || isa<LoadInst>(&inst))) {
      instToDelete.emplace_back(&inst);
    } else if (InstToDeleteAfter->isIdenticalTo(&inst)) {
      isAfterCutoff = true;
    }
  }
  for (Instruction *inst : instToDelete) {
    inst->dropAllReferences();
    inst->replaceAllUsesWith(UndefValue::get(inst->getType()));
    inst->eraseFromParent();
  }
}

void transformIdxKernel(Function &F, FunctionAnalysisManager &AM, const int ithKernel,
                        const int storesSoFarInclusive, const int numStores, const Value *addr,
                        Instruction *memInst) {
  CallInst *pipeWriteCall = getNthPipeCall(F, 0);
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

  // Add idx struct store (storing to {idx, tag} struct).
  auto idxTypeForPipe = idxSctructStore->getOperand(0)->getType();
  auto idxCasted = TruncInst::CreateTruncOrBitCast(idxVal, idxTypeForPipe, "",
                                                   dyn_cast<Instruction>(idxSctructStore));
  idxSctructStore->setOperand(0, idxCasted);

  // Add tag struct store and tag generation instructions. 
  // TODO: generalise tag generation to nested loops
  IRBuilder<> Builder(F.getEntryBlock().getTerminator());

  // Initialize base tag to 0.
  auto tagTypeForPipe = tagSctructStore->getOperand(0)->getType();
  Value *baseTagAddr = Builder.CreateAlloca(tagTypeForPipe);  
  Builder.CreateStore(ConstantInt::get(tagTypeForPipe, 0), baseTagAddr);

  // Load base tag in the BB where the {idx, tag} pipe write occurs.
  Builder.SetInsertPoint(tagSctructStore);
  Value *baseTag = Builder.CreateLoad(tagTypeForPipe, baseTagAddr);

  // Calculate: tag = loopInductionVar * kNumStoresInLoop + storesSoFarInclusive
  auto storesSoFarInclVal = ConstantInt::get(tagTypeForPipe, storesSoFarInclusive);
  auto numStoresVal = ConstantInt::get(tagTypeForPipe, numStores);
  auto baseTagTimesNumStoresVal = Builder.CreateMul(baseTag, numStoresVal);
  Value *tag = Builder.CreateAdd(baseTagTimesNumStoresVal, storesSoFarInclVal);
  tagSctructStore->setOperand(0, tag);

  // Increment base tag after {idx, tag} pipe write.
  Builder.SetInsertPoint(pipeWriteCall);
  Value *baseTagNext = Builder.CreateAdd(baseTag, ConstantInt::get(tagTypeForPipe, 1));
  Builder.CreateStore(baseTagNext, baseTagAddr);

  deleteSideEffectInstrAfter(pipeWriteCall);
}

void transformMainKernel(Function &F, FunctionAnalysisManager &AM, json::Object &report,
                         SmallVector<Instruction *> &stores, SmallVector<Instruction *> &loads) {
  // Get pipe calls.
  SmallVector<CallInst*> loadPipeReadCalls(loads.size());
  SmallVector<CallInst*> storePipeWriteCalls(stores.size());
  CallInst* endSignalPipeWriteCall = getNthPipeCall(F, stores.size() + loads.size());
  Value* storeReqValPtr = endSignalPipeWriteCall->getOperand(0);

  for (size_t i=0; i<loads.size(); ++i) 
    loadPipeReadCalls[i] = getNthPipeCall(F, i);
  for (size_t i=0; i<stores.size(); ++i) 
    storePipeWriteCalls[i] = getNthPipeCall(F, i + loads.size());

  // Replace load instructions with calls to pipe::read
  for (size_t iLoad=0; iLoad<loads.size(); ++iLoad) {
    loadPipeReadCalls[iLoad]->moveBefore(loads[iLoad]);
    Value* loadVal = dyn_cast<Value>(loads[iLoad]);
    loadVal->replaceAllUsesWith(loadPipeReadCalls[iLoad]);
  }

  // Replace store instructions with calls to pipe::write
  for (size_t iStore=0; iStore<stores.size(); ++iStore) {
    storePipeWriteCalls[iStore]->moveAfter(stores[iStore]);
    stores[iStore]->setOperand(1, storePipeWriteCalls[iStore]->getOperand(0));
    // Increment num_store_req value for every store_val_pipe write call.
    IRBuilder<> IR(stores[iStore]);
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

/// Given a {ld -> st} inter-iteration dependence, move both instructions to the same BB, if possible.
///   Example this: 
///       x = hist[idx_scalar];
///       if (wt > 0)
///           hist[idx_scalar] = x + 10.0;
///   Would be transformed into this: 
///       if (wt > 0)
///           x = hist[idx_scalar];
///           hist[idx_scalar] = x + 10.0;
/// Don't do anything if the move is not possible, e.g. if the ld value is used before the st.
void moveIntoSameBB(Function &F, FunctionAnalysisManager &AM, Instruction *ldI, Instruction *stI) {
  if (ldI->getParent() == stI->getParent())
    return;

  // If exists use of ldI that is not dominated by stI, then we cannot move the ldI.
  auto &DT = AM.getResult<DominatorTreeAnalysis>(F);
  for (auto ldUser : ldI->users()) {
    if (auto ldUserI = dyn_cast<Instruction>(ldUser)) {
      if (!DT.dominates(stI->getParent(), ldUserI->getParent())) {
        // errs() << "Store does not dominate ld user\n";
        return;
      }
    }
  }

  // errs() << "Ld moved\n";
  ldI->moveBefore(stI->getParent()->getFirstNonPHI());
}

/// Given json file name, return llvm::json::Value
json::Value parseJsonReport() {
  if (const char* fname = std::getenv("LOOP_RAW_REPORT")) {
    std::ifstream t(fname);
    std::stringstream buffer;
    buffer << t.rdbuf();

    auto Json = json::parse(llvm::StringRef(buffer.str()));
    assert(Json && "Error parsing json loop-raw-report");

    return *Json;
  }

  assert("Error parsing json loop-raw-report");
  return json::Value(nullptr);
}

/// return the number of instr from {instrs} that occur {beforeThisI} in {F}.
/// Works even if instructions are in differnt basic blocks.
int getNumInstrsBeforeThisInstr(const SmallVector<Instruction *> &instrs, Instruction *beforeThisI,
                                Function &F) {  
  int res = 0;
  for (auto &BB : F) {
    for (auto &I : BB) {
      if (I.isIdenticalTo(beforeThisI))
        return res;
      
      for (auto &instrsI : instrs) {
        if (instrsI->isIdenticalTo(&I))
          res++;
      }
    }
  }

  assert("{beforeThisI} not present in F.");
  return res;
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
      report = *parseJsonReport().getAsObject();
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

        SmallVector<Instruction *> loads;
        SmallVector<Instruction *> stores;
        SmallVector<DepPairT> depPairs;
        getDepMemOps(F, AM, loads, stores, depPairs);
        
        for (size_t i=0; i<stores.size(); ++i) 
          moveIntoSameBB(F, AM, loads[i], stores[i]);

        if (load_matches.size() > 1) {
          int iLoad = std::stoi(load_matches[1]);
          int storesSoFarInclusive = getNumInstrsBeforeThisInstr(stores, loads[iLoad], F);
          transformIdxKernel(F, AM, iLoad, storesSoFarInclusive, loads.size(),
                             dyn_cast<LoadInst>(loads[iLoad])->getPointerOperand(), loads[iLoad]);
        } else if (store_matches.size() > 1) {
          int iStore = std::stoi(store_matches[1]);
          int storesSoFarInclusive = iStore + 1;
          transformIdxKernel(F, AM, iStore, storesSoFarInclusive, stores.size(),
                             dyn_cast<StoreInst>(stores[iStore])->getPointerOperand(), stores[iStore]);
        } else {
          transformMainKernel(F, AM, report, stores, loads);
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

} // end namespace storeq
