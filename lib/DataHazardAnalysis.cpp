#include "CommonLLVM.h"
#include "DataHazardAnalysis.h"

#include <numeric>
#include <sstream>
#include <string>


using namespace llvm;

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
    if (std::any_of(kv.getSecond().begin(), kv.getSecond().end(), isaLoad)) {
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
  std::sort(sortingIndices.begin(), sortingIndices.end(), [&](int a, int b) {
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

  clusteredInstructions = splitIntoTypedClusters(filteredInstructions);
}
