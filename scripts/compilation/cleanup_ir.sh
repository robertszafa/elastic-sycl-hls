#!/bin/bash

# Remove dead code after the transformation.

# simplifycfg: cleanup loops after loop-simplifycfg, i.e. remove basic blocks 
#              reuqired for Loop Simpligy Form if the are not used.
# mem2reg: turn alloca+stores+loads (mostly used for LSQ tags) into registers+phis. 
# the rest: remove dead code
$LT_LLVM_INSTALL_DIR/build/bin/opt "$1" -o "$1" \
    -passes='deadargelim-sycl,mem2reg,gvn,dse,dce,adce,simplifycfg,instsimplify,loop-deletion'

# Get human readable bitcode
$LT_LLVM_INSTALL_DIR/build/bin/llvm-dis "$1" -o "$1".ll

# And demangle it.
$LT_LLVM_INSTALL_DIR/build/bin/llvm-cxxfilt < "$1".ll > "$1".demangled.ll

