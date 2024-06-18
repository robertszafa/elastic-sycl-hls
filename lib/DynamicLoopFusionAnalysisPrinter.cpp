#include "CommonLLVM.h"
#include "DynamicLoopFusionAnalysis.h"

using namespace llvm;


namespace llvm {

using MemoryRequest = DynamicLoopFusionAnalysis::MemoryRequest;
using DecoupledLoopInfo = DynamicLoopFusionAnalysis::DecoupledLoopInfo;
using MemoryDependencyInfo = DynamicLoopFusionAnalysis::MemoryDependencyInfo;
using DepDir = DynamicLoopFusionAnalysis::DepDir;

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
    "using LoadAddrPipes_{0} = PipeArray<class _LoadAddr_{0}, ld_req_t<{1}, {2}>, {3}, {4}>;\n"
    "using LoadValPipes_{0} = PipeArray<class _LoadVal_{0}, {5}, {3}, {4}>;\n"
    "using StoreAddrPipes_{0} = PipeArray<class _StoreAddr_{0}, st_req_t<{2}>, {3}, {1}, 2>;\n"
    "using StoreValPipes_{0} = PipeArray<class _StoreVal_{0}, {5}, {3}, {1}>;\n",
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
    Res.push_back(std::move(infoJson));
  }

  return Res;
}

json::Array
getPipeCallsInAguJson(DecoupledLoopInfo &DecoupleInfo,
                      MapVector<int, MemoryDependencyInfo> &MemDepInfos) {
  auto Res = json::Array();

  for (auto &LdReq : DecoupleInfo.loads) {
    auto MemDep = MemDepInfos[LdReq.memoryId];
    int ReqId = MemDep.loadReqIds[LdReq.memOp];

    json::Object infoJson;
    std::string pipeCallStr;
    llvm::raw_string_ostream O(pipeCallStr);
    O << llvm::formatv("ld_req_t<{0}, {1}> ld_req_{2}_{3} (0u);\n"
                       "InitBundle(ld_req_{2}_{3}.sched, 0u);\n"
                       "InitBundle(ld_req_{2}_{3}.posDepDist, false);\n"
                       "InitBundle(ld_req_{2}_{3}.isMaxIter, false);\n"
                       "LoadAddrPipes_{1}::PipeAt<{3}>::write(ld_req_{2}_{3});\n",
                       MemDep.numStores, MemDep.maxLoopDepth, MemDep.id, ReqId);

    infoJson["instructionIdx"] = getIndexIntoParent(LdReq.memOp);
    infoJson["instructionBasicBlockIdx"] =
        getIndexIntoParent(LdReq.memOp->getParent());
    infoJson["isWrite"] = true;
    infoJson["pipeCall"] = pipeCallStr;
    Res.push_back(std::move(infoJson));
  }

  for (auto &StReq : DecoupleInfo.stores) {
    auto MemDep = MemDepInfos[StReq.memoryId];
    int ReqId = MemDep.storeReqIds[StReq.memOp];

    json::Object infoJson;
    std::string pipeCallStr;
    llvm::raw_string_ostream O(pipeCallStr);
    O << llvm::formatv("st_req_t<{0}> st_req_{1}_{2} (0u);\n"
                       "InitBundle(st_req_{1}_{2}.sched, 0u);\n"
                       "InitBundle(st_req_{1}_{2}.isMaxIter, false);\n"
                       "StoreAddrPipes_{1}::PipeAt<{2}, 0>::write(st_req_{1}_{2});\n"
                       "StoreAddrPipes_{1}::PipeAt<{2}, 1>::write(st_req_{1}_{2});\n",
                       MemDep.maxLoopDepth, MemDep.id, ReqId);

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
    int ReqId = MemDep.loadReqIds[LdReq.memOp];

    json::Object infoJson;
    std::string pipeCallStr;
    llvm::raw_string_ostream O(pipeCallStr);
    O << llvm::formatv(
        "auto _ldVal_{0}_{1} = LoadValPipes_{0}::PipeAt<{1}>::read();\n",
        MemDep.id, ReqId);

    infoJson["instructionIdx"] = getIndexIntoParent(LdReq.memOp);
    infoJson["instructionBasicBlockIdx"] =
        getIndexIntoParent(LdReq.memOp->getParent());
    infoJson["isWrite"] = false;
    infoJson["pipeCall"] = pipeCallStr;
    Res.push_back(std::move(infoJson));
  }

  for (auto &StReq : DecoupleInfo.stores) {
    auto MemDep = MemDepInfos[StReq.memoryId];
    int ReqId = MemDep.storeReqIds[StReq.memOp];

    json::Object infoJson;
    std::string pipeCallStr;
    llvm::raw_string_ostream O(pipeCallStr);
    O << llvm::formatv("StoreValPipes_{0}::PipeAt<{1}>::write();\n", MemDep.id,
                       ReqId);

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
                                   LoopInfo &LI) {
  auto Res = json::Array();

  auto memDepInfo = DLFA.getMemoryDependencyInfo();
  for (auto &decoupleInfo : DLFA.getLoopsToDecouple()) {
    json::Object info;
    info["type"] = "compute";
    info["id"] = decoupleInfo.id;
    info["innerLoopBlockId"] =
        getIndexIntoParent(decoupleInfo.inner()->getHeader());
    info["pipeCalls"] = getPipeCallsInComputeJson(decoupleInfo, memDepInfo);
    Res.push_back(std::move(info));
  }
  for (auto &decoupleInfo : DLFA.getAgusToDecouple()) {
    json::Object info;
    info["type"] = "agu";
    info["id"] = decoupleInfo.id;
    info["innerLoopBlockId"] =
        getIndexIntoParent(decoupleInfo.inner()->getHeader());
    info["pipeCalls"] = getPipeCallsInAguJson(decoupleInfo, memDepInfo);
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
  
      json::Object report;
      report["mainKernelName"] = demangle(std::string(F.getName()));
      report["kernelStartLine"] = F.getSubprogram()->getLine();
      report["memoryToProtect"] = getMemoryDependenciesJson(*DLFA);
      report["loopsToDecouple"] = getLoopsToDecoupleJson(*DLFA, LI);

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
    AU.addRequiredID(DominatorTreeAnalysis::ID());
    AU.addRequiredID(PostDominatorTreeAnalysis::ID());
    AU.addRequiredID(DependenceAnalysis::ID());
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
