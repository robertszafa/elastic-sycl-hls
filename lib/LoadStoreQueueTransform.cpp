#include "CommonLLVM.h"

#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/iterator_range.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

#include <fstream>
#include <iterator>
#include <regex>

using namespace llvm;

namespace llvm {

/// Assummes that the first n spir_func calls in F are pipe calls.
CallInst *getNthPipeCall(Function &F, const int n) {
  int callsSoFar = 0;
  for (auto &bb : F) {
    for (auto &instruction : bb) {
      if (CallInst *callInst = dyn_cast<CallInst>(&instruction)) {
        if (Function *calledFunction = callInst->getCalledFunction()) {
          if (calledFunction->getCallingConv() == CallingConv::SPIR_FUNC &&
              callsSoFar == n) {
            return callInst;
          } else if (calledFunction->getCallingConv() ==
                     CallingConv::SPIR_FUNC) {
            callsSoFar++;
          }
        }
      }
    }
  }

  return nullptr;
}

CallInst *getPipeCall(Function &F, json::Object pipeInfo) {
  /*
        {
          "pipe_name": "pipe_ld_val_0_0..",
          "struct_id": 0
        },
        {
          "pipe_name": "pipe_st_val_0..",
          "struct_id": -1
        }
  */
  // -1 means it's not a PipeArray

  return nullptr;
}

void deleteInstruction(Instruction *inst) {
  inst->dropAllReferences();
  inst->replaceAllUsesWith(UndefValue::get(inst->getType()));
  inst->eraseFromParent();
}

/// Given a {cutOffI}, delete all stores in F except {preserveIs} instructions.
void deleteStoresAfterI(Function &F, DominatorTree &DT, Instruction *cutOffI,
                        SmallVector<Instruction *> &preserveIs) {
  // First collect stores in the cutOffI's BB occuring after cutOffI.
  SmallVector<Instruction *> instToDelete;
  bool isAfterCutoff = false;
  BasicBlock *cutOffBB = cutOffI->getParent();
  for (Instruction &inst : cutOffBB->getInstList()) {
    if (isAfterCutoff && (isa<StoreInst>(&inst) || isa<LoadInst>(&inst))) {
      instToDelete.push_back(&inst);
    } else if (cutOffI->isIdenticalTo(&inst)) {
      isAfterCutoff = true;
    }
  }

  // Next collect all stores in BB's dominated by cuttOffI's BB.
  for (auto &BB : F) {
    for (auto &I : BB) {
      if (isa<StoreInst>(&I) && DT.properlyDominates(cutOffBB, &BB))
        instToDelete.push_back(&I);
    }
  }

  // Finally, delete the collected instructions.
  auto canBeDeleted = [&preserveIs](auto I) {
    return llvm::find(preserveIs, I) == preserveIs.end();
  };
  llvm::map_range(llvm::make_filter_range(instToDelete, canBeDeleted),
                  deleteInstruction);
}

/// Given a memory instruction (load or store), change it to a sycl pipe write
/// instruction, where the value being written is the load/store address.
/// Also, if the memInstr is a store, then increment the value at {baseTagAddr}.
SmallVector<Instruction *> transformAddressGeneration(Function &F,
                                                      Instruction *memInst,
                                                      Value *baseTag,
                                                      CallInst *pipeWriteCall) {
  SmallVector<Instruction *> createdIs;

  // The pipe writes a {address, tag} struct. Collect the store instructions
  // writing to the struct.
  SmallVector<StoreInst *, 2> storesToIdxTagStruct;
  for (auto user : pipeWriteCall->getArgOperand(0)->users()) {
    if (auto gep = dyn_cast<GetElementPtrInst>(user)) {
      for (auto userGEP : gep->users()) {
        if (auto stInstr = dyn_cast<StoreInst>(userGEP)) {
          storesToIdxTagStruct.push_back(stInstr);
          break;
        }
      }
    }
  }
  auto tagStore = storesToIdxTagStruct[0];
  auto addressStore = storesToIdxTagStruct[1];

  // Move everything needed to setup the pipe write call to memInst's location.
  pipeWriteCall->moveBefore(memInst);
  tagStore->moveBefore(pipeWriteCall);
  addressStore->moveBefore(pipeWriteCall);

  // Write the address value of the memInst into the pipeWrite address field.
  Value *memInstAddr = isaStore(memInst)
                           ? dyn_cast<StoreInst>(memInst)->getPointerOperand()
                           : dyn_cast<LoadInst>(memInst)->getPointerOperand();
  // Ensure address matches pipe idx type (we use i64).
  auto typeForAddressPipe = addressStore->getOperand(0)->getType();
  auto addressCastedToi64 = BitCastInst::CreatePointerCast(
      memInstAddr, typeForAddressPipe, "", dyn_cast<Instruction>(addressStore));
  addressStore->setOperand(0, addressCastedToi64);

  // Write baseTag into struct.idx. If it's a store, increment the tag first.
  IRBuilder<> Builder(tagStore);
  auto tagType = tagStore->getOperand(0)->getType();
  Value *baseTagVal = Builder.CreateLoad(tagType, baseTag, "baseTagVal");
  auto baseTagPlusOne = Builder.CreateAdd(
      baseTagVal, ConstantInt::get(tagType, 1), "baseTagPlus1");
  if (isaStore(memInst)) {
    tagStore->setOperand(0, baseTagPlusOne);
    auto newTagStore = Builder.CreateStore(baseTagPlusOne, baseTag);
    createdIs.push_back(newTagStore);
    createdIs.push_back(dyn_cast<Instruction>(baseTagPlusOne));
  } else {
    tagStore->setOperand(0, baseTagVal);
  }
  createdIs.push_back(tagStore);
  createdIs.push_back(addressStore);
  createdIs.push_back(dyn_cast<Instruction>(baseTagVal));

  return createdIs;
}

/// Given store/load instructions, change them to read/write from/to sycl pipes.
void transformMainKernel(Function &F, SmallVector<Instruction *> &stores,
                         SmallVector<Instruction *> &loads) {
  // Collect pipe read and pipe write instructions.
  SmallVector<CallInst *> loadPipeReadCalls(loads.size());
  SmallVector<CallInst *> storePipeWriteCalls(stores.size());
  for (size_t i = 0; i < loads.size(); ++i)
    loadPipeReadCalls[i] = getNthPipeCall(F, i);
  for (size_t i = 0; i < stores.size(); ++i)
    storePipeWriteCalls[i] = getNthPipeCall(F, i + loads.size());

  // Replace load instructions with calls to pipe::read.
  for (size_t iLoad = 0; iLoad < loads.size(); ++iLoad) {
    loadPipeReadCalls[iLoad]->moveBefore(loads[iLoad]);
    Value *loadVal = dyn_cast<Value>(loads[iLoad]);
    loadVal->replaceAllUsesWith(loadPipeReadCalls[iLoad]);
  }

  // Replace store instructions with calls to pipe::write.
  for (size_t iStore = 0; iStore < stores.size(); ++iStore) {
    storePipeWriteCalls[iStore]->moveAfter(stores[iStore]);
    stores[iStore]->setOperand(1, storePipeWriteCalls[iStore]->getOperand(0));
  }
}

/// Given an {endSignalPipeWrite} instruction, ensure it is moved before
/// every function return, and the {baseTag} value is written to its operand.
void addEndLsqSignalPipeWrite(Function &F, CallInst *endSignalPipeWrite,
                              Value *baseTag) {
  // The end signal pipe should be written to before every function exit.
  // In practice, our kernels have one exit.
  for (auto &BB : F) {
    for (auto &I : BB) {
      if (isa<ReturnInst>(I)) {
        auto thisEndSignalPipeWrite = endSignalPipeWrite->clone();
        thisEndSignalPipeWrite->insertBefore(&I);

        // Load the baseTag and store the value to the pipe operand.
        IRBuilder<> IR(thisEndSignalPipeWrite);
        LoadInst *ldBaseTag =
            IR.CreateLoad(Type::getInt32Ty(IR.getContext()), baseTag);
        Value *endSignalValuePtr = endSignalPipeWrite->getOperand(0);
        IR.CreateStore(ldBaseTag, endSignalValuePtr);
      }
    }
  }

  endSignalPipeWrite->eraseFromParent();
}

/// Given a json file name, return llvm::json::Value.
json::Value parseJsonReport() {
  if (const char *fname = std::getenv("LOOP_RAW_REPORT")) {
    std::ifstream t(fname);
    std::stringstream buffer;
    buffer << t.rdbuf();

    auto Json = json::parse(llvm::StringRef(buffer.str()));
    assert(Json && "Error parsing json loop-raw-report");

    return *Json;
  }

  assert("No LOOP_RAW_REPORT environment value.");
  return json::Value(nullptr);
}

/// Collect data hazard instructions (loads and stores) in {F}
/// given a json report from a DataHazardAnalysis pass.
SmallVector<SmallVector<Instruction *>>
collectInstructionCluster(json::Object report, Function &F) {
  /// Lambda to collect instructions from {F} given an array of descriptions.
  auto getIsFromArray = [&F](json::Array instrDescriptions)  {
    SmallVector<Instruction *> result;

    for (json::Value &iVal : instrDescriptions) {
      auto iObj = *iVal.getAsObject();
      auto bbIdx = int(iObj["basic_block_idx"].getAsInteger().getValue());
      auto instrIdx = int(iObj["instruction_idx"].getAsInteger().getValue());

      // Given indexes of children in parents, arrive at the right instruction.
      auto BB = getChildWithIndex<Function, BasicBlock>(&F, bbIdx);
      auto I = getChildWithIndex<BasicBlock, Instruction>(BB, instrIdx);

      result.push_back(I);
    }

    return result;
  };

  SmallVector<SmallVector<Instruction *>> clusters;
  // Go over each base_address json value.
  for (json::Value &baseAddr : *report["base_addresses"].getAsArray()) {
    auto baseAddrObj = *baseAddr.getAsObject();

    // Collect load store instructions for this base address (loads first).
    SmallVector<Instruction *> thisClusterIs;
    llvm::append_range(
        thisClusterIs,
        getIsFromArray(*baseAddrObj["load_instructions"].getAsArray()));
    llvm::append_range(
        thisClusterIs,
        getIsFromArray(*baseAddrObj["store_instructions"].getAsArray()));

    clusters.push_back(thisClusterIs);
  }

  return clusters;
}

struct LoadStoreQueueTransform : PassInfoMixin<LoadStoreQueueTransform> {
  json::Object report;

  /// Keeps track of the number of data hazard clusters that were processed.
  /// This is needed when transforming multiple address generation kernels,
  /// where each kernel generates addresses for a different cluster.
  int clusterIdx = 0;

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    Module *M = F.getParent();

    // Read in report once.
    if (report.empty())
      report = *(parseJsonReport().getAsObject());

    // SPIR lambda kernel functions are guaranteed to be called just once.
    auto callers = getCallerFunctions(M, F);
    if (callers.size() != 1)
      return PreservedAnalyses::all();

    std::string mainKernelName =
        std::string(report["kernel_class_name"].getAsString().getValue());
    std::string thisKernelName = demangle(std::string(callers[0]->getName()));

    if (F.getCallingConv() == CallingConv::SPIR_FUNC) {
      // The names of kernels that we need to transform are guaranteed to begin
      // with mainKernelName. NOTE: Check only mainKernelName.size() characters.
      if (std::equal(mainKernelName.begin(), mainKernelName.end(),
                     thisKernelName.begin())) {
        // Find out if this F should be an AGU or Main kernel,
        // or both if address generation is not decoupled.
        std::regex agu_kernel_regex{mainKernelName + "_AGU",
                                    std::regex_constants::ECMAScript};
        std::smatch agu_kernel_match;
        std::regex_search(thisKernelName, agu_kernel_match, agu_kernel_regex);
        bool isAddressGenKernel = agu_kernel_match.size() > 0;
        bool isDecoupledAddress =
            report["decouple_address"].getAsInteger().getValue() == 1;

        // Collect the same load/store instructions as during analysis.
        auto dataHazardClusters = collectInstructionCluster(report, F);
        // If we are processing a single AGU kernel, then we need to
        // look only at one data hazard cluster.
        auto thisClusterPtr = dataHazardClusters.begin() + clusterIdx;
        auto dataHazardRange =
            isDecoupledAddress && isAddressGenKernel
                ? llvm::make_range(thisClusterPtr, thisClusterPtr + 1)
                : dataHazardClusters;
        clusterIdx += int(isDecoupledAddress && isAddressGenKernel);

        // Keep track of instructions created during the pass.
        SmallVector<Instruction *> preserveIs;
        for (auto &dataHazards : dataHazardRange) {
          SmallVector<Instruction *> stores = getStores(dataHazards);
          SmallVector<Instruction *> loads = getLoads(dataHazards);

          if (isAddressGenKernel || !isDecoupledAddress) {
            // At AGU function entry, initialize a base tag to 0.
            IRBuilder<> Builder(F.getEntryBlock().getTerminator());
            auto tagType = Type::getInt32Ty(F.getContext());
            Value *baseTag = Builder.CreateAlloca(tagType);
            Builder.CreateStore(ConstantInt::get(tagType, 0), baseTag);

            // First deal with loads because that's the order of pipes.
            // The 0th pipe is always the next to be used.
            // The address gen transformation deals with one instruction.
            for (auto &I : llvm::concat<Instruction *>(loads, stores)) {
              auto createdIs = transformAddressGeneration(F, I, baseTag,
                                                          getNthPipeCall(F, 0));
              llvm::concat<Instruction *>(preserveIs, createdIs);
            }

            // The address tag value at the end of F will be equal to the number
            // of stores sent to the LSQ. Write this value to endLSQSignal pipe.
            addEndLsqSignalPipeWrite(F, getNthPipeCall(F, 0), baseTag);
          }

          if (!isAddressGenKernel) 
            transformMainKernel(F, stores, loads);
        }

        // In the address generation kernel, delete all side effect instructions
        // not part of our {preserveIs}. The rest will be deleted by DCE.
        auto &DT = AM.getResult<DominatorTreeAnalysis>(F);
        if (isAddressGenKernel && isDecoupledAddress) {
          // This includes the data hazard stores, so get those first.
          SmallVector<Instruction *> allStores;
          for (auto &memInstrs : dataHazardClusters) {
            for (auto &iS : getStores(memInstrs)) 
              allStores.push_back(iS);
          }

          for (auto st_i : allStores) {
            bool canDelete = llvm::find(preserveIs, st_i) == preserveIs.end();
            if (st_i->getParent() != nullptr && canDelete) {
              deleteStoresAfterI(F, DT, st_i, preserveIs);
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
    AU.addRequiredID(DominatorTreeAnalysis::ID());
  }
};

//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getLoadStoreQueueTransformPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "LoadStoreQueueTransform",
          LLVM_VERSION_STRING, [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "lsq-transform") {
                    FPM.addPass(LoadStoreQueueTransform());
                    return true;
                  }
                  return false;
                });
          }};
}

// This is the core interface for pass plugins. It guarantees that 'opt' will
// be able to recognize the pass via '-passes=stq-insert'
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getLoadStoreQueueTransformPluginInfo();
}

} // end namespace llvm
