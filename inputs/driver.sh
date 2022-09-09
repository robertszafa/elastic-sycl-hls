
if [ "$1" == "sim" ]; then
  echo "SIM"
  TO_BC=./compile_to_bc_sim.sh
  FROM_BC=./compile_from_bc_sim.sh
else
  TO_BC=./compile_to_bc.sh
  FROM_BC=./compile_from_bc.sh
fi

# Get IR of original source with kernel.
$TO_BC ex1/ex1.cpp

# Generate analysis json report.
~/git/llvm/build/bin/opt -load-pass-plugin ~/git/llvm-sycl-passes/build/lib/libLoopRAWHazardReport.so \
                         -passes=loop-raw-report ex1/ex1.cpp.ll -o ex1/ex1.cpp.ll > loop-raw-report.json

# Given json report, make kernel copies and pipe read/write calls from correct kernels.
# Output from this source-to-source transformation will be in ex1/ex1.cpp.tmp.cpp
python3 genKernelsAndPipes.py loop-raw-report.json ex1/ex1.cpp

# Get IR of source with kernels and pipes instantiated. 
$TO_BC ex1/ex1.tmp.cpp

# Transform each kernel to do only one correct work and write/read correct pipes.
~/git/llvm/build/bin/opt -load-pass-plugin ~/git/llvm-sycl-passes/build/lib/libStoreQueueTransform.so \
                         -passes=stq-transform ex1/ex1.tmp.cpp.bc -o ex1/ex1.tmp.cpp.bc.out

# Cleanup. The transformation leaves a lot of dead code, unused kernel args, etc.
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
                         ex1/ex1.tmp.cpp.bc.out -o ex1/ex1.tmp.cpp.bc.out

# For human readable IR checks.
~/git/llvm/build/bin/llvm-dis ex1/ex1.tmp.cpp.bc.out -o ex1/ex1.tmp.cpp.out.ll
~/git/llvm/build/bin/llvm-cxxfilt < ex1/ex1.tmp.cpp.out.ll > ex1/ex1.tmp.cpp.demangled.out.ll

# Produce final binary.
$FROM_BC ex1/ex1.tmp.cpp.bc.out ex1/ex1.tmp.cpp

echo "done"
