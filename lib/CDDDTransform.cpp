#include "CommonLLVM.h"
#include <numeric>

using namespace llvm;

namespace llvm {

const std::string ENVIRONMENT_VARIBALE_REPORT = "CDDD_REPORT_FILE";

/// For each {PipeRead: Val} mapping, move the pipe call to the {insertPoint}
/// in {BB} and replace all uses of the Val with the PipeRead value.
void changeVal2PipeRead(Function &F, SmallVector<Pipe2Inst> &P2IMap,
                        BasicBlock *BB, bool insertAtEndOfBB = false) {
  // Move all pipe read calls to the start of {BB}. Replace all uses of the
  // dependency in instructions with the value of the pipe read.
  for (auto &P2I : P2IMap) {
    auto insPt = insertAtEndOfBB ? BB->getTerminator() : BB->getFirstNonPHI();
    P2I.pipeCall->moveBefore(insPt);
    P2I.instr->replaceAllUsesWith(P2I.pipeCall);
  }
}

/// For each {PipeWrite: Val} mapping, move the pipe call to the {insertPoint}
/// in {BB} and make the Val the operand of the PipeWrite.
void insertPipeWriteOfVal(Function &F, SmallVector<Pipe2Inst> &P2IMap,
                          BasicBlock *BB, bool insertAtEndOfBB = false) {
  for (auto &P2I : P2IMap) {
    auto insPt = insertAtEndOfBB ? BB->getTerminator() : BB->getFirstNonPHI();
    P2I.pipeCall->moveBefore(insPt);
    storeValIntoPipe(P2I.instr, P2I.pipeCall);
  }
}

/// Delete all instrucions in {BB} except the {exceptions}.
void deleteInstructions(BasicBlock *BB, SmallVector<Instruction *> exceptions) {
  SmallVector<Instruction *> toRemove;
  for (auto &I : *BB) {
    if (!llvm::is_contained(exceptions, &I))
      toRemove.push_back(&I);
  }

  for (auto I : toRemove) 
    deleteInstruction(I);
}

/// Delete all basic block in {F} except the {exceptions}.
void deleteBlocks(Function &F, SmallVector<BasicBlock *> exceptions) {
  SmallVector<BasicBlock *> toRemove;
  for (auto &BB : F) {
    if (!llvm::is_contained(exceptions, &BB)) {
      deleteInstructions(&BB, {});
      toRemove.push_back(&BB);
    }
  }
  for (auto BB : toRemove) 
    BB->removeFromParent();
}

/// Gicen a {predPipe} write instruction, move it into the beginning of {BB}
/// and set the value of its operand to {true} at that point. Also move it to
/// the return block of {F} with a value of {false}.
void addPredicateWrites(Function &F, BasicBlock *BB, CallInst *predPipe) {
  auto predType = Type::getInt8Ty(F.getContext());

  predPipe->moveBefore(*BB, BB->begin());
  storeValIntoPipe(ConstantInt::get(predType, 1), predPipe);

  auto predPipeClone = dyn_cast<CallInst>(predPipe->clone());
  auto returnBlockF = getReturnBlock(F);
  predPipeClone->insertBefore(returnBlockF->getFirstNonPHI());
  storeValIntoPipe(ConstantInt::get(predType, 0), predPipeClone);
}

/// Given a {BB}, wrap it into a while (predPipe::read()) { BB } code structure.
/// Remove the rest of the BBs in {F} - this is guaranteed to be save since all
/// in/out def-use SSA values in {BB} have been replaced by pipe reads/writes.
void block2PedicatedWhileLoop(Function &F, CallInst *predPipe, BasicBlock *BB) {
  // There will be 4 blocks left in {F}. Block 0 is F.entry.

  // 1: The return BB only needs the ret instruction.
  auto returnBB = getReturnBlock(F);
  deleteInstructions(returnBB, {returnBB->getTerminator()});

  // 2: The loop header block needs to read from the predicate pipe and based on
  // the condition branch to exit or to {BB}: "while (predPipe::read()) {BB}".
  auto headerBB = F.getEntryBlock().getSingleSuccessor();
  IRBuilder<> Builder(headerBB->getTerminator());
  auto loopBranch = Builder.CreateCondBr(predPipe, BB, returnBB);
  predPipe->moveBefore(loopBranch);
  deleteInstructions(headerBB, {predPipe, loopBranch});

  // 3: In the {BB} block, create a backedge to the loop header BB.
  Builder.SetInsertPoint(BB->getTerminator());
  Builder.CreateBr(headerBB);
  deleteInstruction(BB->getTerminator());

  // Remove all other blocks from F.
  deleteBlocks(F, {&F.getEntryBlock(), BB, returnBB, headerBB});
}

/// Sort all vectors based on the post dominance relation in {blocks}.
void postDominanceOrder(PostDominatorTree &PDT,
                        SmallVector<SmallVector<Pipe2Inst>> &depIn,
                        SmallVector<SmallVector<Pipe2Inst>> &depOut,
                        SmallVector<CallInst *> &predPipes,
                        SmallVector<BasicBlock *> &blocks) {
  SmallVector<int> indexes(blocks.size());
  std::iota(indexes.begin(), indexes.end(), 0);

  auto isNotPostDominated = [&PDT, &blocks](int idx0, int idx1) {
    return !PDT.dominates(blocks[idx1], blocks[idx0]);
  };
  llvm::sort(indexes, isNotPostDominated);

  for (size_t i = 0; i < indexes.size() / 2; ++i) {
    std::swap(blocks[i], blocks[indexes[i]]);
    std::swap(depIn[i], depIn[indexes[i]]);
    std::swap(depOut[i], depOut[indexes[i]]);
    std::swap(predPipes[i], predPipes[indexes[i]]);
  }
}

/// Given a json report generated by a CDDD analysis pass collect:
/// - the basic blocks to decouple,
/// - mappings between instructions generating {incoming_uses}/{outgoing_defs},
//    and sycl pipe call instructions,
/// - call instructions to predicate pipes.
void collectLLVMValues(json::Object &report, Function &F, bool isMain,
                       SmallVector<SmallVector<Pipe2Inst>> &depIn,
                       SmallVector<SmallVector<Pipe2Inst>> &depOut,
                       SmallVector<CallInst *> &predPipes,
                       SmallVector<BasicBlock *> &blocks) {
  for (json::Value &blockInfoVal : *report["blocks_to_decouple"].getAsArray()) {
    auto blockInfo = *blockInfoVal.getAsObject();
    // If looking for pipes for a specific kernel copy, then ignore the rest.
    auto copyName = blockInfo["kernel_copy_name_full"].getAsString().value();
    if (!isMain && copyName != demangle(std::string(F.getName())))
      continue;

    auto inDepsArray = *blockInfo["incoming_uses"].getAsArray();
    auto outDepsArray = *blockInfo["outgoing_defs"].getAsArray();
    depIn.push_back(getPipe2InstMaps(F, inDepsArray));
    depOut.push_back(getPipe2InstMaps(F, outDepsArray));

    auto predPipeObject = *blockInfo["pred_pipe"].getAsObject();
    predPipes.push_back(getPipeCall(F, predPipeObject));

    int idxBB = blockInfo["basic_block_idx"].getAsInteger().value();
    blocks.push_back(getChildWithIndex<Function, BasicBlock>(&F, idxBB));
  }
}

struct CDDDTransform : PassInfoMixin<CDDDTransform> {
  json::Object report;

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    // Read in report once.
    if (report.empty()) 
      report = *parseJsonReport(ENVIRONMENT_VARIBALE_REPORT).getAsObject();

    // Determine if this is our original kernel, a copy kernel, or neither.
    std::string thisKernelName = demangle(std::string(F.getName()));
    auto mainKernel =
        std::string(report["kernel_name_full"].getAsString().value());
    bool isMain = mainKernel == thisKernelName;
    bool isOurKernel = std::equal(mainKernel.begin(), mainKernel.end(),
                                  thisKernelName.begin());
    if (F.getCallingConv() != CallingConv::SPIR_KERNEL || !isOurKernel) {
      return PreservedAnalyses::all();
    }

    // Collect mappings between instructions (from decoupled blocks) and the
    // pipe read/writes that will replace them. The main kernel needs to know
    // all of these mappings, whereas the kernels where individual blocks are
    // decoupled into need to only know about their mappings.
    SmallVector<SmallVector<Pipe2Inst>> depIn, depOut;
    SmallVector<CallInst *> predPipes;
    SmallVector<BasicBlock *> blocks;
    collectLLVMValues(report, F, isMain, depIn, depOut, predPipes, blocks);

    // Ensure we start with the block that is not post dominated by any other
    // block that we will decouple. Otherwise we would have to keep track of the
    // LLVM values changed to pipe ops and update them for all blocks.
    auto &PDT = AM.getResult<PostDominatorTreeAnalysis>(F);
    postDominanceOrder(PDT, depIn, depOut, predPipes, blocks);

    if (isMain) {
      // In the main kernel, deal with in/out dependencies for all blocks.
      for (size_t iB = 0; iB < blocks.size(); ++iB) {
        // Get values defined in a decoupled block, and used in the main kernel.
        changeVal2PipeRead(F, depOut[iB], blocks[iB], true);

        // Delete all instructions in the block except the terminator and pipes.
        SmallVector<Instruction *> exceptions;
        auto terminator = blocks[iB]->getTerminator();
        exceptions.push_back(terminator);
        for (auto P2I : depOut[iB]) 
          exceptions.push_back(P2I.pipeCall);
        deleteInstructions(blocks[iB], exceptions);

        // Now insert pipe writes supplying values defined in the main kernel
        // but used in the decoupled block.
        insertPipeWriteOfVal(F, depIn[iB], blocks[iB]);

        // Add a true predicate write to start of BB, and false write to F.end.
        addPredicateWrites(F, blocks[iB], predPipes[iB]);
      }
    } else { 
      // Create a predicated kernel just for this block.
      changeVal2PipeRead(F, depIn[0], blocks[0]);
      insertPipeWriteOfVal(F, depOut[0], blocks[0], true);
      block2PedicatedWhileLoop(F, predPipes[0], blocks[0]);
    }

    return PreservedAnalyses::none();
  }

  // Without isRequired returning true, this pass will be skipped for functions
  // decorated with the optnone LLVM attribute. Note that clang -O0 decorates
  // all functions with optnone.
  static bool isRequired() { return true; }

  void getAnalysisUsage(AnalysisUsage &AU) const {
    AU.addRequiredID(PostDominatorTreeAnalysis::ID());
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
