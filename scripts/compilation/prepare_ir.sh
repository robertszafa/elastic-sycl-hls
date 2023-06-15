#!/bin/bash

# Perform a number of transformations to for easier analysis.

#  Inline functions and optimize memcpys resulting from using structs.
 $LT_LLVM_INSTALL_DIR/build/bin/opt "$1" -o  "$1" -passes=always-inline 

# Our hoist-const-gep pass is similar to licm but we hoist GEPs with all constant 
# indices to the entry BB of the function. This means all SYCL global pointers 
# will be in the function entry block.
 $LT_LLVM_INSTALL_DIR/build/bin/opt "$1" -o "$1" \
                          --load-pass-plugin $ELASTIC_SYCL_HLS_DIR/build/lib/libHoistConstantGepTransform.so \
                          -passes=hoist-const-gep

# loop-simplify: Our analysis expects code in Loop Simplify Form. Each loop
#                has a preheader, header, latch, and exit block.
# mergereturn: We expect each function to have a single function exit block.
# sroa: Makes life simpler when dealing with stores to structs (e.g. LSQ requests).
# The rest will just remove unused ssa values.
$LT_LLVM_INSTALL_DIR/build/bin/opt "$1" -o "$1" \
  -passes='deadargelim-sycl,mem2reg,sroa,gvn,dce,mergereturn,loop-simplify'

# Get human readable bitcode
$LT_LLVM_INSTALL_DIR/build/bin/llvm-dis "$1" -o "$1".ll

# And demangle it.
$LT_LLVM_INSTALL_DIR/build/bin/llvm-cxxfilt < "$1".ll > "$1".demangled.ll
