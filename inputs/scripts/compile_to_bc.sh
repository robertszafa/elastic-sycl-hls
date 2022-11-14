#!/bin/bash

# $1 - sim/emu/hw
# $2 - filename to compile

BASE_FNAME=$(basename ${2})
INT_HEADER=$HOME/tmp/$BASE_FNAME-integration-header.h
INT_FOOTER=$HOME/tmp/$BASE_FNAME-integration-footer.h

OUTPUT_FILE="$2.bc"

if [ "$1" == "sim" ]; then
  "/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/clang++" -cc1 -triple spir64_fpga-unknown-unknown -aux-triple x86_64-unknown-linux-gnu -fsycl-is-device -fdeclare-spirv-builtins -fno-sycl-early-optimizations -fenable-sycl-dae -Wno-sycl-strict -fsycl-int-header=$INT_HEADER -fsycl-int-footer=$INT_FOOTER -sycl-std=2020 -fsycl-unnamed-lambda -fsycl-unique-prefix=0251c73b0e837ff4 -fsycl-disable-range-rounding -Wspir-compat -emit-llvm-bc -emit-llvm-uselists -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name "$2" -mrelocation-model static -fveclib=SVML -mframe-pointer=all -menable-no-infs -menable-no-nans -menable-unsafe-fp-math -fno-signed-zeros -mreassociate -freciprocal-math -fdenormal-fp-math=preserve-sign,preserve-sign -ffp-contract=fast -fno-rounding-math -ffast-math -ffinite-math-only -fno-verbose-asm -mconstructor-aliases -aux-target-cpu x86-64 -debug-info-kind=limited -dwarf-version=4 -debugger-tuning=gdb -resource-dir /opt/intel/oneapi/compiler/2022.1.0/linux/lib/clang/14.0.0 -dependency-file /tmp/ex1-5139c3.d -MT ex1.o -internal-isystem /opt/intel/oneapi/compiler/2022.1.0/linux/lib/oclfpga/include -internal-isystem /opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/../include/sycl -internal-isystem /opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/../include -I /home/rob/git/sycl-playground/include -D FPGA=1 -I/opt/intel/oneapi/vpl/2022.1.0/include -I/opt/intel/oneapi/tbb/2021.6.0/env/../include -I/opt/intel/oneapi/mpi/2021.6.0//include -I/opt/intel/oneapi/mkl/2022.1.0/include -I/opt/intel/oneapi/ippcp/2021.6.0/include -I/opt/intel/oneapi/ipp/2021.6.0/include -I/opt/intel/oneapi/dpl/2021.7.0/linux/include -I/opt/intel/oneapi/dpcpp-ct/2022.1.0/include -I/opt/intel/oneapi/dnnl/2022.1.0/cpu_dpcpp_gpu_dpcpp/include -I/opt/intel/oneapi/dev-utilities/2021.6.0/include -I/opt/intel/oneapi/dal/2021.6.0/include -I/opt/intel/oneapi/ccl/2021.6.0/include/cpu_gpu_dpcpp -internal-isystem /opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/../compiler/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/backward -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/backward -internal-isystem /opt/intel/oneapi/compiler/2022.1.0/linux/lib/clang/14.0.0/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -internal-isystem /opt/intel/oneapi/compiler/2022.1.0/linux/lib/clang/14.0.0/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -O2 -std=c++17 -fdeprecated-macro -fdebug-compilation-dir=/home/rob/git/llvm-sycl-passes/inputs/ex1 -ferror-limit 19 -fheinous-gnu-extensions -fgnuc-version=4.2.1 -fcxx-exceptions -fexceptions -fcolor-diagnostics -vectorize-loops -vectorize-slp -dwarf-debug-flags " --driver-mode=g++ --dpcpp -std=c++17 -O2 -I /home/rob/git/sycl-playground/include -qactypes -fintelfpga -c $1 -o bin/ex1.dev.o -D FPGA=1 -g -fveclib=SVML -fheinous-gnu-extensions" -D__GCC_HAVE_DWARF2_CFI_ASM=1 -mllvm -disable-hir-generate-mkl-call -mllvm -intel-libirc-allowed -mllvm -loopopt=0 -floopopt-pipeline=none -o $OUTPUT_FILE -x c++ "$2"
elif  [ "$1" == "emu" ]; then
  "/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/clang++" -cc1 -triple spir64_fpga-unknown-unknown -aux-triple x86_64-unknown-linux-gnu -fsycl-is-device -fdeclare-spirv-builtins -fno-sycl-early-optimizations -fenable-sycl-dae -Wno-sycl-strict -fsycl-int-header=$INT_HEADER -fsycl-int-footer=$INT_FOOTER -sycl-std=2020 -fsycl-unnamed-lambda -fsycl-unique-prefix=c868ca7c1f188435 -fsycl-disable-range-rounding -Wspir-compat -emit-llvm-bc -emit-llvm-uselists -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name "$2" -mrelocation-model static -fveclib=SVML -mframe-pointer=all -menable-no-infs -menable-no-nans -menable-unsafe-fp-math -fno-signed-zeros -mreassociate -freciprocal-math -fdenormal-fp-math=preserve-sign,preserve-sign -ffp-contract=fast -fno-rounding-math -ffast-math -ffinite-math-only -fno-verbose-asm -mconstructor-aliases -aux-target-cpu x86-64 -debug-info-kind=limited -dwarf-version=4 -debugger-tuning=gdb -resource-dir /opt/intel/oneapi/compiler/2022.1.0/linux/lib/clang/14.0.0 -dependency-file /tmp/ex1-8adf6e.d -MT ex1.o -internal-isystem /opt/intel/oneapi/compiler/2022.1.0/linux/lib/oclfpga/include -internal-isystem /opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/../include/sycl -internal-isystem /opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/../include -I /home/rob/git/sycl-playground/include -D FPGA_EMULATOR=1 -I/opt/intel/oneapi/vpl/2022.1.0/include -I/opt/intel/oneapi/tbb/2021.6.0/env/../include -I/opt/intel/oneapi/mpi/2021.6.0//include -I/opt/intel/oneapi/mkl/2022.1.0/include -I/opt/intel/oneapi/ippcp/2021.6.0/include -I/opt/intel/oneapi/ipp/2021.6.0/include -I/opt/intel/oneapi/dpl/2021.7.0/linux/include -I/opt/intel/oneapi/dpcpp-ct/2022.1.0/include -I/opt/intel/oneapi/dnnl/2022.1.0/cpu_dpcpp_gpu_dpcpp/include -I/opt/intel/oneapi/dev-utilities/2021.6.0/include -I/opt/intel/oneapi/dal/2021.6.0/include -I/opt/intel/oneapi/ccl/2021.6.0/include/cpu_gpu_dpcpp -internal-isystem /opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/../compiler/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/backward -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/backward -internal-isystem /opt/intel/oneapi/compiler/2022.1.0/linux/lib/clang/14.0.0/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -internal-isystem /opt/intel/oneapi/compiler/2022.1.0/linux/lib/clang/14.0.0/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -O2 -std=c++17 -fdeprecated-macro -fdebug-compilation-dir=/home/rob/git/llvm-sycl-passes/inputs/ex1 -ferror-limit 19 -fheinous-gnu-extensions -fgnuc-version=4.2.1 -fcxx-exceptions -fexceptions -fcolor-diagnostics -vectorize-loops -vectorize-slp -dwarf-debug-flags " --driver-mode=g++ --dpcpp -std=c++17 -O2 -I /home/rob/git/sycl-playground/include -qactypes -fintelfpga $1 -o bin/ex1.fpga_emu -D FPGA_EMULATOR=1 -g -fveclib=SVML -fheinous-gnu-extensions" -D__GCC_HAVE_DWARF2_CFI_ASM=1 -mllvm -disable-hir-generate-mkl-call -mllvm -intel-libirc-allowed -mllvm -loopopt=0 -floopopt-pipeline=none -o $OUTPUT_FILE -x c++ "$2"
elif [ "$1" == "hw" ]; then
  # This works only on devcloud
 "/glob/development-tools/versions/oneapi/2022.3/oneapi/compiler/2022.2.0/linux/bin-llvm/clang++" -cc1 -triple spir64_fpga-unknown-unknown -aux-triple x86_64-unknown-linux-gnu -fsycl-is-device -fdeclare-spirv-builtins -fno-sycl-early-optimizations -fenable-sycl-dae -fsycl-instrument-device-code -Wno-sycl-strict -fsycl-int-header=$INT_HEADER -fsycl-int-footer=$INT_FOOTER -sycl-std=2020 -fsycl-unnamed-lambda -fsycl-unique-prefix=a353902c1428e68f -fsycl-disable-range-rounding -fintelfpga -Wspir-compat -emit-llvm-bc -emit-llvm-uselists -disable-free -clear-ast-before-backend -disable-llvm-verifier -main-file-name $2 -mrelocation-model static -fveclib=SVML -mframe-pointer=all -menable-no-infs -menable-no-nans -fapprox-func -menable-unsafe-fp-math -fno-signed-zeros -mreassociate -freciprocal-math -fdenormal-fp-math=preserve-sign,preserve-sign -ffp-contract=fast -fno-rounding-math -ffast-math -ffinite-math-only -fno-verbose-asm -mconstructor-aliases -aux-target-cpu x86-64 -mllvm -treat-scalable-fixed-error-as-warning -debug-info-kind=limited -dwarf-version=4 -debugger-tuning=gdb -resource-dir /glob/development-tools/versions/oneapi/2022.3/oneapi/compiler/2022.2.0/linux/lib/clang/15.0.0 -dependency-file /home/u119070/tmp/histogram-a1fcd0.d -MT histogram.o -internal-isystem /glob/development-tools/versions/oneapi/2022.3/oneapi/compiler/2022.2.0/linux/lib/oclfpga/include -internal-isystem /glob/development-tools/versions/oneapi/2022.3/oneapi/compiler/2022.2.0/linux/bin-llvm/../include/sycl -internal-isystem /glob/development-tools/versions/oneapi/2022.3/oneapi/compiler/2022.2.0/linux/bin-llvm/../include -I /home/u119070/git/sycl-playground/include -D FPGA=1 -I/glob/development-tools/versions/oneapi/2022.3/oneapi/vpl/2022.2.0/include -I/glob/development-tools/versions/oneapi/2022.3/oneapi/tbb/2021.7.0/env/../include -I/glob/development-tools/versions/oneapi/2022.3/oneapi/mpi/2021.7.0//include -I/glob/development-tools/versions/oneapi/2022.3/oneapi/mkl/2022.2.0/include -I/glob/development-tools/versions/oneapi/2022.3/oneapi/ipp/2021.6.1/include -I/glob/development-tools/versions/oneapi/2022.3/oneapi/ippcp/2021.6.1/include -I/glob/development-tools/versions/oneapi/2022.3/oneapi/ipp/2021.6.1/include -I/glob/development-tools/versions/oneapi/2022.3/oneapi/dpl/2021.7.1/linux/include -I/glob/development-tools/versions/oneapi/2022.3/oneapi/dpcpp-ct/2022.2.0/include -I/glob/development-tools/versions/oneapi/2022.3/oneapi/dnnl/2022.2.0/cpu_dpcpp_gpu_dpcpp/include -I/glob/development-tools/versions/oneapi/2022.3/oneapi/dev-utilities/2021.7.0/include -I/glob/development-tools/versions/oneapi/2022.3/oneapi/dal/2021.7.0/include -I/glob/development-tools/versions/oneapi/2022.3/oneapi/ccl/2021.7.0/include/cpu_gpu_dpcpp -cxx-isystem /glob/development-tools/versions/oneapi/2022.3/oneapi/clck/2021.7.0/include -internal-isystem /glob/development-tools/versions/oneapi/2022.3/oneapi/compiler/2022.2.0/linux/bin-llvm/../compiler/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/backward -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/backward -internal-isystem /glob/development-tools/versions/oneapi/2022.3/oneapi/compiler/2022.2.0/linux/lib/clang/15.0.0/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -internal-isystem /glob/development-tools/versions/oneapi/2022.3/oneapi/compiler/2022.2.0/linux/lib/clang/15.0.0/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -O2 -std=c++17 -fdeprecated-macro -fdebug-compilation-dir=/home/u119070/git/llvm-sycl-passes/inputs/histogram -ferror-limit 19 -fheinous-gnu-extensions -fgnuc-version=4.2.1 -fcxx-exceptions -fexceptions -fcolor-diagnostics -vectorize-loops -vectorize-slp -dwarf-debug-flags " --driver-mode=g++ --dpcpp -std=c++17 -O2 -I /home/u119070/git/sycl-playground/include -qactypes -fintelfpga -c $2 -o bin/histogram.dev.o -D FPGA=1 -g -fveclib=SVML -fheinous-gnu-extensions" -D__GCC_HAVE_DWARF2_CFI_ASM=1 -fintel-compatibility -fintel-compatibility-disable=FakeLoad -fintel-libirc-allowed -mllvm -disable-hir-generate-mkl-call -mllvm -intel-abi-compatible=true -o $2.bc -x c++ $2
fi
