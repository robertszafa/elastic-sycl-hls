#!/bin/bash

# $1 - sim/emu/hw
# $2 - filename to compile
# $3 - Q_SIZE

set -e

SRC_FILE="$2"
Q_SIZE="$3"

TMP_SRC_FILE="$2".tmp.cpp
SRC_FILE_BASENAME=`basename "$2"`
SRC_FILE_DIR=`dirname "$2"`
LOOP_REPORT_FILE="$SRC_FILE_DIR/loop-raw-report.json"
# Demangled (human readable) IR files are generated at each step.

mkdir -p "$SRC_FILE_DIR/bin"

FINAL_BINARY="$SRC_FILE_DIR/bin/$SRC_FILE_BASENAME.fpga_$1"

###
### STAGE 1: Get IR of original source and prepare it for mem dep analysis.
###
./scripts/compile_to_bc.sh "$1" $SRC_FILE
./scripts/prepare_ir.sh $SRC_FILE.bc

###
### STAGE 2: Generate analysis json report.
###
export LOOP_RAW_REPORT=$LOOP_REPORT_FILE
~/git/llvm/build/bin/opt -load-pass-plugin ~/git/llvm-sycl-passes/build/lib/libLoopRAWHazardReport.so \
                         -passes=loop-raw-report $SRC_FILE.bc -o /dev/null > $LOOP_REPORT_FILE

###
### STAGE 3: Generate kernel & pipe scaffolding code based on report. 
###
# Given json report, make kernel copies and pipe read/write calls from correct kernels.
# Output from this source-to-source transformation will be in $SRC_FILE.tmp.cpp
# TODO: use clang AST Transormer
python3 scripts/genKernelsAndPipes.py $LOOP_REPORT_FILE $SRC_FILE $Q_SIZE

###
### STAGE 4: Fix IR inside kernels.
###
echo "-- Running libStoreQueueTransform on refactored source"
# Get IR of source with kernels and pipes instantiated. 
./scripts/compile_to_bc.sh "$1" $TMP_SRC_FILE
./scripts/prepare_ir.sh $TMP_SRC_FILE.bc
~/git/llvm/build/bin/opt -load-pass-plugin ~/git/llvm-sycl-passes/build/lib/libStoreQueueTransform.so \
                         -passes=stq-transform $TMP_SRC_FILE.bc -o $TMP_SRC_FILE.out.bc

###
### STAGE 5: Produce final binary.
###
echo "-- Compiling into $FINAL_BINARY"
# Cleanup the transformed IR. The transformation leaves a lot of dead code, unused kernel args, etc.
./scripts/prepare_ir.sh $TMP_SRC_FILE.out.bc
./scripts/compile_from_bc.sh $1 $TMP_SRC_FILE.out.bc $TMP_SRC_FILE $FINAL_BINARY


echo "done"
