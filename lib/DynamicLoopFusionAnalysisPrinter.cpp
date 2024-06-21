#include "CommonLLVM.h"
#include "DynamicLoopFusionAnalysis.h"

using namespace llvm;


namespace llvm {

using MemoryRequest = DynamicLoopFusionAnalysis::MemoryRequest;
using DecoupledLoopInfo = DynamicLoopFusionAnalysis::DecoupledLoopInfo;
using MemoryDependencyInfo = DynamicLoopFusionAnalysis::MemoryDependencyInfo;
using DepDir = DynamicLoopFusionAnalysis::DepDir;

llvm::raw_ostream &operator<<(llvm::raw_ostream &os, const DepDir &depDir) {
  if (depDir == DepDir::BACK) {
    os << "BACK";
  } else {
    os << "FORWARD";
  }
  return os;
}

std::string getDepInfoStructDef(MemoryDependencyInfo &DepI) {
  std::string res;
  llvm::raw_string_ostream O(res);

  auto print1D = [&O] (auto vec1d) {
    O << "{";
    for (auto elem : vec1d)
      O << elem << ", ";
    O << "}";
  };
  auto print2D = [&print1D, &O] (auto vec2d) {
    O << "{\n    ";
    for (auto vec1d : vec2d) {
      print1D(vec1d);
      O << ", ";
    }
    O << "\n  }";
  };

  O << "\ntemplate <> struct DepInfo<" << DepI.id << "> {\n";
  O << "  static constexpr int NUM_LOADS = " << DepI.numLoads << ";\n";
  O << "  static constexpr int NUM_STORES = " << DepI.numStores << ";\n";
  O << "  static constexpr int MAX_LOOP_DEPTH = " << DepI.maxLoopDepth << ";\n\n";
  
  // Loads
  O << "  static constexpr int LOAD_LOOP_DEPTH[NUM_LOADS] = ";
  print1D(DepI.loadLoopDepth);
  O << ";\n";

  O << "  static constexpr bool LOAD_IS_MAX_ITER_NEEDED[NUM_LOADS][MAX_LOOP_DEPTH] = ";
  print2D(DepI.loadIsMaxIterNeeded);
  O << ";\n";

  O << "  static constexpr bool LOAD_STORE_IN_SAME_LOOP[NUM_LOADS][NUM_STORES] = ";
  print2D(DepI.loadStoreInSameLoop);
  O << ";\n";

  O << "  static constexpr int LOAD_STORE_COMMON_LOOP_DEPTH[NUM_LOADS][NUM_STORES] = ";
  print2D(DepI.loadStoreCommonLoopDepth);
  O << ";\n";

  O << "  static constexpr DEP_DIR LOAD_STORE_DEP_DIR[NUM_LOADS][NUM_STORES] = ";
  print2D(DepI.loadStoreDepDir);
  O << ";\n\n";

  // Stores
  O << "  static constexpr int STORE_LOOP_DEPTH[NUM_STORES] = ";
  print1D(DepI.storeLoopDepth);
  O << ";\n";

  O << "  static constexpr bool STORE_IS_MAX_ITER_NEEDED[NUM_STORES][MAX_LOOP_DEPTH] = ";
  print2D(DepI.storeIsMaxIterNeeded);
  O << ";\n";

  O << "  static constexpr bool STORE_STORE_IN_SAME_LOOP[NUM_STORES][NUM_STORES] = ";
  print2D(DepI.storeStoreInSameLoop);
  O << ";\n";

  O << "  static constexpr int STORE_STORE_COMMON_LOOP_DEPTH[NUM_STORES][NUM_STORES] = ";
  print2D(DepI.storeStoreCommonLoopDepth);
  O << ";\n";

  O << "  static constexpr int STORE_STORE_DEP_DIR[NUM_STORES][NUM_STORES] = ";
  print2D(DepI.storeStoreDepDir);
  O << ";\n\n";

  O << "}; // end DepInfo \n";

  return res;
}

std::string getPipeDefString(MemoryDependencyInfo &MemDep) {
  std::string res;
  llvm::raw_string_ostream O(res);

  constexpr unsigned kReqPipeDepth = 16;

  O << llvm::formatv(
    "using LoadReqPipes_{0} = PipeArray<class _LoadReqPipes_{0}, ld_req_t<{1}, {2}>, {3}, {4}>;\n"
    "using LoadValPipes_{0} = PipeArray<class _LoadValPipes_{0}, {5}, {3}, {4}>;\n"
    "using StoreReqPipes_{0} = PipeArray<class _StoreReqPipes_{0}, st_req_t<{2}>, {3}, {1}, 2>;\n"
    "using StoreValPipes_{0} = PipeArray<class _StoreValPipes_{0}, {5}, {3}, {1}>;\n",
    MemDep.id, MemDep.numStores, MemDep.maxLoopDepth, kReqPipeDepth, MemDep.numLoads, MemDep.cType);

  return res;
}

json::Array getMemoryDependenciesJson(DynamicLoopFusionAnalysis &DLFA) {
  auto Res = json::Array();
  for (auto &[id, memDepInfo] : DLFA.getMemoryDependencyInfo()) {
    json::Object infoJson;
    infoJson["id"] = int(id);
    infoJson["structDef"] = getDepInfoStructDef(memDepInfo);
    infoJson["pipeDefs"] = getPipeDefString(memDepInfo);
    infoJson["cType"] = memDepInfo.cType;
    Res.push_back(std::move(infoJson));
  }

  return Res;
}

json::Array
getPipeCallsInAguJson(DecoupledLoopInfo &DecoupleInfo,
                      MapVector<int, MemoryDependencyInfo> &MemDepInfos) {
  auto Res = json::Array();

  for (auto LdReq : DecoupleInfo.loads) {
    auto MemDep = MemDepInfos[LdReq.memoryId];
    json::Object infoJson;
    std::string pipeCallStr;
    llvm::raw_string_ostream O(pipeCallStr);
    O << llvm::formatv("ld_req_t<{0}, {1}> ld_req_{2}_{3};\n"
                       "ld_req_{2}_{3}.addr = 0u;\n",
                       MemDep.numStores, MemDep.maxLoopDepth, MemDep.id,
                       LdReq.reqId);
    for (int iSt = 0; iSt < MemDep.numStores; ++iSt) {
      O << llvm::formatv("ld_req_{0}_{1}.posDepDist[{2}] = false;\n", MemDep.id,
                         LdReq.reqId, iSt);
    }
    for (int iD = 0; iD < MemDep.maxLoopDepth; ++iD) {
      O << llvm::formatv("ld_req_{0}_{1}.sched[{2}] = 0u;\n"
                         "ld_req_{0}_{1}.isMaxIter[{2}] = false;\n",
                         MemDep.id, LdReq.reqId, iD);
    }
    O << llvm::formatv(
        "LoadReqPipes_{0}::PipeAt<{1}>::write(ld_req_{0}_{1});\n", MemDep.id,
        LdReq.reqId);

    infoJson["instructionIdx"] = getIndexIntoParent(LdReq.memOp);
    infoJson["instructionBasicBlockIdx"] =
        getIndexIntoParent(LdReq.memOp->getParent());
    infoJson["isWrite"] = true;
    infoJson["pipeCall"] = pipeCallStr;
    Res.push_back(std::move(infoJson));
  }

  for (auto StReq : DecoupleInfo.stores) {
    auto MemDep = MemDepInfos[StReq.memoryId];

    json::Object infoJson;
    std::string pipeCallStr;
    llvm::raw_string_ostream O(pipeCallStr);
    O << llvm::formatv("st_req_t<{0}> st_req_{1}_{2};\n"
                       "st_req_{1}_{2}.addr = 0u;\n",
                       MemDep.maxLoopDepth, MemDep.id, StReq.reqId);
    for (int iD = 0; iD < MemDep.maxLoopDepth; ++iD) {
      O << llvm::formatv("st_req_{0}_{1}.sched[{2}] = 0u;\n"
                         "st_req_{0}_{1}.isMaxIter[{2}] = false;\n",
                         MemDep.id, StReq.reqId, iD);
    }
    O << llvm::formatv(
        "StoreReqPipes_{0}::PipeAt<{1}, 0>::write(st_req_{0}_{1});\n"
        "StoreReqPipes_{0}::PipeAt<{1}, 1>::write(st_req_{0}_{1});\n",
        MemDep.id, StReq.reqId);

    infoJson["instructionIdx"] = getIndexIntoParent(StReq.memOp);
    infoJson["instructionBasicBlockIdx"] =
        getIndexIntoParent(StReq.memOp->getParent());
    infoJson["isWrite"] = true;
    infoJson["pipeCall"] = pipeCallStr;
    Res.push_back(std::move(infoJson));
  }

  return Res;
}

json::Array
getPipeCallsInComputeJson(DecoupledLoopInfo &DecoupleInfo,
                      MapVector<int, MemoryDependencyInfo> &MemDepInfos) {
  auto Res = json::Array();

  for (auto &LdReq : DecoupleInfo.loads) {
    auto MemDep = MemDepInfos[LdReq.memoryId];

    json::Object infoJson;
    std::string pipeCallStr;
    llvm::raw_string_ostream O(pipeCallStr);
    O << llvm::formatv("[[maybe_unused]] auto _ldVal_{0}_{1} = "
                       "LoadValPipes_{0}::PipeAt<{1}>::read();\n",
                       MemDep.id, LdReq.reqId);

    infoJson["memoryId"] = LdReq.memoryId;
    infoJson["loopId"] = LdReq.loopId;
    infoJson["reqId"] = LdReq.reqId;
    infoJson["instructionIdx"] = getIndexIntoParent(LdReq.memOp);
    infoJson["instructionBasicBlockIdx"] =
        getIndexIntoParent(LdReq.memOp->getParent());
    infoJson["isWrite"] = false;
    infoJson["pipeCall"] = pipeCallStr;
    Res.push_back(std::move(infoJson));
  }

  for (auto &StReq : DecoupleInfo.stores) {
    auto MemDep = MemDepInfos[StReq.memoryId];

    json::Object infoJson;
    std::string pipeCallStr;
    llvm::raw_string_ostream O(pipeCallStr);
    O << llvm::formatv("StoreValPipes_{0}::PipeAt<{1}>::write(0);\n", MemDep.id,
                       StReq.reqId);

    infoJson["memoryId"] = StReq.memoryId;
    infoJson["loopId"] = StReq.loopId;
    infoJson["reqId"] = StReq.reqId;
    infoJson["instructionIdx"] = getIndexIntoParent(StReq.memOp);
    infoJson["instructionBasicBlockIdx"] =
        getIndexIntoParent(StReq.memOp->getParent());
    infoJson["isWrite"] = true;
    infoJson["pipeCall"] = pipeCallStr;
    Res.push_back(std::move(infoJson));
  }

  return Res;
}

json::Array getLoopsToDecoupleJson(DynamicLoopFusionAnalysis &DLFA,
                                   LoopInfo &LI, const std::string &kernelName) {
  auto Res = json::Array();

  auto memDepInfo = DLFA.getMemoryDependencyInfo();
  for (auto [iK, decoupleInfo] : llvm::enumerate(DLFA.getAgusToDecouple())) {
    json::Object info;
    info["type"] = "agu";
    info["id"] = decoupleInfo.id;
    info["kernelName"] =
        (iK == 0) ?  kernelName + "_agu"
                  :  kernelName + "_agu_" + std::to_string(decoupleInfo.id);
    info["innerLoopBlockId"] =
        getIndexIntoParent(decoupleInfo.inner()->getHeader());
    info["pipeCalls"] = getPipeCallsInAguJson(decoupleInfo, memDepInfo);
    Res.push_back(std::move(info));
  }
  for (auto [iK, decoupleInfo] : llvm::enumerate(DLFA.getLoopsToDecouple())) {
    json::Object info;
    info["type"] = "compute";
    info["id"] = decoupleInfo.id;
    info["kernelName"] =
        (iK == 0) ? kernelName
                  : kernelName + "_" + std::to_string(decoupleInfo.id);
    info["innerLoopBlockId"] =
        getIndexIntoParent(decoupleInfo.inner()->getHeader());
    info["pipeCalls"] = getPipeCallsInComputeJson(decoupleInfo, memDepInfo);
    Res.push_back(std::move(info));
  }

  return Res;
}

struct DynamicLoopFusionAnalysisPrinter
    : PassInfoMixin<DynamicLoopFusionAnalysisPrinter> {

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    if (F.getCallingConv() == CallingConv::SPIR_KERNEL) {
      auto &LI = AM.getResult<LoopAnalysis>(F);
      auto &SE = AM.getResult<ScalarEvolutionAnalysis>(F);
      auto *DLFA = new DynamicLoopFusionAnalysis(LI, SE);

      const std::string kernelName = demangle(std::string(F.getName()));
  
      json::Object report;
      report["mainKernelName"] = kernelName;
      report["kernelStartLine"] = F.getSubprogram()->getLine();
      report["memoryToProtect"] = getMemoryDependenciesJson(*DLFA);
      report["loopsToDecouple"] = getLoopsToDecoupleJson(*DLFA, LI, kernelName);

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
    AU.addRequiredID(ScalarEvolutionAnalysis::ID());
  }
};

//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getDynamicLoopFusionAnalysisPrinterPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "DynamicLoopFusionAnalysisPrinter",
          LLVM_VERSION_STRING, [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "dynamic-loop-fusion-analysis") {
                    FPM.addPass(DynamicLoopFusionAnalysisPrinter());
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
  return getDynamicLoopFusionAnalysisPrinterPluginInfo();
}

} // end namespace llvm
