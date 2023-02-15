#include "CommonLLVM.h"

#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/iterator_range.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace llvm {

const std::string ENVIRONMENT_VARIBALE_REPORT = "LOOP_RAW_REPORT";

/// Delete {inst} from it's function.
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

/// Delete {storesToDel} in F and any store instruction dominated by all
/// instructions in {storesToDel}. Don't delete instructions from {exceptions.}
void deleteInstructionsInAGU(Function &F, DominatorTree &DT,
                             SmallVector<Instruction *> &storesToDel,
                             SmallVector<Instruction *> &exceptions) {
  for (auto st_i : storesToDel) {
    bool canDelete = llvm::find(exceptions, st_i) == exceptions.end();
    if (st_i->getParent() != nullptr && canDelete) {
      deleteStoresAfterI(F, DT, st_i, exceptions);
      deleteInstruction(st_i);
    }
  }
}

/// Given a {pipeWrite} writing to a LSQ request, collect the stores to the
/// address field and tag field of the LSQ request.
void collectLsqReqStructStores(const CallInst *pipeWrite, StoreInst *&tagStore,
                               StoreInst *&addressStore) {
  SmallVector<StoreInst *, 2> storesToLsqReqStruct;
  for (auto user : pipeWrite->getArgOperand(0)->users()) {
    if (auto gep = dyn_cast<GetElementPtrInst>(user)) {
      for (auto userGEP : gep->users()) {
        if (auto stInstr = dyn_cast<StoreInst>(userGEP)) {
          storesToLsqReqStruct.push_back(stInstr);
          break;
        }
      }
    }
  }

  tagStore = storesToLsqReqStruct[0];
  addressStore = storesToLsqReqStruct[1];
}

/// Given a memory instruction (load or store), change it to a sycl pipe write
/// instruction, where the value being written is the load/store address.
/// Also, if the memInstr is a store, then increment the value at {baseTagAddr}.
SmallVector<Instruction *> instr2PipeLsqReqChange(Function &F,
                                                  Instruction *memInst,
                                                  CallInst *pipeWrite,
                                                  Value *baseTag) {
  SmallVector<Instruction *> createdIs;

  // The pipe writes a {address, tag} struct. 
  StoreInst *tagStore, *addressStore;
  collectLsqReqStructStores(pipeWrite, tagStore, addressStore);

  // Move everything needed to setup the pipe write call to memInst's location.
  pipeWrite->moveBefore(memInst);
  tagStore->moveBefore(pipeWrite);
  addressStore->moveBefore(pipeWrite);

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

/// Given a load instruction, swap it for a pipe read.
void ldInstr2PipeValChange(Function &F, Instruction *ldI, CallInst *pipeRead) {
  pipeRead->moveBefore(ldI);
  Value *loadVal = dyn_cast<Value>(ldI);
  loadVal->replaceAllUsesWith(pipeRead);
}

/// Given a store instuction, swap it for a pipe write.
void stInstr2PipeValChange(Function &F, Instruction *stI, CallInst *pipeWrite) {
  pipeWrite->moveAfter(stI);
  // Instead of storing to memory, store into the pipe.
  stI->setOperand(1, pipeWrite->getOperand(0));
}

/// Given an {endLsqPipe} pipe write, ensure it is moved before
/// every function return, and the {baseTag} value is written to its operand.
void addEndLsqPipeWrite(Function &F, CallInst *endLsqPipe, Value *baseTag) {
  // The end signal pipe should be written to before every function exit.
  // In practice, our kernels have one exit.
  for (auto &BB : F) {
    for (auto &I : BB) {
      if (isa<ReturnInst>(I)) {
        auto thisEndSignalPipeWrite = endLsqPipe->clone();
        thisEndSignalPipeWrite->insertBefore(&I);

        // Load the baseTag and store the value to the pipe operand.
        IRBuilder<> IR(thisEndSignalPipeWrite);
        LoadInst *ldBaseTag =
            IR.CreateLoad(Type::getInt32Ty(IR.getContext()), baseTag);
        Value *endSignalValuePtr = endLsqPipe->getOperand(0);
        IR.CreateStore(ldBaseTag, endSignalValuePtr);
      }
    }
  }

  endLsqPipe->eraseFromParent();
}

/// Collect mappings between instructions (loads and stores) in {F}, and sycl
/// pipe call instructions, based on the info from the json report.
void collectPipe2InstMappings(json::Object &report, Function &F,
                              bool isMainKernel,
                              SmallVector<Pipe2Inst> &ldReqMaps,
                              SmallVector<Pipe2Inst> &ldValMaps,
                              SmallVector<Pipe2Inst> &stReqMaps,
                              SmallVector<Pipe2Inst> &stValMaps) {
  // Go over each base_address json value, and collect mappings. For the address
  // gen kernel (AGU), we only need ld/st request pipes. Note that if the
  // address generation is not decoupled, then {kernel_agu_name} ==
  // {main_kernel_name}, and the mappings will also be collected.
  for (json::Value &baseAddr : *report["base_addresses"].getAsArray()) {
      auto baseAddrOb = *baseAddr.getAsObject();
      // Match this {F} to a specific base_address json object.
      auto aguName = baseAddrOb["kernel_agu_name"].getAsString().getValue();
      if (aguName != getKernelName(F))
        continue;

      llvm::append_range(
          ldReqMaps,
          getPipe2InstMaps(F, *baseAddrOb["pipes_ld_req"].getAsArray()));
      llvm::append_range(
          stReqMaps,
          getPipe2InstMaps(F, *baseAddrOb["pipes_st_req"].getAsArray()));
  }

  // The main kernel gets the value pipes.
  if (isMainKernel) {
      for (json::Value &baseAddr : *report["base_addresses"].getAsArray()) {
        auto baseAddrOb = *baseAddr.getAsObject();
        llvm::append_range(
            stValMaps,
            getPipe2InstMaps(F, *baseAddrOb["pipes_st_val"].getAsArray()));
        llvm::append_range(
            ldValMaps,
            getPipe2InstMaps(F, *baseAddrOb["pipes_ld_val"].getAsArray()));
      }
  }
}

/// Return all store instructions for all base addresses in the json {report}.
SmallVector<Instruction *> getAllStores(json::Object &report, Function &F) {
  SmallVector<Instruction *> result;
  for (json::Value &baseAddr : *report["base_addresses"].getAsArray()) {
    auto baseAddrOb = *baseAddr.getAsObject();
    for (json::Value &storeI : *baseAddrOb["store_instructions"].getAsArray())
      result.push_back(getInstruction(F, *storeI.getAsObject()));
  }

  return result;
}

struct LoadStoreQueueTransform : PassInfoMixin<LoadStoreQueueTransform> {
  json::Object report;

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    // Read in report once.
    if (report.empty())
      report = *parseJsonReport(ENVIRONMENT_VARIBALE_REPORT).getAsObject();
    
    // Determine if this is our original kernel, an AGU kernel, or neither.
    std::string thisKernelName = getKernelName(F);
    auto mainKernel = std::string(report["kernel_name"].getAsString().getValue());
    bool isMain = mainKernel == thisKernelName;
    bool isOurKernel = std::equal(mainKernel.begin(), mainKernel.end(),
                                  thisKernelName.begin());
    if (F.getCallingConv() != CallingConv::SPIR_FUNC || !isOurKernel) {
      return PreservedAnalyses::all();
    }

    // Collect mappings between ld/st instructions and the pipe read/writes that
    // will replace them. Also collect all hazard strores for any base address.
    SmallVector<Pipe2Inst> ldReqs, ldVals, stReqs, stVals;
    collectPipe2InstMappings(report, F, isMain, ldReqs, ldVals, stReqs, stVals);
    SmallVector<Instruction *> dataHazardStores = getAllStores(report, F);

    // IR builder for the address tag.
    IRBuilder<> Builder(F.getEntryBlock().getTerminator());
    auto tagType = Type::getInt32Ty(F.getContext());
    Value *baseTag = Builder.CreateAlloca(tagType);
    Builder.CreateStore(ConstantInt::get(tagType, 0), baseTag);

    // Collect instructions which manipulate the address tag inserted into {F}.
    SmallVector<Instruction *> preserveIs;

    // Given the Pipe2Inst maps, change the IR data instructions for pipe calls.
    for (auto &P2I : ldReqs) {
      llvm::append_range(preserveIs, instr2PipeLsqReqChange(
                                         F, P2I.second, P2I.first, baseTag));
    }
    for (auto &P2I : stReqs) {
      llvm::append_range(preserveIs, instr2PipeLsqReqChange(
                                         F, P2I.second, P2I.first, baseTag));
    }
    for (auto &P2I : ldVals)
      ldInstr2PipeValChange(F, P2I.second, P2I.first);
    for (auto &P2I : stVals)
      stInstr2PipeValChange(F, P2I.second, P2I.first);

    // The lsq_end_signal pipes should be in the same kernel as the lsq reqs.
    for (json::Value &baseAddr : *report["base_addresses"].getAsArray()) {
      auto baseAddrOb = *baseAddr.getAsObject();
      auto aguName = baseAddrOb["kernel_agu_name"].getAsString().getValue();
      auto endPipeObj = *baseAddrOb["pipe_end_lsq"].getAsObject();

      if (aguName == thisKernelName) {
        addEndLsqPipeWrite(F, getPipeCall(F, endPipeObj), baseTag);
      }
    }

    // In the address generation kernel, delete all side effect instructions
    // not part of our {preserveIs}. The rest will be deleted by DCE.
    if (!isMain) {
      deleteInstructionsInAGU(F, AM.getResult<DominatorTreeAnalysis>(F), 
                              dataHazardStores, preserveIs);
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
