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
#include "llvm/ADT/SCCIterator.h"

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

using CFGEdge = std::pair<BasicBlock *, BasicBlock *>;

/// Lambdas for easier use in range based algorithms.
auto isaLoad = [](auto i) { return isa<LoadInst>(i); };
auto isaStore = [](auto i) { return isa<StoreInst>(i); };

[[maybe_unused]] bool
isSpirKernelWithSubstring(Function &F, const std::string &SearchString) {
  std::string fName = demangle(F.getNameOrAsOperand());
  return (fName.find(SearchString) < fName.size()) &&
         (F.getCallingConv() == CallingConv::SPIR_KERNEL);
}

[[maybe_unused]] bool isSpirFuncWithSubstring(Function &F,
                                              const std::string &SearchString) {
  std::string fName = demangle(F.getNameOrAsOperand());
  return (fName.find(SearchString) < fName.size()) &&
         (F.getCallingConv() == CallingConv::SPIR_FUNC);
}

[[maybe_unused]] CallInst *getPipeCall(Instruction *I) {
  const std::string PIPE_CALL = "ext::intel::pipe";
  const std::string EXP_PIPE_CALL = "ext::intel::experimental::pipe";

  if (CallInst *callInst = dyn_cast<CallInst>(I)) {
    if (Function *f = callInst->getCalledFunction()) {
      if (isSpirFuncWithSubstring(*f, PIPE_CALL) ||
          isSpirFuncWithSubstring(*f, EXP_PIPE_CALL)) {
        return callInst;
      }
    }
  }

  return nullptr;
}

[[maybe_unused]] std::string getLLVMTypeString(const Instruction *I) {
  std::string typeStr;
  llvm::raw_string_ostream rso(typeStr);
  I->getType()->print(rso);

  return typeStr;
}

[[maybe_unused]] std::string getCTypeString(const Instruction *I) {
  const std::string llvmtype = getLLVMTypeString(I);

  if (llvmtype.find("i1") != std::string::npos)
    return "bool";
  else if (llvmtype.find("i8") != std::string::npos)
    return "signed char";
  else if (llvmtype.find("i16") != std::string::npos)
    return "signed short";
  else if (llvmtype.find("i32") != std::string::npos)
    return "int";
  else if (llvmtype.find("i64") != std::string::npos)
    return "signed long int";
  else if (llvmtype.find("addrspace") != std::string::npos)
    return "int64_t";

  return llvmtype;
}

[[maybe_unused]] std::string getLoadStoreCTypeString(const Instruction *I) {
  if (isaLoad(I))
    return getCTypeString(I); 
  else if (isaStore(I))
    return getCTypeString(dyn_cast<Instruction>(I->getOperand(0)));

  assert(false && "Not a load or store.");
  return "";
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

/// Return the pipe call instruction corresponding to the pipeName and idx pack.
[[maybe_unused]] CallInst *getPipeCall(Function &F, const std::string &pipeName,
                                       const SmallVector<int> pipeIdxs={}) {
  static StringMap<int> collectedCalls;

  // Flatten the index pack into a single string.
  std::string pipeIdxAccess = "StructId<";
  for (size_t i = 0; i < pipeIdxs.size(); ++i) {
    if (i > 0)
      pipeIdxAccess += ", ";
    pipeIdxAccess += std::to_string(pipeIdxs[i]) + "ul";
  }
  pipeIdxAccess += ">";

  // Keep track of already found pipe calls.
  std::string pipeIdKey = std::string(F.getName()) + pipeName + pipeIdxAccess;
  int pipeCallsToSkip = 0;
  if (collectedCalls.contains(pipeIdKey)) {
    pipeCallsToSkip = collectedCalls[pipeIdKey];
    collectedCalls[pipeIdKey]++;
  } else {
    collectedCalls[pipeIdKey] = 1;
  }

  /// Lambda. Returns true if {call} is a call to our pipe.
  auto isThisPipe = [&](std::string &thisPipeName) {
    std::regex pipeNameReg{pipeName + "_?", std::regex_constants::ECMAScript};
    std::regex pipeIdxRegex{pipeIdxAccess, std::regex_constants::ECMAScript};

    std::smatch nameMatch;
    std::smatch idxMatch;
    bool pipeNameOk = std::regex_search(thisPipeName, nameMatch, pipeNameReg);
    bool pipeIdxOk = pipeIdxs.empty() ||
                     std::regex_search(thisPipeName, idxMatch, pipeIdxRegex);

    return pipeNameOk && pipeIdxOk;
  };

  int numCallsSkipped = 0;
  for (auto &bb : F) {
    for (auto &instruction : bb) {
      if (auto pipeCall = getPipeCall(&instruction)) {
        auto thisPipeName =
            demangle(std::string(pipeCall->getCalledFunction()->getName()));
        if (isThisPipe(thisPipeName)) {
          if (numCallsSkipped == pipeCallsToSkip)
            return pipeCall;
          else
            numCallsSkipped++;
        }
      }
    }
  }

  errs() << "ERROR: Not found pipe name " << pipeName
         << " with idxs: " << pipeIdxAccess << " in kernel "
         << demangle(std::string(F.getNameOrAsOperand())) << "\n";
  assert(false && "Pipe in getPipeCall(F, json::Object) not found.");
  return nullptr;
}

[[maybe_unused]] CallInst *getPipeCall(Function &F, json::Object &pipeInfo) {
  auto pipeNameOpt = pipeInfo.getString("pipeName");
  assert(pipeNameOpt && "Pipe in getPipeCall(F, json::Object) not found.");
  auto pipeName = pipeNameOpt->str();

  // If no pipe_array_idx or repeat_id, then use defaults.
  auto pipeIdxOpt = pipeInfo.getInteger("pipeArrayIdx");
  if (pipeIdxOpt)
    return getPipeCall(F, pipeName, {int(pipeIdxOpt.value())});
  else
    return getPipeCall(F, pipeName, {});
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
  if (dyn_cast<Instruction>(pointerOperand)->getParent() == entryBlockF &&
      !isa<GetElementPtrInst>(pointerOperand)) {
    // stop;
  } else if (auto cast = dyn_cast<BitCastInst>(pointerOperand)) {
    return getPointerBase(dyn_cast<Instruction>(cast->getOperand(0)));
  } else if (auto load = dyn_cast<LoadInst>(pointerOperand)) {
    return getPointerBase(dyn_cast<Instruction>(load->getOperand(0)));
  } else if (auto gep = dyn_cast<GetElementPtrInst>(pointerOperand)) {
    if (auto gepPtr = gep->getPointerOperand()) {
      auto ptrSSaName = demangle(gepPtr->getNameOrAsOperand());
      const std::string KERNEL_ARGS = "SYCLKernel";
      bool isPtrToKernelArgs = ptrSSaName.find(KERNEL_ARGS) < ptrSSaName.size();
      
      // Stop if the ptrOp points to a struct of SYCL kernel arguments.
      if (!isPtrToKernelArgs)
        return getPointerBase(dyn_cast<Instruction>(gepPtr));
    }
  }

  return dyn_cast<Instruction>(pointerOperand);
}

[[maybe_unused]] AllocaInst *getAllocaOfPointerBase(Value *pointerBase) {
  SmallVector<Value *> Worklist {pointerBase};
  SetVector<Value *> done;

  while (!Worklist.empty()) {
    auto CurrVal = Worklist.pop_back_val();

    if (auto AllocaI = dyn_cast<AllocaInst>(CurrVal))
      return AllocaI;

    if (auto CurrI = dyn_cast<Instruction>(CurrVal)) {
      for (auto &Op : CurrI->operands()) {
        if (auto OpVal = dyn_cast<Value>(Op)) {
          if (!done.contains(OpVal)) {
            Worklist.push_back(OpVal);
            done.insert(OpVal);
          }
        }
      }

    }
  }

  return nullptr;
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
      return IR.CreateSelect(IR.CreateICmpSGT(LHS, RHS), LHS, RHS);
    else if (type == scUMinExpr || type == scSMinExpr)
      return IR.CreateSelect(IR.CreateICmpSLT(LHS, RHS), LHS, RHS);
  }

  return nullptr;
}

/// Return blocks unique to this loop, i.e. not contained in any subloop.
[[maybe_unused]] SmallVector<BasicBlock *> getUniqueLoopBlocks(Loop *L) {
  SetVector<BasicBlock *> blocksOfSubloops;
  for (auto subLoop : L->getSubLoops()) {
    for (auto BB : subLoop->blocks()) {
      if (BB != L->getHeader())
        blocksOfSubloops.insert(BB);
    }
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

/// Given an instruction {I}, return the first user of {I} in {BB}. 
/// Return a nullptr if none exists.
[[maybe_unused]] Instruction *getFirstUserInBB(Instruction *I, BasicBlock *BB) {
  for (auto user : I->users()) {
    if (auto userI = dyn_cast<Instruction>(user)) {
      if (userI->getParent() == BB)
        return userI;
    }
  }

  return nullptr;
}

/// Determine whether block 'To' is reachable from 'From' within the loop 'L',
/// i.e. without taking the loop back edge.
///
/// CFG.h has an isPotentiallyReachable API, but that assumes that every block
/// within loop has a path to every other block (via the backedge). It would
/// require adding loop exit/header to the exclusion list. The below is simpler.
[[maybe_unused]] bool isReachableWithinLoop(const BasicBlock *From,
                                            const BasicBlock *To,
                                            const Loop *L) {
  assert(L->contains(From) && L->contains(To) &&
         "isReachableWithinLoop(From, To, L): From/To blocks not in L body.");

  const BasicBlock *Stop = L->getLoopLatch();

  if (Stop == To)
    return true; // 'From' in loop body always reaches loop latch. 

  SetVector<const BasicBlock *> visited;
  SmallVector<const BasicBlock *> worklist = {From};

  bool isReachable = false;
  while (!worklist.empty()) {
    auto Current = worklist.pop_back_val();
    
    if (Current == To) {
      isReachable = true;
      break;
    } else if (Current == Stop) {
      continue;
    } else if (!visited.contains(Current)) {
      visited.insert(Current);
      worklist.append(succ_begin(Current), succ_end(Current));
    }
  }

  return isReachable;
}

/// Return true, if there is a possibility that {src} could be executed on any
/// of the def-use path leading to {dst}.
[[maybe_unused]] bool isInDefUsePath(Instruction *src, Instruction *dst) {
  SmallVector<Instruction *> workList, doneList;
  workList.push_back(dst);

  // Work up the def-use chain of {dst} until hitting {src} or arriving the the
  // F.entry. Special treatment of phi nodes: add terminating instructions of
  // the incoming basic blocks to the worklist (if {src} is in the def-use chain
  // of the terminator, then there is a possibility that it will be executed on
  // the path to {dst}.
  while (!workList.empty()) {
    auto currDst = workList.pop_back_val();
    doneList.push_back(currDst);

    // Case 1: There is a direct data dependency, so this def-use chain exists: 
    // F.entry ~> .. ~> {src} .. ~> {dst}
    for (auto &Use : currDst->operands()) {
      if (auto UseI = dyn_cast<Instruction>(Use.get())) {
        if (UseI == src)
          return true;

        if (!llvm::is_contained(doneList, UseI))
          workList.push_back(UseI);
      }
    }

    // Case 2: Some value in {dst} def-use chain is a phi node and the
    // terminator of the incoming basic block has {src} in this def-use chain.
    if (auto dstPhi = dyn_cast<PHINode>(currDst)) {
      for (auto incomingBB : dstPhi->blocks()) {
        if (!llvm::is_contained(doneList, incomingBB->getTerminator()))
          workList.push_back(incomingBB->getTerminator());
      }
    }
  }

  return false;
}

/// Return vector of function basic blocks in reverse post (topological) order.
[[maybe_unused]] SmallVector<BasicBlock *> getTopologicalOrder(Function &F) {
  SmallVector<BasicBlock *> Result;

  // Within loops, reverse post order of a DAG is the same as its topological
  // order. First, iterate over the loops (SCCs) in reverse post order.
  for (scc_iterator<Function *> SCCI = scc_begin(&F); !SCCI.isAtEnd(); ++SCCI) {
    const std::vector<BasicBlock *> &NextSCC = *SCCI;
    for (BasicBlock *BB : NextSCC) {
      Result.push_back(BB);
    }
  }

  // Remeber to reverse.
  std::reverse(Result.begin(), Result.end());
  return Result;
}

} // namespace

#endif
