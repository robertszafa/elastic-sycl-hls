/// Duplicated reads across AGU and CU: keep read in AGU, and create new
/// AGU-->CU pipe write with new pipe.
///
/// Duplicated write across AGU and CU: keep write inCU, delete it in AGU.

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/Instructions.h"

#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/Demangle/Demangle.h"

#include "llvm/Transforms/Utils/Cloning.h"

#include "CommonLLVM.h"

const std::string MAIN_KERNEL = "MainKernel";
const std::string AGU_KERNEL = "AGU";

using namespace llvm;

namespace llvm {

bool isSpirKernelWithSubstring(Function &F, const std::string &SearchString) {
  std::string fName = demangle(F.getNameOrAsOperand());
  return (fName.find(SearchString) < fName.size()) &&
         (F.getCallingConv() == CallingConv::SPIR_KERNEL);
}

std::string getPipeNameOrEmpty(Instruction *I) {
  if (auto PipeCall = getPipeCall(I)) {
    auto PipeCallFunction = PipeCall->getCalledFunction();
    return demangle(PipeCallFunction->getNameOrAsOperand());
  }
  return "";
}

SmallVector<std::string> getAllPipeNames(Function &F) {
  SmallVector<std::string> AllPipes{};
  for (auto &BB : F) {
    for (auto &I : BB) {
      auto PipeOpName = getPipeNameOrEmpty(&I);
      if (!PipeOpName.empty()) {
        AllPipes.push_back(PipeOpName);
      }
    }
  }

  return AllPipes;
}

SmallVector<CallInst *> getPipeCallsWithName(Function &F,
                                             const std::string &PipeOpName) {
  SmallVector<CallInst *> Res;

  for (auto &BB : F) {
    for (auto &I : BB) {
      auto ThisPipeName = getPipeNameOrEmpty(&I);
      if (ThisPipeName.compare(PipeOpName) == 0) {
        Res.push_back(dyn_cast<CallInst>(&I));
      }
    }
  }

  return Res;
}

std::map<const std::string, SetVector<Function *>> getDuplicatePipes(
    const std::map<Function *, SmallVector<std::string>> &PipesInFunc) {
  std::map<const std::string, SetVector<Function *>> DuplicatePipes{};

  for (auto &[F1, Pipes1] : PipesInFunc) {
    for (auto &[F2, Pipes2] : PipesInFunc) {
      if (F1 == F2)
        continue;

      for (auto PipeName1 : Pipes1) {
        for (auto PipeName2 : Pipes2) {
          if (PipeName1.compare(PipeName2) == 0) {
            if (DuplicatePipes.find(PipeName1) == DuplicatePipes.end()) {
              DuplicatePipes[PipeName1] = SetVector<Function *>{};
              DuplicatePipes[PipeName1].insert(F1);
            }

            DuplicatePipes[PipeName1].insert(F2);
            break;
          }
        }
      }
    }
  }

  return DuplicatePipes;
}

bool isPipeRead(const std::string &PipeOpName) {
  return PipeOpName.find("::read()") < PipeOpName.size();
}

bool isPipeWrite(const std::string &PipeOpName) {
  return PipeOpName.find("::write(") < PipeOpName.size();
}

void deleteDuplicateWrites(
    const std::map<const std::string, SetVector<Function *>> &DuplicatePipes) {
  for (auto &[PipeOpName, Functions] : DuplicatePipes) {
    if (isPipeRead(PipeOpName))
      continue;

    for (auto F : Functions) {
      if (isSpirKernelWithSubstring(*F, AGU_KERNEL)) {
        auto ToDeleteInFunc = getPipeCallsWithName(*F, PipeOpName);

        for (auto I : ToDeleteInFunc) {
          deleteInstruction(I);
        }
      }
    }
  }
}

GlobalVariable *getPipeStorageGlobalObj(Module &M,
                                        const std::string &PipeReadOp) {
  auto nameEndPos = PipeReadOp.find(">::read");
  for (auto &GV : M.globals()) {
    auto GVName = demangle(GV.getNameOrAsOperand());
    bool equal = true;
    for (size_t i = 0; i < nameEndPos; ++i)
      equal &= (PipeReadOp[i] == GVName[i]);

    if (equal) {
      return &GV;
    }
  }

  return nullptr;
}

GlobalVariable *copyPipeStorage(Module &M, GlobalVariable *CopyFrom) {
  GlobalVariable *Result = new GlobalVariable(
      M, CopyFrom->getValueType(), CopyFrom->isConstant(),
      CopyFrom->getLinkage(), CopyFrom->getInitializer(), CopyFrom->getName());
  Result->copyAttributesFrom(CopyFrom);

  return Result;
}

std::string getCorrespondingBlockingPipeWrite(Module &M,
                                              const std::string &PipeReadOp) {
  auto nameEndPos = PipeReadOp.find(">::read");
  auto isSamePipeAsPipeRead = [&](const std::string &OtherPipeOp) {
    for (size_t i = 0; i < nameEndPos; ++i) {
      if (PipeReadOp[i] != OtherPipeOp[i]) {
        return false;
      }
    }
    return true;
  };

  auto isBlockingPipe = [](const std::string &PipeOpName) {
    std::string NonBlockingPipeArguments = "int const&, bool&";
    return PipeOpName.find(NonBlockingPipeArguments) >= PipeOpName.size();
  };

  for (auto &F : M) {
    for (auto PipeOpName : getAllPipeNames(F)) {
      if (isPipeWrite(PipeOpName) && isSamePipeAsPipeRead(PipeOpName) &&
          isBlockingPipe(PipeOpName)) {
        return PipeOpName;
      }
    }
  }

  return "";
}

Function *getPipeFunctionWithName(Module &M,
                                  const std::string &PipeWriteOpName) {
  for (auto &F : M) {
    std::string fName = demangle(F.getNameOrAsOperand());
    if (fName.compare(PipeWriteOpName) == 0) {
      return &F;
    }
  }

  return nullptr;
}

/// Copy the pipe read/write function definition and change the global variable
/// to the underlying pipe storage.
Function *copyPipeFunctionDef(Function &CopyFrom, GlobalVariable *Old,
                              GlobalVariable *New) {
  auto ValueSwapMap = ValueToValueMapTy{};
  ValueSwapMap[Old] = New;
  auto NewFunction = CloneFunction(&CopyFrom, ValueSwapMap);
  NewFunction->copyAttributesFrom(&CopyFrom);
  return NewFunction;
}

/// Given pipeA.read() operations in both kernelAGU and kernelCU, do:
/// Globally in Module:
///   Create new pipeB with same type as pipeA. This involves creating a new
///   Global Variable representing the pipe storage, and read/write function
///   definitions in the module that use the new global variable. We do most of
///   this by copying pipeA.
/// In AGU:
///   Insert pipeB.write(val), where val = pipeA.read()
/// In CU:
///   Change val = pipeA.read() to val = pipeB.read()
void forwardDuplicatedReads(
    Module &M,
    const std::map<const std::string, SetVector<Function *>> &DuplicatePipes) {
  for (auto &[PipeReadOpName, Functions] : DuplicatePipes) {
    if (!isPipeRead(PipeReadOpName))
      continue;

    SmallVector<Function *> ChainOfFunctions;
    for (auto F : Functions) {
      if (isSpirKernelWithSubstring(*F, AGU_KERNEL))
        ChainOfFunctions.push_back(F);
    }
    for (auto F : Functions) {
      if (!isSpirKernelWithSubstring(*F, AGU_KERNEL))
        ChainOfFunctions.push_back(F);
    }

    auto PipeStorage = getPipeStorageGlobalObj(M, PipeReadOpName);

    // Copy pipe storage global variable
    for (size_t i = 0; i < ChainOfFunctions.size() - 1; ++i) {
      // Sliding window over the chain of functions.
      Function *LeftF = ChainOfFunctions[i];
      Function *RightF = ChainOfFunctions[i + 1];

      SmallVector<CallInst *> ReadsToForwardInLeft =
          getPipeCallsWithName(*LeftF, PipeReadOpName);
      SmallVector<CallInst *> ReadsToForwardInRight =
          getPipeCallsWithName(*RightF, PipeReadOpName);

      std::string PipeWriteOpName =
          getCorrespondingBlockingPipeWrite(M, PipeReadOpName);
      Function *PipeWriteFunc = getPipeFunctionWithName(M, PipeWriteOpName);
      Function *PipeReadFunc = getPipeFunctionWithName(M, PipeReadOpName);

      // Create a Left -> Right Write(Read()) call for each read in left.
      for (size_t i = 0; i < ReadsToForwardInLeft.size(); ++i) {
        // Each Left/Write read() pair gets a new pipe.
        GlobalVariable *CopyOfPipeStorage = copyPipeStorage(M, PipeStorage);
        Function *CopyOfPipeWriteFunc =
            copyPipeFunctionDef(*PipeWriteFunc, PipeStorage, CopyOfPipeStorage);
        
        // Create pipe write call in left, a struct to pass as argument, and a
        // store to the struct storing the value of read in left.
        CallInst *ReadInLeft = ReadsToForwardInLeft[i];
        IRBuilder<> Builder(LeftF->getEntryBlock().getFirstNonPHI());
        auto PipeReadType = ReadInLeft->getType();
        Value *valAddr =
            Builder.CreateAlloca(PipeReadType, nullptr, "forwardPipeVal");
        auto PipeWriteFuncArgType = CopyOfPipeWriteFunc->getArg(0)->getType();
        auto valAddrCasted =
            Builder.CreateAddrSpaceCast(valAddr, PipeWriteFuncArgType);
        auto storeIntoForwardPipe = Builder.CreateStore(ReadInLeft, valAddrCasted);
        storeIntoForwardPipe->moveAfter(ReadInLeft);
        auto ForwardPipeCall =
            Builder.CreateCall(CopyOfPipeWriteFunc, {valAddrCasted});
        ForwardPipeCall->moveAfter(storeIntoForwardPipe);
        ForwardPipeCall->setDebugLoc(ReadInLeft->getDebugLoc());

        // Change read in right to read from the new pipe.
        CallInst *ReadInRight = ReadsToForwardInRight[i];
        Function *CopyOfPipeReadFunc =
            copyPipeFunctionDef(*PipeReadFunc, PipeStorage, CopyOfPipeStorage);
        ReadInRight->setCalledFunction(CopyOfPipeReadFunc);
      }
    }
  }
}

struct PipeDeduplication : PassInfoMixin<PipeDeduplication> {
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {
    bool wasChanged = false;

    // Pipes are identified by their type/class name.
    std::map<Function *, SmallVector<std::string>> PipesInFunc;
    for (auto &F : M) {
      if (isSpirKernelWithSubstring(F, MAIN_KERNEL)) {
        PipesInFunc[&F] = getAllPipeNames(F);
      }
    }

    std::map<const std::string, SetVector<Function *>> DuplicatePipes =
        getDuplicatePipes(PipesInFunc);

    // Duplicated writes are deleted in the AGU.
    deleteDuplicateWrites(DuplicatePipes);
    // Duplicated reads are deleted in the CU and are exchanged for a new pipe
    // read that reads a value forwared by the AGU.
    forwardDuplicatedReads(M, DuplicatePipes);

    return wasChanged ? PreservedAnalyses::none() : PreservedAnalyses::all();
  }

  // Without isRequired returning true, this pass will be skipped for functions
  // decorated with the optnone LLVM attribute. Note that clang -O0 decorates
  // all functions with optnone.
  static bool isRequired() { return true; }

  void getAnalysisUsage(AnalysisUsage &AU) const {}
};

//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getPipeDeduplicationPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "PipeDeduplication", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, ModulePassManager &MPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "pipe-deduplication") {
                    MPM.addPass(PipeDeduplication());
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
  return getPipeDeduplicationPluginInfo();
}

} // end namespace llvm