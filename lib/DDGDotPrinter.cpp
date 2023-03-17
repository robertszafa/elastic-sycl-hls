//===- DDGDotPrinter.cpp - DOT printer for the data dependence graph -------==//
//
//===----------------------------------------------------------------------===//
//
// This file defines the `-dot-ddg-sccs` pass, which emits a dot file to stdout
// of instructions forming a Strongly Connected Component (SCC) in the DDG.
//
// We specialize DOTGraphTraits  to only print nodes that form a PiBlock (same
// as SCC) in the DDG.
//===----------------------------------------------------------------------===//

#include "CommonLLVM.h"

#include "llvm/Analysis/DDG.h"
#include "llvm/Support/GraphWriter.h"

#include "CDG.h"

using namespace llvm;

namespace llvm {

std::unique_ptr<ControlDependenceGraph> globalCDG;

/// A specialization of the DOTGraphTraits class to only print DDG SCCs 
/// (LLVM calls them pi-blocks).
template<> 
struct DOTGraphTraits<DataDependenceGraph *> : public DefaultDOTGraphTraits {
  DOTGraphTraits(bool simple = false) : DefaultDOTGraphTraits(simple) {}
  
  std::string getGraphName(const DataDependenceGraph *G) {
    auto kernelName = demangle(std::string(G->getName()));
    // Get only kernel name, ignoring paramenters.
    std::string::size_type pos = kernelName.find('(');
    if (pos != std::string::npos)
        kernelName = kernelName.substr(0, pos);
    
    return "\nSCCs in the Data Dependence Graph of '" + kernelName + "'";
  }

  /// Print instructions inside the node.
  static std::string getNodeDescription(const DDGNode *Node,
                                        const DataDependenceGraph *DDG) {
    std::string str = "";
    llvm::raw_string_ostream rso(str);

    if (auto simpleNode = dyn_cast<SimpleDDGNode>(Node)) {
      for (auto I : simpleNode->getInstructions()) {
        I->print(rso);
        rso << "\n";
      }
    }

    return str;
  }

  /// If the node is not part on an SCC, hide it. Root nodes and PiBlock nodes
  /// are not part of an SCC by definition.
  static bool isNodeHidden(const DDGNode *Node,
                           const DataDependenceGraph *DDG) {
    auto SCC = DDG->getPiBlock(*Node);
    if (!SCC)
      return true;
    
    return false;

    // Hide if inside SCC but has no def-use edges pointing inside the SCC.
    // auto SCCNodes = SCC->getNodes();
    // auto isDefUseInsideSCC = [&SCCNodes](auto e) {
    //   return e->isDefUse() && llvm::is_contained(SCCNodes, &e->getTargetNode());
    // };
    // if (!llvm::any_of(Node->getEdges(), isDefUseInsideSCC))
    //   return true;
    // auto pointsToThisNode = [&Node](auto e) {
    //   return e->isDefUse() && &e->getTargetNode() == Node;
    // };
    // for (auto N : SCCNodes) {
    //   if (llvm::any_of(N->getEdges(), pointsToThisNode)) 
    //     return false;
    // }
    return true;
  }

  static std::string getNodeLabel(const DDGNode *Node,
                                  const DataDependenceGraph *DDG) {
    if (auto sN = dyn_cast<SimpleDDGNode>(Node)) {
      auto BB = sN->getFirstInstruction()->getParent();
      auto str = "BB: " + BB->getNameOrAsOperand();
      if (auto depSrc = globalCDG->getControlDependencySource(BB)) 
         str += " (ctrl. dep. on " + depSrc->getNameOrAsOperand() + ")";
      return str;
    }

    return "";
  }

  /// The same as LLVMs dot-ddg
  static std::string
  getEdgeAttributes(const DDGNode *Node,
                    GraphTraits<DDGNode *>::ChildIteratorType I,
                    const DataDependenceGraph *G) {
    const DDGEdge *Edge = static_cast<const DDGEdge *>(*I.getCurrent());

    if (Edge->getKind() == DDGEdge::EdgeKind::RegisterDefUse)
      return "label=\"[def-use]\"";
    else if (Edge->getKind() == DDGEdge::EdgeKind::MemoryDependence)
      return "label=\"[memory]\"";
      // return "label=\"[" + G->getDependenceString(*Node, Edge->getTargetNode()) + "]\"";

    return "";
  }
  
};

struct DDGDotPrinter : PassInfoMixin<DDGDotPrinter> {
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    // Only interested in kernel functions.
    if (F.getCallingConv() == CallingConv::SPIR_FUNC) {
      auto &DI = AM.getResult<DependenceAnalysis>(F);
      auto &PDT = AM.getResult<PostDominatorTreeAnalysis>(F);
      auto &LI = AM.getResult<LoopAnalysis>(F);

      // Our custom printer uses this global variable CDG to label BBs as
      // control dependent.
      globalCDG = std::unique_ptr<ControlDependenceGraph>(
          new ControlDependenceGraph(F, PDT));

      // A DDG for the whole function to show dependencies beytween loops.
      std::unique_ptr<DataDependenceGraph> functionDDG(
          new DataDependenceGraph(F, DI));
      WriteGraph(functionDDG.get(), "DDG_function_sccs");

      // A DDG for each loop. The II of a recurrence will only be affected by
      // dependencies inside of that loop.
      int iLoop = 0;
      for (Loop *TopLevelLoop : LI) {
         for (Loop *L : depth_first(TopLevelLoop)) {
           std::unique_ptr<DataDependenceGraph> DDG(
               new DataDependenceGraph(*L, LI, DI));
           WriteGraph(DDG.get(), "DDG_loop_" + std::to_string(iLoop));
           iLoop++;
         }
      }
    }

    return PreservedAnalyses::all();
  }

  // Without isRequired returning true, this pass will be skipped for functions
  // decorated with the optnone LLVM attribute. Note that clang -O0 decorates
  // all functions with optnone.
  static bool isRequired() { return true; }

  void getAnalysisUsage(AnalysisUsage &AU) const {
    AU.addRequiredID(LoopAnalysis::ID());
    AU.addRequiredID(DependenceAnalysis::ID());
    AU.addRequiredID(PostDominatorTreeAnalysis::ID());
  }
};

//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getDDGDotPrinterPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "DDGDotPrinter", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "dot-ddg-sccs") {
                    FPM.addPass(DDGDotPrinter());
                    return true;
                  }
                  return false;
                });
          }};
}

// This is the core interface for pass plugins. It guarantees that 'opt' will
// find the pass.
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getDDGDotPrinterPluginInfo();
}


} // end namespace llvm