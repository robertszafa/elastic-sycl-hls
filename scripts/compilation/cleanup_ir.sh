#!/bin/bash


 $LT_LLVM_INSTALL_DIR/build/bin/opt "$1" -o "$1" \
    -passes='deadargelim-sycl,mem2reg,gvn,dse,dce,adce,simplifycfg,loop-simplify,instsimplify,loop-deletion'
# simplifycfg to cleanup loops after loop-simplifycfg

# Get human readable bitcode
$LT_LLVM_INSTALL_DIR/build/bin/llvm-dis "$1" -o "$1".ll

# And demangle it.
$LT_LLVM_INSTALL_DIR/build/bin/llvm-cxxfilt < "$1".ll > "$1".demangled.ll

