#include "CommonLLVM.h"
#include "DataHazardAnalysis.h"
#include "TableOperationLatency.h"
#include "llvm/ADT/BreadthFirstIterator.h"


using namespace llvm;

namespace llvm {

using CFGEdge = std::pair<BasicBlock *, BasicBlock *>;
using BlockPath = SmallVector<BasicBlock *>;
using EdgePath = SmallVector<CFGEdge>;
using InstructionSet = DataHazardAnalysis::InstructionSet;

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

SmallVector<BasicBlock *>
DataHazardAnalysis::getLodCtrlDepSources(BasicBlock *BB, LoopInfo &LI,
                                         ControlDependenceGraph &CDG) {
  SmallVector<BasicBlock *> Result;
  auto L = LI.getLoopFor(BB);
  const BasicBlock *LoopHeader = L ? L->getHeader() : nullptr;
  SetVector<BasicBlock *> Worklist, Done;
  Worklist.insert(BB);
  // Check all ctrl dependencies of BB. Stop at inner loop header (if exists).
  while (!Worklist.empty()) {
    auto CurrBB = Worklist.pop_back_val();
    Done.insert(CurrBB);

    // Get control depenencies.
    for (auto &ctrlDepSrcBB : CDG.getControlDependencySources(CurrBB)) {
      // Check if that control dependency depends on values from the CU.
      if (getLodDataDependencySrc(ctrlDepSrcBB->getTerminator())) {
        Result.push_back(ctrlDepSrcBB);
      } else if (ctrlDepSrcBB != LoopHeader && !Done.contains(ctrlDepSrcBB)) {
        // If this control dependency doesn't cause a LoD, there might still be
        // one "hogher up" that does.
        Worklist.insert(ctrlDepSrcBB);
      }
    }
  }

  return Result;
}

/// Return values for the {instr} key in {addr2InstMap}.
/// Return nullptr if {addr2InstMap} doesn't have the {instr} key.
InstructionSet *
getClusterPtrForBasePtr(MapVector<Instruction *, InstructionSet> &addr2InstMap,
                        const Instruction *instr) {
  for (auto &kv : addr2InstMap) {
    // if (kv.getFirst()->isSameOperationAs(instr))
    if (kv.first->isIdenticalTo(instr))
      return &kv.second;
  }

  return nullptr;
}

/// Insert {instr} into the set associated with the {basePointer} key in
/// {addr2InstMap}. If {addr2InstMap} doesn't have the {basePointer} key, then
/// create it and insert there.
void insertInMap(MapVector<Instruction *, InstructionSet> &addr2InstMap,
                 Instruction *basePointer, Instruction *instr) {
  for (const auto &kv : addr2InstMap) {
    if (kv.first->isIdenticalTo(basePointer)) {
      addr2InstMap[kv.first].insert(instr);
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

/// For each baseAddress (i.e. for each LSQ), decide if the address generation
/// can be decoupled into a seperate thread of execution. 
void DataHazardAnalysis::calculateDecoupling() {
  for (auto cluster : hazardInstrs) {
    bool noMemOpWithLodDataDep = llvm::all_of(cluster, [&](auto *memOp) {
      return getLodDataDependencySrc(memOp) == nullptr;
    });
    bool canDecoupleCluster = !aguDecouplingOff && noMemOpWithLodDataDep;
    decouplingDecisions.push_back(canDecoupleCluster);
  }
}

/// For each baseAddress (i.e. for each LSQ), decide if any of the address 
/// generation instructions need to be speculated to avoid loss of decoupling.
void DataHazardAnalysis::calculateSpeculation(ControlDependenceGraph &CDG,
                                              LoopInfo &LI) {
  for (size_t i = 0; i < hazardInstrs.size(); ++i) {
    if (!decouplingDecisions[i]) {
      // No need for speculation if AGU is not decoupled.
      speculationDecisions.push_back(false);
      continue;
    }

    SmallVector<Instruction *> cluster = hazardInstrs[i];
    bool needsSpeculation = llvm::any_of(cluster, [&](auto *memOp) {
      return !getLodCtrlDepSources(memOp, LI, CDG).empty();
    });
    speculationDecisions.push_back(needsSpeculation);
  }
}

/// Iteratively hoist memory operations to their LoD control dependency source block.
/// At the end, we will have a requestMap with {block: instructions} pairs, 
/// where the {block} is the location where the {instructions} were hoisted.
void DataHazardAnalysis::hoistSpeculativeRequests(Function &F,
                                                  ControlDependenceGraph &CDG,
                                                  LoopInfo &LI) {
  // Init request map, i.e. at the start each instruction is in its original BB.
  // Perform hoisting in topological program order.
  for (auto BB : getTopologicalOrder(F))
    requestMap[BB] = InstructionSet();

  for (size_t i = 0; i < hazardInstrs.size(); ++i) {
    if (speculationDecisions[i]) {
      for (auto &I : hazardInstrs[i]) {
        // A mem op is added to the map only if it has a LoD ctrl dep.
        if (getLodCtrlDepSources(I->getParent(), LI, CDG).size() > 0)
          requestMap[I->getParent()].insert(I);
      }
    }
  }

  // Iterative hositing, Algorithm 1 from the paper. 
  bool done = false;
  while (!done) {
    done = true;

    for (auto &[fromBB, requests] : requestMap) {
      // Instead of deleting keys in requestMap, skip keys without any requests.
      if (requests.empty())
        continue;

      for (auto &toBB : getLodCtrlDepSources(fromBB, LI, CDG)) {
        // Do not hoist across loops.
        if (LI.getLoopFor(fromBB) != LI.getLoopFor(toBB))
          continue;
        
        done = false;
        for (auto &r : requests) {
          // hoist fromBB -> toBB
          requestMap[toBB].insert(r);
          requestMap[fromBB].remove(r);
        }
      }
    }
  }

  // Clean up: delete empty requestMap entries; remove non-speculated mem ops.
  SmallVector<BasicBlock *> keysToDelete;
  for (auto &[specBB, requests] : requestMap) {
    // A requests whose specBB is the same as the original BB is not speculated.
    for (auto &r : requests)
      if (r->getParent() == specBB)
        requestMap[specBB].remove(r);

    if (requestMap[specBB].empty())
      keysToDelete.push_back(specBB);
  }
  for (auto k : keysToDelete)
    requestMap.erase(k);
}

/// Given a block {BB}, return all possible paths from {BB} to the loop latch
/// (or function exit if BB not in loop).
SmallVector<BlockPath> getAllBlockPathsInLoop(BasicBlock *BB, LoopInfo &LI) {
  Loop *L = LI.getLoopFor(BB);
  BasicBlock *StopAt = L ? L->getLoopLatch() : getReturnBlock(*BB->getParent()); 

  SmallVector<BlockPath> AllPaths;
  std::queue<BlockPath> Queue;
  Queue.push({BB});

  // BFS traversal.
  while (!Queue.empty()) {
    BlockPath CurrPath = Queue.front();
    Queue.pop();

    BasicBlock *PathFrontier = CurrPath.back();

    if (PathFrontier == StopAt) {
      AllPaths.push_back(CurrPath);
      continue; 
    }

    for (auto SuccBB : successors(PathFrontier)) {
      if (!LI.isLoopHeader(SuccBB)) {
        BlockPath Continuation {CurrPath};
        Continuation.push_back(SuccBB);
        Queue.push(Continuation);
      }
    }
  }

  return AllPaths;
}

/// Given a vector of blocks where neighbours are successors in the CFG, return 
/// a vector of CFG edges. 
SmallVector<EdgePath> blockToEdgePath(SmallVector<BlockPath> &AllBlockPaths) {
  SmallVector<EdgePath> AllEdgePaths;

  for (auto &CurrBlockPath : AllBlockPaths) {
    EdgePath CurrEdgePath;
    BasicBlock *CurrBB = nullptr;
    for (auto &BB : CurrBlockPath) {
      if (CurrBB)
        CurrEdgePath.push_back({CurrBB, BB});
      CurrBB = BB;
    }
    
    AllEdgePaths.push_back(CurrEdgePath);
  }

  return AllEdgePaths;
}

/// Algorithm 2 from the paper. 
void DataHazardAnalysis::calculatePoisonBlocks(Function &F, DominatorTree &DT,
                                               LoopInfo &LI) {
  for (auto &[specBB, requests] : requestMap) {
    auto L = LI.getLoopFor(specBB);
    auto AllBlockPaths = getAllBlockPathsInLoop(specBB, LI);
    auto AllEdgePaths = blockToEdgePath(AllBlockPaths);    

    for (size_t iPath = 0; iPath < AllEdgePaths.size(); ++iPath) {
      auto EdgePath = AllEdgePaths[iPath];
      auto BlockPath = AllBlockPaths[iPath];

      // Map of {trueBlock: requests originally in trueBlock}
      MapVector<BasicBlock *, InstructionSet> requestsForPath;
      for (auto &r : requests) {
        if (!requestsForPath.contains(r->getParent()))
          requestsForPath[r->getParent()] = InstructionSet();
        requestsForPath[r->getParent()].insert(r);
      }

      for (auto &[EdgeSrc, EdgeDst] : EdgePath) {
        for (auto &[trueBB, requests] : requestsForPath) {
          if (requests.empty())
            continue;

          if (EdgeDst == trueBB) {
            requestsForPath[trueBB].clear();
            break; // to next edge
          }

          if (!isReachableWithinLoop(EdgeDst, trueBB, L)) {
            for (auto r : requests)
              poisonLocations[r].insert({EdgeSrc, EdgeDst});
            requestsForPath[trueBB].clear();
          }
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
  MapVector<Instruction *, InstructionSet> addr2InstMap;
  for (Loop *TopLevelLoop : LI) {
    for (Loop *L : breadth_first(TopLevelLoop)) {
      SmallVector<Instruction *> loopLoads, loopStores;
      for (auto BB : breadth_first(L->getHeader())) {
        if (!L->contains(BB))
          continue;

        for (auto &I : *BB) {
          if (isaStore(&I)) 
            loopStores.push_back(&I);
          else if (isaLoad(&I)) 
            loopLoads.push_back(&I);
        }
      }

      for (auto &si : loopStores) {
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
        if (auto cluster = getClusterPtrForBasePtr(addr2InstMap, ptrBase))
          cluster->insert(&I);
      }
    }
  }

  // Take only memory instructions. Remove clusters with only stores (no RAW).
  for (auto &kv : addr2InstMap) {
    if (!llvm::all_of(kv.second, isaStore))
      hazardInstrs.push_back(kv.second.takeVector());
  }

  // In a real implementation, these could all be done in a single pass, without
  // intermediate storage. We factor the steps out into functions for clarity.
  calculateDecoupling();
  calculateSpeculation(CDG, LI);
  hoistSpeculativeRequests(F, CDG, LI);
  calculatePoisonBlocks(F, DT, LI);

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
