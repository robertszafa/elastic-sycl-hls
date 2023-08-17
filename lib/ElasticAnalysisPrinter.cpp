#include "CommonLLVM.h"
#include "DataHazardAnalysis.h"
#include "CDDDAnalysis.h"

using namespace llvm;

namespace llvm {

/// Print all analysis information related to dynamically scheduling memory via
/// a load-store queue.
void lsqReportPrinter(DataHazardAnalysis *DHA, json::Object &report) {
  auto instr2pipeArray = *report["instr2pipe_directives"].getAsArray();
  auto lsqArray = *report["lsq_array"].getAsArray();
  auto mainKernelName = std::string(*report["main_kernel_name"].getAsString());

  for (size_t iLSQ = 0; iLSQ < DHA->getHazardInstructions().size(); ++iLSQ) {
    bool isDecoupled = DHA->getDecoupligDecisions()[iLSQ];
    bool isOnChip = DHA->getIsOnChip()[iLSQ];
    auto instrCluster = DHA->getHazardInstructions()[iLSQ];
    auto isAnySpeculation = DHA->getSpeculationDecisions()[iLSQ];
    SmallVector<Instruction *> loads, stores;
    SmallVector<int> loadsSpecSrcBB, storesSpecSrcBB;
    for (size_t i = 0; i < instrCluster.size(); ++i) {
      if (isaLoad(instrCluster[i])) 
        loads.push_back(instrCluster[i]);
      else
        stores.push_back(instrCluster[i]);
    }
    SmallVector<int> ldSeqInBB = getSeqInBB(loads);
    SmallVector<int> stSeqInBB = getSeqInBB(stores);
    int maxLdsBB = *std::max_element(ldSeqInBB.begin(), ldSeqInBB.end()) + 1;
    int maxStsBB = *std::max_element(stSeqInBB.begin(), stSeqInBB.end()) + 1;
    auto aguName = mainKernelName + "_AGU_" + std::to_string(iLSQ);

    MapVector<BasicBlock *, int> loadsPerBB, storesPerBB;

    // General info about the LSQ as a whole.
    json::Object thisMemory;
    thisMemory["max_loads_per_bb"] = maxLdsBB;
    thisMemory["max_stores_per_bb"] = maxStsBB;
    thisMemory["is_onchip"] = isOnChip;
    thisMemory["store_queue_size"] = DHA->getStoreQueueSizes()[iLSQ];
    thisMemory["decouple_address"] = isDecoupled;
    thisMemory["is_any_speculation"] = isAnySpeculation;
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

    // Directives for load request/allocation writes and load value reads.
    for (size_t iLd = 0; iLd < loads.size(); ++iLd) {
      if (!loadsPerBB.contains(loads[iLd]->getParent()))
        loadsPerBB[loads[iLd]->getParent()] = 0;

      // Load value read in main kernel.
      json::Object ldValDirective;
      ldValDirective["directive_type"] = "ld_val";
      ldValDirective["instruction"] = genJsonForInstruction(loads[iLd]);
      ldValDirective["read/write"] = "read";
      ldValDirective["kernel_name"] = mainKernelName;
      ldValDirective["pipe_name"] =
          "pipes_ld_val_" + std::to_string(iLSQ) + "_class";
      ldValDirective["pipe_type"] = typeStr;
      ldValDirective["seq_in_bb"] = loadsPerBB[loads[iLd]->getParent()];
      ldValDirective["is_address_decoupled"] = isDecoupled;
      instr2pipeArray.push_back(std::move(ldValDirective));

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
      ldReqDirective["seq_in_bb"] = loadsPerBB[loads[iLd]->getParent()];
      ldReqDirective["max_loads_per_bb"] = maxLdsBB;
      ldReqDirective["is_address_decoupled"] = isDecoupled;
      instr2pipeArray.push_back(std::move(ldReqDirective));

      loadsPerBB[loads[iLd]->getParent()]++;
    }

    // Directives for store request/allocation writes and store value writes.
    for (size_t iSt = 0; iSt < stores.size(); ++iSt) {
      if (!storesPerBB.contains(stores[iSt]->getParent()))
        storesPerBB[stores[iSt]->getParent()] = 0;

      // Store value write in main kernel.
      json::Object stValDirective;
      stValDirective["directive_type"] = "st_val";
      stValDirective["instruction"] = genJsonForInstruction(stores[iSt]);
      stValDirective["read/write"] = "write";
      stValDirective["kernel_name"] = mainKernelName;
      stValDirective["pipe_name"] =
          "pipes_st_val_" + std::to_string(iLSQ) + "_class";
      stValDirective["pipe_type"] =
          isOnChip ? "tagged_val_lsq_bram_t<" + typeStr + ">"
                   : "tagged_val_lsq_dram_t<" + typeStr + ">";
      stValDirective["seq_in_bb"] = storesPerBB[stores[iSt]->getParent()];
      stValDirective["max_stores_per_bb"] = maxStsBB;
      stValDirective["is_address_decoupled"] = isDecoupled;
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
      stReqDirective["seq_in_bb"] = storesPerBB[stores[iSt]->getParent()];
      stReqDirective["is_address_decoupled"] = isDecoupled;
      instr2pipeArray.push_back(std::move(stReqDirective));

      storesPerBB[stores[iSt]->getParent()]++;
    }

    // Info about speculatively allocating addresses in the AGU.
    json::Array hoistingInfoArray;
    if (isAnySpeculation) {
      for (auto [specBB, allocStack] : DHA->getSpeculationStack()) {
        json::Object thisSpecBB;
        thisSpecBB["hoist_into_basic_block_idx"] =
            getIndexOfChild(specBB->getParent(), specBB);

        json::Array speculativeAllocations;
        for (auto I : allocStack)
          speculativeAllocations.push_back(genJsonForInstruction(I));
        thisSpecBB["hoisted_instructions"] = std::move(speculativeAllocations);

        hoistingInfoArray.push_back(std::move(thisSpecBB));
      }

      // Directives for poison read/writes on misspeculation.
      DenseMap<std::pair<BasicBlock *, BasicBlock *>, int> loadsPerPoisonBB,
          storesPerPoisonBB;
      for (auto [I, poisonLocations] : DHA->getPoisonLocations()) {
        for (auto [predBB, succBB] : poisonLocations) {
          json::Object poisonDirective;
          poisonDirective["directive_type"] = "poison";
          poisonDirective["read/write"] = isaLoad(I) ? "read" : "write";
          poisonDirective["kernel_name"] = mainKernelName;
          poisonDirective["pipe_name"] =
              isaLoad(I) ? "pipes_ld_val_" + std::to_string(iLSQ) + "_class"
                        : "pipes_st_val_" + std::to_string(iLSQ) + "_class";
          // A poison block will be inserted on pred->succ edge.
          poisonDirective["pred_basic_block_idx"] = 
              getIndexOfChild(predBB->getParent(), predBB);
          poisonDirective["succ_basic_block_idx"] = 
              getIndexOfChild(succBB->getParent(), succBB);
          
          auto &ldOrStSeq = isaLoad(I) ? loadsPerPoisonBB : storesPerPoisonBB;
          if (!ldOrStSeq.contains({predBB, succBB}))
            ldOrStSeq[{predBB, succBB}] = 0;
          poisonDirective["seq_in_bb"] = ldOrStSeq[{predBB, succBB}];
          poisonDirective["max_stores_per_bb"] = maxStsBB;
          ldOrStSeq[{predBB, succBB}]++;
          
          instr2pipeArray.push_back(std::move(poisonDirective));
        }
      }
    }
    thisMemory["speculation_hoisting_info"] = std::move(hoistingInfoArray);

    auto end_signal_pipe_name =
        "pipe_end_lsq_signal_" + std::to_string(iLSQ) + "_class";
    json::Object endSignalPipeDirective;
    endSignalPipeDirective["directive_type"] = "end_lsq_signal";
    endSignalPipeDirective["pipe_name"] = end_signal_pipe_name;
    endSignalPipeDirective["read/write"] = "write";
    endSignalPipeDirective["kernel_name"] = mainKernelName;
    endSignalPipeDirective["pipe_type"] = "bool";
    instr2pipeArray.push_back(std::move(endSignalPipeDirective));

    lsqArray.push_back(std::move(thisMemory));
  }

  report["lsq_array"] = std::move(lsqArray);
  report["instr2pipe_directives"] = std::move(instr2pipeArray);
}

/// Print all analysis information related to dynamically scheduling BBs.
void decoupledBlocksReportPrinter(ControlDependentDataDependencyAnalysis *CDDD,
                                  json::Object &report) {
  auto instr2pipeArray = *report["instr2pipe_directives"].getAsArray();
  auto peArray = *report["pe_array"].getAsArray();
  auto mainKernelName = std::string(*report["main_kernel_name"].getAsString());
  auto blocksToDecouple = CDDD->getBlocksToDecouple();

  for (size_t iBB = 0; iBB < blocksToDecouple.size(); ++iBB) {
    auto peKernelName = mainKernelName + "_PE_BB_" + std::to_string(iBB);
    auto BB = blocksToDecouple[iBB];
    auto bbIdx = getIndexOfChild(BB->getParent(), BB);

    auto depIn = CDDD->getBlockInputDependencies(BB);
    for (size_t iDepIn = 0; iDepIn < depIn.size(); ++iDepIn) {
      auto thisInstr = depIn[iDepIn];
      auto pipeName = "pipe_pe_" + std::to_string(iBB) + "_dep_in_" +
                      std::to_string(iDepIn) + "_class";

      json::Object depInWriteDirective;
      depInWriteDirective["directive_type"] = "ssa";
      depInWriteDirective["instruction"] = genJsonForInstruction(thisInstr);
      depInWriteDirective["basic_block_idx"] = bbIdx;
      depInWriteDirective["read/write"] = "write";
      depInWriteDirective["kernel_name"] = mainKernelName;
      depInWriteDirective["pipe_name"] = pipeName;
      depInWriteDirective["pipe_type"] = getTypeString(thisInstr);
      depInWriteDirective["pe_type"] = "block";

      json::Object depInReadDirective;
      depInReadDirective["directive_type"] = "ssa";
      depInReadDirective["instruction"] = genJsonForInstruction(thisInstr);
      depInReadDirective["basic_block_idx"] = bbIdx;
      depInReadDirective["read/write"] = "read";
      depInReadDirective["kernel_name"] = peKernelName;
      depInReadDirective["pipe_name"] = pipeName;
      depInReadDirective["pipe_type"] = getTypeString(thisInstr);
      depInReadDirective["pe_type"] = "block";

      instr2pipeArray.push_back(std::move(depInWriteDirective));
      instr2pipeArray.push_back(std::move(depInReadDirective));
    }

    auto depOut = CDDD->getBlockOutputDependencies(BB);
    for (size_t iDepOut = 0; iDepOut < depOut.size(); ++iDepOut) {
      auto thisInstr = depOut[iDepOut];
      auto pipeName = "pipe_pe_" + std::to_string(iBB) + "_dep_out_" +
                      std::to_string(iDepOut) + "_class";

      json::Object depOutWriteDirective;
      depOutWriteDirective["directive_type"] = "ssa";
      depOutWriteDirective["instruction"] = genJsonForInstruction(thisInstr);
      depOutWriteDirective["basic_block_idx"] = bbIdx;
      depOutWriteDirective["read/write"] = "write";
      depOutWriteDirective["kernel_name"] = peKernelName;
      depOutWriteDirective["pipe_name"] = pipeName;
      depOutWriteDirective["pipe_type"] = getTypeString(thisInstr);
      depOutWriteDirective["pe_type"] = "block";

      json::Object depOutReadDirective;
      depOutReadDirective["directive_type"] = "ssa";
      depOutReadDirective["instruction"] = genJsonForInstruction(thisInstr);
      depOutReadDirective["basic_block_idx"] = bbIdx;
      depOutReadDirective["read/write"] = "read";
      depOutReadDirective["kernel_name"] = mainKernelName;
      depOutReadDirective["pipe_name"] = pipeName;
      depOutReadDirective["pipe_type"] = getTypeString(thisInstr);
      depOutReadDirective["pe_type"] = "block";

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
    predPipeReadDirective["pipe_type"] = "int8_t";
    predPipeReadDirective["basic_block_idx"] = bbIdx;
    predPipeReadDirective["pe_type"] = "block";
    json::Object predPipeWriteDirective;
    predPipeWriteDirective["directive_type"] = "pred";
    predPipeWriteDirective["pipe_name"] = predicate_pipe_name;
    predPipeWriteDirective["read/write"] = "write";
    predPipeWriteDirective["kernel_name"] = mainKernelName;
    predPipeWriteDirective["pipe_type"] = "int8_t";
    predPipeWriteDirective["basic_block_idx"] = bbIdx;
    predPipeWriteDirective["pe_type"] = "block";
    instr2pipeArray.push_back(std::move(predPipeReadDirective));
    instr2pipeArray.push_back(std::move(predPipeWriteDirective));

    json::Object thisBlockOb;
    thisBlockOb["pe_kernel_name"] = peKernelName;
    thisBlockOb["basic_block_idx"] = bbIdx;
    thisBlockOb["pe_type"] = "block";
    peArray.push_back(std::move(thisBlockOb));


  }

  report["pe_array"] = std::move(peArray);
  report["instr2pipe_directives"] = std::move(instr2pipeArray);
}

/// Print all analysis information related to dynamically scheduling loops.
void decoupledLoopsReportPrinter(ControlDependentDataDependencyAnalysis *CDDD,
                                 json::Object &report) {
  auto instr2pipeArray = *report["instr2pipe_directives"].getAsArray();
  auto peArray = *report["pe_array"].getAsArray();
  auto mainKernelName = std::string(*report["main_kernel_name"].getAsString());
  auto loopsToDecouple = CDDD->getLoopsToDecouple();

  for (size_t iL = 0; iL < loopsToDecouple.size(); ++iL) {
    auto peKernelName = mainKernelName + "_PE_LOOP_" + std::to_string(iL);
    auto L = loopsToDecouple[iL];
    auto headerIdx = getIndexOfChild(L->getHeader()->getParent(), L->getHeader());
    auto depIn = CDDD->getLoopInputDependencies(L);
    auto depout = CDDD->getLoopOutputDependencies(L);

    for (size_t iDepIn = 0; iDepIn < depIn.size(); ++iDepIn) {
      auto thisInstr = depIn[iDepIn];
      auto pipeName = "pipe_pe_loop_" + std::to_string(iL) + "_dep_in_" +
                      std::to_string(iDepIn) + "_class";

      json::Object depInWriteDirective;
      depInWriteDirective["directive_type"] = "ssa";
      depInWriteDirective["instruction"] = genJsonForInstruction(thisInstr);
      depInWriteDirective["basic_block_idx"] = headerIdx;
      depInWriteDirective["read/write"] = "write";
      depInWriteDirective["kernel_name"] = mainKernelName;
      depInWriteDirective["pipe_name"] = pipeName;
      depInWriteDirective["pipe_type"] = getTypeString(thisInstr);
      depInWriteDirective["pe_type"] = "loop";

      json::Object depInReadDirective;
      depInReadDirective["directive_type"] = "ssa";
      depInReadDirective["instruction"] = genJsonForInstruction(thisInstr);
      depInReadDirective["basic_block_idx"] = headerIdx;
      depInReadDirective["read/write"] = "read";
      depInReadDirective["kernel_name"] = peKernelName;
      depInReadDirective["pipe_name"] = pipeName;
      depInReadDirective["pipe_type"] = getTypeString(thisInstr);
      depInReadDirective["pe_type"] = "loop";

      instr2pipeArray.push_back(std::move(depInWriteDirective));
      instr2pipeArray.push_back(std::move(depInReadDirective));
    }

    auto depOut = CDDD->getLoopOutputDependencies(L);
    for (size_t iDepOut = 0; iDepOut < depOut.size(); ++iDepOut) {
      auto thisInstr = depOut[iDepOut];
      auto pipeName = "pipe_pe_loop_" + std::to_string(iL) + "_dep_out_" +
                      std::to_string(iDepOut) + "_class";

      json::Object depOutWriteDirective;
      depOutWriteDirective["directive_type"] = "ssa";
      depOutWriteDirective["instruction"] = genJsonForInstruction(thisInstr);
      depOutWriteDirective["basic_block_idx"] = headerIdx;
      depOutWriteDirective["read/write"] = "write";
      depOutWriteDirective["kernel_name"] = peKernelName;
      depOutWriteDirective["pipe_name"] = pipeName;
      depOutWriteDirective["pipe_type"] = getTypeString(thisInstr);
      depOutWriteDirective["pe_type"] = "loop";

      json::Object depOutReadDirective;
      depOutReadDirective["directive_type"] = "ssa";
      depOutReadDirective["instruction"] = genJsonForInstruction(thisInstr);
      depOutReadDirective["basic_block_idx"] = headerIdx;
      depOutReadDirective["read/write"] = "read";
      depOutReadDirective["kernel_name"] = mainKernelName;
      depOutReadDirective["pipe_name"] = pipeName;
      depOutReadDirective["pipe_type"] = getTypeString(thisInstr);
      depOutReadDirective["pe_type"] = "loop";

      instr2pipeArray.push_back(std::move(depOutWriteDirective));
      instr2pipeArray.push_back(std::move(depOutReadDirective));
    }

    // One predicate pipe per decoupled PE.
    auto predicate_pipe_name =
        "pipe_pe_loop_" + std::to_string(iL) + "_pred_class";
    json::Object predPipeReadDirective;
    predPipeReadDirective["directive_type"] = "pred";
    predPipeReadDirective["pipe_name"] = predicate_pipe_name;
    predPipeReadDirective["read/write"] = "read";
    predPipeReadDirective["kernel_name"] = peKernelName;
    predPipeReadDirective["pipe_type"] = "int8_t";
    predPipeReadDirective["basic_block_idx"] = headerIdx;
    predPipeReadDirective["pe_type"] = "loop";
    json::Object predPipeWriteDirective;
    predPipeWriteDirective["directive_type"] = "pred";
    predPipeWriteDirective["pipe_name"] = predicate_pipe_name;
    predPipeWriteDirective["read/write"] = "write";
    predPipeWriteDirective["kernel_name"] = mainKernelName;
    predPipeWriteDirective["pipe_type"] = "int8_t";
    predPipeWriteDirective["basic_block_idx"] = headerIdx;
    predPipeWriteDirective["pe_type"] = "loop";
    instr2pipeArray.push_back(std::move(predPipeReadDirective));
    instr2pipeArray.push_back(std::move(predPipeWriteDirective));

    json::Object thisLoopOb;
    thisLoopOb["pe_kernel_name"] = peKernelName;
    thisLoopOb["basic_block_idx"] = headerIdx;
    thisLoopOb["pe_type"] = "loop";
    peArray.push_back(std::move(thisLoopOb));
  }

  report["pe_array"] = std::move(peArray);
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
      auto &DT = AM.getResult<DominatorTreeAnalysis>(F);
      auto &PDT = AM.getResult<PostDominatorTreeAnalysis>(F);
      auto &DI = AM.getResult<DependenceAnalysis>(F);
      std::shared_ptr<DataDependenceGraph> DDG(new DataDependenceGraph(F, DI));
      std::shared_ptr<ControlDependenceGraph> CDG(
          new ControlDependenceGraph(F, PDT));

      // Get all memory loads and stores that form a RAW data hazard.
      auto *DHA = new DataHazardAnalysis(F, LI, SE, DT, PDT, *DDG, *CDG);
      // Get all control-dependent data dependencies that increase the recII.
      auto *CDDD = new ControlDependentDataDependencyAnalysis(LI, *DDG, *CDG);

      // Generate a json report with the required info for a transformation that
      // wioll decouple the data hazards and cddd recurrences.
      json::Object report;
      report["main_kernel_name"] = demangle(std::string(F.getName()));
      report["kernel_start_line"] = F.getSubprogram()->getLine();
      // This info will be filled out by the data hazard and cddd analysis.
      report["instr2pipe_directives"] = json::Array();
      report["lsq_array"] = json::Array();
      report["pe_array"] = json::Array();

      lsqReportPrinter(DHA, report);
      decoupledBlocksReportPrinter(CDDD, report);
      decoupledLoopsReportPrinter(CDDD, report);

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
    AU.addRequiredID(DominatorTreeAnalysis::ID());
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