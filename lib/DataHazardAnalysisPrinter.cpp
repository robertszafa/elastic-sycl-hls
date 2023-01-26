#include "CommonLLVM.h"
#include "DataHazardAnalysis.h"

#include <regex>
#include <string>

using namespace llvm;

namespace llvm {

/// Return a json object recording the data hazard analysis result.
///
// Example report for histogram_if:
// {
//   "base_addresses": [
//     {
//       "array_type": "float",
//       "num_loads": 1,
//       "num_stores": 1
//     }
//   ],
//   "decouple_address": 1,
//   "kernel_class_name": "typeinfo name for MainKernel",
//   "spir_func_name": "histogram_if_kernel(cl::sycl::queue&, ...)"
// }
json::Object
generateReport(Function &F,
               SmallVector<SmallVector<Instruction *>> &memInstrsAll) {
  json::Object report;
  auto callers = getCallerFunctions(F.getParent(), F);

  // A spir_func lambda is called only once from one kernel.
  if (callers.size() == 1) {
    report["kernel_class_name"] = demangle(std::string(callers[0]->getName()));
    report["spir_func_name"] = demangle(std::string(F.getName()));
    json::Array base_addresses;
    for (auto &memInstrs : memInstrsAll) {
      llvm::json::Object thisBaseAddr;
      thisBaseAddr["num_loads"] = llvm::count_if(memInstrs, isaLoad);
      thisBaseAddr["num_stores"] = llvm::count_if(memInstrs, isaStore);
      // TODO: deal with different types
      std::string typeStr;
      llvm::raw_string_ostream rso(typeStr);
      memInstrs[0]->getOperand(0)->getType()->print(rso);
      thisBaseAddr["array_type"] = typeStr;
      // int(memInstrs[0]->getOperand(0)->getType()->getPrimitiveSizeInBits());

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
    json::Object report = generateReport(F, hazardInstrs);

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