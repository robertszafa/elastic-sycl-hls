#include "StoreqUtils.h"

#include "llvm/Analysis/AssumptionCache.h"
#include "llvm/Analysis/DependenceAnalysis.h"
#include "llvm/Analysis/GlobalsModRef.h"
#include "llvm/Analysis/LoopAccessAnalysis.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Operator.h"
#include <cstddef>
#include <llvm/IR/Attributes.h>
#include <llvm/IR/Constant.h>
#include <llvm/IR/Function.h>
#include "llvm/Transforms/Utils/ScalarEvolutionExpander.h"
#include "llvm/Transforms/Utils/Cloning.h"

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
void deleteSideEffectInstrAfter(Instruction* cutOffInst) {
  SmallVector<Instruction *> instToDelete;

  bool isAfterCutoff = false;
  BasicBlock *cutOffBB = cutOffInst->getParent();
  for (Instruction &inst : cutOffBB->getInstList()) {
    if (isAfterCutoff && (isa<StoreInst>(&inst) || isa<LoadInst>(&inst))) {
      instToDelete.emplace_back(&inst);
    } else if (cutOffInst->isIdenticalTo(&inst)) {
      isAfterCutoff = true;
    }
  }

  for (Instruction *inst : instToDelete) {
    inst->dropAllReferences();
    inst->replaceAllUsesWith(UndefValue::get(inst->getType()));
    inst->eraseFromParent();
  }
}

/// Given a {keepI} instruction, delete all stores in F where a {keepI} doesn't dependent on it.  
void deleteStoresAfterI(Function &F, FunctionAnalysisManager &AM, Instruction *cutOffI) {
  auto &DT = AM.getResult<DominatorTreeAnalysis>(F);

  // First delete stores in the cutOffI's BB.
  SmallVector<Instruction *> instToDelete;
  bool isAfterCutoff = false;
  BasicBlock *cutOffBB = cutOffI->getParent();
  for (Instruction &inst : cutOffBB->getInstList()) {
    if (isAfterCutoff && (isa<StoreInst>(&inst) || isa<LoadInst>(&inst))) {
      instToDelete.emplace_back(&inst);
    } else if (cutOffI->isIdenticalTo(&inst)) {
      isAfterCutoff = true;
    }
  }

  // Now delete all stores in BB's dominated by cuttOffI.
  for (auto &BB : F) {
    for (auto &I : BB) {
      if (isa<StoreInst>(&I) && DT.properlyDominates(cutOffBB, &BB)) 
        instToDelete.emplace_back(&I);
    }
  }

  for (Instruction *inst : instToDelete) 
    deleteInstruction(inst);
}

/// Assumes F has one exit BB after the --mergereturn pass.
BasicBlock* getExitBB(Function &F) {
  BasicBlock *exit = nullptr;
  for (auto &BB : F) {
    for (auto &I : BB) {
      assert(!(isa<ReturnInst>(I) && exit != nullptr) && "Precondition of single exit BB violated\n");

      if (isa<ReturnInst>(I) && exit == nullptr) {
        exit = &BB;
        continue;
      }
    }
  }

  return exit;
}

// Check if {I} is control dependent, i.e. is there a path going through L.latch without {I}?
bool isControlDependent(Function &F, FunctionAnalysisManager &AM, Instruction *I) {
  auto &DT = AM.getResult<DominatorTreeAnalysis>(F);
  
  Loop *L = getBBLoop(AM.getResult<LoopAnalysis>(F), I->getParent());
  assert(L && "Instruction not part of loop in isControlDependent()\n");
  auto latchBB = L->getLoopLatch();

  return !(DT.dominates(I->getParent(), latchBB));
}


void transformIdxKernel(Function &F, FunctionAnalysisManager &AM, const int ithKernel,
                        const int storesSoFarInclusive, const int numStores, const Value *addr,
                        Instruction *memInst) {
  CallInst *pipeWriteCall = getNthPipeCall(F, 0);
  assert(pipeWriteCall && "No pipe call in this function\n");

  Loop *L = getBBLoop(AM.getResult<LoopAnalysis>(F), memInst->getParent());
  
  // {idx, tag} struct store instructions.
  SmallVector<StoreInst *, 2> storesToStruct;
  for (auto user : pipeWriteCall->getArgOperand(0)->users()) {
    if (auto gep = dyn_cast<GetElementPtrInst>(user)) {
      for (auto userGEP : gep->users()) {
        if (auto stInstr = dyn_cast<StoreInst>(userGEP)) {
          storesToStruct.emplace_back(stInstr);
          break;
        }
      }
    }
  }
  auto tagStoreInHeader = storesToStruct[0];
  auto idxStoreInHEader = storesToStruct[1];

  // Move {idx, tag} struct strores into Loop header. Pipe write into loop latch.
  // The loop header will store correct tag, and dummy idx. Later the loop body 
  // (depending on control flow) will store the correct idx.
  // The loop latch BB can be the only BB in the body, so need to insert call at the end.
  pipeWriteCall->moveBefore(L->getLoopLatch()->getTerminator());
  for (auto &st : storesToStruct)
    st->moveBefore(L->getHeader()->getTerminator());

  // Add idx struct store (storing to {idx, tag} struct).
  // Dummy idx=-1 in loop header. Actual idx (which might be control dependent) in loop body.
  const GetElementPtrInst *idxGEP = dyn_cast<GetElementPtrInst>(addr);
  Value *idxVal = idxGEP->getOperand(1);
  auto idxTypeForPipe = idxStoreInHEader->getOperand(0)->getType();
  idxStoreInHEader->setOperand(0, ConstantInt::get(idxTypeForPipe, -1));
  auto idxStoreInBody = idxStoreInHEader->clone();
  idxStoreInBody->insertBefore(memInst);
  auto idxCasted = TruncInst::CreateTruncOrBitCast(idxVal, idxTypeForPipe, "",
                                                   dyn_cast<Instruction>(idxStoreInBody));
  idxStoreInBody->setOperand(0, idxCasted);

  // No need for memInst store anyore, delete it and any other stores dominated by it. 
  // Any values that are postdominated by the deleted instructions will be deleted by dce.
  deleteStoresAfterI(F, AM, memInst);
  if (isa<StoreInst>(memInst)) deleteInstruction(memInst);

  // Add tag struct store and tag generation instructions. 
  // TODO: generalise tag generation to nested loops
  IRBuilder<> Builder(F.getEntryBlock().getTerminator());
  // In function entry, alloca for baseTag. Set baseTag=0.
  auto tagTypeForPipe = tagStoreInHeader->getOperand(0)->getType();
  Value *baseTagAddr = Builder.CreateAlloca(tagTypeForPipe);  
  Builder.CreateStore(ConstantInt::get(tagTypeForPipe, 0), baseTagAddr);
  // In loop header, load base tag in the BB where the {idx, tag} pipe write occurs.
  // Calculate: tag = loopInductionVar * kNumStoresInLoop + storesSoFarInclusive
  // Builder.SetInsertPoint(L->getHeader());
  Builder.SetInsertPoint(tagStoreInHeader);
  Value *baseTag = Builder.CreateLoad(tagTypeForPipe, baseTagAddr);
  auto storesSoFarInclVal = ConstantInt::get(tagTypeForPipe, storesSoFarInclusive);
  auto numStoresVal = ConstantInt::get(tagTypeForPipe, numStores);
  auto baseTagTimesNumStoresVal = Builder.CreateMul(baseTag, numStoresVal);
  Value *tag = Builder.CreateAdd(baseTagTimesNumStoresVal, storesSoFarInclVal);
  tagStoreInHeader->setOperand(0, tag);

  // In loop latch, increment base tag after {idx, tag} pipe write.
  Builder.SetInsertPoint(pipeWriteCall);
  Value *baseTagNext = Builder.CreateAdd(baseTag, ConstantInt::get(tagTypeForPipe, 1));
  Builder.CreateStore(baseTagNext, baseTagAddr);
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
        ifConversionForStores(F, AM);

        std::smatch load_matches, store_matches;
        std::regex_search(thisKernelName, load_matches, load_regex);
        std::regex_search(thisKernelName, store_matches, store_regex);

        SmallVector<Instruction *> loads;
        SmallVector<Instruction *> stores;
        // Holds {loads[i], storej[j]} pairs. 
        SmallVector<DepPairT> depPairs;

        getDepMemOps(F, AM, loads, stores, depPairs);

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
    AU.addRequiredID(PostDominatorTreeAnalysis::ID());
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
