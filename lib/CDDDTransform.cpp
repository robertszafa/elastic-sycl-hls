#include "CommonLLVM.h"

using namespace llvm;

namespace llvm {

const std::string ENVIRONMENT_VARIBALE_REPORT = "CDDD_REPORT_FILE";

/// Collect mappings between instructions (loads and stores) in {F}, and sycl
/// pipe call instructions, based on the info from the json report.
void collectPipe2InstMappings(json::Object &report, Function &F, bool isMain,
                              SmallVector<Pipe2Inst> &depIn,
                              SmallVector<Pipe2Inst> &depOut,
                              SmallVector<CallInst *> &predPipes) {
  for (json::Value &bottleneck : *report["bottlenecks"].getAsArray()) {
    auto bottleneckOb = *bottleneck.getAsObject();
    // If looking for pipes for a specific kernel copy, then ignore the rest.
    auto copyName = bottleneckOb["kernel_copy_name"].getAsString().getValue();
    if (!isMain && copyName != getKernelName(F))
      continue;

    auto inDepsArray = *bottleneckOb["dependencies_in"].getAsArray();
    auto outDepsArray = *bottleneckOb["dependencies_out"].getAsArray();
    llvm::append_range(depIn, getPipe2InstMaps(F, inDepsArray));
    llvm::append_range(depOut, getPipe2InstMaps(F, outDepsArray));

    auto predPipeObject = *bottleneckOb["pred_pipe"].getAsObject();
    predPipes.push_back(getPipeCall(F, predPipeObject));
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

    // Collect mappings between bottleneck instructions and the pipe read/writes
    // that will replace them. The main kernel needs to know all of these
    // mappings, whereas the kernels where individsual bottlenecks are decoupled
    // into need to only know about their mappings.
    SmallVector<Pipe2Inst> depIn, depOut;
    SmallVector<CallInst *> predPipes;
    collectPipe2InstMappings(report, F, isMain, depIn, depOut, predPipes);

    errs() << "\n\n-- isMain? " << isMain << "\n";

    // TODO: look at depIn, depOut
    errs() << "\nDepIn:\n";
    for (auto &in : depIn) {
      errs() << "\nI: ";
      in.second->print(errs());
    }
    errs() << "\n\nDepOut:\n";
    for (auto &out : depOut) {
      errs() << "\nI: ";
      out.second->print(errs());
    }
    errs() << "\n\nPredPipe:\n";
    predPipes[0]->print(errs());

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
