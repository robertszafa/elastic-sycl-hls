#!/bin/bash


 $LT_LLVM_INSTALL_DIR/build/bin/opt "$1" -o "$1" \
    -passes='mem2reg,lcssa,simplifycfg,loop-simplifycfg,instsimplify,aggressive-instcombine,dse,adce,dce,sroa,gvn,mergereturn'


# Get human readable bitcode
$LT_LLVM_INSTALL_DIR/build/bin/llvm-dis "$1" -o "$1".ll

# And demangle it.
$LT_LLVM_INSTALL_DIR/build/bin/llvm-cxxfilt < "$1".ll > "$1".demangled.ll

