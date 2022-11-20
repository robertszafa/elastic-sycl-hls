#!/bin/bash


 ~/git/llvm/build/bin/opt "$1" -o "$1" \
                          --mem2reg \
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


# Get human readable bitcode
~/git/llvm/build/bin/llvm-dis "$1" -o "$1".ll

# And demangle it.
~/git/llvm/build/bin/llvm-cxxfilt < "$1".ll > "$1".demangled.ll

