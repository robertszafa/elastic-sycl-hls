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
  // The load tag is optional.
  StoreInst *ldTagStore =
      reqStructStores.size() > 2 ? reqStructStores[2] : nullptr;
  auto addrType = addressStore->getOperand(0)->getType();
  auto tagType = stTagStore->getOperand(0)->getType();
  auto isBRAM = (i2pInfo.getString("pipe_type") == "ld_req_lsq_bram_t");

  // Move everything needed to setup the pipe write call to memInst's location.
  pipeWrite->moveBefore(I);
  stTagStore->moveBefore(pipeWrite);
  addressStore->moveBefore(pipeWrite);

  Value *memInstAddr = dyn_cast<LoadInst>(I)->getPointerOperand();

  // Write address.
  if (isBRAM) {
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
  if (isBRAM && *i2pInfo.getInteger("max_loads_per_bb") > 1) {
    // A load tag is first used in the load request.
    ldTagStore->moveBefore(pipeWrite);
    Builder.SetInsertPoint(ldTagStore);
    Value *ldTagVal = Builder.CreateLoad(tagType, ldTagAddr, "baseLdTagVal");
    ldTagStore->setOperand(0, ldTagVal);
    // And then incremented.
    Builder.SetInsertPoint(pipeWrite);
    auto ldTagInc = Builder.CreateAdd(ldTagVal, ConstantInt::get(tagType, 1));
    auto storeForNewLdTag = Builder.CreateStore(ldTagInc, ldTagAddr);
    created.push_back(ldTagStore);
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

/// Given a store instuction, swap it for a tagged value pipe write.
void instr2pipeStVal(json::Object &i2pInfo, Value *tagAddr, 
                           SmallVector<Instruction *> &created) {
  auto pipeWrite =
      reinterpret_cast<CallInst *>(*i2pInfo.getInteger("llvm_pipe_call"));
  auto I =
      reinterpret_cast<Instruction *>(*i2pInfo.getInteger("llvm_instruction"));
  // Stores into the tagged value struct.
  auto stValStructStores = getPipeOpStructStores(pipeWrite);
  StoreInst *valStore = stValStructStores[0];

  // Set the valid bit for all store values coming from this pipe.
  StoreInst *validStore = stValStructStores[2];
  validStore->setOperand(
      0, ConstantInt::get(validStore->getOperand(0)->getType(), 1));
  created.push_back(validStore);

  // If multiple store values are written in one BB, then add a tag for muxing.
  if (*i2pInfo.getInteger("max_stores_per_bb") > 1) {
    IRBuilder<> IR(I);
    StoreInst *tagStore = stValStructStores[1];
    auto tagType = tagStore->getOperand(0)->getType();
    LoadInst *tagVal = IR.CreateLoad(tagType, tagAddr);
    auto tagPlusOne = IR.CreateAdd(tagVal, ConstantInt::get(tagType, 1));
    auto tagStoreToTag = IR.CreateStore(tagPlusOne, tagAddr);
    auto tagStoreToPipe = IR.CreateStore(tagPlusOne, tagStore->getOperand(1));

    created.push_back(tagStoreToTag);
    created.push_back(tagStoreToPipe);
  }

  // Instead of storing value to memory, store into the valStore struct member.
  pipeWrite->moveAfter(I);
  I->setOperand(1, valStore->getOperand(1));
}

/// Create a new basic block with a poison pipe read/write to deallocate
/// a misspeculated address allocation to the LSQ. 
void instr2pipePoisonAlloc(json::Object &i2pInfo, Value *tagAddr,
                           SmallVector<Instruction *> &created) {
  // Map of CFG edges to poison basic blocks that already have been created.
  static MapVector<std::pair<BasicBlock *, BasicBlock *>, BasicBlock *>
      createdBlocks;

  auto pipeCall =
      reinterpret_cast<CallInst *>(*i2pInfo.getInteger("llvm_pipe_call"));

  // Create BB or get one that already exists on pred-->succ edge
  auto predBB = reinterpret_cast<BasicBlock *>(
      *i2pInfo.getInteger("llvm_pred_basic_block"));
  auto succBB = reinterpret_cast<BasicBlock *>(
      *i2pInfo.getInteger("llvm_succ_basic_block"));
  BasicBlock *poisonBB;
  IRBuilder<> IR(predBB->getContext());
  if (!createdBlocks.contains({predBB, succBB})) {
    poisonBB = BasicBlock::Create(predBB->getContext(), "Poison",
                                  predBB->getParent(), succBB);
    createdBlocks[{predBB, succBB}] = poisonBB;

    // Insert BB on pred-->succ edge
    // Before changing edges, make all phis from predBB now come from PoisonBB
    predBB->replaceSuccessorsPhiUsesWith(predBB, poisonBB);
    IR.SetInsertPoint(poisonBB);
    IR.CreateBr(succBB);
    auto branchInPredBB = dyn_cast<BranchInst>(predBB->getTerminator());
    branchInPredBB->replaceSuccessorWith(succBB, poisonBB);
  } else {
    poisonBB = createdBlocks[{predBB, succBB}];
  }

  // Create poison read/write. A poison write just doesn't set the st_val valid
  // bit. A read just discard its value (i.e. gets no SSA name).
  pipeCall->moveBefore(poisonBB->getTerminator());

  // If multiple store values are written in one BB, add a tag for muxing.
  if (*i2pInfo.getInteger("max_stores_per_bb") > 1) {
    auto stValPipeStructStores = getPipeOpStructStores(pipeCall);
    IR.SetInsertPoint(pipeCall);
    StoreInst *tagStore = stValPipeStructStores[1];
    auto tagType = tagStore->getOperand(0)->getType();
    LoadInst *tagVal = IR.CreateLoad(tagType, tagAddr);
    auto tagPlusOne = IR.CreateAdd(tagVal, ConstantInt::get(tagType, 1));
    auto tagStoreToTag = IR.CreateStore(tagPlusOne, tagAddr);
    auto tagStoreToPipe = IR.CreateStore(tagPlusOne, tagStore->getOperand(1));

    created.push_back(tagStoreToTag);
    created.push_back(tagStoreToPipe);
  }
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
      I->getFunction(), *i2pInfo.getInteger("basic_block_idx"));

  // In PE reads happen at start of the decoupled BB and writes at the end.
  // Hoisted out reads happend in function entry. Hoisted writes in exit.
  if (i2pInfo.getString("read/write") == "read") {
    pipeCall->moveBefore(&decoupledBB->front());

    // Sometimes we need to cast, e.g. if the pipe is carrying an address.
    if (I->getType() != pipeCall->getType()) {
      auto pipeReadCasted =
          BitCastInst::CreateBitOrPointerCast(pipeCall, I->getType(), "");
      pipeReadCasted->insertAfter(pipeCall);
      I->replaceAllUsesWith(pipeReadCasted);
      created.push_back(pipeReadCasted);
    } else {
      I->replaceAllUsesWith(pipeCall);
    }
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
  auto F = I->getFunction();
  auto decoupledBB = getChildWithIndex<Function, BasicBlock>(
      F, *i2pInfo.getInteger("basic_block_idx"));

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

    // Check if we need to cast, e.g. when communicating an address.
    auto pipeOpType =
        pipeCall->getOperand(0)->getType()->getNonOpaquePointerElementType();
    if (I->getType() != pipeOpType) {
      auto ICasted =
          BitCastInst::CreateBitOrPointerCast(I, pipeOpType, "", pipeCall);
      created.push_back(ICasted);
      created.push_back(storeValIntoPipe(ICasted, pipeCall));
    } else {
      created.push_back(storeValIntoPipe(I, pipeCall));
    }

    // If this pipe call writes an input dep to a loop PE, then there will be 
    // one superflous read of this pipe in the loop PE. Match it with this call. 
    if (*i2pInfo.getString("pe_type") == "loop") 
      pipeCall->clone()->insertBefore(getReturnBlock(*F)->getTerminator());
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

/// Given a basic block or an entire loop, wrap it into a 
/// "while (predPipe::read()) { BB/Loop }" code structure.
void createPredicatedPE(json::Object &i2pInfo, LoopInfo &LI) {
  auto predPipeRead =
      reinterpret_cast<CallInst *>(*i2pInfo.getInteger("llvm_pipe_call"));
  auto F = predPipeRead->getFunction();
  auto funcEntryBB = &F->getEntryBlock();
  auto funcReturnBB = getReturnBlock(*F);
  auto dcpldBB = getChildWithIndex<Function, BasicBlock>(
      F, *i2pInfo.getInteger("basic_block_idx"));
  auto L = LI.getLoopFor(dcpldBB);
  bool isLoopPE = *i2pInfo.getString("pe_type") == "loop";
    
  SmallVector<BasicBlock *> blocksToDelete;
  for (auto &BB : *F) {
    if (dcpldBB == &BB || funcEntryBB == &BB || funcReturnBB == &BB ||
        (isLoopPE && (L->getExitBlock() == &BB || L->contains(&BB)))) {
      // Keep the function entry/return block, and the decoupledBlock/loop.
      // If this is a PE for a loop, then also keep its exit block.
      continue;
    }
    blocksToDelete.push_back(&BB);
  }

  // entryBlockF --> peHeader unconditional branch.
  auto peHeader = BasicBlock::Create(F->getContext(), "peHeader", F, dcpldBB);
  dyn_cast<BranchInst>(funcEntryBB->getTerminator())->setSuccessor(0, peHeader);

  IRBuilder<> Builder(peHeader);
  // peHeader --> peBody conditional branch.
  auto peBranch = Builder.CreateCondBr(predPipeRead, dcpldBB, funcReturnBB);
  predPipeRead->moveBefore(peBranch);

  // peBody --> peHeader unconditional branch.
  auto blockWithBackedge =
      (*i2pInfo.getString("pe_type") == "loop") ? L->getExitBlock() : dcpldBB;
  auto oldBranch = blockWithBackedge->getTerminator();
  Builder.SetInsertPoint(oldBranch);
  Builder.CreateBr(peHeader);
  deleteInstruction(oldBranch);

  if (isLoopPE) {
    // Move depIn pipe reads from decoupledBB to peHeader.
    SmallVector<Instruction *> toMove;
    for (auto &I : *dcpldBB) 
      if (getPipeCall(&I)) 
        toMove.push_back(&I);
    for (auto I : toMove) 
      I->moveBefore(peBranch);
  }
    
  // Update phi sources in decoupledBB
  for (auto &phi : dcpldBB->phis()) {
    for (auto srcBB : phi.blocks()) {
      if (llvm::is_contained(blocksToDelete, srcBB))
        dcpldBB->replacePhiUsesWith(srcBB, peHeader);
    }
  }

  // Delete the collected blocks and the instructions within them.
  SmallVector<Instruction *> instrsToDelete;
  for (auto BB : blocksToDelete) 
    for (auto &I : *BB)
      instrsToDelete.push_back(&I);
  for (auto &I : instrsToDelete)
    deleteInstruction(I);
  for (auto BB : blocksToDelete) 
    BB->removeFromParent();
}

void addPredicateWrites(json::Object &i2pInfo, LoopInfo &LI,
                        SmallVector<Instruction *> &created) {
  auto predPipeWrite =
      reinterpret_cast<CallInst *>(*i2pInfo.getInteger("llvm_pipe_call"));
  auto dcpldBB = getChildWithIndex<Function, BasicBlock>(
      predPipeWrite->getFunction(), *i2pInfo.getInteger("basic_block_idx"));
  auto predType = Type::getInt8Ty(predPipeWrite->getContext());
  bool isLoopPE = (*i2pInfo.getString("pe_type") == "loop");

  // In decoupled BB invoke the predicated PE.
  predPipeWrite->moveBefore(dcpldBB->getFirstNonPHI());
  auto stTrue = storeValIntoPipe(ConstantInt::get(predType, 1), predPipeWrite);

  // Terminate the predicated PE kernel at the end of the loop/function.
  // TODO: Change this when moving to multi-value predicates.
  auto predPipeClone = dyn_cast<CallInst>(predPipeWrite->clone());
  auto insertInBlock = isLoopPE ? getReturnBlock(*dcpldBB->getParent())
                                : LI.getLoopFor(dcpldBB)->getExitBlock();
  predPipeClone->insertBefore(insertInBlock->getTerminator());
  auto stFalse = storeValIntoPipe(ConstantInt::get(predType, 0), predPipeClone);
    
  if (isLoopPE) {
    // Skip loop body: unconditional branch from loop header (i.e. dcpldBB) to
    // the successor of the loop exit block.
    IRBuilder<> Builder(dcpldBB->getTerminator());
    Builder.CreateBr(
        LI.getLoopFor(dcpldBB)->getExitBlock()->getSingleSuccessor());
  }

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

/// Hoist a pipe write to loop header, or a pipe read to loop exit blocks.
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
      pipeCall->moveBefore(&L->getExitBlock()->front());
      recStart->replaceAllUsesWith(pipeCall);
      if (auto recEnd = dyn_cast<Instruction>(
              recStart->DoPHITranslation(loopHeader, L->getLoopLatch()))) {
        deleteInstruction(recEnd);
      }
      deleteInstruction(recStart);
    }
  }
}

/// Hoist selected memory operations out of their if-condition. 
void hoistSpeculativeLSQAllocations(Function &F, const json::Object &report) {
  // First transform json info into llvm values.
  MapVector<BasicBlock *, SetVector<BasicBlock *>> mergingInfo;
  for (auto memInfoVal : *report.getArray("lsq_array")) {
    auto memInfo = *memInfoVal.getAsObject();
    for (auto hoistInfoVal : *memInfo.getArray("speculation_hoisting_info")) {
      auto hoistInfo = *hoistInfoVal.getAsObject();
      auto specBB = getChildWithIndex<Function, BasicBlock>(
          &F, *hoistInfo.getInteger("hoist_into_basic_block_idx"));
      mergingInfo[specBB] = SetVector<BasicBlock *>();
      for (auto instrInfoVal : *hoistInfo.getArray("hoisted_instructions")) {
        mergingInfo[specBB].insert(
            getInstruction(F, *instrInfoVal.getAsObject())->getParent());
      }
    }
  }

  // Now do the actual hoisting of mem ops that need to have addresses
  // speculatively allocated.
  for (auto [specBB, mergeBlocks] : mergingInfo) {
    for (auto block : mergeBlocks) {
      // Move address and any needed values to specBB
      SmallVector<Instruction *> toMove;
      for (auto &I : *block) {
        if (block->getTerminator() != &I) 
          toMove.push_back(&I);
      }
      for (auto I : toMove) 
        I->moveBefore(specBB->getTerminator());
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

/// Return pipes supplying in/out dependencied to/from a predicated basic block
/// PE, in such a way that their read/write can be hoisted to the loop
/// header/exit block without affecting the correctness of the supplied values.
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
    
    // Only pipes supplying in/out values to decoupled blocks can be hoisted.
    if (i2pInfo.getString("directive_type") != "ssa" ||
        i2pInfo.getString("pe_type") != "block") {
      continue;
    }

    auto I = reinterpret_cast<Instruction *>(
        *i2pInfo.getInteger("llvm_instruction"));
    auto pipeCall =
        reinterpret_cast<CallInst *>(*i2pInfo.getInteger("llvm_pipe_call"));
    auto decoupledBB = getChildWithIndex<Function, BasicBlock>(
        I->getFunction(), *i2pInfo.getInteger("basic_block_idx"));
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

    if (canBeHoisted && recStart) {
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

/// Return a vector of all instructions that will be decoupled into a PE.
SmallVector<Instruction *> getDecoupledI(Function &F, LoopInfo &LI,
                                         const json::Object &report) {
  SmallVector<Instruction *> res;

  for (auto peInfoVal : *report.getArray("pe_array")) {
    auto peInfo = *peInfoVal.getAsObject();
    auto dcpldBB = getChildWithIndex<Function, BasicBlock>(
      &F, *peInfo.getInteger("basic_block_idx"));

    if (peInfo["pe_type"] == "block") {
      for (auto &I : *dcpldBB) {
        if (!I.isTerminator())
          res.push_back(&I);
      }
    } else { // entire loop, dcpldBB is the loop header
      auto L = LI.getLoopFor(dcpldBB);
      for (auto BB : L->getBlocksVector()) {
        for (auto &I : *BB) {
          res.push_back(&I);
        }
      }
    }
  }

  return res;
}

/// Delete all basic blocks that belong to decoupled loops.
void deleteDecoupledLoopBodies(Function &F, LoopInfo &LI,
                               const json::Object &report) {
  for (auto peInfoVal : *report.getArray("pe_array")) {
    auto peInfo = *peInfoVal.getAsObject();
    auto dcpldBB = getChildWithIndex<Function, BasicBlock>(
      &F, *peInfo.getInteger("basic_block_idx"));

    if (peInfo["pe_type"] == "loop") {
      auto L = LI.getLoopFor(dcpldBB);
      for (auto BB : L->getBlocksVector()) {
        if (BB != dcpldBB)
          BB->removeFromParent();
      }
    }
  }
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

    // The poison directives have a predBB --> sucBB edge.
    if (auto optPredBBIdx = i2pInfo.getInteger("pred_basic_block_idx")) {
      i2pInfo["llvm_pred_basic_block"] = reinterpret_cast<std::intptr_t>(
          getChildWithIndex<Function, BasicBlock>(&F, *optPredBBIdx));
    }
    if (auto optSuccBBIdx = i2pInfo.getInteger("succ_basic_block_idx")) {
      i2pInfo["llvm_succ_basic_block"] = reinterpret_cast<std::intptr_t>(
          getChildWithIndex<Function, BasicBlock>(&F, *optSuccBBIdx));
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
    bool isBlockPE = thisKernelName.find("_PE_BB_") < thisKernelName.size();
    bool isLoopPE = thisKernelName.find("_PE_LOOP_") < thisKernelName.size();
    if (F.getCallingConv() != CallingConv::SPIR_KERNEL ||
        !(isMain || isAGU || isBlockPE || isLoopPE)) {
      return PreservedAnalyses::all();
    }

    // Each loop is in Loop Simplify Form -- preheader, header, body, latch,
    // exit blocks. Get LoopInfo to use these blocks.
    auto &LI = AM.getResult<LoopAnalysis>(F);

    // Instruction-2-pipe transformation directives for this function. 
    json::Array directives = getDirectives(F, report);
    // A mapping between recurrence start instructions and pipes to hoist out.
    // This is used to remove unnecessary blockPE <--> MainKernel comms.
    json::Array hoistDirectives = getPipesToHoist(directives, LI);
    // Instructions decoupled out of the main kernel into a predicated PE.
    SmallVector<Instruction *> decoupledI = getDecoupledI(F, LI, report);
    // Record instructions during transformation, which shouldn't be deleted.
    SmallVector<Instruction *> toKeep;

    // Optionally hoist LSQ allocations out of their if-condition.
    if (isAGU) 
      hoistSpeculativeLSQAllocations(F, report);

    // Tags for ordering of LSQ reqeusts and values. Not necessarily used, e.g.
    // the st val tag will only be used if there are >1 stores.
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
        instr2pipeStVal(i2pInfo, lsqStValTag, toKeep);
      else if (directiveType == "ld_req")
        instr2pipeLsqLdReq(i2pInfo, lsqStReqTag, lsqLdReqTag, toKeep);
      else if (directiveType == "st_req")
        instr2pipeLsqStReq(i2pInfo, lsqStReqTag, toKeep);
      else if (directiveType == "poison")
        instr2pipePoisonAlloc(i2pInfo, lsqStValTag, toKeep);
      else if (directiveType == "end_lsq_signal")
        moveEndLsqSignalToReturnBB(i2pInfo);
      // Predicated PE related:
      else if (directiveType == "ssa" && (isBlockPE || isLoopPE)) 
        instr2pipeSsaPe(i2pInfo, toKeep);
      else if (directiveType == "ssa" && isMain) 
        instr2pipeSsaMain(i2pInfo, toKeep);
      else if (directiveType == "pred" && (isBlockPE || isLoopPE)) 
        createPredicatedPE(i2pInfo, LI);
      else if (directiveType == "pred" && isMain)
        addPredicateWrites(i2pInfo, LI, toKeep);
    }

    // The hoisting transformation can move some pipes out of the decoupled BB
    // to the loop preheader or exit blocks to remove unnecessary read/writes.
    if (isBlockPE)
      hoistPipesPE(hoistDirectives);
    else if (isMain)
      hoistPipesMain(hoistDirectives, LI);
    
    // The AGU and PE kernels have most instructions deleted. We keep track of
    // stores writing to pipes which should not be deleted. Other stores get
    // deleted and then DCE is ran to delete the rest of instructions.
    if (isAGU) {
      deleteAllStores(F, toKeep);
    } else if (isBlockPE || isLoopPE) {
      llvm::append_range(toKeep, decoupledI);
      deleteAllStores(F, toKeep);
    } else if (isMain) {
      // Delete instructions decoupled into a PE, except toKeep and pipe calls.
      for (auto &I : decoupledI) {
        if (!getPipeCall(I) && !llvm::is_contained(toKeep, I))
          deleteInstruction(I);
      }

      deleteDecoupledLoopBodies(F, LI, report);
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
