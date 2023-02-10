#!/bin/bash

# $1 - sim/emu/hw
# $2 - filename to compile
# $3 - Q_SIZE (optional, default 8)
# $4 - -d (Debug flag, optional. If passed, temporary files are preserved.)

set -e

SRC_FILE="$2"
Q_SIZE=${3:-8}
if [[ "$3" == *"-d"* ]] # Case with a debug flag but no Q_SIZE specified.
then
  Q_SIZE=8
fi

CANONICALIZED_SRC_FILE="$2".format.cpp
AST_TRANSFORMED_SRC_FILE="$2".tmp.cpp
SRC_FILE_BASENAME=`basename "$2"`
SRC_FILE_DIR=`dirname "$2"`
LOOP_REPORT_FILE="$SRC_FILE_DIR/loop-raw-report.json"

mkdir -p "$SRC_FILE_DIR/bin"

FINAL_BINARY="$SRC_FILE_DIR/bin/$SRC_FILE_BASENAME.fpga_$1"

###
### STAGE 0: Ensure a canonical kernel call: queue.single_task<> submission on one line, no empty lines.
###
~/git/llvm/build/bin/clang-format --style="{ColumnLimit: 2000, MaxEmptyLinesToKeep: 0}" $SRC_FILE > $CANONICALIZED_SRC_FILE

###
### STAGE 1: Get IR of original source and prepare it for mem dep analysis.
###
./scripts/compilation/compile_to_bc.sh "$1" $CANONICALIZED_SRC_FILE
./scripts/compilation/prepare_ir.sh $CANONICALIZED_SRC_FILE.bc

###
### STAGE 2: Generate analysis json report.
###
export LOOP_RAW_REPORT=$LOOP_REPORT_FILE
~/git/llvm/build/bin/opt -load-pass-plugin ~/git/llvm-sycl-passes/build/lib/libDataHazardAnalysisPrinter.so \
                         -passes=data-hazard-report $CANONICALIZED_SRC_FILE.bc -o /dev/null > $LOOP_REPORT_FILE

###
### STAGE 3: Generate kernel & pipe scaffolding code based on report. 
###
# Given json report, make kernel copies and pipe read/write calls from correct kernels.
# The json report provides a precise src location range for the kernel body.
# Nothing else is required (no variable/array names, etc.).
python3 scripts/ASTTransformLSQ.py $LOOP_REPORT_FILE $CANONICALIZED_SRC_FILE $AST_TRANSFORMED_SRC_FILE $Q_SIZE
# Make the AST transformed code pretty
~/git/llvm/build/bin/clang-format $AST_TRANSFORMED_SRC_FILE > $AST_TRANSFORMED_SRC_FILE.tmp
mv $AST_TRANSFORMED_SRC_FILE.tmp $AST_TRANSFORMED_SRC_FILE

###
### STAGE 4: Fix IR inside kernels.
###
echo ">> Running lsq-transform"
# Get IR of source with kernels and pipes instantiated. 
./scripts/compilation/compile_to_bc.sh "$1" $AST_TRANSFORMED_SRC_FILE
./scripts/compilation/prepare_ir.sh $AST_TRANSFORMED_SRC_FILE.bc
~/git/llvm/build/bin/opt -load-pass-plugin ~/git/llvm-sycl-passes/build/lib/libLoadStoreQueueTransform.so \
                         -passes=lsq-transform $AST_TRANSFORMED_SRC_FILE.bc -o $AST_TRANSFORMED_SRC_FILE.out.bc

###
### STAGE 5: Produce final binary.
###
echo ">> Compiling $FINAL_BINARY"
# Cleanup the transformed IR. The transformation leaves a lot of dead code, unused kernel args, etc.
./scripts/compilation/cleanup_ir.sh $AST_TRANSFORMED_SRC_FILE.out.bc
./scripts/compilation/compile_from_bc.sh $1 $AST_TRANSFORMED_SRC_FILE.out.bc $AST_TRANSFORMED_SRC_FILE $FINAL_BINARY

# Remove created temporaried, if the "-d" flag was not supplied.
if [[ "$*" != *"-d"* ]] 
then
  rm $CANONICALIZED_SRC_FILE
  rm $CANONICALIZED_SRC_FILE.bc
  rm $AST_TRANSFORMED_SRC_FILE
  rm $AST_TRANSFORMED_SRC_FILE.bc
  rm $AST_TRANSFORMED_SRC_FILE.out.bc
  rm $LOOP_REPORT_FILE
fi


echo "done"
