#include "CommonLLVM.h"

using namespace llvm;

namespace llvm {

const std::string REPORT_ENV_NAME = "ELASTIC_PASS_REPORT";

// Delete all store instructions in F (except exceptions).
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

/// Given a {pipeWrite} with a struct address as its operand, collect stores to
/// the struct fields.
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

void instr2pipeLsqLdReq(json::Object &i2pInfo, Value *stTagAddr,
                        Value *ldTagAddr, SmallVector<Instruction *> &created) {
  auto pipeWrite =
      reinterpret_cast<CallInst *>(*i2pInfo.getInteger("llvm_pipe_call"));
  auto I =
      reinterpret_cast<Instruction *>(*i2pInfo.getInteger("llvm_instruction"));

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
  if (i2pInfo.getString("pipe_type") == "ld_req_lsq_bram_t") {
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
  if (*i2pInfo.getInteger("max_seq_in_bb") > 1) {
    // A load tag is first used in the load request.
    ldTagStore->moveBefore(pipeWrite);
    Builder.SetInsertPoint(ldTagStore);
    Value *ldTagVal = Builder.CreateLoad(tagType, ldTagAddr, "baseLdTagVal");
    ldTagStore->setOperand(0, ldTagVal);
    // And then incremented.
    Builder.SetInsertPoint(pipeWrite);
    auto ldTagInc = Builder.CreateAdd(ldTagVal, ConstantInt::get(tagType, 1));
    auto storeForNewLdTag = Builder.CreateStore(ldTagInc, ldTagAddr);
    created.push_back(storeForNewLdTag);
  }

  created.push_back(stTagStore);
  created.push_back(addressStore);
}

void instr2pipeLsqStReq(json::Object &i2pInfo, Value *stTagAddr,
                        SmallVector<Instruction *> &created) {
  auto pipeWrite =
      reinterpret_cast<CallInst *>(*i2pInfo.getInteger("llvm_pipe_call"));
  auto I =
      reinterpret_cast<Instruction *>(*i2pInfo.getInteger("llvm_instruction"));

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
  if (i2pInfo.getString("pipe_type") == "st_req_lsq_bram_t") {
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

  created.push_back(storeForNewStTag);
  created.push_back(stTagStore);
  created.push_back(addressStore);
}

/// Given a load instruction, swap it for a pipe read.
void instr2pipeLdVal(json::Object &i2pInfo) {
  auto pipeRead =
      reinterpret_cast<CallInst *>(*i2pInfo.getInteger("llvm_pipe_call"));
  auto I =
      reinterpret_cast<Instruction *>(*i2pInfo.getInteger("llvm_instruction"));
  pipeRead->moveBefore(I);
  Value *loadVal = dyn_cast<Value>(I);
  loadVal->replaceAllUsesWith(pipeRead);
}

/// Given a store instuction, swap it for a pipe write.
void instr2pipeStVal(json::Object &i2pInfo) {
  auto pipeWrite =
      reinterpret_cast<CallInst *>(*i2pInfo.getInteger("llvm_pipe_call"));
  auto I =
      reinterpret_cast<Instruction *>(*i2pInfo.getInteger("llvm_instruction"));
  pipeWrite->moveAfter(I);
  // Instead of storing to memory, store into the pipe.
  I->setOperand(1, pipeWrite->getOperand(0));
}

/// Given a store instuction, swap it for a tagged value pipe write.
void instr2pipeStValTagged(json::Object &i2pInfo, Value *tagAddr, 
                           SmallVector<Instruction *> &created) {
  auto pipeWrite =
      reinterpret_cast<CallInst *>(*i2pInfo.getInteger("llvm_pipe_call"));
  auto I =
      reinterpret_cast<Instruction *>(*i2pInfo.getInteger("llvm_instruction"));
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

  created.push_back(tagStoreToTag);
  created.push_back(tagStoreToPipe);
}

/// Given {F}, insert an LSQ end_signal pipe call that carries the final {tag}
/// value, if {F} was is address gen. kernel. Also, if {F} is the main kernel,
/// add end_signal pipe writes to Mux kernels for each base address (if exist).
void moveEndLsqSignalToReturnBB(json::Object &i2pInfo) {
  auto pipeWrite =
      reinterpret_cast<CallInst *>(*i2pInfo.getInteger("llvm_pipe_call"));
  // The end signal pipe should be at the end of the function exit BB.
  // After the -merge-return pass, there will be only one such BB.
  auto returnBB = getReturnBlock(*pipeWrite->getFunction());
  pipeWrite->moveBefore(returnBB->getTerminator());
}

void instr2pipeSsaPe(json::Object &i2pInfo, 
                     SmallVector<Instruction *> &created) {
  auto pipeCall =
      reinterpret_cast<CallInst *>(*i2pInfo.getInteger("llvm_pipe_call"));
  auto I =
      reinterpret_cast<Instruction *>(*i2pInfo.getInteger("llvm_instruction"));
  auto decoupledBB = getChildWithIndex<Function, BasicBlock>(
      I->getFunction(), *i2pInfo.getInteger("pe_basic_block_idx"));

  // In PE reads happen at start of the decoupled BB and writes at the end.
  // Hoisted out reads happend in function entry. Hoisted writes in exit.
  if (i2pInfo.getString("read/write") == "read") {
    pipeCall->moveBefore(&decoupledBB->front());
    I->replaceAllUsesWith(pipeCall);
  } else {
    pipeCall->moveBefore(decoupledBB->getTerminator());
    created.push_back(storeValIntoPipe(I, pipeCall));
  } 
}

void instr2pipeSsaMain(json::Object &i2pInfo,
                       SmallVector<Instruction *> &created) {
  auto pipeCall =
      reinterpret_cast<CallInst *>(*i2pInfo.getInteger("llvm_pipe_call"));
  auto I =
      reinterpret_cast<Instruction *>(*i2pInfo.getInteger("llvm_instruction"));
  auto decoupledBB = getChildWithIndex<Function, BasicBlock>(
      I->getFunction(), *i2pInfo.getInteger("pe_basic_block_idx"));

  if (i2pInfo.getString("read/write") == "read") {
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
    created.push_back(storeValIntoPipe(I, pipeCall));
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
void createPredicatedPE(json::Object &i2pInfo) {
  auto predPipeRead =
      reinterpret_cast<CallInst *>(*i2pInfo.getInteger("llvm_pipe_call"));
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

void addPredicateWrites(json::Object &i2pInfo, LoopInfo &LI,
                        SmallVector<Instruction *> &created) {
  auto predPipeWrite =
      reinterpret_cast<CallInst *>(*i2pInfo.getInteger("llvm_pipe_call"));
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

  created.push_back(stTrue);
  created.push_back(stFalse);
}

/// Create an int32 val using alloca at the beginning of {F} and initialize it 
/// with {initVal}. Return its address.
Value *createTag(Function &F, SmallVector<Instruction *> &created) {
  IRBuilder<> Builder(F.getEntryBlock().getTerminator());
  // LLVM doesn't make a distinction between signed and unsigned. And the only
  // op done on tags is 2's complement addition, so the bit pattern is the same.
  auto tagType = Type::getInt32Ty(F.getContext());
  Value *tagAddr = Builder.CreateAlloca(tagType);
  auto initStore = Builder.CreateStore(ConstantInt::get(tagType, 0), tagAddr);
  created.push_back(dyn_cast<Instruction>(tagAddr));
  created.push_back(initStore);
  return tagAddr;
}

void hoistPipesPE(json::Array &directives) {
  for (const json::Value &i2pInfoVal : directives) {
    auto i2pInfo = *i2pInfoVal.getAsObject();

    auto pipeCall =
        reinterpret_cast<CallInst *>(*i2pInfo.getInteger("llvm_pipe_call"));
    auto F = pipeCall->getFunction();
    auto decoupledBB = pipeCall->getParent();
    auto loopHeader = decoupledBB->getPrevNode();
    
    if (i2pInfo.getString("read/write") == "read") {
      pipeCall->moveBefore(F->getEntryBlock().getTerminator());
    } else {
      auto recStartPipeRead = reinterpret_cast<Instruction *>(
          *i2pInfo.getInteger("llvm_recurrence_start_pipe_read"));

      auto storeIntoPipe = pipeCall->getPrevNode();
      auto storedValue = storeIntoPipe->getOperand(0);
      pipeCall->moveBefore(&getReturnBlock(*F)->front());
      storeIntoPipe->moveBefore(pipeCall);

      // If the {storedValue} is used by a phi recurrence start, then we need to
      // recreate the recurrence start phi in the PE.
      if (*i2pInfo.getBoolean("used_by_recurrence_start")) {
        auto recPhi =
            PHINode::Create(storedValue->getType(), 2, "hoistedRecurrence",
                            &loopHeader->front());
        recPhi->addIncoming(recStartPipeRead, &F->getEntryBlock());
        recPhi->addIncoming(storedValue, decoupledBB);
        storeIntoPipe->setOperand(0, recPhi);

        recStartPipeRead->replaceUsesOutsideBlock(recPhi, loopHeader);
      }
    }
  }
}

void hoistPipesMain(json::Array &directives, LoopInfo &LI) {
 for (const json::Value &i2pInfoVal : directives) {
    auto i2pInfo = *i2pInfoVal.getAsObject();

    auto isPipeRead = (i2pInfo.getString("read/write") == "read");
    auto pipeCall =
        reinterpret_cast<CallInst *>(*i2pInfo.getInteger("llvm_pipe_call"));
    auto recStart = reinterpret_cast<PHINode *>(
        *i2pInfo.getInteger("llvm_recurrence_start"));
    auto decoupledBB = pipeCall->getParent();
    auto L = LI.getLoopFor(decoupledBB);
    auto loopHeader = L->getHeader();
    auto loopPreHeader = L->getLoopPreheader();
    
    if (!isPipeRead) {
      // Supply initital value to the PE before entering loop. 
      auto initVal = recStart->DoPHITranslation(loopHeader, loopPreHeader);

      // Delete the previously created store into the pipe from the decoupledBB.
      Instruction *toDeleteStore;
      for (auto &I : *decoupledBB) {
        if (auto stI = dyn_cast<StoreInst>(&I)) {
          if (stI->getOperand(1) == pipeCall->getOperand(0))
            toDeleteStore = stI;
        }
      }

      pipeCall->moveBefore(loopPreHeader->getTerminator());
      storeValIntoPipe(initVal, pipeCall);
      deleteInstruction(toDeleteStore);
    } else {
      pipeCall->moveBefore(L->getExitBlock()->getTerminator());
      recStart->replaceAllUsesWith(pipeCall);
      if (auto recEnd = dyn_cast<Instruction>(
              recStart->DoPHITranslation(loopHeader, L->getLoopLatch()))) {
        deleteInstruction(recEnd);
      }
      deleteInstruction(recStart);
    }
  }
}

/// Return instructions in loop L which use instruction I, including I itself.
SmallVector<Instruction *> getLoopInstructionsDependingOnI(Instruction *I,
                                                           Loop *L) {
  SmallVector<Instruction *> stack{I};
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

json::Array getPipesToHoist(const json::Array &allDirectives, LoopInfo &LI) {
  json::Array hoistDirectives;

  // First collect llvm values. For each isntruction, get recurrence start.
  SmallVector<Instruction *> instructions;
  SmallVector<CallInst *> pipeCalls;
  SmallVector<BasicBlock *> decoupledBBs;
  SmallVector<const json::Object *> initialHoistDirectives;
  MapVector<Instruction *, CallInst *> recStart2pipeRead;
  for (const json::Value &i2pInfoVal : allDirectives) {
    auto i2pInfo = *i2pInfoVal.getAsObject();
    if (i2pInfo.getString("directive_type") != "ssa") 
      continue;

    auto I = reinterpret_cast<Instruction *>(
        *i2pInfo.getInteger("llvm_instruction"));
    auto pipeCall =
        reinterpret_cast<CallInst *>(*i2pInfo.getInteger("llvm_pipe_call"));
    auto decoupledBB = getChildWithIndex<Function, BasicBlock>(
        I->getFunction(), *i2pInfo.getInteger("pe_basic_block_idx"));
    auto isPipeRead = (i2pInfo.getString("read/write") == "read");

    instructions.push_back(I);
    pipeCalls.push_back(pipeCall);
    decoupledBBs.push_back(decoupledBB);
    initialHoistDirectives.push_back(i2pInfoVal.getAsObject());
    
    if (LI.isLoopHeader(I->getParent()) && isa<PHINode>(I) && isPipeRead)
      recStart2pipeRead[I] = pipeCall;
  }

  // For each pipe call, check if can be hoisted out of its loop.
  for (size_t iP = 0; iP < pipeCalls.size(); ++iP) {
    auto I = instructions[iP];
    auto decoupledBB = decoupledBBs[iP];
    auto L = LI.getLoopFor(decoupledBB);

    bool canBeHoisted = true;
    PHINode *recStart = nullptr;
    auto loopUsesOfI = getLoopInstructionsDependingOnI(I, L);
    auto allowedBlocks = {L->getHeader(), decoupledBB, L->getLoopLatch()};
    for (auto instrUseInL : loopUsesOfI) {
      // A pipe call cannot be hosited if:
      // 1: If I is used in the L loop in other blocks then {allowedBlocks}.
      // 2: I is a load in the loop L.
      // 3: I is a store operand for an instruction in the loop L.
      if (!llvm::is_contained(allowedBlocks, instrUseInL->getParent()) ||
          (isaLoad(I) && L->contains(I)) ||
          (isaStore(instrUseInL) && L->contains( instrUseInL))) {
        canBeHoisted = false;
        break;
      }
      
      // Record the instructions for the start of the recurrence. 
      if (instrUseInL->getParent() == L->getHeader())
        if (auto phiI = dyn_cast<PHINode>(instrUseInL))
          recStart = phiI;
    }

    if (canBeHoisted) {
      assert(recStart && "Did not find recurrence start, cannot hoist.");

      // Check if I is used by the recurrence_start phi, i.e. is there a path: 
      // recStart_phiNode -> I -> loopLatch_phiNode -> recStart_phiNode
      bool isUsedByRecurrenceStart = false;
      for (auto instrUseInL : loopUsesOfI) {
        if (auto latchPhiUsingI = dyn_cast<PHINode>(instrUseInL)) {
          if (latchPhiUsingI->getParent() == L->getLoopLatch()) {
            for (auto &recPhiInVal : recStart->incoming_values()) {
              if (latchPhiUsingI == recPhiInVal.get()) {
                isUsedByRecurrenceStart = true;
              }
            }
          }
        }
      }

      // Add info needed for the hosting transformation.
      auto i2pInfo = *initialHoistDirectives[iP];
      i2pInfo["llvm_recurrence_start"] = reinterpret_cast<intptr_t>(recStart);
      i2pInfo["llvm_recurrence_start_pipe_read"] =
          reinterpret_cast<intptr_t>(recStart2pipeRead[recStart]);
      i2pInfo["used_by_recurrence_start"] = isUsedByRecurrenceStart;
      hoistDirectives.push_back(std::move(i2pInfo));
    }
  }

  return hoistDirectives;
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
    json::Array hoistDirectives = getPipesToHoist(directives, LI);
    // Instructions decoupled out of the main kernel into a predicated PE.
    SmallVector<Instruction *> decoupledI = getInstructionsToDecouple(F, report);
    // Record instructions during transformation, which shouldn't be deleted.
    SmallVector<Instruction *> toKeep;

    // Tags for ordering of LSQ reqeusts and values. Not necessarily used.
    auto lsqStValTag = createTag(F, toKeep);
    auto lsqStReqTag = createTag(F, toKeep);
    auto lsqLdReqTag = createTag(F, toKeep);

    // Process all instruction-to-pipe directives for this function.
    for (json::Value &i2pInfoVal : directives) {
      auto i2pInfo = *i2pInfoVal.getAsObject();
      auto directiveType = i2pInfo.getString("directive_type");

      // LSQ related: 
      if (directiveType == "ld_val")
        instr2pipeLdVal(i2pInfo);
      else if (directiveType == "st_val")
        instr2pipeStVal(i2pInfo);
      else if (directiveType == "st_val_tagged")
        instr2pipeStValTagged(i2pInfo, lsqStValTag, toKeep);
      else if (directiveType == "ld_req")
        instr2pipeLsqLdReq(i2pInfo, lsqStReqTag, lsqLdReqTag, toKeep);
      else if (directiveType == "st_req")
        instr2pipeLsqStReq(i2pInfo, lsqStReqTag, toKeep);
      else if (directiveType == "end_lsq_signal")
        moveEndLsqSignalToReturnBB(i2pInfo);
      // Predicated PE related:
      else if (directiveType == "ssa" && isPE) 
        instr2pipeSsaPe(i2pInfo, toKeep);
      else if (directiveType == "ssa" && isMain) 
        instr2pipeSsaMain(i2pInfo, toKeep);
      else if (directiveType == "pred" && isPE) 
        createPredicatedPE(i2pInfo);
      else if (directiveType == "pred" && isMain)
        addPredicateWrites(i2pInfo, LI, toKeep);
    }

    // So far, the transformation is local to the decoupled basic block and
    // doesn’t require updating SSA values in other basic blocks. The hoisting
    // transformation can move some pipes to the loop preheader or exit blocks.
    if (isPE) 
      hoistPipesPE(hoistDirectives);
    else if (isMain)
      hoistPipesMain(hoistDirectives, LI);
    
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
