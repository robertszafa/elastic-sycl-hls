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
      thisBaseAddr["num_loads"] =
          std::count_if(memInstrs.begin(), memInstrs.end(), isaLoad);
      thisBaseAddr["num_stores"] =
          std::count_if(memInstrs.begin(), memInstrs.end(), isaStore);
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

// Returns true if instA is control dependent (may_occur && !must_occur),
// where the control dependency depends on {instB}.
//
// The motivation behind this check is to see, if the generation of store
// addresses can be hosited into it's own kernel, without using the base address
// of the {st} instruction.
//
// Go over all users of {onI} recursively:
//  - if we hit a branch:
//      - if the store is in any but not all of the BB dominated by the branch,
//      return true
// Record pairs into {checkedPairs} to prune the recursive search.
bool isAConditionalOnB(
    DominatorTree &DT, Instruction *instA, Instruction *instB,
    SmallVector<SmallVector<Instruction *, 2>> &checkedPairs) {
  SmallVector<Instruction *, 2> thisPair(2);
  thisPair[0] = instA;
  thisPair[1] = instB;
  if (std::find(checkedPairs.begin(), checkedPairs.end(), thisPair) !=
      checkedPairs.end())
    return false;

  checkedPairs.emplace_back(thisPair);

  for (auto user : instB->users()) {
    if (!isa<Instruction>(user))
      continue;

    auto userI = dyn_cast<Instruction>(user);
    if (auto brI = dyn_cast<BranchInst>(userI)) {
      bool atLeastOneDominatesStore = false;
      bool allDominateStore = true;
      for (auto succ : brI->successors()) {
        allDominateStore &= DT.dominates(succ, instA->getParent());
        atLeastOneDominatesStore |= DT.dominates(succ, instA->getParent());
      }

      if (atLeastOneDominatesStore && !allDominateStore)
        return true;
    }

    return isAConditionalOnB(DT, instA, userI, checkedPairs);
  }

  return false;
}

bool isInUsersOf(Instruction *I0, Instruction *I1) {
  for (auto user : I1->users()) {
    if (dyn_cast<Instruction>(user) == I0)
      return true;
  }

  return false;
}

SmallVector<Instruction *>
getInstructionsUsedByI(Function &F, DominatorTree &DT, Instruction *I) {
  SmallVector<Instruction *> result;
  if (!I)
    return result;

  for (auto &BB : F) {
    if (DT.properlyDominates(I->getParent(), &BB))
      continue;

    for (auto &bbI : BB) {
      if (isInUsersOf(I, &bbI))
        result.push_back(&bbI);
    }
  }

  return result;
}

bool isAnyStoreControlDepOnAnyLoad(
    DominatorTree &DT, const SmallVector<Instruction *> &memInstrs) {
  for (auto si : memInstrs) { // For all stores
    if (!isaStore(si))
      continue;
    for (auto li : memInstrs) { // For all loads
      if (!isaLoad(li))
        continue;

      SmallVector<SmallVector<Instruction *, 2>> checkedPairs;
      if (isAConditionalOnB(DT, si, li, checkedPairs))
        return true;
    }
  }

  return false;
}

/// Given load and store instructions, decide if the instructions that generate
/// addresses for the loads and stores can be decoupled from the rest of the
/// instructions in function {F}. If ANY of the address generation instructions
/// cannot be decoupled, return false.
///
/// Algorithm for making the decision:
/// 1. If any of the stores is control dependent on any of the loads, return
///    false. (This is because we cannot know if a given store address should be
///    produced, without looking at the load value).
/// 2. If any of the values producing a given address is needed in a basic block
///    properly dominated by the last mem. instruction in the 
///    {F}.entry->{F}.exit path, return false.
bool canDecoupleAddressGenFromCompute(Function &F, DominatorTree &DT,
                                      SmallVector<Instruction *> memInstrs) {
  // All BBs that contain an instruction \in {memInstr} are collected.
  // Also all address generation instructions for the memInstrs are
  // collected, including parent nodes in the DDG.
  SetVector<BasicBlock *> allMemInstrBBs;
  SetVector<Instruction *> allAddressIntrs;
  for (auto I : memInstrs) {
    allMemInstrBBs.insert(I->getParent());
    auto addressI =
        isaStore(I)
            ? dyn_cast<Instruction>(dyn_cast<Instruction>(I->getOperand(1)))
            : dyn_cast<Instruction>(dyn_cast<Instruction>(I->getOperand(0)));
    allAddressIntrs.insert(addressI);
    auto usedByLi = getInstructionsUsedByI(F, DT, addressI);
    allAddressIntrs.insert(addressI);
    for (auto &I : usedByLi)
      allAddressIntrs.insert(I);
  }

  // Get the last basic block in the function.entry->function.exit CFG path
  // from {allBBs} in the function dominator tree, i.e. the BB which
  // is not properly dominated by any other BB from {allBBs}.
  BasicBlock *lastBB = nullptr;
  for (auto candidateBB : allMemInstrBBs) {
    if (!std::any_of(allMemInstrBBs.begin(), allMemInstrBBs.end(),
                     [&](BasicBlock *b) {
                       return DT.properlyDominates(candidateBB, b);
                     })) {
      lastBB = candidateBB;
      break;
    }
  }

  // Get all BBs which are properly dominated by the lastBB.
  SetVector<BasicBlock *> dominatedByLastBB;
  for (auto &BB : F) {
    if (DT.properlyDominates(lastBB, &BB))
      dominatedByLastBB.insert(&BB);
  }

  // If there is an any instruction in {dominatedByLastBB} which uses
  // any of the address generation instructions, return FALSE.
  for (auto &I : allAddressIntrs) {
    for (auto &useI : I->uses()) {
      for (auto &dBB : dominatedByLastBB) {
        if (useI->isUsedInBasicBlock(dBB))
          return false;
      }
    }
  }

  // TODO: If there is any instruction in {stores+loads} which depends
  // (including control dependence) on the value returned by any load, then
  // return FALSE.
  return !isAnyStoreControlDepOnAnyLoad(DT, memInstrs);
}

void dataHazardPrinter(Function &F, LoopInfo &LI, ScalarEvolution &SE,
                       DominatorTree &DT) {
  // Get all memory loads and stores that form a RAW hazard dependence.
  auto *DHA = new DataHazardAnalysis(F, LI, SE, DT);
  SmallVector<SmallVector<Instruction *>> memInstrsAll = DHA->getResult();

  if (memInstrsAll.size() > 0) {
    json::Object report = generateReport(F, memInstrsAll);

    // Add "decouple_address: [1|0]" field to the report.
    // Check if the store address generation can be split from the compute.
    bool canDecoupleAddress = std::all_of(
        memInstrsAll.begin(), memInstrsAll.end(), [&](auto memInstrs) {
          return canDecoupleAddressGenFromCompute(F, DT, memInstrs);
        });
    report["decouple_address"] = int(canDecoupleAddress);

    // Print json report to std::out.
    outs() << formatv("{0:2}", json::Value(std::move(report))) << "\n";

    // Print quick info to dbgs() stream.
    dbgs() << "\n************* Data Hazard Report *************\n";
    dbgs() << "Number of base addresses: " << memInstrsAll.size() << "\n";
    dbgs() << "Decoupled address gen: " << canDecoupleAddress << "\n";
    for (auto &memInstructions : memInstrsAll) {
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
    AU.addRequiredID(PostDominatorTreeAnalysis::ID());
    AU.addRequiredID(DependenceAnalysis::ID());
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