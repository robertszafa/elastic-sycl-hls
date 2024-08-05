#include "CommonLLVM.h"
#include "DataHazardAnalysis.h"
#include "TableOperationLatency.h"

#include <numeric>

using namespace llvm;

namespace llvm {

SmallVector<Instruction *> DataHazardAnalysis::getAllLoads() {
  SmallVector<Instruction *> lsqLoads;
  for (auto cluster : hazardInstrs)
    for (auto li : llvm::make_filter_range(cluster, isaLoad))
      lsqLoads.push_back(li);

  return lsqLoads;
}

Instruction *DataHazardAnalysis::getLodDataDependencySrc(Instruction *I) {
  // If we have a mem op as I, then get the address instruction
  if (auto addrVal = getLoadStorePointerOperand(I)) 
    I = dyn_cast<Instruction>(addrVal);
  
  // In our implementation, a src of a LoD data dependency can be any load
  // coming from a LSQ. Another implementation could only have a LoD on loads
  // from the same LSQ.
  for (auto li : getAllLoads())
    if (isInDefUsePath(li, I))
      return li;

  return nullptr;
}

BasicBlock *
DataHazardAnalysis::getLodControlDependencySrc(BasicBlock *BB, LoopInfo &LI,
                                               ControlDependenceGraph &CDG) {
  // Check all ctrl dependencies of BB. Stop at inner loop header (if exists).
  auto L = LI.getLoopFor(BB);
  const BasicBlock *LoopHeader = L ? L->getHeader() : nullptr;
  BasicBlock *CurrBB = BB, *Result = nullptr;
  while (CurrBB && CurrBB != LoopHeader && !CurrBB->isEntryBlock()) {
    // Get control depenency.
    if (auto ctrlDepSrcBB = CDG.getControlDependencySource(CurrBB)) {
      // Check if that control dependency depends on values from the CU.
      if (getLodDataDependencySrc(ctrlDepSrcBB->getTerminator()))
        Result = ctrlDepSrcBB;

      // If this control dependency doesn't cause a LoD, there might still be
      // one "hogher up" that does.
      CurrBB = ctrlDepSrcBB;
    } else {
      break;
    }
  }

  return Result;
};

/// Return values for the {instr} key in {addr2InstMap}.
/// Return nullptr if {addr2InstMap} doesn't have the {instr} key.
SetVector<Instruction *> *getClusterForBasePtr(
    DenseMap<Instruction *, SetVector<Instruction *>> &addr2InstMap,
    const Instruction *instr) {
  for (auto &kv : addr2InstMap) {
    if (kv.getFirst()->isIdenticalTo(instr))
    // if (kv.getFirst()->isSameOperationAs(instr))
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
void DataHazardAnalysis::calculateDecoupling(ControlDependenceGraph &CDG,
                                             LoopInfo &LI) {
  auto hasLodDataDep = [&](auto *memOp) {
    return getLodDataDependencySrc(memOp) != nullptr;
  };

  // For each LSQ (cluster), check if its address generation can be decoupled,
  // and if yes, check if any of its address allocations need to be speculated.
  using InstructionSet = SetVector<Instruction *, SmallVector<Instruction *>>;
  MapVector<BasicBlock *, InstructionSet> tmpSpeculationStack;
  for (auto cluster : hazardInstrs) {
    bool canDecoupleCluster =
        !aguDecouplingOff && !llvm::any_of(cluster, hasLodDataDep);

    // If there is no data dependency LoD, then check if speculation is needed.
    SmallVector<BasicBlock *> thisSpecialCtrlDepSrsBlocks;
    if (canDecoupleCluster) {
      for (auto memOp : cluster)  {
        auto specCtrlDepSrc =
            getLodControlDependencySrc(memOp->getParent(), LI, CDG);
        thisSpecialCtrlDepSrsBlocks.push_back(specCtrlDepSrc);

        if (specCtrlDepSrc) {
          if (!tmpSpeculationStack.contains(specCtrlDepSrc))
            tmpSpeculationStack[specCtrlDepSrc] = InstructionSet();
          tmpSpeculationStack[specCtrlDepSrc].insert(memOp);
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
    for (auto [currentSpecCtrlDepSrc, allocStack] : tmpSpeculationStack) {
      // Check if we have already moved everything out of this basic block.
      if (tmpSpeculationStack[currentSpecCtrlDepSrc].empty())
        continue;

      if (auto newSpecCtrlDepSrc =
              getLodControlDependencySrc(currentSpecCtrlDepSrc, LI, CDG)) {
        done = false;

        if (!tmpSpeculationStack.contains(newSpecCtrlDepSrc))
          tmpSpeculationStack[newSpecCtrlDepSrc] = InstructionSet();
        for (auto I : allocStack) {
          tmpSpeculationStack[currentSpecCtrlDepSrc].remove(I);
          tmpSpeculationStack[newSpecCtrlDepSrc].insert(I);
        }
      }
    }
  }

  for (auto [specCtrlDepSrc, allocStack] : tmpSpeculationStack) {
    speculationStack[specCtrlDepSrc] = allocStack.takeVector();
  }
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
  // Go through every specBB that contains speculative allocations.
  for (auto [specBB, allocStack] : speculationStack) {
    const auto L = LI.getLoopFor(specBB);

    // Init
    for (auto alloc : allocStack) 
      poisonLocations[alloc] = SmallVector<PoisonLocation>();

    // Go through speculative allocations in stack order.
    for (auto alloc : allocStack) {
      auto trueBB = alloc->getParent();

      // Check every pred~>succ CFG edge dominated by specBB.
      for (auto EdgeStart : L->blocks()) {
        if (!DT.dominates(specBB, EdgeStart))
          continue;

        for (auto EdgeEnd : successors(EdgeStart)) {
          bool edgeLeadsToTrueBB = (EdgeStart == trueBB || EdgeEnd == trueBB);
          if (edgeLeadsToTrueBB)
            continue;

          // Check if poisonBB for {alloc} on this edge breaks allocStack order.
          bool breaksSpeculationOrder = false;
          for (auto allocI : allocStack) {
            // Go up to current trueBB. 
            if (allocI->getParent() == trueBB)
              break;

            if (isReachableWithinLoop(EdgeEnd, allocI->getParent(), L)) {
              breaksSpeculationOrder = true;
              break;
            }
          }

          // When taking the pred~>suc CFG edge, does trueBB become unreachable?
          bool trueBlockBecomesUnreachable =
              isReachableWithinLoop(EdgeStart, trueBB, L) &&
              !isReachableWithinLoop(EdgeEnd, trueBB, L);

          if (trueBlockBecomesUnreachable && !breaksSpeculationOrder) 
            poisonLocations[alloc].push_back({EdgeStart, EdgeEnd});
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
  aguDecouplingOff = std::getenv("NO_AGU_DECOUPLING") && 
                     strcmp(std::getenv("NO_AGU_DECOUPLING"), "1") == 0;

  // Collect all base addresses that are stored to
  // and that have an uncomputable Scalar Evolution index.
  DenseMap<Instruction *, SetVector<Instruction *>> addr2InstMap;
  for (Loop *TopLevelLoop : LI) {
    for (Loop *L : depth_first(TopLevelLoop)) {
      SmallVector<Instruction *> loopLoads, loopStores;
      for (auto &BB : L->getBlocks()) {
        for (auto &I : *BB) {
          if (isaStore(&I)) 
            loopStores.push_back(&I);
          else if (isaLoad(&I)) 
            loopLoads.push_back(&I);
        }
      }

      for (auto si : loopStores) {
        auto ptrOp = getLoadStorePointerOperand(si);
        auto siPointerSE = SE.getSCEV(ptrOp);
        auto siPointerBase = getPointerBase(ptrOp);

        bool hasSpecialCtrlDep = false;
        if (auto ctrlDepSrcBB = CDG.getControlDependencySource(si)) {
          for (auto li : loopLoads) {
            auto ldPointerBase = getPointerBase(getLoadStorePointerOperand(li));
            if (ldPointerBase->isIdenticalTo(siPointerBase) &&
                isInDefUsePath(li, ctrlDepSrcBB->getTerminator())) {
              hasSpecialCtrlDep = true;
            }
          }
        }

        if (hasSpecialCtrlDep || !isAddressAnalyzable(SE, L, siPointerSE))
          insertInMap(addr2InstMap, siPointerBase, si);
      }
    }
  }

  // Collect all loads and stores to already collected base pointers.
  for (auto &BB : F) {
    for (auto &I : BB) {
      if (I.getType()->isPointerTy())
        continue;

      if (auto pointerOperand = getLoadStorePointerOperand(&I)) {
        auto ptrBase = getPointerBase(pointerOperand);
        if (auto cluster = getClusterForBasePtr(addr2InstMap, ptrBase))
          cluster->insert(&I);
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
  calculateDecoupling(CDG, LI);

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
