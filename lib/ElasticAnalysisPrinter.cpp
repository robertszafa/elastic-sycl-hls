/// This file is the analysis driver. It uses DataHazardAnalysis and
/// CDDDAnalysis to identify memory instructions, basic blocks, and whole loops
/// which can benefit from dynamic scheduling.

#include "CommonLLVM.h"
#include "DataHazardAnalysis.h"
#include "CDDDAnalysis.h"

using namespace llvm;

namespace llvm {

/// For any given pipe, all its reads an all writes should be in the same kernel
/// (reads and writes in a different kernel). Therefore, we cannot reuse the
/// same LSQ pipe if the BBs of its use will be decoupled into different kernel.
bool canReusePipesToLSQ(SmallVector<Instruction *> &memOps,
                        ControlDependentDataDependencyAnalysis *CDDD) {
  // We will count the unique blocks where any of the memOps are used.
  SetVector<BasicBlock *> usedInPE, usedInMain; 
  for (auto &memOp : memOps) {
    auto BB = memOp->getParent();
    if (CDDD->getBlocksToDecouple().contains(BB)) {
      usedInPE.insert(BB);
      continue;
    } 
    
    bool isInLoopPE = false;
    for (auto L : CDDD->getLoopsToDecouple()) {
      if (L->contains(BB)) {
        // When a memOp is used in a decoupled loop, then use the loop header
        // to cluster all memOps from that loop to one BB.
        usedInPE.insert(L->getHeader());
        isInLoopPE = true;
        break;
      }
    }

    if (!isInLoopPE)
      usedInMain.insert(BB);
  }

  // Can reuse if nothing decoupled, or everything decoupled into same kernel.
  return (usedInPE.size() == 0) || 
         (usedInPE.size() == 1 && usedInMain.size() == 0);
}

/// Print all analysis information related to dynamically scheduling memory.
void lsqReportPrinter(DataHazardAnalysis *DHA,
                      ControlDependentDataDependencyAnalysis *CDDD,
                      json::Object &report) {
  auto instr2pipeArray = *report["instr2pipe_directives"].getAsArray();
  auto lsqArray = *report["lsq_array"].getAsArray();
  auto mainKernelName = std::string(*report["main_kernel_name"].getAsString());

  for (size_t iLSQ = 0; iLSQ < DHA->getHazardInstructions().size(); ++iLSQ) {
    bool isDecoupled = DHA->getDecoupligDecisions()[iLSQ];
    bool isOnChip = DHA->getIsOnChip()[iLSQ];
    auto instrCluster = DHA->getHazardInstructions()[iLSQ];
    auto isAnySpeculation = DHA->getSpeculationDecisions()[iLSQ];
    SmallVector<Instruction *> loads, stores;
    for (size_t i = 0; i < instrCluster.size(); ++i) {
      if (isaLoad(instrCluster[i])) 
        loads.push_back(instrCluster[i]);
      else
        stores.push_back(instrCluster[i]);
    }

    // We can reuse ld/st pipes connected to the LSQ for ld/st from different 
    // BB if all the BBs are in the same kernel.
    const bool reuseLdPipesLSQ = canReusePipesToLSQ(loads, CDDD);
    const bool reuseStPipesLSQ = canReusePipesToLSQ(stores, CDDD);

    // Only used when reusePipesLSQ is true. Multiple loads/stores pipes
    // connected to the LSQ are represented as an array of pipes. The array is
    // named, and the individual pipes within it are identified via indices.
    SmallVector<int> ldSeqInBB = getSeqInBB(loads);
    SmallVector<int> stSeqInBB = getSeqInBB(stores);
    int maxLdsBB = *std::max_element(ldSeqInBB.begin(), ldSeqInBB.end()) + 1;
    int maxStsBB = *std::max_element(stSeqInBB.begin(), stSeqInBB.end()) + 1;
    // Mapping used to reuse LSQ pipes across basic blocks.
    MapVector<BasicBlock *, int> loadsPerBB, storesPerBB;
    // Mapping used to recover the pipe array idx for a given instruction.
    MapVector<Instruction *, int> pipeArrayIdxMap;

    // General info about the LSQ as a whole.
    auto aguName = mainKernelName + "_AGU_" + std::to_string(iLSQ);
    json::Object thisMemory;
    thisMemory["lsq_id"] = int(iLSQ);
    thisMemory["num_load_pipes"] =
        reuseLdPipesLSQ ? maxLdsBB : int(loads.size());
    thisMemory["num_store_pipes"] =
        reuseStPipesLSQ ? maxStsBB : int(stores.size());
    thisMemory["all_stores_in_same_kernel"] = reuseStPipesLSQ;
    thisMemory["all_loads_in_same_kernel"] = reuseLdPipesLSQ;
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
      // Only used when reusePipesLSQ is true.
      if (!loadsPerBB.contains(loads[iLd]->getParent()))
        loadsPerBB[loads[iLd]->getParent()] = 0;

      // Load value read in main kernel.
      json::Object ldValDirective;
      int ldValPipeArrayIdx =
          reuseLdPipesLSQ ? loadsPerBB[loads[iLd]->getParent()] : iLd;
      ldValDirective["directive_type"] = "ld_val";
      ldValDirective["instruction"] = genJsonForInstruction(loads[iLd]);
      ldValDirective["read/write"] = "read";
      ldValDirective["kernel_name"] = mainKernelName;
      ldValDirective["pipe_name"] =
          "pipes_ld_val_" + std::to_string(iLSQ) + "_class";
      ldValDirective["pipe_type"] = typeStr;
      ldValDirective["pipe_array_reuse"] = reuseLdPipesLSQ;
      ldValDirective["pipe_array_idx"] = ldValPipeArrayIdx;
      ldValDirective["is_address_decoupled"] = isDecoupled;
      ldValDirective["lsq_id"] = int(iLSQ);
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
      ldReqDirective["pipe_array_idx"] =
          reuseLdPipesLSQ ? loadsPerBB[loads[iLd]->getParent()] : iLd;
      ldReqDirective["num_load_pipes"] =
          reuseLdPipesLSQ ? maxLdsBB : loads.size();
      ldReqDirective["is_address_decoupled"] = isDecoupled;
      ldReqDirective["lsq_id"] = int(iLSQ);
      instr2pipeArray.push_back(std::move(ldReqDirective));

      pipeArrayIdxMap[loads[iLd]] = ldValPipeArrayIdx;
      loadsPerBB[loads[iLd]->getParent()]++;
    }

    // Directives for store request/allocation writes and store value writes.
    for (size_t iSt = 0; iSt < stores.size(); ++iSt) {
      // Only used when reusePipesLSQ is true.
      if (!storesPerBB.contains(stores[iSt]->getParent()))
        storesPerBB[stores[iSt]->getParent()] = 0;

      // Store value write in main kernel.
      json::Object stValDirective;
      int stValPipeArrayIdx =
          reuseStPipesLSQ ? storesPerBB[stores[iSt]->getParent()] : iSt;
      stValDirective["directive_type"] = "st_val";
      stValDirective["instruction"] = genJsonForInstruction(stores[iSt]);
      stValDirective["read/write"] = "write";
      stValDirective["kernel_name"] = mainKernelName;
      stValDirective["pipe_name"] =
          "pipes_st_val_" + std::to_string(iLSQ) + "_class";
      stValDirective["pipe_type"] =
          isOnChip ? "tagged_val_lsq_bram_t<" + typeStr + ">"
                   : "tagged_val_lsq_dram_t<" + typeStr + ">";
      stValDirective["pipe_array_reuse"] = reuseStPipesLSQ;
      stValDirective["pipe_array_idx"] = stValPipeArrayIdx;
      stValDirective["num_store_pipes"] =
          reuseStPipesLSQ ? maxStsBB : stores.size();
      stValDirective["lsq_id"] = int(iLSQ);
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
      stReqDirective["pipe_array_idx"] =
          reuseStPipesLSQ ? storesPerBB[stores[iSt]->getParent()] : iSt;
      stReqDirective["lsq_id"] = int(iLSQ);
      stReqDirective["is_address_decoupled"] = isDecoupled;
      instr2pipeArray.push_back(std::move(stReqDirective));

      pipeArrayIdxMap[stores[iSt]] = stValPipeArrayIdx;
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
      for (auto [I, poisonLocations] : DHA->getPoisonLocations()) {
        for (auto [predBB, succBB] : poisonLocations) {
          json::Object poisonDirective;
          poisonDirective["directive_type"] = "poison";
          poisonDirective["read/write"] = isaLoad(I) ? "read" : "write";
          poisonDirective["kernel_name"] = mainKernelName;
          poisonDirective["pipe_name"] =
              isaLoad(I) ? "pipes_ld_val_" + std::to_string(iLSQ) + "_class"
                         : "pipes_st_val_" + std::to_string(iLSQ) + "_class";
          // The block where the speculation is true.
          poisonDirective["basic_block_idx"] = 
              getIndexOfChild(predBB->getParent(), I->getParent());
          // A poison block will be inserted on pred->succ edge.
          poisonDirective["pred_basic_block_idx"] = 
              getIndexOfChild(predBB->getParent(), predBB);
          poisonDirective["succ_basic_block_idx"] = 
              getIndexOfChild(succBB->getParent(), succBB);
          poisonDirective["lsq_id"] = int(iLSQ);
          poisonDirective["num_store_pipes"] = 
            reuseStPipesLSQ ? maxStsBB : int(stores.size());
          poisonDirective["num_load_pipes"] = 
            reuseLdPipesLSQ ? maxLdsBB : int(loads.size());
          poisonDirective["pipe_array_reuse"] = reuseStPipesLSQ;
          poisonDirective["pipe_array_idx"] = pipeArrayIdxMap[I];
          
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
                                  LoopInfo &LI, json::Object &report) {
  auto instr2pipeArray = *report["instr2pipe_directives"].getAsArray();
  auto peArray = *report["pe_array"].getAsArray();
  auto mainKernelName = std::string(*report["main_kernel_name"].getAsString());
  auto blocksToDecouple = CDDD->getBlocksToDecouple();

  for (size_t iBB = 0; iBB < blocksToDecouple.size(); ++iBB) {
    auto peKernelName = mainKernelName + "_PE_BB_" + std::to_string(iBB);
    auto BB = blocksToDecouple[iBB];
    int bbIdx = getIndexOfChild(BB->getParent(), BB);
    auto L = LI.getLoopFor(BB);
    int loopExitIdx = getIndexOfChild(BB->getParent(), L->getExitBlock());
    int loopHeaderIdx = getIndexOfChild(BB->getParent(), L->getHeader());

    auto depIn = CDDD->getBlockInputDependencies(BB);
    for (size_t iDepIn = 0; iDepIn < depIn.size(); ++iDepIn) {
      auto thisInstr = depIn[iDepIn];
      auto pipeName = "pipe_pe_bb_" + std::to_string(iBB) + "_dep_in_" +
                      std::to_string(iDepIn) + "_class";

      json::Object depInWriteDirective;
      depInWriteDirective["directive_type"] = "ssa";
      depInWriteDirective["instruction"] = genJsonForInstruction(thisInstr);
      depInWriteDirective["basic_block_idx"] = bbIdx;
      depInWriteDirective["loop_exit_block_idx"] = loopExitIdx;
      depInWriteDirective["loop_header_block_idx"] = loopHeaderIdx;
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
      depOutReadDirective["loop_exit_block_idx"] = loopExitIdx;
      depOutReadDirective["loop_header_block_idx"] = loopHeaderIdx;
      depOutReadDirective["read/write"] = "read";
      depOutReadDirective["kernel_name"] = mainKernelName;
      depOutReadDirective["pipe_name"] = pipeName;
      depOutReadDirective["pipe_type"] = getTypeString(thisInstr);
      depOutReadDirective["pe_type"] = "block";

      instr2pipeArray.push_back(std::move(depOutWriteDirective));
      instr2pipeArray.push_back(std::move(depOutReadDirective));
    }
    
    // One predicate pipe per decoupled PE.
    auto pred_pipe_name = "pipe_pe_bb_" + std::to_string(iBB) + "_pred_class";
    json::Object predPipeReadDirective;
    predPipeReadDirective["directive_type"] = "pred";
    predPipeReadDirective["pipe_name"] = pred_pipe_name;
    predPipeReadDirective["read/write"] = "read";
    predPipeReadDirective["kernel_name"] = peKernelName;
    predPipeReadDirective["pipe_type"] = "int8_t";
    predPipeReadDirective["basic_block_idx"] = bbIdx;
    predPipeReadDirective["pe_type"] = "block";
    json::Object predPipeWriteDirective;
    predPipeWriteDirective["directive_type"] = "pred";
    predPipeWriteDirective["pipe_name"] = pred_pipe_name;
    predPipeWriteDirective["read/write"] = "write";
    predPipeWriteDirective["kernel_name"] = mainKernelName;
    predPipeWriteDirective["pipe_type"] = "int8_t";
    predPipeWriteDirective["basic_block_idx"] = bbIdx;
    predPipeWriteDirective["loop_exit_block_idx"] = loopExitIdx;
    predPipeWriteDirective["loop_header_block_idx"] = loopHeaderIdx;
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
    auto F = L->getHeader()->getParent();
    int loopHeaderIdx = getIndexOfChild(F, L->getHeader());
    int loopExitIdx = getIndexOfChild(F, L->getExitBlock());
    auto depIn = CDDD->getLoopInputDependencies(L);
    auto depout = CDDD->getLoopOutputDependencies(L);

    for (size_t iDepIn = 0; iDepIn < depIn.size(); ++iDepIn) {
      auto thisInstr = depIn[iDepIn];
      auto pipeName = "pipe_pe_loop_" + std::to_string(iL) + "_dep_in_" +
                      std::to_string(iDepIn) + "_class";

      json::Object depInWriteDirective;
      depInWriteDirective["directive_type"] = "ssa";
      depInWriteDirective["instruction"] = genJsonForInstruction(thisInstr);
      depInWriteDirective["basic_block_idx"] = loopHeaderIdx;
      depInWriteDirective["loop_exit_block_idx"] = loopExitIdx;
      depInWriteDirective["loop_header_block_idx"] = loopHeaderIdx;
      depInWriteDirective["read/write"] = "write";
      depInWriteDirective["kernel_name"] = mainKernelName;
      depInWriteDirective["pipe_name"] = pipeName;
      depInWriteDirective["pipe_type"] = getTypeString(thisInstr);
      depInWriteDirective["pe_type"] = "loop";

      json::Object depInReadDirective;
      depInReadDirective["directive_type"] = "ssa";
      depInReadDirective["instruction"] = genJsonForInstruction(thisInstr);
      depInReadDirective["basic_block_idx"] = loopHeaderIdx;
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
      depOutWriteDirective["basic_block_idx"] = loopHeaderIdx;
      depOutWriteDirective["read/write"] = "write";
      depOutWriteDirective["kernel_name"] = peKernelName;
      depOutWriteDirective["pipe_name"] = pipeName;
      depOutWriteDirective["pipe_type"] = getTypeString(thisInstr);
      depOutWriteDirective["pe_type"] = "loop";

      json::Object depOutReadDirective;
      depOutReadDirective["directive_type"] = "ssa";
      depOutReadDirective["instruction"] = genJsonForInstruction(thisInstr);
      depOutReadDirective["basic_block_idx"] = loopHeaderIdx;
      depOutReadDirective["loop_exit_block_idx"] = loopExitIdx;
      depOutReadDirective["loop_header_block_idx"] = loopHeaderIdx;
      depOutReadDirective["read/write"] = "read";
      depOutReadDirective["kernel_name"] = mainKernelName;
      depOutReadDirective["pipe_name"] = pipeName;
      depOutReadDirective["pipe_type"] = getTypeString(thisInstr);
      depOutReadDirective["pe_type"] = "loop";

      instr2pipeArray.push_back(std::move(depOutWriteDirective));
      instr2pipeArray.push_back(std::move(depOutReadDirective));
    }

    // One predicate pipe per decoupled PE.
    auto pred_pipe_name = "pipe_pe_loop_" + std::to_string(iL) + "_pred_class";
    json::Object predPipeReadDirective;
    predPipeReadDirective["directive_type"] = "pred";
    predPipeReadDirective["pipe_name"] = pred_pipe_name;
    predPipeReadDirective["read/write"] = "read";
    predPipeReadDirective["kernel_name"] = peKernelName;
    predPipeReadDirective["pipe_type"] = "int8_t";
    predPipeReadDirective["basic_block_idx"] = loopHeaderIdx;
    predPipeReadDirective["pe_type"] = "loop";
    json::Object predPipeWriteDirective;
    predPipeWriteDirective["directive_type"] = "pred";
    predPipeWriteDirective["pipe_name"] = pred_pipe_name;
    predPipeWriteDirective["read/write"] = "write";
    predPipeWriteDirective["kernel_name"] = mainKernelName;
    predPipeWriteDirective["pipe_type"] = "int8_t";
    predPipeWriteDirective["basic_block_idx"] = loopHeaderIdx;
    predPipeWriteDirective["loop_exit_block_idx"] = loopExitIdx;
    predPipeWriteDirective["loop_header_block_idx"] = loopHeaderIdx;
    predPipeWriteDirective["pe_type"] = "loop";
    instr2pipeArray.push_back(std::move(predPipeReadDirective));
    instr2pipeArray.push_back(std::move(predPipeWriteDirective));

    json::Object thisLoopOb;
    thisLoopOb["pe_kernel_name"] = peKernelName;
    thisLoopOb["basic_block_idx"] = loopHeaderIdx;
    thisLoopOb["pe_type"] = "loop";
    peArray.push_back(std::move(thisLoopOb));
  }

  report["pe_array"] = std::move(peArray);
  report["instr2pipe_directives"] = std::move(instr2pipeArray);
}

/// Given CDDD analysis information about decoupled basic blocks and loops,
/// update the kernel names for LSQ ld/st value pipe read/writes if they occur
/// in the decoupled blocks. 
void adjustLsqValuePipesPlacement(Function &F,
                                  ControlDependentDataDependencyAnalysis *CDDD,
                                  json::Object &report) {
  auto freshDirectivesArray = json::Array();
  auto blocksToDecouple = CDDD->getBlocksToDecouple();
  auto loopsToDecouple = CDDD->getLoopsToDecouple();
  auto mainKernelName = std::string(*report["main_kernel_name"].getAsString());
  const auto lsqArray = *report.getArray("lsq_array");

  auto newDirectives = json::Array();
  // Ensure only one stValTag directive per PE.
  SetVector<Loop *> createdStValTagDirsForLoopPEs;
  SetVector<BasicBlock *> createdStValTagDirsForBlockPEs;
  // On any given CFG edge, there should be at most one PREDICATE::POISON.
  SetVector<std::pair<int, int>> poisonPredicatesForCFGEdge;

  auto countStoresInBB = [] (int bbIdx, const json::Object &lsqInfo) {
    int count = 0;

    for (auto stIVal : *lsqInfo.getArray("store_instructions")) {
      auto stI = *stIVal.getAsObject();
      if (*stI.getInteger("basic_block_idx") == bbIdx)
        count++;
    }

    return count;
  };

  for (auto i2pInfoVal : *report.getArray("instr2pipe_directives")) {
    auto i2pInfo = *i2pInfoVal.getAsObject();

    bool appendDirectiveToEnd = false;

    if (i2pInfo.getString("directive_type") == "st_val" ||
        i2pInfo.getString("directive_type") == "ld_val" ||
        i2pInfo.getString("directive_type") == "poison") {
      auto instrObj = i2pInfo.getObject("instruction");
      // TODO: flatten instruction object.
      auto bbIdx = (instrObj) ? *instrObj->getInteger("basic_block_idx")
                              : *i2pInfo.getInteger("basic_block_idx");
      auto lsqIdx = *i2pInfo.getInteger("lsq_id");
      auto lsqInfo = *lsqArray[lsqIdx].getAsObject();
      int storesInBB = countStoresInBB(bbIdx, lsqInfo);
      auto pipeBB = getChildWithIndex<Function, BasicBlock>(&F, bbIdx);

      for (size_t iL = 0; iL < loopsToDecouple.size(); ++iL) {
        if (loopsToDecouple[iL]->contains(pipeBB)) {
          auto headerBB = loopsToDecouple[iL]->getHeader();
          auto headerBBIdx = getIndexOfChild(headerBB->getParent(), headerBB);
          auto exitBBIdx = getIndexOfChild(headerBB->getParent(),
                                           loopsToDecouple[iL]->getExitBlock());

          auto peKernelName = mainKernelName + "_PE_LOOP_" + std::to_string(iL);
          i2pInfo["kernel_name"] = peKernelName;

          // If a st_val pipe ends up in a decoupled loop, we might need to send
          // the st_val tag to the decoupled loop, and then get it back. 
          if (i2pInfo.getString("directive_type") == "st_val" &&
              !(*lsqInfo.getBoolean("all_stores_in_same_kernel")) &&
              !createdStValTagDirsForLoopPEs.contains(loopsToDecouple[iL])) {

            auto st_val_tag_in_pipe = "pipe_pe_loop_" + std::to_string(iL) +
                                      "_st_val_tag_in_lsq_" +
                                      std::to_string(lsqIdx) + "_class";
            auto st_val_tag_out_pipe = "pipe_pe_loop_" + std::to_string(iL) +
                                       "_st_val_tag_out_lsq_" +
                                       std::to_string(lsqIdx) + "_class";

            // main -> PE pipe read
            json::Object stValTagPeRdDirective;
            stValTagPeRdDirective["directive_type"] = "st_val_tag";
            stValTagPeRdDirective["pipe_name"] = st_val_tag_in_pipe;
            stValTagPeRdDirective["read/write"] = "read";
            stValTagPeRdDirective["pe_type"] = "loop";
            stValTagPeRdDirective["kernel_name"] = peKernelName;
            stValTagPeRdDirective["pipe_type"] = "uint";
            stValTagPeRdDirective["basic_block_idx"] = headerBBIdx;
            newDirectives.push_back(std::move(stValTagPeRdDirective));

            // main -> PE pipe write
            json::Object stValTagMainWrDirective;
            stValTagMainWrDirective["directive_type"] = "st_val_tag";
            stValTagMainWrDirective["pipe_name"] = st_val_tag_in_pipe;
            stValTagMainWrDirective["read/write"] = "write";
            stValTagMainWrDirective["pe_type"] = "loop";
            stValTagMainWrDirective["kernel_name"] = mainKernelName;
            stValTagMainWrDirective["pipe_type"] = "uint";
            stValTagMainWrDirective["basic_block_idx"] = headerBBIdx;
            newDirectives.push_back(std::move(stValTagMainWrDirective));

            // PE -> main pipe write
            json::Object stValTagPeWrDirective;
            stValTagPeWrDirective["directive_type"] = "st_val_tag";
            stValTagPeWrDirective["pipe_name"] = st_val_tag_out_pipe;
            stValTagPeWrDirective["read/write"] = "write";
            stValTagPeWrDirective["pe_type"] = "loop";
            stValTagPeWrDirective["kernel_name"] = peKernelName;
            stValTagPeWrDirective["pipe_type"] = "uint";
            stValTagPeWrDirective["basic_block_idx"] = exitBBIdx;
            newDirectives.push_back(std::move(stValTagPeWrDirective));

            // PE -> main pipe read
            json::Object stValTagMainRdDirective;
            stValTagMainRdDirective["directive_type"] = "st_val_tag";
            stValTagMainRdDirective["pipe_name"] = st_val_tag_out_pipe;
            stValTagMainRdDirective["read/write"] = "read";
            stValTagMainRdDirective["pe_type"] = "loop";
            stValTagMainRdDirective["kernel_name"] = mainKernelName;
            stValTagMainRdDirective["pipe_type"] = "uint";
            stValTagMainRdDirective["basic_block_idx"] = headerBBIdx;
            newDirectives.push_back(std::move(stValTagMainRdDirective));

            createdStValTagDirsForLoopPEs.insert(loopsToDecouple[iL]);
          }
        }
      }

      // A decoupled block will take precedence over a decoupled loop.
      for (size_t iPE = 0; iPE < blocksToDecouple.size(); ++iPE) {
        if (blocksToDecouple[iPE] == pipeBB) {
          auto peKernelName = mainKernelName + "_PE_BB_" + std::to_string(iPE);
          i2pInfo["kernel_name"] = peKernelName;

          // If a st_val pipe end up in a decoupled BB, we need to send
          // the st_val tag to the decoupled BB. We don't need to read it back,
          // since we can just increment it by the number of stores in the BB.
          if (i2pInfo.getString("directive_type") == "st_val" &&
              !(*lsqInfo.getBoolean("all_stores_in_same_kernel")) &&
              !createdStValTagDirsForBlockPEs.contains(pipeBB)) {
            auto st_val_tag_pipe = "pipe_pe_block_" + std::to_string(iPE) +
                                   "_st_val_tag_in_lsq_" +
                                   std::to_string(lsqIdx) + "_class";

            // Main --> PE st_val_tag write
            json::Object stValTagPipeReadDirective;
            stValTagPipeReadDirective["directive_type"] = "st_val_tag";
            stValTagPipeReadDirective["pipe_name"] = st_val_tag_pipe;
            stValTagPipeReadDirective["read/write"] = "read";
            stValTagPipeReadDirective["pe_type"] = "block";
            stValTagPipeReadDirective["kernel_name"] = peKernelName;
            stValTagPipeReadDirective["pipe_type"] = "uint";
            stValTagPipeReadDirective["basic_block_idx"] = bbIdx;
            json::Object stValTagPipeWriteDirective;
            stValTagPipeWriteDirective["directive_type"] = "st_val_tag";
            stValTagPipeWriteDirective["pipe_name"] = st_val_tag_pipe;
            stValTagPipeWriteDirective["read/write"] = "write";
            stValTagPipeWriteDirective["pe_type"] = "block";
            stValTagPipeWriteDirective["kernel_name"] = mainKernelName;
            stValTagPipeWriteDirective["pipe_type"] = "uint";
            stValTagPipeWriteDirective["basic_block_idx"] = bbIdx;
            stValTagPipeWriteDirective["stores_in_bb"] = storesInBB;

            newDirectives.push_back(std::move(stValTagPipeReadDirective));
            newDirectives.push_back(std::move(stValTagPipeWriteDirective));
            
            // Ensure we only communicate the tag once.
            createdStValTagDirsForBlockPEs.insert(pipeBB);
          }

          // If a poison happens in a dcpld BB...
          if (i2pInfo.getString("directive_type") == "poison") {
            // Then the poison BB in the main kernel will supply a specific
            // PEDICATE to the PE that will cause the PE to supply the poison.
            std::pair<int, int> edgeCFG {
                *i2pInfo.getInteger("pred_basic_block_idx"),
                *i2pInfo.getInteger("succ_basic_block_idx")};
            if (!poisonPredicatesForCFGEdge.contains(edgeCFG)) {
              json::Object predPoisonDir;
              predPoisonDir["directive_type"] = "pred_poison";
              predPoisonDir["basic_block_idx"] =
                  *i2pInfo.getInteger("basic_block_idx");
              predPoisonDir["kernel_name"] = mainKernelName;
              predPoisonDir["lsq_id"] = *i2pInfo.getInteger("lsq_id");
              predPoisonDir["pipe_name"] = 
                  "pipe_pe_bb_" + std::to_string(iPE) + "_pred_class";
              predPoisonDir["pipe_type"] = "int8_t";
              predPoisonDir["read/write"] = "write";
              predPoisonDir["stores_in_bb"] = storesInBB; // To incr stValTag
              predPoisonDir["num_store_pipes"] =
                  *i2pInfo.getInteger("num_store_pipes");
              predPoisonDir["pred_basic_block_idx"] =
                  *i2pInfo.getInteger("pred_basic_block_idx");
              predPoisonDir["succ_basic_block_idx"] =
                  *i2pInfo.getInteger("succ_basic_block_idx");
              newDirectives.push_back(std::move(predPoisonDir));

              poisonPredicatesForCFGEdge.insert(edgeCFG);
            }

            // And the actual poison pipe call will happen in the PE.
            i2pInfo["directive_type"] = "poison_in_bb_pe";
            i2pInfo["kernel_name"] = peKernelName;
            appendDirectiveToEnd = true;
          }
        }
      }
    }

    // TODO: specify a partial order of directive_types, and sort at the end.
    if (appendDirectiveToEnd)
      newDirectives.push_back(std::move(i2pInfo));
    else
      freshDirectivesArray.push_back(std::move(i2pInfo));
  }

  // Append st_val_tag directives at the end.
  for (auto i2pInfoVal : newDirectives) {
    auto i2pInfo = *i2pInfoVal.getAsObject();
    freshDirectivesArray.push_back(std::move(i2pInfo));
  }

  report["instr2pipe_directives"] = std::move(freshDirectivesArray);
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

      // TODO: save all info in a struct and only write out to json at the end
      //       to avoid needless copying of the {report} json object.
      lsqReportPrinter(DHA, CDDD, report);
      decoupledBlocksReportPrinter(CDDD, LI, report);
      decoupledLoopsReportPrinter(CDDD, report);
      // If a LSQ ld/st value pipe ends up in a decoupled kernel, then we
      // need to update the associated instr2pipe directive kernel name.
      adjustLsqValuePipesPlacement(F, CDDD, report);

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