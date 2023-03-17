#ifndef TABLE_OPERATION_LATENCY_H
#define TABLE_OPERATION_LATENCY_H

#include "llvm/ADT/MapVector.h"
#include "llvm/IR/Instruction.h"

namespace llvm {

/// A very crude estimation of operation latency on the Arria 10 FPGA.
const llvm::DenseMap<unsigned, int> LATENCY_TABLE_ARRIA10 = {
  // Floating point.
  {llvm::Instruction::FAdd, 3},
  {llvm::Instruction::FSub, 3},
  {llvm::Instruction::FMul, 3},
  {llvm::Instruction::FPTrunc, 3},
  {llvm::Instruction::FDiv, 14},
  {llvm::Instruction::FPToUI, 1},
  {llvm::Instruction::FPToSI, 1},
  {llvm::Instruction::UIToFP, 1},
  {llvm::Instruction::SIToFP, 1},
  {llvm::Instruction::FPTrunc, 1},
  {llvm::Instruction::FPExt, 1},

  // Integer.
  {llvm::Instruction::Mul, 1},
  {llvm::Instruction::Add, 1},
  {llvm::Instruction::Sub, 1},
  {llvm::Instruction::UDiv, 4},
  {llvm::Instruction::SDiv, 4},
  {llvm::Instruction::URem, 1},
  {llvm::Instruction::SRem, 1},
  {llvm::Instruction::FRem, 1},
  {llvm::Instruction::Trunc, 1},
  {llvm::Instruction::ZExt, 1},
  {llvm::Instruction::SExt, 1},

  // Logical (these should probably be <1)
  {llvm::Instruction::Shl, 1},
  {llvm::Instruction::LShr, 1},
  {llvm::Instruction::AShr, 1},
  {llvm::Instruction::And, 1},
  {llvm::Instruction::Or, 1},
  {llvm::Instruction::Xor, 1},
};

} // end namespace llvm

#endif