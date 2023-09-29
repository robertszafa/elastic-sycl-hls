#include "CDDDAnalysis.h"
#include "CDG.h"
#include "CommonLLVM.h"

#include "TableOperationLatency.h"

using namespace llvm;

namespace llvm {

int getPathII(SmallVector<Instruction *> &SCC) {
  int ii = 0;
  for (auto I : SCC) {
    // Given an opcode, get its latency and add it to II. If an opcode is
    // not found in the table, then 0 is returned (e.g. phis have no latency).
    ii += LATENCY_TABLE_ARRIA10.lookup(I->getOpcode());
  }

  return ii;
}

/// Given a loop recurrence (SCC of the data dependency graph), return all
/// possible paths through the recurrence that can result from control flow.
SmallVector<SmallVector<SimpleDDGNode *>>
ControlDependentDataDependencyAnalysis::getSCCPaths(PiBlockDDGNode &SCC) {
  SmallVector<SmallVector<SimpleDDGNode *>> AllPaths;

  if (SCC.getNodes().size() == 0)
    return AllPaths;

  // All paths end when hitting an edge to the start node.
  const auto startNode = SCC.getNodes()[0];
  auto nextEdge = startNode->getEdges()[0];

  // The nodes in the stack form the prefix of all currently explorable paths.
  SmallVector<SimpleDDGNode *> stack;
  stack.push_back(dyn_cast<SimpleDDGNode>(startNode));

  // To prevent invinitely going in cycles, record for each path prefix which
  // edges have already been taken.
  SmallVector<std::pair<int, DDGEdge *>> edgesDoneInThisPrefix;
  int pathPrefix = 0;
  while (stack.size() > 0) {
    edgesDoneInThisPrefix.push_back({pathPrefix, nextEdge});

    // We will either extend the path prefix, or terminate the current path.
    auto nextNode = &nextEdge->getTargetNode();
    if (nextNode == startNode) {
      AllPaths.push_back(stack);
    } else {
      stack.push_back(dyn_cast<SimpleDDGNode>(nextNode));
      pathPrefix++;
    }

    // Find the next edge to take.
    while (pathPrefix >= 0) {
      // Is there any edge not yet taken at the frontier of the current path?
      bool foundEdge = false;
      for (auto E : stack[pathPrefix]->getEdges()) {
        // Avoid returning to the same node.
        auto targetNode = &E->getTargetNode();
        if (targetNode != startNode && llvm::is_contained(stack, targetNode))
          continue;

        // Was this edge explored in this path?
        std::pair<int, DDGEdge *> nodeEdgePair = {pathPrefix, E};
        if (!llvm::is_contained(edgesDoneInThisPrefix, nodeEdgePair)) {
          nextEdge = E;
          foundEdge = true;
          break;
        }
      }
      
      if (foundEdge) {
        break; // Explore this edge
      } else {
        // Backtrack and look again. 
        // Once stack empties (pathPrefix < 0), all paths have been explored.
        stack.pop_back();
        pathPrefix--;
      }
    }
  }

  return AllPaths;
}

/// Convert DDG nodes in SCC paths into instructions.
SmallVector<SmallVector<Instruction *>>
ControlDependentDataDependencyAnalysis::DDGNodesToInstructions(
    SmallVector<SmallVector<SimpleDDGNode *>> &AllSccPaths) {
  SmallVector<SmallVector<Instruction *>> AllSCCInstructionPaths;

  for (auto Path : AllSccPaths) {
    SmallVector<Instruction *> thisPath;
    for (auto N : Path) {
      for (auto &I : N->getInstructions()) {
        // Ignore llvm void instrinsics.
        if (!I->getType()->isVoidTy())
          thisPath.push_back(I);
      }
    }

    AllSCCInstructionPaths.push_back(thisPath);
  }

  return AllSCCInstructionPaths;
}

void ControlDependentDataDependencyAnalysis::collectBlocksToDecouple(
    LoopInfo &LI, ControlDependenceGraph &CDG,
    SmallVector<SmallVector<Instruction *>> &AllSCCInstructionPaths) {
  // Go through all unique SCC paths.
  for (auto Path : AllSCCInstructionPaths) {
    if (Path.empty()) continue;

    auto L = LI.getLoopFor(Path[0]->getParent());
    auto loopHeader = L->getHeader();
    auto loopLatch = L->getLoopLatch();

    PHINode *recStart = nullptr, *recEnd = nullptr;
    SetVector<BasicBlock *> allBlocksOnPath;
    for (auto &I : Path) {
      allBlocksOnPath.insert(I->getParent());

      if (auto phiI = dyn_cast<PHINode>(I)) {
        if (phiI->getParent() == loopHeader)
          recStart = phiI;
        else if (phiI->getParent() == loopLatch)
          recEnd = phiI;
      }
    }

    // A recurrence through registers will have at least two PHI nodes.
    if (!recStart || !recEnd)
      continue;

    // Go through all unique basic blocks on the path.
    for (auto candidateBB : allBlocksOnPath) {
      auto ctrlDepSrc = CDG.getControlDependencySource(candidateBB);

      // These conditions have to hold for candidateBB to be considered:
      if (getPathII(Path) > 1 && ctrlDepSrc && !LI.isLoopHeader(ctrlDepSrc) &&
          !isLoopUnrolled(L)) {
        SmallVector<Instruction *> pathWithoutBB;
        for (auto &I : Path)
          if (I->getParent() != candidateBB)
            pathWithoutBB.push_back(I);

        SmallVector<Instruction *> subPath;
        for (auto &I : *candidateBB)
          subPath.push_back(&I);

        // The final condition is that {thisPath} II should decrease 
        // when the candidate block would be decoupled.
        if (getPathII(subPath) > 1) {
          this->blocksToDecouple.insert(candidateBB);
          this->recurrenceStarts[candidateBB] = recStart;
          this->recurrenceEnds[candidateBB] = recEnd;

          // Decoupled all instructions in the BB (except terminator and phis).
          for (auto &I : *candidateBB) {
            if (!I.isTerminator() && !isa<PHINode>(&I)) 
              instrToDecoupleInBB[candidateBB].insert(&I);
          }

          // Alternatively, decouple only instructions on the current SCC path.
          // Collect instructions to decouple from this BB. We pick only
          // instructions that are on the currently evaluated SCC path. Loads 
          // and stores are not picked up. Use a set vector since the same 
          // instructions for a BB could be collected from different SCC paths.
          // for (auto &I : allSCCInstructionPaths[iPath]) {
          //   if (I->getParent() == candidateBB && canDecouple(I)) {
          //     instrToDecoupleInBB[candidateBB].insert(I);
          //   }
          // }
        }
      }
    }
  }
}

void ControlDependentDataDependencyAnalysis::collectBlockInOutDependencies() {
  for (auto [BB, instrToDecouple] : instrToDecoupleInBB) {
    SetVector<Instruction *> inputDepForThisBB;
    SetVector<Instruction *> outputDepForThisBB;

    // PHI nodes in the dcpldBB become input dependencies.
    for (auto &phi : BB->phis())
      blockInputDependencies[BB].insert(&phi);

    for (auto &I : instrToDecouple) {
      for (size_t iOp = 0; iOp < I->getNumOperands(); ++iOp) {
        if (auto opInstr = dyn_cast<Instruction>(I->getOperand(iOp))) {
          if (!opInstr->getParent()->isEntryBlock() &&
              !instrToDecouple.contains(opInstr)) {
            blockInputDependencies[BB].insert(opInstr);
          }
        }
      }

      // Check if I is an output dep.
      for (auto User : I->users()) {
        if (auto UserI = dyn_cast<Instruction>(User)) {
          if (!instrToDecouple.contains(UserI)) {
            blockOutputDependencies[BB].insert(I);
          }
        }
      }
    }
  }
}

/// Collect decoupled LLVM values that are only updated by decoupled blocks:
///   loopHeader: 
///     recStart = phi(loopPreheader, loopLatch)
///   decoupledBlock:
///     update = use(recStart) 
///   loopLatch:
///     recEnd = phi(loopHEader, decoupledBlock) 
/// Such in/out dependencies can be communicated once before and after the loop.
void ControlDependentDataDependencyAnalysis::collectDependenciesToHoist(
    LoopInfo &LI) {
  for (auto [decoupledBB, inDeps] : blockInputDependencies) {
    auto outDeps = blockOutputDependencies[decoupledBB];

    auto L = LI.getLoopFor(decoupledBB);
    auto loopHeader = L->getHeader();
    auto loopLatch = L->getLoopLatch();
    auto recStart = recurrenceStarts[decoupledBB];
    auto recEnd = recurrenceEnds[decoupledBB];

    // Cannot hoist if the recurrence start is used anywhere else in the loop
    // body but the decoupled block.
    bool canHoistStart = true;
    for (auto User : recStart->users()) {
      if (auto UserI = dyn_cast<Instruction>(User)) {
        auto UserBB = UserI->getParent();
        if (!L->contains(UserBB))
          continue;
        if (UserBB != decoupledBB && UserBB != loopLatch) 
          canHoistStart = false;
      }
    }

    // Also cannot hoist if any of the incoming values to the reccurence end
    // have been produced in any loop body block but the decoupled block.
    bool canHoistEnd = true;
    bool recEndUsesRecStart = false;
    // Get the value coming from the PE during the traversal.
    Instruction *recEndValFromPE = nullptr;
    for (auto &srcVal : recEnd->incoming_values()) {
      if (srcVal == recStart)
        recEndUsesRecStart = true;

      if (auto srcI = dyn_cast<Instruction>(&srcVal)) {
        auto srcBB = srcI->getParent();
        if (srcBB != loopHeader && srcBB != decoupledBB)
          canHoistEnd = false;
        else if (outDeps.contains(srcI))
          recEndValFromPE = srcI;
      }
    }

    if (canHoistStart && canHoistEnd && recEndUsesRecStart && recEndValFromPE) {
      instrToHoistInBB[decoupledBB].insert(recStart);
      instrToHoistInBB[decoupledBB].insert(recEndValFromPE);
    }
  }
}

/// Collect loops to decouple. A loop will be marked for decoupling if either:
// 1. it is ctrl dependent on anything else than another loop's header, 
// 2. it has sibling loops (loops at the same nesting depth), such that there 
//    are no data and memory dependencies between the sibling loops. 
// Unrolled loops are ignored in all cases.
void ControlDependentDataDependencyAnalysis::collectLoopsToDecouple(
    LoopInfo &LI, ControlDependenceGraph &CDG) {
  // Traverse all loops in breadth first order. This is important for marking,
  // because we add to {doNotDecouple} loops that come later in the traversal.
  auto loopsInBFS = LI.getLoopsInPreorder();
  llvm::sort(loopsInBFS, [](Loop *L1, Loop *L2) {
    return L1->getLoopDepth() < L2->getLoopDepth();
  });

  // 1) collect some useful information about each loop. 
  MapVector<Loop *, SetVector<Instruction *>> memoryUsesInLoop;
  MapVector<int, int> numLoopsAtLevel;
  SetVector<Instruction *> atLeastOneStore;
  for (Loop *L : loopsInBFS) {
    // Init.
    if (!memoryUsesInLoop.contains(L))
      memoryUsesInLoop[L] = SetVector<Instruction *>();
    if (!numLoopsAtLevel.contains(L->getLoopDepth()))
      numLoopsAtLevel[L->getLoopDepth()] = 1;
    else
      numLoopsAtLevel[L->getLoopDepth()]++;

    // Collect memory arrays used in the loop and check if there are any stores.
    for (auto &BB : getUniqueLoopBlocks(L)) {
      for (auto &I : *BB) {
        if (auto si = dyn_cast<StoreInst>(&I)) {
          memoryUsesInLoop[L].insert(getPointerBase(si->getPointerOperand()));
          atLeastOneStore.insert(getPointerBase(si->getPointerOperand()));
        } else if (auto li = dyn_cast<LoadInst>(&I)) {
          memoryUsesInLoop[L].insert(getPointerBase(li->getPointerOperand()));
        }
      }
    }
  }

  // 2) check which loops should be decoupled. 
  // Used to record loops that contain the destination of a register dependence.
  SetVector<Loop *> doNotDecouple; 
  for (auto &[L, memories] : memoryUsesInLoop) {
    // Condition 1: loop inside an if-condition.
    bool noMemAliasWithOther = true, noRegisterDep = true, isControlDep = false;

    if (auto ctrlDepSrc = CDG.getControlDependencySource(L->getHeader())) {
      if (!LI.isLoopHeader(ctrlDepSrc) && !isLoopUnrolled(L)) {
        isControlDep = true;
      }
    }

    // At this point: skip aloop if either holds:
    // - marked unrolled, or as doNotDecouple (i.e. dst of register dependency)
    // - is the very last outermost loop (front() gives the last not first...)
    // - has no siblings and is not control dependent
    if (isLoopUnrolled(L) || doNotDecouple.contains(L) || 
        (LI.getTopLevelLoops().front() == L) ||
        (numLoopsAtLevel[L->getLoopDepth()] == 1 && !isControlDep)) {
      continue;
    }

    // Condition 2: sibling loops with no dependencies between any other loop.
    for (auto &[otherL, otherMemories] : memoryUsesInLoop) {
      if (otherL == L)
        continue;
      
      // Check mem aliasing.
      for (auto mem : memories) 
        if (atLeastOneStore.contains(mem) && otherMemories.contains(mem)) 
          noMemAliasWithOther = false;
      
      // Check for register dep.
      auto otherLoopInstrs = getUniqueLoopInstructions(otherL);
      for (auto &I : *L->getHeader()) {
        for (auto UserOfI : I.users()) {
          if (auto UserI = dyn_cast<Instruction>(UserOfI)) {
            if (llvm::is_contained(otherLoopInstrs, UserI)) {
              noRegisterDep = false;
              // The destination of the reg. dep. should also not be decoupled.
              doNotDecouple.insert(otherL);
            }
          }
        }
      }
    }

    if (noMemAliasWithOther && (noRegisterDep || isControlDep)) 
      loopsToDecouple.push_back(L);
  }
}

void ControlDependentDataDependencyAnalysis::collectLoopInOutDependencies() {
  for (auto L : loopsToDecouple) {
    auto F = L->getHeader()->getParent();

    SetVector<Instruction *> inDeps, outDeps;
    for (auto &BB : *F) {
      // Instr. from entry block will be left in the loop PE.
      if (&F->getEntryBlock() == &BB) 
        continue;
      
      // If some I is declared outside L, but used in L, then it is an inDep.
      if (!L->contains(&BB)) {
        for (auto &I : BB) {
          for (auto useOfI : I.users()) {
            if (auto instrForUse = dyn_cast<Instruction>(useOfI)) {
              if (L->contains(instrForUse))
                inDeps.insert(&I);
            }
          }

        }
      } else { // If declared in L and used outside L, then it's an outDep.
        for (auto &I : BB) {
          for (auto useOfI : I.users()) {
            if (auto instrForUse = dyn_cast<Instruction>(useOfI)) {
              if (!L->contains(instrForUse))
                outDeps.insert(&I);
            }
          }
        }

      }
    }

    this->loopInputDependencies[L] = inDeps;
    this->loopOutputDependencies[L] = outDeps;
  }
}

} // end namespace llvm
