#include "CDDDAnalysis.h"
#include "CDG.h"
#include "CommonLLVM.h"

#include "TableOperationLatency.h"

using namespace llvm;

namespace llvm {

bool canDecouple(Instruction *I) {
  return !I->isTerminator() && !I->mayReadOrWriteMemory() &&
         !I->isDebugOrPseudoInst() && !isa<StoreInst>(I) && !isa<LoadInst>(I) &&
         !isa<GetElementPtrInst>(I) && !isa<AddrSpaceCastInst>(I);
}

int ControlDependentDataDependencyAnalysis::calculatePathII(
    SmallVector<Instruction *> &SCC) {
  int ii = 0;
  for (auto I : SCC) {
    // Given an opcode, get its latency and add it to II. If an opcode is
    // not found in the table, then 0 is returned (e.g. phis have no latency).
    ii += LATENCY_TABLE_ARRIA10.lookup(I->getOpcode());
  }

  return ii;
}

/// Given a loop recurrence (SCC of the data dependency graph), collect all
/// possible paths through the recurrence that can result from control flow.
///
/// The paths are allowed to contain memory-dep edges.
void ControlDependentDataDependencyAnalysis::getSCCPaths(PiBlockDDGNode &SCC) {
  if (SCC.getNodes().size() == 0) return;

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
      this->sccPaths.push_back(stack);
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
}

/// Given a list of DDGNodes, return a list of instruction from all nodes.
void ControlDependentDataDependencyAnalysis::DDGNodesToInstructions() {
  for (auto Path : this->sccPaths) {
    SmallVector<Instruction *> thisPath;
    for (auto N : Path) {
      for (auto &I : N->getInstructions()) {
          // Ignore llvm void instrinsics. 
          if (!I->getType()->isVoidTy()) 
            thisPath.push_back(I);
      }
    }

    this->allSCCInstructionPaths.push_back(thisPath);
  }
}

/// Calculate the II of each path through the SCC.
void ControlDependentDataDependencyAnalysis::calculateAllPathII() {
  for (auto SCC : allSCCInstructionPaths) {
    int thisSccII = calculatePathII(SCC);
    sccIIs.push_back(thisSccII);

    if (thisSccII >= maxII) {
      maxII = thisSccII;
      criticalPath = SCC;
    }
  }
}

void ControlDependentDataDependencyAnalysis::calculateRegionsToDecouple(
    LoopInfo &LI, ControlDependenceGraph &CDG) {
  // Go through all unique SCC paths.  
  for (size_t iPath = 0; iPath < allSCCInstructionPaths.size(); ++iPath) {
    auto thisPath = allSCCInstructionPaths[iPath];
    SetVector<Instruction *> uniquePhisOnPath;
    SetVector<BasicBlock *> allBlocksOnPath;
    for (auto &I : thisPath) {
      allBlocksOnPath.insert(I->getParent());
      if (isa<PHINode>(I))
        uniquePhisOnPath.insert(I);
    }

    // A recurrence through registers will have at least 2 connected phis.
    // We handle recurrences through memory in a separate pass.
    // TODO: Make this more robust: check if the phis use each other.
    if (uniquePhisOnPath.size() < 2)
      continue;

    // Go through all unique basic blocks on the path.
    for (auto candidateBB : allBlocksOnPath) {
      auto ctrlDepSrc = CDG.getControlDependencySource(candidateBB);

      // These conditions have to hold for candidateBB to be considered:
      if (sccIIs[iPath] > 1 && ctrlDepSrc && !LI.isLoopHeader(ctrlDepSrc) &&
          !isLoopUnrolled(LI.getLoopFor(candidateBB))) {
        SmallVector<Instruction *> pathWithoutBB;
        for (auto &I : thisPath)
          if (I->getParent() != candidateBB)
            pathWithoutBB.push_back(I);

        SmallVector<Instruction *> subPath;
        for (auto &I : *candidateBB)
          subPath.push_back(&I);

        // The final condition is that {thisPath} II should decrease 
        // when the candidate block would be decoupled.
        if (calculatePathII(subPath) > 1) {
          this->blocksToDecouple.insert(candidateBB);

          // Decoupled all instructions in the BB (except loads and stores).
          for (auto &I : *candidateBB) {
            if (canDecouple(&I)) {
              instrToDecoupleInBB[candidateBB].insert(&I);
            }
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

void ControlDependentDataDependencyAnalysis::calculateInOutDependencies() {
  for (auto blockToInstr : instrToDecoupleInBB) {
    auto BB = blockToInstr.first;
    auto instrToDecouple = blockToInstr.second;

    SetVector<Instruction *> inputDepForThisBB;
    SetVector<Instruction *> outputDepForThisBB;
    for (auto &I : instrToDecouple) {
      // Check the operands of I for input dependencies.
      for (size_t iOp = 0; iOp < I->getNumOperands(); ++iOp) {
        if (auto opInstr = dyn_cast<Instruction>(I->getOperand(iOp))) {
          if (!instrToDecouple.contains(opInstr)) {
            inputDependencies[BB].insert(opInstr);
          }
        }
      }

      // Check if I is an output dep.
      for (auto User : I->users()) {
        if (auto UserI = dyn_cast<Instruction>(User)) {
          if (!instrToDecouple.contains(UserI)) {
            outputDependencies[BB].insert(I);
          }
        }
      }
    }
  }
}

} // end namespace llvm
