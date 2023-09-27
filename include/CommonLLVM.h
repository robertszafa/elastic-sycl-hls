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

/// Lambdas for easier use in range based algorithms.
auto isaLoad = [](auto i) { return isa<LoadInst>(i); };
auto isaStore = [](auto i) { return isa<StoreInst>(i); };

[[maybe_unused]] CallInst *getPipeCall(Instruction *I) {
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

/// Given a range of instructions, for each I return its position in its basic
/// block relative to all other instructions in the range.
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

/// Delete {inst} from its function.
[[maybe_unused]] void deleteInstruction(Instruction *I) {
  I->dropAllReferences();
  I->replaceAllUsesWith(UndefValue::get(I->getType()));
  I->eraseFromParent();
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
template <typename T> [[maybe_unused]] int getIndexIntoParent(T *Child) {
  int idx = -1;
  for (auto &ch : *Child->getParent()) {
    idx++;
    if (&ch == Child)
      break;
  }
  return idx;
}

[[maybe_unused]] BasicBlock *getBlock(Function &F, const int idx) {
  int i = 0;
  for (auto &BB : F) {
    if (i == idx)
      return &BB;
    i++;
  }

  return nullptr;
}

[[maybe_unused]] Instruction *getInstruction(BasicBlock &BB, const int idx) {
  int i = 0;
  for (auto &I : BB) {
    if (i == idx)
      return &I;
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

/// Return the pipe call instruction corresponding to the pipeInfo json obj.
[[maybe_unused]] CallInst *getPipeCall(Function &F, json::Object &pipeInfo) {
  static StringMap<int> collectedCalls;

  auto pipeNameOpt = pipeInfo.getString("pipeName");
  assert(pipeNameOpt && "Pipe in getPipeCall(F, json::Object) not found.");
  auto pipeName = pipeNameOpt->str();
  // If no pipe_array_idx or repeat_id, then use defaults.
  auto seqNumOpt = pipeInfo.getInteger("pipeArrayIdx");
  auto seqNum = seqNumOpt ? seqNumOpt.value() : -1;

  const std::string pipeIdKey =
      std::string(F.getName()) + pipeName + std::to_string(seqNum);
  int pipeCallsToSkip = 0;
  if (collectedCalls.contains(pipeIdKey)) {
    pipeCallsToSkip = collectedCalls[pipeIdKey];
    collectedCalls[pipeIdKey]++;
  } else {
    collectedCalls[pipeIdKey] = 1;
  }

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

  errs() << "Pipe name " << pipeName << "\n";
  assert(false && "Pipe in getPipeCall(F, json::Object) not found.");
  return nullptr;
}

[[maybe_unused]] CallInst *getPipeWithPattern(BasicBlock &BB,
                                              const std::string &pattern) {
  for (auto &I : BB) {
    if (auto pipeCall = getPipeCall(&I)) {
      auto pipeName =
          demangle(std::string(pipeCall->getCalledFunction()->getName()));
      if (pipeName.find(pattern) < pipeName.size())
        return pipeCall;
    }
  }

  return nullptr;
}

[[maybe_unused]] CallInst *getPipeWithPattern(Function &F,
                                              const std::string &pattern) {
  for (auto &BB : F) {
    if (auto pipeCall = getPipeWithPattern(BB, pattern))
      return pipeCall;
  }

  return nullptr;
}

[[maybe_unused]] bool isPipeRead(Instruction *I) {
  if (auto pipeCall = getPipeCall(I)) {
    auto pipeName =
        demangle(std::string(pipeCall->getCalledFunction()->getName()));
    return pipeName.find("::read(") < pipeName.size();
  }

  return false;
}

[[maybe_unused]] bool isPipeWrite(Instruction *I) {
  if (auto pipeCall = getPipeCall(I)) {
    auto pipeName =
        demangle(std::string(pipeCall->getCalledFunction()->getName()));
    return pipeName.find("::write(") < pipeName.size();
  }

  return false;
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

/// Given a SCEV, return its string representation.
[[maybe_unused]] std::string getSCEVString(const SCEV *scev) {
  std::string valueStr;
  llvm::raw_string_ostream rso(valueStr);
  scev->print(rso);

  return valueStr;
}

/// Given a SCEV string name, return its LLVM value.
[[maybe_unused]] Value *getValueForSCEVName(Function &F,
                                            const std::string &name) {
  // If not LLVM value, then create a constant.
  if (name[0] != '%') {
    return ConstantInt::get(Type::getInt32Ty(F.getContext()), std::stoi(name));
  }

  // Check function arguments.
  for (auto &Arg : F.args()) {
    if (auto ArgVal = dyn_cast<Value>(&Arg)) {
      // The first char in <name> is "%", so ignore that.
      if (name.find(ArgVal->getName()) == 1)
        return ArgVal;
    }
  }
  // Check function instructions.
  for (auto &BB : F) {
    for (auto &I : BB) {
      if (auto IVal = dyn_cast<Value>(&I)) {
        if (name.find(IVal->getName()) == 1)
          return IVal;
      }
    }
  }

  return nullptr;
}

/// Given a SCEV, recursively build an expression using LLVM instructions that
/// calculates its value. Return the final LLVM value.
[[maybe_unused]] Value *buildSCEVExpr(Function &F, const SCEV *scev,
                                      Instruction *insrtBefore) {
  const int numOperands = scev->operands().size();

  if (numOperands == 0) {
    return getValueForSCEVName(F, getSCEVString(scev));
  } else if (numOperands == 1) {
    // TODO single oper SCEVs: scVScale, scTruncate, scZeroExtend, scSignExtend,
    return nullptr;
  } else {
    auto LHS = buildSCEVExpr(F, scev->operands()[0], insrtBefore);
    auto RHS = buildSCEVExpr(F, scev->operands()[1], insrtBefore);
    IRBuilder<> IR(insrtBefore);

    auto type = scev->getSCEVType();
    if (type == scAddExpr || type == scAddRecExpr)
      return IR.CreateAdd(LHS, RHS);
    else if (type == scMulExpr)
      return IR.CreateMul(LHS, RHS);
    else if (type == scUDivExpr)
      return IR.CreateUDiv(LHS, RHS);
    else if (type == scUMaxExpr || type == scSMaxExpr)
      return IR.CreateMaximum(LHS, RHS);
    else if (type == scUMinExpr || type == scSMinExpr)
      return IR.CreateMaximum(LHS, RHS);
  }

  return nullptr;
}

/// Return blocks unique to this loop, i.e. not contained in any subloop.
[[maybe_unused]] SmallVector<BasicBlock *> getUniqueLoopBlocks(Loop *L) {
  SetVector<BasicBlock *> blocksOfSubloops;
  for (auto subLoop : L->getSubLoops()) {
    for (auto BB : subLoop->blocks())
      blocksOfSubloops.insert(BB);
  }

  SmallVector<BasicBlock *> thisLoopBlocks;
  for (auto BB : L->getBlocks()) {
    if (!blocksOfSubloops.contains(BB))
      thisLoopBlocks.push_back(BB);
  }

  return thisLoopBlocks;
}

/// Return instructions unique to this loop, i.e. not contained in any subloop.
[[maybe_unused]] SmallVector<Instruction *> getUniqueLoopInstructions(Loop *L) {
  SmallVector<Instruction *> res;

  for (auto BB : getUniqueLoopBlocks(L)) {
    for (auto &I : *BB) {
      res.push_back(&I);
    }
  }

  return res;
}

} // namespace

#endif
