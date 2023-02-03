#include "CommonLLVM.h"
#include "DataHazardAnalysis.h"

#include <regex>
#include <string>

using namespace llvm;

namespace llvm {

/// Given an instruction, return a json object with its description. E.g.:
///   {"basic_block_idx": 8, "instruction_idx": 9}
json::Object genJsonForInstruction(Instruction *I) {
  llvm::json::Object obj;
  auto iBB = I->getParent();
  obj["basic_block_idx"] = getIndexOfChild(iBB->getParent(), iBB);
  obj["instruction_idx"] = getIndexOfChild(iBB, I);

  return obj;
}

/// Return a json object recording the data hazard analysis result.
json::Object genReport(Function &F,
                       SmallVector<SmallVector<Instruction *>> &iClustsers) {
  json::Object report;
  auto callers = getCallerFunctions(F.getParent(), F);

  // A spir_func lambda is called only once from one kernel.
  if (callers.size() == 1) {
    report["kernel_class_name"] = demangle(std::string(callers[0]->getName()));
    report["spir_func_name"] = demangle(std::string(F.getName()));

    json::Array base_addresses;
    // For each instruction cluster, create a 'base_address' json object
    // that describes the hazardous instructions, decoupling decision, types...
    for (auto &instrCluster : iClustsers) {
      SmallVector<Instruction *> loads = getLoads(instrCluster);
      SmallVector<Instruction *> stores = getStores(instrCluster);

      llvm::json::Object thisBaseAddr;
      thisBaseAddr["num_loads"] = loads.size();
      thisBaseAddr["num_stores"] = stores.size();

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
                       DominatorTree &DT) {
  // Get all memory loads and stores that form a RAW hazard dependence.
  auto *DHA = new DataHazardAnalysis(F, LI, SE, DT);
  auto hazardInstrs = DHA->getResult();
  auto decouplingDecisions = DHA->getDecoupligDecisions();

  if (hazardInstrs.size() > 0) {
    json::Object report = genReport(F, hazardInstrs);

    // TODO: Treat each base address decouplig separately.
    int canDecoupleAddress =
        int(llvm::all_of(decouplingDecisions, [](auto e) { return e; }));
    report["decouple_address"] = canDecoupleAddress;

    // Print json report to std::out.
    outs() << formatv("{0:2}", json::Value(std::move(report))) << "\n";

    // Print quick info to dbgs() stream.
    dbgs() << "\n************* Data Hazard Report *************\n";
    dbgs() << "Number of base addresses: " << hazardInstrs.size() << "\n";
    dbgs() << "Decoupled address gen: " << canDecoupleAddress << "\n";
    for (auto &memInstructions : hazardInstrs) {
      SmallVector<Instruction *> stores = getStores(memInstructions);
      SmallVector<Instruction *> loads = getLoads(memInstructions);

      dbgs() << "\n-------------------------\nStores " << stores.size()
             << ":\n";
      for (auto &si : stores) {
        si->print(dbgs());
        dbgs() << "\n";
      }
      dbgs() << "\nLoads " << loads.size() << ":\n";
      for (auto &li : loads) {
        li->print(dbgs());
        dbgs() << "\n";
      }
    }
    dbgs() << "************* Data Hazard Report *************\n\n";
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
      auto &DT = AM.getResult<DominatorTreeAnalysis>(F);
      dataHazardPrinter(F, LI, SE, DT);
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
    AU.addRequiredID(DominatorTreeAnalysis::ID());
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