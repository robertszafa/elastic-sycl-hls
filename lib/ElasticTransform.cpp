#include "CommonLLVM.h"
#include "AnalysisReportSchema.h"

using namespace llvm;

namespace llvm {

const std::string REPORT_ENV_NAME = "ELASTIC_PASS_REPORT";
const std::string POISON_BB_NAME = "poisonBB";

enum PE_PREDICATE_CODES { EXECUTE, RESET, EXIT, POISON };

/// Merge poison blocks that have the same list of posion pipe calls, and the
/// same (one) successor basic block.
void mergePoisonBlocks(Function &F) {
  using VecOfPipes = SmallVector<CallInst *>;
  MapVector<BasicBlock *, VecOfPipes> poisonBB2PipeCalls;

  for (auto &BB : F) {
    // Hacky approach to getting poison blocks but okay for now.
    auto bbName = BB.getNameOrAsOperand();
    bool isPoisonBB = bbName.find(POISON_BB_NAME) < bbName.size();

    if (isPoisonBB) {
      poisonBB2PipeCalls[&BB] = VecOfPipes();

      for (auto &I : BB) {
        if (auto PipeCallI = getPipeCall(&I)) {
          poisonBB2PipeCalls[&BB].push_back(PipeCallI);
        }
      }
    }
  }

  auto areEquivalent = [](VecOfPipes Vec1, VecOfPipes Vec2) {
    if (Vec1.size() != Vec2.size() || Vec1.size() == 0)
      return false;
    
    for (size_t i = 0; i < Vec1.size(); ++i) {
      if (Vec1[i]->getCalledFunction() != Vec2[i]->getCalledFunction())
        return false;
    }
    return true;
  };

  auto mergeBlocks = [](BasicBlock *BB1, BasicBlock *BB2) {
    for (auto &BB : *BB1->getParent()) {
      BB.getTerminator()->replaceSuccessorWith(BB2, BB1);
      BB2->replacePhiUsesWith(BB2, BB1);
    }
  };

  // Merge poison blocks that have equivalent pipe calls and the same successor.
  for (auto &[poisonBB1, pipeCalls1] : poisonBB2PipeCalls) {
    for (auto &[poisonBB2, pipeCalls2] : poisonBB2PipeCalls) {
      if (poisonBB1 != poisonBB2 && areEquivalent(pipeCalls1, pipeCalls2)) {
        // Repeated merge(A, B) and merge(B, A) are not a problem.
        mergeBlocks(poisonBB1, poisonBB2);
      }
    }
  }
}

/// Create and return a new basic blocks on the predBB --> succBB CFG edge.
/// Cache the created block and return it if called again with the same edge.
BasicBlock *createBlockOnEdge(BasicBlock *predBB, BasicBlock *succBB) {
  // Map of CFG edges to poison basic blocks that already have been created.
  static MapVector<CFGEdge, BasicBlock *> createdBlocks;
  
  const CFGEdge requestedEdge{predBB, succBB};
  if (!createdBlocks.contains(requestedEdge)) {
    auto F = predBB->getParent();
    IRBuilder<> Builder(F->getContext());
    auto newBB = BasicBlock::Create(F->getContext(), POISON_BB_NAME, F, succBB);
    createdBlocks[requestedEdge] = newBB;

    // Insert BB on pred-->succ edge
    // Before changing edges, make all phis from predBB now come from PoisonBB
    predBB->replaceSuccessorsPhiUsesWith(predBB, newBB);
    Builder.SetInsertPoint(newBB);
    Builder.CreateBr(succBB);
    auto branchInPredBB = dyn_cast<BranchInst>(predBB->getTerminator());
    branchInPredBB->replaceSuccessorWith(succBB, newBB);
  } 
  
  return createdBlocks[requestedEdge];
}

/// Move non-terminating instructions in {BB} to the end of {hoistLocation}.
/// Repeated hoistBlock(bb1, bb2) calls are not a problem.
void hoistBlock(BasicBlock *BB, BasicBlock *hoistLocation) {
  SmallVector<Instruction *> toMove;
  for (auto &I : *BB)
    if (!I.isTerminator())
      toMove.push_back(&I);

  for (auto I : toMove)
    I->moveBefore(hoistLocation->getTerminator());
}

/// Create a zero-init uint32 tag using alloca at {F.entry}. Return its address.
Value *createTag(Function &F, SetVector<Instruction *> &toKeep) {
  IRBuilder<> Builder(F.getEntryBlock().getFirstNonPHI());
  // LLVM doesn't make a distinction between signed and unsigned. And the only
  // op done on tags is 2's complement addition, so the bit pattern is the same.
  auto tagType = Type::getInt32Ty(F.getContext());
  Value *tagAddr = Builder.CreateAlloca(tagType, nullptr, "tagAddr");
  auto initStore = Builder.CreateStore(ConstantInt::get(tagType, 0), tagAddr);
  toKeep.insert(initStore);
  return tagAddr;
}

/// If pipeCall is a write, then cast {I} to pipeType, if different types. 
/// If pipeCall is a read, then cast {pipeCall} to iType, if different types. 
/// Return the casted value, or the original  
Instruction *castIfNeeded(CallInst *pipeCall, Instruction *I) {
  // If writing, then cast I to pipeType
  if (isPipeWrite(pipeCall)) {
    auto pipeType =
        pipeCall->getOperand(0)->getType()->getNonOpaquePointerElementType();
    if (I->getType()->getTypeID() != pipeType->getTypeID()) {
      return BitCastInst::CreateBitOrPointerCast(I, pipeType, "", pipeCall);
    }
    return I;
  } else { // Otherwise, if read, cast pipeCall to iType
    if (I->getType()->getTypeID() != pipeCall->getType()->getTypeID()) {
      return BitCastInst::CreateBitOrPointerCast(pipeCall, I->getType(), "",
                                                 pipeCall->getNextNode());
    }
    return pipeCall;
  }

  assert(false && "Unexpected behaviour in castIfNeeded(pipeCall, I).");
  return nullptr;
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
    } else if (auto stInstr = dyn_cast<StoreInst>(user)) {
      storesToStruct.push_back(stInstr);
      break;
    }
  }

  // The struct stores were collected in reverse above.
  std::reverse(storesToStruct.begin(), storesToStruct.end());
  return storesToStruct;
}

/// Given information abound decoupled blocks and loops, split {F} into
/// toKeep/toDelete blocks and instructions.
void splitCode(Function &F, SmallVector<PEInfo> &peArray,
               SetVector<Instruction *> &toDeleteI,
               SetVector<Instruction *> &toKeepI,
               SetVector<BasicBlock *> &toDeleteBB,
               SetVector<BasicBlock *> &toKeepBB) {
  std::string fName = demangle(std::string(F.getNameOrAsOperand()));

  toKeepBB.insert(&F.getEntryBlock());
  toKeepBB.insert(getReturnBlock(F));

  // Blocks belonging to blocksPEs.
  SetVector<BasicBlock *> blockPEs;
  for (auto &peInfo : peArray) {
    bool isThisPE = (peInfo.peKernelName == fName);

    if (isThisPE && peInfo.peType == PE_TYPE::BLOCK) {
      blockPEs.insert(peInfo.basicBlock);

      /// TODO: Use toDecouple I's from analysis?
      for (auto &I : *peInfo.basicBlock) {
        if (isa<PHINode>(&I))
          toDeleteI.insert(&I);
        else
         toKeepI.insert(&I);
      }
      for (auto &I : *getReturnBlock(F)) {
        if (!I.isTerminator())
          toDeleteI.insert(&I);
      }

      toKeepBB.insert(peInfo.basicBlock);
      for (auto &BB : F) 
        toDeleteBB.insert(&BB);
    } else if (isThisPE && peInfo.peType == PE_TYPE::LOOP) {
      toKeepBB.insert(peInfo.loop->getExitBlock());
      for (auto &BB : F) {
        if (!peInfo.loop->contains(&BB))
          toDeleteBB.insert(&BB);
      }
      for (auto &I : *getReturnBlock(F)) {
        if (!I.isTerminator())
          toDeleteI.insert(&I);
      }
    } else if (!isThisPE && peInfo.peType == PE_TYPE::BLOCK) {
      for (auto &I : *peInfo.basicBlock)
        if (!I.isTerminator()) // Leave control flow.
          toDeleteI.insert(&I);
    } else if (!isThisPE && peInfo.peType == PE_TYPE::LOOP) {
      // Connect loop header directly to loop exit.
      auto loopHeader = peInfo.loop->getHeader();
      auto loopExit = peInfo.loop->getExitBlock();

      IRBuilder<> Builder(loopHeader);
      auto oldBranch = loopHeader->getTerminator();
      Builder.CreateBr(loopExit);
      deleteInstruction(oldBranch);
      
      for (auto &I : *loopHeader) {
        if (isa<PHINode>(&I))
          toDeleteI.insert(&I);
      }

      for (auto &BB : peInfo.loop->blocks()) {
        if (BB != loopHeader)
          toDeleteBB.insert(BB);
      }
    }
  }
}

/// Delete basic blocks and instructions in {F}.
void deleteCode(Function &F, const bool isAGU,
                SetVector<Instruction *> &toDeleteI,
                SetVector<Instruction *> &toKeepI,
                SetVector<BasicBlock *> &toDeleteBB,
                SetVector<BasicBlock *> &toKeepBB) {
  auto isPrintf = [](Instruction *I) {
    if (auto CallI = dyn_cast<CallInst>(I))
      return isSpirFuncWithSubstring(*CallI->getCalledFunction(), "printf");
    return false;
  };

  // Pipe argumenets are alloca instructions that are not removed by mem2reg
  // If a store writes to memory from such an alloca, then we need to keep it.
  auto isStoreToScalarAlloc = [](Instruction *stI) {
    if (auto stPtrOp = getLoadStorePointerOperand(stI)) {
      auto siPointerBase = getPointerBase(stPtrOp);
      if (auto AllocaI = getAllocaOfPointerBase(siPointerBase)) {
        return !AllocaI->getAllocatedType()->isArrayTy();
      }
    }
    return false;
  };

  // In the AGU, we delete side-effect instructions not contributing to address
  // generation. The rest is deleted using an LLVM DCE pass.
  if (isAGU) {
    for (auto &BB : F) {
      if (&F.getEntryBlock() != &BB) {
        for (auto &I : BB) {
          bool isDebug = I.isDebugOrPseudoInst();
          bool isPrintInAGU = isAGU && isPrintf(&I);
          bool isNonPipeStore = isa<StoreInst>(&I) && !isStoreToScalarAlloc(&I);

          if (isDebug || isPrintInAGU || isNonPipeStore)
            toDeleteI.insert(&I);
        }
      }
    }
  }

  // Collect instructions to delete from blocks to delete.
  for (auto &BB : toDeleteBB) {
    if (!toKeepBB.contains(BB)) {
      for (auto &I : *BB) 
        toDeleteI.insert(&I);
    }
  }

  // Delete instructions.
  for (auto &I : toDeleteI) {
    if (!toKeepI.contains(I) && !getPipeCall(I) && I->getParent()) 
      deleteInstruction(I);
  }

  // Delete blocks.
  for (auto &BB : toDeleteBB) {
    if (!toKeepBB.contains(BB))
      BB->removeFromParent();
  }
}

/// Cast {Address} value {ToType} if required and return the instruction.
Value *castToMemReqAddress(Value *Address, Type *ToType,
                           Instruction *InsertCastBeforeThis) {
  if (Address->getType() != ToType && Address->getType()->isPointerTy()) {
    return BitCastInst::CreateBitOrPointerCast(Address, ToType, "",
                                               InsertCastBeforeThis);
  } else if (Address->getType() != ToType) {
    return BitCastInst::CreateIntegerCast(Address, ToType, true, "",
                                          InsertCastBeforeThis);
  }

  return Address;
}

/************************ Rewrite rules start ************************/

/******************** LSQ related: */ 

void doLdReqWrite(const RewriteRule &rule, const LSQInfo &lsqInfo,
                  SetVector<Instruction *> &toKeep) {
  // The pipe writes a {address, tag} struct.
  auto reqStructStores = getPipeOpStructStores(rule.pipeCall);
  StoreInst *addressStore = reqStructStores[0];
  StoreInst *stTagStore = reqStructStores[1];
  // The load tag is optional.
  StoreInst *ldTagStore =
      reqStructStores.size() > 2 ? reqStructStores[2] : nullptr;
  auto addrType = addressStore->getOperand(0)->getType();
  auto tagType = stTagStore->getOperand(0)->getType();

  // Move everything to instruction location.
  rule.pipeCall->moveBefore(rule.instruction);
  stTagStore->moveBefore(rule.pipeCall);
  addressStore->moveBefore(rule.pipeCall);

  Value *loadAddr = dyn_cast<LoadInst>(rule.instruction)->getPointerOperand();

  // Write address.
  if (lsqInfo.isOnChipMem) {
    // For BRAM accesses, we only get the index value, not the full pointer.
    auto gepAddr = dyn_cast<GetElementPtrInst>(loadAddr);
    loadAddr = gepAddr->getOperand(gepAddr->getNumOperands() - 1);
    if (auto sextI = dyn_cast<SExtInst>(loadAddr)) 
      loadAddr = sextI->getOperand(0);
  }
  auto addressCasted = castToMemReqAddress(loadAddr, addrType, addressStore);
  addressStore->setOperand(0, addressCasted);

  // Write store tag.
  IRBuilder<> Builder(stTagStore);
  Value *stTagVal = Builder.CreateLoad(tagType, lsqInfo.stTagAddr, "stTag");
  stTagStore->setOperand(0, stTagVal);

  // Also write a ld tag if multiple loads have to be muxed going into the LSQ.
  if (lsqInfo.isOnChipMem && lsqInfo.numLoadPipes > 1) {
    // A load tag is first used in the load request.
    ldTagStore->moveBefore(rule.pipeCall);
    Builder.SetInsertPoint(ldTagStore);
    Value *ldTagVal = Builder.CreateLoad(tagType, lsqInfo.ldTagAddr, "ldTag");
    ldTagStore->setOperand(0, ldTagVal);
    // And then incremented.
    Builder.SetInsertPoint(rule.pipeCall);
    auto ldTagInc = Builder.CreateAdd(ldTagVal, ConstantInt::get(tagType, 1));
    auto storeForNewLdTag = Builder.CreateStore(ldTagInc, lsqInfo.ldTagAddr);
    toKeep.insert(ldTagStore);
    toKeep.insert(storeForNewLdTag);
  }

  toKeep.insert(stTagStore);
  toKeep.insert(addressStore);

  // If this address allocation is speculated, then move instructions to specBB. 
  if (rule.specBasicBlock)
    hoistBlock(rule.basicBlock, rule.specBasicBlock);

  if (lsqInfo.isAddressGenDecoupled)
    deleteInstruction(rule.instruction);
}

void doStReqWrite(const RewriteRule &rule, const LSQInfo &lsqInfo,
                  SetVector<Instruction *> &toKeep) {
  // The pipe writes a {address, tag} struct.
  auto reqStructStores = getPipeOpStructStores(rule.pipeCall);
  StoreInst *addressStore = reqStructStores[0];
  StoreInst *stTagStore = reqStructStores[1];
  auto addrType = addressStore->getOperand(0)->getType();
  auto tagType = stTagStore->getOperand(0)->getType();

  // Move everything to instruction location.
  rule.pipeCall->moveBefore(rule.instruction);
  stTagStore->moveBefore(rule.pipeCall);
  addressStore->moveBefore(rule.pipeCall);

  // Write address.
  Value *stAddr = dyn_cast<StoreInst>(rule.instruction)->getPointerOperand();
  if (lsqInfo.isOnChipMem) {
    // For BRAM accesses, we only get the index value, not the full pointer.
    auto gepAddr = dyn_cast<GetElementPtrInst>(stAddr);
    stAddr = gepAddr->getOperand(gepAddr->getNumOperands() - 1);
    if (auto sextI = dyn_cast<SExtInst>(stAddr)) 
      stAddr = sextI->getOperand(0);
  }
  auto addressCasted = castToMemReqAddress(stAddr, addrType, addressStore);
  addressStore->setOperand(0, addressCasted);

  // Write tag. Since it's a store, increment the tag first.
  IRBuilder<> Builder(stTagStore);
  Value *stTagVal = Builder.CreateLoad(tagType, lsqInfo.stTagAddr, "stTag");
  auto stTagInc = Builder.CreateAdd(stTagVal, ConstantInt::get(tagType, 1));
  stTagStore->setOperand(0, stTagInc);
  auto storeForNewStTag = Builder.CreateStore(stTagInc, lsqInfo.stTagAddr);

  toKeep.insert(storeForNewStTag);
  toKeep.insert(stTagStore);
  toKeep.insert(addressStore);

  // If this address allocation is speculated, then move instructions to specBB. 
  if (rule.specBasicBlock)
    hoistBlock(rule.basicBlock, rule.specBasicBlock);

  if (lsqInfo.isAddressGenDecoupled)
    deleteInstruction(rule.instruction);
}

void doLdValRead(const RewriteRule &rule) {
  rule.pipeCall->moveAfter(rule.instruction);
  Value *loadVal = dyn_cast<Value>(rule.instruction);
  loadVal->replaceAllUsesWith(rule.pipeCall);

  // Move load out of if-condition if specBB is set.
  if (rule.specBasicBlock) {
    rule.pipeCall->moveBefore(rule.specBasicBlock->getTerminator());
  }
}

void doStValWrite(const RewriteRule &rule, const LSQInfo &lsqInfo,
                  SetVector<Instruction *> &toKeep) {
  // Stores into the tagged value struct.
  auto stValStructStores = getPipeOpStructStores(rule.pipeCall);
  StoreInst *valStoreIntoPipe = stValStructStores[0];

  // Set the valid bit for all store values coming from this pipe.
  StoreInst *validStore = stValStructStores[2];
  auto validBitType = validStore->getOperand(0)->getType();
  validStore->setOperand(0, ConstantInt::get(validBitType, 1));
  toKeep.insert(validStore);

  // Instead of storing value to memory, store into the valStore struct member.
  rule.pipeCall->moveAfter(rule.instruction);
  validStore->moveBefore(rule.pipeCall);
  rule.instruction->setOperand(1, valStoreIntoPipe->getOperand(1));
  toKeep.insert(rule.instruction);

  // If multiple store values are written in one BB, then add a tag for muxing.
  if (lsqInfo.numStoreValPipes > 1) {
    StoreInst *tagStore = stValStructStores[1];
    auto tagType = tagStore->getOperand(0)->getType();
    tagStore->moveBefore(rule.pipeCall);
    IRBuilder<> IR(tagStore);
    LoadInst *tagVal = IR.CreateLoad(tagType, lsqInfo.stTagAddr);

    // If not decoupled, then the increment is already done by the st request.
    if (lsqInfo.isAddressGenDecoupled) {
      auto tagPlusOne = IR.CreateAdd(tagVal, ConstantInt::get(tagType, 1));
      toKeep.insert(IR.CreateStore(tagPlusOne, lsqInfo.stTagAddr));
      tagStore->setOperand(0, tagPlusOne);
    } else {
      tagStore->setOperand(0, tagVal);
    }
    
    toKeep.insert(tagStore);
  }
}

void doPoisonLdRead(const RewriteRule &rule) {
  // We just move the load to the speculation location. Alternatively it could 
  // moved to the poison location.
  // auto *poisonBB = createBlockOnEdge(rule.predBasicBlock, rule.succBasicBlock);
  rule.pipeCall->moveBefore(rule.specBasicBlock->getTerminator());
}

void doPoisonStWrite(const RewriteRule &rule, const LSQInfo &lsqInfo,
                     const LoopInfo &LI, SetVector<Instruction *> &toKeep) {

  // Check if the edge requires creating a new block (is the edge.dst reachable
  // from trueBB?)
  auto trueBB = rule.basicBlock;
  auto L = LI.getLoopFor(trueBB);
  if (isReachableWithinLoop(trueBB, rule.succBasicBlock, L)) {
    // Triangle branch.
    auto *newBB = createBlockOnEdge(rule.predBasicBlock, rule.succBasicBlock);
    rule.pipeCall->moveBefore(newBB->getTerminator());
  } else {
    rule.pipeCall->moveBefore(rule.succBasicBlock->getFirstNonPHI());
  }

  auto stValStructStores = getPipeOpStructStores(rule.pipeCall);
  StoreInst *validStore = stValStructStores[2];
  auto validBitType = validStore->getOperand(0)->getType();
  validStore->setOperand(0, ConstantInt::get(validBitType, 0));
  validStore->moveBefore(rule.pipeCall);
  toKeep.insert(validStore);

  // If store value tag is used, then we need to increment and use it.
  if (lsqInfo.numStoreValPipes > 1) {
    StoreInst *tagStore = stValStructStores[1];
    auto tagType = tagStore->getOperand(0)->getType();
    tagStore->moveBefore(rule.pipeCall);
    IRBuilder<> IR(tagStore);
    LoadInst *tagVal = IR.CreateLoad(tagType, lsqInfo.stTagAddr);
    auto tagPlusOne = IR.CreateAdd(tagVal, ConstantInt::get(tagType, 1));
    tagStore->setOperand(0, tagPlusOne);

    toKeep.insert(tagStore);
    toKeep.insert(IR.CreateStore(tagPlusOne, lsqInfo.stTagAddr));
  }
}

void doEndLsqSignalWrite(const RewriteRule &rule) {
  // Just move the pipe call to the function exit block.
  rule.pipeCall->moveBefore(rule.basicBlock->getTerminator());
}

/******************** PE related: */ 
/** Block PEs: */

void doPredBbWrite(const RewriteRule &rule, const PEInfo &peInfo,
                   SetVector<Instruction *> &toKeep) {
  auto funcExitBB = getReturnBlock(*rule.pipeCall->getFunction());
  auto predType = Type::getInt8Ty(funcExitBB->getContext());

  // Write PRED::EXEC in the decoupled block.
  rule.pipeCall->moveBefore(rule.basicBlock->getFirstNonPHI());
  toKeep.insert(storeValIntoPipe(
      ConstantInt::get(predType, PE_PREDICATE_CODES::EXECUTE), rule.pipeCall));

  // If the predicated PE needs to be reset, then write the RESET predicate on
  // loop exit and the EXIT predicate on function exit.
  if (peInfo.needsResetPredicate) {
    auto pipeResetCall = dyn_cast<CallInst>(rule.pipeCall->clone());
    pipeResetCall->insertBefore(rule.loopExitBlock->getFirstNonPHI());
    toKeep.insert(storeValIntoPipe(
        ConstantInt::get(predType, PE_PREDICATE_CODES::RESET), pipeResetCall));

    // Write PRED::EXIT in the function exit block.
    auto pipeExitCall = dyn_cast<CallInst>(rule.pipeCall->clone());
    pipeExitCall->insertBefore(funcExitBB->getFirstNonPHI());
    toKeep.insert(storeValIntoPipe(
        ConstantInt::get(predType, PE_PREDICATE_CODES::EXIT), pipeExitCall));
  } else {
    // Otherwise, just write the EXIT predicate on loop exit.
    auto pipeResetCall = dyn_cast<CallInst>(rule.pipeCall->clone());
    pipeResetCall->insertBefore(rule.loopExitBlock->getFirstNonPHI());
    toKeep.insert(storeValIntoPipe(
        ConstantInt::get(predType, PE_PREDICATE_CODES::EXIT), pipeResetCall));
  }
}

/// Creates a predicated block PE. 
void doPredBbRead(const RewriteRule &rule, PEInfo &peInfo) {
  auto F = rule.pipeCall->getFunction();
  auto entryBB = &F->getEntryBlock();
  auto exitBB = getReturnBlock(*F);
  auto predType = Type::getInt8Ty(F->getContext());
    
  // We will wrap the decoupled rule.basicBlock in a custom FSM. The FSM states
  // (i.e. basic blocks) are changed based on the predicate pipe value.
  //                      ↓~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~¬
  //    funcEntry -> preHeader -> header <-> decoupledBB -> exiting -> funcExit
  peInfo.peHeader =
      BasicBlock::Create(F->getContext(), "header", F, rule.basicBlock);
  peInfo.pePreHeader =
      BasicBlock::Create(F->getContext(), "preHeader", F, peInfo.peHeader);
  peInfo.peExiting = BasicBlock::Create(F->getContext(), "exiting", F, exitBB);
  peInfo.peExit = exitBB;

  // entry -> preHeader
  auto entryBbBranch = dyn_cast<BranchInst>(entryBB->getTerminator());
  entryBbBranch->setSuccessor(0, peInfo.pePreHeader);

  // preHeader -> header
  IRBuilder<> Builder(peInfo.pePreHeader);
  Builder.CreateBr(peInfo.peHeader);

  // header -> body OR header -> exiting
  Builder.SetInsertPoint(peInfo.peHeader);
  auto isExec = Builder.CreateICmpEQ(
      rule.pipeCall, ConstantInt::get(predType, PE_PREDICATE_CODES::EXECUTE));
  Builder.CreateCondBr(isExec, rule.basicBlock, peInfo.peExiting);
  rule.pipeCall->moveBefore(&peInfo.peHeader->front());

  // decoupledBasicBlock -> header
  Builder.SetInsertPoint(rule.basicBlock);
  auto oldBranch = rule.basicBlock->getTerminator();
  Builder.CreateBr(peInfo.peHeader);
  deleteInstruction(oldBranch);

  // exitingBB -> preHeader OR exitingBB -> exit
  Builder.SetInsertPoint(peInfo.peExiting);
  auto isReset = Builder.CreateICmpEQ(
      rule.pipeCall, ConstantInt::get(predType, PE_PREDICATE_CODES::RESET));
  Builder.CreateCondBr(isReset, peInfo.pePreHeader, peInfo.peExit);

  // Optional poison block on header -> exiting CFG edge.
  if (peInfo.needsPoisonBlock) {
    // Need 2 new blocks to either go to poisonBB or coninue to exitingBB.
    auto beforeExiting = BasicBlock::Create(F->getContext(), "beforeExiting", F,
                                            peInfo.peExiting);
    peInfo.pePoisonBlock =
        BasicBlock::Create(F->getContext(), "poison", F, peInfo.peExiting);
    // Change "header -> exiting" to "header -> beforeExiting"
    peInfo.peHeader->getTerminator()->setOperand(1, beforeExiting);
    Builder.SetInsertPoint(beforeExiting);
    // beforeExiting -> poisonBB OR beforeExiting -> exiting
    auto isPoison = Builder.CreateICmpEQ(
        rule.pipeCall, ConstantInt::get(predType, PE_PREDICATE_CODES::POISON));
    Builder.CreateCondBr(isPoison, peInfo.pePoisonBlock, peInfo.peExiting);
    // poisonBB -> header
    Builder.SetInsertPoint(peInfo.pePoisonBlock);
    Builder.CreateBr(peInfo.peHeader);
  }
}

void doSsaBbInWrite(const RewriteRule &rule, const PEInfo &peInfo,
                    SetVector<Instruction *> &toKeep) {
  if (rule.isHoistedOutOfLoop) {
    assert(rule.instruction == rule.recurrenceStart &&
           "Can only hoist instructions at the start/end of a recurrence.");

    // Hoist pipe write to loop preHeader.
    rule.pipeCall->moveBefore(rule.loopPreHeaderBlock->getTerminator());
    auto valToWrite =
        rule.recurrenceStart->getIncomingValueForBlock(rule.loopPreHeaderBlock);
    toKeep.insert(storeValIntoPipe(valToWrite, rule.pipeCall));

    // PEs that use a RESET predicate will only receive an EXIT predicate on
    // function exit. To prevent deadlock, we need to repeat hoisted input
    // depedencies writes in function exit because the PE will wait for them.
    if (peInfo.needsResetPredicate) {
      auto funcExitBB = getReturnBlock(*rule.pipeCall->getFunction());
      rule.pipeCall->clone()->insertBefore(funcExitBB->getFirstNonPHI());
    }
  } else {
    rule.pipeCall->moveBefore(rule.basicBlock->getFirstNonPHI());
    auto valToWrite = castIfNeeded(rule.pipeCall, rule.instruction);
    toKeep.insert(storeValIntoPipe(valToWrite, rule.pipeCall));
  }
}

void doSsaBbInRead(const RewriteRule &rule, const PEInfo &peInfo) {
  if (rule.isHoistedOutOfLoop) {
    assert(rule.instruction == rule.recurrenceStart &&
           "Can only hoist instructions at the start/end of a recurrence.");

    // Hoist to pePreHeader. Note no "rule.instruction->replaceUsesWith()".
    // The PE body will still use the recStartPHI. The pipeCall value will be
    // one of the incoming values into the PHI./
    rule.pipeCall->moveBefore(peInfo.pePreHeader->getTerminator());
    rule.recurrenceStart->moveBefore(peInfo.peHeader->getFirstNonPHI());

    // recurrenceStart inititalization coming from pipe read in the pePrehEader.
    rule.recurrenceStart->replaceIncomingBlockWith(rule.loopPreHeaderBlock,
                                                   peInfo.pePreHeader);
    rule.recurrenceStart->setIncomingValueForBlock(peInfo.pePreHeader,
                                                   rule.pipeCall);

    // recurrenceStart value from PE execution coming from rule.basicBlock
    auto valFromPE = rule.recurrenceEnd->DoPHITranslation(
        rule.recurrenceEnd->getParent(), rule.basicBlock);
    rule.recurrenceStart->replaceIncomingBlockWith(rule.loopLatchBlock,
                                                   rule.basicBlock);
    rule.recurrenceStart->setIncomingValueForBlock(rule.basicBlock, valFromPE);

    // If we have a poisonBB in the PE, then also add it to the phi.
    if (peInfo.pePoisonBlock) {
      rule.recurrenceStart->addIncoming(rule.recurrenceStart,
                                        peInfo.pePoisonBlock);
    }
  } else {
    rule.pipeCall->moveBefore(rule.basicBlock->getFirstNonPHI());
    auto valRead = castIfNeeded(rule.pipeCall, rule.instruction);
    rule.instruction->replaceAllUsesWith(valRead);
  }
}

void doSsaBbOutWrite(const RewriteRule &rule, const PEInfo &peInfo,
                     SetVector<Instruction *> &toKeep) {
  if (rule.isHoistedOutOfLoop) {
    // A hoisted output dependency from the PE to its parentKernel is
    // communicated in the peExiting block. The value is the recurrenceStartPhi.
    rule.pipeCall->moveBefore(peInfo.peExiting->getTerminator());
    toKeep.insert(storeValIntoPipe(rule.recurrenceStart, rule.pipeCall));
  } else {
    rule.pipeCall->moveBefore(rule.basicBlock->getTerminator());
    auto valToWrite = castIfNeeded(rule.pipeCall, rule.instruction);
    toKeep.insert(storeValIntoPipe(valToWrite, rule.pipeCall));
  }
}

void doSsaBbOutRead(const RewriteRule &rule, const PEInfo &peInfo) {
  if (rule.isHoistedOutOfLoop) {
    // The depOut pipe read should happen after the predicate pipe write for
    // that PE. So insert at end of block, or before the first use if exists.
    auto firstUse = getFirstUserInBB(rule.recurrenceStart, rule.loopExitBlock);
    auto insertPt = firstUse ? firstUse : rule.loopExitBlock->getTerminator();
    rule.pipeCall->moveBefore(insertPt);

    // The final recurrence value is held by the recStartPhi in the loop header
    // after the loop exits. Change the uses of that value to the pipe call.
    auto valRead = castIfNeeded(rule.pipeCall, rule.recurrenceEnd);
    rule.recurrenceStart->replaceAllUsesWith(valRead);

    // PEs that use a RESET predicate will only receive an EXIT predicate on
    // function exit. At that point, a PE will send hoisted output dependencies
    // one final time. Repeat the pipe read to prevent deadlock.
    if (peInfo.needsResetPredicate) {
      auto funcExitBB = getReturnBlock(*rule.pipeCall->getFunction());
      rule.pipeCall->clone()->insertBefore(funcExitBB->getTerminator());
    }

    // The recurrence PHIs have been completely hoisted out of the loop now.
    deleteInstruction(rule.recurrenceStart);
    deleteInstruction(rule.recurrenceEnd);
  } else {
    rule.pipeCall->moveBefore(rule.basicBlock->getTerminator());
    auto valRead = castIfNeeded(rule.pipeCall, rule.instruction);
    rule.instruction->replaceAllUsesWith(valRead);
  }
}

/** Loop PEs: */ 

void doPredLoopWrite(const RewriteRule &rule,
                     SetVector<Instruction *> &toKeep) {
  auto F = rule.pipeCall->getFunction();
  auto predType = Type::getInt8Ty(F->getContext());

  // Write PRED::EXEC in the decoupled loop header block.
  rule.pipeCall->moveBefore(rule.basicBlock->getFirstNonPHI());
  toKeep.insert(storeValIntoPipe(
      ConstantInt::get(predType, PE_PREDICATE_CODES::EXECUTE), rule.pipeCall));

  // Write PRED::EXIT in the function exit block.
  auto pipeExitCall = dyn_cast<CallInst>(rule.pipeCall->clone());
  pipeExitCall->insertBefore(getReturnBlock(*F)->getFirstNonPHI());
  toKeep.insert(storeValIntoPipe(
      ConstantInt::get(predType, PE_PREDICATE_CODES::EXIT), pipeExitCall));
}

/// Creates a predicated loop PE. 
void doPredLoopRead(const RewriteRule &rule, PEInfo &peInfo) {
  auto F = rule.pipeCall->getFunction();
  auto predType = Type::getInt8Ty(F->getContext());
  auto funcEntryBB = &F->getEntryBlock();
  auto funcExitBB = getReturnBlock(*F);

  // 1) We will wrap the decoupled loop in a custom FSM. The FSM states
  // (i.e. basic blocks) are changed based on the predicate pipe value.
  peInfo.peHeader =
      BasicBlock::Create(F->getContext(), "peHeader", F, rule.loopHeaderBlock);
  peInfo.peExit = funcExitBB;

  // entry -> peHeader
  auto funcEntryBranch = dyn_cast<BranchInst>(funcEntryBB->getTerminator());
  funcEntryBranch->setSuccessor(0, peInfo.peHeader);

  // peHeader -> decoupledLoop OR peHeader -> funcExit
  IRBuilder<> Builder(peInfo.peHeader);
  Builder.SetInsertPoint(peInfo.peHeader);
  auto isExec = Builder.CreateICmpEQ(rule.pipeCall, 
      ConstantInt::get(predType, PE_PREDICATE_CODES::EXECUTE));
  Builder.CreateCondBr(isExec, rule.loopHeaderBlock, funcExitBB);
  rule.pipeCall->moveBefore(&peInfo.peHeader->front());

  // loopExit -> peHeader 
  Builder.SetInsertPoint(rule.loopExitBlock);
  auto oldBranch = rule.loopExitBlock->getTerminator();
  Builder.CreateBr(peInfo.peHeader);
  deleteInstruction(oldBranch);

  // Update phi sources in loopHeader to come from peHeader.
  for (auto &phi : rule.loopHeaderBlock->phis()) {
    for (auto srcBB : phi.blocks()) {
      if (srcBB != rule.loopLatchBlock) {
        rule.loopHeaderBlock->replacePhiUsesWith(srcBB, peInfo.peHeader);
      }
    }
  }
}

void doSsaLoopInWrite(const RewriteRule &rule,
                      SetVector<Instruction *> &toKeep) {
  rule.pipeCall->moveBefore(rule.loopHeaderBlock->getFirstNonPHI());
  auto valToWrite = castIfNeeded(rule.pipeCall, rule.instruction);
  toKeep.insert(storeValIntoPipe(valToWrite, rule.pipeCall));

  // For a loopPE to terminate, it needs one "dummy" read of its input deps.
  auto funcExit = getReturnBlock(*rule.pipeCall->getFunction());
  rule.pipeCall->clone()->insertBefore(funcExit->getTerminator());
}

void doSsaLoopInRead(const RewriteRule &rule, const PEInfo &peInfo) {
  rule.pipeCall->moveBefore(peInfo.peHeader->getFirstNonPHI());
  auto valRead = castIfNeeded(rule.pipeCall, rule.instruction);
  rule.instruction->replaceAllUsesWith(valRead);
}

void doSsaLoopOutWrite(const RewriteRule &rule, const PEInfo &peInfo,
                       SetVector<Instruction *> &toKeep) {
  rule.pipeCall->moveBefore(rule.loopExitBlock->getTerminator());
  auto valToWrite = castIfNeeded(rule.pipeCall, rule.instruction);
  toKeep.insert(storeValIntoPipe(valToWrite, rule.pipeCall));
}

void doSsaLoopOutRead(const RewriteRule &rule) {
  rule.pipeCall->moveBefore(rule.loopHeaderBlock->getTerminator());
  auto valRead = castIfNeeded(rule.pipeCall, rule.instruction);
  rule.instruction->replaceAllUsesWith(valRead);
}

/******************** Composition of PEs and LSQs related: */ 

void doStValTagInRead(const RewriteRule &rule, const LSQInfo &lsqInfo,
                      const PEInfo &peInfo, SetVector<Instruction *> &toKeep) {
  if (peInfo.peType == PE_TYPE::LOOP) {
    rule.pipeCall->moveBefore(peInfo.peHeader->getFirstNonPHI());
  }
  else { // BLOCK PE
    // The read might happen in the blockPE poison block.
    auto isInPoisonBB = rule.predBasicBlock && rule.succBasicBlock;
    auto insertInBlock = isInPoisonBB ? peInfo.pePoisonBlock : rule.basicBlock;
    rule.pipeCall->moveBefore(insertInBlock->getFirstNonPHI());
  }

  // Read tag from pipe and store into tagAddr.
  IRBuilder<> IR(rule.pipeCall);
  auto tagSt = IR.CreateStore(rule.pipeCall, lsqInfo.stTagAddr);
  tagSt->moveAfter(rule.pipeCall);
  toKeep.insert(tagSt);
}

void doStValTagToBBWrite(const RewriteRule &rule, const LSQInfo &lsqInfo,
                         const PEInfo &peInfo,
                         SetVector<Instruction *> &toKeep) {
  // The write might happen in a poison block.
  auto isInPoisonBB = rule.predBasicBlock && rule.succBasicBlock;
  auto insertInBlock =
      isInPoisonBB ? createBlockOnEdge(rule.predBasicBlock, rule.succBasicBlock)
                   : rule.basicBlock;
  rule.pipeCall->moveBefore(insertInBlock->getFirstNonPHI());

  // Load lsq st tag and store into pipe 
  IRBuilder<> IR(rule.pipeCall);
  auto stIntoTagPipe = getPipeOpStructStores(rule.pipeCall)[0];
  auto tagType = stIntoTagPipe->getOperand(0)->getType();
  auto tagValLd = IR.CreateLoad(tagType, lsqInfo.stTagAddr);
  stIntoTagPipe->moveAfter(tagValLd);
  stIntoTagPipe->setOperand(0, tagValLd);
  toKeep.insert(stIntoTagPipe);

  // When communicating a st_val_tag to a decoupled basic block, we can
  // directly increment the tag in the main kernel by the number of storesInBB.
  auto incrementConst = ConstantInt::get(tagType, rule.numStoresInBlock);
  auto newTagVal = IR.CreateAdd(tagValLd, incrementConst);
  toKeep.insert(IR.CreateStore(newTagVal, lsqInfo.stTagAddr));
  toKeep.insert(dyn_cast<Instruction>(newTagVal));
}

/// Create a new basic block with a poison pipe to the LSQ.
void doPoisonPredBbWrite(const RewriteRule &rule, const LSQInfo &lsqInfo,
                         SetVector<Instruction *> &toKeep) {
  auto poisonBB = createBlockOnEdge(rule.predBasicBlock, rule.succBasicBlock);

  rule.pipeCall->moveBefore(poisonBB->getTerminator());

  auto predType = Type::getInt8Ty(rule.pipeCall->getContext());
  toKeep.insert(storeValIntoPipe(
      ConstantInt::get(predType, PE_PREDICATE_CODES::POISON), rule.pipeCall));
}

void doPoisonInBbPeLdRead(const RewriteRule &rule, const PEInfo &peInfo,
                          SetVector<Instruction *> &toKeep) {
  // The pipe read value will not be used.
  rule.pipeCall->moveBefore(peInfo.pePoisonBlock->getTerminator());
}

void doPoisonInBbPeStWrite(const RewriteRule &rule, const LSQInfo &lsqInfo,
                           const PEInfo &peInfo,
                           SetVector<Instruction *> &toKeep) {
  rule.pipeCall->moveBefore(peInfo.pePoisonBlock->getTerminator());

  // Use tag for the poison st_val write if needed.
  if (lsqInfo.numStoreValPipes > 1) {
    IRBuilder<> Builder(rule.pipeCall);
    auto stValPipeStructStores = getPipeOpStructStores(rule.pipeCall);
    StoreInst *tagStore = stValPipeStructStores[1];
    auto tagType = tagStore->getOperand(0)->getType();
    LoadInst *tagVal = Builder.CreateLoad(tagType, lsqInfo.stTagAddr);
    auto tagPlusOne = Builder.CreateAdd(tagVal, ConstantInt::get(tagType, 1));

    toKeep.insert(Builder.CreateStore(tagPlusOne, lsqInfo.stTagAddr));
    toKeep.insert(Builder.CreateStore(tagPlusOne, tagStore->getOperand(1)));
  }
}

void doStValTagToLoopWrite(const RewriteRule &rule, const LSQInfo &lsqInfo,
                           ScalarEvolution &SE,
                           SetVector<Instruction *> &toKeep) {
  rule.pipeCall->moveBefore(rule.loopHeaderBlock->getFirstNonPHI());

  IRBuilder<> IR(rule.pipeCall);
  auto F = rule.pipeCall->getFunction();
  // Load tag from lsqInfo st tag addr and store into pipe 
  auto stIntoTagPipe = getPipeOpStructStores(rule.pipeCall)[0];
  auto tagType = stIntoTagPipe->getOperand(0)->getType();
  auto tagValLd = IR.CreateLoad(tagType, lsqInfo.stTagAddr);
  stIntoTagPipe->moveAfter(tagValLd);
  stIntoTagPipe->setOperand(0, tagValLd);
  toKeep.insert(stIntoTagPipe);

  // Sometimes, we can just increment the tag in the main kernel by the
  // number of total stores that will happen in the decoupled loop.
  if (rule.canBuildNumStoresInLoopExpr) {
    auto numIterExpr =
        buildSCEVExpr(*F, SE.getBackedgeTakenCount(rule.loop), rule.pipeCall);
    auto totalStoresInL = IR.CreateMul(
        ConstantInt::get(tagType, rule.numStoresInLoop), numIterExpr);

    // Add the total to the tag.
    auto newTagVal = IR.CreateAdd(tagValLd, totalStoresInL);
    toKeep.insert(IR.CreateStore(newTagVal, lsqInfo.stTagAddr));
    toKeep.insert(dyn_cast<Instruction>(newTagVal));
  }

  // Since the loopPE has a stVal pipe read in PE header that will be called 
  // every time a pred is read, we need to supply a dummy tag on func exit.
  rule.pipeCall->clone()->insertBefore(getReturnBlock(*F)->getFirstNonPHI());
}

void doStValTagLoopOutWrite(const RewriteRule &rule, const LSQInfo &lsqInfo,
                            const PEInfo &peInfo,
                            SetVector<Instruction *> &toKeep) {
  rule.pipeCall->moveBefore(rule.loopExitBlock->getFirstNonPHI());

  IRBuilder<> IR(rule.pipeCall);
  // Load tag from tag addr and store into pipe
  auto stIntoTagPipe = getPipeOpStructStores(rule.pipeCall)[0];
  auto tagType = stIntoTagPipe->getOperand(0)->getType();
  auto tagValLd = IR.CreateLoad(tagType, lsqInfo.stTagAddr);
  stIntoTagPipe->moveAfter(tagValLd);
  stIntoTagPipe->setOperand(0, tagValLd);
  toKeep.insert(stIntoTagPipe);
}

/************************ Rewrite rules end ************************/

struct ElasticTransform : PassInfoMixin<ElasticTransform> {
  json::Object report;

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    // Read in report once.
    if (report.empty())
      report = *parseJsonReport(REPORT_ENV_NAME).getAsObject();

    // Only transform if this is the main kernel, an AGU or a PE.
    std::string thisKernelName = demangle(std::string(F.getNameOrAsOperand()));
    auto mainKernelName = *report["mainKernelName"].getAsString();
    bool isMain = mainKernelName == thisKernelName;
    bool isAGU = thisKernelName.find(AGU_ID) < thisKernelName.size();
    bool isBlockPE = thisKernelName.find(BLOCK_PE_ID) < thisKernelName.size();
    bool isLoopPE = thisKernelName.find(LOOP_PE_ID) < thisKernelName.size();
    if (F.getCallingConv() != CallingConv::SPIR_KERNEL ||
        !(isMain || isAGU || isBlockPE || isLoopPE)) {
      return PreservedAnalyses::all();
    }

    auto &LI = AM.getResult<LoopAnalysis>(F);
    auto &SE = AM.getResult<ScalarEvolutionAnalysis>(F);

    // Reconstruct LLVM values from the json analysis report.
    SmallVector<LSQInfo> lsqArray;
    SmallVector<PEInfo> peArray;
    SmallVector<RewriteRule> rewriteRules;
    deserializeAnalysis(F, LI, report, lsqArray, peArray, rewriteRules);

    SetVector<Instruction *> toDeleteI, toKeepI;
    SetVector<BasicBlock *> toDeleteBB, toKeepBB;
    if (!isAGU)  // The address gen unit keeps equivalent ctrl flow.
      splitCode(F, peArray, toDeleteI, toKeepI, toDeleteBB, toKeepBB);

    // Tags for the ordering of LSQ requests and values. 
    for (auto &lsqInfo : lsqArray) {
      lsqInfo.stTagAddr = createTag(F, toKeepI);
      lsqInfo.ldTagAddr = createTag(F, toKeepI);
    }

    // Apply all rewrite rules for this kernel.
    for (auto &rule : rewriteRules) {
      /******************** LSQ related: */ 
      if (rule.ruleType == LD_REQ_WRITE)
        doLdReqWrite(rule, lsqArray[rule.lsqIdx], toKeepI);
      else if (rule.ruleType == ST_REQ_WRITE)
        doStReqWrite(rule, lsqArray[rule.lsqIdx], toKeepI);
      else if (rule.ruleType == LD_VAL_READ)
        doLdValRead(rule);
      else if (rule.ruleType == ST_VAL_WRITE)
        doStValWrite(rule, lsqArray[rule.lsqIdx], toKeepI);
      else if (rule.ruleType == POISON_LD_READ)
        doPoisonLdRead(rule);
      else if (rule.ruleType == POISON_ST_WRITE)
        doPoisonStWrite(rule, lsqArray[rule.lsqIdx], LI, toKeepI);
      else if (rule.ruleType == END_LSQ_SIGNAL_WRITE)
        doEndLsqSignalWrite(rule);
      /******************** PE related: */ 
      else if (rule.ruleType == PRED_BB_WRITE)
        doPredBbWrite(rule, peArray[rule.peIdx], toKeepI);
      else if (rule.ruleType == PRED_BB_READ)
        doPredBbRead(rule, peArray[rule.peIdx]);
      else if (rule.ruleType == SSA_BB_IN_WRITE)
        doSsaBbInWrite(rule, peArray[rule.peIdx], toKeepI);
      else if (rule.ruleType == SSA_BB_IN_READ)
        doSsaBbInRead(rule, peArray[rule.peIdx]);
      else if (rule.ruleType == SSA_BB_OUT_WRITE)
        doSsaBbOutWrite(rule, peArray[rule.peIdx], toKeepI);
      else if (rule.ruleType == SSA_BB_OUT_READ)
        doSsaBbOutRead(rule, peArray[rule.peIdx]);
      else if (rule.ruleType == PRED_LOOP_WRITE)
        doPredLoopWrite(rule, toKeepI);
      else if (rule.ruleType == PRED_LOOP_READ)
        doPredLoopRead(rule, peArray[rule.peIdx]);
      else if (rule.ruleType == SSA_LOOP_IN_WRITE)
        doSsaLoopInWrite(rule, toKeepI);
      else if (rule.ruleType == SSA_LOOP_IN_READ)
        doSsaLoopInRead(rule, peArray[rule.peIdx]);
      else if (rule.ruleType == SSA_LOOP_OUT_WRITE)
        doSsaLoopOutWrite(rule, peArray[rule.peIdx], toKeepI);
      else if (rule.ruleType == SSA_LOOP_OUT_READ)
        doSsaLoopOutRead(rule);
      /******************** Composition of PEs and LSQs related: */ 
      else if (rule.ruleType == ST_VAL_TAG_IN_READ)
        doStValTagInRead(rule, lsqArray[rule.lsqIdx], peArray[rule.peIdx], toKeepI);
      else if (rule.ruleType == ST_VAL_TAG_TO_BB_WRITE)
        doStValTagToBBWrite(rule, lsqArray[rule.lsqIdx], peArray[rule.peIdx], toKeepI);
      else if (rule.ruleType == POISON_PRED_BB_WRITE)
        doPoisonPredBbWrite(rule, lsqArray[rule.lsqIdx], toKeepI);
      else if (rule.ruleType == POISON_IN_BB_PE_LD_READ)
        doPoisonInBbPeLdRead(rule, peArray[rule.peIdx], toKeepI);
      else if (rule.ruleType == POISON_IN_BB_PE_ST_WRITE)
        doPoisonInBbPeStWrite(rule, lsqArray[rule.lsqIdx], peArray[rule.peIdx], toKeepI);
      else if (rule.ruleType == ST_VAL_TAG_TO_LOOP_WRITE)
        doStValTagToLoopWrite(rule, lsqArray[rule.lsqIdx], SE, toKeepI);
      else if (rule.ruleType == ST_VAL_TAG_LOOP_OUT_WRITE)
        doStValTagLoopOutWrite(rule, lsqArray[rule.lsqIdx], peArray[rule.peIdx], toKeepI);
    }

    mergePoisonBlocks(F);

    // After all rules executed, delete code decoupled out of this kernel. 
    deleteCode(F, isAGU, toDeleteI, toKeepI, toDeleteBB, toKeepBB);

    return PreservedAnalyses::none();
  }


  static bool isRequired() { return true; }

  void getAnalysisUsage(AnalysisUsage &AU) const {
    AU.addRequiredID(LoopAnalysis::ID());
    AU.addRequiredID(ScalarEvolutionAnalysis::ID());
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
// be able to recognize the pass via '-passes=pass-name'
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getElasticTransformPluginInfo();
}

} // end namespace llvm
