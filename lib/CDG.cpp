//===----------------------------------------------------------------------===//
//
// The implementation for the control dependence graph.
//
//===----------------------------------------------------------------------===//

#include "CDG.h"


using namespace llvm;

template class llvm::DGEdge<CDGNode, CDGEdge>;
template class llvm::DGNode<CDGNode, CDGEdge>;
template class llvm::DirectedGraph<CDGNode, CDGEdge>;

//===--------------------------------------------------------------------===//
// CDGNode implementation
//===--------------------------------------------------------------------===//
CDGNode::~CDGNode() = default;

raw_ostream &llvm::operator<<(raw_ostream &OS, const CDGNode::NodeKind K) {
  const char *Out;
  switch (K) {
  case CDGNode::NodeKind::BasicBlock:
    Out = "basic block";
    break;
  case CDGNode::NodeKind::Root:
    Out = "root";
    break;
  case CDGNode::NodeKind::Unknown:
    Out = "?? (error)";
    break;
  }
  OS << Out;
  return OS;
}

raw_ostream &llvm::operator<<(raw_ostream &OS, const CDGNode &N) {
  if (auto BN = dyn_cast<BlockCDGNode>(&N)) 
    OS << "basic block " << BN->getBasicBlock()->getNameOrAsOperand();
  else if (isa<RootCDGNode>(N)) 
    OS << "root";
  else 
    OS << "?? (error)";

  OS << (N.getEdges().empty() ? " Edges:none!\n" : " Edges:\n");
  for (const auto &E : N.getEdges())
    OS.indent(2) << *E;
  return OS;
}

//===--------------------------------------------------------------------===//
// BlockCDGNode implementation
//===--------------------------------------------------------------------===//

BlockCDGNode::BlockCDGNode(BasicBlock *BB)
    : CDGNode(NodeKind::BasicBlock) {
  basicBlock = BB;
}

BlockCDGNode::BlockCDGNode(const BlockCDGNode &N)
    : CDGNode(N), basicBlock(N.basicBlock) {
  assert((getKind() == NodeKind::BasicBlock && basicBlock) &&
         "constructing from invalid bloc node.");
}

BlockCDGNode::BlockCDGNode(BlockCDGNode &&N)
    : CDGNode(std::move(N)), basicBlock(std::move(N.basicBlock)) {
  assert((getKind() == NodeKind::BasicBlock && basicBlock) &&
         "constructing from invalid bloc node.");
}

BlockCDGNode::~BlockCDGNode() { basicBlock = nullptr; }

//===--------------------------------------------------------------------===//
// CDGEdge implementation
//===--------------------------------------------------------------------===//

raw_ostream &llvm::operator<<(raw_ostream &OS, const CDGEdge::EdgeKind K) {
  const char *Out;
  switch (K) {
  case CDGEdge::EdgeKind::True:
    Out = "true";
    break;
  case CDGEdge::EdgeKind::False:
    Out = "false";
    break;
  case CDGEdge::EdgeKind::Rooted:
    Out = "rooted";
    break;
  case CDGEdge::EdgeKind::Unknown:
    Out = "?? (error)";
    break;
  }
  OS << Out;
  return OS;
}

raw_ostream &llvm::operator<<(raw_ostream &OS, const CDGEdge &E) {
  // dst is guaranteed to be a BB.
  auto dstBBName = dyn_cast<BlockCDGNode>(&E.getTargetNode())
                       ->getBasicBlock()
                       ->getNameOrAsOperand();
  OS << "[" << E.getKind() << "] to " << dstBBName << "\n";
  return OS;
}

//===--------------------------------------------------------------------===//
// ControlDependenceGraph implementation
//===--------------------------------------------------------------------===//

bool ControlDependenceGraph::isControlDependent(const BasicBlock *B,
                                                const BasicBlock *A,
                                                PostDominatorTree &PDT) {
  // If B is guaranteed to execute when A executes, then it's not ctrl. dep.
  // The "properly" ensures that A and B are not the same block.
  if (PDT.properlyDominates(B, A)) 
    return false;

  // So, when control is at A, then B is not guranteed to execute. Is there an
  // edge from A that, when taken, guarantees that B is executed?
  return llvm::any_of(successors(A), [&PDT, &B](auto succA) {
    return PDT.dominates(B, succA);
  });
}

BasicBlock *ControlDependenceGraph::getControlDependencySource(BasicBlock *BB) {
  // If there is an edge from the CDG root to the parent of {interIterDep},
  // then {interIterDep} is not control dependent.
  auto cdgNode = this->getBlockNode(BB);
  if (this->getRoot()->hasEdgeTo(*cdgNode))
    return nullptr;

  // {I} is control dependent on some block. Find out the source block.
  for (auto &N : *this) {
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

void ControlDependenceGraph::calculateCDG(Function &F, PostDominatorTree &PDT) {
  // Create a special node for the CDG root and a block node for each BB.
  auto &root = createRootNode();
  for (auto &BB : F)
    createBlockNode(&BB);

  // Calculate control dependence edges between all CDG nodes.
  for (auto &BB : F) {
    auto BBN = getBlockNode(&BB);
    bool isControlDependentOnAny = false;

    for (auto &predBB : F) {
      if (isControlDependent(&BB, &predBB, PDT)) {
        isControlDependentOnAny = true;
        auto predBBN = getBlockNode(&predBB);
        createEdge(*predBBN, *BBN);
      }
    }

    // Blocks guaranteed to be executed are directly reachable from the root.
    if (!isControlDependentOnAny)
      createEdge(root, *BBN);
  }
}

bool ControlDependenceGraph::addNode(CDGNode &N) {
  if (!CDGBase::addNode(N))
    return false;

  if (isa<RootCDGNode>(N))
    Root = &N;

  return true;
}

BlockCDGNode *ControlDependenceGraph::getBlockNode(const BasicBlock *BB) {
  auto nodeBBs =
      make_filter_range(Nodes, [](CDGNode *N) { return isa<BlockCDGNode>(N); });
  auto searchIter = find_if(nodeBBs, [&](auto N) {
    return dyn_cast<BlockCDGNode>(N)->getBasicBlock() == BB;
  });

  return searchIter != nodeBBs.end() ? dyn_cast<BlockCDGNode>(*searchIter)
                                     : nullptr;
}

CDGEdge::EdgeKind ControlDependenceGraph::getEdgeKind(const BasicBlock *A,
                                                      const BasicBlock *B) {
  assert(isPotentiallyReachable(A, B) && "Blocks A and B are not connected.");
  
  const BranchInst *branchA = dyn_cast<BranchInst>(A->getTerminator());
  int numSucc = branchA->getNumSuccessors();
  
  // Case 1 where B is a direct descendant of B.
  if (branchA->getSuccessor(0) == B)
    return CDGEdge::EdgeKind::True;
  else if (numSucc > 1 && branchA->getSuccessor(1) == B)
    return CDGEdge::EdgeKind::False;

  // Case 2 where the A~>B path has intermediate blocks. 
  // Find out if we have to take the True or False branch at A.
  if (isPotentiallyReachable(branchA->getSuccessor(0), B))
    return CDGEdge::EdgeKind::True;
  else if (numSucc > 1 && isPotentiallyReachable(branchA->getSuccessor(1), B))
    return CDGEdge::EdgeKind::False;

  assert(false && "Could not determine edge kind");
}

CDGEdge &ControlDependenceGraph::createEdge(CDGNode &Src, CDGNode &Tgt) {
  auto EdgeKind = CDGEdge::EdgeKind::Unknown;
  if (isa<RootCDGNode>(Src)) {
    EdgeKind = CDGEdge::EdgeKind::Rooted;
  } else {
    // If not a RootCDGNode, then a block node. Is the edge True or False?
    auto ABB = dyn_cast<BlockCDGNode>(&Src)->getBasicBlock();
    auto BBB = dyn_cast<BlockCDGNode>(&Tgt)->getBasicBlock();
    assert((ABB && BBB) && "Expected two basic blocks.");
    EdgeKind = getEdgeKind(ABB, BBB);
  }

  auto *E = new CDGEdge(Tgt, EdgeKind);
  connect(Src, Tgt, *E);
  return *E;
}

raw_ostream &llvm::operator<<(raw_ostream &OS, const ControlDependenceGraph &G) {
  for (CDGNode *Node : G)
    OS << *Node << "\n";
  OS << "\n";
  return OS;
}
