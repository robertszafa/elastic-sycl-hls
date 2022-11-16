#!/bin/bash

# Perform a number of transformations to easy analysis.
 ~/git/llvm/build/bin/opt --mem2reg \
                          --licm \
                          --lcssa \
                          --deadargelim-sycl \
                          --simplifycfg \
                          --loop-simplifycfg \
                          --instcombine \
                          --instsimplify \
                          --aggressive-instcombine \
                          --interleaved-access \
                          --dse \
                          --adce \
                          --dce \
                          --sroa \
                          --gvn \
                          --mergereturn \
                          "$1" -o "$1"
                          # -polly-canonicalize -polly-simplify -polly-optree -polly-delicm -polly-simplify -polly-process-unprofitable -polly-opt-isl -polly-scops \

# Get human readable bitcode
~/git/llvm/build/bin/llvm-dis "$1" -o "$1".ll

# And demangle it.
~/git/llvm/build/bin/llvm-cxxfilt < "$1".ll > "$1".demangled.ll

