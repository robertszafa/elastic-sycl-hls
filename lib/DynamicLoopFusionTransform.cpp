#include "CommonLLVM.h"
#include "DynamicLoopFusionAnalysis.h"

using namespace llvm;

namespace llvm {

const std::string REPORT_ENV_NAME = "ELASTIC_PASS_REPORT";

using MemoryRequest = DynamicLoopFusionAnalysis::MemoryRequest;
using DecoupledLoopInfo = DynamicLoopFusionAnalysis::DecoupledLoopInfo;
using MemoryDependencyInfo = DynamicLoopFusionAnalysis::MemoryDependencyInfo;
using DepDir = DynamicLoopFusionAnalysis::DepDir;

/// Get loops to be decoupled in this function.
SmallVector<DecoupledLoopInfo, 4>
getLoopsToDecouple(DynamicLoopFusionAnalysis &DLFA,
                   const std::string &thisKernelName,
                   const std::string &originalKernelName, const bool isAGU) {
  auto loopsToDecouple =
      isAGU ? DLFA.getAgusToDecouple() : DLFA.getLoopsToDecouple();
  
  SmallVector<DecoupledLoopInfo, 4> LoopsToDecouple;
  auto MemDepInfos = DLFA.getMemoryDependencyInfo();
  for (auto &&decoupleInfo : loopsToDecouple) {
    std::string decoupleInfoKernelName =
        isAGU ? originalKernelName + "_agu" : originalKernelName;
    if (decoupleInfo.id > 0)
      decoupleInfoKernelName += "_" + std::to_string(decoupleInfo.id);
    
    if (decoupleInfoKernelName == thisKernelName) {
      LoopsToDecouple.push_back(decoupleInfo);
    }
  }

  return LoopsToDecouple;
}

/// Get the memory requests in this function that will be connected to our IP.
/// Also collect the corresponding pipe calls that the requets will be replcaed
/// with.
SmallVector<MemoryRequest>
getMemoryRequests(Function &F,
                  SmallVector<DecoupledLoopInfo, 4> &loopsToDecouple,
                  const bool isAGU) {
  SmallVector<MemoryRequest> MemoryRequests;
  for (auto decoupleInfo : loopsToDecouple) {
    for (auto &LdReq : decoupleInfo.loads) {
      std::string pipeName =
          isAGU ? "LoadReqPipes_" + std::to_string(LdReq.memoryId)
                : "LoadValPipes_" + std::to_string(LdReq.memoryId);
      LdReq.pipeCalls.push_back(getPipeCall(F, pipeName, {LdReq.reqId}));
      MemoryRequests.push_back(LdReq);
    }

    for (auto &StReq : decoupleInfo.stores) {
      std::string pipeName =
          isAGU ? "StoreReqPipes_" + std::to_string(StReq.memoryId)
                : "StoreValPipes_" + std::to_string(StReq.memoryId);
      if (isAGU) {
        // In the agu, the stReq pipe has another dimension.
        StReq.pipeCalls.push_back(getPipeCall(F, pipeName, {StReq.reqId, 0}));
        StReq.pipeCalls.push_back(getPipeCall(F, pipeName, {StReq.reqId, 1}));
      } else {
        StReq.pipeCalls.push_back(getPipeCall(F, pipeName, {StReq.reqId}));
      }
      MemoryRequests.push_back(StReq);
    }
  }

  return MemoryRequests;
}

/// Create and return a new basic blocks on the predBB --> succBB CFG edge.
/// Cache the created block and return it if called again with the same edge.
BasicBlock *createBlockOnEdge(BasicBlock *predBB, BasicBlock *succBB,
                              const std::string name = "") {
  // Map of CFG edges to poison basic blocks that already have been created.
  static MapVector<CFGEdge, BasicBlock *> createdBlocks;
  
  const CFGEdge requestedEdge{predBB, succBB};
  if (!createdBlocks.contains(requestedEdge)) {
    auto F = predBB->getParent();
    IRBuilder<> Builder(F->getContext());
    auto newBB = BasicBlock::Create(F->getContext(), name, F, succBB);
    createdBlocks[requestedEdge] = newBB;

    // Insert BB on pred-->succ edge
    // Before changing edges, make all phis from predBB now come from PoisonBB
    predBB->replaceSuccessorsPhiUsesWith(predBB, newBB);
    Builder.SetInsertPoint(newBB);
    Builder.CreateBr(succBB);
    auto branchInPredBB = dyn_cast<BranchInst>(predBB->getTerminator());
    branchInPredBB->replaceSuccessorWith(succBB, newBB);
  } 
  
  return createdBlocks[requestedEdge];
}

/// Move non-terminating instructions in {BB} to the end of {hoistLocation}.
void hoistBlock(BasicBlock *BB, BasicBlock *hoistLocation) {
  static SetVector<CFGEdge> alreadyHoisted;

  const CFGEdge requestedEdge{BB, hoistLocation};
  if (!alreadyHoisted.contains(requestedEdge)) {
    SmallVector<Instruction *> toMove;
    for (auto &I : *BB) {
      if (!I.isTerminator()) {
        toMove.push_back(&I);
      }
    }

    for (auto I : toMove)
      I->moveBefore(hoistLocation->getTerminator());

    alreadyHoisted.insert(requestedEdge);
  }
}

/// Create a zero-init uint32 tag using alloca at {F.entry}. Return its address.
Value *createTag(Function &F, SetVector<Instruction *> &toKeep) {
  IRBuilder<> Builder(F.getEntryBlock().getTerminator());
  // LLVM doesn't make a distinction between signed and unsigned. And the only
  // op done on tags is 2's complement addition, so the bit pattern is the same.
  auto tagType = Type::getInt32Ty(F.getContext());
  Value *tagAddr = Builder.CreateAlloca(tagType, nullptr, "tagAddr");
  auto initStore = Builder.CreateStore(ConstantInt::get(tagType, 0), tagAddr);
  toKeep.insert(initStore);
  return tagAddr;
}

/// If pipeCall is a write, then cast {I} to pipeType, if different types. 
/// If pipeCall is a read, then cast {pipeCall} to iType, if different types. 
/// Return the casted value, or the original  
Instruction *castIfNeeded(CallInst *pipeCall, Instruction *I) {
  // If writing, then cast I to pipeType
  if (isPipeWrite(pipeCall)) {
    auto pipeType =
        pipeCall->getOperand(0)->getType()->getNonOpaquePointerElementType();
    if (I->getType()->getTypeID() != pipeType->getTypeID()) {
      return BitCastInst::CreateBitOrPointerCast(I, pipeType, "", pipeCall);
    }
    return I;
  } else { // Otherwise, if read, cast pipeCall to iType
    if (I->getType()->getTypeID() != pipeCall->getType()->getTypeID()) {
      return BitCastInst::CreateBitOrPointerCast(pipeCall, I->getType(), "",
                                                 pipeCall->getNextNode());
    }
    return pipeCall;
  }

  assert(false && "Unexpected behaviour in castIfNeeded(pipeCall, I).");
  return nullptr;
}

/// Given a {pipeWrite} with a struct address as its operand, collect stores to
/// the struct fields.
SmallVector<StoreInst *> getPipeOpStructStores(const CallInst *pipeWrite) {
  SmallVector<StoreInst *> storesToStruct;
  for (auto user : pipeWrite->getArgOperand(0)->users()) {
    if (auto gep = dyn_cast<GetElementPtrInst>(user)) {
      for (auto userGEP : gep->users()) {
        if (auto stInstr = dyn_cast<StoreInst>(userGEP)) {
          storesToStruct.push_back(stInstr);
          break;
        }
      }
    } else if (auto stInstr = dyn_cast<StoreInst>(user)) {
      storesToStruct.push_back(stInstr);
      break;
    }
  }

  // The struct stores were collected in reverse above.
  std::reverse(storesToStruct.begin(), storesToStruct.end());
  return storesToStruct;
}

struct DynamicLoopFusionTransform : PassInfoMixin<DynamicLoopFusionTransform> {
  json::Object report;
  std::string originalKernelName = "";

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    // Read in report once.
    if (report.empty()) {
      report = *parseJsonReport(REPORT_ENV_NAME).getAsObject();
      originalKernelName = *report["mainKernelName"].getAsString();
    }

    std::string thisKernelName = demangle(std::string(F.getNameOrAsOperand()));
    bool isAGU = thisKernelName.find("_agu") < thisKernelName.size();
    if ((F.getCallingConv() != CallingConv::SPIR_KERNEL) ||
        (thisKernelName.find(originalKernelName) >= thisKernelName.size())) {
      return PreservedAnalyses::all();
    }

    auto &LI = AM.getResult<LoopAnalysis>(F);
    auto &SE = AM.getResult<ScalarEvolutionAnalysis>(F);
    auto *DLFA = new DynamicLoopFusionAnalysis(LI, SE);

    auto loopsToDecouple = getLoopsToDecouple(*DLFA, thisKernelName, 
                                              originalKernelName, isAGU);
    auto memoryRequests = getMemoryRequests(F, loopsToDecouple, isAGU);

    return PreservedAnalyses::none();
  }


  static bool isRequired() { return true; }

  void getAnalysisUsage(AnalysisUsage &AU) const {
    AU.addRequiredID(LoopAnalysis::ID());
    AU.addRequiredID(ScalarEvolutionAnalysis::ID());
  }
};

//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getDynamicLoopFusionTransformPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "DynamicLoopFusionTransform",
          LLVM_VERSION_STRING, [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "dynamic-loop-fusion-transform") {
                    FPM.addPass(DynamicLoopFusionTransform());
                    return true;
                  }
                  return false;
                });
          }};
}

// This is the core interface for pass plugins. It guarantees that 'opt' will
// be able to recognize the pass via '-passes=pass-name'
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getDynamicLoopFusionTransformPluginInfo();
}

} // end namespace llvm
