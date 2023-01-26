//===----------------------------------------------------------------------===//
//
// This file defines the Control-Dependence Graph (CDG) as presented in
// the in Ferrante et al. "The Program Dependence Graph and Its Use
// in Optimization".
//
// The class structure roughly follows LLVM's DDG analysis pass.
//
//===----------------------------------------------------------------------===//

#ifndef CDG_H
#define CDG_H

#include "llvm/ADT/DirectedGraph.h"
#include "llvm/ADT/SmallVector.h"

#include "llvm/Analysis/CFG.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"

namespace llvm {
class Function;
class PostDominatorTree;
class CDGNode;
class CDGEdge;
using CDGNodeBase = DGNode<CDGNode, CDGEdge>;
using CDGEdgeBase = DGEdge<CDGNode, CDGEdge>;
using CDGBase = DirectedGraph<CDGNode, CDGEdge>;

/// Control Dependence Graph Node corresponding.
/// The graph can represent the following types of nodes:
/// 1. Basic block of instructions.
/// 2. Root node is a special node that represents the entry to a function.
class CDGNode : public CDGNodeBase {
public:
  enum class NodeKind {
    Unknown,
    BasicBlock,
    Root,
  };

  CDGNode() = delete;
  CDGNode(const NodeKind K) : Kind(K) {}
  CDGNode(const CDGNode &N) = default;
  CDGNode(CDGNode &&N) : CDGNodeBase(std::move(N)), Kind(N.Kind) {}
  virtual ~CDGNode() = 0;

  CDGNode &operator=(const CDGNode &N) {
    DGNode::operator=(N);
    Kind = N.Kind;
    return *this;
  }

  CDGNode &operator=(CDGNode &&N) {
    DGNode::operator=(std::move(N));
    Kind = N.Kind;
    return *this;
  }

  /// Getter for the kind of this node.
  NodeKind getKind() const { return Kind; }

protected:
  /// Setter for the kind of this node.
  void setKind(NodeKind K) { Kind = K; }

private:
  NodeKind Kind;
};

/// Subclass of CDGNode representing the root node of the graph.
/// There should only be one such node in a given graph.
class RootCDGNode : public CDGNode {
public:
  RootCDGNode() : CDGNode(NodeKind::Root) {}
  RootCDGNode(const RootCDGNode &N) = delete;
  RootCDGNode(RootCDGNode &&N) : CDGNode(std::move(N)) {}
  ~RootCDGNode() = default;

  /// Define classof to be able to use isa<>, cast<>, dyn_cast<>, etc.
  static bool classof(const CDGNode *N) {
    return N->getKind() == NodeKind::Root;
  }
  static bool classof(const RootCDGNode *N) { return true; }
};

/// Subclass of CDGNode representing basic block node.
class BlockCDGNode : public CDGNode {
  friend class CDGBuilder;

public:
  BlockCDGNode() = delete;
  BlockCDGNode(BasicBlock *BB);
  BlockCDGNode(const BlockCDGNode &N);
  BlockCDGNode(BlockCDGNode &&N);
  ~BlockCDGNode();

  BlockCDGNode &operator=(const BlockCDGNode &N) = default;

  BlockCDGNode &operator=(BlockCDGNode &&N) {
    CDGNode::operator=(std::move(N));
    basicBlock = std::move(N.basicBlock);
    return *this;
  }

  /// Get the basic block for this node.
  BasicBlock *getBasicBlock() const {
    assert(basicBlock && "Basic Block is null.");
    return basicBlock;
  }

  /// Define classof to be able to use isa<>, cast<>, dyn_cast<>, etc.
  static bool classof(const CDGNode *N) {
    return N->getKind() == NodeKind::BasicBlock;
  }
  static bool classof(const BlockCDGNode *N) { return true; }

private:
  /// Set the basic block for this node.
  void setBasicBlock(BasicBlock *block) { basicBlock = block; }

  /// The basic block for this node.
  BasicBlock *basicBlock;
};

/// Control Dependency Graph Edge.
/// An edge in the CDG represents a possible transfer of control
/// between two nodes.
/// A rooted edge from the root node to to a given node N means
/// that the node N is guaranteed to execute.
class CDGEdge : public CDGEdgeBase {
public:
  /// The kind of edge in the CDG
  enum class EdgeKind {
    Unknown,
    True,
    False,
    Rooted,
    Last = Rooted // Must be equal to the largest enum value.
  };

  explicit CDGEdge(CDGNode &N) = delete;
  CDGEdge(CDGNode &N, EdgeKind K) : CDGEdgeBase(N), Kind(K) {}
  CDGEdge(const CDGEdge &E) : CDGEdgeBase(E), Kind(E.getKind()) {}
  CDGEdge(CDGEdge &&E) : CDGEdgeBase(std::move(E)), Kind(E.Kind) {}
  CDGEdge &operator=(const CDGEdge &E) = default;

  CDGEdge &operator=(CDGEdge &&E) {
    CDGEdgeBase::operator=(std::move(E));
    Kind = E.Kind;
    return *this;
  }

  /// Get the edge kind
  EdgeKind getKind() const { return Kind; };

  /// Return true if this is a true edge, and false otherwise.
  bool isTrueEdge() const { return Kind == EdgeKind::True; }

  /// Return true if this is a false edge, and false otherwise.
  bool isFalseEdge() const { return Kind == EdgeKind::False; }

  /// Return true if this is an edge stemming from the root node, 
  /// and false otherwise.
  bool isRooted() const { return Kind == EdgeKind::Rooted; }

private:
  EdgeKind Kind;
};

/// Control Dependency Graph
class ControlDependenceGraph : public CDGBase {
public:
  using NodeType = CDGNode;
  using EdgeType = CDGEdge;

  explicit ControlDependenceGraph(Function &F, PostDominatorTree &PTD) {
    calculateCDG(F, PTD);
  }
  ~ControlDependenceGraph() {
    for (auto *N : Nodes) {
      for (auto *E : *N)
        delete E;
      delete N;
    }
  }

  /// Return the root node of the graph.
  NodeType *getRoot() const {
    assert(Root && "Root node is not available yet. Graph construction may "
                   "still be in progress\n");
    return Root;
  }
  
  /// Return the BlockCDGNode for the BB, and nullptr otherwise.
  BlockCDGNode *getBlockNode(const BasicBlock *BB);

  /// Return the EdgeKind needed to be taken from A to reach B in the CFG
  /// (either true or false). 
  CDGEdge::EdgeKind getEdgeKind(const BasicBlock *A, const BasicBlock *B);

  /// Return true if block B is control dependent on block A.
  /// This amounts to checking if there is an A->B edge in the CDG.
  bool isControlDependent(const BasicBlock *B, const BasicBlock *A) {
    auto BNode = getBlockNode(B), ANode = getBlockNode(A);
    assert((ANode && BNode) && "Basic block not part of the CDG.");
    return ANode->hasEdgeTo(*BNode);
  }

private:
  /// A special node in the graph representing root of the CDG.
  NodeType *Root = nullptr;

  /// Return true is B is control dependent on A, false otherwise.
  /// B is control-dependent on A iff A determines whether B executes, i.e.:
  ///   - there exists a path from A to B such that every node
  ///     in the path other than A & B is post-dominated by B
  ///   - A is not post-dominated by B
  bool isControlDependent(const BasicBlock *B, const BasicBlock *A,
                          PostDominatorTree &PDT);

  void calculateCDG(Function &F, PostDominatorTree &PTD);

  /// Add node \p N to the graph, if it's not added yet. 
  /// Return true if node is successfully added.
  bool addNode(NodeType &N);

  /// Create a root node and add it to the CDG.
  CDGNode &createRootNode() {
    auto *RN = new RootCDGNode();
    assert(RN && "Failed to allocate memory for CDG root node.");
    addNode(*RN);
    return *RN;
  }

  /// Create a basic block node and add it to the CDG.
  CDGNode &createBlockNode(BasicBlock *BB) {
    auto *BN = new BlockCDGNode(BB);
    assert(BN && "Failed to allocate memory for basic block node.");
    addNode(*BN);
    return *BN;
  }

  /// Create an endge between Src and Tgt CDG nodes.
  /// Decide between True, False, Rooted EdgeKind automatically.
  CDGEdge &createEdge(NodeType &Src, NodeType &Tgt);
};

raw_ostream &operator<<(raw_ostream &OS, const CDGNode &N);
raw_ostream &operator<<(raw_ostream &OS, const CDGNode::NodeKind K);
raw_ostream &operator<<(raw_ostream &OS, const CDGEdge &E);
raw_ostream &operator<<(raw_ostream &OS, const CDGEdge::EdgeKind K);
raw_ostream &operator<<(raw_ostream &OS, const ControlDependenceGraph &G);

//===--------------------------------------------------------------------===//
// GraphTraits specializations for the CDG
//===--------------------------------------------------------------------===//

/// non-const versions of the grapth trait specializations for CDG
template <> struct GraphTraits<CDGNode *> {
  using NodeRef = CDGNode *;

  static CDGNode *CDGGetTargetNode(DGEdge<CDGNode, CDGEdge> *P) {
    return &P->getTargetNode();
  }

  // Provide a mapped iterator so that the GraphTrait-based implementations can
  // find the target nodes without having to explicitly go through the edges.
  using ChildIteratorType =
      mapped_iterator<CDGNode::iterator, decltype(&CDGGetTargetNode)>;
  using ChildEdgeIteratorType = CDGNode::iterator;

  static NodeRef getEntryNode(NodeRef N) { return N; }
  static ChildIteratorType child_begin(NodeRef N) {
    return ChildIteratorType(N->begin(), &CDGGetTargetNode);
  }
  static ChildIteratorType child_end(NodeRef N) {
    return ChildIteratorType(N->end(), &CDGGetTargetNode);
  }

  static ChildEdgeIteratorType child_edge_begin(NodeRef N) {
    return N->begin();
  }
  static ChildEdgeIteratorType child_edge_end(NodeRef N) { return N->end(); }
};

template <>
struct GraphTraits<ControlDependenceGraph *> : public GraphTraits<CDGNode *> {
  using nodes_iterator = ControlDependenceGraph::iterator;
  static NodeRef getEntryNode(ControlDependenceGraph *DG) {
    return DG->getRoot();
  }
  static nodes_iterator nodes_begin(ControlDependenceGraph *DG) {
    return DG->begin();
  }
  static nodes_iterator nodes_end(ControlDependenceGraph *DG) {
    return DG->end();
  }
};

/// const versions of the grapth trait specializations for CDG
template <> struct GraphTraits<const CDGNode *> {
  using NodeRef = const CDGNode *;

  static const CDGNode *CDGGetTargetNode(const DGEdge<CDGNode, CDGEdge> *P) {
    return &P->getTargetNode();
  }

  // Provide a mapped iterator so that the GraphTrait-based implementations can
  // find the target nodes without having to explicitly go through the edges.
  using ChildIteratorType =
      mapped_iterator<CDGNode::const_iterator, decltype(&CDGGetTargetNode)>;
  using ChildEdgeIteratorType = CDGNode::const_iterator;

  static NodeRef getEntryNode(NodeRef N) { return N; }
  static ChildIteratorType child_begin(NodeRef N) {
    return ChildIteratorType(N->begin(), &CDGGetTargetNode);
  }
  static ChildIteratorType child_end(NodeRef N) {
    return ChildIteratorType(N->end(), &CDGGetTargetNode);
  }

  static ChildEdgeIteratorType child_edge_begin(NodeRef N) {
    return N->begin();
  }
  static ChildEdgeIteratorType child_edge_end(NodeRef N) { return N->end(); }
};

template <>
struct GraphTraits<const ControlDependenceGraph *>
    : public GraphTraits<const CDGNode *> {
  using nodes_iterator = ControlDependenceGraph::const_iterator;
  static NodeRef getEntryNode(const ControlDependenceGraph *DG) {
    return DG->getRoot();
  }
  static nodes_iterator nodes_begin(const ControlDependenceGraph *DG) {
    return DG->begin();
  }
  static nodes_iterator nodes_end(const ControlDependenceGraph *DG) {
    return DG->end();
  }
};

} // namespace llvm

#endif // CDG_H
