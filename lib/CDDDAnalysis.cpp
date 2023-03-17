#include "CDDDAnalysis.h"
#include "CDG.h"
#include "CommonLLVM.h"

#include "TableOperationLatency.h"

using namespace llvm;

namespace llvm {

/// Gicen a SCC from a loop DDG, check if the sequence of DDG nodes obeys 
/// control flow.
bool isPathPossible(const SmallVector<SimpleDDGNode *> &Path,
                    const PiBlockDDGNode &SCC) {
  auto sccNodes = SCC.getNodes();

  auto getBB = [](DDGNode *N) {
    return dyn_cast<SimpleDDGNode>(N)->getFirstInstruction()->getParent();
  };

  for (auto N : Path) {
    for (auto sccN : sccNodes) {
      if (N == sccN)
        continue;

      // For a path to be valid, all SCC nodes with the same BB should be in it.
      if (getBB(N) == getBB(sccN) && !llvm::is_contained(Path, sccN)) 
        return false;
    }
  }

  return true;
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
      // If the path has improssible control flow, ignore it.
      if (isPathPossible(stack, SCC))
        this->sccPaths.push_back(stack);
    }
    else {
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
void ControlDependentDataDependencyAnalysis::nodesToInstructions() {
  for (auto Path : this->sccPaths) {
    SmallVector<Instruction *> thisPath;
    for (auto N : Path) 
      append_range(thisPath, N->getInstructions());
    
    this->sccInstructionPaths.push_back(thisPath);
  }
}

/// Calculate the II of each path through the SCC.
void ControlDependentDataDependencyAnalysis::calculateAllPathII() {
  for (auto SCC : sccInstructionPaths) {
    int thisSccII = calculatePathII(SCC);
    this->sccIIs.push_back(thisSccII);

    if (thisSccII >= maxII) {
      this->maxII = thisSccII;
      this->criticalPath = SCC;
    }
  }
}

void ControlDependentDataDependencyAnalysis::calculateBlocksToDecouple(
    LoopInfo &LI, ControlDependenceGraph &CDG) {
  for (size_t iPath = 0; iPath < sccInstructionPaths.size(); ++iPath) {
    for (auto I : sccInstructionPaths[iPath]) {
      auto ctrlDepSrc = CDG.getControlDependencySource(I);
      auto candidateBB = I->getParent();

      // Ignore candidateBB if the path has an II of 1, or if the block is not
      // ctrl. dep. on a block that is not a loop header,
      if (sccIIs[iPath] > 1 && ctrlDepSrc && !LI.isLoopHeader(ctrlDepSrc)) {
        // Finally, check if the instructions in the block increase the II.
        auto subPath = getInstructions(candidateBB);
        if (calculatePathII(subPath) > 1) 
          this->blocksToDecouple.insert(candidateBB);
      }
    }
  }
}

void ControlDependentDataDependencyAnalysis::calculateIncomingUses() {
  for (auto BB : this->blocksToDecouple) {
    auto isUserInBB = [&BB](auto User) {
      if (auto UserI = dyn_cast<Instruction>(User))
        return UserI->getParent() == BB;
      return false;
    };

    SmallVector<Instruction *> usesInThisBB;
    for (auto &otherBB : *BB->getParent()) {
      for (auto &I : otherBB) {
        if (&otherBB != BB && llvm::any_of(I.users(), isUserInBB)) 
          usesInThisBB.push_back(&I);
      }
    }

    this->incomingUses.push_back(usesInThisBB);
  }
}

void ControlDependentDataDependencyAnalysis::calculateOutgoingDefs() {
  for (auto BB : this->blocksToDecouple) {
    SmallVector<Instruction *> defsInThisBB;
    for (auto &I : *BB) {
      for (auto User : I.users()) {
        if (auto UserI = dyn_cast<Instruction>(User)) {
          // If the user of an instruction defined in this block is not in this
          // block, then add it to our list.
          if (UserI->getParent() != BB) {
            defsInThisBB.push_back(&I);
            break;
          }
        }
      }

    }

    // Since the terminator instruction of the block will not be decoupled, 
    // check if its operand is defined in this block and add it if yes.
    auto blockTerminator = BB->getTerminator();
    if (auto cond = dyn_cast<Instruction>(blockTerminator->getOperand(0))) {
      if (cond->getParent() == BB)
        defsInThisBB.push_back(cond);
    }

    this->outgoingDefs.push_back(defsInThisBB);
  }
}

void ControlDependentDataDependencyAnalysis::print(raw_ostream &out, int id) {
  out << "\n##### Recurrence Info " << id << " #####"
      << "\n-- Estimated max recurrence II=" << std::to_string(maxII) << "\n"
      << "\n-- Critical path:\n";  
  
  if (criticalPath.size() > 10) {
    out << "  not showing because too long (" + std::to_string(criticalPath.size()) +
            " instructions)\n";
  }
  else {
    for (auto I : criticalPath) {
      out << "  From block " << I->getParent()->getNameOrAsOperand() << ": ";
      I->print(out);
      out << "\n";
    }
  }
  
  out << "\n-- " << blocksToDecouple.size() << " control dependent blocks:\n";
  for (auto BB : blocksToDecouple)
    out << "\t" << BB->getNameOrAsOperand() << "\n";

  out << "\n### End Recurrence Info " << id << " ###\n";
}

} // end namespace llvm
