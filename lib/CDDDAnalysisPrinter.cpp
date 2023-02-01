#include "CommonLLVM.h"
#include "CDDDAnalysis.h"
#include "CDG.h"
#include "llvm/Analysis/DependenceAnalysis.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Support/raw_ostream.h"

#include <regex>
#include <string>
#include <fstream>
#include <sstream>

using namespace llvm;

namespace llvm {

/// Given a source file line in function {F}, return the corresponding
/// LLVM instruction using DebugLoc metadata. If there are multiple such 
/// instructions, return the last one based on the post dominance relation.
Instruction *getInstructionForFileLine(Function &F, PostDominatorTree &PDT,
                                       unsigned int line) {
  Instruction *result = nullptr;

  for (auto &BB : F) {
    for (auto &I : BB) {
      if (I.getDebugLoc()->getLine() == line) {
        if (result == nullptr || PDT.dominates(&I, result))
          result = &I;
      }
    }
  }

  return result;
}

/// Return lines from getenv(BOTTLENECK_LINES_FILE) as a vector of ints.
SmallVector<unsigned int> parseBottleneckFile() {
  SmallVector<unsigned int> result;

  if (const char *fname = std::getenv("BOTTLENECK_LINES_FILE")) {
    std::ifstream infile(fname);
    int line;
    while (infile >> line)
      result.push_back(line);
  }

  return result;
}

json::Object
generateReport(Function &F,
               SmallVector<SmallVector<Instruction *>> &dependenciesIn,
               SmallVector<SmallVector<Instruction *>> &dependenciesOut) {
  // TODO
  json::Object report;
  return report;
}

struct CDDDAnalysisPrinter : PassInfoMixin<CDDDAnalysisPrinter> {

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    if (F.getCallingConv() == CallingConv::SPIR_FUNC) {
      SmallVector<unsigned int> bottlenecLines = parseBottleneckFile();
      assert(bottlenecLines.size() > 0 && "No bottlecks.");

      auto &PDT = AM.getResult<PostDominatorTreeAnalysis>(F);
      auto &LI = AM.getResult<LoopAnalysis>(F);
      std::unique_ptr<ControlDependenceGraph> CDG(
          new ControlDependenceGraph(F, PDT));

      SmallVector<Instruction *> ctrlDepBottlenecks;

      // For each bottleneck, check if it's a control dependent data dependency.
      for (unsigned int line : bottlenecLines) {
        auto bottleneckI = getInstructionForFileLine(F, PDT, line);
        auto CDDD = new ControlDependentDataDependencyAnalysis(F, *CDG, LI,
                                                               bottleneckI);

        if (CDDD->isCtrlDepInsideLoop()) {
          errs() << "Ctrl dep src block "
                 << CDDD->getCtrlDepSrcBlock()->getNameOrAsOperand() << "\n";

          ctrlDepBottlenecks.push_back(bottleneckI);
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
    AU.addRequiredID(PostDominatorTreeAnalysis::ID());
  }
};

//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getCDDDAnalysisPrinterPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "CDDDAnalysisPrinter",
          LLVM_VERSION_STRING, [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "cddd-bottlenecks") {
                    FPM.addPass(CDDDAnalysisPrinter());
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
  return getCDDDAnalysisPrinterPluginInfo();
}

} // end namespace llvm