#include "CommonLLVM.h"
#include "DataHazardAnalysis.h"
#include "CDDDAnalysis.h"

using namespace llvm;

namespace llvm {

void lsqReportPrinter(DataHazardAnalysis *DHA, json::Object &report) {
  auto instr2pipeArray = *report["instr2pipe_directives"].getAsArray();
  auto mainKernelName = std::string(*report["main_kernel_name"].getAsString());

  // For each instruction cluster, create a 'base_address' json object
  // that describes the hazardous instructions, decoupling decision, types...
  json::Array memoryToDecouple;
  for (size_t iLSQ = 0; iLSQ < DHA->getHazardInstructions().size(); ++iLSQ) {
    bool isDecoupled = DHA->getDecoupligDecisions()[iLSQ];
    bool isOnChip = DHA->getIsOnChip()[iLSQ];
    auto instrCluster = DHA->getHazardInstructions()[iLSQ];
    SmallVector<Instruction *> loads = getLoads(instrCluster);
    SmallVector<Instruction *> stores = getStores(instrCluster);
    SmallVector<int> ldSeqInBB = getSeqInBB(loads);
    SmallVector<int> stSeqInBB = getSeqInBB(stores);
    int maxLdsBB = *std::max_element(ldSeqInBB.begin(), ldSeqInBB.end()) + 1;
    int maxStsBB = *std::max_element(stSeqInBB.begin(), stSeqInBB.end()) + 1;
    auto aguName = mainKernelName + "_AGU_" + std::to_string(iLSQ);

    json::Object thisMemory;
    thisMemory["max_loads_per_bb"] = maxLdsBB;
    thisMemory["max_stores_per_bb"] = maxStsBB;
    thisMemory["is_onchip"] = isOnChip;
    thisMemory["decouple_address"] = isDecoupled;
    thisMemory["agu_kernel_name"] = isDecoupled ? aguName : "";
    thisMemory["array_size"] = DHA->getMemorySizes()[iLSQ];
    std::string typeStr;
    llvm::raw_string_ostream rso(typeStr);
    instrCluster[0]->getOperand(0)->getType()->print(rso);
    thisMemory["array_type"] = typeStr;
    json::Array storeIs;
    for (auto stI : stores) 
      storeIs.push_back(genJsonForInstruction(stI));
    thisMemory["store_instructions"] = std::move(storeIs);

    for (size_t iLd = 0; iLd < loads.size(); ++iLd) {
      // Load value read in main kernel.
      json::Object ldValDirective;
      ldValDirective["directive_type"] = "ld_val";
      ldValDirective["instruction"] = genJsonForInstruction(loads[iLd]);
      ldValDirective["read/write"] = "read";
      ldValDirective["kernel_name"] = mainKernelName;
      ldValDirective["pipe_name"] =
          "pipes_ld_val_" + std::to_string(iLSQ) + "_class";
      ldValDirective["pipe_type"] = typeStr;
      // Only a single ld val pipe for LSQ_BRAM.
      ldValDirective["seq_in_bb"] = ldSeqInBB[iLd];

      // Load request write in AGU (or possibly main) kernel.
      json::Object ldReqDirective;
      ldReqDirective["directive_type"] = "ld_req";
      ldReqDirective["instruction"] = genJsonForInstruction(loads[iLd]);
      ldReqDirective["read/write"] = "write";
      ldReqDirective["kernel_name"] = isDecoupled ? aguName : mainKernelName;
      ldReqDirective["pipe_name"] =
          "pipes_ld_req_" + std::to_string(iLSQ) + "_class";
      ldReqDirective["pipe_type"] =
          isOnChip ? "ld_req_lsq_bram_t" : "req_lsq_dram_t";
      ldReqDirective["seq_in_bb"] = ldSeqInBB[iLd];
      ldReqDirective["max_seq_in_bb"] = maxLdsBB;

      instr2pipeArray.push_back(std::move(ldValDirective));
      instr2pipeArray.push_back(std::move(ldReqDirective));
    }

    for (size_t iSt = 0; iSt < stores.size(); ++iSt) {
      // Store value write in main kernel.
      json::Object stValDirective;
      stValDirective["directive_type"] = 
        (maxStsBB > 1) ? "st_val_tagged" : "st_val";
      stValDirective["instruction"] = genJsonForInstruction(stores[iSt]);
      stValDirective["read/write"] = "write";
      stValDirective["kernel_name"] = mainKernelName;
      stValDirective["pipe_name"] =
          "pipes_st_val_" + std::to_string(iLSQ) + "_class";
      stValDirective["pipe_type"] =
          (maxStsBB > 1) ? isOnChip ? "tagged_val_lsq_bram_t<" + typeStr + ">"
                                    : "tagged_val_lsq_dram_t<" + typeStr + ">"
                         : typeStr;
      stValDirective["seq_in_bb"] = stSeqInBB[iSt];
      instr2pipeArray.push_back(std::move(stValDirective));

      json::Object stReqDirective;
      stReqDirective["directive_type"] = "st_req";
      stReqDirective["instruction"] = genJsonForInstruction(stores[iSt]);
      stReqDirective["read/write"] = "write";
      stReqDirective["kernel_name"] = isDecoupled ? aguName : mainKernelName;
      stReqDirective["pipe_name"] =
          "pipes_st_req_" + std::to_string(iLSQ) + "_class";
      stReqDirective["pipe_type"] = 
          isOnChip ? "st_req_lsq_bram_t" : "st_req_lsq_dram_t";
      stReqDirective["seq_in_bb"] = stSeqInBB[iSt];
      instr2pipeArray.push_back(std::move(stReqDirective));
    }

    auto end_signal_pipe_name =
        "pipe_end_lsq_signal_" + std::to_string(iLSQ) + "_class";
    json::Object endSignalPipeDirective;
    endSignalPipeDirective["directive_type"] = "end_lsq_signal";
    // endSignalPipeDirective["instruction"] = json::Object();
    endSignalPipeDirective["pipe_name"] = end_signal_pipe_name;
    endSignalPipeDirective["read/write"] = "write";
    endSignalPipeDirective["kernel_name"] = mainKernelName;
    endSignalPipeDirective["pipe_type"] = "bool";
    instr2pipeArray.push_back(std::move(endSignalPipeDirective));

    memoryToDecouple.push_back(std::move(thisMemory));
  }

  report["memory_to_decouple"] = std::move(memoryToDecouple);
  report["instr2pipe_directives"] = std::move(instr2pipeArray);
}

void cdddReportPrinter(ControlDependentDataDependencyAnalysis *CDDD,
                       json::Object &report) {
  auto instr2pipeArray = *report["instr2pipe_directives"].getAsArray();
  auto mainKernelName = std::string(*report["main_kernel_name"].getAsString());
  auto blocksToDecouple = CDDD->getBlocksToDecouple();

  // Now create json.
  json::Array blocksArray;
  for (size_t iBB = 0; iBB < blocksToDecouple.size(); ++iBB) {
    auto peKernelName = mainKernelName + "_PE_" + std::to_string(iBB);
    auto BB = blocksToDecouple[iBB];
    auto bbIdx = getIndexOfChild(BB->getParent(), BB);
    auto instrToDecouple = CDDD->getInstructionsToDecouple(BB);
    auto depIn = CDDD->getInputDependencies(BB);
    for (size_t iDepIn = 0; iDepIn < depIn.size(); ++iDepIn) {
      auto thisInstr = depIn[iDepIn];
      auto pipeName = "pipe_pe_" + std::to_string(iBB) + "_dep_in_" +
                      std::to_string(iDepIn) + "_class";

      json::Object depInWriteDirective;
      depInWriteDirective["directive_type"] = "ssa";
      depInWriteDirective["instruction"] = genJsonForInstruction(thisInstr);
      depInWriteDirective["pe_basic_block_idx"] = bbIdx;
      depInWriteDirective["read/write"] = "write";
      depInWriteDirective["kernel_name"] = mainKernelName;
      depInWriteDirective["pipe_name"] = pipeName;
      depInWriteDirective["pipe_type"] = getTypeString(thisInstr);

      json::Object depInReadDirective;
      depInReadDirective["directive_type"] = "ssa";
      depInReadDirective["instruction"] = genJsonForInstruction(thisInstr);
      depInReadDirective["pe_basic_block_idx"] = bbIdx;
      depInReadDirective["read/write"] = "read";
      depInReadDirective["kernel_name"] = peKernelName;
      depInReadDirective["pipe_name"] = pipeName;
      depInReadDirective["pipe_type"] = getTypeString(thisInstr);

      instr2pipeArray.push_back(std::move(depInWriteDirective));
      instr2pipeArray.push_back(std::move(depInReadDirective));
    }

    auto depOut = CDDD->getOutputDependencies(BB);
    for (size_t iDepOut = 0; iDepOut < depOut.size(); ++iDepOut) {
      auto thisInstr = depOut[iDepOut];
      auto pipeName = "pipe_pe_" + std::to_string(iBB) + "_dep_out_" +
                      std::to_string(iDepOut) + "_class";

      json::Object depOutWriteDirective;
      depOutWriteDirective["directive_type"] = "ssa";
      depOutWriteDirective["instruction"] = genJsonForInstruction(thisInstr);
      depOutWriteDirective["pe_basic_block_idx"] = bbIdx;
      depOutWriteDirective["read/write"] = "write";
      depOutWriteDirective["kernel_name"] = peKernelName;
      depOutWriteDirective["pipe_name"] = pipeName;
      depOutWriteDirective["pipe_type"] = getTypeString(thisInstr);

      json::Object depOutReadDirective;
      depOutReadDirective["directive_type"] = "ssa";
      depOutReadDirective["instruction"] = genJsonForInstruction(thisInstr);
      depOutReadDirective["pe_basic_block_idx"] = bbIdx;
      depOutReadDirective["read/write"] = "read";
      depOutReadDirective["kernel_name"] = mainKernelName;
      depOutReadDirective["pipe_name"] = pipeName;
      depOutReadDirective["pipe_type"] = getTypeString(thisInstr);

      instr2pipeArray.push_back(std::move(depOutWriteDirective));
      instr2pipeArray.push_back(std::move(depOutReadDirective));
    }

    // One predicate pipe per decoupled PE.
    auto predicate_pipe_name = "pipe_pe_" + std::to_string(iBB) + "_pred_class";
    json::Object predPipeReadDirective;
    predPipeReadDirective["directive_type"] = "pred";
    predPipeReadDirective["pipe_name"] = predicate_pipe_name;
    predPipeReadDirective["read/write"] = "read";
    predPipeReadDirective["kernel_name"] = peKernelName;
    predPipeReadDirective["pipe_type"] = "bool";
    predPipeReadDirective["basic_block_idx"] = bbIdx;
    json::Object predPipeWriteDirective;
    predPipeWriteDirective["directive_type"] = "pred";
    predPipeWriteDirective["pipe_name"] = predicate_pipe_name;
    predPipeWriteDirective["read/write"] = "write";
    predPipeWriteDirective["kernel_name"] = mainKernelName;
    predPipeWriteDirective["pipe_type"] = "bool";
    predPipeWriteDirective["basic_block_idx"] = bbIdx;
    instr2pipeArray.push_back(std::move(predPipeReadDirective));
    instr2pipeArray.push_back(std::move(predPipeWriteDirective));

    json::Object thisBlockOb;
    thisBlockOb["pe_kernel_name"] = peKernelName;
    thisBlockOb["basic_block_idx"] = bbIdx;
    // All decoupled instructions
    json::Array instrToDecoupleJson;
    for (auto I : instrToDecouple) 
      instrToDecoupleJson.push_back(genJsonForInstruction(I));
    thisBlockOb["decoupled_instructions"] = std::move(instrToDecoupleJson);

    blocksArray.push_back(std::move(thisBlockOb));
  }

  report["blocks_to_decouple"] = std::move(blocksArray);
  report["instr2pipe_directives"] = std::move(instr2pipeArray);
}

/// Generate a report for memory instructions that form 
/// a RAW inter-iteration data hazard.
struct ElasticAnalysisPrinter : PassInfoMixin<ElasticAnalysisPrinter> {

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    if (F.getCallingConv() == CallingConv::SPIR_KERNEL) {
      // Get all required analysis'. Only the CDG is custom written.
      auto &LI = AM.getResult<LoopAnalysis>(F);
      auto &SE = AM.getResult<ScalarEvolutionAnalysis>(F);
      auto &PDT = AM.getResult<PostDominatorTreeAnalysis>(F);
      auto &DI = AM.getResult<DependenceAnalysis>(F);
      std::shared_ptr<DataDependenceGraph> DDG(new DataDependenceGraph(F, DI));
      std::shared_ptr<ControlDependenceGraph> CDG(
          new ControlDependenceGraph(F, PDT));

      // Get all memory loads and stores that form a RAW data hazard.
      auto *DHA = new DataHazardAnalysis(F, LI, SE, PDT);
      // Get all control-dependent data dependencies that increase the recII.
      auto *CDDD = new ControlDependentDataDependencyAnalysis(LI, *DDG, *CDG);

      // Generate a json report with the required info for a transformation that
      // wioll decouple the data hazards and cddd recurrences.
      json::Object report;
      report["main_kernel_name"] = demangle(std::string(F.getName()));
      report["kernel_start_line"] = F.getSubprogram()->getLine();
      // This info will be filled out by the data hazard and cddd analysis.
      report["instr2pipe_directives"] = json::Array();
      lsqReportPrinter(DHA, report);
      cdddReportPrinter(CDDD, report);

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
    AU.addRequiredID(ScalarEvolutionAnalysis::ID());
    AU.addRequiredID(PostDominatorTreeAnalysis::ID());
    AU.addRequiredID(DependenceAnalysis::ID());
  }
};

//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getElasticAnalysisPrinterPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "ElasticAnalysisPrinter",
          LLVM_VERSION_STRING, [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "elastic-analysis") {
                    FPM.addPass(ElasticAnalysisPrinter());
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
  return getElasticAnalysisPrinterPluginInfo();
}

} // end namespace llvm