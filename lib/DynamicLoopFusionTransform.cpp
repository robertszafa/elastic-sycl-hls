#include "CommonLLVM.h"
#include "DynamicLoopFusionAnalysis.h"
#include "CDG.h"

using namespace llvm;

namespace llvm {

const std::string REPORT_ENV_NAME = "ELASTIC_PASS_REPORT";
const std::string POISON_BB_NAME = "poisonBB";

using MemoryRequest = DynamicLoopFusionAnalysis::MemoryRequest;
using DecoupledLoopType = DynamicLoopFusionAnalysis::DecoupledLoopType;
using DecoupledLoopInfo = DynamicLoopFusionAnalysis::DecoupledLoopInfo;

DecoupledLoopType getLoopType(Function &F) {
  std::string fName = demangle(std::string(F.getNameOrAsOperand()));
  return (fName.find("_agu") < fName.size())    ? DecoupledLoopType::agu
         : (fName.find("_mem_") < fName.size()) ? DecoupledLoopType::memory
                                                : DecoupledLoopType::compute;
}

void collectPipeCalls(Function &F, SmallVector<MemoryRequest, 4> &Requests) {
  auto loopType = getLoopType(F);
  for (auto &Req : Requests) {
    Req.collectPipeCalls(F, loopType);
    Req.collectAGURequestStores(F, loopType);
    Req.collectStoreValPipeStores(F, loopType);
  }
}

void addSentinelPipeWrite(Function &F, MemoryRequest &Req) {
  const uint ADDR_SENTINEL = isaLoad(Req.memOp) ?  (1<<29) - 2 : (1<<29) - 1;
  const uint SCHED_SENTINEL = (1<<30);

  auto returnI = getReturnBlock(F)->getTerminator();

  auto sentinelAddrStore = Req.addrReqStore->clone();
  sentinelAddrStore->insertBefore(returnI);
  sentinelAddrStore->setOperand(
      0, ConstantInt::get(Req.addrReqStore->getOperand(0)->getType(),
                          ADDR_SENTINEL));

  for (auto &st : Req.schedReqStore) {
    auto newSt = st->clone();
    newSt->insertBefore(returnI);
    newSt->setOperand(
        0, ConstantInt::get(st->getOperand(0)->getType(), SCHED_SENTINEL));
  }

  // Req.isPosDepDistReqStore will be empty for stores.
  for (auto &st : Req.isPosDepDistReqStore) {
    auto newSt = st->clone();
    newSt->insertBefore(returnI);
    newSt->setOperand(0, ConstantInt::get(st->getOperand(0)->getType(), 1));
  }

  for (auto &st : Req.isLastIterReqStore) {
    auto newSt = st->clone();
    newSt->insertBefore(returnI);
    newSt->setOperand(0, ConstantInt::get(st->getOperand(0)->getType(), 1));
  }

  for (auto &pipeCall : Req.pipeCalls)
    pipeCall->clone()->insertBefore(returnI);
}

void addScheduleInstructions(Function &F, MemoryRequest &Req) {
  for (int iD = 0; iD <= Req.loopDepth; ++iD) {
    auto schedStInEntryBB = Req.schedReqStore[iD];
    auto schedStAddr = getLoadStorePointerOperand(schedStInEntryBB);
    auto schedStType = schedStInEntryBB->getOperand(0)->getType();

    // Load sched value in appropriate loop and increment it.
    IRBuilder<> Builder(getFirstBodyBlock(Req.loopNest[iD])->getFirstNonPHI());
    auto constantOne = ConstantInt::get(schedStType, 1);
    Value *oldSchedVal = Builder.CreateLoad(schedStType, schedStAddr);
    Value *newSchedVal = Builder.CreateAdd(oldSchedVal, constantOne);

    // Store incremented value.
    auto newSchedSt = schedStInEntryBB->clone();
    newSchedSt->insertAfter(dyn_cast<Instruction>(newSchedVal));
    newSchedSt->setOperand(0, newSchedVal);
  }
}

PHINode *getInductionVariable(Loop *L) {
  auto LoopBranch = L->getHeader()->getTerminator();
  auto LoopPredicateVal = LoopBranch->getOperand(0);
  if (auto PredicateI = dyn_cast<Instruction>(LoopPredicateVal)) {
    for (auto &Op : PredicateI->operands()) {
      if (auto PhiI = dyn_cast<PHINode>(Op)) {
        if (PhiI->getParent() == L->getHeader()) {
          return PhiI;
        }
      }
    }
  }

  return nullptr;
}

/// Return the instructions producing the next loop induction value.
SmallVector<Instruction *> getInductionChain(PHINode *InductionPhi, Loop *L) {
  SmallVector<Instruction *> Result;
  BasicBlock *LatchBB = L->getLoopLatch();
  auto InductionLatchVal = InductionPhi->getIncomingValueForBlock(LatchBB);
  auto InductionLatchInstr = dyn_cast<Instruction>(InductionLatchVal);
  if (!InductionLatchInstr)
    return Result;

  for (auto &BB : getUniqueLoopBlocks(L)) {
    for (auto &I : *BB) {
      if (isInDefUsePath(&I, InductionLatchInstr) && InductionPhi != &I) {
        Result.push_back(&I);
      }
    }
  }

  return Result;
}

/// Copy the instruction chain producing the next loop induction value.
/// Return the instruction producing the final induction value.
Instruction *copyInductionChain(SmallVector<Instruction *> &InductionChain,
                                Instruction *InsertBefore) {
  SmallVector<Instruction *> NewInductionChain(InductionChain.size());
  auto NextInduction = InductionChain[0];
  for (auto [currInductionI, InductionI] : llvm::enumerate(InductionChain)) {
    NextInduction = InductionI->clone();
    NextInduction->insertBefore(InsertBefore);
    
    NewInductionChain[currInductionI] = NextInduction;
    for (size_t iPrev = 0; iPrev < currInductionI; ++iPrev) {
      NextInduction->replaceUsesOfWith(InductionChain[iPrev],
                                       NewInductionChain[iPrev]);
    }
  }

  return NextInduction;
}

/// Create a bit value that will be set to true on the last loop iteration.  
/// If the check is not possible to synthesize, return nullptr.
Value *createIsLastIterCheck(Function *F, Loop *L, Type *ResultType) {
  static MapVector<Function *, MapVector<Loop*, Value*>> Cache;
  if (Cache.contains(F) && Cache[F].contains(L))
    return Cache[F][L];

  // Execute the induction function one iteration in advance and check if loop
  // branch leading to loop body becomes false.
  Value *Result = nullptr;
  if (auto InductionPhi = getInductionVariable(L)) {
    auto LoopPredicate =
        dyn_cast<Instruction>(L->getHeader()->getTerminator()->getOperand(0));
    auto InductionChain = getInductionChain(InductionPhi, L);

    if (LoopPredicate && !InductionChain.empty()) {
      auto InsertBeforI = L->getHeader()->getTerminator();

      // Execute induction chain again at start of loop body.
      auto NextInduction = copyInductionChain(InductionChain, InsertBeforI);

      // Clone loop branch predicate instruction to start of loop body, and 
      // change the old induction to our NextInduction.
      auto NextLoopPredicate = LoopPredicate->clone();
      NextLoopPredicate->insertBefore(InsertBeforI);
      NextLoopPredicate->replaceUsesOfWith(InductionPhi, NextInduction);

      // Now synthesize: isLastIter = !nextLoopPredicate
      IRBuilder<> Builder(InsertBeforI); 
      auto IsLastIter = Builder.CreateNot(NextLoopPredicate, "IsLastIter");
      // Ensure correct type.
      Result = Builder.CreateCast(Instruction::CastOps::ZExt, IsLastIter,
                                  ResultType, "IsLastIter");
    }
  }

  // Will be nullptr if check not possible.
  Cache[F][L] = Result;
  return Result;
}

/// For each non-monotonic outer loop of a given memory request, generate a
/// last-iteration predicate bit and store it into the AGU request.
void addIsLastIterInstructions(Function &F, MemoryRequest &Req) {
  for (int iD = 0; iD <= Req.loopDepth; ++iD) {
    if (!Req.isLastIterNeeded[iD])
      continue;
    
    auto L = Req.loopNest[iD];
    auto IsLastIterType = Req.isLastIterReqStore[iD]->getOperand(0)->getType();

    if (auto IsLastIterCheck = createIsLastIterCheck(&F, L, IsLastIterType)) {
      auto IsLastIterStore = Req.isLastIterReqStore[iD]->clone();
      IsLastIterStore->insertAfter(dyn_cast<Instruction>(IsLastIterCheck));
      IsLastIterStore->setOperand(0, IsLastIterCheck);
    } else {
      errs() << "INFO: isLastIterCheck cannot be synthesized for loop depth "
             << iD << " (header block " << L->getHeader()->getNameOrAsOperand()
             << ")\nIsLastIter hint is ommited for this loop\n";
    }
  }
}

void addIsPosDepDistInstructions(Function &F, MemoryRequest &LdReq,
                                 SmallVector<MemoryRequest, 4> &OtherRequests) {
  for (auto &StReq : OtherRequests) {
    if (isaStore(StReq.memOp) && (StReq.memoryId == LdReq.memoryId) &&
        (StReq.loopNest.back() == LdReq.loopNest.back())) {
      IRBuilder<> Builder(LdReq.memOp);

      auto AddrType = LdReq.addrReqStore->getOperand(0)->getType();
      auto LdAddr = Builder.CreateLoad(
          AddrType, getLoadStorePointerOperand(LdReq.addrReqStore), "ldAddr");
      auto StAddr = Builder.CreateLoad(
          AddrType, getLoadStorePointerOperand(StReq.addrReqStore), "stAddr");

      auto LdAddrIsGreater = Builder.CreateCmp(CmpInst::Predicate::ICMP_UGT,
                                               LdAddr, StAddr, "isPosDepDist");

      auto IsPosDepDistSt = LdReq.isPosDepDistReqStore[StReq.reqId]->clone();
      auto IsPosDepDistStType =
          LdReq.isPosDepDistReqStore[StReq.reqId]->getOperand(0)->getType();
      auto LdAddrIsGreaterCasted = Builder.CreateCast(
          Instruction::CastOps::ZExt, LdAddrIsGreater, IsPosDepDistStType);
      IsPosDepDistSt->setOperand(0, LdAddrIsGreaterCasted);
      IsPosDepDistSt->insertBefore(LdReq.memOp);
    }
  }
}

void addAddrInstructions(Function &F, MemoryRequest &Req) {
  auto ReqAddrSt = Req.addrReqStore->clone();
  ReqAddrSt->insertBefore(Req.memOp);

  Value *AddrVal = getLoadStorePointerOperand(Req.memOp);
  auto AddrType = ReqAddrSt->getOperand(0)->getType();
  auto AddrValCasted =
      BitCastInst::CreateBitOrPointerCast(AddrVal, AddrType, "", ReqAddrSt);
  ReqAddrSt->setOperand(0, AddrValCasted);
}

void swapMemOpForReqPipe(MemoryRequest &Req) {
  for (auto &pipe : Req.pipeCalls)
    pipe->moveBefore(Req.memOp);
  deleteInstruction(Req.memOp);
}

void swapMemOpForValPipe(MemoryRequest &Req) {
  Req.pipeCalls[0]->moveAfter(Req.memOp);

  if (isaStore(Req.memOp) && Req.storeValueStore) { 
    // pipe::write({value, valid}) store
    Req.storeValidStore->moveBefore(Req.pipeCalls[0]);
    // Store into the pipes value field.
    Req.memOp->setOperand(1, Req.storeValueStore->getOperand(1));
    // Set valid bit.
    auto validBitType = Req.storeValidStore->getOperand(0)->getType();
    Req.storeValidStore->setOperand(0, ConstantInt::get(validBitType, 1));
  } 
  else if (isaStore(Req.memOp)) {
    // pipe::write(value) store
    Req.memOp->setOperand(1, Req.pipeCalls[0]->getOperand(0));
  }
  else {
    // value = pipe::read()
    Req.memOp->replaceAllUsesWith(Req.pipeCalls[0]);
  }
}

void swapMemOpForPipeReadWrite(MemoryRequest &Req) {
  if (isaLoad(Req.memOp)) {
    auto storeIntoPipe = getAllStoresInBlockUpTo(Req.pipeCalls[0])[0];
    Req.pipeCalls[0]->moveAfter(Req.memOp);
    storeIntoPipe->moveBefore(Req.pipeCalls[0]);
    storeIntoPipe->setOperand(0, Req.memOp);
  } else {
    Req.pipeCalls[0]->moveBefore(Req.memOp);
    Req.memOp->setOperand(0, Req.pipeCalls[0]);
  }
}

/// Return the if-condition src of BB, if it exisits.
BasicBlock *getIfSrcBlockInLoop(ControlDependenceGraph &CDG, LoopInfo &LI,
                                BasicBlock *BB) {
  auto L = LI.getLoopFor(BB);
  if (L && L->getExitBlock() != BB) {
    if (auto CtrlDepBB = CDG.getControlDependencySource(BB)) {
      // Checking if the ctrl dep src is not the loop condition.
      // A loop condition can be spread over multiple blocks.
      if (!LI.isLoopHeader(CtrlDepBB) &&
          !isReachableWithinLoop(CtrlDepBB, L->getExitBlock(), L)) {
        return CtrlDepBB;
      }
    }
  }

  return nullptr;
}

/// Return memory requests that have been speculated in the AGU.
SmallVector<MemoryRequest, 4>
getSpeculatedRequests(ControlDependenceGraph &CDG, LoopInfo &LI,
                      SmallVector<MemoryRequest, 4> &Requests) {
  SmallVector<MemoryRequest, 4> RequestsToSpeculate;
  for (auto &Req : Requests) {
    if (getIfSrcBlockInLoop(CDG, LI, Req.memOp->getParent()))
      RequestsToSpeculate.push_back(Req);
  }
  return  RequestsToSpeculate;
}

/// Move Req (and any isntructions in its address def-use chain) to {ToBB}.
void hoistRequest(MemoryRequest &Req, BasicBlock *ToBB) {
  auto ReqBB = Req.memOp->getParent();
  if (ReqBB == ToBB)
    return;

  SmallVector<Instruction *> toMove;
  for (auto &I : *ReqBB) {
    if (isInDefUsePath(&I, Req.memOp) || &I == Req.memOp) {
      toMove.push_back(&I);
    } 
  }

  for (auto I : toMove)
    I->moveBefore(ToBB->getTerminator());
}

/// Map each if-condition body BB to its if condition src block. 
MapVector<BasicBlock *, BasicBlock *>
getIfBodyToConditionMap(Function &F, ControlDependenceGraph &CDG,
                        LoopInfo &LI) {
  MapVector<BasicBlock *, BasicBlock *> Res;
  for (auto &BB : F) {
    if (!LI.isLoopHeader(&BB)) {
      if (auto IfSrcBB = getIfSrcBlockInLoop(CDG, LI, &BB)) {
        Res[&BB] = IfSrcBB;
      }
    }
  }

  return Res;
}

/// Given a IfBodyToConditionSrcBlockMap, map each key to its top level
/// if-condition src block.
void walkToTopLevelIfSrcBB(
    MapVector<BasicBlock *, BasicBlock *> &IfBodyToConditionMap) {
  bool done = false;
  while (!done) {
    done = true;
    for (auto [IfBodyBB, IfSrcBB] : IfBodyToConditionMap) {
      if (IfBodyToConditionMap.contains(IfSrcBB)) {
        IfBodyToConditionMap[IfBodyBB] = IfBodyToConditionMap[IfSrcBB];
        done = false;
      }
    }
  }
}

/// Hoist memory requests out of if-conditions.
void speculateRequests(Function &F, ControlDependenceGraph &CDG, LoopInfo &LI,
                       const SmallVector<MemoryRequest, 4> &Requests) {
  // auto SortedRequests = Requests;
  // topologicalOrderSort(F, SortedRequests);

  auto HoistFromToMap = getIfBodyToConditionMap(F, CDG, LI);
  
  // Iteratively hoist requests out of chains of if-conditions.
  bool Done = false;
  while (!Done) {
    Done = true;
    for (auto Req : Requests) {
      auto ReqBB = Req.memOp->getParent();
      if (HoistFromToMap.contains(ReqBB)) {
        hoistRequest(Req, HoistFromToMap[ReqBB]);
        Done = false;
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

/// Given the FromBB->ToBB mapping of hoisting performed in the AGU, return
/// a mapping of poison blocks to a list of speculated memory requests that
/// should be invalidated in those blocks. Note that the blocks may involve
/// newly created blocks (see createBlockOnEdge() function).
///
/// The algorithm traverses every loop CFG path to guarante that the order of
/// store values (poisoned or not) matches the order of store requests made in
/// the AGU.
MapVector<BasicBlock *, SmallVector<MemoryRequest, 4>>
getPoisonBlocks(Function &F, LoopInfo &LI,
                MapVector<BasicBlock *, BasicBlock *> &HoistedFromToMap,
                SmallVector<MemoryRequest, 4> &SpeculatedRequests) {
  MapVector<CFGEdge, SmallVector<MemoryRequest, 4>> PoisonLocations;

  for (auto &Req : SpeculatedRequests) {
    // Loads are hoisted to the same location where they have been speculated. 
    if (isaLoad(Req.memOp))
      continue;

    auto L = LI.getLoopFor(Req.memOp->getParent());
    auto SpecBB = HoistedFromToMap[Req.memOp->getParent()];
    auto AllBlockPaths = getAllBlockPathsInLoop(SpecBB, LI);
    auto AllEdgePaths = blockToEdgePath(AllBlockPaths);

    for (size_t iPath = 0; iPath < AllEdgePaths.size(); ++iPath) {
      auto EdgePath = AllEdgePaths[iPath];
      auto BlockPath = AllBlockPaths[iPath];

      // Map of {trueBlock: requests originally in trueBlock}
      MapVector<BasicBlock *, SmallVector<MemoryRequest, 4>> trueBlocks;
      for (auto &r : SpeculatedRequests) {
        auto trueBB = r.memOp->getParent();
        if (!trueBlocks.contains(trueBB))
          trueBlocks[trueBB] = SmallVector<MemoryRequest, 4>();

        trueBlocks[trueBB].push_back(r);
      }

      for (auto &[EdgeSrc, EdgeDst] : EdgePath) {
        CFGEdge Edge = {EdgeSrc, EdgeDst};
        for (auto &[trueBB, requests] : trueBlocks) {
          if (requests.empty())
            continue;

          if (EdgeDst == trueBB) {
            trueBlocks[trueBB].clear();
            break; // to next edge
          }

          if (!isReachableWithinLoop(EdgeDst, trueBB, L)) {
            if (!PoisonLocations.contains(Edge))
              PoisonLocations[Edge] = SmallVector<MemoryRequest, 4>();

            llvm::append_range(PoisonLocations[Edge], requests);
            trueBlocks[trueBB].clear();
          }
        }
      }
    }
  }

  // Map edges to blocks, and remove repetitions.
  MapVector<BasicBlock *, SmallVector<MemoryRequest, 4>> PoisonBlocks;
  for (auto [Edge, Requests] : PoisonLocations) {
    auto PoisonBB = createBlockOnEdge(Edge.first, Edge.second);
    PoisonBlocks[PoisonBB] = SmallVector<MemoryRequest, 4>();
    
    SetVector<Instruction*> Done;
    for (auto Req : Requests) {
      if (!Done.contains(Req.memOp)) {
        PoisonBlocks[PoisonBB].push_back(Req);
        Done.insert(Req.memOp);
      }
    }
  }

  return PoisonBlocks;
}

/// Place a store value pipe at the end of {PoisonBB} with a 0 valid bit.
void insertInvalidStoreValPipe(MemoryRequest &Req, BasicBlock *PoisonBB) {
  auto PoisonCall = Req.pipeCalls[0]->clone();
  auto ValidStore = Req.storeValidStore->clone();
  PoisonCall->insertBefore(PoisonBB->getTerminator());
  ValidStore->insertBefore(PoisonCall);
  auto validBitType = ValidStore->getOperand(0)->getType();
  ValidStore->setOperand(0, ConstantInt::get(validBitType, 0));
}

/// Check which requests were speculated in the AGU and insert poison store
/// value calls on CFG paths where the speculation turns out false.
void poisonMisspeculations(Function &F, ControlDependenceGraph &CDG,
                           LoopInfo &LI,
                           SmallVector<MemoryRequest, 4> &Requests) {
  // Get speculated requests in the topological program order.
  SmallVector<MemoryRequest, 4> SpeculatedRequests =
      getSpeculatedRequests(CDG, LI, Requests);
  // topologicalOrderSort(F, SpeculatedRequests);

  // Get a mapping of where the speculated requests were hoisted to in the AGU.
  auto HoistedFromToMap = getIfBodyToConditionMap(F, CDG, LI);
  walkToTopLevelIfSrcBB(HoistedFromToMap);

  // Given the above info, get poison blocks where to insert poison calls.
  MapVector<BasicBlock *, SmallVector<MemoryRequest, 4>> PoisonLocations =
      getPoisonBlocks(F, LI, HoistedFromToMap, SpeculatedRequests);

  for (auto [PoisonBB, RequestsToPoison] : PoisonLocations) {
    for (auto Req : RequestsToPoison) {
      auto ReqBB = Req.memOp->getParent();
      auto SpecBB = HoistedFromToMap[ReqBB];
      
      if (isaStore(Req.memOp)) {
        insertInvalidStoreValPipe(Req, PoisonBB);
      } else {
        Req.pipeCalls[0]->moveBefore(SpecBB->getTerminator());
      }
    }
  }
}

/// Merge poison blocks that have the same list of poison pipe calls, and the
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

  SetVector<std::pair<BasicBlock *, BasicBlock *>> done;
  // Merge poison blocks that have equivalent pipe calls and the same successor.
  for (auto &[poisonBB1, pipeCalls1] : poisonBB2PipeCalls) {
    for (auto &[poisonBB2, pipeCalls2] : poisonBB2PipeCalls) {
      if (poisonBB1 != poisonBB2 && areEquivalent(pipeCalls1, pipeCalls2)) {
        // Repeated merge(A, B) and merge(B, A) are not a problem.
        mergeBlocks(poisonBB1, poisonBB2);
        done.insert({poisonBB1, poisonBB2});
      }
    }
  }
}

SmallVector<Instruction *>
getSideEffectsToDelete(Function &F,
                       SmallVector<DecoupledLoopInfo, 4> &DecoupledLoops) {
  if (getLoopType(F) == DecoupledLoopType::compute) {
    // Compute loops will contain all side effects belonging to their loop
    // nests. Side effects from other loops nests will be deleted by deleting
    // that whole loop nest.
    return {};
  }

  SetVector<Instruction *> ToKeep;
  for (auto &Info : DecoupledLoops) {
    for (auto StReq : Info.stores)
      ToKeep.insert(StReq.memOp);
  }

  SmallVector<Instruction *> ToDelete;
  for (auto &BB : F) {
    if (BB.isEntryBlock())
      continue;

    for (auto &I : BB) {
      bool isToKeep = ToKeep.contains(&I) || getPipeCall(&I);
      bool isSideEffect = isaStore(&I) || isa<CallInst>(&I);
      if (isSideEffect && !isToKeep) {
        ToDelete.push_back(&I);
      }
    }
  }

  return ToDelete;
}

SmallVector<Loop *>
getLoopsToDelete(LoopInfo &LI,
                 SmallVector<DecoupledLoopInfo, 4> &DecoupledLoops) {
  SetVector<Loop *> ToKeep;
  for (auto &Info : DecoupledLoops) {
    for (auto &L : Info.loops)
      ToKeep.insert(L);
  }

  SmallVector<Loop *> ToDelete;
  for (auto &L : LI.getLoopsInPreorder()) {
    if (!ToKeep.contains(L))
      ToDelete.push_back(L);
  }

  return ToDelete;
}

void deleteCode(SmallVector<Loop *> &toDeleteL,
                SmallVector<Instruction *> &toDeleteI) {
  for (auto &L : toDeleteL)
    deleteLoop(L);
  for (auto &I : toDeleteI)
    deleteInstruction(I);
}

struct DynamicLoopFusionTransform : PassInfoMixin<DynamicLoopFusionTransform> {
  json::Object report;
  std::string originalKernelName = "";

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    // Read in report once.
    if (report.empty()) {
      report = *parseJsonReport(REPORT_ENV_NAME).getAsObject();
      originalKernelName = *report["mainKernelName"].getAsString();
    }

    std::string fName = demangle(std::string(F.getNameOrAsOperand()));
    if ((F.getCallingConv() != CallingConv::SPIR_KERNEL) ||
        (fName.find(originalKernelName) >= fName.size())) {
      return PreservedAnalyses::all();
    }

    auto &LI = AM.getResult<LoopAnalysis>(F);
    auto &SE = AM.getResult<ScalarEvolutionAnalysis>(F);
    auto &PDT = AM.getResult<PostDominatorTreeAnalysis>(F);
    std::shared_ptr<ControlDependenceGraph> CDG(
        new ControlDependenceGraph(F, PDT));
    auto *DLFA = new DynamicLoopFusionAnalysis(F, LI, SE, originalKernelName);

    auto DecoupledLoops = DLFA->getDecoupledLoops(fName);
    auto MemoryRequests = DLFA->getKernelRequestsInTopologicalOrder(fName);
    collectPipeCalls(F, MemoryRequests);

    auto LoopsToDelete = getLoopsToDelete(LI, DecoupledLoops);
    auto InstrToDelete = getSideEffectsToDelete(F, DecoupledLoops); 

    const auto loopType = getLoopType(F);
    
    // In AGUs, requests are hoisted out of if-conditions.
    if (loopType == DecoupledLoopType::agu)
      speculateRequests(F, *CDG, LI, MemoryRequests);

    for (auto &Req : MemoryRequests) {
      if (loopType == DecoupledLoopType::agu) {
        addAddrInstructions(F, Req);
        addScheduleInstructions(F, Req);
        addIsLastIterInstructions(F, Req);
        if (isaLoad(Req.memOp))
          addIsPosDepDistInstructions(F, Req, MemoryRequests);
        addSentinelPipeWrite(F, Req);
        swapMemOpForReqPipe(Req);
      } else if (loopType == DecoupledLoopType::memory) {
        swapMemOpForPipeReadWrite(Req);
      } else {
        swapMemOpForValPipe(Req);
      }
    }

    // In CUs, invalidate misspeculated requests.
    if (loopType == DecoupledLoopType::compute) {
      poisonMisspeculations(F, *CDG, LI, MemoryRequests);
      mergePoisonBlocks(F);
    }

    deleteCode(LoopsToDelete, InstrToDelete);

    return PreservedAnalyses::none();
  }


  static bool isRequired() { return true; }

  void getAnalysisUsage(AnalysisUsage &AU) const {
    AU.addRequiredID(LoopAnalysis::ID());
    AU.addRequiredID(ScalarEvolutionAnalysis::ID());
    AU.addRequiredID(PostDominatorTreeAnalysis::ID());
  }
};

//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getDynamicLoopFusionTransformPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "DynamicLoopFusionTransform",
          LLVM_VERSION_STRING, [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "dynamic-loop-fusion-transform") {
                    FPM.addPass(DynamicLoopFusionTransform());
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
  return getDynamicLoopFusionTransformPluginInfo();
}

} // end namespace llvm
