/*
This file defines the information collected during the analysis step.
The PEInfo struct has info needed to create a PE kernel.
The LSQInfo struct has info needed to create an LSQ kernel.
The RewriteRule struct has info needed to perform a certain transformation
(replacing instructions with pipe calls, creating predictaed PEs, or inserting
poison basic blocks).

There are also functions to serialize and deserialize the structs to/from JSON.
*/

#include "CommonLLVM.h"

namespace llvm {

const std::string AGU_ID = "_AGU_";
const std::string LOOP_PE_ID = "_LOOP_PE_";
const std::string BLOCK_PE_ID = "_BLOCK_PE_";

/// All types of transformations performed by our compiler pass to introduce
/// selective dynamic scheduling. The enum also defines the easiest order in
/// which the transformations should be performed.
enum REWRITE_RULE_TYPE {
  UNDEF,

  /******************** LSQ related: */ 
  /// Change a load instruction to a load request pipe write.
  LD_REQ_WRITE,
  /// Change a load instruction to a load value pipe read.
  ST_REQ_WRITE,
  /// Change a store instruction to a store value pipe write.
  LD_VAL_READ,
  /// Change a store instruction to a store request pipe write.
  ST_VAL_WRITE,
  /// Insert an LSQ load value pipe read to poison a misspeculated allocation.
  POISON_LD_READ,
  /// Insert an LSQ store value pipe write to poison a misspeculated allocation.
  POISON_ST_WRITE,
  /// Insert a pipe write with an 'end' signal to an LSQ (in function exit BB).
  END_LSQ_SIGNAL_WRITE,

  /******************** PE related: */ 
  /** Block PEs: */ 
  /// Insert a predicate pipe write that will activate a predicated block PE.
  PRED_BB_WRITE,
  /// Change function to a predicated loop PE and insert a predicate pipe read.
  PRED_BB_READ,
  /// Change an ssa value to a mainKernel-->blockPE pipe write.
  SSA_BB_IN_WRITE,
  /// Change an ssa value to a mainKernel-->blockPE pipe read.
  SSA_BB_IN_READ,
  /// Change an ssa value to a blockPE-->mainKernel pipe write.
  SSA_BB_OUT_WRITE,
  /// Change an ssa value to a blockPE-->mainKernel pipe read.
  SSA_BB_OUT_READ,
  /** Loop PEs: */ 
  /// Change function to a predicated block PE and insert a predicate pipe read.
  PRED_LOOP_WRITE,
  /// Insert a predicate pipe write that will activate a predicated loop PE.
  PRED_LOOP_READ,
  /// Change an ssa value to a mainKernel-->loopPE pipe write.
  SSA_LOOP_IN_WRITE,
  /// Change an ssa value to a mainKernel-->loopPE pipe read.
  SSA_LOOP_IN_READ,
  /// Change an ssa value to a loopPE-->mainKernel pipe write.
  SSA_LOOP_OUT_WRITE,
  /// Change an ssa value to a loopPE-->mainKernel pipe read.
  SSA_LOOP_OUT_READ,

  /******************** Composition of PEs and LSQs related: */ 
  /// Insert a store value tag pipe read.
  ST_VAL_TAG_IN_READ,
  /// Insert a store tag pipe to blockPE, and increment the tag by numBBStores.
  ST_VAL_TAG_TO_BB_WRITE,
  /// Insert a pred write to let a block PE kill misspeculated LSQ allocations.
  POISON_PRED_BB_WRITE,
  /// Create a poison BB in the block PE and insert a poison pipe load in it.
  POISON_IN_BB_PE_LD_READ,
  /// Create a poison BB in the block PE and insert a poison pipe write in it.
  POISON_IN_BB_PE_ST_WRITE,
  /// Insert a store tag pipe to loopPE, and increment the tag, if possible.
  ST_VAL_TAG_TO_LOOP_WRITE,
  /// Insert a store value tag pipe write in loopPE (loopPe --> mainKernel).
  ST_VAL_TAG_LOOP_OUT_WRITE,
};


enum PE_TYPE { BLOCK, LOOP };
struct PEInfo {
  PE_TYPE peType = PE_TYPE::BLOCK;
  std::string peKernelName = "";
  /// Decoupled block if this is a blockPE, or the loop header block if loopPE.
  BasicBlock *basicBlock = nullptr;
  Loop *loop = nullptr;

  /// A block PE will have a special poisonBB if its LSQ allocations are
  /// speculated. Poison blocks in loop PEs are constructed in the usual way.
  bool needsPoisonBlock = false;
  /// If a block PE is invoked inside a nested loop, then it needs to be reset
  /// when exiting the nested loop.
  bool needsResetPredicate = false;

  /// New blocks defining the PE FSM. Created during transformation.
  BasicBlock *pePreHeader = nullptr;
  BasicBlock *peHeader = nullptr;
  BasicBlock *peExiting = nullptr;
  BasicBlock *peExit = nullptr;
  // By definition, there needs to be only one poisonBB, even for multiple LSQs.
  BasicBlock *pePoisonBlock = nullptr;

};

struct LSQInfo {
  int lsqIdx = -1;
  std::string aguKernelName = "";
  std::string arrayType = "";
  bool isAddressGenDecoupled = false;
  bool isAnySpeculation = false;
  bool reuseLdPipesAcrossBB = false;
  bool reuseStPipesAcrossBB = false;
  int numLoadPipes = -1;
  int numStorePipes = -1;
  int allocationQueueSize = -1;
  bool isOnChipMem = false;
  /// Only used if this is a constant sized BRAM array.
  int arraySize = -1;

  /// Used during the transformation. These hold addresses for load/store tags.
  Value *stTagAddr = nullptr;
  Value *ldTagAddr = nullptr;
};

struct RewriteRule {
  // Every rule has to have the following 5 pieces of information:
  REWRITE_RULE_TYPE ruleType = UNDEF;
  std::string kernelName = "";
  Instruction *instruction = nullptr;
  BasicBlock *basicBlock = nullptr;
  CallInst *pipeCall = nullptr;

  // Info used to find a specific pipeCall instruction in a function.
  std::string pipeName = "";
  std::string pipeType = "";
  int pipeArrayIdx = -1;
  
  /// Which LSQ or predicated PE does this rule correspond to.
  int lsqIdx = -1;
  int peIdx = -1;

  /// The recurrence start/end PHIs for instructions in a decoupled block PE.
  PHINode *recurrenceStart = nullptr;
  PHINode *recurrenceEnd = nullptr;

  /// Used for hoisting SSA pipes to loop header/exit blocks.
  bool isHoistedOutOfLoop = false;

  /// Info used to increment the store value tag when LSQ stores span kernels.
  int numStoresInBlock = -1;
  int numStoresInLoop = -1;
  bool canBuildNumStoresInLoopExpr = false;

  /// The basic block to which a speculated LSQ allocation should be hoisted to. 
  BasicBlock *specBasicBlock = nullptr;
  /// Info used to create LSQ poison basic blocks.
  BasicBlock *predBasicBlock = nullptr;
  BasicBlock *succBasicBlock = nullptr;

  // Loop info is only added during transformation.
  Loop *loop = nullptr;
  BasicBlock *loopPreHeaderBlock = nullptr;
  BasicBlock *loopHeaderBlock = nullptr;
  BasicBlock *loopLatchBlock = nullptr;
  BasicBlock *loopExitBlock = nullptr;
};

[[maybe_unused]] std::string toString(REWRITE_RULE_TYPE type) {
  switch (type) {
    case UNDEF: return "UNDEF";
    case LD_REQ_WRITE: return "LD_REQ_WRITE";
    case ST_REQ_WRITE: return "ST_REQ_WRITE";
    case LD_VAL_READ: return "LD_VAL_READ";
    case ST_VAL_WRITE: return "ST_VAL_WRITE";
    case POISON_LD_READ: return "POISON_LD_READ";
    case POISON_ST_WRITE: return "POISON_ST_WRITE";
    case END_LSQ_SIGNAL_WRITE: return "END_LSQ_SIGNAL_WRITE";
    case PRED_BB_WRITE: return "PRED_BB_WRITE";
    case PRED_BB_READ: return "PRED_BB_READ";
    case SSA_BB_IN_WRITE: return "SSA_BB_IN_WRITE";
    case SSA_BB_IN_READ: return "SSA_BB_IN_READ";
    case SSA_BB_OUT_WRITE: return "SSA_BB_OUT_WRITE";
    case SSA_BB_OUT_READ: return "SSA_BB_OUT_READ";
    case PRED_LOOP_WRITE: return "PRED_LOOP_WRITE";
    case PRED_LOOP_READ: return "PRED_LOOP_READ";
    case SSA_LOOP_IN_WRITE: return "SSA_LOOP_IN_WRITE";
    case SSA_LOOP_IN_READ: return "SSA_LOOP_IN_READ";
    case SSA_LOOP_OUT_WRITE: return "SSA_LOOP_OUT_WRITE";
    case SSA_LOOP_OUT_READ: return "SSA_LOOP_OUT_READ";
    case ST_VAL_TAG_IN_READ: return "ST_VAL_TAG_IN_READ";
    case ST_VAL_TAG_TO_BB_WRITE: return "ST_VAL_TAG_TO_BB_WRITE";
    case POISON_PRED_BB_WRITE: return "POISON_PRED_BB_WRITE";
    case POISON_IN_BB_PE_LD_READ: return "POISON_IN_BB_PE_LD_READ";
    case POISON_IN_BB_PE_ST_WRITE: return "POISON_IN_BB_PE_ST_WRITE";
    case ST_VAL_TAG_TO_LOOP_WRITE: return "ST_VAL_TAG_TO_LOOP_WRITE";
    case ST_VAL_TAG_LOOP_OUT_WRITE: return "ST_VAL_TAG_LOOP_OUT_WRITE";
  }
  return "";
}

[[maybe_unused]] PEInfo jsonToPEInfo(Function &F, LoopInfo &LI,
                                     json::Object &obj) {
  PEInfo res;
  res.peType = PE_TYPE(*obj.getInteger("peType"));
  res.peKernelName = *obj.getString("peKernelName");
  res.basicBlock = getBlock(F, *obj.getInteger("basicBlockIdx"));
  res.loop = LI.getLoopFor(res.basicBlock);
  res.needsPoisonBlock = *obj.getBoolean("needsPoisonBlock");
  res.needsResetPredicate = res.loop->getLoopDepth() > 1;

  return res;
}

[[maybe_unused]] json::Object PEInfoToJson(PEInfo &peInfo) {
  json::Object res;

  res["peType"] = int(peInfo.peType);
  res["peKernelName"] = peInfo.peKernelName;
  res["basicBlockIdx"] = getIndexIntoParent(peInfo.basicBlock);
  res["needsPoisonBlock"] = peInfo.needsPoisonBlock;

  return res;
}

[[maybe_unused]] LSQInfo jsonToLSQInfo(Function &F, json::Object &obj) {
  LSQInfo res;

  res.lsqIdx = *obj.getInteger("lsqIdx");
  res.aguKernelName = *obj.getString("aguKernelName");
  res.isAddressGenDecoupled = *obj.getBoolean("isAddressGenDecoupled");
  res.isAnySpeculation = *obj.getBoolean("isAnySpeculation");
  res.reuseLdPipesAcrossBB = *obj.getBoolean("reuseLdPipesAcrossBB");
  res.reuseStPipesAcrossBB = *obj.getBoolean("reuseStPipesAcrossBB");
  res.numLoadPipes = *obj.getInteger("numLoadPipes");
  res.numStorePipes = *obj.getInteger("numStorePipes");
  res.isOnChipMem = *obj.getBoolean("isOnChipMem");

  return res;
}

[[maybe_unused]] json::Object LSQInfoToJson(LSQInfo &lsqInfo) {
  json::Object res;

  res["lsqIdx"] = lsqInfo.lsqIdx;
  res["aguKernelName"] = lsqInfo.aguKernelName;
  res["arrayType"] = lsqInfo.arrayType;
  res["isAddressGenDecoupled"] = lsqInfo.isAddressGenDecoupled;
  res["isAnySpeculation"] = lsqInfo.isAnySpeculation;
  res["reuseLdPipesAcrossBB"] = lsqInfo.reuseLdPipesAcrossBB;
  res["reuseStPipesAcrossBB"] = lsqInfo.reuseStPipesAcrossBB;
  res["numLoadPipes"] = lsqInfo.numLoadPipes;
  res["numStorePipes"] = lsqInfo.numStorePipes;
  res["allocationQueueSize"] = lsqInfo.allocationQueueSize;
  res["isOnChipMem"] = lsqInfo.isOnChipMem;
  res["arraySize"] = lsqInfo.arraySize;

  return res;
}

[[maybe_unused]] RewriteRule jsonToRewriteRule(Function &F, LoopInfo &LI,
                                               json::Object obj) {
  RewriteRule res;

  res.ruleType = REWRITE_RULE_TYPE(*obj.getInteger("ruleType"));
  res.kernelName = std::string(*obj.getString("kernelName"));

  assert(res.kernelName == demangle(std::string(F.getNameOrAsOperand())) &&
         "Tried to deserialize a rewriteRule for another function.");

  auto instrBB = getBlock(F, *obj.getInteger("instructionBasicBlockIdx"));
  res.instruction = getInstruction(*instrBB, *obj.getInteger("instructionIdx"));
  res.basicBlock = getBlock(F, *obj.getInteger("basicBlockIdx"));
  res.pipeCall = getPipeCall(F, obj);

  if (auto opt = obj.getInteger("lsqIdx"))
    res.lsqIdx = *opt;
  if (auto opt = obj.getInteger("peIdx"))
    res.peIdx = *opt;
  if (auto opt = obj.getInteger("numStoresInBlock"))
    res.numStoresInBlock = *opt;
  if (auto opt = obj.getBoolean("canBuildNumStoresInLoopExpr"))
    res.canBuildNumStoresInLoopExpr = *opt;
  if (auto opt = obj.getInteger("predBasicBlockIdx"))
    res.predBasicBlock = getBlock(F, *opt);
  if (auto opt = obj.getInteger("succBasicBlockIdx"))
    res.succBasicBlock = getBlock(F, *opt);
  if (auto opt = obj.getInteger("specBasicBlockIdx"))
    res.specBasicBlock = getBlock(F, *opt);
  if (auto opt = obj.getBoolean("isHoistedOutOfLoop"))
    res.isHoistedOutOfLoop = *opt;
  if (auto opt = obj.getInteger("recurrenceStartBasicBlockIdx")) {
    auto recStartBB = getBlock(F, *opt);
    res.recurrenceStart = dyn_cast<PHINode>(
        getInstruction(*recStartBB, *obj.getInteger("recurrenceStartIdx")));
  }
  if (auto opt = obj.getInteger("recurrenceEndBasicBlockIdx")) {
    auto recEndBB = getBlock(F, *opt);
    res.recurrenceEnd = dyn_cast<PHINode>(
        getInstruction(*recEndBB, *obj.getInteger("recurrenceEndIdx")));
  }

  // Get loop info.
  res.loop = LI.getLoopFor(res.basicBlock);
  if (res.loop) {
    res.loopPreHeaderBlock = res.loop->getLoopPreheader();
    res.loopHeaderBlock = res.loop->getHeader();
    res.loopLatchBlock = res.loop->getLoopLatch();
    res.loopExitBlock = res.loop->getExitBlock();
  }

  return res;
}

[[maybe_unused]] json::Object rewriteRuleToJson(RewriteRule &rule) {
  json::Object res;

  res["ruleType"] = int(rule.ruleType);
  res["ruleTypeString"] = toString(rule.ruleType);
  res["kernelName"] = rule.kernelName;
  // Intstruction and basic blocks are serialized into indices.
  res["instructionIdx"] = getIndexIntoParent(rule.instruction);
  // Note that the BB held by a rule might be different than instruction's BB.
  res["instructionBasicBlockIdx"] =
      getIndexIntoParent(rule.instruction->getParent());
  res["basicBlockIdx"] = getIndexIntoParent(rule.basicBlock);

  res["pipeName"] = rule.pipeName;
  res["pipeType"] = rule.pipeType;
  if (rule.pipeArrayIdx >= 0)
    res["pipeArrayIdx"] = rule.pipeArrayIdx;

  if (rule.lsqIdx >= 0)
    res["lsqIdx"] = rule.lsqIdx;
  if (rule.peIdx >= 0)
    res["peIdx"] = rule.peIdx;
  if (rule.numStoresInBlock >= 0)
    res["numStoresInBlock"] = rule.numStoresInBlock;
  if (rule.ruleType == REWRITE_RULE_TYPE::ST_VAL_TAG_TO_LOOP_WRITE)
    res["canBuildNumStoresInLoopExpr"] = rule.canBuildNumStoresInLoopExpr;
  if (rule.predBasicBlock)
    res["predBasicBlockIdx"] = getIndexIntoParent(rule.predBasicBlock);
  if (rule.succBasicBlock)
    res["succBasicBlockIdx"] = getIndexIntoParent(rule.succBasicBlock);
  if (rule.specBasicBlock)
    res["specBasicBlockIdx"] = getIndexIntoParent(rule.specBasicBlock);
  if (rule.peIdx >= 0)
    res["isHoistedOutOfLoop"] = rule.isHoistedOutOfLoop;
  if (rule.recurrenceStart) {
    res["recurrenceStartBasicBlockIdx"] =
        getIndexIntoParent(rule.recurrenceStart->getParent());
    res["recurrenceStartIdx"] = getIndexIntoParent(rule.recurrenceStart);
  }
  if (rule.recurrenceEnd) {
    res["recurrenceEndBasicBlockIdx"] =
        getIndexIntoParent(rule.recurrenceEnd->getParent());
    res["recurrenceEndIdx"] = getIndexIntoParent(rule.recurrenceEnd);
  }

  return res;
}

[[maybe_unused]] llvm::raw_ostream &operator<<(llvm::raw_ostream &os,
                                               const RewriteRule &rule) {
  os << "type: " << toString(rule.ruleType) << "\n";
  os << "kernelName: " << rule.kernelName << "\n";
  os << "instruction: ";
  rule.instruction->print(os);
  os << "\n";
  os << "basicBlock: " << rule.basicBlock->getNameOrAsOperand() << "\n";
  os << "pipeCall: ";
  rule.pipeCall->print(os);
  os << "\n";

  if (rule.loopHeaderBlock)
    os << "loopHeaderBlock: " << rule.loopHeaderBlock->getNameOrAsOperand()
       << "\n";
  if (rule.loopLatchBlock)
    os << "loopLatchBlock: " << rule.loopLatchBlock->getNameOrAsOperand()
       << "\n";
  if (rule.loopExitBlock)
    os << "loopExitBlock: " << rule.loopExitBlock->getNameOrAsOperand() << "\n";
  if (rule.lsqIdx >= 0)
    os << "lsqIdx: " << rule.lsqIdx << "\n";
  if (rule.numStoresInBlock >= 0)
    os << "numStoresInBlock: " << rule.numStoresInBlock << "\n";
  if (rule.ruleType == REWRITE_RULE_TYPE::ST_VAL_TAG_TO_LOOP_WRITE)
    os << "canBuildNumStoresInLoopExpr: " << rule.canBuildNumStoresInLoopExpr
       << "\n";
  if (rule.predBasicBlock)
    os << "predBasicBlockIdx: " << rule.predBasicBlock->getNameOrAsOperand()
       << "\n";
  if (rule.succBasicBlock)
    os << "succBasicBlockIdx: " << rule.succBasicBlock->getNameOrAsOperand()
       << "\n";
  if (rule.specBasicBlock)
    os << "specBasicBlockIdx: " << rule.specBasicBlock->getNameOrAsOperand()
       << "\n";
  
  return os;
}

/// Transform the LSQInfo, PEInfo, and RewriteRule structs into json objects.
/// Return a top level json report with all three arrays.
[[maybe_unused]] json::Object
serializeAnalysis(Function &F, SmallVector<LSQInfo> &lsqArray,
                  SmallVector<PEInfo> &peArray,
                  SmallVector<RewriteRule> &rewriteRules) {
  json::Object report;
  report["mainKernelName"] = demangle(std::string(F.getName()));
  report["kernelStartLine"] = F.getSubprogram()->getLine();

  auto peArrayJson = json::Array();
  for (auto &peInfo : peArray)
    peArrayJson.push_back(PEInfoToJson(peInfo));
  report["peArray"] = std::move(peArrayJson);

  auto lsqArrayJson = json::Array();
  for (auto &lsqInfo : lsqArray)
    lsqArrayJson.push_back(LSQInfoToJson(lsqInfo));
  report["lsqArray"] = std::move(lsqArrayJson);
  
  auto rewriteRulesJson = json::Array();
  for (auto &rule : rewriteRules)
    rewriteRulesJson.push_back(rewriteRuleToJson(rule));
  report["rewriteRules"] = std::move(rewriteRulesJson);

  return report;
}

/// Collect the LSQInfo, PEInfo, and RewriteRule structs from a json object.
[[maybe_unused]] void
deserializeAnalysis(Function &F, LoopInfo &LI, const json::Object &report,
                    SmallVector<LSQInfo> &lsqArray,
                    SmallVector<PEInfo> &peArray,
                    SmallVector<RewriteRule> &rewriteRules) {
  auto thisKernelName = demangle(std::string(F.getNameOrAsOperand()));
  for (auto ruleJsonVal : *report.getArray("rewriteRules")) {
    auto ruleJson = *ruleJsonVal.getAsObject();
    if (ruleJson.getString("kernelName") == thisKernelName) 
      rewriteRules.push_back(jsonToRewriteRule(F, LI, ruleJson));
  }

  for (auto peInfoJson : *report.getArray("peArray")) 
    peArray.push_back(jsonToPEInfo(F, LI, *peInfoJson.getAsObject()));
  for (auto lsqInfoJson : *report.getArray("lsqArray")) 
    lsqArray.push_back(jsonToLSQInfo(F, *lsqInfoJson.getAsObject()));
}

} // end namespace llvm
