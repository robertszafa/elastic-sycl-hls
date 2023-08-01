#include "CommonLLVM.h"
#include "DataHazardAnalysis.h"
#include "TableOperationLatency.h"

#include <numeric>

using namespace llvm;

namespace llvm {

/// Return true, if there is a possibility that {src} could be executed on any
/// of the def-use path leading to {dst}.
bool isInDefUsePath(Instruction *src, Instruction *dst) {
  SmallVector<Instruction *> workList, doneList;
  workList.push_back(dst);

  // Work up the def-use chain of {dst} until hitting {src} or arriving the the
  // F.entry. Special treatment of phi nodes: add terminating instructions of
  // the incoming basic blocks to the worklist (if {src} is in the def-use chain
  // of the terminator, then there is a possibility that it will be executed on
  // the path to {dst}.
  while (!workList.empty()) {
    auto currDst = workList.pop_back_val();
    doneList.push_back(currDst);

    // Case 1: There is a direct data dependency, so this def-use chain exists: 
    // F.entry ~> .. ~> {src} .. ~> {dst}
    for (auto &Use : currDst->operands()) {
      if (auto UseI = dyn_cast<Instruction>(Use.get())) {
        if (UseI == src)
          return true;

        if (!llvm::is_contained(doneList, UseI))
          workList.push_back(UseI);
      }
    }

    // Case 2: Some value in {dst} def-use chain is a phi node and the
    // terminator of the incoming basic block has {src} in this def-use chain.
    if (auto dstPhi = dyn_cast<PHINode>(currDst)) {
      for (auto incomingBB : dstPhi->blocks()) {
        if (!llvm::is_contained(doneList, incomingBB->getTerminator()))
          workList.push_back(incomingBB->getTerminator());
      }
    }
  }

  return false;
}

/// Return values for the {instr} key in {addr2InstMap}.
/// Return nullptr if {addr2InstMap} doesn't have the {instr} key.
SetVector<Instruction *> *getEquivalentInMap(
    DenseMap<Instruction *, SetVector<Instruction *>> &addr2InstMap,
    const Instruction *instr) {
  for (auto &kv : addr2InstMap) {
    if (kv.getFirst()->isIdenticalTo(instr))
      return &kv.getSecond();
  }

  return nullptr;
}

/// Insert {instr} into the set associated with the {basePointer} key in
/// {addr2InstMap}. If {addr2InstMap} doesn't have the {basePointer} key, then
/// create it and insert there.
void insertInMap(
    DenseMap<Instruction *, SetVector<Instruction *>> &addr2InstMap,
    Instruction *basePointer, Instruction *instr) {
  for (const auto &kv : addr2InstMap) {
    if (kv.getFirst()->isIdenticalTo(basePointer)) {
      addr2InstMap[kv.getFirst()].insert(instr);
      return;
    }
  }

  addr2InstMap[basePointer].insert(instr);
}

/// Given a SCEV representing a memory address as a recurrence function,
/// decide if the function is analyzable, i.e. can we predict
/// that {address} will never have the same value on two different iterations?
bool isAddressAnalyzable(ScalarEvolution &SE, const Loop *L,
                         const SCEV *address) {
  if (SE.hasComputableLoopEvolution(address, L))
    return true;

  auto idxSE = SE.removePointerBase(address);
  auto idxRange = SE.getUnsignedRange(idxSE);

  return idxRange.isAllNonNegative();
}

/// Sort clusters of instructions based on the postdominance relation
/// of the instructions producing the base address. We don't care about the
/// actual sorting relation, as long as it gives some ordering.
SmallVector<SmallVector<Instruction *>> getSortedVectorClusters(
    const DenseMap<Instruction *, SetVector<Instruction *>> &clusterMap,
    PostDominatorTree &PDT) {
  // Break up map into two corresponding vectors.
  SmallVector<Instruction *> baseAddrI;
  SmallVector<SmallVector<Instruction *>> iClusters;
  for (auto &kv : clusterMap) {
    baseAddrI.push_back(kv.getFirst());
    SmallVector<Instruction *> newCluster;
    llvm::copy(kv.getSecond(), std::back_inserter(newCluster));
    iClusters.push_back(newCluster);
  }

  // Use base address instruction dominance relation
  // to sort the collected instructions.
  SmallVector<int> sortingIndices(iClusters.size());
  std::iota(sortingIndices.begin(), sortingIndices.end(), 0);
  llvm::sort(sortingIndices, [&](int a, int b) {
    return PDT.dominates(baseAddrI[b], baseAddrI[a]);
  });

  // Create a new vector for the sorted instructions.
  SmallVector<SmallVector<Instruction *>> iClustersSorted(iClusters.size());
  for (uint i = 0; i < sortingIndices.size(); ++i)
    iClustersSorted[i] = iClusters[sortingIndices[i]];

  return iClustersSorted;
}

/// Given a pointer to memory, return its allocated number of elements if 
/// this is known, otherwise return 0. 
int getTargetMemorySize(Instruction *basePointer) {
  if (auto addrCastI = dyn_cast<AddrSpaceCastInst>(basePointer)) {
    if (auto allocI = dyn_cast<AllocaInst>(addrCastI->getPointerOperand())) {
      if (auto allocaType = allocI->getAllocatedType()) 
        return allocaType->getArrayNumElements();
    }
  }

  return 0;
}

/// For each baseAddress (i.e. for each LSQ), decide if the address generation
/// can be decoupled, and check address allocations need to be speculated in
/// order to achieve decoupling.
/// Decoupling is not possible if the address of a memOp depends on another
/// memOp from the same base address.
void DataHazardAnalysis::calculateDecoupling(ControlDependenceGraph &CDG) {
  // Collect all loads that will be routed through an LSQ.
  SmallVector<Instruction *> lsqLoads;
  for (auto cluster : hazardInstrs) 
    for (auto li : llvm::make_filter_range(cluster, isaLoad)) 
      lsqLoads.push_back(li);

  // Given a ld/st, return false if its address depends on any lsq load.
  auto canDecouple = [&lsqLoads] (Instruction *memOp) -> bool {
    auto addrVal = isaLoad(memOp) ? dyn_cast<LoadInst>(memOp)->getOperand(0)
                                  : dyn_cast<StoreInst>(memOp)->getOperand(1);
    for (auto li : lsqLoads) {
      if (isInDefUsePath(li, dyn_cast<Instruction>(addrVal)))
        return false;
    }
    return true;
  };

  // Given a BB, return its special control dep. src block, if it exists.
  auto getSpecialCtrlDepSrc = [&lsqLoads, &CDG] (BasicBlock *BB) -> BasicBlock * {
    if (auto ctrlDepSrcBB = CDG.getControlDependencySource(BB)) {
      for (auto li : lsqLoads) {
        if (isInDefUsePath(li, ctrlDepSrcBB->getTerminator())) {
          return ctrlDepSrcBB;
        }
      }
    }

    return nullptr;
  };

  // For each LSQ (cluster), check if its address generation can be decoupled,
  // and if yes, check if any of its address allocations need to be speculated.
  for (auto cluster : hazardInstrs) {
    bool canDecoupleCluster = llvm::all_of(cluster, canDecouple);

    SmallVector<BasicBlock *> thisSpecialCtrlDepSrsBlocks;
    if (canDecoupleCluster) {
      for (auto memOp : cluster)  {
        auto specCtrlDepSrc = getSpecialCtrlDepSrc(memOp->getParent());
        thisSpecialCtrlDepSrsBlocks.push_back(specCtrlDepSrc);

        if (specCtrlDepSrc) {
          if (!speculationStack.contains(specCtrlDepSrc))
            speculationStack[specCtrlDepSrc] = SmallVector<Instruction *>();
          speculationStack[specCtrlDepSrc].push_back(memOp);
        }
      }
    }
    bool isAnySpeculation = llvm::any_of(thisSpecialCtrlDepSrsBlocks,
                                         [](auto BB) { return BB != nullptr; });

    // Can the addresses for this LSQ be decoupled?
    decouplingDecisions.push_back(canDecoupleCluster);
    // If decoupled, do any of the addresses have to be speculated?
    speculationDecisions.push_back(isAnySpeculation);
    // If any speculation, record the special ctrl dep src block for each alloc.
    specialCtrlDepSrsBlocks.push_back(thisSpecialCtrlDepSrsBlocks);
  }

  // Now iteratively move speculations to a block that doesn't have a special
  // control dependency.
  bool done = false;
  while (!done) {
    done = true;
    MapVector<BasicBlock *, SmallVector<Instruction *>> newSpeculationStack;

    for (auto [specBB, allocStack] : speculationStack) {
      if (auto ctrlDepSrcBB = getSpecialCtrlDepSrc(specBB)) {
        if (!newSpeculationStack.contains(ctrlDepSrcBB))
          newSpeculationStack[ctrlDepSrcBB] = SmallVector<Instruction *>();
        llvm::append_range(newSpeculationStack[ctrlDepSrcBB], allocStack);
        done = false;
      }
    }

    if (!done)
      speculationStack = newSpeculationStack;
  }
}

/// Does {BB} exist on the {PathStart} ~~> {PathEnd} CFG path?
bool existsOnPath(BasicBlock *BB, BasicBlock *PathStart, BasicBlock *PathEnd) {
  SetVector<BasicBlock*> done;
  SmallVector<BasicBlock *> stack = {PathStart};

  while (!stack.empty()) {
    auto curr = stack.pop_back_val();
    done.insert(curr);
    if (curr == BB)
      return true;
    
    if (curr == PathEnd)
      continue;

    for (auto succBB : successors(curr)) {
      if (!done.contains(succBB))
        stack.push_back(succBB);
    }
  }

  return false;
}

/// Given {memOps} which will be routed through an LSQ, return the required
/// number of out-of-order store addresses that the LSQ needs to hold in order 
/// to achieve perfect pipelining in the case of no true data hazards.
/// This equates to max(loadToStoreDelay * numStoresBeforeLoad).
int getStoreQueueSize(SmallVector<Instruction *> &memOps, LoopInfo &LI,
                      ScalarEvolution &SE, DataDependenceGraph &DDG,
                      bool hasSpeculation) {

  // Lambda that returns the number of iterations in an unrolled loop {L}.
  auto getMinimumUnrolledLoopLatency = [&SE](Loop *L) -> int {
    if (auto loopItersSCEV = SE.getBackedgeTakenCount(L)) {
      std::string loopIterStr;
      llvm::raw_string_ostream rso(loopIterStr);
      loopItersSCEV->print(rso);
      return std::stoi(loopIterStr);
    }
    return 1;
  };

  // Mapping of loops to the number of LSQ ld/st in them.
  MapVector<Loop *, int> storesInLoop, loadsInLoop;
  int maxNumStInLoop = 1, maxNumLdInLoop = 1;
  for (auto si : llvm::make_filter_range(memOps, isaStore)) {
    auto L = LI.getLoopFor(si->getParent());
    if (storesInLoop.contains(L))
      storesInLoop[L]++;
    else
      storesInLoop[L] = 1;
    
    maxNumStInLoop = std::max(maxNumStInLoop, storesInLoop[L]);
  }
  for (auto li : llvm::make_filter_range(memOps, isaLoad)) {
    auto L = LI.getLoopFor(li->getParent());
    if (loadsInLoop.contains(L))
      loadsInLoop[L]++;
    else
      loadsInLoop[L] = 1;

    maxNumLdInLoop = std::max(maxNumLdInLoop, loadsInLoop[L]);
  }

  // Start with min size.
  int size = 4;
  for (auto nodeDDG : DDG) {
    // Now find max st to ld delay in a RAW data dep. Start with an 
    // overapproximation (not all ld values have to be part of the RAW dep)
    int delay = 2 + maxNumLdInLoop;
    SetVector<Loop *> unrolledLoopsCounted;
    if (auto loopSCC = dyn_cast<PiBlockDDGNode>(nodeDDG)) {
      for (auto N : loopSCC->getNodes()) {
        if (auto simpleN = dyn_cast<SimpleDDGNode>(N)) {
          for (auto I : simpleN->getInstructions()) {
            auto instrLat = LATENCY_TABLE_ARRIA10.lookup(I->getOpcode());

            // If the instruction is part of an unrolled loop, then add the
            // latency of the loop instead of the instruction (treat all
            // instructions in a loop as a single op). To avoid double counting,
            // keep a {unrolledLoopsCounted} set. The latency of nested unrolled 
            // loops gets multiplied, but again only once.
            if (auto L = LI.getLoopFor(I->getParent())) {
              if (isLoopUnrolled(L) && !unrolledLoopsCounted.contains(L)) {
                // Approx. loop latency with its number of iters. + overhead. 
                auto loopLat = 1 + getMinimumUnrolledLoopLatency(L);
                for (auto otherUnrolledL : unrolledLoopsCounted) {
                  if (llvm::is_contained(L->getSubLoops(), otherUnrolledL) ||
                      llvm::is_contained(otherUnrolledL->getSubLoops(), L)) {
                    loopLat *= getMinimumUnrolledLoopLatency(otherUnrolledL);
                  }
                }
                instrLat = loopLat;
                unrolledLoopsCounted.insert(L);
              }
            }

            delay += instrLat;
          }
        }

      }
    }

    delay *= maxNumStInLoop;  
    size = std::max(size, delay);
  }

  // Adding Poison blocks needed for speculation can increase the required 
  // store queue size if the original ld->st delay is small.
  if (hasSpeculation)
    size += 4;
  
  // Rounding up to the next power-of-2 is only beneficial for small SQ sizes.
  if (size < 32) {
    int nextPower = 1;
    while(nextPower < size) nextPower *= 2;
    size = nextPower;
  }

  return size;
}

/// Given the {speculationStack}, calculate where in the main CFG should given
/// speculations be poisoned. I.e. at which point does the mem op for a given 
/// speeculative address allocation become unreachable?
/// Record the first {predBB} --> {succBB} edge where this is true.
void DataHazardAnalysis::calculatePoisonBlocks(DominatorTree &DT,
                                               LoopInfo &LI) {
  for (auto [specBB, allocStack] : speculationStack) {
    auto L = LI.getLoopFor(specBB);
    auto loopLatch = L->getLoopLatch();

    // Init
    for (auto alloc : allocStack) 
      poisonLocations[alloc] = SmallVector<PoisonLocation>();

    for (auto predBB : L->blocks()) {
      if (L->isLoopLatch(predBB) || !DT.dominates(specBB, predBB)) 
        continue;

      for (auto succBB : successors(predBB)) {
        for (auto alloc : allocStack) {

          auto allocBB = alloc->getParent();
          if (allocBB == predBB)
            continue;

          // Cond 1
          bool usedOnPath = existsOnPath(allocBB, specBB, predBB);
          bool poisonedOnPath = false;
          for (auto poisonLoc : poisonLocations[alloc]) {
            if (existsOnPath(poisonLoc.second, specBB, predBB))
              poisonedOnPath = true;
          }
          // Cond 2
          bool isAllocStillPossible = existsOnPath(allocBB, succBB, loopLatch);

          if (!usedOnPath && !poisonedOnPath && !isAllocStillPossible) 
            poisonLocations[alloc].push_back({predBB, succBB});
        }
      }
    }
  }
}

DataHazardAnalysis::DataHazardAnalysis(Function &F, LoopInfo &LI,
                                       ScalarEvolution &SE,
                                       DominatorTree &DT,
                                       PostDominatorTree &PDT,
                                       DataDependenceGraph &DDG,
                                       ControlDependenceGraph &CDG) {
  // Collect all base addresses that are stored to
  // and that have an uncomputable Scalar Evolution index.
  DenseMap<Instruction *, SetVector<Instruction *>> addr2InstMap;
  SetVector<Instruction *> discardedStores;
  for (Loop *TopLevelLoop : LI) {
    for (Loop *L : depth_first(TopLevelLoop)) {
      SmallVector<Instruction *> loads, stores;
      for (auto &BB : L->getBlocks()) {
        for (auto &I : *BB) {
          if (isaStore(&I)) 
            stores.push_back(&I);
          else if (isaLoad(&I)) 
            loads.push_back(&I);
        }
      }

      for (auto si : stores) {
        auto ptrOp = dyn_cast<StoreInst>(si)->getPointerOperand();
        auto siPointerSE = SE.getSCEV(ptrOp);
        auto siPointerBase = getPointerBase(ptrOp);

        bool hasSpecialCtrlDep = false;
        if (auto ctrlDepSrcBB = CDG.getControlDependencySource(si)) {
          for (auto li : loads) {
            auto ldPointerBase =
                getPointerBase(dyn_cast<LoadInst>(li)->getPointerOperand());
            if (ldPointerBase->isIdenticalTo(siPointerBase) &&
                isInDefUsePath(li, ctrlDepSrcBB->getTerminator())) {
              hasSpecialCtrlDep = true;
            }
          }
        }

        if (hasSpecialCtrlDep || !isAddressAnalyzable(SE, L, siPointerSE)) {
          insertInMap(addr2InstMap, siPointerBase, si);
        } else {
          discardedStores.insert(si);
        }
      }
    }
  }

  // Collect all stores previously discarded, if there was a different
  // unpredictable store to the same base address.
  for (auto &I : discardedStores) {
    auto ptrBase = getPointerBase(dyn_cast<StoreInst>(I)->getPointerOperand());
    if (auto bucketToInsert = getEquivalentInMap(addr2InstMap, ptrBase)) 
      bucketToInsert->insert(I);
  }

  // Collect loads seperately, once all unpredictable stores are known.
  for (Loop *TopLevelLoop : LI) {
    for (Loop *L : depth_first(TopLevelLoop)) {
      for (auto &BB : L->getBlocks()) {
        for (auto &I : *BB) {
          if (auto li = dyn_cast<LoadInst>(&I)) {
            // Ignore loads of pointers.
            if (li->getType()->isPointerTy())
              continue;

            auto pointerBaseInstr = getPointerBase(li->getPointerOperand());
            // We are only intersted in loads where there 
            // is a store to the the same base address.
            if (auto bucketToInsert =
                    getEquivalentInMap(addr2InstMap, pointerBaseInstr)) {
              bucketToInsert->insert(&I);
            }
          }
        }
      }
    }
  }

  // Remove clusters that only have stores (no RAW hazard).
  llvm::remove_if(addr2InstMap, [](auto kv) {
    return llvm::all_of(kv.getSecond(), isaStore);
  });

  // Take only the mem instrs and ensure the order is deterministic.
  hazardInstrs = getSortedVectorClusters(addr2InstMap, PDT);

  // Check if address generation can be decoupled from execution. Also check if
  // addresses need to be speculatively generated to achieve decoupling.
  calculateDecoupling(CDG);

  calculatePoisonBlocks(DT, LI);

  // For each cluster: 
  // - check if memory is on- or off-chip (on-chip memory has const size), 
  // - collect size of eventual on-chip memory,
  // - calculate the required store queue size for ideal throughput.
  for (size_t iLSQ = 0; iLSQ < hazardInstrs.size(); ++ iLSQ) {
    auto cluster = hazardInstrs[iLSQ];
    // First I in cluster is always a store, with the address in operand 1.
    auto basePtr = getPointerBase(cluster[0]->getOperand(1));
    auto memSize = getTargetMemorySize(basePtr);

    storeQueueSizes.push_back(
        getStoreQueueSize(cluster, LI, SE, DDG, speculationDecisions[iLSQ]));
    memorySizes.push_back(memSize);
    isOnChip.push_back(memSize > 0);
    baseAddresses.push_back(basePtr);
  }
}

} // namespace llvm
