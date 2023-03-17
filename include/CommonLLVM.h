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
using Pipe2Inst = std::pair<CallInst *, Instruction *>;

/// Lambdas for easier use in range based algorithms.
auto isaLoad = [](auto i) { return isa<LoadInst>(i); };
auto isaStore = [](auto i) { return isa<StoreInst>(i); };

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

/// Given a {val}, store it into the operand of the {pipe} write.
[[maybe_unused]] void storeValIntoPipe(Value *val, CallInst *pipe) {
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

  Builder.CreateStore(val, pipeOperandAddr);
}

/// Delete {inst} from it's function.
[[maybe_unused]] void deleteInstruction(Instruction *inst) {
  inst->dropAllReferences();
  inst->replaceAllUsesWith(UndefValue::get(inst->getType()));
  inst->eraseFromParent();
}

/// Given Function {F}, return all Functions that call {F}.
[[maybe_unused]] SmallVector<Function *> getCallerFunctions(Module *M,
                                                            Function &F) {
  // The expected case is one caller.
  SmallVector<Function *, 1> callers;
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

/// Return the sycl kernel name for this function, or "" if not a kernel.
[[maybe_unused]] std::string getKernelName(Function &F) {
    auto callers = getCallerFunctions(F.getParent(), F);
    if (callers.size() == 0)
      return "";

    // Get the "MainKernel" bit in "typeinfo name for MainKernel".
    auto fullName = demangle(std::string(callers[0]->getName()));
    size_t lastSpace = 0;
    for (size_t i=0; i<fullName.size(); ++i) {
      if (fullName[i] == ' ')
        lastSpace = i;
    }
    return std::string(fullName.begin() + lastSpace + 1, fullName.end());
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

/// Return the line associated witht the return from {F}.
[[maybe_unused]] int getReturnLine(Function &F) {
  if (auto retBB = getReturnBlock(F))
      return retBB->getTerminator()->getDebugLoc().getLine();

  assert(false && "Did not find return statement in function.");
  return -1;
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
    assert(Json && "Error parsing json loop-raw-report");

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
  auto bbIdx = int(iDescr["basic_block_idx"].getAsInteger().getValue());
  auto instrIdx = int(iDescr["instruction_idx"].getAsInteger().getValue());
  auto BB = getChildWithIndex<Function, BasicBlock>(&F, bbIdx);
  auto I = getChildWithIndex<BasicBlock, Instruction>(BB, instrIdx);
  assert(I && "Instruction for given iDescr not found.");
  return I;
}

/// Return the pipe call instruction corresponding to the pipeInfo json obj.
[[maybe_unused]] CallInst *getPipeCall(Function &F, json::Object pipeInfo) {
  auto pipeName = std::string(pipeInfo["name"].getAsString().getValue());
  // If no struct_id or repeat_id, then use defaults.
  auto structIdOptional = pipeInfo["struct_id"].getAsInteger();
  auto structId = structIdOptional ? structIdOptional.getValue() : -1;
  auto repeatIdOptional = pipeInfo["repeat_id"].getAsInteger();
  auto repeatId = repeatIdOptional ? repeatIdOptional.getValue() : 0;

  /// Lambda. Returns true, if {PIPE_CALL} is a substring of {call}.
  auto isaPipe = [] (std::string call) { 
    const std::string PIPE_CALL = "ext::intel::pipe";
    return call.find(PIPE_CALL) != std::string::npos;
  };
  /// Lambda. Returns true if {call} is a call to our pipe.
  auto isThisPipe = [&pipeName, &structId] (std::string call) { 
    std::regex pipe_regex{pipeName, std::regex_constants::ECMAScript};
    std::smatch pipe_match;
    std::regex_search(call, pipe_match, pipe_regex);

    std::regex struct_regex{"StructId<" + std::to_string(structId) + "ul>", 
                            std::regex_constants::ECMAScript};
    std::smatch struct_match;
    std::regex_search(call, struct_match, struct_regex);

    // If structId < 0, then this is not a pipe array and don't check the id.
    if ((pipe_match.size() > 0 && structId < 0) || 
        (pipe_match.size() > 0 && struct_match.size() > 0)) {
      return true;
    }

    return false;
  };

  // Go through every instruction in {F} to find the desired pipe call.
  // Pipe info specifies which Nth call to the same pipe we should return.
  int callsSoFar = 0;
  for (auto &bb : F) {
    for (auto &instruction : bb) {
      if (CallInst *callInst = dyn_cast<CallInst>(&instruction)) {
        if (Function *f = callInst->getCalledFunction()) {
          auto fName = demangle(std::string(f->getName()));
          if (f->getCallingConv() == CallingConv::SPIR_FUNC && isaPipe(fName) &&
              isThisPipe(fName)) {
            if (repeatId == callsSoFar)
              return callInst;
            else
              callsSoFar++;
          }
        }
      }
    }
  }

  assert(false && "Pipe for given {pipeInfo} not found.");
  return nullptr;
}

/// Given a json array with objects describing pipe to instruction objects,
/// return a vector of such Pipe2Inst mappings.
///
/// Example {mapDescriptions}:
///   {
///     "instruction": {
///       "basic_block_idx": 1,
///       "instruction_idx": 0
///     },
///     "type": "float",
///     "name": "pipe_float_kernel0_in0_class",
///     "repeat_id": 0,
///     "struct_id": -1
///   }
[[maybe_unused]] SmallVector<Pipe2Inst>
getPipe2InstMaps(Function &F, json::Array &mapDescriptions) {
  SmallVector<Pipe2Inst> result;

  for (json::Value &mapDescr : mapDescriptions) {
    // Given the pipe name, find the pipe call instruction.
    auto mapDescrObj = *mapDescr.getAsObject();
    auto pipeCall = getPipeCall(F, mapDescrObj);
    // Given indexes of children in parents, arrive at the right instruction.
    auto I = getInstruction(F, *mapDescrObj["instruction"].getAsObject());
    result.push_back({pipeCall, I});
  }

  return result;
}

} // namespace

#endif
