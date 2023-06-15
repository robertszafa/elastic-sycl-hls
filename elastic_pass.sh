#!/bin/bash

# $1 - sim/emu/hw
# $2 - filename to compile
# $3 - -d (Debug flag, optional. If passed, temporary files are preserved.)
TARGET="$1"
SRC_FILE="$2"

set -e

# Get hold of names for tmp workdir, intermediate files, and final binary.
SRC_FILE_BASENAME_WITH_EXT=`basename "$SRC_FILE"`
SRC_FILE_BASENAME=${SRC_FILE_BASENAME_WITH_EXT%.*}
SRC_FILE_DIR=`dirname "$SRC_FILE"`
PASS_WORKDIR="$SRC_FILE_DIR/${SRC_FILE_BASENAME}_elastic_pass_workdir"
SRC_FILE_WORKDIR=$PASS_WORKDIR/$SRC_FILE_BASENAME_WITH_EXT
SRC_FILE_AST="$PASS_WORKDIR/$SRC_FILE_BASENAME.ast.cpp"
ANALYSIS_REPORT="$PASS_WORKDIR/elastic_pass_report.json"
FINAL_BINARY="$SRC_FILE_DIR/bin/${SRC_FILE_BASENAME}.elastic.fpga_$TARGET"

mkdir -p $PASS_WORKDIR
cp $SRC_FILE $PASS_WORKDIR
mkdir -p "$SRC_FILE_DIR/bin"


echo "-------- Elastic passes start --------"

###
### STAGE 0: Ensure a canonical kernel call: queue.single_task<> submission on one line, no empty lines, no commnets.
###
# SRC_FILE_NO_COMMENTS="$PASS_WORKDIR/${SRC_FILE_BASENAME}_no_comments.cpp"
gcc -fpreprocessed -dD -E $SRC_FILE_WORKDIR > $SRC_FILE_WORKDIR.tmp
sed -i '1,1d' $SRC_FILE_WORKDIR.tmp # gcc adds a line --> remove it
$LT_LLVM_INSTALL_DIR/build/bin/clang-format --style="{ColumnLimit: 2000, MaxEmptyLinesToKeep: 0}" $SRC_FILE_WORKDIR.tmp > $SRC_FILE_WORKDIR

###
### STAGE 1: Get IR of original source and prepare it analysis.
###
./scripts/compilation/compile_to_bc.sh $TARGET $SRC_FILE_WORKDIR
./scripts/compilation/prepare_ir.sh $SRC_FILE_WORKDIR.bc

###
### STAGE 2: Generate analysis json report.
###
export ELASTIC_PASS_REPORT=$ANALYSIS_REPORT
$LT_LLVM_INSTALL_DIR/build/bin/opt -load-pass-plugin $ELASTIC_SYCL_HLS_DIR/build/lib/libElasticAnalysisPrinter.so \
                                   -passes=elastic-analysis $SRC_FILE_WORKDIR.bc -o /dev/null > $ANALYSIS_REPORT

###
### STAGE 3: Generate kernel & pipe scaffolding code based on report. 
###
# Given json report, make kernel copies and pipe read/write calls from correct kernels.
# The json report provides a precise src location range for the kernel body.
# Nothing else is required (no variable/array names, etc.).
python3 scripts/compilation/ASTTransform.py $ANALYSIS_REPORT $SRC_FILE_WORKDIR $SRC_FILE_AST
# Make the AST transformed code pretty
$LT_LLVM_INSTALL_DIR/build/bin/clang-format $SRC_FILE_AST > $SRC_FILE_AST.tmp
mv $SRC_FILE_AST.tmp $SRC_FILE_AST

###
### STAGE 4: Fix IR inside kernels.
###
./scripts/compilation/compile_to_bc.sh $TARGET $SRC_FILE_AST
./scripts/compilation/prepare_ir.sh $SRC_FILE_AST.bc
$LT_LLVM_INSTALL_DIR/build/bin/opt -load-pass-plugin $ELASTIC_SYCL_HLS_DIR/build/lib/libElasticTransform.so \
                                   -passes=elastic-transform $SRC_FILE_AST.bc -o $SRC_FILE_AST.elastic.bc
# Cleanup the transformed IR. The transformation leaves dead code, unused kernel args, etc.
./scripts/compilation/cleanup_ir.sh $SRC_FILE_AST.elastic.bc

echo "Info: Passing transformed IR to downstream HLS compiler" 
echo "-------- Elastic passes end --------"
echo "Compiling $FINAL_BINARY"

###
### STAGE 5: Produce final binary.
###
./scripts/compilation/compile_from_bc.sh $TARGET $SRC_FILE_AST.elastic.bc $SRC_FILE_AST $FINAL_BINARY

# Remove created temporaried, if the "-d" flag was not supplied.
if [[ "$*" != *"-d"* ]] 
then
  rm -rf $PASS_WORKDIR
fi

