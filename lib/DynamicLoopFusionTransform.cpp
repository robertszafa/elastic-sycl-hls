#include "CommonLLVM.h"
#include "DynamicLoopFusionAnalysis.h"

using namespace llvm;

namespace llvm {

const std::string REPORT_ENV_NAME = "ELASTIC_PASS_REPORT";

using MemoryRequest = DynamicLoopFusionAnalysis::MemoryRequest;
using DecoupledLoopInfo = DynamicLoopFusionAnalysis::DecoupledLoopInfo;
using MemoryDependencyInfo = DynamicLoopFusionAnalysis::MemoryDependencyInfo;
using DepDir = DynamicLoopFusionAnalysis::DepDir;

/// Get loops to be decoupled in this function.
SmallVector<DecoupledLoopInfo, 4>
getLoopsToDecouple(DynamicLoopFusionAnalysis &DLFA,
                   const std::string &thisKernelName,
                   const std::string &originalKernelName, const bool isAGU) {
  auto loopsToDecouple =
      isAGU ? DLFA.getAgusToDecouple() : DLFA.getLoopsToDecouple();
  
  SmallVector<DecoupledLoopInfo, 4> LoopsToDecouple;
  auto MemDepInfos = DLFA.getMemoryDependencyInfo();
  for (auto &decoupleInfo : loopsToDecouple) {
    std::string decoupleInfoKernelName =
        isAGU ? originalKernelName + "_agu" : originalKernelName;
    if (decoupleInfo.id > 0)
      decoupleInfoKernelName += "_" + std::to_string(decoupleInfo.id);
    
    if (decoupleInfoKernelName == thisKernelName) {
      LoopsToDecouple.push_back(decoupleInfo);
    }
  }

  return LoopsToDecouple;
}

void collectMemoryRequestStores(Function &F, MemoryRequest &Req) {
  // Get all stores between pipe call and basic block start.
  SmallVector<StoreInst *> reqStores;
  Instruction *currI = Req.pipeCalls[0];
  while (currI) {
    if (auto stI = dyn_cast<StoreInst>(currI)) 
      reqStores.push_back(stI);
    currI = currI->getPrevNonDebugInstruction(true);
  }

  // Now go backwards in the stores, starting at the pipe call, until everything
  // is filled.
  auto currSt = reqStores.begin();

  for (int iD = 0; iD < Req.maxLoopDepthInMemoryId; ++iD) {
    Req.isMaxIterReqStore.push_back(*currSt);
    currSt++;
    Req.schedReqStore.push_back(*currSt);
    currSt++;
  }

  if (isaLoad(Req.memOp)) {
    for (int iSt = 0; iSt < Req.numStoresInMemoryId; ++iSt) {
      Req.isPosDepDistReqStore.push_back(*currSt);
      currSt++;
    }
  }

  Req.addrReqStore = *currSt;
  currSt++;
}

/// Get the memory requests in this function that will be connected to our IP.
/// Also collect the corresponding pipe calls that the requets will be replcaed
/// with.
SmallVector<MemoryRequest, 4>
getMemoryRequests(Function &F,
                  SmallVector<DecoupledLoopInfo, 4> &loopsToDecouple,
                  const bool isAGU) {
  SmallVector<MemoryRequest, 4> MemoryRequests;
  for (auto &decoupleInfo : loopsToDecouple) {
    for (auto &LdReq : decoupleInfo.loads) {
      std::string pipeName =
          isAGU ? "LoadReqPipes_" + std::to_string(LdReq.memoryId)
                : "LoadValPipes_" + std::to_string(LdReq.memoryId);
      LdReq.pipeCalls.push_back(getPipeCall(F, pipeName, {LdReq.reqId}));
      if (isAGU)
        collectMemoryRequestStores(F, LdReq);
      MemoryRequests.push_back(LdReq);
    }

    for (auto &StReq : decoupleInfo.stores) {
      std::string pipeName =
          isAGU ? "StoreReqPipes_" + std::to_string(StReq.memoryId)
                : "StoreValPipes_" + std::to_string(StReq.memoryId);
      if (isAGU) {
        // In the agu, the stReq pipe has another dimension.
        StReq.pipeCalls.push_back(getPipeCall(F, pipeName, {StReq.reqId, 0}));
        StReq.pipeCalls.push_back(getPipeCall(F, pipeName, {StReq.reqId, 1}));
        collectMemoryRequestStores(F, StReq);
      } else {
        StReq.pipeCalls.push_back(getPipeCall(F, pipeName, {StReq.reqId}));
      }
      MemoryRequests.push_back(StReq);
    }
  }

  return MemoryRequests;
}

/// Create a zero-init uint32 tag using alloca at {F.entry}. Return its address.
Value *createTag(Function &F, SetVector<Instruction *> &toKeep) {
  IRBuilder<> Builder(F.getEntryBlock().getTerminator());
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

  for (auto &st : Req.isMaxIterReqStore) {
    auto newSt = st->clone();
    newSt->insertBefore(returnI);
    newSt->setOperand(0, ConstantInt::get(st->getOperand(0)->getType(), 1));
  }

  Req.pipeCalls[0]->clone()->insertBefore(returnI);
}

void addScheduleInstructions(Function &F, MemoryRequest &Req) {
  for (int iD = 0; iD <= Req.loopDepth; ++iD) {
    auto entrySchedSt = Req.schedReqStore[iD];
    auto entrySchedAddr = getLoadStorePointerOperand(entrySchedSt);
    auto entrySchedType = entrySchedSt->getOperand(0)->getType();

    IRBuilder<> Builder(getFirstBodyBlock(Req.loopNest[iD])->getFirstNonPHI());
    auto constantOne = ConstantInt::get(entrySchedType, 1);
    Value *oldSchedVal = Builder.CreateLoad(entrySchedType, entrySchedAddr);
    Value *newSchedVal = Builder.CreateAdd(oldSchedVal, constantOne);

    auto newSchedSt = entrySchedSt->clone();
    newSchedSt->insertAfter(dyn_cast<Instruction>(newSchedVal));
    newSchedSt->setOperand(0, newSchedVal);
  }
}

Value *getLoopHeaderExecutedNum(Loop *L) {
  static MapVector<Loop *, Value *> done;
  if (done.contains(L))
    return done[L];

  IRBuilder<> Builder(L->getHeader()->getFirstNonPHI());
  
  auto iterPhi = Builder.CreatePHI(Builder.getInt32Ty(), 2, "num_header_exec");
  auto iterPlusOne =
      dyn_cast<Instruction>(Builder.CreateAdd(iterPhi, Builder.getInt32(1)));
  iterPlusOne->moveBefore(L->getLoopLatch()->getFirstNonPHI());

  // We start at one because on the first evaluation (i.e. coming from
  // preheader), the loop header will have been executed once
  iterPhi->addIncoming(Builder.getInt32(1), L->getLoopPreheader());
  iterPhi->addIncoming(iterPlusOne, L->getLoopLatch());

  done[L] = iterPhi;

  return iterPhi;
}

void addIsMaxIterInstructions(Function &F, MemoryRequest &Req,
                              ScalarEvolution &SE) {
  for (int iD = 0; iD <= Req.loopDepth; ++iD) {
    if (!Req.isMaxIterNeeded[iD])
      continue;

    auto backedgeTakenScev = SE.getBackedgeTakenCount(Req.loopNest[iD]);
    if (backedgeTakenScev->getSCEVType() == SCEVTypes::scCouldNotCompute) {
      // outs() << "Backedge taken count non-analyzable for loop with header "
      errs() << "Backedge taken count non-analyzable for loop with header "
             << Req.loopNest[iD]->getHeader()->getNameOrAsOperand()
             << "\nIsMaxIter hint is ommited for this loop\n";
      continue;
    }

    auto insertBeforeI = getFirstBodyBlock(Req.loopNest[iD])->getFirstNonPHI();
    IRBuilder<> Builder(insertBeforeI);
    // Value evaluating to the number of times the backedge will be taken.
    auto numBackedgesInL = buildSCEVExpr(F, backedgeTakenScev, insertBeforeI);
    // Value evaluating to the number of times the backedge was taken so far.
    auto numTimesHeaderExec = getLoopHeaderExecutedNum(Req.loopNest[iD]);
    // Compare the two. If equal, then this is the last iteration of the loop.
    auto cmpRes = Builder.CreateCmp(CmpInst::Predicate::ICMP_EQ,
                                    numBackedgesInL, numTimesHeaderExec);
    auto isMaxIterType = Req.isMaxIterReqStore[iD]->getOperand(0)->getType();
    auto cmpResCasted = Builder.CreateCast(Instruction::CastOps::ZExt,
                                           cmpRes, isMaxIterType);

    auto newIsMaxIterSt = Req.isMaxIterReqStore[iD]->clone();
    newIsMaxIterSt->insertAfter(dyn_cast<Instruction>(cmpResCasted));
    newIsMaxIterSt->setOperand(0, cmpResCasted);
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

void swapReqForPipe(MemoryRequest &Req) {
  for (auto &pipe : Req.pipeCalls)
    pipe->moveBefore(Req.memOp);
  deleteInstruction(Req.memOp);
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

    std::string thisKernelName = demangle(std::string(F.getNameOrAsOperand()));
    bool isAGU = thisKernelName.find("_agu") < thisKernelName.size();
    if ((F.getCallingConv() != CallingConv::SPIR_KERNEL) ||
        (thisKernelName.find(originalKernelName) >= thisKernelName.size())) {
      return PreservedAnalyses::all();
    }

    auto &LI = AM.getResult<LoopAnalysis>(F);
    auto &SE = AM.getResult<ScalarEvolutionAnalysis>(F);
    auto *DLFA = new DynamicLoopFusionAnalysis(LI, SE);

    errs() << "\n***********\n" << thisKernelName << "\n***********\n";

    auto LoopsToDecouple = getLoopsToDecouple(*DLFA, thisKernelName, 
                                              originalKernelName, isAGU);
    auto MemoryRequests = getMemoryRequests(F, LoopsToDecouple, isAGU);

    for (auto &Req : MemoryRequests) {
      if (isAGU) {
        addAddrInstructions(F, Req);
        addScheduleInstructions(F, Req);
        addIsMaxIterInstructions(F, Req, SE);
        if (isaLoad(Req.memOp))
          addIsPosDepDistInstructions(F, Req, MemoryRequests);
        addSentinelPipeWrite(F, Req);
        swapReqForPipe(Req);
      } else {

      }
    }

    // if (isAGU) {
    //   errs() << "**********************\n";
    //   F.print(errs());
    //   errs() << "\n**********************\n";
    // }

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
