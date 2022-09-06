./compile_to_bc.sh ex1/ex1.cpp

~/git/llvm/build/bin/opt -load-pass-plugin ~/git/llvm-sycl-passes/build/lib/libLoopRAWHazardReport.so \
                         -passes=loop-raw-report ex1/ex1.cpp.ll -o ex1/ex1.cpp.ll 

~/git/llvm/build/bin/opt -load-pass-plugin ~/git/llvm-sycl-passes/build/lib/libLoopRAWHazardReport.so \
                         -passes=loop-raw-report ex1/ex1.cpp.ll -o ex1/ex1.cpp.ll > loop-raw-report.json

python3 genKernelsAndPipes.py loop-raw-report.json ex1/ex1.cpp
