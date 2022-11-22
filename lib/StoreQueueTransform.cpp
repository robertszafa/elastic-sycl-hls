#include "StoreqUtils.h"

#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Operator.h"
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
#include "llvm/Support/JSON.h"
#include "llvm/Support/raw_ostream.h"

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

void deleteInstruction(Instruction *inst) {
  inst->dropAllReferences();
  inst->replaceAllUsesWith(UndefValue::get(inst->getType()));
  inst->eraseFromParent();
}

/// Given a {cutOffI} instruction, delete all stores in F where a {keepI} doesn't dependent on it.  
/// Don't delete an instruction if it's part of {preserveInstructions}.
void deleteStoresAfterI(Function &F, FunctionAnalysisManager &AM, Instruction *cutOffI, 
                        SmallVector<Instruction *> &preserveInstructions) {
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

  for (Instruction *inst : instToDelete) {
    if (std::find(preserveInstructions.begin(), preserveInstructions.end(), inst) ==
        preserveInstructions.end()) {
      deleteInstruction(inst);
    }
  }
}

void transformAddressGeneration(Function &F, FunctionAnalysisManager &AM, Instruction *memInst,
                                Value *baseTagAddr, CallInst *pipeWriteCall,
                                SmallVector<Instruction *> &preserveInst) {
  // {idx, tag} struct store instructions.
  SmallVector<StoreInst *, 2> storesToIdxTagStruct;
  for (auto user : pipeWriteCall->getArgOperand(0)->users()) {
    if (auto gep = dyn_cast<GetElementPtrInst>(user)) {
      for (auto userGEP : gep->users()) {
        if (auto stInstr = dyn_cast<StoreInst>(userGEP)) {
          storesToIdxTagStruct.emplace_back(stInstr);
          break;
        }
      }
    }
  }
  auto tagStore = storesToIdxTagStruct[0];
  auto idxStore = storesToIdxTagStruct[1];

  // Move everything needed to setup the pipe write call to where the memInst occurs.
  pipeWriteCall->moveBefore(memInst);
  tagStore->moveBefore(pipeWriteCall);
  idxStore->moveBefore(pipeWriteCall);

  // Write the GEP value of the memInst into the pipeWrite idx field.
  Value *memInstAddr = isaStore(memInst) ? dyn_cast<StoreInst>(memInst)->getPointerOperand()
                                         : dyn_cast<LoadInst>(memInst)->getPointerOperand();
  Value *idxVal = dyn_cast<GetElementPtrInst>(memInstAddr)->getOperand(1);
  auto idxTypeForPipe = idxStore->getOperand(0)->getType();
  // Ensure idx matches pipe idx type (we assume i32 is fine but we might have to check 
  // properly in the future).
  auto idxCasted = TruncInst::CreateTruncOrBitCast(idxVal, idxTypeForPipe, "",
                                                   dyn_cast<Instruction>(idxStore));
  idxStore->setOperand(0, idxCasted);

  // Write baseTag into the pipeWrite idx field. If it's a store, increment the tag by one before.
  IRBuilder<> Builder(tagStore);
  auto tagType = tagStore->getOperand(0)->getType();
  Value *baseTagVal = Builder.CreateLoad(tagType, baseTagAddr, "baseTagVal");
  auto baseTagPlusOne = Builder.CreateAdd(baseTagVal, ConstantInt::get(tagType, 1), "baseTagPlus1");
  if (isaStore(memInst)) {
    tagStore->setOperand(0, baseTagPlusOne);
    auto newTagStore = Builder.CreateStore(baseTagPlusOne, baseTagAddr);
    preserveInst.emplace_back(newTagStore);
    preserveInst.emplace_back(dyn_cast<Instruction>(baseTagPlusOne));
  }
  else {
    tagStore->setOperand(0, baseTagVal);
  }

  // Ensure the created instructions are not deleted later.
  preserveInst.emplace_back(tagStore);
  preserveInst.emplace_back(idxStore);
  preserveInst.emplace_back(dyn_cast<Instruction>(baseTagVal));
}

void transformMainKernel(Function &F, FunctionAnalysisManager &AM,
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

struct StoreQueueTransform : PassInfoMixin<StoreQueueTransform> {
  json::Object report;

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    Module *M = F.getParent();

    // Read in report once.
    if (report.empty()) report = *(parseJsonReport().getAsObject());

    // SPIR lambda kernel functions are guaranteed to be called just once.
    auto callers = getCallerFunctions(M, F);
    if (callers.size() != 1) return PreservedAnalyses::all();

    std::string mainKernelName = std::string(report["kernel_class_name"].getAsString().getValue());
    std::string thisKernelName = demangle(std::string(callers[0]->getName()));

    if (F.getCallingConv() == CallingConv::SPIR_FUNC) {
      // The names of kernels that we need to transform are guaranteed to begin with mainKernelName.
      if (std::equal(mainKernelName.begin(), mainKernelName.end(), thisKernelName.begin())) {
        // Find out if this F should be an AGU or Main kernel.
        std::regex agu_kernel_regex{mainKernelName + "_AGU", std::regex_constants::ECMAScript};
        std::smatch agu_kernel_match;
        std::regex_search(thisKernelName, agu_kernel_match, agu_kernel_regex);
        bool isAddressGenKernel = agu_kernel_match.size() > 0;
        // If we could not split the address generation from the compute, 
        // then we need to merge the AGU kernel into the main kernel.
        bool isDecoupledAddress = (report["decouple_address"].getAsInteger().getValue() == 1);

        // Collect the same load/store instructions as during analysis.
        SmallVector<SmallVector<Instruction*>> memInstrsAll = getRAWMemInstrs(F, AM);

        // Keep track of instructions created during the pass which should not be deleted.
        SmallVector<Instruction *> preserveInst;
        for (auto &memInstrs : memInstrsAll) {
          SmallVector<Instruction*> stores = getStores(memInstrs);
          SmallVector<Instruction*> loads = getLoads(memInstrs);

          if (isAddressGenKernel || !isDecoupledAddress) {
            // At AGU function entry, initialize a base tag to 0.
            IRBuilder<> Builder(F.getEntryBlock().getTerminator());
            auto tagType = Type::getInt32Ty(F.getContext());
            Value *baseTagAddr = Builder.CreateAlloca(tagType);
            Builder.CreateStore(ConstantInt::get(tagType, 0), baseTagAddr);

            // First deal with loads, then stores.
            for (auto &I : loads) {
              // Always get 0th pipe, since pipeWriteCall will be moved away the 0th position in F.
              CallInst *pipeWriteCall = getNthPipeCall(F, 0);
              transformAddressGeneration(F, AM, I, baseTagAddr, pipeWriteCall, preserveInst);
            }
            for (auto &I : stores) {
              // Always get 0th pipe, since pipeWriteCall will be moved away the 0th position in F.
              CallInst *pipeWriteCall = getNthPipeCall(F, 0);
              transformAddressGeneration(F, AM, I, baseTagAddr, pipeWriteCall, preserveInst);
            }
          }
          
          if (!isAddressGenKernel) {
            transformMainKernel(F, AM, stores, loads);
          }
        }
          
        if (isAddressGenKernel && isDecoupledAddress) {
          // Get all stores for all base addresses.
          SmallVector<Instruction*> allStores;
          for (auto &memInstrs : memInstrsAll) {
            for (auto &iS : getStores(memInstrs)) {
              allStores.push_back(iS);
            }
          }

          // And delete them, as well as any other store after that.
          for (auto st_i : allStores) {
            bool canDelete = std::find(preserveInst.begin(), preserveInst.end(), st_i) ==
                             preserveInst.end();
            if (st_i->getParent() != nullptr && canDelete) {
              deleteStoresAfterI(F, AM, st_i, preserveInst);
              deleteInstruction(st_i);
            }
          }
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
    AU.addRequiredID(LoopAnalysis::ID());
    AU.addRequiredID(ScalarEvolutionAnalysis::ID());
    AU.addRequiredID(DominatorTreeAnalysis::ID());
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
