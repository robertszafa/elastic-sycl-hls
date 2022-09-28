#!/bin/bash

# Perform a number of transformations to easy analysis.
 ~/git/llvm/build/bin/opt --mem2reg \
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
                          "$1" -o "$1"
                          # --licm \
                          # --loop-rotate \
                          # --lcssa \
                          # --stats \

# Get human readable bitcode
~/git/llvm/build/bin/llvm-dis "$1" -o "$1".ll

# And demangle it.
~/git/llvm/build/bin/llvm-cxxfilt < "$1".ll > "$1".ll.demangled

