#include "DynamicLoopFusionAnalysis.h"
#include "CDG.h"
#include "CommonLLVM.h"

using namespace llvm;

namespace llvm {

using MemoryRequestType = DynamicLoopFusionAnalysis::MemoryRequestType;
using DecoupledLoopType = DynamicLoopFusionAnalysis::DecoupledLoopType;
using MemoryRequest = DynamicLoopFusionAnalysis::MemoryRequest;
using DecoupledLoopInfo = DynamicLoopFusionAnalysis::DecoupledLoopInfo;
using MemoryDependencyInfo = DynamicLoopFusionAnalysis::MemoryDependencyInfo;
using DepDir = DynamicLoopFusionAnalysis::DepDir;

std::string DynamicLoopFusionAnalysis::MemoryRequest::getPipeName(
    DecoupledLoopType loopType) {
  if (type == MemoryRequestType::protectedMem &&
      loopType == DecoupledLoopType::agu) {
    if (isaLoad(memOp))
      return "LoadReqPipes_" + std::to_string(memoryId);
    else
      return "StoreReqPipes_" + std::to_string(memoryId);
  } else if (type == MemoryRequestType::protectedMem &&
             loopType == DecoupledLoopType::compute) {
    if (isaLoad(memOp))
      return "LoadValPipes_" + std::to_string(memoryId);
    else
      return "StoreValPipes_" + std::to_string(memoryId);
  } else if (type == MemoryRequestType::simpleMem) {
    if (isaLoad(memOp))
      return "MemoryLoadPipe_" + std::to_string(loopId) + "_" +
             std::to_string(reqId);
    else
      return "MemoryStorePipe_" + std::to_string(loopId) + "_" +
             std::to_string(reqId);
  }

  assert(false && "Could not create pipe name.");
  return "";
}

void DynamicLoopFusionAnalysis::MemoryRequest::collectPipeCalls(
    Function &F, DecoupledLoopType loopType) {
  std::string pipeName = getPipeName(loopType);
  if (type == MemoryRequestType::protectedMem) {
    // In the AGU, stores to protected memory have another dimension.
    if (loopType == DecoupledLoopType::agu && isaStore(memOp)) {
      pipeCalls.push_back(getPipeCall(F, pipeName, {reqId, 0}));
      pipeCalls.push_back(getPipeCall(F, pipeName, {reqId, 1}));
    } else {
      pipeCalls.push_back(getPipeCall(F, pipeName, {reqId}));
    }
  } else {
    // A store to a non-protected memory uses a 1D pipe.
    pipeCalls.push_back(getPipeCall(F, pipeName));
  }
}

void DynamicLoopFusionAnalysis::MemoryRequest::collectStoresToRequestStruct(
    Function &F, DecoupledLoopType loopType) {
  // We only have stores to memory request stores in the AGU kernels.
  if (loopType != DecoupledLoopType::agu)
    return;

  // Go backwards in the stores, starting at the pipe call.
  auto reqStores = getAllStoresInBlockUpTo(pipeCalls[0]);
  auto currSt = reqStores.begin();

  for (int iD = 0; iD < maxLoopDepthInMemoryId; ++iD) {
    isMaxIterReqStore.push_back(*currSt);
    currSt++;
    schedReqStore.push_back(*currSt);
    currSt++;
  }

  if (isaLoad(memOp)) {
    for (int iSt = 0; iSt < numStoresInMemoryId; ++iSt) {
      isPosDepDistReqStore.push_back(*currSt);
      currSt++;
    }
  }

  addrReqStore = *currSt;
  currSt++;

  // Since we were collecting from the back, need to reverse.
  std::reverse(schedReqStore.begin(), schedReqStore.end());
  std::reverse(isMaxIterReqStore.begin(), isMaxIterReqStore.end());
  std::reverse(isPosDepDistReqStore.begin(), isPosDepDistReqStore.end());
}

Instruction *getBasePtrOfInstr(Instruction *I) {
  if (auto si = dyn_cast<StoreInst>(I))
    return getPointerBase(si->getPointerOperand());
  else if (auto li = dyn_cast<LoadInst>(I))
    return getPointerBase(li->getPointerOperand());
  return nullptr;
};

SmallVector<Loop *> getLoopNest(Loop *L) {
  SmallVector<Loop *> LoopNest;
  auto ParentL = L;
  while (ParentL) {
    LoopNest.push_back(ParentL);
    ParentL = ParentL->getParentLoop();
  }
  std::reverse(LoopNest.begin(), LoopNest.end());

  return LoopNest;
}

/// Collect loops to decouple. A loop will be marked for decoupling if it has
/// a body (not counting sub-loops) with side effects (memory operations).
void DynamicLoopFusionAnalysis::collectComputeLoops(LoopInfo &LI) {
  int id = 0;
  for (auto L : LI.getLoopsInPreorder()) {
    if (L->isInnermost()) {
      std::string kernelName =
          (id == 0) ? fName : fName + "_" + std::to_string(id);
      DecoupledLoopInfo DecoupleInfo = {.id = id++,
                                        .type = DecoupledLoopType::compute,
                                        .loops = getLoopNest(L),
                                        .kernelName = kernelName};
      ComputeLoops.push_back(DecoupleInfo);
    }
  }
}

/// A given memory (identified by an opencl base ptr) will be marked for
/// protection, if there exist at least two decoupled loops accessing the base
/// pointer and one of the accesses is a store.
void DynamicLoopFusionAnalysis::collectBasePointersToProtect(LoopInfo &LI) {
  auto anotherLoopHasMemOpWithBasePtr = [&](Loop *L, Instruction *BasePtr) {
    for (auto otherDecoupleInfo : ComputeLoops)
      for (auto otherL : otherDecoupleInfo.loops)
        if (otherL != L)
          for (auto otherI : getUniqueLoopInstructions(otherL))
            if (getBasePtrOfInstr(otherI) == BasePtr)
              return true;

    return false;
  };

  for (auto &decoupleInfo : ComputeLoops) {
    for (auto L : decoupleInfo.loops) {
      for (auto storeI : getUniqueLoopInstructions(L)) {
        if (isa<StoreInst>(storeI)) {
          const auto BasePtr = getBasePtrOfInstr(storeI);
          if (!BasePtrsToProtect.contains(BasePtr) &&
              anotherLoopHasMemOpWithBasePtr(L, BasePtr))
            BasePtrsToProtect.insert(BasePtr);
        }
      }
    }
  }
}

/// For each base pointer to protect, collect all memory requests using it.
void DynamicLoopFusionAnalysis::collectProtectedMemoryRequests(LoopInfo &LI) {
  MapVector<Instruction *, int> BasePtrId, BasePtrNumStores, BasePtrNumLoads,
      BasePtrMaxLoopDepth;
  for (auto [id, BasePtr] : llvm::enumerate(BasePtrsToProtect)) {
    BasePtrId[BasePtr] = 100 + id;
    BasePtrNumStores[BasePtr] = 0;
    BasePtrNumLoads[BasePtr] = 0;
    BasePtrMaxLoopDepth[BasePtr] = 0;
  }

  for (auto &decoupleInfo : ComputeLoops) {
    SmallVector<Loop *> CurrentLoopNest;
    for (auto L : decoupleInfo.loops) {
      CurrentLoopNest.push_back(L);

      for (auto I : getUniqueLoopInstructions(L)) {
        if (auto BasePtr = getBasePtrOfInstr(I)) {
          if (BasePtrsToProtect.contains(BasePtr)) {
            // LLVM starts loops depths at 1, we start at 0.
            int ReqLoopDepth = L->getLoopDepth() - 1;
            MemoryRequest Req{.memoryId = BasePtrId[BasePtr],
                              .loopId = decoupleInfo.id,
                              .type = MemoryRequestType::protectedMem,
                              .memOp = I,
                              .basePtr = BasePtr,
                              .loopDepth = ReqLoopDepth,
                              .loopNest = CurrentLoopNest};
            if (isaLoad(I)) {
              Req.reqId = BasePtrNumLoads[BasePtr]++;
              decoupleInfo.loads.push_back(Req);
            } else {
              Req.reqId = BasePtrNumStores[BasePtr]++;
              decoupleInfo.stores.push_back(Req);
            }

            BasePtrMaxLoopDepth[BasePtr] =
                std::max(BasePtrMaxLoopDepth[BasePtr], ReqLoopDepth + 1);
          }
        }
      }
    }
  }

  // Add info about max loop depth and num of other memory requests.
  for (auto &DecoupleInfo : ComputeLoops) {
    auto &&AllMemoryRequests = llvm::concat<MemoryRequest>(DecoupleInfo.loads, 
                                                           DecoupleInfo.stores);
    for (auto &Req : AllMemoryRequests) {
      Req.numLoadsInMemoryId = BasePtrNumLoads[Req.basePtr];
      Req.numStoresInMemoryId = BasePtrNumStores[Req.basePtr];
      Req.maxLoopDepthInMemoryId = BasePtrMaxLoopDepth[Req.basePtr];
    }
  }
}

const SCEV *getLastUnaryScev(const SCEV *scev) {
  return scev->operands().empty() ? scev
                                  : getLastUnaryScev(scev->operands().back());
}

void getAllAddRecurrences(const SCEV *scev,
                          SmallVector<const SCEVAddRecExpr *> &collection) {
  if (scev->getSCEVType() == SCEVTypes::scAddRecExpr) {
    collection.push_back(cast<SCEVAddRecExpr>(scev));
  }

  for (auto Op : scev->operands()) {
    getAllAddRecurrences(Op, collection);
  }
}

MapVector<const Loop *, const SCEVAddRecExpr *>
getLoopToAddrRecMap(const SCEV *AddrScev, const SmallVector<Loop *> loops) {
  SmallVector<const SCEVAddRecExpr *> AllRecurrences;
  getAllAddRecurrences(AddrScev, AllRecurrences);

  MapVector<const Loop *, const SCEVAddRecExpr *> Result;
  for (auto AddRec : AllRecurrences) {
    Result[AddRec->getLoop()] = AddRec;
  }

  return Result;
}

bool isMaxIterNeededForLoop(
    ScalarEvolution &SE, Loop *L,
    MapVector<const Loop *, const SCEVAddRecExpr *> &LoopNestRecurrences) {
  if (L->isInnermost()) {
    // TODO: Check for "__attribute__((annotate("monotonic")))"?
    return false;
  }
  if (!LoopNestRecurrences.contains(L)) {
    return true;
  }

  // The max step value in the address expression when advancing in L.
  auto LoopStepScev = getLastUnaryScev(LoopNestRecurrences[L]);
  APInt LoopStepMaxVal = SE.getUnsignedRangeMax(LoopStepScev);
  for (auto [SubLoop, SubLoopScev] : LoopNestRecurrences) {
    if (SubLoop->getLoopDepth() <= L->getLoopDepth()) {
      continue;
    }

    // The max step value in the address expression when advancing in subLoop.
    const auto SubLoopStepScev = getLastUnaryScev(SubLoopScev);
    APInt SubLoopStepMaxVal = SE.getUnsignedRangeMax(SubLoopStepScev);

    // If the step in a subloop is larger than the step of {L}, then L the
    // address is not monotonic as a function of advancing in L.
    if (LoopStepMaxVal.ult(SubLoopStepMaxVal)) {
      return true;
    }
  }

  return false;
}

void DynamicLoopFusionAnalysis::checkIsMaxIterNeeded(LoopInfo &LI,
                                                     ScalarEvolution &SE) {
  for (auto &DecoupleInfo : ComputeLoops) {
    auto &&AllMemoryRequests =
        llvm::concat<MemoryRequest>(DecoupleInfo.loads, DecoupleInfo.stores);

    for (auto &Req : AllMemoryRequests) {
      if (auto AddrVal = getPointerOperand(Req.memOp)) {
        auto AddrScev = SE.removePointerBase(SE.getSCEV(AddrVal));
        auto Loop2AddRec = getLoopToAddrRecMap(AddrScev, Req.loopNest);

        for (int iD = 0; iD < Req.maxLoopDepthInMemoryId; ++iD) {
          if (iD <= Req.loopDepth) {
            Req.isMaxIterNeeded.push_back(
                isMaxIterNeededForLoop(SE, Req.loopNest[iD], Loop2AddRec));
          } else {
            Req.isMaxIterNeeded.push_back(false);
          }
        }
      }
    }
  }
}

MapVector<int, SmallVector<MemoryRequest *>>
getCluesteredLoadRequests(SmallVector<DecoupledLoopInfo, 4> &loopsToDecouple) {
  MapVector<int, SmallVector<MemoryRequest *>> loadsForMem;
  for (auto &DecoupleInfo : loopsToDecouple) {
    for (auto &Req : DecoupleInfo.loads) {
      if (!loadsForMem.contains(Req.memoryId)) {
        loadsForMem[Req.memoryId] = SmallVector<MemoryRequest *>{};
      }
      loadsForMem[Req.memoryId].push_back(&Req);
    }
  }
  return loadsForMem;
}

MapVector<int, SmallVector<MemoryRequest *>>
getCluesteredStoreRequests(SmallVector<DecoupledLoopInfo, 4> &loopsToDecouple) {
  MapVector<int, SmallVector<MemoryRequest *>> storesForMem;
  for (auto &DecoupleInfo : loopsToDecouple) {
    for (auto &Req : DecoupleInfo.stores) {
      if (!storesForMem.contains(Req.memoryId)) {
        storesForMem[Req.memoryId] = SmallVector<MemoryRequest *>{};
      }
      storesForMem[Req.memoryId].push_back(&Req);
    }
  }
  return storesForMem;
}

/// Return the deepest loop depth where the L1 and L2 loop nests share a loop. 
int getCommonLoopDepth(Loop *L1, Loop *L2) {
  SmallVector<Loop *> nestL1 = getLoopNest(L1);
  SmallVector<Loop *> nestL2 = getLoopNest(L2);

  int Res = -1;
  for (auto &l1 : nestL1) {
    for (auto &l2 : nestL2) {
      // We start counting at 0, LLVM starts at 1.
      int loopDepth = l1->getLoopDepth() - 1;
      if (l1 == l2 && loopDepth > Res)
        Res = loopDepth;
    }
  }

  return Res;
}

/// Return the direction of the A --depends-on--> B dependency. Return
/// ENUM_DIR::BACK if A comes first in program order, else return FORWARD.
DepDir getDependencyDir(Instruction *A, Instruction *B, LoopInfo &LI) {
  auto LA = LI.getLoopFor(A->getParent());
  auto LB = LI.getLoopFor(B->getParent());

  if (LA == LB) {
    if (A->getParent() == B->getParent()) {
      return getIndexIntoParent(A) < getIndexIntoParent(B) ? DepDir::BACK
                                                           : DepDir::FORWARD;
    }

    for (auto BB : LA->blocks()) {
      if (BB == A->getParent()) {
        return DepDir::BACK;
      } else if (BB == B->getParent()) {
        return DepDir::FORWARD;
      }
    }
  }

  for (auto L : LI.getLoopsInPreorder()) {
    if (L == LA) {
      return DepDir::BACK;
    } else if (L == LB) {
      return DepDir::FORWARD;
    }
  }

  return DepDir::FORWARD;
}

void DynamicLoopFusionAnalysis::collectProtectedMemoryInfo(LoopInfo &LI) {
  auto LoadsForMem = getCluesteredLoadRequests(ComputeLoops);
  auto StoresForMem = getCluesteredStoreRequests(ComputeLoops);

  // init
  for (auto kv : StoresForMem) {
    ProtectedMemoryInfo[kv.first] = MemoryDependencyInfo{
        .id = kv.first,
        .numLoads = int(LoadsForMem[kv.first].size()),
        .numStores = int(StoresForMem[kv.first].size()),
        .cType = getCTypeString(LoadsForMem[kv.first][0]->memOp)};
  }

  for (auto [MemId, LdRequests] : LoadsForMem) {
    for (auto &LdReq : LdRequests) {
      auto LdLoop = LI.getLoopFor(LdReq->memOp->getParent());
      ProtectedMemoryInfo[MemId].maxLoopDepth = LdReq->maxLoopDepthInMemoryId;
      ProtectedMemoryInfo[MemId].loadLoopDepth.push_back(LdReq->loopDepth);
      ProtectedMemoryInfo[MemId].loadIsMaxIterNeeded.push_back(
          LdReq->isMaxIterNeeded);

      SmallVector<bool> StoreInSameLoop;
      SmallVector<int> StoreCommonLoopDepth;
      SmallVector<DepDir> StoreDepDir;
      for (auto &StReq : StoresForMem[MemId]) {
        auto StLoop = LI.getLoopFor(StReq->memOp->getParent());
        StoreInSameLoop.push_back(LdLoop == StLoop);
        StoreCommonLoopDepth.push_back(getCommonLoopDepth(LdLoop, StLoop));
        StoreDepDir.push_back(getDependencyDir(LdReq->memOp, StReq->memOp, LI));
      }
      ProtectedMemoryInfo[MemId].loadStoreInSameLoop.push_back(StoreInSameLoop);
      ProtectedMemoryInfo[MemId].loadStoreCommonLoopDepth.push_back(
          StoreCommonLoopDepth);
      ProtectedMemoryInfo[MemId].loadStoreDepDir.push_back(StoreDepDir);
    }
  }

  for (auto [MemId, StRequests] : StoresForMem) {
    for (auto StReq : StRequests) {
      auto StReqLoop = LI.getLoopFor(StReq->memOp->getParent());
      ProtectedMemoryInfo[MemId].maxLoopDepth = StReq->maxLoopDepthInMemoryId;
      ProtectedMemoryInfo[MemId].storeLoopDepth.push_back(StReq->loopDepth);
      ProtectedMemoryInfo[MemId].storeIsMaxIterNeeded.push_back(
          StReq->isMaxIterNeeded);

      SmallVector<bool> StoreInSameLoop;
      SmallVector<int> StoreCommonLoopDepth;
      SmallVector<DepDir> StoreDepDir;
      for (auto &OtherStReq : StoresForMem[MemId]) {
        auto OtherStLoop = LI.getLoopFor(OtherStReq->memOp->getParent());
        StoreInSameLoop.push_back(StReqLoop == OtherStLoop);
        StoreCommonLoopDepth.push_back(
            getCommonLoopDepth(StReqLoop, OtherStLoop));
        StoreDepDir.push_back(
            getDependencyDir(StReq->memOp, OtherStReq->memOp, LI));
      }
      ProtectedMemoryInfo[MemId].storeStoreInSameLoop.push_back(
          StoreInSameLoop);
      ProtectedMemoryInfo[MemId].storeStoreCommonLoopDepth.push_back(
          StoreCommonLoopDepth);
      ProtectedMemoryInfo[MemId].storeStoreDepDir.push_back(StoreDepDir);
    }
  }

}

void DynamicLoopFusionAnalysis::collectAguLoops() {
  auto loopHasAnyProtectedMemOp = [&](Loop *L) {
    for (auto I : getUniqueLoopInstructions(L)) 
      if (auto BasePtr = getBasePtrOfInstr(I)) 
        if (BasePtrsToProtect.contains(BasePtr)) 
          return true;
    
    return false;
  };

  for (auto &decoupleInfo : ComputeLoops) {
    for (auto L : decoupleInfo.loops) {
      if (loopHasAnyProtectedMemOp(L)) {
        DecoupledLoopInfo AguLoop = {decoupleInfo};
        AguLoop.type = DecoupledLoopType::agu;
        AguLoop.kernelName += "_agu";
        AguLoops.push_back(AguLoop);
        break;
      }
    }
  }

  // Remove loops from AGU that do not contribute to the generation of requests.
  for (auto &AguLoop : AguLoops) {
    for (auto L : llvm::reverse(AguLoop.loops)) {
      if (!loopHasAnyProtectedMemOp(L))
        AguLoop.loops.pop_back();
      else
        break;
    }
  }
}

void addRequestToComputeLoop(MemoryRequest &Req, SmallVector<DecoupledLoopInfo, 4> &ComputeLoops) {
  for (auto &decoupleInfo : ComputeLoops) {
    if (llvm::is_contained(decoupleInfo.loops, Req.loopNest.back())) {
      MemoryRequest NewReq = {Req};
      NewReq.loopId = decoupleInfo.id;
      if (isaLoad(Req.memOp))
        decoupleInfo.loads.push_back(Req);
      else
        decoupleInfo.stores.push_back(Req);
    }
  }
}

/// Each memory load/store, that does not require hazard protection, gets
/// decoupled into its own loop.
void DynamicLoopFusionAnalysis::collectSimpleMemoryLoops(LoopInfo &LI) {
  int id = 0;
  for (auto L : LI.getLoopsInPreorder()) {
    for (auto I : getUniqueLoopInstructions(L)) {
      // Don't decouple loads.
      if (isaLoad(I))
        continue;

      if (auto BasePtr = getBasePtrOfInstr(I)) {
        bool isGlobalPtr = BasePtr->getParent() ==
                           &BasePtr->getParent()->getParent()->getEntryBlock();
        if (!BasePtrsToProtect.contains(BasePtr) && isGlobalPtr) {
          std::string kernelName = fName + "_mem_" + std::to_string(id);
          DecoupledLoopInfo DecoupleInfo = {.id = id,
                                            .type = DecoupledLoopType::memory,
                                            .loops = getLoopNest(L),
                                            .kernelName = kernelName};

          MemoryRequest Req = {.loopId = id,
                               .reqId = 0,
                               .type = MemoryRequestType::simpleMem,
                               .memOp = I,
                               .basePtr = BasePtr,
                               .loopDepth = int(L->getLoopDepth() - 1),
                               .loopNest = getLoopNest(L)};
          if (isaLoad(I))
            DecoupleInfo.loads.push_back(Req);
          else
            DecoupleInfo.stores.push_back(Req);

          addRequestToComputeLoop(Req, ComputeLoops);

          SimpleMemoryLoops.push_back(DecoupleInfo);
          id++;
        }
      }
    }
  }
}

} // end namespace llvm
