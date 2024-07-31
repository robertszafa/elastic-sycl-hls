/// This file is the analysis driver. It uses DataHazardAnalysis and
/// CDDDAnalysis to identify memory instructions, basic blocks, and whole loops
/// which can benefit from dynamic scheduling.

#include "CommonLLVM.h"
#include "AnalysisReportSchema.h"
#include "DataHazardAnalysis.h"
#include "CDDDAnalysis.h"

using namespace llvm;

namespace llvm {

/// Given a basic block, return the name of the kernel that contains it.
std::string getKernelName(BasicBlock *BB, const SmallVector<PEInfo> &peArray) {
  // If the BB is decoupled into a block PE, then return its name.
  for (auto &peInfo : peArray) {
    if (peInfo.peType == BLOCK && peInfo.basicBlock == BB)
      return peInfo.peKernelName;
  }
  // Otherwise, if the BB is part of a loop PE, then return its name.
  for (auto &peInfo : peArray) {
    if (peInfo.peType == LOOP && peInfo.loop->contains(BB))
      return peInfo.peKernelName;
  }
  // Otherwise, the BB will not be decoupled. Return the original kernel name.
  return demangle(std::string(BB->getParent()->getName()));
}

/// Fill the lsqArray with info about LSQs to generate.
/// Fill the rewriteRules with info about transformations needed to use the LSQ.
void generateInfoLSQ(Function &F, LoopInfo &LI, DataHazardAnalysis *DHA,
                     ControlDependentDataDependencyAnalysis *CDDD,
                     const SmallVector<PEInfo> &peArray,
                     SmallVector<LSQInfo> &lsqArray,
                     SmallVector<RewriteRule> &rewriteRules) {
  auto mainKernelName = demangle(std::string(F.getName()));

  /// We cannot reuse the same LSQ pipe across BBs if some of the BBs get
  /// decoupled into different kernels.
  auto canReuseLSQPipes = [&](SmallVector<Instruction *> &memOps) -> bool {
    // We will count the unique blocks where any of the memOps are used.
    SetVector<BasicBlock *> peUse, mainUse;
    for (auto &memOp : memOps) {
      auto BB = memOp->getParent();
      if (getKernelName(BB, peArray) == mainKernelName)
        mainUse.insert(BB);
      else
        peUse.insert(BB);
    }
    // Can reuse if nothing decoupled, or everything decoupled into one PE.
    return (peUse.size() == 0) || (peUse.size() == 1 && mainUse.size() == 0);
  };

  /// Given a range of instructions, for each I return its position in its basic
  /// block relative to all other instructions in the range.
  auto getMaxMemOpsInAnyLoop = [&LI](const SmallVector<Instruction *> &Range) {
    SmallMapVector<Loop *, int, 4> seen;
    int maxSoFar = 1;
    for (auto &I : Range) {
      auto L = LI.getLoopFor(I->getParent());
      if (L && seen.contains(L))
        seen[L]++;
      else if (L)
        seen[L] = 1;

      maxSoFar = (seen[L] > maxSoFar) ? seen[L] : maxSoFar;
    }

    return maxSoFar;
  };

  auto getAguKernelName = [&](LSQInfo &lsqInfo) -> std::string {
    const bool useOneAgu = std::getenv("USE_ONE_AGU") &&
                           strcmp(std::getenv("USE_ONE_AGU"), "1") == 0;
    std::string AguIdx = useOneAgu ? "" : "_" + std::to_string(lsqInfo.lsqIdx);
    return lsqInfo.isAddressGenDecoupled ? mainKernelName + "_AGU" + AguIdx
                                         : mainKernelName;
  };

  for (size_t iLSQ = 0; iLSQ < DHA->getHazardInstructions().size(); ++iLSQ) {
    // First collect some info about the memory accesses for this LSQ.
    auto instrCluster = DHA->getHazardInstructions()[iLSQ];
    SmallVector<Instruction *> loads, stores;
    for (size_t i = 0; i < instrCluster.size(); ++i) {
      if (isaLoad(instrCluster[i])) 
        loads.push_back(instrCluster[i]);
      else
        stores.push_back(instrCluster[i]);
    }
    // LSQ ld/st pipes are represented as an array of pipes (name + idx).
    // If we can reuse pipes across loops, then we need to keep track of the
    // indexes, otherwsise we can use a unique pipe for every ld/st. First we 
    // check, if all loops where the pipe array is used, are in the same kernel.
    const bool reuseLdPipesLSQ = canReuseLSQPipes(loads);
    const bool reuseStPipesLSQ = canReuseLSQPipes(stores);
    int maxLdsInLoop = getMaxMemOpsInAnyLoop(loads);
    int maxStsInLoop = getMaxMemOpsInAnyLoop(stores);
    // Mapping used to reuse LSQ pipes across loops.
    MapVector<Loop *, int> loadsPerLoop, storesPerLoop;

    // General info about the LSQ as a whole.
    LSQInfo lsqInfo;
    lsqInfo.lsqIdx = iLSQ;
    lsqInfo.numLoadPipes = reuseLdPipesLSQ ? maxLdsInLoop : int(loads.size());
    lsqInfo.numStorePipes = reuseStPipesLSQ ? maxStsInLoop : int(stores.size());
    lsqInfo.reuseLdPipesAcrossBB = reuseLdPipesLSQ;
    lsqInfo.reuseStPipesAcrossBB = reuseStPipesLSQ;
    lsqInfo.isOnChipMem = DHA->getIsOnChip()[iLSQ];
    lsqInfo.allocationQueueSize = DHA->getStoreQueueSizes()[iLSQ];
    lsqInfo.isAddressGenDecoupled = DHA->getDecoupligDecisions()[iLSQ];
    lsqInfo.isAnySpeculation = DHA->getSpeculationDecisions()[iLSQ];
    lsqInfo.arraySize = DHA->getMemorySizes()[iLSQ];
    lsqInfo.arrayType = getLLVMTypeString(dyn_cast<Instruction>(loads[0]));
    lsqInfo.aguKernelName = getAguKernelName(lsqInfo);
    lsqArray.push_back(lsqInfo);

    // Now generate rewrite rules needed to swap load/store instructions with
    // pipe read/writes, and to implement speculated LSQ allocations, if needed.

    // Loads
    for (size_t iLd = 0; iLd < loads.size(); ++iLd) {
      // {loadsPerBB} is only used when reusePipesLSQ is true.
      auto ldBB = loads[iLd]->getParent();
      auto ldLoop = LI.getLoopFor(ldBB);
      if (!loadsPerLoop.contains(ldLoop))
        loadsPerLoop[ldLoop] = 0;
      int pipeIdx = reuseLdPipesLSQ ? loadsPerLoop[ldLoop]++ : iLd;

      RewriteRule ldReq{LD_REQ_WRITE, lsqInfo.aguKernelName, loads[iLd], ldBB};
      ldReq.lsqIdx = iLSQ;
      ldReq.pipeName = "pipes_ld_req_" + std::to_string(iLSQ) + "_class";
      ldReq.pipeType =
          lsqInfo.isOnChipMem ? "ld_req_lsq_bram_t" : "req_lsq_dram_t";
      ldReq.pipeArrayIdx = pipeIdx;
      ldReq.specBasicBlock = DHA->getSpeculationBlock(loads[iLd]);

      auto ldKernelName = getKernelName(loads[iLd]->getParent(), peArray);
      RewriteRule ldVal{LD_VAL_READ, ldKernelName, loads[iLd], ldBB};
      ldVal.lsqIdx = iLSQ;
      ldVal.pipeName = "pipes_ld_val_" + std::to_string(iLSQ) + "_class";
      ldVal.pipeType = lsqInfo.arrayType;
      ldVal.pipeArrayIdx = pipeIdx;
      
      // If the speculated load is used in another PE, then add a poison load.
      // Otherwise, hoist the load to the same location where it is speculated.
      if (!CDDD->getBlocksToDecouple().contains(ldVal.basicBlock)) {
        ldVal.specBasicBlock = DHA->getSpeculationBlock(loads[iLd]);
      } else {
        // If loads cannot be reordered for some reason, then we can also insert 
        // poison blocks (same as for stores).
        if (auto specBB = DHA->getSpeculationBlock(loads[iLd])) {
          for (auto [predBB, succBB] : DHA->getPoisonLocations(loads[iLd])) {
            RewriteRule poison{POISON_LD_READ, mainKernelName, loads[iLd], ldBB};
            poison.lsqIdx = iLSQ;
            poison.pipeName = "pipes_ld_val_" + std::to_string(iLSQ) + "_class";
            poison.predBasicBlock = predBB;
            poison.succBasicBlock = succBB;
            poison.specBasicBlock = specBB;
            poison.pipeArrayIdx = pipeIdx;

            rewriteRules.push_back(poison);
          }
        } 
      }

      rewriteRules.push_back(ldReq);
      rewriteRules.push_back(ldVal);
    }

    // Stores
    for (size_t iSt = 0; iSt < stores.size(); ++iSt) {
      // {storesPerBB} i only used when reusePipesLSQ is true.
      auto stBB = stores[iSt]->getParent();
      auto stLoop = LI.getLoopFor(stBB);
      if (!storesPerLoop.contains(stLoop))
        storesPerLoop[stLoop] = 0;
      int pipeIdx = reuseStPipesLSQ ? storesPerLoop[stLoop]++ : iSt;

      RewriteRule stReq{ST_REQ_WRITE, lsqInfo.aguKernelName, stores[iSt], stBB};
      stReq.lsqIdx = iLSQ;
      stReq.pipeName = "pipes_st_req_" + std::to_string(iLSQ) + "_class";
      stReq.pipeType =
          lsqInfo.isOnChipMem ? "st_req_lsq_bram_t" : "st_req_lsq_dram_t";
      stReq.pipeArrayIdx = pipeIdx;
      stReq.specBasicBlock = DHA->getSpeculationBlock(stores[iSt]);

      RewriteRule stVal{ST_VAL_WRITE, mainKernelName, stores[iSt], stBB};
      stVal.lsqIdx = iLSQ;
      stVal.pipeName = "pipes_st_val_" + std::to_string(iLSQ) + "_class";
      stVal.pipeType = lsqInfo.isOnChipMem
                           ? "tagged_val_lsq_bram_t<" + lsqInfo.arrayType + ">"
                           : "tagged_val_lsq_dram_t<" + lsqInfo.arrayType + ">";
      stVal.pipeArrayIdx = pipeIdx;

      rewriteRules.push_back(stReq);
      rewriteRules.push_back(stVal);
      
      if (auto specBB = DHA->getSpeculationBlock(stores[iSt])) {
        for (auto [predBB, succBB] : DHA->getPoisonLocations(stores[iSt])) {
          RewriteRule poison{POISON_ST_WRITE, mainKernelName, stores[iSt], stBB};
          poison.lsqIdx = iLSQ;
          poison.pipeName = "pipes_st_val_" + std::to_string(iLSQ) + "_class";
          poison.predBasicBlock = predBB;
          poison.succBasicBlock = succBB;
          poison.specBasicBlock = specBB;
          poison.pipeArrayIdx = pipeIdx;

          rewriteRules.push_back(poison);
        }
      } 
    }

    // At the end of the function, each LSQ needs an "end signal".
    auto funcExit = getReturnBlock(F);
    RewriteRule endSignal{END_LSQ_SIGNAL_WRITE, mainKernelName,
                          funcExit->getTerminator(), funcExit};
    endSignal.pipeName =
        "pipe_end_lsq_signal_" + std::to_string(iLSQ) + "_class";
    endSignal.pipeType = "bool";
    rewriteRules.push_back(endSignal);
  }
}

/// Fill the peArray with info about block PEs to generate.
/// Fill the rewriteRules with info about transformations needed to use the PE.
void generateInfoBlockPE(Function &F, LoopInfo &LI,
                         ControlDependentDataDependencyAnalysis *CDDD,
                         SmallVector<PEInfo> &peArray,
                         SmallVector<RewriteRule> &rewriteRules) {
  auto mainKernelName = demangle(std::string(F.getName()));
  auto blocksToDecouple = CDDD->getBlocksToDecouple();
  for (size_t iBB = 0; iBB < blocksToDecouple.size(); ++iBB) {
    auto peKernelName = mainKernelName + BLOCK_PE_ID + std::to_string(iBB);
    auto BB = blocksToDecouple[iBB];
    auto L = LI.getLoopFor(BB);
    auto loopHeader = L->getHeader();
    auto loopLatch = L->getLoopLatch();
    auto loopExit = L->getExitBlock();

    peArray.push_back({PE_TYPE::BLOCK, peKernelName, BB});
    const int peIdx = peArray.size() - 1;

    auto dependenciesToHoist = CDDD->getInstructionsToHoist(BB);
    auto recStart = CDDD->getRecurrenceStart(BB);
    auto recEnd = CDDD->getRecurrenceEnd(BB);

    // MainKernel --> blockPE dependencies
    auto depIn = CDDD->getBlockInputDependencies(BB);
    for (size_t iDep = 0; iDep < depIn.size(); ++iDep) {
      RewriteRule ssaInWr{SSA_BB_IN_WRITE, mainKernelName, depIn[iDep], BB};
      ssaInWr.pipeName = "pipe_pe_" + std::to_string(peIdx) + "_bb_dep_in_" +
                         std::to_string(iDep) + "_class";
      ssaInWr.pipeType = getLLVMTypeString(depIn[iDep]);
      ssaInWr.loopHeaderBlock = loopHeader;
      ssaInWr.loopLatchBlock = loopLatch;
      ssaInWr.loopExitBlock = loopExit;
      ssaInWr.isHoistedOutOfLoop = dependenciesToHoist.contains(depIn[iDep]);
      ssaInWr.recurrenceStart = recStart;
      ssaInWr.recurrenceEnd = recEnd;
      ssaInWr.peIdx = peIdx;

      RewriteRule ssaInRd{ssaInWr};
      ssaInRd.ruleType = SSA_BB_IN_READ;
      ssaInRd.kernelName = peKernelName;

      rewriteRules.push_back(ssaInWr);
      rewriteRules.push_back(ssaInRd);
    }

    // blockPE --> MainKernel dependencies
    auto depOut = CDDD->getBlockOutputDependencies(BB);
    for (size_t iDep = 0; iDep < depOut.size(); ++iDep) {
      RewriteRule ssaOutWr{SSA_BB_OUT_WRITE, peKernelName, depOut[iDep], BB};
      ssaOutWr.pipeName = "pipe_pe_" + std::to_string(peIdx) + "_bb_dep_out_" +
                          std::to_string(iDep) + "_class";
      ssaOutWr.pipeType = getLLVMTypeString(depOut[iDep]);
      ssaOutWr.loopHeaderBlock = loopHeader;
      ssaOutWr.loopLatchBlock = loopLatch;
      ssaOutWr.loopExitBlock = loopExit;
      ssaOutWr.isHoistedOutOfLoop = dependenciesToHoist.contains(depOut[iDep]);
      ssaOutWr.recurrenceStart = recStart;
      ssaOutWr.recurrenceEnd = recEnd;
      ssaOutWr.peIdx = peIdx;

      RewriteRule ssaOutRd{ssaOutWr};
      ssaOutRd.ruleType = SSA_BB_OUT_READ;
      ssaOutRd.kernelName = mainKernelName;

      rewriteRules.push_back(ssaOutWr);
      rewriteRules.push_back(ssaOutRd);
    }

    // One predicate pipe per decoupled block PE.
    RewriteRule predWr{PRED_BB_WRITE, mainKernelName, BB->getFirstNonPHI(), BB};
    predWr.loopHeaderBlock = loopHeader;
    predWr.loopLatchBlock = loopLatch;
    predWr.loopExitBlock = loopExit;
    predWr.pipeName = "pipe_pe_" + std::to_string(peIdx) + "_bb_pred_class";
    predWr.pipeType = "int8_t";
    predWr.peIdx = peIdx;
    RewriteRule predRd{predWr};
    predRd.ruleType = PRED_BB_READ;
    predRd.kernelName = peKernelName;
    rewriteRules.push_back(predWr);
    rewriteRules.push_back(predRd);
  }
}

/// Fill the peArray with info about loop PEs to generate.
/// Fill the rewriteRules with info about transformations needed to use the PE.
void generateInfoLoopPE(Function &F,
                        ControlDependentDataDependencyAnalysis *CDDD,
                        SmallVector<PEInfo> &peArray,
                        SmallVector<RewriteRule> &rewriteRules) {
  auto mainKernelName = demangle(std::string(F.getName()));
  auto loopsToDecouple = CDDD->getLoopsToDecouple();
  for (size_t iL = 0; iL < loopsToDecouple.size(); ++iL) {
    auto peKernelName = mainKernelName + LOOP_PE_ID + std::to_string(iL);
    auto L = loopsToDecouple[iL];
    // All rewrite rules for loop PEs have the loop header as is "basicBlock".
    auto loopHeader = L->getHeader();
    auto loopLatch = L->getLoopLatch();
    auto loopExit = L->getExitBlock();

    peArray.push_back({PE_TYPE::LOOP, peKernelName, loopHeader, L});
    const int peIdx = peArray.size() - 1;

    // MainKernel --> loopPE dependencies
    auto depIn = CDDD->getLoopInputDependencies(L);
    for (size_t iDep = 0; iDep < depIn.size(); ++iDep) {
      RewriteRule ssaInWr{SSA_LOOP_IN_WRITE, mainKernelName, depIn[iDep],
                          loopHeader};
      ssaInWr.pipeName = "pipe_pe_" + std::to_string(peIdx) + "_loop_dep_in_" +
                         std::to_string(iDep) + "_class";
      ssaInWr.pipeType = getLLVMTypeString(depIn[iDep]);
      ssaInWr.loopHeaderBlock = loopHeader;
      ssaInWr.loopLatchBlock = loopLatch;
      ssaInWr.loopExitBlock = loopExit;
      ssaInWr.peIdx = peIdx;

      RewriteRule ssaInRd{ssaInWr};
      ssaInRd.ruleType = SSA_LOOP_IN_READ;
      ssaInRd.kernelName = peKernelName;

      rewriteRules.push_back(ssaInWr);
      rewriteRules.push_back(ssaInRd);     
    }

    // loopPE --> MainKernel dependencies
    auto depOut = CDDD->getLoopOutputDependencies(L);
    for (size_t iDep = 0; iDep < depOut.size(); ++iDep) {
      RewriteRule ssaOutWr{SSA_LOOP_OUT_WRITE, peKernelName, depOut[iDep],
                           loopHeader};
      ssaOutWr.pipeName = "pipe_pe_" + std::to_string(peIdx) +
                          "_loop_dep_out_" + std::to_string(iDep) + "_class";
      ssaOutWr.pipeType = getLLVMTypeString(depOut[iDep]);
      ssaOutWr.loopHeaderBlock = loopHeader;
      ssaOutWr.loopLatchBlock = loopLatch;
      ssaOutWr.loopExitBlock = loopExit;
      ssaOutWr.peIdx = peIdx;

      RewriteRule ssaOutRd{ssaOutWr};
      ssaOutRd.ruleType = SSA_LOOP_OUT_READ;
      ssaOutRd.kernelName = mainKernelName;

      rewriteRules.push_back(ssaOutWr);
      rewriteRules.push_back(ssaOutRd);     
    }

    // One predicate pipe per decoupled loop PE.
    RewriteRule predWr{PRED_LOOP_WRITE, mainKernelName,
                       loopHeader->getFirstNonPHI(), loopHeader};
    predWr.loopHeaderBlock = loopHeader;
    predWr.loopLatchBlock = loopLatch;
    predWr.loopExitBlock = loopExit;
    predWr.pipeType = "int8_t";
    predWr.pipeName = "pipe_pe_" + std::to_string(peIdx) + "_loop_pred_class";
    predWr.peIdx = peIdx;
    RewriteRule predRd{predWr};
    predRd.ruleType = PRED_LOOP_READ;
    predRd.kernelName = peKernelName;
    rewriteRules.push_back(predWr);
    rewriteRules.push_back(predRd);
  }
}

/// Given information about decoupled memory instructions, basic blocks, and
/// loops ensure that the pipe calls specified by the rewrite rules are placed
/// in the correct kernel. Some compositions require the creation of additional
/// rewrite rules (e.g. to communicate a store value tag across kernels).
void composeDecoupledKernels(Function &F, DataHazardAnalysis &DHA,
                             ControlDependenceGraph &CDG, ScalarEvolution &SE,
                             LoopInfo &LI, SmallVector<LSQInfo> &lsqArray,
                             SmallVector<PEInfo> &peArray,
                             SmallVector<RewriteRule> &rewriteRules) {
  /// Return the number of stores to {lsqIdx} in {BB}.
  auto countStoresInBB = [&rewriteRules](int lsqIdx, BasicBlock *BB) {
    int count = 0;
    for (auto &rule : rewriteRules) {
      if (rule.ruleType == ST_VAL_WRITE && rule.lsqIdx == lsqIdx &&
          rule.basicBlock == BB) {
        count++;
      }
    }
    return count;
  };

  /// Return the number of {lsqIdx} stores in {L}. Return -1, if uncertain.
  auto countUncondStoresInLoop = [&rewriteRules, &CDG] (Loop *L, int lsqIdx) {
    int count = 0;
    for (auto &rule : rewriteRules) {
      if (rule.ruleType == ST_VAL_WRITE && rule.lsqIdx == lsqIdx &&
          L->contains(rule.basicBlock)) {
        count++;
        if (CDG.getControlDependencySource(rule.basicBlock) != L->getHeader())
          return -1; // Conditional store.
      }
    }
    return count;
  };

  /// Return the index into the peArray of the PE with {peName}.
  auto getPeIdx = [&peArray](const std::string &peName) -> int {
    for (size_t iPE = 0; iPE < peArray.size(); ++iPE) {
      if (peArray[iPE].peKernelName == peName)
        return iPE;
    }
    return -1;
  };

  auto isLoopPE = [](std::string &N) {
    return N.find(LOOP_PE_ID) != std::string::npos;
  };
  auto isBlockPE = [](std::string &N) {
    return N.find(BLOCK_PE_ID) != std::string::npos;
  };

  // Ensure only one POISON_IN_BB_PE per instruction directive in block PE.
  SetVector<Instruction *> poisonsInBlockPE;
  // Ensure only one ST_VAL_TAG_TO_BLOCK_WRITE per lsqIdx & per block PE.
  SetVector<std::pair<int, BasicBlock *>> stValTagInBlockPE;
  // Ensure only one POISON_PRED_WRITE per poison block.
  SetVector<std::pair<int, CFGEdge>> poisonPredWrPoisonBlock;
  // Ensure only one ST_VAL_TAG_TO_BLOCK_WRITE per poison block.
  SetVector<std::pair<int, CFGEdge>> stValTagWrPoisonBlock;
  // Ensure only one ST_VAL_TAG_IN_READ for poison per in blockPE.
  SetVector<int> stValTagRdPoisonBlock;
  // Ensure only one ST_VAL_TAG_TO_LOOP_WRITE per lsqIdx & loop PE.
  SetVector<std::pair<int, Loop *>> stValTagInLoopPE;

  // For each rule, check if its kerneName should be changed.
  for (auto &rule : rewriteRules) {
    auto L = LI.getLoopFor(rule.basicBlock);
    auto newKernelName = getKernelName(rule.basicBlock, peArray);
    auto peIdx = getPeIdx(newKernelName);
    // If the BB for this rule is not in a loop, then no change.
    auto parentKernelName =
        L ? getKernelName(L->getHeader(), peArray) : rule.kernelName;

    if (rule.ruleType == LD_VAL_READ || rule.ruleType == ST_VAL_WRITE) {
      // LSQ ld/st decoupled into a block or loop PE.
      rule.kernelName = newKernelName;

      // If stores happen across kernels, then need to send stValTag.
      if (isBlockPE(newKernelName) && rule.ruleType == ST_VAL_WRITE &&
          !lsqArray[rule.lsqIdx].reuseStPipesAcrossBB &&
          !stValTagInBlockPE.contains({rule.lsqIdx, rule.basicBlock})) {
        auto stValTagPipeName = "pipe_pe_" + std::to_string(peIdx) +
                                "_bb_st_val_tag_in_lsq_" +
                                std::to_string(rule.lsqIdx) + "_class";

      RewriteRule stValTagWr{rule};
      stValTagWr.ruleType = ST_VAL_TAG_TO_BB_WRITE;
      stValTagWr.kernelName = parentKernelName;
      stValTagWr.pipeName = stValTagPipeName;
      stValTagWr.pipeType = "uint";
      stValTagWr.pipeArrayIdx = -1;
      stValTagWr.peIdx = peIdx;
      stValTagWr.numStoresInBlock =
          countStoresInBB(rule.lsqIdx, rule.basicBlock);
      rewriteRules.push_back(stValTagWr);

      RewriteRule stValTagRd{stValTagWr};
      stValTagRd.ruleType = ST_VAL_TAG_IN_READ;
      stValTagRd.kernelName = newKernelName;
      rewriteRules.push_back(stValTagRd);

      stValTagInBlockPE.insert({rule.lsqIdx, rule.basicBlock});
      } else if (isLoopPE(newKernelName) && rule.ruleType == ST_VAL_WRITE &&
                 !lsqArray[rule.lsqIdx].reuseStPipesAcrossBB &&
                 !stValTagInLoopPE.contains({rule.lsqIdx, L})) {
        auto L = LI.getLoopFor(rule.basicBlock);
        auto stValTagPipeName = "pipe_pe_" + std::to_string(peIdx) +
                                "_loop_st_val_tag_in_lsq_" +
                                std::to_string(rule.lsqIdx) + "_class";

        RewriteRule stValTagInWr{rule};
        stValTagInWr.ruleType = ST_VAL_TAG_TO_LOOP_WRITE;
        stValTagInWr.kernelName = parentKernelName;
        stValTagInWr.basicBlock = L->getHeader();
        stValTagInWr.pipeName = stValTagPipeName;
        stValTagInWr.pipeType = "uint";
        stValTagInWr.pipeArrayIdx = -1;
        stValTagInWr.numStoresInLoop = countUncondStoresInLoop(L, rule.lsqIdx);
        // Check if we can directly calculate the number of stores in the loop.
        stValTagInWr.canBuildNumStoresInLoopExpr =
            (stValTagInWr.numStoresInLoop != -1) &&
            SE.hasLoopInvariantBackedgeTakenCount(L) &&
            (buildSCEVExpr(F, SE.getBackedgeTakenCount(L),
                           L->getHeader()->getTerminator()) != nullptr);
        rewriteRules.push_back(stValTagInWr);

        RewriteRule stValTagInRd{stValTagInWr};
        stValTagInRd.ruleType = ST_VAL_TAG_IN_READ;
        stValTagInRd.kernelName = newKernelName;
        stValTagInRd.peIdx = peIdx;
        rewriteRules.push_back(stValTagInRd);
        
        // If we can't, then need to communicate stValTag back from loopPE.
        if (!stValTagInWr.canBuildNumStoresInLoopExpr) {
          RewriteRule stValTagOutWr{stValTagInRd};
          stValTagOutWr.ruleType = ST_VAL_TAG_LOOP_OUT_WRITE;
          stValTagOutWr.basicBlock = L->getExitBlock();
          rewriteRules.push_back(stValTagOutWr);

          RewriteRule stValTagOutRd{stValTagInWr};
          stValTagOutWr.ruleType = ST_VAL_TAG_IN_READ;
          stValTagOutWr.kernelName = parentKernelName;
          stValTagOutWr.basicBlock = L->getExitBlock();
          rewriteRules.push_back(stValTagOutRd);
        }

        stValTagInLoopPE.insert({rule.lsqIdx, LI.getLoopFor(rule.basicBlock)});
      }
    } else if (rule.ruleType == POISON_LD_READ ||
               rule.ruleType == POISON_ST_WRITE) {
      rule.kernelName = newKernelName;

      if (isBlockPE(newKernelName)) {
        const bool isaStore = rule.ruleType == POISON_ST_WRITE;
        CFGEdge poisonBB = {rule.predBasicBlock, rule.succBasicBlock};

        // 1) Then create a new POISON_IN_BB_PE rewrite rule (but only one).
        if (!poisonsInBlockPE.contains(rule.instruction)) {
          RewriteRule poisonInBB{rule};
          poisonInBB.ruleType =
              isaStore ? POISON_IN_BB_PE_ST_WRITE : POISON_IN_BB_PE_LD_READ;
          poisonInBB.peIdx = peIdx;

          peArray[peIdx].needsPoisonBlock = true;

          rewriteRules.push_back(poisonInBB);
          poisonsInBlockPE.insert(rule.instruction);
        }

        // 2) Change this rule to send a poisonPredicate to the blockPE. The
        // predicate should be sent from the kernel that contains the blockPE.
        if (!poisonPredWrPoisonBlock.contains({rule.lsqIdx, poisonBB})) {
          rule.kernelName = parentKernelName;
          rule.ruleType = POISON_PRED_BB_WRITE;
          rule.pipeName = "pipe_pe_" + std::to_string(peIdx) + "_bb_pred_class";
          rule.pipeType = "int8_t";
          rule.pipeArrayIdx = -1;
          rule.numStoresInBlock = countStoresInBB(rule.lsqIdx, rule.basicBlock);

          poisonPredWrPoisonBlock.insert({rule.lsqIdx, poisonBB});
        } else {
          // If a poisonPred already exists for this block, then drop the rule.
          rule.ruleType = UNDEF;
        }

        // 3) Deal with stValTag communication for poisoned blocks. 
        if (isaStore &&
            !stValTagWrPoisonBlock.contains({rule.lsqIdx, poisonBB}) &&
            !lsqArray[rule.lsqIdx].reuseStPipesAcrossBB) {
          auto stValTagPipeName = "pipe_pe_" + std::to_string(peIdx) +
                                  "_bb_st_val_tag_in_lsq_" +
                                  std::to_string(rule.lsqIdx) + "_class";

          RewriteRule stValTagWr{rule};
          stValTagWr.ruleType = ST_VAL_TAG_TO_BB_WRITE;
          stValTagWr.kernelName = parentKernelName;
          stValTagWr.pipeName = stValTagPipeName;
          stValTagWr.pipeType = "uint";
          stValTagWr.pipeArrayIdx = -1;
          stValTagWr.peIdx = peIdx;
          stValTagWr.numStoresInBlock =
              countStoresInBB(rule.lsqIdx, rule.basicBlock);
          rewriteRules.push_back(stValTagWr);
          
          stValTagWrPoisonBlock.insert({rule.lsqIdx, poisonBB});

          // There can be multiple poisonBBs in the parentKernel CFG where a
          // ST_VAL_WR happens, but in the block PE, there will be only one
          // poisonBB, and so only one read is needed.
          if (!stValTagRdPoisonBlock.contains(rule.lsqIdx)) {
            RewriteRule stValTagRd{stValTagWr};
            stValTagRd.ruleType = ST_VAL_TAG_IN_READ;
            stValTagRd.kernelName = newKernelName;
            rewriteRules.push_back(stValTagRd);

            stValTagRdPoisonBlock.insert(rule.lsqIdx);
          }
        }
      }
    } else if (rule.ruleType == SSA_BB_IN_WRITE ||
               rule.ruleType == SSA_BB_OUT_READ ||
               rule.ruleType == PRED_BB_WRITE) {
      rule.kernelName = parentKernelName;
    } else if (rule.ruleType == SSA_LOOP_IN_WRITE ||
               rule.ruleType == SSA_LOOP_OUT_READ ||
               rule.ruleType == PRED_LOOP_WRITE) {
      // A loopPE is activated by another loopPE (i.e. its parent loop).
      if (auto parentLoop = L->getParentLoop())
        rule.kernelName = getKernelName(parentLoop->getHeader(), peArray);
    }
  }
}

/// Generate a report for memory instructions that form 
/// a RAW inter-iteration data hazard.
struct ElasticAnalysisPrinter : PassInfoMixin<ElasticAnalysisPrinter> {

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    std::string thisKernelName = demangle(std::string(F.getNameOrAsOperand()));
    bool isMain = thisKernelName.find("MainKernel") < thisKernelName.size();

    if (F.getCallingConv() == CallingConv::SPIR_KERNEL && isMain) {
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
      // Get all control-dependent data dependencies that increase the
      // recurrence constrained initiation interval of a loop.
      auto *CDDD = new ControlDependentDataDependencyAnalysis(LI, *DDG, *CDG);

      // These structs define the analysis/transformation interface.
      SmallVector<LSQInfo> lsqArray;
      SmallVector<PEInfo> peArray;
      SmallVector<RewriteRule> rewriteRules;

      generateInfoBlockPE(F, LI, CDDD, peArray, rewriteRules);
      generateInfoLoopPE(F, CDDD, peArray, rewriteRules);
      // Do LSQ at the end, to check if LSQ is used across kernels.
      generateInfoLSQ(F, LI, DHA, CDDD, peArray, lsqArray, rewriteRules);

      composeDecoupledKernels(F, *DHA, *CDG, SE, LI, lsqArray, peArray, rewriteRules);

      // Remove UNDEF rules and ensure deterministic order.
      llvm::remove_if(rewriteRules,
                      [](RewriteRule &rule) { return rule.ruleType == UNDEF; });
      llvm::sort(rewriteRules, [](RewriteRule &ruleA, RewriteRule &ruleB) {
        return ruleA.ruleType < ruleB.ruleType;
      });

      // Print report to stdout to be picked up by later tools.
      auto reportJson = serializeAnalysis(F, lsqArray, peArray, rewriteRules);

      outs() << formatv("{0:2}", json::Value(std::move(reportJson))) << "\n";
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