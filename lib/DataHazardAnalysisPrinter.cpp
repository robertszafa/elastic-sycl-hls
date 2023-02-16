#include "CommonLLVM.h"
#include "DataHazardAnalysis.h"

using namespace llvm;

namespace llvm {

/// Return a json object recording the data hazard analysis result.
json::Object genReport(Function &F, DataHazardAnalysis &DHA) {
  json::Object report;
  auto callers = getCallerFunctions(F.getParent(), F);

  // A spir_func lambda is called only once from one kernel.
  if (callers.size() == 1) {
    report["kernel_class_name"] = demangle(std::string(callers[0]->getName()));
    report["spir_func_name"] = demangle(std::string(F.getName()));
    report["kernel_start_line"] = callers[0]->getSubprogram()->getLine();
    report["kernel_end_line"] = getReturnLine(F);

    json::Array base_addresses;
    // For each instruction cluster, create a 'base_address' json object
    // that describes the hazardous instructions, decoupling decision, types...
    for (size_t iC = 0; iC < DHA.getResult().size(); ++iC) {
      auto instrCluster = DHA.getResult()[iC];

      SmallVector<Instruction *> loads = getLoads(instrCluster);
      SmallVector<Instruction *> stores = getStores(instrCluster);

      llvm::json::Object thisBaseAddr;
      thisBaseAddr["num_loads"] = loads.size();
      thisBaseAddr["num_stores"] = stores.size();

      thisBaseAddr["decouple_address"] = DHA.getDecoupligDecisions()[iC];

      std::string typeStr;
      llvm::raw_string_ostream rso(typeStr);
      instrCluster[0]->getOperand(0)->getType()->print(rso);
      thisBaseAddr["array_type"] = typeStr;

      json::Array loadIs;
      for (auto &iLd : loads) 
        loadIs.push_back(std::move(genJsonForInstruction(iLd)));
      thisBaseAddr["load_instructions"] = std::move(loadIs);

      json::Array storeIs;
      for (auto &iSt : stores) 
        storeIs.push_back(std::move(genJsonForInstruction(iSt)));
      thisBaseAddr["store_instructions"] = std::move(storeIs);

      base_addresses.push_back(std::move(thisBaseAddr));
    }
    report["base_addresses"] = std::move(base_addresses);
  }

  return report;
}

void dataHazardPrinter(Function &F, LoopInfo &LI, ScalarEvolution &SE,
                       PostDominatorTree &PDT) {
  // Get all memory loads and stores that form a RAW hazard dependence.
  auto *DHA = new DataHazardAnalysis(F, LI, SE, PDT);

  if (DHA->getResult().size() > 0) {
    json::Object report = genReport(F, *DHA);

    // Print json report to std::out.
    outs() << formatv("{0:2}", json::Value(std::move(report))) << "\n";

    // Print quick info to dbgs() stream.
    dbgs() << "\n--- Data Hazard Report ------------------------------------\n";
    for (size_t iC = 0; iC < DHA->getResult().size(); ++iC) {
      SmallVector<Instruction *> stores = getStores(DHA->getResult()[iC]);
      SmallVector<Instruction *> loads = getLoads(DHA->getResult()[iC]);

      dbgs() << "Address " << iC << ": " << stores.size() << " stores, "
             << loads.size() << " loads, address gen "
             << (DHA->getDecoupligDecisions()[iC] ? "is" : "is NOT")
             << " decoupled.\n";
    }
    dbgs() << "-----------------------------------------------------------\n\n";
  } else {
    errs() << "Warning: Report not generated - no RAW hazards.\n";
  }
}

/// Generate a report for memory instructions that form 
/// a RAW inter-iteration data hazard.
struct DataHazardAnalysisPrinter : PassInfoMixin<DataHazardAnalysisPrinter> {

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    if (F.getCallingConv() == CallingConv::SPIR_FUNC) {
      auto &LI = AM.getResult<LoopAnalysis>(F);
      auto &SE = AM.getResult<ScalarEvolutionAnalysis>(F);
      auto &PDT = AM.getResult<PostDominatorTreeAnalysis>(F);
      dataHazardPrinter(F, LI, SE, PDT);
    }

    return PreservedAnalyses::all();
  }

  // Without isRequired returning true, this pass will be skipped for functions
  // decorated with the optnone LLVM attribute. Note that clang -O0 decorates
  // all functions with optnone.
  static bool isRequired() { return true; }

  void getAnalysisUsage(AnalysisUsage &AU) const {
    AU.addRequiredID(LoopAnalysis::ID());
    AU.addRequiredID(ScalarEvolutionAnalysis::ID());
    AU.addRequiredID(PostDominatorTreeAnalysis::ID());
  }
};

//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getDataHazardAnalysisPrinterPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "DataHazardAnalysisPrinter",
          LLVM_VERSION_STRING, [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "data-hazard-report") {
                    FPM.addPass(DataHazardAnalysisPrinter());
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
  return getDataHazardAnalysisPrinterPluginInfo();
}

} // end namespace llvm