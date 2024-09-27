#include "DynamicLoopFusionAnalysis.h"
#include "CDG.h"
#include "CommonLLVM.h"

using namespace llvm;

namespace llvm {

using MemoryRequestType = DynamicLoopFusionAnalysis::MemoryRequestType;
using DecoupledLoopType = DynamicLoopFusionAnalysis::DecoupledLoopType;
using MemoryRequest = DynamicLoopFusionAnalysis::MemoryRequest;
using DecoupledLoopInfo = DynamicLoopFusionAnalysis::DecoupledLoopInfo;
using DUInfo = DynamicLoopFusionAnalysis::DUInfo;

void topologicalOrderSort(Function &F, SmallVector<MemoryRequest, 4> &ToSort) {
  MapVector<BasicBlock*, int> TopologicalOrder;
  for (auto [iOrd, BB] : llvm::enumerate(getTopologicalOrder(F)))
    TopologicalOrder[BB] = iOrd;

  llvm::sort(ToSort, [&TopologicalOrder](auto A, auto B) {
    return TopologicalOrder[A.memOp->getParent()] <
           TopologicalOrder[B.memOp->getParent()];
  });
}

/// Return the DecoupledLoopInfo for kernel with {KernelName}.
SmallVector<DecoupledLoopInfo, 4>
DynamicLoopFusionAnalysis::getDecoupledLoops(const std::string &KernelName) {
  SmallVector<DecoupledLoopInfo, 4> KernelLoops;
  for (auto &DecoupleInfo : llvm::concat<DecoupledLoopInfo>(
           this->ComputeLoops, this->AguLoops, this->SimpleMemoryLoops)) {
    if (KernelName == DecoupleInfo.kernelName) {
      KernelLoops.push_back(DecoupleInfo);
    }
  }
  return KernelLoops;
}

/// Return all memory requests for kernel with {KernelName} in topological order
SmallVector<MemoryRequest, 4>
DynamicLoopFusionAnalysis::getKernelRequestsInTopologicalOrder(
    const std::string &KernelName) {
  auto DecoupledLoops = getDecoupledLoops(KernelName);
  SmallVector<MemoryRequest, 4> KernelRequests;
  for (auto &DecoupleInfo : DecoupledLoops) {
    for (auto &Req :
         llvm::concat<MemoryRequest>(DecoupleInfo.loads, DecoupleInfo.stores)) {
      KernelRequests.push_back(Req);
    }
  }
  if (!KernelRequests.empty()) {
    topologicalOrderSort(*KernelRequests[0].memOp->getFunction(), 
                         KernelRequests);
  }

  return KernelRequests;
}

/// Return all memory requests for data unit with {DUID} in topological order.
SmallVector<MemoryRequest, 4>
DynamicLoopFusionAnalysis::getDuRequestsInTopologicalOrder(const int DUID) {
  SmallVector<MemoryRequest, 4> RequestsForDU;
  for (auto &AGU : this->AguLoops) {
    for (auto &Req : llvm::concat<MemoryRequest>(AGU.loads, AGU.stores)) {
      if (Req.memoryId != DUID) {
        continue;
      }
      RequestsForDU.push_back(Req);
    }
  }
  if (!RequestsForDU.empty()) {
    topologicalOrderSort(*RequestsForDU[0].memOp->getFunction(), RequestsForDU);
  }

  return RequestsForDU;
}

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

void DynamicLoopFusionAnalysis::MemoryRequest::collectAGURequestStores(
    Function &F, DecoupledLoopType loopType) {
  // We only have stores to memory request stores in the AGU kernels.
  if (loopType != DecoupledLoopType::agu)
    return;

  // Go backwards in the stores, starting at the pipe call.
  auto storesIntoPipe = getAllStoresInBlockUpTo(this->pipeCalls[0]);
  auto currSt = storesIntoPipe.begin();

  for (int iD = 0; iD < maxLoopDepthInMemoryId; ++iD) {
    this->isLastIterReqStore.push_back(*currSt);
    currSt++;
    this->schedReqStore.push_back(*currSt);
    currSt++;
  }

  if (isaLoad(memOp)) {
    for (int iSt = 0; iSt < numStoresInMemoryId; ++iSt) {
      this->isPosDepDistReqStore.push_back(*currSt);
      currSt++;
    }
  }

  this->addrReqStore = *currSt;
  currSt++;

  // Since we were collecting from the back, need to reverse.
  std::reverse(this->schedReqStore.begin(), this->schedReqStore.end());
  std::reverse(this->isLastIterReqStore.begin(), this->isLastIterReqStore.end());
  std::reverse(this->isPosDepDistReqStore.begin(),
               this->isPosDepDistReqStore.end());
}

void DynamicLoopFusionAnalysis::MemoryRequest::collectStoreValPipeStores(
    Function &F, DecoupledLoopType loopType) {
  if (loopType != DecoupledLoopType::compute || !isPipeWrite(this->pipeCalls[0]))
    return;

  SmallVector<StoreInst *> storesIntoPipe;
  for (auto user : this->pipeCalls[0]->getArgOperand(0)->users()) {
    if (auto gep = dyn_cast<GetElementPtrInst>(user)) {
      for (auto userGEP : gep->users()) {
        if (auto stInstr = dyn_cast<StoreInst>(userGEP)) {
          storesIntoPipe.push_back(stInstr);
          break;
        }
      }
    } else if (auto stInstr = dyn_cast<StoreInst>(user)) {
      storesIntoPipe.push_back(stInstr);
      break;
    }
  }
  
  // Not a {value, valid} struct.
  if (storesIntoPipe.size() != 2)
    return;

  this->storeValueStore = storesIntoPipe[1];
  this->storeValidStore = storesIntoPipe[0];
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

/// Return true if {I} should not be included in the DecoupledLoop.
/// For example, ld0 should be included in loop j, ld1 and ld2 go into loop k:
///   for i:
///     ld0
///     for j: ...
///     ld1
///     for k: ...
///     ld2
bool instrBelongsToAnotherDecoupledLoop(Instruction *I, Loop *ILoop,
                                        DecoupledLoopInfo &DecoupledInfo) {
  auto getImmediateSubLoops = [](auto ParentL, auto Loops) {
    SmallVector<Loop *> Res;
    for (auto L : Loops) {
      if (L->getParentLoop() == ParentL)
        Res.push_back(L);
    }

    return Res;
  };

  if (ILoop == DecoupledInfo.inner())
    return false;

  auto ILoopChildren = getImmediateSubLoops(ILoop, ILoop->getLoopsInPreorder());
  Loop *PointInDecoupled = getImmediateSubLoops(ILoop, DecoupledInfo.loops)[0];
  bool DecoupledLoopComesBeforeI = isReachableWithinLoop(
      PointInDecoupled->getHeader(), I->getParent(), ILoop);
  bool DecoupledLoopIsLastImmediateChildOfILoop =
      ILoopChildren.back() != PointInDecoupled;

  return DecoupledLoopIsLastImmediateChildOfILoop && DecoupledLoopComesBeforeI;
}

/// For each base pointer to protect, collect all memory requests using it.
void DynamicLoopFusionAnalysis::collectDURequests(LoopInfo &LI) {
  MapVector<Instruction *, int> BasePtrId, BasePtrNumStores, BasePtrNumLoads,
      BasePtrMaxLoopDepth;
  for (auto [id, BasePtr] : llvm::enumerate(BasePtrsToProtect)) {
    BasePtrId[BasePtr] = 100 + id;
    BasePtrNumStores[BasePtr] = 0;
    BasePtrNumLoads[BasePtr] = 0;
    BasePtrMaxLoopDepth[BasePtr] = 0;
  }

  SetVector<Instruction *> done; 
  for (auto &DecoupledInfo : ComputeLoops) {
    SmallVector<Loop *> CurrentLoopNest;
    for (auto L : DecoupledInfo.loops) {
      CurrentLoopNest.push_back(L);

      for (auto I : getUniqueLoopInstructions(L)) {
        // 1. Only mem ops. 
        // 2. One memory request object per mem op.
        // 3. If a memory requests is part of loop nests of multiple decoupled
        //    threads, then add it to the first thread whose inner loop is
        //    dominated by the memory request.
        if (!(isaLoad(I) || isaStore(I)) || done.contains(I) ||
            instrBelongsToAnotherDecoupledLoop(I, L, DecoupledInfo))
          continue;

        if (auto BasePtr = getBasePtrOfInstr(I)) {
          if (BasePtrsToProtect.contains(BasePtr)) {
            done.insert(I);

            // LLVM starts loops depths at 1, we start at 0.
            int ReqLoopDepth = L->getLoopDepth() - 1;
            MemoryRequest Req{.memoryId = BasePtrId[BasePtr],
                              .loopId = DecoupledInfo.id,
                              .type = MemoryRequestType::protectedMem,
                              .memOp = I,
                              .basePtr = BasePtr,
                              .loopDepth = ReqLoopDepth,
                              .loopNest = CurrentLoopNest};
            if (isaLoad(I)) {
              Req.reqId = BasePtrNumLoads[BasePtr]++;
              DecoupledInfo.loads.push_back(Req);
            } else {
              Req.reqId = BasePtrNumStores[BasePtr]++;
              DecoupledInfo.stores.push_back(Req);
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

/// Given a SCEV value, collect all sub-expressions that are a SCEVAddRecExpr.
void getAllAddRecurrences(const SCEV *scev,
                          SmallVector<const SCEVAddRecExpr *> &collection) {
  if (scev->getSCEVType() == SCEVTypes::scAddRecExpr) {
    collection.push_back(cast<SCEVAddRecExpr>(scev));
  }

  for (auto Op : scev->operands()) {
    getAllAddRecurrences(Op, collection);
  }
}

/// Given an address value, map each loop in the loop nest to its step value in
/// the SCEV representing the address value.
MapVector<const Loop *, const SCEV *>
getLoopsStepSCEV(Value *AddressVal, ScalarEvolution &SE,
                 const SmallVector<Loop *> &loopNest) {
  // Ensure all SCEVs are always i64. 
  auto i64Type = Type::getInt64Ty(SE.getContext());

  auto AddrScev = SE.removePointerBase(SE.getSCEV(AddressVal));
  // Collect address recurrences.
  SmallVector<const SCEVAddRecExpr *> AllRecurrences;
  getAllAddRecurrences(AddrScev, AllRecurrences);

  // Map each address recurrence to a loop in the loop nest, get its step SCEV.
  MapVector<const Loop *, const SCEV *> Result;
  for (auto AddRec : AllRecurrences) {
    auto LoopRec = AddRec->getStepRecurrence(SE);
    Result[AddRec->getLoop()] = SE.getNoopOrAnyExtend(LoopRec, i64Type);
  }

  return Result;
}

/// Return the number of iterations in loop {L}. If the number is not
/// analyzable, return a maximum int value.
const SCEV *getNumLoopItersOrMax(Loop *L, ScalarEvolution &SE) {
  auto i64Type = Type::getInt64Ty(SE.getContext());

  auto numLoopItersSCEV = SE.getBackedgeTakenCount(L);
  if (numLoopItersSCEV->getSCEVType() == scCouldNotCompute) {
    numLoopItersSCEV = SE.getSCEV(ConstantInt::get(i64Type, ~0ULL));
  }

  return SE.getNoopOrAnyExtend(numLoopItersSCEV, i64Type);
}

/// Return loopStepValue * loopNumIters.
/// If we cannot analyze loopNumIters, it will be set to MAX_INT.
APInt getLoopStepSum(Loop *L, const SCEV *LoopStepSCEV, ScalarEvolution &SE) {
  // If the loop doesn't have a SCEV, then step is 0, and so is the step sum.
  if (!LoopStepSCEV) {
    return APInt::getZero(64);
  }

  auto numLoopItersSCEV = getNumLoopItersOrMax(L, SE);
  APInt maxNumLoopIters = SE.getUnsignedRangeMax(numLoopItersSCEV);
  auto sumInnerStepSCEV = SE.getMulExpr(numLoopItersSCEV, LoopStepSCEV);

  return SE.getUnsignedRangeMax(sumInnerStepSCEV);
}

/// For each outer loop of each memory request, check if it is non-monotonic
/// w.r.t. to the evolution of the address value.
void DynamicLoopFusionAnalysis::checkisLastIterNeeded(LoopInfo &LI,
                                                      ScalarEvolution &SE) {
  for (auto &DecoupleInfo : ComputeLoops) {
    auto &&AllMemoryRequests =
        llvm::concat<MemoryRequest>(DecoupleInfo.loads, DecoupleInfo.stores);

    for (auto &Req : AllMemoryRequests) {
      // Init.
      for (int iD = 0; iD < Req.maxLoopDepthInMemoryId; ++iD) {
        Req.isLastIterNeeded.push_back(iD < Req.loopDepth);
      }

      // Now, we set isLastIterCheckNeeded to false if the corresponding loop
      // will never cause a wrap around in the address expression.
      if (auto PtrOp = getPointerOperand(Req.memOp)) {
        MapVector<const Loop *, const SCEV *> LoopStepSCEVs =
            getLoopsStepSCEV(PtrOp, SE, Req.loopNest);

        // Calculate the maximum possible offset to the address attributablke to
        // the execution of the entire inner loop.
        Loop *InnermostLoop = LI.getLoopFor(Req.memOp->getParent());
        APInt InnermostLoopStepSum =
            getLoopStepSum(InnermostLoop, LoopStepSCEVs[InnermostLoop], SE);

        /// Now, each outer loop with a address_SCEV.step value that is lower
        /// than InnermostLoopStepSum is non-monotonic.
        for (int iD = 0; iD < Req.loopDepth; ++iD) {
          // If the outer loop doesn't have a step, then it's non-monotonic.
          if (LoopStepSCEVs.contains(Req.loopNest[iD])) {
            APInt OuterLoopStep =
                SE.getUnsignedRangeMax(LoopStepSCEVs[Req.loopNest[iD]]);
            Req.isLastIterNeeded[iD] = OuterLoopStep.ult(InnermostLoopStepSum);
          }

          }
      }
    }
  }
}

/// Cluster allRequests, loadRequests, storeRequests based on the Data Unit id.
void collectCluesteredRequests(
    const SmallVector<DecoupledLoopInfo, 4> &loopsToDecouple,
    MapVector<int, SmallVector<MemoryRequest, 4>> &RequestsForMem,
    MapVector<int, SmallVector<MemoryRequest, 4>> &LoadsForMem,
    MapVector<int, SmallVector<MemoryRequest, 4>> &StoresForMem) {
  for (auto &DecoupleInfo : loopsToDecouple) {
    for (auto &Req : DecoupleInfo.loads) {
      if (!RequestsForMem.contains(Req.memoryId)) {
        RequestsForMem[Req.memoryId] = SmallVector<MemoryRequest, 4>{};
        LoadsForMem[Req.memoryId] = SmallVector<MemoryRequest, 4>{};
      }
      LoadsForMem[Req.memoryId].push_back(Req);
      RequestsForMem[Req.memoryId].push_back(Req);
    }
    for (auto &Req : DecoupleInfo.stores) {
      if (!RequestsForMem.contains(Req.memoryId)) {
        RequestsForMem[Req.memoryId] = SmallVector<MemoryRequest, 4>{};
        StoresForMem[Req.memoryId] = SmallVector<MemoryRequest, 4>{};
      }
      StoresForMem[Req.memoryId].push_back(Req);
      RequestsForMem[Req.memoryId].push_back(Req);
    }
  }
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

/// Return the mem instruction and request id of the dependency source for the
/// {DepDst} request in loop {L}.
std::pair<Instruction *, int>
getDependencySourceInLoop(MemoryRequest &DepDst, Loop *L,
                          SmallVector<MemoryRequest, 4> &AllRequests,
                          bool LoadCanBeSource) {
  // If no common loop, then take all requests.
  auto LoopReqs = make_filter_range(AllRequests, [&](MemoryRequest &Req) {
    return llvm::is_contained(Req.loopNest, L) || (L == nullptr);
  });
  auto BackedgeReqs = make_filter_range(AllRequests, [&](MemoryRequest &Req) {
    return llvm::is_contained(Req.loopNest, L) && Req.memOp != DepDst.memOp;
  });

  // At most one dep src in a loop. Two cases:
  //  1: .., st_n, ld  --> load only needs to compare against st_n
  //  2: ld, ..., st_m --> load only needs to compare against st_m
  // The below loop duplicates the memory requests in the range (except the req
  // of interest):
  //  [ld0, ld1*, st0] becomes [ld0, st0, ld0, ld1*, st0]
  // and records the last store id before hitting our LoadReq.
  Instruction *DepSrcMemOp = nullptr;
  int DepSrcId = -1;
  for (auto &Req : llvm::concat<MemoryRequest>(BackedgeReqs, LoopReqs)) {
    if (Req.memOp == DepDst.memOp) {
      break;
    } else if (isaStore(Req.memOp) || LoadCanBeSource) {
      DepSrcMemOp = Req.memOp;
      DepSrcId = Req.reqId;
    }
  }

  return {DepSrcMemOp, DepSrcId};
}

/// Collect the load/store request id of {Req} dependency sources.
void collectDependencySources(MemoryRequest &Req,
                              SmallVector<MemoryRequest, 4> &AllRequests,
                              SetVector<int> &LoadDepSources,
                              SetVector<int> &StoreDepSources) {
  bool LoadCanBeSource = isaStore(Req.memOp);
  // To also collect possible RAW deps from other loops, we go over ALlReqs,
  // i.e., the common loop is null.
  SmallVector<Loop *> noCommonLoop{nullptr};
  for (auto L : llvm::concat<Loop *>(Req.loopNest, noCommonLoop)) {
    auto [DepSrcI, DepSrcReqId] =
        getDependencySourceInLoop(Req, L, AllRequests, LoadCanBeSource);
    if (DepSrcI) {
      if (isaStore(DepSrcI)) {
        StoreDepSources.insert(DepSrcReqId);
      } else {
        LoadDepSources.insert(DepSrcReqId);
      }
    }
  }
}

/// Fill the DU info structs. This is the analsysis info used by our Data Unit
/// IP template.
void DynamicLoopFusionAnalysis::collectDUInfos(Function &F, LoopInfo &LI) {
  /// Sorted in topological order below.
  MapVector<int, SmallVector<MemoryRequest, 4>> RequestsForMem, LoadsForMem,
      StoresForMem;
  collectCluesteredRequests(ComputeLoops, RequestsForMem, LoadsForMem,
                            StoresForMem);

  /// Return true if A comes before B in the topological program order.
  auto aPrecedsB = [&RequestsForMem](MemoryRequest &A, MemoryRequest &B,
                                     const int DUID) -> bool {
    for (auto &Req : RequestsForMem[DUID]) {
      if (Req.memOp == A.memOp)
        return true;
      else if (Req.memOp == B.memOp)
        return false;
    }
    return false;
  };

  // init and topological sort of requests
  for (auto [DUID, _] : RequestsForMem) {
    topologicalOrderSort(F, RequestsForMem[DUID]);
    topologicalOrderSort(F, LoadsForMem[DUID]);
    topologicalOrderSort(F, StoresForMem[DUID]);

    DUInfos[DUID] = DUInfo{
        .id = DUID,
        .numLoads = int(LoadsForMem[DUID].size()),
        .numStores = int(StoresForMem[DUID].size()),
        .cType = getCTypeString(LoadsForMem[DUID][0].memOp)};
  }

  for (auto [DUID, LdRequests] : LoadsForMem) {
    for (auto &LdReq : LdRequests) {
      auto LdLoop = LI.getLoopFor(LdReq.memOp->getParent());
      DUInfos[DUID].maxLoopDepth = LdReq.maxLoopDepthInMemoryId;
      DUInfos[DUID].loadLoopDepth.push_back(LdReq.loopDepth);
      DUInfos[DUID].loadisLastIterNeeded.push_back(LdReq.isLastIterNeeded);

      SetVector<int> LoadDependenies, StoreDependenies;
      collectDependencySources(LdReq, RequestsForMem[DUID], LoadDependenies,
                               StoreDependenies);

      SmallVector<int> StoreCommonLoopDepth;
      SmallVector<bool> StoreInSameLoop, StoreInSameThread, LoadPrecStore,
          LoadCheckStore;
      for (auto &StReq : StoresForMem[DUID]) {
        auto StLoop = LI.getLoopFor(StReq.memOp->getParent());
        StoreInSameLoop.push_back(LdLoop == StLoop);
        StoreInSameThread.push_back(LdReq.loopId == StReq.loopId);
        StoreCommonLoopDepth.push_back(getCommonLoopDepth(LdLoop, StLoop));
        LoadPrecStore.push_back(aPrecedsB(LdReq, StReq, DUID));
        LoadCheckStore.push_back(StoreDependenies.contains(StReq.reqId));
      }
      DUInfos[DUID].loadStoreInSameLoop.push_back(StoreInSameLoop);
      DUInfos[DUID].loadStoreInSameThread.push_back(StoreInSameThread);
      DUInfos[DUID].loadStoreCommonLoopDepth.push_back(StoreCommonLoopDepth);
      DUInfos[DUID].loadPrecedsStore.push_back(LoadPrecStore);
      DUInfos[DUID].loadCheckStore.push_back(LoadCheckStore);
    }
  }

  for (auto [DUID, StRequests] : StoresForMem) {
    for (auto StReq : StRequests) {
      auto StReqLoop = LI.getLoopFor(StReq.memOp->getParent());
      DUInfos[DUID].maxLoopDepth = StReq.maxLoopDepthInMemoryId;
      DUInfos[DUID].storeLoopDepth.push_back(StReq.loopDepth);
      DUInfos[DUID].storeisLastIterNeeded.push_back(StReq.isLastIterNeeded);

      SetVector<int> LoadDependenies, StoreDependenies;
      collectDependencySources(StReq, RequestsForMem[DUID], LoadDependenies,
                               StoreDependenies);

      SmallVector<int> StoreCommonLoopDepth;
      SmallVector<bool> StoreInSameLoop, StorePrecedsOtherStore,
          StoreCheckStore, StoreCheckLoad;
      for (auto &OtherStReq : StoresForMem[DUID]) {
        auto OtherStLoop = LI.getLoopFor(OtherStReq.memOp->getParent());
        StoreInSameLoop.push_back(StReqLoop == OtherStLoop);
        StoreCommonLoopDepth.push_back(
            getCommonLoopDepth(StReqLoop, OtherStLoop));
        StorePrecedsOtherStore.push_back(aPrecedsB(StReq, OtherStReq, DUID));
        StoreCheckStore.push_back(StoreDependenies.contains(OtherStReq.reqId));
      }
      for (auto &LdReq : LoadsForMem[DUID]) {
        StoreCheckLoad.push_back(LoadDependenies.contains(LdReq.reqId));
      }

      DUInfos[DUID].storeStoreInSameLoop.push_back(StoreInSameLoop);
      DUInfos[DUID].storeStoreCommonLoopDepth.push_back(StoreCommonLoopDepth);
      DUInfos[DUID].storePrecedsOtherStore.push_back(StorePrecedsOtherStore);
      DUInfos[DUID].storeCheckLoad.push_back(StoreCheckLoad);
      DUInfos[DUID].storeCheckStore.push_back(StoreCheckStore);
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

void addRequestToComputeLoop(MemoryRequest &Req,
                             SmallVector<DecoupledLoopInfo, 4> &ComputeLoops) {
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
    SetVector<Instruction *> LoadedBasePtrInLoop;

    for (auto I : getUniqueLoopInstructions(L)) {
      auto BasePtr = getBasePtrOfInstr(I);

      bool isGlobalPtr =
          BasePtr && (BasePtr->getParent() ==
                      &BasePtr->getParent()->getParent()->getEntryBlock());

      // Only decouple stores that don't have a data hazard in L.
      if (isGlobalPtr || isaLoad(I) || LoadedBasePtrInLoop.contains(BasePtr)) {
        LoadedBasePtrInLoop.insert(BasePtr);
        continue;
      }


      if (!BasePtrsToProtect.contains(BasePtr) && isGlobalPtr && 
          !LoadedBasePtrInLoop.contains(BasePtr)) {
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

} // end namespace llvm
