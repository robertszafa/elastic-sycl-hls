#include "CommonLLVM.h"
#include "CDDDAnalysis.h"
#include "CDG.h"

using namespace llvm;

namespace llvm {

/// Given a vector of instruction chains, return all unique Basic Blocks.
SmallVector<BasicBlock *>
getBBsFromChains(SmallVector<SmallVector<Instruction *>> &instructionChain,
                 PostDominatorTree &PDT) {
  SmallVector<BasicBlock *> result;

  for (auto chain : instructionChain) {
    for (auto I : chain) {
      if (!is_contained(result, I->getParent()))
        result.push_back(I->getParent());
    }
  }

  return result;
}

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

/// Given a vector of vectors of lines {instructionChain} return a vector of
/// vectors of instructions, where each instruction corresponds to the line.
SmallVector<SmallVector<Instruction *>>
getInstructionChains(Function &F, PostDominatorTree &PDT,
                     SmallVector<SmallVector<unsigned int>> &instructionChain) {
  SmallVector<SmallVector<Instruction *>> result;
  for (auto chain : instructionChain) {
    SmallVector<Instruction *> thisInstructionChain;
    for (auto line : chain) {
      if (auto I = getInstructionForFileLine(F, PDT, line))
        thisInstructionChain.push_back(I);
    }
    result.push_back(thisInstructionChain);
  }

  return result;
}

/// Return lines from getenv(BOTTLENECK_LINES_FILE) as a vector of ints.
SmallVector<SmallVector<unsigned int>> parseBottleneckFile() {
  SmallVector<SmallVector<unsigned int>> result(1);

  if (const char *fname = std::getenv("BOTTLENECK_LINES_FILE")) {
    std::ifstream infile(fname);
    int line;
    int i = 0;
    while (infile >> line) {
      if (infile.peek() == '\n') {
        infile.ignore();
        i++;
        result.push_back(SmallVector<unsigned int>());
      }
      else if (infile.peek() == ' ') {
        infile.ignore();
      }

      result[i].push_back(line);
    }
  }

  if (result[result.size() - 1].size() == 0)
    result.erase(&result[result.size() - 1]);

  return result;
}

json::Object
generateReport(Function &F, SmallVector<BasicBlock *> &bottleneckBBs,
               SmallVector<SmallVector<Instruction *>> &dependenciesIn,
               SmallVector<SmallVector<Instruction *>> &dependenciesOut) {
  json::Object report;

  /// Given an Instruction, return a JSON object:
  ///   {'instruction': {'instr_idx': \d, 'bb_idx': \d}, 'type': I.getType() }.
  auto getJsonForInstructionAndAddType = [](Instruction *I) -> json::Object {
    json::Object res;
    res["instruction"] = genJsonForInstruction(I);

    std::string typeStr;
    llvm::raw_string_ostream rso(typeStr);
    I->getType()->print(rso);
    res["type"] = typeStr;

    return res;
  };

  if (bottleneckBBs.size() > 0) {
    auto callers = getCallerFunctions(F.getParent(), F);
    report["kernel_class_name"] = demangle(std::string(callers[0]->getName()));
    report["spir_func_name"] = demangle(std::string(F.getName()));
    report["kernel_start_line"] = callers[0]->getSubprogram()->getLine();
    report["kernel_end_line"] = getReturnLine(F);

    json::Array kernelDependencies;
    for (size_t i = 0; i < bottleneckBBs.size(); ++i) {
      json::Array thisKernelDepsIn;
      json::Array thisKernelDepsOut;
      llvm::transform(dependenciesIn[i], std::back_inserter(thisKernelDepsIn),
                      getJsonForInstructionAndAddType);
      llvm::transform(dependenciesOut[i], std::back_inserter(thisKernelDepsOut),
                      getJsonForInstructionAndAddType);

      json::Object thisKernelOb;
      thisKernelOb["dependencies_in"] = std::move(thisKernelDepsIn);
      thisKernelOb["dependencies_out"] = std::move(thisKernelDepsOut);
      thisKernelOb["id"] = i;
      thisKernelOb["basic_block_idx"] =
          getIndexOfChild(bottleneckBBs[i]->getParent(), bottleneckBBs[i]);

      kernelDependencies.push_back(std::move(thisKernelOb));
    }

    report["bottlenecks"] = std::move(kernelDependencies);
  }

  return report;
}

struct CDDDAnalysisPrinter : PassInfoMixin<CDDDAnalysisPrinter> {

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    if (F.getCallingConv() == CallingConv::SPIR_FUNC) {
      // For each bottleneck in the report, collect instruction lines.
      SmallVector<SmallVector<unsigned int>> bottleneckChainLines =
          parseBottleneckFile();
      assert(bottleneckChainLines.size() > 0 && "No bottlenecks.");

      auto &PDT = AM.getResult<PostDominatorTreeAnalysis>(F);
      auto &LI = AM.getResult<LoopAnalysis>(F);
      std::unique_ptr<ControlDependenceGraph> CDG(
          new ControlDependenceGraph(F, PDT));

      // For each bottleneck, collect the instructions corresponding to lines.
      SmallVector<SmallVector<Instruction *>> instrBottleneckChains =
          getInstructionChains(F, PDT, bottleneckChainLines);
      // For all bottleneck, collect basic blocks containing above instructions.
      SmallVector<BasicBlock *> bottleneckBBs =
          getBBsFromChains(instrBottleneckChains, PDT);
      // Ensure the bottleneck BBs start from the end.
      llvm::sort(bottleneckBBs,
                 [&PDT](auto b1, auto b2) { return !PDT.dominates(b2, b1); });

      // For each bottleneck, check if it's a control dependent data dependency.
      // If yes, collect in/out SSA values for that block.
      SmallVector<BasicBlock *> ctrlDepBottlenecks;
      SmallVector<SmallVector<Instruction *>> dependenciesIn, dependenciesOut;
      for (auto bottleneckBB : bottleneckBBs) {
        auto CDDD = new ControlDependentDataDependencyAnalysis(F, *CDG, LI,
                                                               bottleneckBB);
        if (CDDD->isCtrlDepInsideLoop()) {
          ctrlDepBottlenecks.push_back(bottleneckBB);
          dependenciesIn.push_back(CDDD->getDependenciesIn());
          dependenciesOut.push_back(CDDD->getDependenciesOut());
        }
      }

      auto report = generateReport(F, ctrlDepBottlenecks, dependenciesIn,
                                   dependenciesOut);
      
      // Print report to stdout to be picked up by later tools.
      outs() << formatv("{0:2}", json::Value(std::move(report))) << "\n";
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