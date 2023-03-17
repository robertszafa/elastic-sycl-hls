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

FINAL_BINARY="$SRC_FILE_DIR/bin/${SRC_FILE_BASENAME%.*}.cddd.fpga_$1"

BOTTLENECK_JSON_FILE="$SRC_FILE_DIR/bin/$BENCHMARK.cpp.format.prj/reports/lib/json/bottleneck.json" 
BOTTLENECK_LINES_FILE="$SRC_FILE_DIR/bin/$BENCHMARK.cpp.format.prj/bottleneck-lines.txt"
CDDD_REPORT_FILE="$SRC_FILE_DIR/cddd-report.json"

###
### STAGE 0: Ensure canonical kernel call. queue.single_task<> submission on one line, no empty lines.
###
# Don't recreating the file if exists so at to not trigger the make rule every time.
$LT_LLVM_INSTALL_DIR/build/bin/clang-format --style="{ColumnLimit: 2000, MaxEmptyLinesToKeep: 0}" \
                                  $SRC_FILE > $CANONICALIZED_SRC_FILE

###
### STAGE 1: Generate control dependent data dependency analysis report.
###
# STAGE 1.1: Generate LLVM IR.
./scripts/compilation/compile_to_bc.sh "$1" $CANONICALIZED_SRC_FILE
./scripts/compilation/prepare_ir.sh $CANONICALIZED_SRC_FILE.bc
# STAGE 1.2: Run CDDD analysis pass to generate report.
$LT_LLVM_INSTALL_DIR/build/bin/opt -load-pass-plugin $LLVM_SYCL_PASSES_DIR/build/lib/libCDDDAnalysisPrinter.so \
                                   -passes=cddd-analysis $CANONICALIZED_SRC_FILE.bc -o /dev/null > $CDDD_REPORT_FILE
export CDDD_REPORT_FILE=$CDDD_REPORT_FILE
if [[ "$*" == *"-d"* ]] 
then
  printf "\n##### Analysis Report: #####\n"
  cat $CDDD_REPORT_FILE
  printf "\n### End Analysis Report: ###\n\n"
fi

###
### STAGE 2: Generate kernel & pipe scaffolding code based on report. 
###
python3 scripts/ast/ASTTransformCDDD.py $CDDD_REPORT_FILE $CANONICALIZED_SRC_FILE $AST_TRANSFORMED_SRC_FILE
# Make the AST transformed code pretty
$LT_LLVM_INSTALL_DIR/build/bin/clang-format $AST_TRANSFORMED_SRC_FILE > $AST_TRANSFORMED_SRC_FILE.tmp
mv $AST_TRANSFORMED_SRC_FILE.tmp $AST_TRANSFORMED_SRC_FILE

###
### STAGE 3: Swap the bottleneck instructions for pipe rd/write operations. 
###
./scripts/compilation/compile_to_bc.sh "$1" $AST_TRANSFORMED_SRC_FILE
./scripts/compilation/prepare_ir.sh $AST_TRANSFORMED_SRC_FILE.bc
$LT_LLVM_INSTALL_DIR/build/bin/opt -load-pass-plugin $LLVM_SYCL_PASSES_DIR/build/lib/libCDDDTransform.so \
                                   -passes=cddd-transform $AST_TRANSFORMED_SRC_FILE.bc -o $AST_TRANSFORMED_SRC_FILE.out.bc

###
### STAGE 4: Produce final binary.
###
echo ">> Compiling $FINAL_BINARY"
mkdir -p $SRC_FILE_DIR/bin
# Cleanup the transformed IR. The transformation leaves a lot of dead code, unused kernel args, etc.
./scripts/compilation/cleanup_ir.sh $AST_TRANSFORMED_SRC_FILE.out.bc
./scripts/compilation/compile_from_bc.sh $1 $AST_TRANSFORMED_SRC_FILE.out.bc $AST_TRANSFORMED_SRC_FILE $FINAL_BINARY


# Remove created temporaried, if the "-d" flag was not supplied.
if [[ "$*" != *"-d"* ]] 
then
  rm $CANONICALIZED_SRC_FILE*
  rm $AST_TRANSFORMED_SRC_FILE*
fi


