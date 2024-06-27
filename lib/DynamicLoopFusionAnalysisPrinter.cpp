#include "CommonLLVM.h"
#include "DynamicLoopFusionAnalysis.h"

using namespace llvm;


namespace llvm {

using DecoupledLoopType = DynamicLoopFusionAnalysis::DecoupledLoopType;
using MemoryRequestType = DynamicLoopFusionAnalysis::MemoryRequestType;
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

json::Array getMemoryDependenciesJson(DynamicLoopFusionAnalysis &DLFA) {
  auto Res = json::Array();
  for (auto &[id, memDepInfo] : DLFA.getProtectedMemoryInfo()) {
    json::Object infoJson;
    infoJson["id"] = int(id);
    infoJson["structDef"] = getDepInfoStructDef(memDepInfo);
    infoJson["cType"] = memDepInfo.cType;
    Res.push_back(std::move(infoJson));
  }

  return Res;
}

std::string getPipeDefenitionsString(DynamicLoopFusionAnalysis &DLFA) {
  constexpr unsigned PIPE_DEPTH = 16;

  std::string AllDefs;
  llvm::raw_string_ostream O(AllDefs);

  for (auto &[_, MemDepInfo] : DLFA.getProtectedMemoryInfo()) {
    O << llvm::formatv("using LoadReqPipes_{0} = PipeArray<class "
                       "LoadReqPipes_{0}_, ld_req_t<{1}, {2}>, {3}, {4}>;\n"
                       "using LoadValPipes_{0} = PipeArray<class "
                       "LoadValPipes_{0}_, {5}, {3}, {4}>;\n"
                       "using StoreReqPipes_{0} = PipeArray<class "
                       "StoreReqPipes_{0}_, st_req_t<{2}>, {3}, {1}, 2>;\n"
                       "using StoreValPipes_{0} = PipeArray<class "
                       "StoreValPipes_{0}_, {5}, {3}, {1}>;\n",
                       MemDepInfo.id, MemDepInfo.numStores,
                       MemDepInfo.maxLoopDepth, PIPE_DEPTH, MemDepInfo.numLoads,
                       MemDepInfo.cType);
  }

  for (auto DecoupleInfo : DLFA.getSimpleMemoryLoops()) {
    for (auto &Req :
         llvm::concat<MemoryRequest>(DecoupleInfo.loads, DecoupleInfo.stores)) {
      std::string cType =
          isaLoad(Req.memOp)
              ? getCTypeString(Req.memOp)
              : getCTypeString(dyn_cast<Instruction>(Req.memOp->getOperand(0)));
      O << llvm::formatv("using {0} = pipe<class _{0}, {1}, {2}>;\n",
                         Req.getPipeName(), cType, PIPE_DEPTH);
    }
  }

  return AllDefs;
}

std::string
getPipeCallsInAgu(DecoupledLoopInfo &DecoupleInfo,
                  MapVector<int, MemoryDependencyInfo> &ProtectedMemInfo) {
  std::string PipeCalls;
  llvm::raw_string_ostream O(PipeCalls);
  for (auto LdReq : DecoupleInfo.loads) {
    auto MemDep = ProtectedMemInfo[LdReq.memoryId];
    auto PipeName = LdReq.getPipeName(DecoupledLoopType::agu);
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
    O << llvm::formatv("{0}::PipeAt<{2}>::write(ld_req_{1}_{2});\n", PipeName,
                       MemDep.id, LdReq.reqId);
  }

  for (auto StReq : DecoupleInfo.stores) {
    auto MemDep = ProtectedMemInfo[StReq.memoryId];
    auto PipeName = StReq.getPipeName(DecoupledLoopType::agu);
    O << llvm::formatv("st_req_t<{0}> st_req_{1}_{2};\n"
                       "st_req_{1}_{2}.addr = 0u;\n",
                       MemDep.maxLoopDepth, MemDep.id, StReq.reqId);
    for (int iD = 0; iD < MemDep.maxLoopDepth; ++iD) {
      O << llvm::formatv("st_req_{0}_{1}.sched[{2}] = 0u;\n"
                         "st_req_{0}_{1}.isMaxIter[{2}] = false;\n",
                         MemDep.id, StReq.reqId, iD);
    }
    O << llvm::formatv("{0}::PipeAt<{2}, 0>::write(st_req_{1}_{2});\n"
                       "{0}::PipeAt<{2}, 1>::write(st_req_{1}_{2});\n",
                       PipeName, MemDep.id, StReq.reqId);
  }

  return PipeCalls;
}

std::string getPipeCallsInCompute(DecoupledLoopInfo &DecoupleInfo) {
  std::string PipeCalls;
  llvm::raw_string_ostream O(PipeCalls);

  for (auto &Req :
       llvm::concat<MemoryRequest>(DecoupleInfo.loads, DecoupleInfo.stores)) {
    auto PipeName = Req.getPipeName(DecoupledLoopType::compute);

    if (isaLoad(Req.memOp))
      O << llvm::formatv("[[maybe_unused]] auto _ldVal_{0} = ", PipeName);
    O << PipeName;
    if (Req.type == MemoryRequestType::protectedMem)
      O << llvm::formatv("::PipeAt<{0}>", Req.reqId);

    if (isaLoad(Req.memOp))
      O << "::read();\n";
    else
      O << "::write({});\n";
  }

  return PipeCalls;
}

std::string getPipeCallsInSimpleMemory(DecoupledLoopInfo &DecoupleInfo) {
  std::string PipeCalls;
  llvm::raw_string_ostream O(PipeCalls);

  for (auto &Req : DecoupleInfo.loads) 
    O << llvm::formatv("{0}::write({{});\n", Req.getPipeName());
  for (auto &Req : DecoupleInfo.stores) 
    O << llvm::formatv("auto {0} = {0}::read();\n", Req.getPipeName());

  return PipeCalls;
}

json::Array getLoopsToDecoupleJson(DynamicLoopFusionAnalysis &DLFA,
                                   LoopInfo &LI) {
  auto Res = json::Array();

  auto ProtectedMemInfo = DLFA.getProtectedMemoryInfo();
  auto AllDecoupledLoops = llvm::concat<DecoupledLoopInfo>(
      DLFA.getSimpleMemoryLoops(), DLFA.getAguLoops(), DLFA.getComputeLoops());
  for (auto decoupleInfo : AllDecoupledLoops) {
    json::Object info;
    info["id"] = decoupleInfo.id;
    info["kernelName"] = decoupleInfo.kernelName;

    if (decoupleInfo.type == DynamicLoopFusionAnalysis::agu) {
      info["type"] = "agu";
      info["pipeCalls"] = getPipeCallsInAgu(decoupleInfo, ProtectedMemInfo);
    } else if (decoupleInfo.type == DynamicLoopFusionAnalysis::compute) {
      info["type"] = "compute";
      info["pipeCalls"] = getPipeCallsInCompute(decoupleInfo);
    } else if (decoupleInfo.type == DynamicLoopFusionAnalysis::memory) {
      info["type"] = "memory";
      info["pipeCalls"] = getPipeCallsInSimpleMemory(decoupleInfo);
    }

    Res.push_back(std::move(info));
  }

  return Res;
}

struct DynamicLoopFusionAnalysisPrinter
    : PassInfoMixin<DynamicLoopFusionAnalysisPrinter> {

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    if (F.getCallingConv() == CallingConv::SPIR_KERNEL) {
      const std::string fName = demangle(std::string(F.getName()));

      auto &LI = AM.getResult<LoopAnalysis>(F);
      auto &SE = AM.getResult<ScalarEvolutionAnalysis>(F);
      auto *DLFA = new DynamicLoopFusionAnalysis(LI, SE, fName);
  
      json::Object report;
      report["mainKernelName"] = fName;
      report["kernelStartLine"] = F.getSubprogram()->getLine();
      report["pipeDefs"] = getPipeDefenitionsString(*DLFA);
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
