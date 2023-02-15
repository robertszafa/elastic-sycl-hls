#include "CommonLLVM.h"

using namespace llvm;

namespace llvm {

const std::string ENVIRONMENT_VARIBALE_REPORT = "CDDD_REPORT_FILE";

/// Collect mappings between instructions (loads and stores) in {F}, and sycl
/// pipe call instructions, based on the info from the json report.
void collectPipe2InstMappings(json::Object &report, Function &F,
                              bool isMainKernel, SmallVector<Pipe2Inst> &depIn,
                              SmallVector<Pipe2Inst> &depOut) {
  for (json::Value &bottleneck : *report["bottlenecks"].getAsArray()) {
    auto bottleneckOb = *bottleneck.getAsObject();
    // If looking for pipes for a specific kernel copy: 
    auto copyName = bottleneckOb["kernel_copy_name"].getAsString().getValue();
    if (!isMainKernel && copyName != getKernelName(F))
      continue;

    llvm::append_range(
        depIn,
        getPipe2InstMaps(F, *bottleneckOb["dependencies_in"].getAsArray()));
    llvm::append_range(
        depOut,
        getPipe2InstMaps(F, *bottleneckOb["dependencies_out"].getAsArray()));
  }
}

struct CDDDTransform : PassInfoMixin<CDDDTransform> {
  json::Object report;

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    // Read in report once.
    if (report.empty())
      report = *parseJsonReport(ENVIRONMENT_VARIBALE_REPORT).getAsObject();

    // Determine if this is our original kernel, a copy kernel, or neither.
    std::string thisKernelName = getKernelName(F);
    auto mainKernel =
        std::string(report["kernel_name"].getAsString().getValue());
    bool isMain = mainKernel == thisKernelName;
    bool isOurKernel = std::equal(mainKernel.begin(), mainKernel.end(),
                                  thisKernelName.begin());
    if (F.getCallingConv() != CallingConv::SPIR_FUNC || !isOurKernel) {
      return PreservedAnalyses::all();
    }

    // Collect mappings between ld/st instructions and the pipe read/writes that
    // will replace them. Also collect all hazard strores for any base address.
    SmallVector<Pipe2Inst> depIn, depOut;
    collectPipe2InstMappings(report, F, isMain, depIn, depOut);

    return PreservedAnalyses::none();
  }

  // Without isRequired returning true, this pass will be skipped for functions
  // decorated with the optnone LLVM attribute. Note that clang -O0 decorates
  // all functions with optnone.
  static bool isRequired() { return true; }

  void getAnalysisUsage(AnalysisUsage &AU) const {
    AU.addRequiredID(DominatorTreeAnalysis::ID());
  }
};

//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getCDDDTransformPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "CDDDTransform", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "cddd-transform") {
                    FPM.addPass(CDDDTransform());
                    return true;
                  }
                  return false;
                });
          }};
}

// This is the core interface for pass plugins. It guarantees that 'opt' will
// be able to recognize the pass via '-passes=stq-insert'
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getCDDDTransformPluginInfo();
}

} // end namespace llvm
