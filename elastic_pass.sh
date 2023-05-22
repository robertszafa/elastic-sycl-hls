#!/bin/bash

# $1 - sim/emu/hw
# $2 - filename to compile
# $3 - -d (Debug flag, optional. If passed, temporary files are preserved.)

set -e

echo "-------- Elastic passes start --------"

SRC_FILE="$2"
CANONICALIZED_SRC_FILE="$2".format.cpp
AST_TRANSFORMED_SRC_FILE="$2".tmp.cpp
SRC_FILE_BASENAME=`basename "$2"`
SRC_FILE_DIR=`dirname "$2"`
LOOP_REPORT_FILE="$SRC_FILE_DIR/elastic-pass-report.json"

mkdir -p "$SRC_FILE_DIR/bin"

FINAL_BINARY="$SRC_FILE_DIR/bin/${SRC_FILE_BASENAME%.*}.elastic.fpga_$1"

###
### STAGE 0: Ensure a canonical kernel call: queue.single_task<> submission on one line, no empty lines, no commnets.
###
gcc -fpreprocessed -dD -E $SRC_FILE > "$SRC_FILE"_no_comments
sed -i '1,1d' "$SRC_FILE"_no_comments # gcc adds a line --> remove it
$LT_LLVM_INSTALL_DIR/build/bin/clang-format --style="{ColumnLimit: 2000, MaxEmptyLinesToKeep: 0}" "$SRC_FILE"_no_comments > $CANONICALIZED_SRC_FILE
rm "$SRC_FILE"_no_comments

###
### STAGE 1: Get IR of original source and prepare it analysis.
###
./scripts/compilation/compile_to_bc.sh "$1" $CANONICALIZED_SRC_FILE
./scripts/compilation/prepare_ir.sh $CANONICALIZED_SRC_FILE.bc

###
### STAGE 2: Generate analysis json report.
###
export ELASTIC_PASS_REPORT=$LOOP_REPORT_FILE
$LT_LLVM_INSTALL_DIR/build/bin/opt -load-pass-plugin $ELASTIC_SYCL_HLS_DIR/build/lib/libElasticAnalysisPrinter.so \
                                   -passes=elastic-analysis $CANONICALIZED_SRC_FILE.bc -o /dev/null > $LOOP_REPORT_FILE

###
### STAGE 3: Generate kernel & pipe scaffolding code based on report. 
###
# Given json report, make kernel copies and pipe read/write calls from correct kernels.
# The json report provides a precise src location range for the kernel body.
# Nothing else is required (no variable/array names, etc.).
python3 scripts/compilation/ASTTransform.py $LOOP_REPORT_FILE $CANONICALIZED_SRC_FILE $AST_TRANSFORMED_SRC_FILE
# Make the AST transformed code pretty
$LT_LLVM_INSTALL_DIR/build/bin/clang-format $AST_TRANSFORMED_SRC_FILE > $AST_TRANSFORMED_SRC_FILE.tmp
mv $AST_TRANSFORMED_SRC_FILE.tmp $AST_TRANSFORMED_SRC_FILE

###
### STAGE 4: Fix IR inside kernels.
###
./scripts/compilation/compile_to_bc.sh "$1" $AST_TRANSFORMED_SRC_FILE
./scripts/compilation/prepare_ir.sh $AST_TRANSFORMED_SRC_FILE.bc
$LT_LLVM_INSTALL_DIR/build/bin/opt -load-pass-plugin $ELASTIC_SYCL_HLS_DIR/build/lib/libElasticTransform.so \
                                   -passes=elastic-transform $AST_TRANSFORMED_SRC_FILE.bc -o $AST_TRANSFORMED_SRC_FILE.out.bc
# Cleanup the transformed IR. The transformation leaves dead code, unused kernel args, etc.
./scripts/compilation/cleanup_ir.sh $AST_TRANSFORMED_SRC_FILE.out.bc

echo "Info: Passing transformed IR to downstream HLS compiler" 
echo "-------- Elastic passes end --------"
echo "Compiling $FINAL_BINARY"

###
### STAGE 5: Produce final binary.
###
./scripts/compilation/compile_from_bc.sh $1 $AST_TRANSFORMED_SRC_FILE.out.bc $AST_TRANSFORMED_SRC_FILE $FINAL_BINARY

# Remove created temporaried, if the "-d" flag was not supplied.
if [[ "$*" != *"-d"* ]] 
then
  rm -f $CANONICALIZED_SRC_FILE
  rm -f $CANONICALIZED_SRC_FILE.bc
  rm -f $AST_TRANSFORMED_SRC_FILE
  rm -f $AST_TRANSFORMED_SRC_FILE.bc
  rm -f $AST_TRANSFORMED_SRC_FILE.out.bc
  rm -f $LOOP_REPORT_FILE
fi

