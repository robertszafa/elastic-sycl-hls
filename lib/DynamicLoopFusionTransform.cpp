#include "CommonLLVM.h"
#include "DynamicLoopFusionAnalysis.h"

using namespace llvm;

namespace llvm {

const std::string REPORT_ENV_NAME = "ELASTIC_PASS_REPORT";

using MemoryRequest = DynamicLoopFusionAnalysis::MemoryRequest;
using DecoupledLoopType = DynamicLoopFusionAnalysis::DecoupledLoopType;
using DecoupledLoopInfo = DynamicLoopFusionAnalysis::DecoupledLoopInfo;
using MemoryDependencyInfo = DynamicLoopFusionAnalysis::MemoryDependencyInfo;

DecoupledLoopType getLoopType(Function &F) {
  std::string fName = demangle(std::string(F.getNameOrAsOperand()));
  return (fName.find("_agu") < fName.size())    ? DecoupledLoopType::agu
         : (fName.find("_mem_") < fName.size()) ? DecoupledLoopType::memory
                                                : DecoupledLoopType::compute;
}

/// Get loops to be decoupled in this function.
SmallVector<DecoupledLoopInfo, 4>
getLoopsToDecouple(Function &F, DynamicLoopFusionAnalysis &DLFA) {
  auto loopsToDecouple = DLFA.getDecoupledLoopsWithType(getLoopType(F));
  auto thisFName = demangle(std::string(F.getNameOrAsOperand()));

  SmallVector<DecoupledLoopInfo, 4> LoopsForThisFunction;
  for (auto &decoupleInfo : loopsToDecouple) {
    if (thisFName == decoupleInfo.kernelName) 
      LoopsForThisFunction.push_back(decoupleInfo);
  }

  return LoopsForThisFunction;
}

/// Get the memory requests in this function.
SmallVector<MemoryRequest, 4>
getMemoryRequests(Function &F,
                  SmallVector<DecoupledLoopInfo, 4> &loopsToDecouple) {
  auto loopType = getLoopType(F);
  SmallVector<MemoryRequest, 4> MemoryRequests;
  for (auto &decoupleInfo : loopsToDecouple) {
    for (auto &Req :
         llvm::concat<MemoryRequest>(decoupleInfo.loads, decoupleInfo.stores)) {
      Req.collectPipeCalls(F, loopType);
      Req.collectAGURequestStores(F, loopType);
      Req.collectStoreValPipeStores(F, loopType);
      MemoryRequests.push_back(Req);
    }
  }

  return MemoryRequests;
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

Value *getLoopHeaderExecutedNum(Loop *L) {
  IRBuilder<> Builder(L->getHeader()->getFirstNonPHI());

  auto iterPhi = Builder.CreatePHI(Builder.getInt32Ty(), 2, "num_header_exec");
  auto iterPlusOne =
      dyn_cast<Instruction>(Builder.CreateAdd(iterPhi, Builder.getInt32(1)));
  iterPlusOne->moveBefore(L->getLoopLatch()->getFirstNonPHI());

  // We start at one because on the first evaluation (i.e. coming from
  // preheader), the loop header will have been executed once
  iterPhi->addIncoming(Builder.getInt32(1), L->getLoopPreheader());
  iterPhi->addIncoming(iterPlusOne, L->getLoopLatch());

  return iterPhi;
}

void addisLastIterInstructions(Function &F, MemoryRequest &Req,
                              ScalarEvolution &SE) {
  for (int iD = 0; iD <= Req.loopDepth; ++iD) {
    if (!Req.isLastIterNeeded[iD])
      continue;

    auto backedgeTakenScev = SE.getBackedgeTakenCount(Req.loopNest[iD]);
    if (backedgeTakenScev->getSCEVType() == SCEVTypes::scCouldNotCompute) {
      // outs() << "Backedge taken count non-analyzable for loop with header "
      errs() << "Backedge taken count non-analyzable for loop with header "
             << Req.loopNest[iD]->getHeader()->getNameOrAsOperand()
             << "\nIsLastIter hint is ommited for this loop\n";
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
    auto isLastIterType = Req.isLastIterReqStore[iD]->getOperand(0)->getType();
    auto cmpResCasted = Builder.CreateCast(Instruction::CastOps::ZExt,
                                           cmpRes, isLastIterType);

    auto newisLastIterSt = Req.isLastIterReqStore[iD]->clone();
    newisLastIterSt->insertAfter(dyn_cast<Instruction>(cmpResCasted));
    newisLastIterSt->setOperand(0, cmpResCasted);
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
      if (isaStore(&I) && !ToKeep.contains(&I)) {
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
    auto *DLFA = new DynamicLoopFusionAnalysis(F, LI, SE, originalKernelName);

    auto DecoupledLoops = getLoopsToDecouple(F, *DLFA);
    auto MemoryRequests = getMemoryRequests(F, DecoupledLoops);

    auto LoopsToDelete = getLoopsToDelete(LI, DecoupledLoops);
    auto InstrToDelete = getSideEffectsToDelete(F, DecoupledLoops); 

    const auto loopType = getLoopType(F);
    for (auto &Req : MemoryRequests) {
      if (loopType == DecoupledLoopType::agu) {
        addAddrInstructions(F, Req);
        addScheduleInstructions(F, Req);
        addisLastIterInstructions(F, Req, SE);
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

    deleteCode(LoopsToDelete, InstrToDelete);

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
