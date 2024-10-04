#!/bin/bash

# $1 - sim/emu/hw
# $2 - filename to compile
# Optional flags:
#   -f (Run dynamic loop fusion pass).
#   -d (Debug flag, optional. If passed, temporary files are preserved.)
TARGET="$1"
SRC_FILE="$2"
IS_FUSION=0
if [[ "$*" == *"-f"* ]]; then 
  IS_FUSION=1
fi

set -e

# Get hold of names for binaries and tmp files.
TMP_DIR=${TMPDIR:-${TEMP:-${TMP:-/tmp}}}
LLVM_BIN_DIR=$ELASTIC_SYCL_HLS_DIR/llvm/build/bin
SRC_FILE_BASENAME_WITH_EXT=`basename "$SRC_FILE"`
SRC_FILE_BASENAME=${SRC_FILE_BASENAME_WITH_EXT%.*}
SRC_FILE_DIR=`dirname "$SRC_FILE"`
HASHED_FILENAME=${SRC_FILE_BASENAME}_$RANDOM
PASS_WORKDIR="$TMP_DIR/${HASHED_FILENAME}_elastic_pass_workdir"
SRC_FILE_WORKDIR=$PASS_WORKDIR/$SRC_FILE_BASENAME_WITH_EXT
SRC_FILE_AST="$PASS_WORKDIR/$SRC_FILE_BASENAME.ast.cpp"
ANALYSIS_REPORT="$PASS_WORKDIR/elastic_pass_report.json"
FINAL_BINARY="$SRC_FILE_DIR/bin/${SRC_FILE_BASENAME}.elastic.fpga_$TARGET"

ANALYSIS_PASS="elastic-analysis"
ANALYSIS_PASS_SO="$ELASTIC_SYCL_HLS_DIR/build/lib/libElasticAnalysisPrinter.so"
TRANSFORM_PASS="elastic-transform"
TRANSFORM_PASS_SO="$ELASTIC_SYCL_HLS_DIR/build/lib/libElasticTransform.so"
if [ $IS_FUSION == 1 ]; then
  ANALYSIS_PASS="dynamic-loop-fusion-analysis"
  ANALYSIS_PASS_SO="$ELASTIC_SYCL_HLS_DIR/build/lib/libDynamicLoopFusionAnalysisPrinter.so"
  TRANSFORM_PASS="dynamic-loop-fusion-transform"
  TRANSFORM_PASS_SO="$ELASTIC_SYCL_HLS_DIR/build/lib/libDynamicLoopFusionTransform.so"
fi

# Run the following passes to ensure canonical SSA form for analysis:
# hoist-const-gep: Ensure SYCL global pointers are declared in function entry block.
# loop-simplify: Our analysis expects code in Loop Simplify Form. 
# mergereturn: We expect each function to have a single function exit block.
# sroa: Makes life simpler when dealing with stores to structs (e.g. LSQ requests).
# adce: remove code produced by our AST transformation but not used anywhere (e.g. results of pipe reads)
prepare_ir() {
  $LLVM_BIN_DIR/opt $1 -o $1 -passes="always-inline,mem2reg,mldst-motion"
  $LLVM_BIN_DIR/opt $1 -o $1 --load-pass-plugin $ELASTIC_SYCL_HLS_DIR/build/lib/libHoistConstantGepTransform.so \
    -passes=hoist-const-gep
  $LLVM_BIN_DIR/opt $1 -o $1 -passes='sroa,adce,loop-simplify,mergereturn,lowerswitch'
  # Save human readable bitcode
  $LLVM_BIN_DIR/llvm-dis $1 -o $1.ll && $LLVM_BIN_DIR/llvm-cxxfilt < $1.ll > $1.demangled.ll
}

# Run the following passes to delete dead code:
cleanup_ir() {
  # A pipe op can have only one read/write end point. If a kernel gets decoupled 
  # and both copies have the same pipe op, then we need to create a new relay pipe / delete some pipe ops.
  # $LLVM_BIN_DIR/opt $1 -o $1 --load-pass-plugin $ELASTIC_SYCL_HLS_DIR/build/lib/libPipeDeduplicationPass.so \
  #   -passes=pipe-deduplication
  # Remove dead code
  $LLVM_BIN_DIR/opt $1 -o $1 -passes='adce,loop-simplify,simplifycfg' # ,simplifycfg
  # $LLVM_BIN_DIR/opt $1 -o $1 -passes='deadargelim-sycl,strip-debug-declare' 
  # Save human readable bitcode
  $LLVM_BIN_DIR/llvm-dis $1 -o $1.ll && $LLVM_BIN_DIR/llvm-cxxfilt < $1.ll > $1.demangled.ll
}

# During compilation, change the pipe and bram verilog IPs to our amended 
# versions located in {ELASTIC_SYCL_HLS_DIR}/ip.
change_ip() {
  if [ $IS_FUSION == 1 ]; then
    # No need to change IPs if we only do dynamic loop fusion.
    return
  fi

  if ! test -d $ELASTIC_SYCL_HLS_DIR/ip; then
    echo "No amended IPs in {ELASTIC_SYCL_HLS_DIR}/ip (see README to generate). Will use default IPs."
    return
  fi

  # Wait for IP files to be coppied by the driver into TMP_DIR.
  while [ ! `find $TMP_DIR/$HASHED_FILENAME* -nowarn -name "acl_channel_fifo.v" 2>/dev/null` ]; do :; done
  # Get exact dirname of IP files.
  IP_DIR=$(dirname $TMP_DIR/$HASHED_FILENAME-*/ip/acl_channel_fifo.v)

  # Copy the changed IPs into IP_DIR every time the compiler changes them.
  while [ ! -f $FINAL_BINARY ]; do # stop when full compile done
    # Use grep to check if our changes are still there.
    if grep -q -s '(ALLOW_HIGH_SPEED_FIFO_USAGE ? "hs" : "ms")' $IP_DIR/acl_channel_fifo.v ; then
      cp $ELASTIC_SYCL_HLS_DIR/ip/acl_channel_fifo.v $IP_DIR
    fi
    if grep -q -s '= RDW_MODE' $IP_DIR/acl_mem1x.v ; then
      cp $ELASTIC_SYCL_HLS_DIR/ip/acl_mem1x.v $IP_DIR
    fi
  done
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
$ELASTIC_SYCL_HLS_DIR/scripts/compilation/compile_to_bc.sh $TARGET $SRC_FILE_WORKDIR $HASHED_FILENAME
prepare_ir $SRC_FILE_WORKDIR.bc

###
### STAGE 2: Generate analysis json report.
###
echo "Info: Generating analysis report: $ANALYSIS_REPORT" 
export ELASTIC_PASS_REPORT=$ANALYSIS_REPORT
$LLVM_BIN_DIR/opt -load-pass-plugin $ANALYSIS_PASS_SO -passes=$ANALYSIS_PASS \
                  $SRC_FILE_WORKDIR.bc -o /dev/null > $ANALYSIS_REPORT
                  # $SRC_FILE_WORKDIR.bc -o /dev/null 

###
### STAGE 3: Generate kernel & pipe scaffolding code based on report. 
###
# Given json report, make kernel copies and pipe read/write calls from correct kernels.
# The json report provides a precise src location range for the kernel body.
# Nothing else is required (no variable/array names, etc.).
python3 $ELASTIC_SYCL_HLS_DIR/scripts/compilation/ASTTransform.py $ANALYSIS_REPORT $SRC_FILE_WORKDIR $SRC_FILE_AST $IS_FUSION

# ###
# ### STAGE 4: Fix IR inside kernels.
# ###
echo "Info: Swapping instructions for pipe calls" 
$ELASTIC_SYCL_HLS_DIR/scripts/compilation/compile_to_bc.sh $TARGET $SRC_FILE_AST $HASHED_FILENAME
prepare_ir $SRC_FILE_AST.bc
$LLVM_BIN_DIR/opt -load-pass-plugin $TRANSFORM_PASS_SO -passes=$TRANSFORM_PASS \
                                    $SRC_FILE_AST.bc -o $SRC_FILE_AST.elastic.bc
echo "Info: Removing dead code" 
cleanup_ir $SRC_FILE_AST.elastic.bc

###
### STAGE 5: Produce final binary.
###
echo "Info: Passing transformed IR to downstream HLS compiler" 
echo "---------------- Elastic passes end ----------------"
echo "Compiling $FINAL_BINARY"
# change_ip & \
$ELASTIC_SYCL_HLS_DIR/scripts/compilation/compile_from_bc.sh $TARGET $SRC_FILE_AST.elastic.bc $SRC_FILE_AST $FINAL_BINARY $HASHED_FILENAME

# Remove created temporaries, if the "-d" flag was not supplied.
if [[ "$*" != *"-d"* ]]; then 
  rm -rf $TMP_DIR/${HASHED_FILENAME}* $TMP_DIR/**/${HASHED_FILENAME}* 
else
  echo "Demnagled original IR: $SRC_FILE_WORKDIR.bc.demangled.ll"
  echo "Demnagled ast IR: $SRC_FILE_AST.bc.demangled.ll"
  echo "Demnagled transformed IR: $SRC_FILE_AST.elastic.bc.demangled.ll"
fi
