#include "CommonLLVM.h"

#include <regex>

using namespace llvm;

namespace llvm {

const std::string REPORT_ENV_NAME = "ELASTIC_PASS_REPORT";

enum PE_PREDICATE_CODES { EXECUTE, RESET, EXIT, POISON };

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
    auto pipeCall = getPipeCall(F, i2pInfo);
    assert(pipeCall && "JSON directive without a pipe instructions.");
    i2pInfo["llvm_pipe_call"] = reinterpret_cast<std::intptr_t>(pipeCall);

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
    // The st_val_tag directives have only a basic block idx.
    if (auto optBlockIdx = i2pInfo.getInteger("basic_block_idx")) {
      i2pInfo["llvm_basic_block"] = reinterpret_cast<std::intptr_t>(
          getChildWithIndex<Function, BasicBlock>(&F, *optBlockIdx));
    }
    // Loop exit block for PREDICATE::RESET pipe writes.
    if (auto optBlockIdx = i2pInfo.getInteger("loop_exit_block_idx")) {
      i2pInfo["llvm_loop_exit_block"] = reinterpret_cast<std::intptr_t>(
          getChildWithIndex<Function, BasicBlock>(&F, *optBlockIdx));
    }
    if (auto optBlockIdx = i2pInfo.getInteger("loop_header_block_idx")) {
      i2pInfo["llvm_loop_header_block"] = reinterpret_cast<std::intptr_t>(
          getChildWithIndex<Function, BasicBlock>(&F, *optBlockIdx));
    }

    directives.push_back(std::move(i2pInfo));
  }

  return directives;
}

// Delete all store instructions in F (except exceptions).
void deleteAllStores(Function &F, SetVector<Instruction *> &exceptions) {
  // First collect then delete. Cannot delete I when iterating over I's in BB.
  SmallVector<Instruction *> toDelete;
  for (auto &BB : F) {
    for (auto &I : BB) {
      if (isaStore(&I) && I.getParent() && !exceptions.contains(&I)) 
        toDelete.push_back(&I);
    }
  }
  for (auto &I : toDelete)
    deleteInstruction(I);
}

/// Delete all instrucions in {BB}, excluding {except} and pipe calls.
void deleteNonPipeInstrsInBB(BasicBlock *BB,
                             SmallVector<Instruction *> except = {}) {
  SmallVector<Instruction *> toRemove;
  for (auto &I : *BB) {
    // Also check if not a pipe call. We might have moved some pipe calls out of
    // the decoupled BB during hoisting.
    if (!llvm::is_contained(except, &I) && !getPipeCall(&I))
      toRemove.push_back(&I);
  }

  for (auto I : toRemove) 
    deleteInstruction(I);
}

void deletePhiNodes(BasicBlock *BB) {
  SmallVector<Instruction *> toRemove;
  for (auto &I : *BB) 
    if (isa<PHINode>(&I))
      toRemove.push_back(&I);

  for (auto I : toRemove) 
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
    } else if (auto stInstr = dyn_cast<StoreInst>(user)) {
      storesToStruct.push_back(stInstr);
      break;
    }
  }

  // The struct stores were collected in reverse above.
  std::reverse(storesToStruct.begin(), storesToStruct.end());
  return storesToStruct;
}

/// Create an int32 val using alloca at the beginning of {F} and initialize it 
/// with {initVal}. Return its address.
Value *createTag(Function &F, SetVector<Instruction *> &created) {
  IRBuilder<> Builder(F.getEntryBlock().getTerminator());
  // LLVM doesn't make a distinction between signed and unsigned. And the only
  // op done on tags is 2's complement addition, so the bit pattern is the same.
  auto tagType = Type::getInt32Ty(F.getContext());
  Value *tagAddr = Builder.CreateAlloca(tagType);
  auto initStore = Builder.CreateStore(ConstantInt::get(tagType, 0), tagAddr);
  created.insert(dyn_cast<Instruction>(tagAddr));
  created.insert(initStore);
  return tagAddr;
}

void instr2pipeLsqLdReq(json::Object &i2pInfo, Value *stTagAddr,
                        Value *ldTagAddr, SetVector<Instruction *> &created) {
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
  if (isBRAM && *i2pInfo.getInteger("num_load_pipes") > 1) {
    // A load tag is first used in the load request.
    ldTagStore->moveBefore(pipeWrite);
    Builder.SetInsertPoint(ldTagStore);
    Value *ldTagVal = Builder.CreateLoad(tagType, ldTagAddr, "baseLdTagVal");
    ldTagStore->setOperand(0, ldTagVal);
    // And then incremented.
    Builder.SetInsertPoint(pipeWrite);
    auto ldTagInc = Builder.CreateAdd(ldTagVal, ConstantInt::get(tagType, 1));
    auto storeForNewLdTag = Builder.CreateStore(ldTagInc, ldTagAddr);
    created.insert(ldTagStore);
    created.insert(storeForNewLdTag);
  }

  created.insert(stTagStore);
  created.insert(addressStore);
}

void instr2pipeLsqStReq(json::Object &i2pInfo, Value *stTagAddr,
                        SetVector<Instruction *> &created) {
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
  pipeWrite->moveAfter(I);
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

  created.insert(storeForNewStTag);
  created.insert(stTagStore);
  created.insert(addressStore);
}

/// Given a load instruction, swap it for a pipe read.
void instr2pipeLdVal(json::Object &i2pInfo) {
  auto pipeRead =
      reinterpret_cast<CallInst *>(*i2pInfo.getInteger("llvm_pipe_call"));
  auto I =
      reinterpret_cast<Instruction *>(*i2pInfo.getInteger("llvm_instruction"));
  // Move after I. Ensures ld_val comes after ld_req in case when both the
  // ld_val and ld_req pipes are in the same kernel.
  pipeRead->moveAfter(I);
  Value *loadVal = dyn_cast<Value>(I);
  loadVal->replaceAllUsesWith(pipeRead);
}

/// Given a store instuction, swap it for a tagged value pipe write.
void instr2pipeStVal(json::Object &i2pInfo, Value *tagAddr,
                     SetVector<Instruction *> &created) {
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
  created.insert(validStore);

  // If multiple store values are written in one BB, then add a tag for muxing.
  if (*i2pInfo.getInteger("num_store_pipes") > 1) {
    IRBuilder<> IR(I);
    StoreInst *tagStore = stValStructStores[1];
    auto tagType = tagStore->getOperand(0)->getType();
    LoadInst *tagVal = IR.CreateLoad(tagType, tagAddr);
    auto tagPlusOne = IR.CreateAdd(tagVal, ConstantInt::get(tagType, 1));
    auto tagStoreToTag = IR.CreateStore(tagPlusOne, tagAddr);
    auto tagStoreToPipe = IR.CreateStore(tagPlusOne, tagStore->getOperand(1));

    created.insert(tagStoreToTag);
    created.insert(tagStoreToPipe);
  }

  // Instead of storing value to memory, store into the valStore struct member.
  pipeWrite->moveAfter(I);
  if (*i2pInfo.getBoolean("is_address_decoupled")) {
    // The "I" store will no longer be needed.
    I->setOperand(1, valStore->getOperand(1));
  } else {
    // The "I" store will be used by the stReq transformation. Don't change it.
    auto storeValueClone = I->clone();
    storeValueClone->insertBefore(pipeWrite);
    storeValueClone->setOperand(1, valStore->getOperand(1));
  }
}

/// Communicate the store_val tag across decoupled kernels.
void instr2pipeStValTag(Function &F, LoopInfo &LI, ScalarEvolution &SE,
                        json::Object &i2pInfo, Value *tagAddr,
                        SetVector<Instruction *> &created) {
  auto pipeOp =
      reinterpret_cast<CallInst *>(*i2pInfo.getInteger("llvm_pipe_call"));

  std::string thisKernelName = demangle(std::string(F.getNameOrAsOperand()));
  bool inLoopPE = thisKernelName.find("_PE_LOOP_") < thisKernelName.size();
  // In a loop PE, the stValPipe read needs to be in the preheader.
  auto pipeBB = (inLoopPE && (i2pInfo.getString("read/write") == "read"))
      ? F.getEntryBlock().getSingleSuccessor()
      : reinterpret_cast<BasicBlock *>(*i2pInfo.getInteger("llvm_basic_block"));

  // Move pipe to the start of the target BB. 
  pipeOp->moveBefore(pipeBB->getFirstNonPHI());

  IRBuilder<> IR(pipeOp);
  if (i2pInfo.getString("read/write") == "read") {
    // Read tag from pipe and store into tagAddr.
    auto tagSt = IR.CreateStore(pipeOp, tagAddr);
    tagSt->moveAfter(pipeOp);
    created.insert(tagSt);
  } else {
    // Load tag from tag addr and store into pipe 
    auto stIntoTagPipe = getPipeOpStructStores(pipeOp)[0];
    auto tagType = stIntoTagPipe->getOperand(0)->getType();
    auto tagValLd = IR.CreateLoad(tagType, tagAddr);
    stIntoTagPipe->moveAfter(tagValLd);
    stIntoTagPipe->setOperand(0, tagValLd);
    created.insert(stIntoTagPipe);

    if (i2pInfo.getString("pe_type") == "loop" && !inLoopPE) {
      // Since the loopPE has a stVal pipe read in PE header that will be called 
      // every time a pred is read, we need to supply a dummy tag on func exit.
      pipeOp->clone()->insertBefore(getReturnBlock(F)->getFirstNonPHI());

      // Sometimes we can just increment the tag in the main kernel by the
      // number of total stores that will happen in the decoupled loop.
      if (*i2pInfo.getBoolean("can_build_num_stores_exp")) {
        int numStoresInL = *i2pInfo.getInteger("stores_in_loop");
        auto numIterExpr = buildSCEVExpr(
            F, SE.getBackedgeTakenCount(LI.getLoopFor(pipeBB)), pipeOp);
        auto totalStoresInL =
            IR.CreateMul(ConstantInt::get(tagType, numStoresInL), numIterExpr);

        // Add the total to the tag.
        auto newTagVal = IR.CreateAdd(tagValLd, totalStoresInL);
        created.insert(IR.CreateStore(newTagVal, tagAddr));
        created.insert(dyn_cast<Instruction>(newTagVal));
      }
    }

    // When communicating a st_val_tag to a decoupled basic block, we can
    // directly increment the tag in the main kernel by the number of stores in
    // the dcpldBB (instead of synthesizing another PE --> Main pipe op).
    if (auto numStoresinBBOpt = i2pInfo.getInteger("stores_in_bb")) {
      // create add
      auto incr = ConstantInt::get(tagType, *numStoresinBBOpt);
      auto newTagVal = IR.CreateAdd(tagValLd, incr);
      created.insert(IR.CreateStore(newTagVal, tagAddr));
      created.insert(dyn_cast<Instruction>(newTagVal));
    } 
  }
}

/// Create a new basic block with a poison pipe to the LSQ. 
void instr2pipePoisonAlloc(json::Object &i2pInfo, Value *tagAddr,
                           SetVector<Instruction *> &created) {
  // Map of CFG edges to poison basic blocks that already have been created.
  static MapVector<std::pair<BasicBlock *, BasicBlock *>, BasicBlock *>
      createdBlocks;
  // For each CFG edge, at most one stValTag pipe write.
  static SetVector<std::pair<BasicBlock *, BasicBlock *>> stValTagWriteDone;

  auto pipeCall =
      reinterpret_cast<CallInst *>(*i2pInfo.getInteger("llvm_pipe_call"));
  
  // Create BB or get one that already exists on pred-->succ edge
  auto predBB = reinterpret_cast<BasicBlock *>(
      *i2pInfo.getInteger("llvm_pred_basic_block"));
  auto succBB = reinterpret_cast<BasicBlock *>(
      *i2pInfo.getInteger("llvm_succ_basic_block"));
  BasicBlock *poisonBB;
  IRBuilder<> Builder(predBB->getContext());
  if (!createdBlocks.contains({predBB, succBB})) {
    poisonBB = BasicBlock::Create(predBB->getContext(), "Poison",
                                  predBB->getParent(), succBB);
    createdBlocks[{predBB, succBB}] = poisonBB;

    // Insert BB on pred-->succ edge
    // Before changing edges, make all phis from predBB now come from PoisonBB
    predBB->replaceSuccessorsPhiUsesWith(predBB, poisonBB);
    Builder.SetInsertPoint(poisonBB);
    Builder.CreateBr(succBB);
    auto branchInPredBB = dyn_cast<BranchInst>(predBB->getTerminator());
    branchInPredBB->replaceSuccessorWith(succBB, poisonBB);
  } else {
    poisonBB = createdBlocks[{predBB, succBB}];
  }

  pipeCall->moveBefore(poisonBB->getTerminator());

  // Special case when the BB where the speculation is true gets decoupled into
  // a separate kernel. In that case we send a predicated::poison and stValTag.
  if (i2pInfo.getString("directive_type") == "pred_poison") {
    auto predType = Type::getInt8Ty(pipeCall->getContext());
    auto predStore = storeValIntoPipe(
        ConstantInt::get(predType, PE_PREDICATE_CODES::POISON), pipeCall);
    created.insert(predStore);

    // Also need to send tag, if its used in the LSQ, but at most once. 
    std::pair<BasicBlock *, BasicBlock *> edgeCFG {predBB, succBB};
    if (!stValTagWriteDone.contains(edgeCFG) &&
        *i2pInfo.getInteger("num_store_pipes") > 1) {
      // Just copy the pipe from the "true block".
      int lsqId = *i2pInfo.getInteger("lsq_id");
      auto stValTagPipeName = "st_val_tag_in_lsq_" + std::to_string(lsqId);
      auto stValTagPipe = dyn_cast<CallInst>(
          getPipeWithPattern(*poisonBB->getParent(), stValTagPipeName)
              ->clone());
      stValTagPipe->insertBefore(&poisonBB->front());
      
      // Load tag from tag addr and store into pipe
      Builder.SetInsertPoint(stValTagPipe);
      auto stIntoTagPipe = getPipeOpStructStores(stValTagPipe)[0]->clone();
      auto tagType = stIntoTagPipe->getOperand(0)->getType();
      auto tagValLd = Builder.CreateLoad(tagType, tagAddr);
      stIntoTagPipe->insertAfter(tagValLd);
      stIntoTagPipe->setOperand(0, tagValLd);
      created.insert(stIntoTagPipe);

      // When communicating a st_val_tag to a decoupled basic block, we can
      // directly increment the tag in the main kernel by the number of stores
      // in the dcpldBB (instead of synthesizing another PE --> Main pipe op).
      if (auto numStoresinBBOpt = i2pInfo.getInteger("stores_in_bb")) {
        // create add
        auto incr = ConstantInt::get(tagType, *numStoresinBBOpt);
        auto newTagVal = Builder.CreateAdd(tagValLd, incr);
        created.insert(Builder.CreateStore(newTagVal, tagAddr));
        created.insert(dyn_cast<Instruction>(newTagVal));
      }

      stValTagWriteDone.insert(edgeCFG);
    }
  } else if (*i2pInfo.getInteger("num_store_pipes") > 1) {
    // If multiple store values are written in one BB, add a tag for muxing.
    auto stValPipeStructStores = getPipeOpStructStores(pipeCall);
    Builder.SetInsertPoint(pipeCall);
    StoreInst *tagStore = stValPipeStructStores[1];
    auto tagType = tagStore->getOperand(0)->getType();
    LoadInst *tagVal = Builder.CreateLoad(tagType, tagAddr);
    auto tagPlusOne = Builder.CreateAdd(tagVal, ConstantInt::get(tagType, 1));
    auto tagStoreToTag = Builder.CreateStore(tagPlusOne, tagAddr);
    auto tagStoreToPipe = Builder.CreateStore(tagPlusOne, tagStore->getOperand(1));

    created.insert(tagStoreToTag);
    created.insert(tagStoreToPipe);
  }
}

/// Add a poison BB to a block predicated PE.
void instr2pipePoisonAllocInPE(json::Object &i2pInfo, Value *tagAddr,
                               SetVector<Instruction *> &created) {
  // Ensure only one poison block is created in the PE (by definition only 1).
  static BasicBlock *poisonBlock = nullptr;
  // For each LSQ used by this PE, at most one stValTag read.
  static SetVector<int> stValTagReadForLSQ;

  auto pipePoisonCall =
      reinterpret_cast<CallInst *>(*i2pInfo.getInteger("llvm_pipe_call"));
  auto F = pipePoisonCall->getFunction();
  
  //                      ↓~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~¬
  //    funcEntry -> preHeader -> header <-> decoupledBB -> exiting -> funcExit
  //                                      \~~~~~~~~~~~~~~~~~~~↑
  //    <Insert poisonBB between header-exiting> --^
  auto header = getChildWithIndex<Function, BasicBlock>(F, 2);
  auto exiting = getChildWithIndex<Function, BasicBlock>(F, 4);
  auto predPipe = dyn_cast<Instruction>(header->getTerminator()->getOperand(0))
                      ->getOperand(0);

  // Create BB or get one that already exists on pred-->succ edge
  IRBuilder<> Builder(F->getContext());
  if (!poisonBlock) {
    // Add the new blocks at the end of F blocks list.
    auto beforeExiting = BasicBlock::Create(F->getContext(), "beforeExiting", F,
                                            getReturnBlock(*F));
    poisonBlock = BasicBlock::Create(F->getContext(), "poison", F,
                                  getReturnBlock(*F));
    header->getTerminator()->setOperand(1, beforeExiting);
    Builder.SetInsertPoint(beforeExiting);
    auto isPoison = Builder.CreateICmpEQ(
        predPipe, ConstantInt::get(Type::getInt8Ty(predPipe->getContext()),
                                   PE_PREDICATE_CODES::POISON));
    Builder.CreateCondBr(isPoison, poisonBlock, exiting);
    Builder.SetInsertPoint(poisonBlock);
    Builder.CreateBr(header);

  }

  // Create poison read/write. A poison write just doesn't set the st_val valid
  // bit. A read just discard its value (i.e. SSA value is not used).
  pipePoisonCall->moveBefore(poisonBlock->getTerminator());

  // If multiple store values are written in one BB, add a tag for muxing.
  if (*i2pInfo.getString("read/write") == "write" &&
      *i2pInfo.getInteger("num_store_pipes") > 1) {
    // Read tag, if used in LSQ and not read yet. 
    int lsqId = *i2pInfo.getInteger("lsq_id");
    if (!stValTagReadForLSQ.contains(lsqId)) {
      // Just copy the pipe from the "dcpld block".
      auto stValTagPipeName = "st_val_tag_in_lsq_" + std::to_string(lsqId);
      auto stValTagPipeRead = dyn_cast<CallInst>(
          getPipeWithPattern(*poisonBlock->getParent(), stValTagPipeName)
              ->clone());
      stValTagPipeRead->insertBefore(&poisonBlock->front());
      auto tagSt = Builder.CreateStore(stValTagPipeRead, tagAddr);
      tagSt->moveAfter(stValTagPipeRead);
      created.insert(tagSt);

      stValTagReadForLSQ.insert(lsqId);
    }

    // Use tag for the posion st_val write.
    auto stValPipeStructStores = getPipeOpStructStores(pipePoisonCall);
    Builder.SetInsertPoint(pipePoisonCall);
    StoreInst *tagStore = stValPipeStructStores[1];
    auto tagType = tagStore->getOperand(0)->getType();
    LoadInst *tagVal = Builder.CreateLoad(tagType, tagAddr);
    auto tagPlusOne = Builder.CreateAdd(tagVal, ConstantInt::get(tagType, 1));
    auto tagStoreToTag = Builder.CreateStore(tagPlusOne, tagAddr);
    auto tagStoreToPipe =
        Builder.CreateStore(tagPlusOne, tagStore->getOperand(1));

    created.insert(tagStoreToTag);
    created.insert(tagStoreToPipe);
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

void instr2pipeSsaPe(json::Object &i2pInfo, SetVector<Instruction *> &created) {
  // Keep track of instructions already replaced by a pipe read.
  static MapVector<Instruction *, Instruction *> instruction2pipe;

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
      created.insert(pipeReadCasted);
      instruction2pipe[I] = pipeReadCasted;
    } else {
      I->replaceAllUsesWith(pipeCall);
      instruction2pipe[I] = pipeCall;
    }

  } else {
    // If an instruction has been replaced by a pipe read, then use that.
    if (instruction2pipe.contains(I))
      I = instruction2pipe[I];

    pipeCall->moveBefore(decoupledBB->getTerminator());
    created.insert(storeValIntoPipe(I, pipeCall));
  } 
}

void instr2pipeSsaMain(json::Object &i2pInfo,
                       SetVector<Instruction *> &created) {
  // Keep track of instructions already replaced by a pipe read.
  static MapVector<Instruction *, Instruction *> instruction2pipe;

  auto pipeCall =
      reinterpret_cast<CallInst *>(*i2pInfo.getInteger("llvm_pipe_call"));
  auto I =
      reinterpret_cast<Instruction *>(*i2pInfo.getInteger("llvm_instruction"));
  auto F = I->getFunction();
  auto decoupledBB = getChildWithIndex<Function, BasicBlock>(
      F, *i2pInfo.getInteger("basic_block_idx"));

  if (i2pInfo.getString("read/write") == "read") {
    pipeCall->moveBefore(decoupledBB->getTerminator());
    I->replaceAllUsesWith(pipeCall);
    instruction2pipe[I] = pipeCall;
  } else {
    pipeCall->moveBefore(getFirstAfterAllPipes(decoupledBB));

    // If an instruction has been replaced by a pipe read, then use that.
    if (instruction2pipe.contains(I))
      I = instruction2pipe[I];

    // Check if we need to cast, e.g. when communicating an address.
    auto pipeOpType =
        pipeCall->getOperand(0)->getType()->getNonOpaquePointerElementType();
    if (I->getType() != pipeOpType) {
      auto ICasted =
          BitCastInst::CreateBitOrPointerCast(I, pipeOpType, "", pipeCall);
      created.insert(ICasted);
      created.insert(storeValIntoPipe(ICasted, pipeCall));
    } else {
      created.insert(storeValIntoPipe(I, pipeCall));
    }

    // If this pipe call writes an input dep to a loop PE, then there will be 
    // one superflous read of this pipe in the loop PE. Match it with this call. 
    if (*i2pInfo.getString("pe_type") == "loop") 
      pipeCall->clone()->insertBefore(getReturnBlock(*F)->getTerminator());
  }
}

/// Given a basic block wrap it in the following control flow:
///                      ↓~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~¬
///    funcEntry -> preHeader -> header <-> decoupledBB -> exiting -> funcExit
///                                      \~~~~~~~~~~~~~~~~~~~↑
/// - the preHeader will read any hoisted out input dependencies (i.e. data
///   communicated once before the loop of the decoupledBB executes).
/// - the header reads the predicate, and goes to dcpldBB or exitingBB.
/// - the dcpld bb has a backedge to the header.
/// - the exiting BB exits the function (predicate carries EXIT), or takes the 
///   backedge to the preHeader (predicate carries RESET).
void createPredicatedBlockPE(json::Object &i2pInfo) {
  auto predPipeRead =
      reinterpret_cast<CallInst *>(*i2pInfo.getInteger("llvm_pipe_call"));
  auto predType = Type::getInt8Ty(predPipeRead->getContext());
  auto F = predPipeRead->getFunction();
  auto entryBB = &F->getEntryBlock();
  auto exitBB = getReturnBlock(*F);
  auto dcpldBB = getChildWithIndex<Function, BasicBlock>(
      F, *i2pInfo.getInteger("basic_block_idx"));
    
  SmallVector<BasicBlock *> blocksToDelete;
  for (auto &BB : *F) {
    if (dcpldBB != &BB && entryBB != &BB && exitBB != &BB)
      blocksToDelete.push_back(&BB);
  }

  auto headerBB = BasicBlock::Create(F->getContext(), "header", F, dcpldBB);
  auto preHeaderBB =
      BasicBlock::Create(F->getContext(), "preHeader", F, headerBB);
  auto exitingBB = BasicBlock::Create(F->getContext(), "exiting", F, exitBB);

  // entry -> preHeader
  dyn_cast<BranchInst>(entryBB->getTerminator())->setSuccessor(0, preHeaderBB);

  // preHeader -> header
  IRBuilder<> Builder(preHeaderBB);
  Builder.CreateBr(headerBB);

  // header -> body | header -> exiting
  Builder.SetInsertPoint(headerBB);
  auto isExec = Builder.CreateICmpEQ(predPipeRead, 
      ConstantInt::get(predType, PE_PREDICATE_CODES::EXECUTE));
  Builder.CreateCondBr(isExec, dcpldBB, exitingBB);
  predPipeRead->moveBefore(&headerBB->front());

  // body -> header
  Builder.SetInsertPoint(dcpldBB);
  auto oldBranch = dcpldBB->getTerminator();
  Builder.CreateBr(headerBB);
  deleteInstruction(oldBranch);

  // exiting -> preHeader | exiting -> exit
  Builder.SetInsertPoint(exitingBB);
  auto isReset = Builder.CreateICmpEQ(predPipeRead,
      ConstantInt::get(predType, PE_PREDICATE_CODES::RESET));
  Builder.CreateCondBr(isReset, preHeaderBB, exitBB);

  // Delete the collected blocks and the instructions within them.
  for (auto BB : blocksToDelete) {
    deleteNonPipeInstrsInBB(BB);
    BB->removeFromParent();
  }
  deletePhiNodes(dcpldBB);
}

/// Given a loop header BB, wrap the entire loop in the following control flow:
///    funcEntry -> headerPE <-> decoupledLoop -> funcExit
///                           \~~~~~~~~~~~~~~~~~~~~~~↑
/// - the headerPE reads all input dependencies and the predicate. Based on the 
///   predicate, it goes to the decoupled loop or to the function exit.
/// - the only change in the dcpldLoop control flow is that the loop exit block
///   transfers control to the headerPE bb.
/// - the decoupled loop exit block also writes out any dependenciesOut.
void createPredicatedLoopPE(json::Object &i2pInfo, LoopInfo &LI) {
  auto predPipeRead =
      reinterpret_cast<CallInst *>(*i2pInfo.getInteger("llvm_pipe_call"));
  auto predType = Type::getInt8Ty(predPipeRead->getContext());
  auto F = predPipeRead->getFunction();
  auto headerOfDcpldLoop = getChildWithIndex<Function, BasicBlock>(
      F, *i2pInfo.getInteger("basic_block_idx"));
  auto L = LI.getLoopFor(headerOfDcpldLoop);
  auto funcEntry = &F->getEntryBlock();
  auto funcExit = getReturnBlock(*F);
  auto loopExit = reinterpret_cast<BasicBlock *>(
      *i2pInfo.getInteger("llvm_loop_exit_block"));

  SmallVector<BasicBlock *> blocksToDelete;
  SmallVector<BasicBlock *> blocksToKeep{headerOfDcpldLoop, funcEntry,
                                         funcExit, loopExit};
  llvm::append_range(blocksToKeep, L->getBlocksVector());
  for (auto &BB : *F) 
    if (!llvm::is_contained(blocksToKeep, &BB)) 
      blocksToDelete.push_back(&BB);

  auto headerPE =
      BasicBlock::Create(F->getContext(), "header", F, headerOfDcpldLoop);

  // entry -> headerPE
  dyn_cast<BranchInst>(funcEntry->getTerminator())->setSuccessor(0, headerPE);

  // headerPE -> dcpldLoop | headerPE -> funcExit
  IRBuilder<> Builder(headerPE);
  Builder.SetInsertPoint(headerPE);
  auto isExec = Builder.CreateICmpEQ(predPipeRead, 
      ConstantInt::get(predType, PE_PREDICATE_CODES::EXECUTE));
  Builder.CreateCondBr(isExec, headerOfDcpldLoop, funcExit);
  predPipeRead->moveBefore(&headerPE->front());

  // loopExit -> headerPE. (if the loop exit is the func exit, then create BB)
  if (loopExit == funcExit) {
    loopExit = BasicBlock::Create(F->getContext(), "loopExit", F, funcExit);
    Builder.SetInsertPoint(loopExit);
    Builder.CreateBr(headerPE);
    headerOfDcpldLoop->getTerminator()->setOperand(1, loopExit);
  } else {
    Builder.SetInsertPoint(loopExit);
    auto oldBranch = loopExit->getTerminator();
    Builder.CreateBr(headerPE);
    deleteInstruction(oldBranch);
  }

  // Pipe read/writes with dependencies should only be called once per entire
  // decoupled loop exec. So move them to headerPE/decoupledLoopExit.
  SmallVector<Instruction *> moveToHeader, moveToLoopExit;
  // All depIn/out will be in the headerOfDcpldLoop.
  for (auto &I : *headerOfDcpldLoop) {
    if (isPipeRead(&I)) {
      moveToHeader.push_back(&I);
    } else if (isPipeWrite(&I)) {
      // Also move the store into the pipe.
      if (isaStore(I.getPrevNode()))
        moveToLoopExit.push_back(I.getPrevNode());
      moveToLoopExit.push_back(&I);
    }
  }
  for (auto I : moveToHeader) 
    I->moveBefore(&headerPE->front());
  for (auto I : moveToLoopExit) 
    I->moveBefore(loopExit->getTerminator());

  // Update phi sources in decoupledLoopHeaderBB.
  for (auto &phi : headerOfDcpldLoop->phis()) {
    for (auto srcBB : phi.blocks()) {
      if (llvm::is_contained(blocksToDelete, srcBB) ||
          srcBB == &F->getEntryBlock()) {
        headerOfDcpldLoop->replacePhiUsesWith(srcBB, headerPE);
      }
    }
  }

  // Delete the collected blocks and the instructions within them.
  for (auto BB : blocksToDelete) {
    deleteNonPipeInstrsInBB(BB);
    BB->removeFromParent();
  }
}

void addPredicateWrites(json::Object &i2pInfo, LoopInfo &LI,
                        SetVector<Instruction *> &created) {
  auto predPipeWrite =
      reinterpret_cast<CallInst *>(*i2pInfo.getInteger("llvm_pipe_call"));
  auto predType = Type::getInt8Ty(predPipeWrite->getContext());
  auto dcpldBB =
      reinterpret_cast<BasicBlock *>(*i2pInfo.getInteger("llvm_basic_block"));
  auto loopExitBlock = reinterpret_cast<BasicBlock *>(
      *i2pInfo.getInteger("llvm_loop_exit_block"));
  auto funcExitBlock = getReturnBlock(*dcpldBB->getParent());
  bool isLoopPE = (*i2pInfo.getString("pe_type") == "loop");

  // Write PRED::EXEC in the dcpldBB. 
  predPipeWrite->moveBefore(dcpldBB->getFirstNonPHI());
  auto gotoBodySt = storeValIntoPipe(
      ConstantInt::get(predType, PE_PREDICATE_CODES::EXECUTE), predPipeWrite);
  created.insert(gotoBodySt);

  // Write PRED::EXIT in the function exit block.
  auto predPipeExit = dyn_cast<CallInst>(predPipeWrite->clone());
  predPipeExit->insertBefore(&funcExitBlock->front());
  auto exitPredSt = storeValIntoPipe(
      ConstantInt::get(predType, PE_PREDICATE_CODES::EXIT), predPipeExit);
  created.insert(exitPredSt);

  // Write PRED::RESET in the loop exit block of the dcpldBB. 
  // This is only needed when decoupling a single BB sitting in a nested loop.
  if (loopExitBlock != funcExitBlock && !isLoopPE) {
    auto predPipeReset = dyn_cast<CallInst>(predPipeWrite->clone());
    predPipeReset->insertBefore(&loopExitBlock->front());
    auto resetPredSt = storeValIntoPipe(
        ConstantInt::get(predType, PE_PREDICATE_CODES::RESET), predPipeReset);
    created.insert(resetPredSt);
  }
}

/// Given dependencyIn/Out directives hoist the associated pipe calls out of the 
/// decoupled BB into the loop preheader (if read), or the exitingBB (if write).
void hoistPipesBlockPE(Function &F, json::Array &directives) {
  auto preHeaderBB = getChildWithIndex<Function, BasicBlock>(&F, 1);
  auto headerBB = getChildWithIndex<Function, BasicBlock>(&F, 2);
  auto decoupledBB = getChildWithIndex<Function, BasicBlock>(&F, 3);
  auto exitingBB = getChildWithIndex<Function, BasicBlock>(&F, 4);

  for (const json::Value &i2pInfoVal : directives) {
    auto i2pInfo = *i2pInfoVal.getAsObject();
    assert(i2pInfo.getString("pe_type") != "loop" &&
           "Loop PEs cannot have dependency pipe read/writes hoisted.");

    auto pipeCall =
        reinterpret_cast<CallInst *>(*i2pInfo.getInteger("llvm_pipe_call"));
    
    if (i2pInfo.getString("read/write") == "read") {
      pipeCall->moveBefore(preHeaderBB->getTerminator());
    } else {
      auto recStartPipeRead = reinterpret_cast<Instruction *>(
          *i2pInfo.getInteger("llvm_recurrence_start_pipe_read"));

      auto storeIntoPipe = pipeCall->getPrevNode();
      auto storedValue = storeIntoPipe->getOperand(0);
      pipeCall->moveBefore(&exitingBB->front());
      storeIntoPipe->moveBefore(pipeCall);

      // If the {storedValue} is used by a phi recurrence start, then we need to
      // recreate the recurrence start phi in the PE.
      if (*i2pInfo.getBoolean("used_by_recurrence_start")) {
        auto recPhi =
            PHINode::Create(storedValue->getType(), 2, "hoistedRecurrence",
                            &headerBB->front());
        recPhi->addIncoming(recStartPipeRead, preHeaderBB);
        recPhi->addIncoming(storedValue, decoupledBB);
        storeIntoPipe->setOperand(0, recPhi);

        recStartPipeRead->replaceUsesOutsideBlock(recPhi, headerBB);
      }
    }
  }

  // Adjust phis in header if there is a posionBB (values don't change).
  if (auto poisonBB = getChildWithIndex<Function, BasicBlock>(&F, 6)) {
    for (auto &phi : headerBB->phis()) 
      phi.addIncoming(dyn_cast<Value>(&phi), poisonBB);
  }
}

/// Hoist a pipe write to loop header, or a pipe read to loop exit blocks.
void hoistPipesMain(json::Array &directives, LoopInfo &LI, bool isLoopPE) {
  for (const json::Value &i2pInfoVal : directives) {
    auto i2pInfo = *i2pInfoVal.getAsObject();

    assert(i2pInfo.getString("pe_type") != "loop" &&
           "Loop PEs cannot have pipe read/writes hoisted at the moment.");

    auto isPipeRead = (i2pInfo.getString("read/write") == "read");
    auto pipeCall =
        reinterpret_cast<CallInst *>(*i2pInfo.getInteger("llvm_pipe_call"));
    auto recStart = reinterpret_cast<PHINode *>(
        *i2pInfo.getInteger("llvm_recurrence_start"));
    auto decoupledBB = pipeCall->getParent();
    auto F = pipeCall->getFunction();
    auto L = LI.getLoopFor(decoupledBB);
    // If inside a loop pe, then the original loop preheader comes after entry.
    auto loopPreHeader = isLoopPE ? F->getEntryBlock().getSingleSuccessor()
                                  : L->getLoopPreheader();
    auto loopHeader = reinterpret_cast<BasicBlock *>(
        *i2pInfo.getInteger("llvm_loop_header_block"));
    auto loopExit = reinterpret_cast<BasicBlock *>(
        *i2pInfo.getInteger("llvm_loop_exit_block"));
    auto funcExit = getReturnBlock(*F);
    
    if (!isPipeRead) {
      // Supply initial value to the PE before entering loop. 
      auto initVal = recStart->DoPHITranslation(loopHeader, loopPreHeader);

      // Move pipe to loop exit and store {initVal} in it.
      Instruction *storeIntoPipe = pipeCall->getPrevNode();
      assert(isaStore(storeIntoPipe) && "Exprected store into pipe here.");
      pipeCall->moveBefore(loopPreHeader->getTerminator());
      storeIntoPipe->setOperand(0, initVal);
      storeIntoPipe->moveBefore(pipeCall);

      if (loopExit != funcExit) 
        pipeCall->clone()->insertBefore(funcExit->getFirstNonPHI());
    } else {
      // The pipe read should happen after the pred::GOTO_EXIT/PREHEADER call.
      std::string thisPipeName = std::string(*i2pInfo.getString("pipe_name"));
      auto correspondingPredPipe =
          std::regex_replace(thisPipeName, std::regex("dep_out_.*"), "pred");
      
      pipeCall->moveAfter(getPipeWithPattern(*loopExit, correspondingPredPipe));
      recStart->replaceAllUsesWith(pipeCall);
      if (auto recEnd = dyn_cast<Instruction>(
              recStart->DoPHITranslation(loopHeader, L->getLoopLatch()))) {
        deleteInstruction(recEnd);
      }
      deleteInstruction(recStart);

      // For every predicate call, read the recurrence out.
      if (loopExit != funcExit) {
        pipeCall->clone()->insertBefore(funcExit->getTerminator());
      }
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
    if (i2pInfo.getString("directive_type") != "ssa_block") 
      continue;

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

/// Collect instructions that will be decoupled into a PE.
void getInstrToDecoupleAndToKeep(Function &F, LoopInfo &LI,
                                 const json::Object &report,
                                 SetVector<Instruction *> &toDecouple,
                                 SetVector<Instruction *> &toKeep) {
  std::string thisKernelName = demangle(std::string(F.getNameOrAsOperand()));

  SetVector<BasicBlock *> blockPEs;
  for (auto peInfoVal : *report.getArray("pe_array")) {
    auto peInfo = *peInfoVal.getAsObject();
    auto dcpldBB = getChildWithIndex<Function, BasicBlock>(
      &F, *peInfo.getInteger("basic_block_idx"));

    auto &targetVector =
        (peInfo["pe_kernel_name"] == thisKernelName) ? toKeep : toDecouple;

    for (auto &I : *dcpldBB) 
      if (!I.isTerminator()) // Leave control flow.
        targetVector.insert(&I);

    // If the decoupled PE is a loop, then collect from other blocks as well.
    if (peInfo["pe_type"] == "loop") {
      auto L = LI.getLoopFor(dcpldBB);
      for (auto BB : L->getBlocksVector()) {
        if (!blockPEs.contains(BB)) {
          for (auto &I : *BB) {
            if (!I.isTerminator()) 
              targetVector.insert(&I);
          }
        }
      }
    } else {
      // Keep track of which blocks are decoupled into a PE, so that their
      // instructions are not added to {toKeep} of a loop PE.
      blockPEs.insert(dcpldBB);
    }
  }
}

/// Delete all basic blocks that belong to decoupled loops.
void deleteDecoupledLoopBodies(Function &F, LoopInfo &LI,
                               const json::Object &report) {
  // First, collect loop headers, loop exits, and all block to delete.
  SmallVector<BasicBlock *> loopHeaders;
  SmallVector<BasicBlock *> loopExits;
  SetVector<BasicBlock *> toDelete;
  for (auto peInfoVal : *report.getArray("pe_array")) {
    auto peInfo = *peInfoVal.getAsObject();

    if (peInfo["pe_type"] == "loop") {
      auto headerBB = getChildWithIndex<Function, BasicBlock>(
          &F, *peInfo.getInteger("basic_block_idx"));
      auto L = LI.getLoopFor(headerBB);
      auto exitBB = L->getExitBlock();

      for (auto BB : L->getBlocks()) {
        if (BB != headerBB && BB != exitBB)
          toDelete.insert(BB);
      }
      loopHeaders.push_back(headerBB);
      loopExits.push_back(exitBB);
    }
  }

  // Next, connect directly the loop header to exit (body becomes uncreachable).
  for (size_t i = 0; i < loopHeaders.size(); ++i) {
    IRBuilder<> Builder(loopHeaders[i]);
    auto oldBranch = loopHeaders[i]->getTerminator();
    Builder.CreateBr(loopExits[i]);
    deleteInstruction(oldBranch);
  }

  // Finally, delete the loop bodies.
  for (auto BB : toDelete) {
    deleteNonPipeInstrsInBB(BB);
    BB->removeFromParent();
  }
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
    auto &SE = AM.getResult<ScalarEvolutionAnalysis>(F);

    // Instruction-2-pipe transformation directives for this function. 
    json::Array directives = getDirectives(F, report);
    // A mapping between recurrence start instructions and pipes to hoist out.
    // This is used to remove unnecessary blockPE <--> MainKernel comms.
    json::Array hoistDirectives = getPipesToHoist(directives, LI);
    // {toKeep} will be updated with instructions created during transformation.
    SetVector<Instruction *> toDecouple, toKeep;
    getInstrToDecoupleAndToKeep(F, LI, report, toDecouple, toKeep);

    // Optionally speculatively hoist LSQ allocations out of their if-condition.
    if (isAGU) 
      hoistSpeculativeLSQAllocations(F, report);

    // Tags for ordering of LSQ reqeusts and values. 
    auto lsqStReqTag = createTag(F, toKeep);
    auto lsqLdReqTag = createTag(F, toKeep);
    // There can be multiple LSQ connections main, so also multiple stValTags.
    SmallVector<Value *> lsqStValTags; 
    for (auto _ : *report.getArray("lsq_array"))
      lsqStValTags.push_back(createTag(F, toKeep));

    // Process all instruction-to-pipe directives for this function.
    // The transformation matters and is ensured by ElasticAnalysisPrinter.
    for (json::Value &i2pInfoVal : directives) {
      auto i2pInfo = *i2pInfoVal.getAsObject();
      auto directiveType = i2pInfo.getString("directive_type");
      int lsqId = i2pInfo.get("lsq_id") ? *i2pInfo.getInteger("lsq_id") : 0;

      // LSQ related: 
      if (directiveType == "ld_val")
        instr2pipeLdVal(i2pInfo);
      else if (directiveType == "st_val")
        instr2pipeStVal(i2pInfo, lsqStValTags[lsqId], toKeep);
      else if (directiveType == "st_val_tag")
        instr2pipeStValTag(F, LI, SE, i2pInfo, lsqStValTags[lsqId], toKeep);
      else if (directiveType == "ld_req")
        instr2pipeLsqLdReq(i2pInfo, lsqStReqTag, lsqLdReqTag, toKeep);
      else if (directiveType == "st_req")
        instr2pipeLsqStReq(i2pInfo, lsqStReqTag, toKeep);
      else if (directiveType == "poison" || directiveType == "pred_poison")
        instr2pipePoisonAlloc(i2pInfo, lsqStValTags[lsqId], toKeep);
      else if (directiveType == "poison_in_bb_pe")
        instr2pipePoisonAllocInPE(i2pInfo, lsqStValTags[lsqId], toKeep);
      else if (directiveType == "end_lsq_signal")
        moveEndLsqSignalToReturnBB(i2pInfo);
      // Predicated PE related:
      else if (directiveType == "ssa_block" && isBlockPE) 
        instr2pipeSsaPe(i2pInfo, toKeep);
      else if (directiveType == "ssa_loop" && isLoopPE)
        instr2pipeSsaPe(i2pInfo, toKeep);
      else if (directiveType->contains("ssa") && (isMain || isLoopPE)) 
        instr2pipeSsaMain(i2pInfo, toKeep);
      else if (directiveType == "pred_block" && isBlockPE) 
        createPredicatedBlockPE(i2pInfo);
      else if (directiveType == "pred_loop" && isLoopPE) 
        createPredicatedLoopPE(i2pInfo, LI);
      else if (directiveType->contains("pred") && (isMain || isLoopPE)) 
        addPredicateWrites(i2pInfo, LI, toKeep);
    }

    // The hoisting transformation can move some pipes out of the decoupled BB
    // to the loop preheader or exit blocks to remove unnecessary read/writes.
    if (isBlockPE)
      hoistPipesBlockPE(F, hoistDirectives);
    else if (isMain || isLoopPE)
      hoistPipesMain(hoistDirectives, LI, isLoopPE);

    // The AGU and PE kernels have most instructions deleted. We don't
    // explicitly delete all instructions, but rather delete side-effect
    // instructions that are not needed, and letting the rest be deleted by DCE.
    if (isAGU || isBlockPE || isLoopPE) 
      deleteAllStores(F, toKeep);

    // In "main", we explicitly delete instructions that have been decoupled.
    if (isMain || isLoopPE) {
      for (auto &I : toDecouple) {
        if (!toKeep.contains(I) && !getPipeCall(I) && I->getParent()) {
          deleteInstruction(I);
        }
      }

      // If a whole loop has been decoupled, then we also need to delete blocks.
      if (isMain)
        deleteDecoupledLoopBodies(F, LI, report);
    }

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
// be able to recognize the pass via '-passes=stq-insert'
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getElasticTransformPluginInfo();
}

} // end namespace llvm
