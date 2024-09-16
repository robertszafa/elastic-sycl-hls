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

/// Create a bit value that will be set to true on the last loop iteration.  
/// If the check is not possible to synthesize, return nullptr.
Value *createIsLastIterCheck(Loop *L, Type *ResultType) {
  // Create an advanced loop predicate value by executing the
  // inductionPhi->inductionLatch chain once in the loop preheader.
  auto LoopPreheader = L->getLoopPreheader();
  auto LoopBranch = L->getHeader()->getTerminator();
  auto LoopPredicateVal = LoopBranch->getOperand(0);

  if (auto InductionPhi = getInductionVariable(L)) {
    auto InductionLatchVal =
        InductionPhi->getIncomingValueForBlock(L->getLoopLatch());
    auto InductionInit =
        InductionPhi->getIncomingValueForBlock(LoopPreheader);

    if (auto InductionLatch = dyn_cast<Instruction>(InductionLatchVal)) {
      if (auto LoopPredicate = dyn_cast<Instruction>(LoopPredicateVal)) {
        auto AdvancedInductionInit = InductionLatch->clone();
        AdvancedInductionInit->insertBefore(LoopPreheader->getTerminator());
        AdvancedInductionInit->replaceUsesOfWith(InductionPhi, InductionInit);

        auto AdvancedInductionPhi = InductionPhi->clone();
        AdvancedInductionPhi->insertAfter(InductionPhi);
        AdvancedInductionPhi->replaceUsesOfWith(InductionInit, 
                                                AdvancedInductionInit);

        auto AdvancedInductionLatch = InductionLatch->clone();
        AdvancedInductionLatch->insertAfter(InductionLatch);
        AdvancedInductionInit->replaceUsesOfWith(InductionPhi, 
                                                 AdvancedInductionPhi);
        

        auto AdvancedLoopPredicate = LoopPredicate->clone();
        AdvancedLoopPredicate->insertAfter(LoopPredicate);
        AdvancedLoopPredicate->replaceUsesOfWith(InductionPhi, 
                                                 AdvancedInductionPhi);

        // Now that we have a predicate that is true iff next iter executes.
        // To get isLastIter, we need to negate it. 
        IRBuilder<> Builder(LoopBranch);
        auto IsLastIterCheck =
            Builder.CreateNot(AdvancedLoopPredicate, "IsLastIter");
        // Return result with the correct type.
        return Builder.CreateCast(Instruction::CastOps::ZExt, IsLastIterCheck,
                                  ResultType, "IsLastIter");
      }
    }
  }

  return nullptr;
}

/// For each non-monotonic outer loop of a given memory request, generate a
/// last-iteration predicate bit and store it into the AGU request.
void addIsLastIterInstructions(Function &F, MemoryRequest &Req) {
  for (int iD = 0; iD <= Req.loopDepth; ++iD) {
    if (!Req.isLastIterNeeded[iD])
      continue;
    
    auto L = Req.loopNest[iD];
    auto IsLastIterType = Req.isLastIterReqStore[iD]->getOperand(0)->getType();

    if (auto IsLastIterCheck = createIsLastIterCheck(L, IsLastIterType)) {
      auto IsLastIterStore = Req.isLastIterReqStore[iD]->clone();
      IsLastIterStore->insertAfter(getFirstBodyBlock(L)->getFirstNonPHI());
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
