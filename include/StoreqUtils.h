#ifndef STOREQ_UTILS_H
#define STOREQ_UTILS_H

#include "llvm/IR/Module.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Constants.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/Demangle/Demangle.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/JSON.h"

#include <fstream>
#include <sstream>
#include <string>

using namespace llvm;


namespace storeq {
  SmallVector<Function *> getCallerFunctions(Module *M, Function &F) {
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


/// This function can be used to identrify spir functions that need to be transformed.
/// This is done by detecting annotations.
SmallVector<Function *> getAnnotatedFunctions(Module *M) {
  SmallVector<Function *> annotFuncs;
  const std::string AnnotationString("load_num_0");

  for (Module::global_iterator I = M->global_begin(), E = M->global_end(); I != E; ++I) {
    if (I->getName() == "llvm.global.annotations") {
      ConstantArray *CA = dyn_cast<ConstantArray>(I->getOperand(0));
      for (auto OI = CA->op_begin(); OI != CA->op_end(); ++OI) {
        ConstantStruct *CS = dyn_cast<ConstantStruct>(OI->get());
        Function *FUNC = dyn_cast<Function>(CS->getOperand(0)->getOperand(0));
        GlobalVariable *AnnotationGL = dyn_cast<GlobalVariable>(CS->getOperand(1)->getOperand(0));
        StringRef annotation =
            dyn_cast<ConstantDataArray>(AnnotationGL->getInitializer())->getAsCString();
        if (annotation.compare(AnnotationString) == 0) {
          annotFuncs.push_back(FUNC);
          // errs() << "Found annotated function " << FUNC->getName()<<"\n";
        }
      }
    }
  }

  return annotFuncs;
}

json::Value parseJsonReport(const std::string fname) {
  std::ifstream t(fname);
  std::stringstream buffer;
  buffer << t.rdbuf();

  auto Json = json::parse(llvm::StringRef(buffer.str()));
  assert(Json && "Error parsing json loop-raw-report");

  if (Json)
    return *Json;
  return json::Value(nullptr);
}

} // end namespace storeq

#endif