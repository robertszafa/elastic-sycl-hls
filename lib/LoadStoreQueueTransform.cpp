#include "CommonLLVM.h"

#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/iterator_range.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace llvm {

const std::string ENVIRONMENT_VARIBALE_REPORT = "LOOP_RAW_REPORT";

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
void collectPairStructStores(const CallInst *pipeWrite, StoreInst *&tagStore,
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
SmallVector<Instruction *> instr2PipeReqChange(Function &F,
                                                  Instruction *memInst,
                                                  CallInst *pipeWrite,
                                                  Value *tagAddr) {
  SmallVector<Instruction *> createdSts;

  // The pipe writes a {address, tag} struct. 
  StoreInst *tagStore, *addressStore;
  collectPairStructStores(pipeWrite, tagStore, addressStore);

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
  Value *tagVal = Builder.CreateLoad(tagType, tagAddr, "baseTagVal");
  auto tagPlusOne = Builder.CreateAdd(tagVal, ConstantInt::get(tagType, 1));
  if (isaStore(memInst)) {
    tagStore->setOperand(0, tagPlusOne);
    auto newTagStore = Builder.CreateStore(tagPlusOne, tagAddr);
    createdSts.push_back(newTagStore);
  } else {
    tagStore->setOperand(0, tagVal);
  }

  createdSts.push_back(tagStore);
  createdSts.push_back(addressStore);

  return createdSts;
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

/// Given a store instuction, swap it for a tagged value pipe write.
SmallVector<Instruction *> stInstr2PipeTaggedValChange(Function &F,
                                                       Instruction *stI,
                                                       CallInst *pipeWrite,
                                                       Value *tagAddr) {
  // Stores into the tagged value struct.
  StoreInst *tagStore, *valStore;
  collectPairStructStores(pipeWrite, tagStore, valStore);
  auto tagType = tagStore->getOperand(0)->getType();

  IRBuilder<> IR(stI);
  LoadInst *tagVal = IR.CreateLoad(Type::getInt32Ty(IR.getContext()), tagAddr);
  auto tagPlusOne = IR.CreateAdd(tagVal, ConstantInt::get(tagType, 1));
  auto tagStoreToTag = IR.CreateStore(tagPlusOne, tagAddr);
  auto tagStoreToPipe = IR.CreateStore(tagPlusOne, tagStore->getOperand(1));
  pipeWrite->moveAfter(stI);

  // Instead of storing value to memory, store into the valStore struct member.
  stI->setOperand(1, valStore->getOperand(1));

  SmallVector<Instruction *> createdSts;
  createdSts.push_back(tagStoreToTag);
  createdSts.push_back(tagStoreToPipe);
  return createdSts;
}

/// Given an {endPipe} pipe write, ensure it is moved before
/// every function return, and the tag value is written to its operand.
void addEndSignalPipeWrite(Function &F, CallInst *endPipe, Value *tagAddr) {
  // The end signal pipe should be written to before every function exit.
  // In practice, our kernels have one exit.
  for (auto &BB : F) {
    for (auto &I : BB) {
      if (isa<ReturnInst>(I)) {
        auto thisEndSignalPipeWrite = endPipe->clone();
        thisEndSignalPipeWrite->insertBefore(&I);

        // Load the baseTag and store the value to the pipe operand.
        IRBuilder<> IR(thisEndSignalPipeWrite);
        LoadInst *ldBaseTag =
            IR.CreateLoad(Type::getInt32Ty(IR.getContext()), tagAddr);
        Value *endSignalValuePtr = endPipe->getOperand(0);
        IR.CreateStore(ldBaseTag, endSignalValuePtr);
      }
    }
  }

  endPipe->eraseFromParent();
}

/// Given {F}, insert an LSQ end_signal pipe call that carries the final {tag}
/// value, if {F} was is address gen. kernel. Also, if {F} is the main kernel,
/// add end_signal pipe writes to Mux kernels for each base address (if exist).
void addAllEndSignals(Function &F, bool isMain, json::Object &report,
                      Value *tagReqAddr, SmallVector<Value *> &tagValAddrs) {
  int i_baseAddr = 0;
  for (json::Value &baseAddr : *report["base_addresses"].getAsArray()) {
    auto baseAddrOb = *baseAddr.getAsObject();
    auto aguName = baseAddrOb["kernel_agu_name"].getAsString().getValue();

    // The kernel that sends lsq requests terminates the LSQ kernel
    // and the mux kernel for lsq store requests (if it exists).
    if (aguName == getKernelName(F)) {
      auto endLsqPipe = *baseAddrOb["pipe_end_lsq"].getAsObject();
      addEndSignalPipeWrite(F, getPipeCall(F, endLsqPipe), tagReqAddr);

      if (baseAddrOb["insert_st_mux"].getAsBoolean().getValue()) {
        auto endReqMuxPipe = *baseAddrOb["pipe_end_mux_streq"].getAsObject();
        addEndSignalPipeWrite(F, getPipeCall(F, endReqMuxPipe), tagReqAddr);
      }
    }

    // The main kernel terminates the mux kernel for store values (for each base
    // addr that has a mux).
    if (isMain && baseAddrOb["insert_st_mux"].getAsBoolean().getValue()) {
      auto endValMuxPipe = *baseAddrOb["pipe_end_mux_stval"].getAsObject();
      addEndSignalPipeWrite(F, getPipeCall(F, endValMuxPipe),
                            tagValAddrs[i_baseAddr]);
    }

    i_baseAddr++;
  }
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

/// Create an int32 val using alloca at the beginning of {F} and initialize it 
/// with {initVal}. Return its address.
Value *createTag(Function &F, int initVal = 0) {
  IRBuilder<> Builder(F.getEntryBlock().getTerminator());
  auto tagType = Type::getInt32Ty(F.getContext());
  Value *tagAddr = Builder.CreateAlloca(tagType);
  Builder.CreateStore(ConstantInt::get(tagType, initVal), tagAddr);
  return tagAddr;
}

/// Create a tag for each base_addr with a mux, or nullptr if mux not needed.
SmallVector<Value *> createTagsForStValues(json::Object &report, Function &F) {
  SmallVector<Value *> tagAddrs;
  for (auto &baseAddr : *report["base_addresses"].getAsArray()) {
    auto baseAddrOb = *baseAddr.getAsObject();
    Value *tagAddr = baseAddrOb["insert_st_mux"].getAsBoolean().getValue()
                         ? createTag(F)
                         : nullptr;
    tagAddrs.push_back(tagAddr);
  }

  return tagAddrs;
}

/// Repeat each {val} {report[num_stores]} times.
SmallVector<Value *> repeatNumStores(SmallVector<Value *> &vals,
                                     json::Object &report) {
  SmallVector<Value *> repeated;
  int i_val = 0;
  for (auto &baseAddr : *report["base_addresses"].getAsArray()) {
    auto baseAddrOb = *baseAddr.getAsObject();
    int numStores = baseAddrOb["num_stores"].getAsInteger().getValue();
    llvm::append_range(repeated, SmallVector<Value *>(numStores, vals[i_val]));
  }

  return repeated;
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
    // Snapshot of the data hazard store instructions that will be deleted.
    SmallVector<Instruction *> dataHazardStores = getAllStores(report, F);

    // Collect stores created during the which should not be deleted.
    SmallVector<Instruction *> preserveSt;
    // Need a tag if this is a kernel that writes LSQ request pipes.
    Value *tagReqAddr = createTag(F);
    // Also need a tag if this is a kernel that writes several store values.
    SmallVector<Value *> tagValAddrs = createTagsForStValues(report, F);
    // For convenience, a 1:1 mapping between stVals and rptdTagValAddrs.
    SmallVector<Value *> rptdTagValAddrs = repeatNumStores(tagValAddrs, report);

    // Given the Pipe2Inst maps, change the IR data instructions for pipe calls.
    // LSQ requests:
    for (auto &P2I : llvm::concat<Pipe2Inst>(ldReqs, stReqs)) {
      auto newIs = instr2PipeReqChange(F, P2I.second, P2I.first, tagReqAddr);
      llvm::append_range(preserveSt, newIs);
    }

    // Loads from the LSQ into the main kernel.
    for (auto &P2I : ldVals)
      ldInstr2PipeValChange(F, P2I.second, P2I.first);

    // Store values into the LSQ (or routed through a Mux).
    for (auto &P2I : stVals) {
      if (auto tagAddr = rptdTagValAddrs[std::distance(stVals.begin(), &P2I)]) {
        auto newIs =
            stInstr2PipeTaggedValChange(F, P2I.second, P2I.first, tagAddr);
        llvm::append_range(preserveSt, newIs);
      } else {
        stInstr2PipeValChange(F, P2I.second, P2I.first);
      }
    }

    // Add end signal pipe calls to LSQ (if this F writes LSQ requests) &
    // to muxes (if muxes exist).
    addAllEndSignals(F, isMain, report, tagReqAddr, tagValAddrs);

    // In the address generation kernel, delete all side effect instructions
    // not part of our {preserveIs}. The rest will be deleted by DCE.
    if (!isMain) {
      deleteInstructionsInAGU(F, AM.getResult<DominatorTreeAnalysis>(F), 
                              dataHazardStores, preserveSt);
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
