//===----------------------------------------------------------------------===//
//
// This file defines functions commonly used during LLVM passes,
// and common LLVM includes.
//
//===----------------------------------------------------------------------===//

#ifndef COMMON_LLVM_H
#define COMMON_LLVM_H

// These are the LLVM analysis passes used.
#include "llvm/Analysis/DependenceAnalysis.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/Analysis/ScalarEvolution.h"

// This includes Instruction, Function, Value and other common IR. 
#include "llvm/IR/IRBuilder.h"

#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/Support/JSON.h"
#include "llvm/Demangle/Demangle.h"

#include <fstream>
#include <iterator>
#include <regex>

using namespace llvm;

// This file is included in several other files.
// Use an anonymous namespace to avoid multiple same definitions.
namespace {

/// A mapping between a pipe read/write and a load/store that it will replace.
struct Pipe2Inst {
  CallInst *pipeCall;
  Instruction *instr;
  bool isOnchip;
};

/// Lambdas for easier use in range based algorithms.
auto isaLoad = [](auto i) { return isa<LoadInst>(i); };
auto isaStore = [](auto i) { return isa<StoreInst>(i); };


CallInst *getPipeCall (Instruction *I) {
  const std::string PIPE_CALL = "ext::intel::pipe";

  if (CallInst *callInst = dyn_cast<CallInst>(I)) {
    if (Function *f = callInst->getCalledFunction()) {
      auto fName = demangle(std::string(f->getName()));
      if (f->getCallingConv() == CallingConv::SPIR_FUNC &&
          fName.find(PIPE_CALL) != std::string::npos) {
        return callInst;
      }
    }
  }

  return nullptr;
}

[[maybe_unused]] SmallVector<Instruction *>
getLoads(const SmallVector<Instruction *> &memInstr) {
  SmallVector<Instruction *> loads;
  for (auto &I : memInstr)
    if (isaLoad(I))
      loads.push_back(I);
  return loads;
}

[[maybe_unused]] SmallVector<Instruction *>
getStores(const SmallVector<Instruction *> &memInstr) {
  SmallVector<Instruction *> stores;
  for (auto &I : memInstr)
    if (isaStore(I))
      stores.push_back(I);
  return stores;
}

[[maybe_unused]] SmallVector<int> 
getSeqInBB(const SmallVector<Instruction *> &Range) {
  SmallMapVector<BasicBlock *, int, 4> seen;
  SmallVector<int> seqInBB;
  for (auto &I : Range) {
    if (seen.contains(I->getParent())) 
      seen[I->getParent()]++;
    else 
      seen[I->getParent()] = 0;
    
    seqInBB.push_back(seen[I->getParent()]);
  }
  
  return seqInBB;
}

[[maybe_unused]] std::string getTypeString(const Instruction *I) {
  std::string typeStr;
  llvm::raw_string_ostream rso(typeStr);
  I->getType()->print(rso);

  return typeStr;
}

/// Given a {val}, store it into the operand of the {pipe} write.
/// Return the created store.
[[maybe_unused]] Instruction *storeValIntoPipe(Value *val, CallInst *pipe) {
  auto pipeOperandAddr = pipe->getOperand(0);
  IRBuilder<> Builder(pipe);

  // Cast valType to pipeType if possible, e.g. val:i1 -> pipe:i8.
  auto pipeType = pipeOperandAddr->getType()->getNonOpaquePointerElementType();
  auto valType = val->getType();
  if (valType != pipeType) {
    auto widthOk =
        valType->getPrimitiveSizeInBits() <= pipeType->getPrimitiveSizeInBits();
    assert(widthOk && "Cannot cast because of precision loss.");
    val = Builder.CreateCast(Instruction::CastOps::SExt, val, pipeType);
  }

  return Builder.CreateStore(val, pipeOperandAddr);
}

/// Delete {inst} from it's function.
[[maybe_unused]] void deleteInstruction(Instruction *I) {
  I->dropAllReferences();
  I->replaceAllUsesWith(UndefValue::get(I->getType()));
  I->eraseFromParent();
}

/// Given Function {F}, return all Functions that call {F}.
[[maybe_unused]] SmallVector<Function *> getCallerFunctions(Module *M,
                                                               Function &F) {
  // The expected case is one caller.
  SmallVector<Function *> callers;
  auto &functionList = M->getFunctionList();
  for (auto &function : functionList) {
    for (auto &bb : function) {
      for (auto &instruction : bb) {
        if (CallInst *callInst = dyn_cast<CallInst>(&instruction)) {
          if (Function *calledFunction = callInst->getCalledFunction()) {
            if (calledFunction->getName() == F.getName()) {
              callers.push_back(&function);
            }
          }
        }
      }
    }
  }

  return callers;
}

/// Return the Basic Block with the return instruction from {F}. {F} is assumed
/// to have a single return, if not, then the first block is returned.
[[maybe_unused]] BasicBlock *getReturnBlock(Function &F) {
    for (auto &BB : llvm::reverse(F)) {
      if (BB.getTerminator() && isa<ReturnInst>(BB.getTerminator())) 
        return &BB;
    }

    return nullptr;
}

/// Return the index of {child} inside of the default traverse of {parent}. 
/// Returns -1 if not found.
template <typename T1, typename T2> 
[[maybe_unused]] int getIndexOfChild(T1 *Parent, T2 *Child) {
  int idx = -1;
  for (auto &ch : *Parent) {
    idx++;
    if (&ch == Child)
      break;
  }
  
  return idx;
}

[[maybe_unused]] SmallVector<Instruction*> getInstructions(BasicBlock *BB) {
  SmallVector<Instruction *> res;
  for (auto &Child : *BB) 
    res.push_back(&Child);

  return res;
}

/// Return the Child at {idx} in {Parent}. Return a nullptr if out of bounds.
template <typename T1, typename T2> 
[[maybe_unused]] T2 *getChildWithIndex(T1 *Parent, int idx) {
  int i = 0;
  for (auto &ch : *Parent) {
    if (i == idx)
      return &ch;
    i++;
  }
  
  return nullptr;
}

/// Given an environment variable name for a json file name, return json::Value.
[[maybe_unused]] json::Value parseJsonReport(const std::string envVariable) {
  if (const char *fname = std::getenv(envVariable.c_str())) {
    std::ifstream t(fname);
    std::stringstream buffer;
    buffer << t.rdbuf();

    auto Json = json::parse(llvm::StringRef(buffer.str()));
    assert(Json && "Error parsing json elastic-pass-report");

    return *Json;
  }

  assert("No json file found.");
  return json::Value(nullptr);
}

/// Given an instruction, return a json object with its description. E.g.:
///   {"basic_block_idx": 8, "instruction_idx": 9}
[[maybe_unused]] json::Object genJsonForInstruction(Instruction *I) {
  llvm::json::Object obj;
  auto iBB = I->getParent();
  obj["basic_block_idx"] = getIndexOfChild(iBB->getParent(), iBB);
  obj["instruction_idx"] = getIndexOfChild(iBB, I);

  return obj;
}

/// Return the instruction corresponding to {iDesc}: BB index and Instr index.
[[maybe_unused]] Instruction *getInstruction(Function &F, json::Object iDescr) {
  auto bbIdx = int(iDescr["basic_block_idx"].getAsInteger().value());
  auto instrIdx = int(iDescr["instruction_idx"].getAsInteger().value());
  auto BB = getChildWithIndex<Function, BasicBlock>(&F, bbIdx);
  auto I = getChildWithIndex<BasicBlock, Instruction>(BB, instrIdx);
  assert(I && "Instruction for given iDescr not found.");
  return I;
}

/// Return the pipe call instruction corresponding to the pipeInfo json obj.
[[maybe_unused]] CallInst *getPipeCall(Function &F, json::Object pipeInfo) {
  static StringMap<int> collectedCalls;

  auto pipeNameOpt = pipeInfo.getString("pipe_name");
  assert(pipeNameOpt && "Pipe with given {pipe_name} not found.");
  auto pipeName = pipeNameOpt->str();
  // If no seq_in_bb or repeat_id, then use defaults.
  auto seqNumOpt = pipeInfo.getInteger("seq_in_bb");
  auto seqNum = seqNumOpt ? seqNumOpt.value() : -1;

  const std::string pipeIdKey = pipeName +
                                pipeInfo.getString("read/write")->str() +
                                std::to_string(seqNum);
  int pipeCallsToSkip = 0;
  if (collectedCalls.contains(pipeIdKey)) 
    pipeCallsToSkip = collectedCalls[pipeIdKey];
  else 
    collectedCalls[pipeIdKey] = 1;

  /// Lambda. Returns true if {call} is a call to our pipe.
  auto isThisPipe = [&pipeName, &seqNum](std::string call) {
    std::regex pipe_regex{pipeName, std::regex_constants::ECMAScript};
    std::smatch pipe_match;
    std::regex_search(call, pipe_match, pipe_regex);

    std::regex struct_regex{"StructId<" + std::to_string(seqNum) + "ul>",
                            std::regex_constants::ECMAScript};
    std::smatch struct_match;
    std::regex_search(call, struct_match, struct_regex);

    // If structId < 0, then this is not a pipe array and don't check the id.
    if ((pipe_match.size() > 0 && seqNum < 0) ||
        (pipe_match.size() > 0 && struct_match.size() > 0)) {
      return true;
    }

    return false;
  };

  int numCallsSkipped = 0;
  for (auto &bb : F) {
    for (auto &instruction : bb) {
      if (auto pipeCall = getPipeCall(&instruction)) {
        auto pipeName =
            demangle(std::string(pipeCall->getCalledFunction()->getName()));
        if (isThisPipe(pipeName)) {
          if (numCallsSkipped == pipeCallsToSkip)
            return pipeCall;
          else
            numCallsSkipped++;
        }
      }
    }
  }

  assert(false && "Pipe for given {pipeInfo} not found.");
  return nullptr;
}


[[maybe_unused]] Instruction *getPointerBase(Value *pointerOperand) {
  const BasicBlock *entryBlockF = &dyn_cast<Instruction>(pointerOperand)
                                       ->getParent()
                                       ->getParent()
                                       ->getEntryBlock();
  if (dyn_cast<Instruction>(pointerOperand)->getParent() == entryBlockF) {
    // stop;
  } else if (auto cast = dyn_cast<BitCastInst>(pointerOperand)) {
    return getPointerBase(dyn_cast<Instruction>(cast->getOperand(0)));
  } else if (auto load = dyn_cast<LoadInst>(pointerOperand)) {
    return getPointerBase(dyn_cast<Instruction>(load->getOperand(0)));
  } else if (auto gep = dyn_cast<GetElementPtrInst>(pointerOperand)) {
    if (gep->getPointerOperand()) // hasAllConstantIndices())
      return getPointerBase(dyn_cast<Instruction>(gep->getPointerOperand()));
  }

  return dyn_cast<Instruction>(pointerOperand);
}

/// Return true if the loop has a "llvm.loop.unroll.enable" metada attached.
[[maybe_unused]] bool isLoopUnrolled(Loop *L) {
  auto latchI = L->getLoopLatch()->getTerminator();
  SmallVector<std::pair<unsigned int, MDNode *>> latchMetadata;
  latchI->getAllMetadataOtherThanDebugLoc(latchMetadata);
  for (auto &m : latchMetadata) {
    for (size_t iO = 0; iO < m.second->getNumOperands(); ++iO) {
      std::string metadataStr;
      llvm::raw_string_ostream rso(metadataStr);
      m.second->getOperand(iO)->print(rso);

      if (metadataStr.size() > 0 &&
          metadataStr.find("llvm.loop.unroll.enable") < metadataStr.size()) {
        return true;
      }
    }
  }

  return false;
}

} // namespace

#endif
