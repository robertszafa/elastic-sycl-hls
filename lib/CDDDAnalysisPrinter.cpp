#include "CommonLLVM.h"
#include "CDDDAnalysis.h"
#include "CDG.h"

using namespace llvm;

namespace llvm {

json::Object
generateReport(Function &F, SmallVector<BasicBlock *> &blocksToDecouple,
               SmallVector<SmallVector<Instruction *>> &incomingUses,
               SmallVector<SmallVector<Instruction *>> &outgoingDefs) {
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

  if (blocksToDecouple.size() > 0) {
    report["kernel_name_full"] = demangle(std::string(F.getName()));
    report["kernel_start_line"] = F.getSubprogram()->getLine();

    json::Array blocksArray;
    for (size_t iB = 0; iB < blocksToDecouple.size(); ++iB) {
      json::Array incomingUsesArray;
      json::Array outgoingDefsArray;
      llvm::transform(incomingUses[iB], std::back_inserter(incomingUsesArray),
                      getJsonForInstructionAndAddType);
      llvm::transform(outgoingDefs[iB], std::back_inserter(outgoingDefsArray),
                      getJsonForInstructionAndAddType);

      json::Object thisBlockOb;
      thisBlockOb["incoming_uses"] = std::move(incomingUsesArray);
      thisBlockOb["outgoing_defs"] = std::move(outgoingDefsArray);
      thisBlockOb["id"] = iB;
      thisBlockOb["basic_block_idx"] = getIndexOfChild(
          blocksToDecouple[iB]->getParent(), blocksToDecouple[iB]);

      blocksArray.push_back(std::move(thisBlockOb));
    }

    report["blocks_to_decouple"] = std::move(blocksArray);
  }

  return report;
}

struct CDDDAnalysisPrinter : PassInfoMixin<CDDDAnalysisPrinter> {

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    if (F.getCallingConv() == CallingConv::SPIR_KERNEL) {
      // Use a Set since the same BB could be collected from multiple SCCs.
      ControlDependentDataDependencyAnalysis::BlockList blocksToDecouple;
      SmallVector<SmallVector<Instruction *>> incomingUses, outgoingDefs;

      auto &LI = AM.getResult<LoopAnalysis>(F);
      auto &DI = AM.getResult<DependenceAnalysis>(F);
      auto &PDT = AM.getResult<PostDominatorTreeAnalysis>(F);
      std::shared_ptr<ControlDependenceGraph> CDG(
          new ControlDependenceGraph(F, PDT));

      int recurrenceId = 0;
      
      // TODO: Go over individual loops?
      // for (Loop *TopLevelLoop : LI) {
        // for (Loop *L : depth_first(TopLevelLoop)) {

      // Get a DDG for the whole function.
      // Look at all recurrences in F. If a recurrence is estimated to increase
      // a loop II, check if some blocks on its path are control dependent and
      // could be decoupled such that II is lowered.
      std::unique_ptr<DataDependenceGraph> DDG(new DataDependenceGraph(F, DI));
      for (auto nodeDDG : *DDG) {
        if (auto loopSCC = dyn_cast<PiBlockDDGNode>(nodeDDG)) {
          auto *CDDD = new ControlDependentDataDependencyAnalysis(*loopSCC,
                                                                  LI, *CDG);
          for (auto block : CDDD->getBlocksToDecouple())
            blocksToDecouple.insert(block);
          llvm::append_range(incomingUses, CDDD->getIncomingUses());
          llvm::append_range(outgoingDefs, CDDD->getOutgoingDefs());

          CDDD->print(dbgs(), recurrenceId);
          recurrenceId++;
        }
      }

      auto blocksVec = blocksToDecouple.takeVector();
      auto report = generateReport(F, blocksVec, incomingUses, outgoingDefs);

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
    AU.addRequiredID(DependenceAnalysis::ID());
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
                  if (Name == "cddd-analysis") {
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