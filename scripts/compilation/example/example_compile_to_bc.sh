#!/bin/bash

# $1 - sim/emu/hw
# $2 - filename to compile


BASE_FNAME=$(basename ${2})
OUTPUT_FILE="$2.bc"
INT_HEADER=$HOME/tmp/$BASE_FNAME-integration-header.h
INT_FOOTER=$HOME/tmp/$BASE_FNAME-integration-footer.h

INCLUDE_FILES=$HOME/git/elastic-sycl-hls/lsq

GCC_V=`ls /usr/lib/gcc/x86_64-linux-gnu/ | sort -r | head -n1`
DPCPP_PATH=`which dpcpp`
ONE_API_DIR=${DPCPP_PATH//bin\/dpcpp/}
CLANG_V=`ls $ONE_API_DIR/lib/clang | sort -r | head -n1`

# These flags can be recovered by running the dpcpp compiler driver with the -v --verbose flags
CC_FLAGS="-cc1 -triple spir64_fpga-unknown-unknown -aux-triple x86_64-unknown-linux-gnu 
          -fsycl-is-device -fdeclare-spirv-builtins -fno-sycl-early-optimizations -fenable-sycl-dae
          -Wno-sycl-strict -fsycl-int-header=$INT_HEADER -fsycl-int-footer=$INT_FOOTER -sycl-std=2020
          -fsycl-unnamed-lambda -fsycl-unique-prefix=0251c73b0e837ff4 -fsycl-disable-range-rounding
          -Wspir-compat -emit-llvm-bc -emit-llvm-uselists -disable-free -clear-ast-before-backend
          -disable-llvm-verifier -discard-value-names -mrelocation-model static
          -fveclib=SVML -mframe-pointer=all -menable-no-infs -menable-no-nans -menable-unsafe-fp-math
          -fno-signed-zeros -mreassociate -freciprocal-math -fdenormal-fp-math=preserve-sign,preserve-sign
          -ffp-contract=fast -fno-rounding-math -ffast-math -ffinite-math-only -fno-verbose-asm
          -mconstructor-aliases -aux-target-cpu x86-64 -debug-info-kind=limited -dwarf-version=4
          -debugger-tuning=gdb -resource-dir $ONE_API_DIR/lib/clang/$CLANG_V
          -internal-isystem $ONE_API_DIR/lib/oclfpga/include
          -internal-isystem $ONE_API_DIR/bin-llvm/../include/sycl
          -internal-isystem $ONE_API_DIR/bin-llvm/../include -I $INCLUDE_FILES
          -internal-isystem $ONE_API_DIR/bin-llvm/../compiler/include
          -internal-isystem $ONE_API_DIR/lib/clang/$CLANG_V/include
          -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/$GCC_V/../../../../include/c++/$GCC_V
          -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/$GCC_V/../../../../include/x86_64-linux-gnu/c++/$GCC_V
          -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/$GCC_V/../../../../include/c++/$GCC_V/backward
          -internal-isystem /usr/local/include
          -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/$GCC_V/../../../../x86_64-linux-gnu/include
          -internal-externc-isystem /usr/include/x86_64-linux-gnu
          -internal-externc-isystem /include -internal-externc-isystem /usr/include
          -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include
          -x c++ -O2 -std=c++17 -fdeprecated-macro -ferror-limit 19 -fheinous-gnu-extensions -fgnuc-version=4.2.1
          -fcxx-exceptions -fexceptions -fcolor-diagnostics -vectorize-loops -D__GCC_HAVE_DWARF2_CFI_ASM=1
          -mllvm -disable-hir-generate-mkl-call -mllvm -intel-libirc-allowed -mllvm -loopopt=0 -floopopt-pipeline=none"


if [ "$1" == "sim" ] || [ "$1" == "hw" ]; then
  "$ONE_API_DIR/bin-llvm/clang++" $CC_FLAGS -o $OUTPUT_FILE "$2" -D FPGA=1 
elif  [ "$1" == "emu" ]; then
  "$ONE_API_DIR/bin-llvm/clang++" $CC_FLAGS -o $OUTPUT_FILE "$2" -D FPGA_EMULATOR=1 
fi
