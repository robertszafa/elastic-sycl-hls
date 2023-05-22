#include "CommonLLVM.h"

#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/iterator_range.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace llvm {

/// The pipe operations for the values at the start and end of a recurrence
/// which is calculated only in a decoupled basic block will be hoisted out.
struct HoistPipeDirective {
  /// For each hoisted out pipe read, there will also be a hoisted out pipe write
  /// forming the start and end of a recurrence which was decoupled into a PE.
  CallInst *read; 
  CallInst *write;
  Loop *loop;
  /// The initital value of the recurrence.
  Value *initVal;
};

const std::string REPORT_ENV_NAME = "ELASTIC_PASS_REPORT";

// Delete all side store instructions in F (except exceptions).
void deleteAllStores(Function &F, SmallVector<Instruction *> &exceptions) {
  // First collect then delete. Cannot delete I when iterating over I's in BB.
  SmallVector<Instruction *> toDelete;
  for (auto &BB : F) {
    for (auto &I : BB) {
      bool notException = llvm::find(exceptions, &I) == exceptions.end();
      if (I.getParent() != nullptr && notException && isaStore(&I)) 
        toDelete.push_back(&I);
    }
  }
  for (auto &I : toDelete)
    deleteInstruction(I);
}

/// Given a {pipeWrite} with a struct as its operand, collect the stores to the
/// all struct fields of the LSQ request.
SmallVector<StoreInst *> getPipeOpStructStores(const CallInst *pipeWrite) {
  SmallVector<StoreInst *> storesToStruct;
  for (auto user : pipeWrite->getArgOperand(0)->users()) {
    if (auto gep = dyn_cast<GetElementPtrInst>(user)) {
      for (auto userGEP : gep->users()) {
        if (auto stInstr = dyn_cast<StoreInst>(userGEP)) {
          storesToStruct.push_back(stInstr);
          break;
        }
      }
    }
  }

  // The struct stores were collected in reverse above.
  std::reverse(storesToStruct.begin(), storesToStruct.end());
  return storesToStruct;
}

void instr2PipeLsqLdReq(Instruction *I, CallInst *pipeWrite, Value *stTagAddr,
                        Value *ldTagAddr, bool isOnChipMem, bool isLdTagNeeded,
                        SmallVector<Instruction *> &preserveStores) {
  // The pipe writes a {address, tag} struct.
  auto reqStructStores = getPipeOpStructStores(pipeWrite);
  StoreInst *addressStore = reqStructStores[0];
  StoreInst *stTagStore = reqStructStores[1];
  StoreInst *ldTagStore = reqStructStores[2];
  auto addrType = addressStore->getOperand(0)->getType();
  auto tagType = stTagStore->getOperand(0)->getType();

  // Move everything needed to setup the pipe write call to memInst's location.
  pipeWrite->moveBefore(I);
  stTagStore->moveBefore(pipeWrite);
  addressStore->moveBefore(pipeWrite);

  Value *memInstAddr = dyn_cast<LoadInst>(I)->getPointerOperand();

  // Write address.
  if (isOnChipMem) {
    // For BRAM accesses, we only get the index value, not the full pointer.
    auto gepAddr = dyn_cast<GetElementPtrInst>(memInstAddr);
    memInstAddr = gepAddr->getOperand(gepAddr->getNumOperands() - 1);
    if (auto sextI = dyn_cast<SExtInst>(memInstAddr)) 
      memInstAddr = sextI->getOperand(0);
  }
  auto addressCasted = BitCastInst::CreateBitOrPointerCast(
      memInstAddr, addrType, "", dyn_cast<Instruction>(addressStore));
  addressStore->setOperand(0, addressCasted);

  // Write tag.
  IRBuilder<> Builder(stTagStore);
  Value *stTagVal = Builder.CreateLoad(tagType, stTagAddr, "baseStTagVal");
  stTagStore->setOperand(0, stTagVal);

  // Optionally, write load tag.
  if (isLdTagNeeded) {
    // A load tag is first used in the load request.
    ldTagStore->moveBefore(pipeWrite);
    Builder.SetInsertPoint(ldTagStore);
    Value *ldTagVal = Builder.CreateLoad(tagType, ldTagAddr, "baseLdTagVal");
    ldTagStore->setOperand(0, ldTagVal);
    // And then incremented.
    Builder.SetInsertPoint(pipeWrite);
    auto ldTagInc = Builder.CreateAdd(ldTagVal, ConstantInt::get(tagType, 1));
    auto storeForNewLdTag = Builder.CreateStore(ldTagInc, ldTagAddr);
    preserveStores.push_back(storeForNewLdTag);
  }

  preserveStores.push_back(stTagStore);
  preserveStores.push_back(addressStore);
}

void instr2PipeLsqStReq(Instruction *I, CallInst *pipeWrite, Value *stTagAddr,
                        bool isOnChipMem,
                        SmallVector<Instruction *> &preserveStores) {
  // The pipe writes a {address, tag} struct.
  auto reqStructStores = getPipeOpStructStores(pipeWrite);
  StoreInst *addressStore = reqStructStores[0];
  StoreInst *stTagStore = reqStructStores[1];
  auto addrType = addressStore->getOperand(0)->getType();
  auto tagType = stTagStore->getOperand(0)->getType();

  // Move everything needed to setup the pipe write call to memInst's location.
  pipeWrite->moveBefore(I);
  stTagStore->moveBefore(pipeWrite);
  addressStore->moveBefore(pipeWrite);

  // Write address.
  Value *memInstAddr = dyn_cast<StoreInst>(I)->getPointerOperand();
  if (isOnChipMem) {
    // For BRAM accesses, we only get the index value, not the full pointer.
    auto gepAddr = dyn_cast<GetElementPtrInst>(memInstAddr);
    memInstAddr = gepAddr->getOperand(gepAddr->getNumOperands() - 1);
    if (auto sextI = dyn_cast<SExtInst>(memInstAddr)) 
      memInstAddr = sextI->getOperand(0);
  }
  auto addressCasted = BitCastInst::CreateBitOrPointerCast(
      memInstAddr, addrType, "", dyn_cast<Instruction>(addressStore));
  addressStore->setOperand(0, addressCasted);

  // Write tag. Since it's a store, increment the tag first.
  IRBuilder<> Builder(stTagStore);
  Value *stTagVal = Builder.CreateLoad(tagType, stTagAddr, "baseStTagVal");
  auto stTagInc = Builder.CreateAdd(stTagVal, ConstantInt::get(tagType, 1));
  stTagStore->setOperand(0, stTagInc);
  auto storeForNewStTag = Builder.CreateStore(stTagInc, stTagAddr);

  preserveStores.push_back(storeForNewStTag);
  preserveStores.push_back(stTagStore);
  preserveStores.push_back(addressStore);
}

/// Given a load instruction, swap it for a pipe read.
void instr2PipeLdVal(Instruction *I, CallInst *pipeRead) {
  pipeRead->moveBefore(I);
  Value *loadVal = dyn_cast<Value>(I);
  loadVal->replaceAllUsesWith(pipeRead);
}

/// Given a store instuction, swap it for a pipe write.
void instr2PipeStVal(Instruction *I, CallInst *pipeWrite) {
  pipeWrite->moveAfter(I);
  // Instead of storing to memory, store into the pipe.
  I->setOperand(1, pipeWrite->getOperand(0));
}

/// Given a store instuction, swap it for a tagged value pipe write.
void instr2PipeStValTagged(Instruction *I, CallInst *pipeWrite, Value *tagAddr, 
                           SmallVector<Instruction *> &toKeep) {
  // Stores into the tagged value struct.
  auto taggedValStructStores = getPipeOpStructStores(pipeWrite);
  StoreInst *valStore = taggedValStructStores[0];
  StoreInst *tagStore = taggedValStructStores[1];
  auto tagType = tagStore->getOperand(0)->getType();

  IRBuilder<> IR(I);
  LoadInst *tagVal = IR.CreateLoad(tagType, tagAddr);
  auto tagPlusOne = IR.CreateAdd(tagVal, ConstantInt::get(tagType, 1));
  auto tagStoreToTag = IR.CreateStore(tagPlusOne, tagAddr);
  auto tagStoreToPipe = IR.CreateStore(tagPlusOne, tagStore->getOperand(1));
  pipeWrite->moveAfter(I);

  // Instead of storing value to memory, store into the valStore struct member.
  I->setOperand(1, valStore->getOperand(1));

  toKeep.push_back(tagStoreToTag);
  toKeep.push_back(tagStoreToPipe);
}

/// Given {F}, insert an LSQ end_signal pipe call that carries the final {tag}
/// value, if {F} was is address gen. kernel. Also, if {F} is the main kernel,
/// add end_signal pipe writes to Mux kernels for each base address (if exist).
void moveEndLsqSignalToReturnBB(Function &F, CallInst *pipeWrite) {
  // The end signal pipe should be at the end of the function exit BB.
  // After the -merge-return pass, there will be only one such BB.
  for (auto &BB : F) {
    for (auto &I : BB) {
      if (isa<ReturnInst>(&I)) {
        pipeWrite->moveBefore(&I);
        return;
      }
    }
  }
}

void instr2PipeSsaPe(Instruction *I, CallInst *pipeCall, json::Object &i2pInfo, 
                     SmallVector<Instruction *> &toKeep) {
  auto decoupledBB = getChildWithIndex<Function, BasicBlock>(
      I->getFunction(), *i2pInfo.getInteger("pe_basic_block_idx"));

  // In PE reads happen at start of the decoupled BB and writes at the end.
  // Hoisted out reads happend in function entry. Hoisted writes in exit.
  if (*i2pInfo.getString("read/write") == "read") {
    pipeCall->moveBefore(&decoupledBB->front());
    I->replaceAllUsesWith(pipeCall);
  } else {
    pipeCall->moveBefore(decoupledBB->getTerminator());
    toKeep.push_back(storeValIntoPipe(I, pipeCall));
  } 
}

void instr2PipeSsaMain(Instruction *I, CallInst *pipeCall,
                       json::Object &i2pInfo,
                       SmallVector<Instruction *> &toKeep) {
  auto decoupledBB = getChildWithIndex<Function, BasicBlock>(
      I->getFunction(), *i2pInfo.getInteger("pe_basic_block_idx"));

  if (*i2pInfo.getString("read/write") == "read") {
    // Insert at the end of the BB or before a store that uses I.
    Instruction *insertPtEnd = decoupledBB->getTerminator();
    for (auto userOfI : I->users()) {
      if (auto useI = dyn_cast<Instruction>(userOfI)) {
        if (useI->getParent() == decoupledBB) {
          insertPtEnd = useI;
          break;
        }
      }
    }
    pipeCall->moveBefore(insertPtEnd);
    I->replaceAllUsesWith(pipeCall);
  } else {
    // Insert at the start of the BB or after a load if I is a load.
    Instruction *insertPtStart =
        isaLoad(I) && I->getParent() == decoupledBB ? I : &decoupledBB->front();
    pipeCall->moveAfter(insertPtStart);
    toKeep.push_back(storeValIntoPipe(I, pipeCall));
  }
}

/// Delete all instrucions in {BB} except the {exceptions}.
void deleteInstrsInPe(BasicBlock *BB, SmallVector<Instruction *> exceptions) {
  SmallVector<Instruction *> toRemove;
  for (auto &I : *BB) {
    // Also check if not a pipe call. We might have moved some pipe calls out of
    // the decoupled BB during hoisting.
    if (!llvm::is_contained(exceptions, &I) && !getPipeCall(&I))
      toRemove.push_back(&I);
  }

  for (auto I : toRemove) 
    deleteInstruction(I);
}

/// Given a {BB}, wrap it into a while (predPipe::read()) { BB } code structure.
/// Remove the rest of the BBs in {F} - this is guaranteed to be save since all
/// in/out def-use SSA values in {BB} have been replaced by pipe reads/writes.
void createPredicatedPE(CallInst *predPipeRead, json::Object &i2pInfo) {
  auto F = predPipeRead->getFunction();
  auto decoupledBB = getChildWithIndex<Function, BasicBlock>(
      F, *i2pInfo.getInteger("basic_block_idx"));
    
  // There will be 4 blocks left in a predicated PE. BB 0: F.entry.
  // Collect instructions to delete in the 4 blocks as we go along.
  SmallVector<Instruction *> instrsToDelete;

  // BB 1: The return BB only needs the ret instruction.
  auto returnBB = getReturnBlock(*F);
  for (auto &I : *returnBB)
    if (!I.isTerminator())
      instrsToDelete.push_back(&I);

  // BB 2: The loop header block reads from the predicate pipe and, based on the
  // condition, exits or branches to {BB}: while (predPipe::read()) {BB}
  auto headerBB = F->getEntryBlock().getSingleSuccessor();
  IRBuilder<> Builder(headerBB->getTerminator());
  auto loopBranch = Builder.CreateCondBr(predPipeRead, decoupledBB, returnBB);
  predPipeRead->moveBefore(loopBranch);
  for (auto &I : *headerBB)
    if (&I != predPipeRead && &I != loopBranch)
      instrsToDelete.push_back(&I);

  // BB 3: In the {BB} block, create a backedge to the loop header BB.
  auto oldBranch = decoupledBB->getTerminator();
  Builder.SetInsertPoint(oldBranch);
  Builder.CreateBr(headerBB);
  instrsToDelete.push_back(oldBranch);

  // Delete not needed instructions and basic blocks.
  auto keepBlocks = {&F->getEntryBlock(), decoupledBB, returnBB, headerBB};
  SmallVector<BasicBlock *> blocksToDelete;
  for (auto &BB : *F) {
    if (!llvm::is_contained(keepBlocks, &BB)) {
      blocksToDelete.push_back(&BB);
      for (auto &I : BB) 
        instrsToDelete.push_back(&I);
    }
  }
  for (auto &I : instrsToDelete) 
    deleteInstruction(I);
  for (auto BB : blocksToDelete) 
    BB->removeFromParent();
}

void addPredicateWrites(CallInst *predPipeWrite, json::Object &i2pInfo,
                        LoopInfo &LI, SmallVector<Instruction *> &toKeep) {
  auto decoupledBB = getChildWithIndex<Function, BasicBlock>(
      predPipeWrite->getFunction(), *i2pInfo.getInteger("basic_block_idx"));
  auto predType = Type::getInt8Ty(predPipeWrite->getContext());

  // In decoupled BB to invoke predicated PE.
  predPipeWrite->moveBefore(decoupledBB->getFirstNonPHI());
  auto stTrue = storeValIntoPipe(ConstantInt::get(predType, 1), predPipeWrite);

  // At end of loop, terminate predicated PE.
  auto loopExitBB = LI.getLoopFor(decoupledBB)->getExitBlock();
  auto predPipeClone = dyn_cast<CallInst>(predPipeWrite->clone());
  predPipeClone->insertBefore(&loopExitBB->front());
  auto stFalse = storeValIntoPipe(ConstantInt::get(predType, 0), predPipeClone);

  toKeep.push_back(stTrue);
  toKeep.push_back(stFalse);
}

/// Create an int32 val using alloca at the beginning of {F} and initialize it 
/// with {initVal}. Return its address.
Value *createTag(Function &F, uint initVal = 0) {
  IRBuilder<> Builder(F.getEntryBlock().getTerminator());
  // LLVM doesn't make a distinction between signed and unsigned. And the only
  // op done on tags is 2's complement addition, so the bit pattern is the same.
  auto tagType = Type::getInt32Ty(F.getContext());
  Value *tagAddr = Builder.CreateAlloca(tagType);
  Builder.CreateStore(ConstantInt::get(tagType, initVal), tagAddr);
  return tagAddr;
}

void hoistPipesPE(MapVector<Instruction *, HoistPipeDirective> &pipesToHoist,
                  LoopInfo &LI) {
  for (auto mapVal : pipesToHoist) {
    auto pipeRead = mapVal.second.read;
    auto pipeWrite = mapVal.second.write;
    auto pipeWriteStore = pipeWrite->getPrevNode();
    auto pipeWriteStoreOperand = pipeWriteStore->getOperand(0);
    auto decoupledBB = pipeWrite->getParent();
    auto loopHeader = decoupledBB->getPrevNode();
    auto F = decoupledBB->getParent();

    pipeRead->moveBefore(F->getEntryBlock().getTerminator());
    auto recPhi = PHINode::Create(pipeRead->getType(), 2, "hoistedRecurrence",
                                  &loopHeader->front());
    recPhi->addIncoming(pipeRead, pipeRead->getParent());
    recPhi->addIncoming(pipeWriteStoreOperand, decoupledBB);
    pipeRead->replaceUsesOutsideBlock(recPhi, loopHeader);

    pipeWrite->moveBefore(&getReturnBlock(*F)->front());
    pipeWriteStore->moveBefore(pipeWrite);
    pipeWriteStore->setOperand(0, recPhi);
  }
}

void hoistPipesMain(MapVector<Instruction *, HoistPipeDirective> &pipesToHoist,
                    LoopInfo &LI) {
  for (auto mapVal : pipesToHoist) {
    auto recStartI = mapVal.first;
    auto pipeRead = mapVal.second.read;
    auto pipeWrite = mapVal.second.write;
    auto L = mapVal.second.loop;
    auto initVal = mapVal.second.initVal;

    // Supply initital value to the PE before entering loop. Need to delete the
    // previously created store into the pipe from the decoupledBB.
    auto decoupledBB = pipeWrite->getParent();
    Instruction *toDeleteStore;
    for (auto &I : *decoupledBB) {
      if (auto stI = dyn_cast<StoreInst>(&I)) {
        if (stI->getOperand(1) == pipeWrite->getOperand(0))
          toDeleteStore = stI;
      }
    }
    pipeWrite->moveBefore(L->getLoopPreheader()->getTerminator());
    storeValIntoPipe(initVal, pipeWrite);
    deleteInstruction(toDeleteStore);
    
    pipeRead->moveBefore(L->getExitBlock()->getTerminator());
    recStartI->replaceAllUsesWith(pipeRead);
    if (auto recEnd = dyn_cast<Instruction>(recStartI->DoPHITranslation(
            recStartI->getParent(), L->getLoopLatch()))) {
      deleteInstruction(recEnd);
    }
    deleteInstruction(recStartI);
  }
}

/// Return instructions in loop L which use instruction I, including I itself.
SmallVector<Instruction *> getLoopUsesOfI(Instruction *I, Loop *L) {
  SmallVector<Instruction *> stack {I};
  SmallVector<Instruction *> done;
  while (stack.size() > 0) {
    auto curr = stack.pop_back_val();
    if (!L->contains(curr))
      continue;

    done.push_back(curr);

    // Check if more possible users by pusing users of I onto the stack. 
    // If I is a PHINode, then we push the incoming values.
    if (auto currPhi = dyn_cast<PHINode>(curr)) {
      for (auto &phiVal : currPhi->incoming_values()) {
        if (auto phiValI = dyn_cast<Instruction>(phiVal)) {
          if (!llvm::is_contained(done, phiValI))
            stack.push_back(phiValI);
        }
      }
    } else {
      for (auto userOfI : I->users()) {
        if (auto userI = dyn_cast<Instruction>(userOfI)) {
          if (!llvm::is_contained(done, userI))
            stack.push_back(userI);
        }
      }
    }
  }

  return done;
}

MapVector<Instruction *, HoistPipeDirective>
getPipesToHoist(Function &F, LoopInfo &LI, json::Array &directives) {
  MapVector<Instruction *, HoistPipeDirective> resMap;

  json::Array hoistDirectives;
  for (json::Value &i2pInfoVal : directives) {
    auto i2pInfo = *i2pInfoVal.getAsObject();
    auto directiveType = i2pInfo.getString("directive_type");
    // Only pipes for ssa directives can be hoisted out of a loop.
    if (directiveType != "ssa")
      continue;
    
    auto pipeCall =
        reinterpret_cast<CallInst *>(*i2pInfo.getInteger("llvm_pipe_call"));
    auto I = reinterpret_cast<Instruction *>(
        *i2pInfo.getInteger("llvm_instruction"));
    auto decoupledBB = getChildWithIndex<Function, BasicBlock>(
        &F, *i2pInfo.getInteger("pe_basic_block_idx"));
    auto L = LI.getLoopFor(decoupledBB);

    // To be hoisted out, I must be an instructions in the loop sequence L:
    // [loop header: phi0] --> [decoupledBB: useI] --> [loop latch: phi1] --
    //          ^----------------------------------------------------------|
    // - if phi0 is used in L, then it should be only in decupledBB in the PE. 
    //   It can't be a store op in decupledBB, but can be used in other loops.
    // - phi1 will only be used by a phi in the header by definition, so ignore.
    auto allowedBlocks = {L->getHeader(), decoupledBB, L->getLoopLatch()};
    auto loopUsesOfI = getLoopUsesOfI(I, L);
    bool canBeHoisted = true;
    Instruction *recStart;
    for (auto useOfI : loopUsesOfI) {
      auto useBB = useOfI->getParent();
      if (!llvm::is_contained(allowedBlocks, useBB) ||
          !(isaStore(useOfI) && useBB == decoupledBB)) {
        canBeHoisted = false;
      }

      if (useOfI->getParent() == L->getHeader())
        recStart = useOfI;
    }

    if (canBeHoisted) {
      resMap[recStart].loop = L;
      // The value incoming into the start of the loop recurrence.
      resMap[recStart].initVal =
          recStart->DoPHITranslation(L->getHeader(), L->getLoopPreheader());

      if (*i2pInfo.getString("read/write") == "read") 
        resMap[recStart].read = pipeCall;
      else 
        resMap[recStart].write = pipeCall;
    }
  }

  return resMap;
}

SmallVector<Instruction *>
getInstructionsToDecouple(Function &F, const json::Object &report) {
  SmallVector<Instruction *> res;

  for (auto blockInfoVal : *report.getArray("blocks_to_decouple")) {
    auto blockInfo = *blockInfoVal.getAsObject();
    for (auto iInfoVal : *blockInfo.getArray("decoupled_instructions")) {
      res.push_back(getInstruction(F, *iInfoVal.getAsObject()));
    }
  }

  return res;
}

/// For every instruction expressed in json, update the json with a pointer to
/// the LLVM instruction object in F.
json::Array getDirectives(Function &F, const json::Object &report) {
  std::string thisKernelName = demangle(std::string(F.getNameOrAsOperand()));

  json::Array directives;
  for (auto i2pInfoVal : *report.getArray("instr2pipe_directives")) {
    auto i2pInfo = *i2pInfoVal.getAsObject();
    if (i2pInfo.getString("kernel_name") != thisKernelName) 
      continue;

    // Each directive has a guaranteed pipe call.
    i2pInfo["llvm_pipe_call"] =
        reinterpret_cast<std::intptr_t>(getPipeCall(F, i2pInfo));

    // Not every directive has an instruction, e.g. end_signal directives.
    if (auto optI = i2pInfo.getObject("instruction")) {
      i2pInfo["llvm_instruction"] =
          reinterpret_cast<std::intptr_t>(getInstruction(F, *optI));
    }

    // If a PE directive specifies that a given pipe call needs to be hoisted
    // out of the loop, then we need the start and end recurrence instructions.
    if (auto optI = i2pInfo.getObject("recurrence_start_instruction")) {
      i2pInfo["llvm_recurrence_start_instruction"] =
          reinterpret_cast<std::intptr_t>(getInstruction(F, *optI));
    }
    if (auto optI = i2pInfo.getObject("recurrence_end_instruction")) {
      i2pInfo["llvm_recurrence_end_instruction"] =
          reinterpret_cast<std::intptr_t>(getInstruction(F, *optI));
    }

    directives.push_back(std::move(i2pInfo));
  }

  return directives;
}

struct ElasticTransform : PassInfoMixin<ElasticTransform> {
  json::Object report;

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    // Read in report once.
    if (report.empty())
      report = *parseJsonReport(REPORT_ENV_NAME).getAsObject();

    // Only transform if this is the main kernel, an AGU or a PE.
    std::string thisKernelName = demangle(std::string(F.getNameOrAsOperand()));
    auto mainKernelName = *report["main_kernel_name"].getAsString();
    bool isMain = mainKernelName == thisKernelName;
    bool isAGU = thisKernelName.find("_AGU_") < thisKernelName.size();
    bool isPE = thisKernelName.find("_PE_") < thisKernelName.size();
    if (F.getCallingConv() != CallingConv::SPIR_KERNEL ||
        !(isMain || isAGU || isPE)) {
      return PreservedAnalyses::all();
    }

    // Each loop is in Loop Simplify Form -- preheader, header, body, latch,
    // exit blocks. We use these blocks to insert pipe calls.
    auto &LI = AM.getResult<LoopAnalysis>(F);

    // Instruction-2-pipe transformation directives for this function. 
    json::Array directives = getDirectives(F, report);
    // A mapping between recurrence start instructions and pipes to hoist out.
    MapVector<Instruction *, HoistPipeDirective> pipesToHoist =
        getPipesToHoist(F, LI, directives);
    // Instructions decoupled out of the main kernel into a predicated PE.
    SmallVector<Instruction *> decoupledI = getInstructionsToDecouple(F, report);
    // Tags for ordering of LSQ reqeusts and values. Not necessarily used.
    auto lsqStValTag = createTag(F);
    auto lsqStReqTag = createTag(F);
    auto lsqLdReqTag = createTag(F);

    // Record instructions during transformation, which shouldn't be deleted.
    SmallVector<Instruction *> toKeep;

    // Process all instruction-to-pipe directives for this function.
    for (json::Value &i2pInfoVal : directives) {
      auto i2pInfo = *i2pInfoVal.getAsObject();
      auto directiveType = i2pInfo.getString("directive_type");
      auto pipeCall =
          reinterpret_cast<CallInst *>(*i2pInfo.getInteger("llvm_pipe_call"));
      auto optI = i2pInfo.getInteger("llvm_instruction");
      auto I = optI ? reinterpret_cast<Instruction *>(*optI) : nullptr;

      // LSQ related: 
      if (directiveType == "ld_val") {
        instr2PipeLdVal(I, pipeCall);
      } else if (directiveType == "st_val") {
        instr2PipeStVal(I, pipeCall);
      } else if (directiveType == "st_val_tagged") {
        instr2PipeStValTagged(I, pipeCall, lsqStValTag, toKeep);
      } else if (directiveType == "ld_req") {
        bool lddTagNeeded = (*i2pInfo.getInteger("max_seq_in_bb") > 1);
        bool onChip = (*i2pInfo.getString("pipe_type") == "ld_req_lsq_bram_t");
        instr2PipeLsqLdReq(I, pipeCall, lsqStReqTag, lsqLdReqTag, onChip,
                           lddTagNeeded, toKeep);
      } else if (directiveType == "st_req") {
        bool onChip = (*i2pInfo.getString("pipe_type") == "st_req_lsq_bram_t");
        instr2PipeLsqStReq(I, pipeCall, lsqStReqTag, onChip, toKeep);
      } else if (directiveType == "end_lsq_signal") {
        moveEndLsqSignalToReturnBB(F, pipeCall);
      }
      // Predicated PE related:
      else if (directiveType == "ssa" && isPE) {
        instr2PipeSsaPe(I, pipeCall, i2pInfo, toKeep);
      } else if (directiveType == "ssa" && isMain) {
        instr2PipeSsaMain(I, pipeCall, i2pInfo, toKeep);
      } else if (directiveType == "pred" && isPE) {
        createPredicatedPE(pipeCall, i2pInfo);
      } else if (directiveType == "pred" && isMain) {
        addPredicateWrites(pipeCall, i2pInfo, LI, toKeep);
      } 
    }

    // So far, the transformation is local to the decoupled basic block and
    // doesn’t require updating SSA values in other blocks. This can change
    // after hoisting redundant pipe operations out of the loop body.
    if (isPE)
      hoistPipesPE(pipesToHoist, LI);
    else if (isMain)
      hoistPipesMain(pipesToHoist, LI);

    // The AGU and PE kernels have most instructions deleted. We keep track of
    // stores writing to pipes which should not be deleted. Other stores get
    // deleted and then DCE is ran to delete the rest of instructions.
    if (isAGU || isPE)
      deleteAllStores(F, toKeep);
    
    if (isMain) {
      // Delete instructions decoupled into a PE, except toKeep and pipe calls.
      for (auto &I : decoupledI) {
        if (!getPipeCall(I) && !llvm::is_contained(toKeep, I))
          deleteInstruction(I);
      }
    }

    return PreservedAnalyses::none();
  }

  static bool isRequired() { return true; }

  void getAnalysisUsage(AnalysisUsage &AU) const {
    AU.addRequiredID(LoopAnalysis::ID());
  }
};

//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getElasticTransformPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "ElasticTransform",
          LLVM_VERSION_STRING, [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "elastic-transform") {
                    FPM.addPass(ElasticTransform());
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
  return getElasticTransformPluginInfo();
}

} // end namespace llvm
