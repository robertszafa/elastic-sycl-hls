#!/bin/bash

# $1 - sim/emu/hw
# $2 - filename to compile
# $3 - -d (Debug flag, optional. If passed, temporary files are preserved.)
TARGET="$1"
SRC_FILE="$2"

set -e

# Get hold of names for binaries and tmp files.
LLVM_BIN_DIR=$ELASTIC_SYCL_HLS_DIR/llvm/build/bin
SRC_FILE_BASENAME_WITH_EXT=`basename "$SRC_FILE"`
SRC_FILE_BASENAME=${SRC_FILE_BASENAME_WITH_EXT%.*}
SRC_FILE_DIR=`dirname "$SRC_FILE"`
PASS_WORKDIR="$SRC_FILE_DIR/${SRC_FILE_BASENAME}_elastic_pass_workdir"
SRC_FILE_WORKDIR=$PASS_WORKDIR/$SRC_FILE_BASENAME_WITH_EXT
SRC_FILE_AST="$PASS_WORKDIR/$SRC_FILE_BASENAME.ast.cpp"
ANALYSIS_REPORT="$PASS_WORKDIR/elastic_pass_report.json"
FINAL_BINARY="$SRC_FILE_DIR/bin/${SRC_FILE_BASENAME}.elastic.fpga_$TARGET"

# Run the following passes to ensure canonical SSA form for analysis:
# hoist-const-gep: Ensure SYCL global pointers are declared in function entry block.
# loop-simplify: Our analysis expects code in Loop Simplify Form. 
# mergereturn: We expect each function to have a single function exit block.
# sroa: Makes life simpler when dealing with stores to structs (e.g. LSQ requests).
# adce: remove code produced by our AST transformation but not used anywhere (e.g. results of pipe reads)
prepare_ir() {
  $LLVM_BIN_DIR/opt $1 -o $1 -passes=always-inline 
  $LLVM_BIN_DIR/opt $1 -o $1 --load-pass-plugin $ELASTIC_SYCL_HLS_DIR/build/lib/libHoistConstantGepTransform.so \
    -passes=hoist-const-gep
  $LLVM_BIN_DIR/opt $1 -o $1 -passes='adce,loop-simplify,mergereturn,sroa'
  # Save human readable bitcode
  $LLVM_BIN_DIR/llvm-dis $1 -o $1.ll && $LLVM_BIN_DIR/llvm-cxxfilt < $1.ll > $1.demangled.ll
}

# Run the following passes to delete dead code:
# deadargelim-sycl: remove kernel arguments if they are not used.
# simplifycfg: remove unused loop blocks generated by loop-simplifycfg.
# the rest: remove dead code
cleanup_ir() {
  $LLVM_BIN_DIR/opt $1 -o $1 -passes='adce,simplifycfg' 
  $LLVM_BIN_DIR/opt $1 -o $1 -passes='deadargelim-sycl,strip-debug-declare' 
  # Save human readable bitcode
  $LLVM_BIN_DIR/llvm-dis $1 -o $1.ll && $LLVM_BIN_DIR/llvm-cxxfilt < $1.ll > $1.demangled.ll
}


echo "---------------- Elastic passes start ----------------"
mkdir -p $PASS_WORKDIR
cp $SRC_FILE $PASS_WORKDIR
mkdir -p "$SRC_FILE_DIR/bin"

###
### STAGE 0: Ensure a canonical kernel call: queue.single_task<> submission on one line, no empty lines, no commnets.
###
gcc -fpreprocessed -dD -E $SRC_FILE_WORKDIR > $SRC_FILE_WORKDIR.tmp
sed -i '1,1d' $SRC_FILE_WORKDIR.tmp # gcc adds a line --> remove it
$LLVM_BIN_DIR/clang-format --style="{ColumnLimit: 2000, MaxEmptyLinesToKeep: 0}" $SRC_FILE_WORKDIR.tmp > $SRC_FILE_WORKDIR

###
### STAGE 1: Get IR of original source and prepare it analysis.
###
$ELASTIC_SYCL_HLS_DIR/scripts/compilation/compile_to_bc.sh $TARGET $SRC_FILE_WORKDIR
prepare_ir $SRC_FILE_WORKDIR.bc

###
### STAGE 2: Generate analysis json report.
###
echo "Info: Generating analysis report: $ANALYSIS_REPORT" 
export ELASTIC_PASS_REPORT=$ANALYSIS_REPORT
$LLVM_BIN_DIR/opt -load-pass-plugin $ELASTIC_SYCL_HLS_DIR/build/lib/libElasticAnalysisPrinter.so \
                                   -passes=elastic-analysis $SRC_FILE_WORKDIR.bc -o /dev/null > $ANALYSIS_REPORT

###
### STAGE 3: Generate kernel & pipe scaffolding code based on report. 
###
# Given json report, make kernel copies and pipe read/write calls from correct kernels.
# The json report provides a precise src location range for the kernel body.
# Nothing else is required (no variable/array names, etc.).
python3 $ELASTIC_SYCL_HLS_DIR/scripts/compilation/ASTTransform.py $ANALYSIS_REPORT $SRC_FILE_WORKDIR $SRC_FILE_AST
# Make the AST transformed code pretty
$LLVM_BIN_DIR/clang-format $SRC_FILE_AST > $SRC_FILE_AST.tmp && mv $SRC_FILE_AST.tmp $SRC_FILE_AST

###
### STAGE 4: Fix IR inside kernels.
###
echo "Info: Swapping instructions for pipe calls" 
$ELASTIC_SYCL_HLS_DIR/scripts/compilation/compile_to_bc.sh $TARGET $SRC_FILE_AST
prepare_ir $SRC_FILE_AST.bc
$LLVM_BIN_DIR/opt -load-pass-plugin $ELASTIC_SYCL_HLS_DIR/build/lib/libElasticTransform.so \
                                   -passes=elastic-transform $SRC_FILE_AST.bc -o $SRC_FILE_AST.elastic.bc
echo "Info: Removing dead code" 
cleanup_ir $SRC_FILE_AST.elastic.bc

###
### STAGE 5: Produce final binary.
###
echo "Info: Passing transformed IR to downstream HLS compiler" 
echo "---------------- Elastic passes end ----------------"
echo "Compiling $FINAL_BINARY"
$ELASTIC_SYCL_HLS_DIR/scripts/compilation/compile_from_bc.sh $TARGET $SRC_FILE_AST.elastic.bc $SRC_FILE_AST $FINAL_BINARY

# Remove created temporaried, if the "-d" flag was not supplied.
if [[ "$*" != *"-d"* ]] 
then
  rm -rf $PASS_WORKDIR
fi
