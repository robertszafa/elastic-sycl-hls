#include "DynamicLoopFusionAnalysis.h"
#include "CDG.h"
#include "CommonLLVM.h"

using namespace llvm;

namespace llvm {

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
  for (auto L : LI.getLoopsInPreorder()) {
    if (L->isInnermost()) {
      DecoupledLoopInfo DecoupleInfo = {};

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
void DynamicLoopFusionAnalysis::collectMemoriesToProtect(LoopInfo &LI) {
  for (auto decoupleInfo : loopsToDecouple) {
    for (auto L : decoupleInfo.loops) {
      for (auto storeI : getUniqueLoopInstructions(L)) {
        if (isa<StoreInst>(storeI)) {
          const auto BasePtr = getBasePtrOfInstr(storeI);
          if (memoryToProtect.contains(BasePtr))
            continue;

          for (auto otherDecoupleInfo : loopsToDecouple) {
            for (auto otherL : otherDecoupleInfo.loops) {
              if (otherL != L) {
                for (auto otherI : getUniqueLoopInstructions(otherL)) {
                  if (getBasePtrOfInstr(storeI) == getBasePtrOfInstr(otherI)) {
                    memoryToProtect.insert(BasePtr);
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  auto getBasePtrId = [&](Instruction *BasePtr) -> int {
    for (size_t i = 0; i < memoryToProtect.size(); ++i) {
      if (memoryToProtect[i] == BasePtr) {
        return i;
      }
    }
    return memoryToProtect.size();
  };

  for (auto &decoupleInfo : loopsToDecouple) {
    SmallVector<Loop *> currentLoopNest;
    for (auto L : decoupleInfo.loops) {
      currentLoopNest.push_back(L);

      for (auto I : getUniqueLoopInstructions(L)) {
        if (auto BasePtr = getBasePtrOfInstr(I)) {
          if (memoryToProtect.contains(BasePtr)) {
            MemoryRequest Req{getBasePtrId(BasePtr), I, BasePtr,
                              int(L->getLoopDepth() - 1), currentLoopNest};
            if (isaLoad(I))
              decoupleInfo.loads.push_back(Req);
            else
              decoupleInfo.stores.push_back(Req);
          }
        }
      }
    }
  }
}

void DynamicLoopFusionAnalysis::collectAGUs(LoopInfo &LI) {
  for (auto decoupleInfo : loopsToDecouple) {
    for (auto I : getUniqueLoopInstructions(decoupleInfo.inner())) {
      if (auto BasePtr = getBasePtrOfInstr(I)) {
        if (memoryToProtect.contains(BasePtr)) {
          agusToDecouple.push_back(decoupleInfo);
        }
      }
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

        for (auto L : Req.loopNest) {
          Req.isMaxIterNeeded.push_back(
              isMaxIterNeededForLoop(SE, L, Loop2AddRec));
        }
      }
    }
  }
}

} // end namespace llvm
