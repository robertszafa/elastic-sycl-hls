#!/bin/bash

# Perform a number of transformations to easy analysis.

#  Inline functions and optimize memcpys resulting from using structs.
 ~/git/llvm/build/bin/opt "$1" -o "$1" \
                          --always-inline \

# Our hoist-const-gep pass is similar to licm but we hoist GEPs with all constant indices 
# (this captures SYCL pointers) to the entry BB of the function.
 ~/git/llvm/build/bin/opt "$1" -o "$1" \
                          --load-pass-plugin ~/git/llvm-sycl-passes/build/lib/libHoistConstGep.so -passes=hoist-const-gep

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
                          --memcpyopt \
                          --sroa \
                          --dse \
                          --adce \
                          --dce \
                          --gvn \
                          --mergereturn \

                          # scalar replacement of aggregates

# Get human readable bitcode
~/git/llvm/build/bin/llvm-dis "$1" -o "$1".ll

# And demangle it.
~/git/llvm/build/bin/llvm-cxxfilt < "$1".ll > "$1".demangled.ll
