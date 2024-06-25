#include "DynamicLoopFusionAnalysis.h"
#include "CDG.h"
#include "CommonLLVM.h"

using namespace llvm;

namespace llvm {

using MemoryRequest = DynamicLoopFusionAnalysis::MemoryRequest;
using DecoupledLoopInfo = DynamicLoopFusionAnalysis::DecoupledLoopInfo;
using MemoryDependencyInfo = DynamicLoopFusionAnalysis::MemoryDependencyInfo;
using DepDir = DynamicLoopFusionAnalysis::DepDir;

template <typename ChildT>
bool loopSpanContains(Loop *Inner, Loop *Outer, ChildT *Child) {
  static_assert(std::is_same_v<ChildT, BasicBlock> ||
                    std::is_same_v<ChildT, Instruction>,
                "Only basic blocks and instructions supported.");

  auto Parent = Inner;
  do {
    SmallVector<ChildT *> ElemsInParent;
    if constexpr (std::is_same_v<ChildT, BasicBlock>)
      ElemsInParent = getUniqueLoopBlocks(Parent);
    else
      ElemsInParent = getUniqueLoopInstructions(Parent);

    if (llvm::is_contained(ElemsInParent, Child)) {
      return true;
    }
    Parent = Parent->getParentLoop();
  } while (Parent != Outer && Parent);

  return false;
}

Instruction *getBasePtrOfInstr(Instruction *I) {
  if (auto si = dyn_cast<StoreInst>(I))
    return getPointerBase(si->getPointerOperand());
  else if (auto li = dyn_cast<LoadInst>(I))
    return getPointerBase(li->getPointerOperand());
  return nullptr;
};

/// Collect loops to decouple. A loop will be marked for decoupling if it has
/// a body (not counting sub-loops) with side effects (memory operations).
void DynamicLoopFusionAnalysis::collectLoopsToDecouple(LoopInfo &LI) {
  int id = 0;
  for (auto L : LI.getLoopsInPreorder()) {
    if (L->isInnermost()) {
      DecoupledLoopInfo DecoupleInfo = {id++};

      auto ParentL = L;
      while (ParentL) {
        DecoupleInfo.loops.push_back(ParentL);
        ParentL = ParentL->getParentLoop();
      }
      std::reverse(DecoupleInfo.loops.begin(), DecoupleInfo.loops.end());

      loopsToDecouple.push_back(DecoupleInfo);
    }
  }
}

/// A given memory (identified by an opencl base ptr) will be marked for
/// protection, if there exist at least two decoupled loops accessing the base
/// pointer and one of the accesses is a store.
void DynamicLoopFusionAnalysis::collectBasePointersToProtect(LoopInfo &LI) {
  for (auto &decoupleInfo : loopsToDecouple) {
    for (auto L : decoupleInfo.loops) {
      for (auto storeI : getUniqueLoopInstructions(L)) {
        if (isa<StoreInst>(storeI)) {
          const auto BasePtr = getBasePtrOfInstr(storeI);
          if (basePtrsToProtect.contains(BasePtr))
            continue;

          for (auto otherDecoupleInfo : loopsToDecouple) {
            for (auto otherL : otherDecoupleInfo.loops) {
              if (otherL != L) {
                for (auto otherI : getUniqueLoopInstructions(otherL)) {
                  if (getBasePtrOfInstr(storeI) == getBasePtrOfInstr(otherI)) {
                    basePtrsToProtect.insert(BasePtr);
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

/// For each base pointer to protect, collect all memory requests using it.
void DynamicLoopFusionAnalysis::collectMemoryRequests(LoopInfo &LI) {
  MapVector<Instruction *, int> BasePtrId, BasePtrNumStores, BasePtrNumLoads,
      BasePtrMaxLoopDepth;
  for (auto [id, BasePtr] : llvm::enumerate(basePtrsToProtect)) {
    BasePtrId[BasePtr] = 100 + id;
    BasePtrNumStores[BasePtr] = 0;
    BasePtrNumLoads[BasePtr] = 0;
    BasePtrMaxLoopDepth[BasePtr] = 0;
  }

  for (auto &decoupleInfo : loopsToDecouple) {
    SmallVector<Loop *> currentLoopNest;
    for (auto L : decoupleInfo.loops) {
      currentLoopNest.push_back(L);

      for (auto I : getUniqueLoopInstructions(L)) {
        if (auto BasePtr = getBasePtrOfInstr(I)) {
          if (basePtrsToProtect.contains(BasePtr)) {
            // LLVM starts loops depths at 1, we start at 0.
            int ReqLoopDepth = L->getLoopDepth() - 1;
            BasePtrMaxLoopDepth[BasePtr] =
                std::max(BasePtrMaxLoopDepth[BasePtr], ReqLoopDepth + 1);
            int ReqId = isaLoad(I) ? BasePtrNumLoads[BasePtr]++
                                   : BasePtrNumStores[BasePtr]++;
            MemoryRequest Req{BasePtrId[BasePtr], decoupleInfo.id, ReqId, I, 
                              BasePtr, ReqLoopDepth, currentLoopNest};
            if (isaLoad(I))
              decoupleInfo.loads.push_back(Req);
            else
              decoupleInfo.stores.push_back(Req);
          }
        }
      }
    }
  }

  // Add info about max loop depth and num of other memory requests.
  for (auto &DecoupleInfo : loopsToDecouple) {
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
  for (auto &DecoupleInfo : loopsToDecouple) {
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

int getCommonLoopDepth(Loop *L1, Loop *L2) {
  if (!L1 || !L2)
    return -1;
  else if (L1 == L2) 
    return L1->getLoopDepth() - 1;
  else if (L1->getLoopDepth() > L2->getLoopDepth()) 
    return getCommonLoopDepth(L1->getParentLoop(), L2);
  else if (L1->getLoopDepth() < L2->getLoopDepth()) 
    return getCommonLoopDepth(L1, L2->getParentLoop());

  return -1;
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

void DynamicLoopFusionAnalysis::collectMemoryDepInfo(LoopInfo &LI) {
  auto LoadsForMem = getCluesteredLoadRequests(loopsToDecouple);
  auto StoresForMem = getCluesteredStoreRequests(loopsToDecouple);

  // init
  for (auto kv : StoresForMem) {
    memDepInfo[kv.first] = MemoryDependencyInfo{
        kv.first,
        int(LoadsForMem[kv.first].size()),
        int(StoresForMem[kv.first].size()),
    };
    memDepInfo[kv.first].cType = getCTypeString(LoadsForMem[kv.first][0]->memOp);
  }

  for (auto [MemId, LdRequests] : LoadsForMem) {
    for (auto &LdReq : LdRequests) {
      auto LdLoop = LI.getLoopFor(LdReq->memOp->getParent());
      memDepInfo[MemId].maxLoopDepth = LdReq->maxLoopDepthInMemoryId;
      memDepInfo[MemId].loadLoopDepth.push_back(LdReq->loopDepth);
      memDepInfo[MemId].loadIsMaxIterNeeded.push_back(LdReq->isMaxIterNeeded);

      SmallVector<bool> StoreInSameLoop;
      SmallVector<int> StoreCommonLoopDepth;
      SmallVector<DepDir> StoreDepDir;
      for (auto &StReq : StoresForMem[MemId]) {
        auto StLoop = LI.getLoopFor(StReq->memOp->getParent());
        StoreInSameLoop.push_back(LdLoop == StLoop);
        StoreCommonLoopDepth.push_back(getCommonLoopDepth(LdLoop, StLoop));
        StoreDepDir.push_back(getDependencyDir(LdReq->memOp, StReq->memOp, LI));
      }
      memDepInfo[MemId].loadStoreInSameLoop.push_back(StoreInSameLoop);
      memDepInfo[MemId].loadStoreCommonLoopDepth.push_back(StoreCommonLoopDepth);
      memDepInfo[MemId].loadStoreDepDir.push_back(StoreDepDir);
    }
  }

  for (auto [MemId, StRequests] : StoresForMem) {
    for (auto StReq : StRequests) {
      auto StReqLoop = LI.getLoopFor(StReq->memOp->getParent());
      memDepInfo[MemId].maxLoopDepth = StReq->maxLoopDepthInMemoryId;
      memDepInfo[MemId].storeLoopDepth.push_back(StReq->loopDepth);
      memDepInfo[MemId].storeIsMaxIterNeeded.push_back(StReq->isMaxIterNeeded);

      SmallVector<bool> StoreInSameLoop;
      SmallVector<int> StoreCommonLoopDepth;
      SmallVector<DepDir> StoreDepDir;
      for (auto &OtherStReq : StoresForMem[MemId]) {
        auto OtherStLoop = LI.getLoopFor(OtherStReq->memOp->getParent());
        StoreInSameLoop.push_back(StReqLoop == OtherStLoop);
        StoreCommonLoopDepth.push_back(getCommonLoopDepth(StReqLoop, OtherStLoop));
        StoreDepDir.push_back(getDependencyDir(StReq->memOp, OtherStReq->memOp, LI));
      }
      memDepInfo[MemId].storeStoreInSameLoop.push_back(StoreInSameLoop);
      memDepInfo[MemId].storeStoreCommonLoopDepth.push_back(StoreCommonLoopDepth);
      memDepInfo[MemId].storeStoreDepDir.push_back(StoreDepDir);
    }
  }

}

void DynamicLoopFusionAnalysis::collectAGUs() {
  auto loopHasAnyProtectedMemOp = [&](Loop *L) {
    for (auto I : getUniqueLoopInstructions(L)) 
      if (auto BasePtr = getBasePtrOfInstr(I)) 
        if (basePtrsToProtect.contains(BasePtr)) 
          return true;
    
    return false;
  };

  for (auto decoupleInfo : loopsToDecouple) {
    for (auto L : decoupleInfo.loops) {
      if (loopHasAnyProtectedMemOp(L)) {
        agusToDecouple.push_back(decoupleInfo);
        break;
      }
    }
  }

  // Remove loops from AGU that do not contribute to the generation of requests.
  for (auto &agu : agusToDecouple) {
    for (auto L : llvm::reverse(agu.loops)) {
      if (!loopHasAnyProtectedMemOp(L))
        agu.loops.pop_back();
      else
        break;
    }
  }
}

} // end namespace llvm
