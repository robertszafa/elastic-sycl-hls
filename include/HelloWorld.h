//==============================================================================
// FILE:
//    HelloWorld.h
//
// DESCRIPTION:
//    Declares the HelloWorld pass for the new and the legacy pass managers.
//
// License: MIT
//==============================================================================

#ifndef LLVM_TUTOR_HELLO_WORLD_H
#define LLVM_TUTOR_HELLO_WORLD_H

#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"


//------------------------------------------------------------------------------
// New PM interface
//------------------------------------------------------------------------------

struct HelloWorld : llvm::PassInfoMixin<HelloWorld> {
  // This is one of the standard run() member functions expected by
  // PassInfoMixin. When the pass is executed by the new PM, this is the
  // function that will be called.
  llvm::PreservedAnalyses run(llvm::Function &Func,
                              llvm::FunctionAnalysisManager &FAM);
};


#endif // !LLVM_TUTOR_HELLO_WORLD_H
