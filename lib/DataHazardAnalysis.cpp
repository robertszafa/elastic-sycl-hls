#include "CommonLLVM.h"
#include "DataHazardAnalysis.h"
#include "CDG.h"

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

/// TODO: use the function defined in CDG.cpp
BasicBlock *getControlDependencySource(Function &F, ControlDependenceGraph &CDG,
                                       Instruction *I) {
  // If there is an edge from the CDG root to the parent of {interIterDep},
  // then {interIterDep} is not control dependent.
  auto cdgNode = CDG.getBlockNode(I->getParent());
  if (CDG.getRoot()->hasEdgeTo(*cdgNode))
    return nullptr;

  // interIterDep is control dependent on some block. Find out the source block.
  for (auto &N : CDG) {
    // We already checked the root.
    if (isa<RootCDGNode>(N))
      continue;

    SmallVector<CDGEdge *, 2> edgesToNode;
    N->findEdgesTo(*cdgNode, edgesToNode);
    if (edgesToNode.size() > 0)
      return dyn_cast<BlockCDGNode>(N)->getBasicBlock();
  }

  return nullptr;
}

/// Return true if the address generation instructions for the loads and stores
/// in {cluster} can be decoupled into it's own function, such that the new
/// function doesn't contain any of the load or store instructions.
///
/// There are two cases where decoupling is not possible.
///
/// Case 1: The store instruction is control dependent, and the terminator
/// instruction def-use chain path contains the load value. For example:
///   for i:N
///       x = data[idx[i]]
///       if (x > 0) data[idx[i]] = f(x)
///
/// Case 2: The store address value depends on the the value returned
/// by a load. Is the load value needed to generate a store address?
/// This could be a data dependency:
///   for i:N
///       x = data[idx[i]]
///       data[g(x)] = f(x)
/// Or a transcient control-dependency:
///   for i:N
///       thisIdx = idx[i]
///       x = data[thisIdx]
///       if (x > 0) thisIdx += 1
///       data[thisIdx] = f(x)
bool canDecoupleAddressGen(Function &F, ControlDependenceGraph &CDG,
                           SmallVector<Instruction *> &cluster) {
  // Check all ld/st pairs. If any of address cannot be decoupled, return false.
  for (auto si : llvm::make_filter_range(cluster, isaStore)) {
    for (auto li : llvm::make_filter_range(cluster, isaLoad)) {
      // Case 1
      if (auto ctrlDepSrcBB = getControlDependencySource(F, CDG, si)) {
        if (isInDefUsePath(li, ctrlDepSrcBB->getTerminator()))
          return false;
      }

      // Case 2
      auto storeAddressI =
          dyn_cast<Instruction>(dyn_cast<StoreInst>(si)->getOperand(1));
      if (isInDefUsePath(li, storeAddressI))
        return false;
    }
  }

  return true;
}

/// This function has the same functionality as ScalarEvolution.getPointerBase,
/// but it returns the Instruction representing the pointer base, not a SCEV.
/// It relies on our previous pass that puts all GEPs (that get global memory
/// pointers) into the F.entry block.
Instruction *getPointerBase(Value *pointerOperand) {
  const BasicBlock *entryBlockF = &dyn_cast<Instruction>(pointerOperand)
                                       ->getParent()
                                       ->getParent()
                                       ->getEntryBlock();
  if (dyn_cast<Instruction>(pointerOperand)->getParent() == entryBlockF) {
    // stop;
  } else if (auto cast = dyn_cast<BitCastInst>(pointerOperand)) {
    return getPointerBase(dyn_cast<Instruction>(cast->getOperand(0)));
  } else if (auto load = dyn_cast<LoadInst>(pointerOperand)) {
    return getPointerBase(dyn_cast<Instruction>(load->getOperand(0)));
  } else if (auto gep = dyn_cast<GetElementPtrInst>(pointerOperand)) {
    if (gep->getPointerOperand()) // hasAllConstantIndices())
      return getPointerBase(dyn_cast<Instruction>(gep->getPointerOperand()));
  }

  return dyn_cast<Instruction>(pointerOperand);
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

DataHazardAnalysis::DataHazardAnalysis(Function &F, LoopInfo &LI,
                                       ScalarEvolution &SE,
                                       PostDominatorTree &PDT) {
  // Collect all base addresses that are stored to
  // and that have an uncomputable Scalar Evolution index.
  DenseMap<Instruction *, SetVector<Instruction *>> addr2InstMap;
  SetVector<Instruction *> discardedStores;
  for (Loop *TopLevelLoop : LI) {
    for (Loop *L : depth_first(TopLevelLoop)) {
      for (auto &BB : L->getBlocks()) {
        for (auto &I : *BB) {
          if (auto si = dyn_cast<StoreInst>(&I)) {
            auto siPointerSE = SE.getSCEV(si->getPointerOperand());

            if (!isAddressAnalyzable(SE, L, siPointerSE)) {
              auto siPointerBase = getPointerBase(si->getPointerOperand());
              insertInMap(addr2InstMap, siPointerBase, &I);
            } else {
              discardedStores.insert(si);
            }
          }
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

  // For each cluster, check if address generation can be decoupled from access.
  auto CDG = new ControlDependenceGraph(F, PDT);
  for (auto cluster : hazardInstrs) 
    decouplingDecisions.push_back(canDecoupleAddressGen(F, *CDG, cluster));
}

} // namespace llvm
