
#include "CommonLLVM.h"
#include "CDDDAnalysis.h"
#include "CDG.h"
#include "llvm/Support/raw_ostream.h"

#include <cassert>
#include <fstream>
#include <sstream>

using namespace llvm;

namespace llvm {

SmallVector<Instruction *> getInstructionsForFileLine(Function &F,
                                                      unsigned int line) {
  SmallVector<Instruction *> result;

  for (auto &BB : F) {
    for (auto &I : BB) {
      if (I.getDebugLoc()->getLine() == line)
        result.push_back(&I);
    }
  }

  return result;
}

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

ControlDependentDataDependencyAnalysis::ControlDependentDataDependencyAnalysis(
    Function &F, ControlDependenceGraph &CDG) {
  auto bottlenecLines = parseBottleneckFile();
  assert(bottlenecLines.size() > 0 && "No bottlecks.");

  for (unsigned int line : bottlenecLines) {
    auto bottleneckIs = getInstructionsForFileLine(F, line);
    errs() << "Instructions for line " << line << "\n";
    for (auto &I : bottleneckIs) {
      I->print(errs());
      errs() << "\n";
    }
    errs() << "\n";
  }
}

} // end namespace llvm
