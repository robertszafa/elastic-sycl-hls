#include "CommonLLVM.h"
#include "DataHazardAnalysis.h"

#include <numeric>
#include <sstream>
#include <string>


using namespace llvm;

namespace llvm {

// Returns true if instA is control dependent (may_occur && !must_occur),
// where the control dependency depends on {instB}.
//
// The motivation behind this check is to see, if the generation of store
// addresses can be hosited into it's own kernel, without using the base address
// of the {st} instruction.
//
// Go over all users of {onI} recursively:
//  - if we hit a branch:
//      - if the store is in any but not all of the BB dominated by the branch,
//      return true
// Record pairs into {checkedPairs} to prune the recursive search.
bool isAConditionalOnB(
    DominatorTree &DT, Instruction *instA, Instruction *instB,
    SmallVector<SmallVector<Instruction *, 2>> &checkedPairs) {
  SmallVector<Instruction *, 2> thisPair(2);
  thisPair[0] = instA;
  thisPair[1] = instB;
  if (llvm::find(checkedPairs, thisPair) != checkedPairs.end())
    return false;

  checkedPairs.emplace_back(thisPair);

  for (auto user : instB->users()) {
    if (!isa<Instruction>(user))
      continue;

    auto userI = dyn_cast<Instruction>(user);
    if (auto brI = dyn_cast<BranchInst>(userI)) {
      bool atLeastOneDominatesStore = false;
      bool allDominateStore = true;
      for (auto succ : brI->successors()) {
        allDominateStore &= DT.dominates(succ, instA->getParent());
        atLeastOneDominatesStore |= DT.dominates(succ, instA->getParent());
      }

      if (atLeastOneDominatesStore && !allDominateStore)
        return true;
    }

    return isAConditionalOnB(DT, instA, userI, checkedPairs);
  }

  return false;
}

bool isAInUsersOfB(Instruction *A, Instruction *B) {
  for (auto user : B->users()) {
    if (dyn_cast<Instruction>(user) == A)
      return true;
  }

  return false;
}

SmallVector<Instruction *>
getInstructionsUsedByI(Function &F, DominatorTree &DT, Instruction *I) {
  SmallVector<Instruction *> result;
  if (!I)
    return result;

  for (auto &BB : F) {
    if (DT.properlyDominates(I->getParent(), &BB))
      continue;

    for (auto &bbI : BB) {
      if (isAInUsersOfB(I, &bbI))
        result.push_back(&bbI);
    }
  }

  return result;
}

bool isAnyStoreControlDepOnAnyLoad(
    DominatorTree &DT, const SmallVector<Instruction *> &memInstrs) {
  for (auto si : memInstrs) { // For all stores
    if (!isaStore(si))
      continue;
    for (auto li : memInstrs) { // For all loads
      if (!isaLoad(li))
        continue;

      SmallVector<SmallVector<Instruction *, 2>> checkedPairs;
      if (isAConditionalOnB(DT, si, li, checkedPairs))
        return true;
    }
  }

  return false;
}

/// Given load and store instructions, decide if the instructions that generate
/// addresses for the loads and stores can be decoupled from the rest of the
/// instructions in function {F}. If ANY of the address generation instructions
/// cannot be decoupled, return false.
///
/// Algorithm for making the decision:
/// 1. If any of the stores is control dependent on any of the loads, return
///    false. (This is because we cannot know if a given store address should be
///    produced, without looking at the load value).
/// 2. If any of the values producing a given address is needed in a basic block
///    properly dominated by the last mem. instruction in the
///    {F}.entry->{F}.exit path, return false.
bool canDecoupleAddressFromAccess(Function &F, DominatorTree &DT,
                                  SmallVector<Instruction *> memInstrs) {
  // All BBs that contain an instruction \in {memInstr} are collected.
  // Also all address generation instructions for the memInstrs are
  // collected, including parent nodes in the DDG.
  SetVector<BasicBlock *> allMemInstrBBs;
  SetVector<Instruction *> allAddressIntrs;
  for (auto I : memInstrs) {
    allMemInstrBBs.insert(I->getParent());
    auto addressI =
        isaStore(I)
            ? dyn_cast<Instruction>(dyn_cast<Instruction>(I->getOperand(1)))
            : dyn_cast<Instruction>(dyn_cast<Instruction>(I->getOperand(0)));
    allAddressIntrs.insert(addressI);
    auto usedByLi = getInstructionsUsedByI(F, DT, addressI);
    allAddressIntrs.insert(addressI);
    for (auto &I : usedByLi)
      allAddressIntrs.insert(I);
  }

  // Get the last basic block in the function.entry->function.exit CFG path
  // from {allBBs} in the function dominator tree, i.e. the BB which
  // is not properly dominated by any other BB from {allBBs}.
  BasicBlock *lastBB = nullptr;
  for (auto candidateBB : allMemInstrBBs) {
    if (!llvm::any_of(allMemInstrBBs, [&](BasicBlock *b) {
          return DT.properlyDominates(candidateBB, b);
        })) {
      lastBB = candidateBB;
      break;
    }
  }

  // Get all BBs which are properly dominated by the lastBB.
  // If there is an any instruction in {dominatedByLastBB} which uses
  // any of the address generation instructions, return FALSE.
  SetVector<BasicBlock *> dominatedByLastBB;
  for (auto &BB : F) {
    if (DT.properlyDominates(lastBB, &BB))
      dominatedByLastBB.insert(&BB);
  }
  for (auto &I : allAddressIntrs) {
    for (auto &useI : I->uses()) {
      for (auto &dBB : dominatedByLastBB) {
        if (useI->isUsedInBasicBlock(dBB))
          return false;
      }
    }
  }

  return !isAnyStoreControlDepOnAnyLoad(DT, memInstrs);
}

/// This function has the same functionality as ScalarEvolution.getPointerBase,
/// but it returns the Instruction representing the pointer base, not a SCEV.
/// Relies on our previous pass that puts all GEPs (that get global memory
/// pointers) into F.entry
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

/// Remove entries where there are only store instructions.
/// Make sure the returned instructions are always in deterministic order.
SmallVector<SmallVector<Instruction *>> filterInstrs(
    const DenseMap<Instruction *, SetVector<Instruction *>> &addr2InstMap,
    DominatorTree &DT) {
  SmallVector<SmallVector<Instruction *>> collectedInstructions;
  SmallVector<Instruction *> instructionKeys;

  // If a given base address has only a store, then there will be no RAW
  // hazards.
  for (auto &kv : addr2InstMap) {
    if (llvm::any_of(kv.getSecond(), isaLoad)) {
      instructionKeys.push_back(kv.getFirst());

      SmallVector<Instruction *> iVec;
      for (auto &I : kv.getSecond())
        iVec.push_back(I);
      collectedInstructions.push_back(iVec);
    }
  }

  // Use base address instruction dominance relation to sort the collected
  // instructions.
  SmallVector<int> sortingIndices(instructionKeys.size());
  std::iota(sortingIndices.begin(), sortingIndices.end(), 0);
  llvm::sort(sortingIndices, [&](int a, int b) {
    return DT.dominates(instructionKeys[a], instructionKeys[b]);
  });
  SmallVector<SmallVector<Instruction *>> collectedInstructionsSorted(
      collectedInstructions.size());
  for (uint i = 0; i < sortingIndices.size(); ++i)
    collectedInstructionsSorted[i] = collectedInstructions[sortingIndices[i]];

  return collectedInstructionsSorted;
}

SmallVector<SmallVector<Instruction *>>
splitIntoTypedClusters(const SmallVector<SmallVector<Instruction *>> &instrs) {
  SmallVector<SmallVector<Instruction *>> splitInstrClusters;

  for (auto &memInstr : instrs) {
    SmallVector<SmallVector<Instruction *>> types2instrs(memInstr.size());
    int numTypes = 0;

    // Preserve deterministic order across runs.
    DenseMap<Type *, int> types2index;
    for (auto &I : memInstr) {
      auto type = isaStore(I) ? I->getOperand(0)->getType() : I->getType();

      if (types2index.find(type) == types2index.end()) {
        types2index[type] = numTypes;
        numTypes++;
      }
      types2instrs[types2index[type]].push_back(I);
    }

    for (int i = 0; i < numTypes; i++) {
      splitInstrClusters.push_back(types2instrs[i]);
    }
  }

  return splitInstrClusters;
}

DataHazardAnalysis::DataHazardAnalysis(Function &F, LoopInfo &LI,
                                       ScalarEvolution &SE, DominatorTree &DT) {
  // Collect all base addresses that are stored to with an SE uncomputable index
  // inside a loop.
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
    auto siPointerBase =
        isaStore(I)
            ? getPointerBase(dyn_cast<StoreInst>(I)->getPointerOperand())
            : getPointerBase(dyn_cast<MemCpyInst>(I)->getOperand(0));
    if (auto bucketToInsert = getEquivalentInMap(addr2InstMap, siPointerBase)) {
      bucketToInsert->insert(I);
    }
  }

  // Collect loads seperately.
  for (Loop *TopLevelLoop : LI) {
    for (Loop *L : depth_first(TopLevelLoop)) {
      for (auto &BB : L->getBlocks()) {
        for (auto &I : *BB) {
          if (auto li = dyn_cast<LoadInst>(&I)) {
            // Ignore loads of pointers.
            if (li->getType()->isPointerTy())
              continue;

            auto pointerBaseInstr = getPointerBase(li->getPointerOperand());
            // We are only intersted in loads where there is a store to the the
            // same base address.
            if (auto bucketToInsert =
                    getEquivalentInMap(addr2InstMap, pointerBaseInstr)) {
              bucketToInsert->insert(&I);
            }
          }
        }
      }
    }
  }

  auto filteredInstructions = filterInstrs(addr2InstMap, DT);

  hazardInstrs = splitIntoTypedClusters(filteredInstructions);

  // For each base address, record if the address generation can be decoupled.
  for (auto cluster : hazardInstrs) 
    decouplingDecisions.push_back(canDecoupleAddressFromAccess(F, DT, cluster));
}

} // namespace llvm
