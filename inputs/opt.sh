
~/git/llvm/build/bin/opt -load-pass-plugin ~/git/llvm-sycl-passes/build/lib/libHelloWorld.so -passes=hello-world "$1"