#!/bin/bash

# $1 - sim/emu/hw
# $2 - source filename

set -e

SRC_FILE="$2"
SRC_FILE_BASENAME=`basename "$SRC_FILE"`
BENCHMARK="${SRC_FILE_BASENAME%.*}"
SRC_FILE_DIR=`dirname "$2"`

TMP_SRC_FILE="$2".tmp.cpp

BOTTLENECK_JSON_FILE="$SRC_FILE_DIR/bin/$BENCHMARK.prj/reports/lib/json/bottleneck.json" 
BOTTLENECK_LINES_FILE="$SRC_FILE_DIR/bin/$BENCHMARK.prj/bottleneck-lines.txt"
CDDD_REPORT_FILE="$SRC_FILE_DIR/cddd-report.json"

###
### STAGE 1: Generate report.
###
cd $SRC_FILE_DIR 
make report FILE=$SRC_FILE_BASENAME > /dev/null
cd - > /dev/null
# dpcpp -std=c++17 -O2 -fintelfpga -c $SRC_FILE \
#   -I/home/rob/git/sycl-playground/include \
#   -o $SRC_FILE_DIR/bin/$SRC_FILE_BASENAME.dev.o -DFPGA=1 > /dev/null
# dpcpp -std=c++17 -O2 -fintelfpga -fsycl-link \
#   -I/home/rob/git/sycl-playground/include \
#   $SRC_FILE_DIR/bin/$SRC_FILE_BASENAME.dev.o -o \
#   $SRC_FILE_DIR/bin/$SRC_FILE_BASENAME.a -Xshardware > /dev/null

###
### STAGE 3: Get line numbers of data dependency bottlenecks.
###
./scripts/GetBottleneckLines.py $BOTTLENECK_JSON_FILE > $BOTTLENECK_LINES_FILE
export BOTTLENECK_LINES_FILE=$BOTTLENECK_LINES_FILE

###
### STAGE 4: Generate control dependent data dependency analysis report.
###
# STAGE 4.1: Generate LLVM IR.
./scripts/compilation/compile_to_bc.sh "$1" $SRC_FILE
./scripts/compilation/prepare_ir.sh $SRC_FILE.bc
# STAGE 4.2: Run CDDD pass.
~/git/llvm/build/bin/opt -load-pass-plugin ~/git/llvm-sycl-passes/build/lib/libCDDDAnalysisPrinter.so \
                         -passes=cddd-bottlenecks $SRC_FILE.bc -o /dev/null > $CDDD_REPORT_FILE

###
### STAGE 5: Generate kernel & pipe scaffolding code based on report. 
###
python3 scripts/ASTTransformCDDD.py $CDDD_REPORT_FILE $SRC_FILE $TMP_SRC_FILE
