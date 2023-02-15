#!/bin/bash

# $1 - sim/emu/hw
# $2 - source filename

set -e

SRC_FILE="$2"
SRC_FILE_BASENAME=`basename "$SRC_FILE"`
BENCHMARK="${SRC_FILE_BASENAME%.*}"
SRC_FILE_DIR=`dirname "$2"`

CANONICALIZED_SRC_FILE="$2".format.cpp
AST_TRANSFORMED_SRC_FILE="$2".tmp.cpp

BOTTLENECK_JSON_FILE="$SRC_FILE_DIR/bin/$BENCHMARK.prj/reports/lib/json/bottleneck.json" 
BOTTLENECK_LINES_FILE="$SRC_FILE_DIR/bin/$BENCHMARK.prj/bottleneck-lines.txt"
CDDD_REPORT_FILE="$SRC_FILE_DIR/cddd-report.json"

###
### STAGE 0: Ensure canonical kernel call. queue.single_task<> submission on one line, no empty lines.
###
~/git/llvm/build/bin/clang-format --style="{ColumnLimit: 2000, MaxEmptyLinesToKeep: 0}" \
                                  $SRC_FILE > $CANONICALIZED_SRC_FILE

###
### STAGE 1: Generate report.
###
# Use makefile during testing to not wait for report generation all the time
# cd $SRC_FILE_DIR && make report FILE=$SRC_FILE_BASENAME > /dev/null && cd - > /dev/null
dpcpp -std=c++17 -O2 -fintelfpga -c $SRC_FILE -Ilsq \
  -o $SRC_FILE_DIR/bin/$SRC_FILE_BASENAME.dev.o -DFPGA=1 > /dev/null 2>&1
dpcpp -std=c++17 -O2 -fintelfpga -fsycl-link -Ilsq \
  $SRC_FILE_DIR/bin/$SRC_FILE_BASENAME.dev.o -o \
  $SRC_FILE_DIR/bin/$SRC_FILE_BASENAME.a -Xshardware > /dev/null 2>&1

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
python3 scripts/ASTTransformCDDD.py $CDDD_REPORT_FILE $SRC_FILE $AST_TRANSFORMED_SRC_FILE
# Make the AST transformed code pretty
~/git/llvm/build/bin/clang-format $AST_TRANSFORMED_SRC_FILE > $AST_TRANSFORMED_SRC_FILE.tmp
mv $AST_TRANSFORMED_SRC_FILE.tmp $AST_TRANSFORMED_SRC_FILE

###
### STAGE 6: Swap the bottleneck instructions for pipe rd/write operations. 
###
