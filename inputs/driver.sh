#!/bin/bash

set -e

SRC_FILE="$2"
TMP_SRC_FILE="$2".tmp.cpp
SRC_FILE_BASENAME=`basename "$2"`
SRC_FILE_DIR=`dirname "$2"`
LOOP_REPORT_FILE=$SRC_FILE_DIR/loop-raw-report.json
# Demangled (human readable) IR files are generated at each step.

mkdir -p "$SRC_FILE_DIR/bin"

if [ "$1" == "sim" ]; then
  TO_BC=./scripts/compile_to_bc_sim.sh
  FROM_BC=./scripts/compile_from_bc_sim.sh
  FINAL_BINARY="$SRC_FILE_DIR/bin/$SRC_FILE_BASENAME.fpga_sim"
else
  TO_BC=./scripts/compile_to_bc.sh
  FROM_BC=./scripts/compile_from_bc.sh
  FINAL_BINARY="$SRC_FILE_DIR/bin/$SRC_FILE_BASENAME.fpga_emu"
fi


# Get IR of original source. Prepare IR for mem dep analysis.
$TO_BC $SRC_FILE

# Generate analysis json report.
~/git/llvm/build/bin/opt -load-pass-plugin ~/git/llvm-sycl-passes/build/lib/libLoopRAWHazardReport.so \
                         -passes=loop-raw-report $SRC_FILE.ll -o $SRC_FILE.ll > $LOOP_REPORT_FILE

# Given json report, make kernel copies and pipe read/write calls from correct kernels.
# Output from this source-to-source transformation will be in $SRC_FILE.tmp.cpp
python3 scripts/genKernelsAndPipes.py $LOOP_REPORT_FILE $SRC_FILE

# Get IR of source with kernels and pipes instantiated. 
$TO_BC $TMP_SRC_FILE

# Transform each kernel to do only one correct work and write/read correct pipes.
echo "-- Running libStoreQueueTransform on refactored source"
~/git/llvm/build/bin/opt -load-pass-plugin ~/git/llvm-sycl-passes/build/lib/libStoreQueueTransform.so \
                         -passes=stq-transform $TMP_SRC_FILE.bc -o $TMP_SRC_FILE.bc.out

# Cleanup the transformed IR. The transformation leaves a lot of dead code, unused kernel args, etc.
~/git/llvm/build/bin/opt --mem2reg \
                         --deadargelim-sycl \
                         --simplifycfg \
                         --loop-simplifycfg \
                         --loop-rotate \
                         --lcssa \
                         --instcombine \
                         --instsimplify \
                         --aggressive-instcombine \
                         --interleaved-access \
                         --dse \
                         --adce \
                         --sroa \
                         --gvn \
                         $TMP_SRC_FILE.bc.out -o $TMP_SRC_FILE.bc.out

~/git/llvm/build/bin/llvm-dis $TMP_SRC_FILE.bc.out -o $TMP_SRC_FILE.out.ll
~/git/llvm/build/bin/llvm-cxxfilt < $TMP_SRC_FILE.out.ll > $TMP_SRC_FILE.demangled.out.ll

# Produce final binary.
echo "-- Compiling into $FINAL_BINARY"
$FROM_BC $TMP_SRC_FILE.bc.out $TMP_SRC_FILE $FINAL_BINARY

echo "done"
