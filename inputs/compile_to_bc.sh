# rm -rf /tmp/*

 "/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/clang++" -cc1 -triple spir64_fpga-unknown-unknown -aux-triple x86_64-unknown-linux-gnu -fsycl-is-device -fdeclare-spirv-builtins -fno-sycl-early-optimizations -fenable-sycl-dae -Wno-sycl-strict -fsycl-int-header=/tmp/ex1-header-f1b77b.h -fsycl-int-footer=/tmp/ex1-footer-b3249b.h -sycl-std=2020 -fsycl-unnamed-lambda -fsycl-unique-prefix=c868ca7c1f188435 -fsycl-disable-range-rounding -Wspir-compat -emit-llvm-bc -emit-llvm-uselists -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name "$1" -mrelocation-model static -fveclib=SVML -mframe-pointer=all -menable-no-infs -menable-no-nans -menable-unsafe-fp-math -fno-signed-zeros -mreassociate -freciprocal-math -fdenormal-fp-math=preserve-sign,preserve-sign -ffp-contract=fast -fno-rounding-math -ffast-math -ffinite-math-only -fno-verbose-asm -mconstructor-aliases -aux-target-cpu x86-64 -debug-info-kind=limited -dwarf-version=4 -debugger-tuning=gdb -resource-dir /opt/intel/oneapi/compiler/2022.1.0/linux/lib/clang/14.0.0 -dependency-file /tmp/ex1-8adf6e.d -MT ex1.o -internal-isystem /opt/intel/oneapi/compiler/2022.1.0/linux/lib/oclfpga/include -internal-isystem /opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/../include/sycl -internal-isystem /opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/../include -I /home/rob/git/sycl-playground/include -D FPGA_EMULATOR=1 -I/opt/intel/oneapi/vpl/2022.1.0/include -I/opt/intel/oneapi/tbb/2021.6.0/env/../include -I/opt/intel/oneapi/mpi/2021.6.0//include -I/opt/intel/oneapi/mkl/2022.1.0/include -I/opt/intel/oneapi/ippcp/2021.6.0/include -I/opt/intel/oneapi/ipp/2021.6.0/include -I/opt/intel/oneapi/dpl/2021.7.0/linux/include -I/opt/intel/oneapi/dpcpp-ct/2022.1.0/include -I/opt/intel/oneapi/dnnl/2022.1.0/cpu_dpcpp_gpu_dpcpp/include -I/opt/intel/oneapi/dev-utilities/2021.6.0/include -I/opt/intel/oneapi/dal/2021.6.0/include -I/opt/intel/oneapi/ccl/2021.6.0/include/cpu_gpu_dpcpp -internal-isystem /opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/../compiler/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/backward -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/backward -internal-isystem /opt/intel/oneapi/compiler/2022.1.0/linux/lib/clang/14.0.0/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -internal-isystem /opt/intel/oneapi/compiler/2022.1.0/linux/lib/clang/14.0.0/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -O2 -std=c++17 -fdeprecated-macro -fdebug-compilation-dir=/home/rob/git/llvm-sycl-passes/inputs/ex1 -ferror-limit 19 -fheinous-gnu-extensions -fgnuc-version=4.2.1 -fcxx-exceptions -fexceptions -fcolor-diagnostics -vectorize-loops -vectorize-slp -dwarf-debug-flags " --driver-mode=g++ --dpcpp -std=c++17 -O2 -I /home/rob/git/sycl-playground/include -qactypes -fintelfpga $1 -o bin/ex1.fpga_emu -D FPGA_EMULATOR=1 -g -fveclib=SVML -fheinous-gnu-extensions" -D__GCC_HAVE_DWARF2_CFI_ASM=1 -mllvm -disable-hir-generate-mkl-call -mllvm -intel-libirc-allowed -mllvm -loopopt=0 -floopopt-pipeline=none -o "$1".bc -x c++ "$1"
 

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
                          "$1".bc -o "$1".bc
                          # -stats \

 ~/git/llvm/build/bin/llvm-dis "$1".bc

~/git/llvm/build/bin/llvm-cxxfilt < "$1".ll > "$1".demangled.ll

