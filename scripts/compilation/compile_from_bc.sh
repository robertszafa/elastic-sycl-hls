#!/bin/bash

    # $1 - emu / sim / hw
    # $2 - bc file
    # $3 - original filename
    # $4 - output binary filename

    
if [ "$1" == "emu" ]; then
	 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/llvm-link" "$2" -o /tmp/icpx-8cfc78/example-f7a731.bc --suppress-warnings
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-crt.o -output=/tmp/icpx-8cfc78/libsycl-crt-8167ed.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-8cfc78
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-complex.o -output=/tmp/icpx-8cfc78/libsycl-complex-67d589.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-8cfc78
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-complex-fp64.o -output=/tmp/icpx-8cfc78/libsycl-complex-fp64-45da34.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-8cfc78
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-cmath.o -output=/tmp/icpx-8cfc78/libsycl-cmath-46a70c.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-8cfc78
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-cmath-fp64.o -output=/tmp/icpx-8cfc78/libsycl-cmath-fp64-504f77.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-8cfc78
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-imf.o -output=/tmp/icpx-8cfc78/libsycl-imf-008676.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-8cfc78
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-imf-fp64.o -output=/tmp/icpx-8cfc78/libsycl-imf-fp64-3ee036.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-8cfc78
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-imf-bf16.o -output=/tmp/icpx-8cfc78/libsycl-imf-bf16-a7c72f.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-8cfc78
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-fallback-cassert.o -output=/tmp/icpx-8cfc78/libsycl-fallback-cassert-b655d6.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-8cfc78
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-fallback-cstring.o -output=/tmp/icpx-8cfc78/libsycl-fallback-cstring-22661d.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-8cfc78
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-fallback-complex.o -output=/tmp/icpx-8cfc78/libsycl-fallback-complex-d2d585.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-8cfc78
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-fallback-complex-fp64.o -output=/tmp/icpx-8cfc78/libsycl-fallback-complex-fp64-bc39c6.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-8cfc78
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-fallback-cmath.o -output=/tmp/icpx-8cfc78/libsycl-fallback-cmath-06ecd0.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-8cfc78
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-fallback-cmath-fp64.o -output=/tmp/icpx-8cfc78/libsycl-fallback-cmath-fp64-b861e3.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-8cfc78
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-fallback-imf.o -output=/tmp/icpx-8cfc78/libsycl-fallback-imf-705d66.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-8cfc78
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-fallback-imf-fp64.o -output=/tmp/icpx-8cfc78/libsycl-fallback-imf-fp64-a38881.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-8cfc78
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-fallback-imf-bf16.o -output=/tmp/icpx-8cfc78/libsycl-fallback-imf-bf16-d3d0d9.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-8cfc78
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-itt-user-wrappers.o -output=/tmp/icpx-8cfc78/libsycl-itt-user-wrappers-6d049d.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-8cfc78
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-itt-compiler-wrappers.o -output=/tmp/icpx-8cfc78/libsycl-itt-compiler-wrappers-446a54.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-8cfc78
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-itt-stubs.o -output=/tmp/icpx-8cfc78/libsycl-itt-stubs-dc429a.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-8cfc78
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/llvm-link" -only-needed /tmp/icpx-8cfc78/example-f7a731.bc /tmp/icpx-8cfc78/libsycl-crt-8167ed.o /tmp/icpx-8cfc78/libsycl-complex-67d589.o /tmp/icpx-8cfc78/libsycl-complex-fp64-45da34.o /tmp/icpx-8cfc78/libsycl-cmath-46a70c.o /tmp/icpx-8cfc78/libsycl-cmath-fp64-504f77.o /tmp/icpx-8cfc78/libsycl-imf-008676.o /tmp/icpx-8cfc78/libsycl-imf-fp64-3ee036.o /tmp/icpx-8cfc78/libsycl-imf-bf16-a7c72f.o /tmp/icpx-8cfc78/libsycl-fallback-cassert-b655d6.o /tmp/icpx-8cfc78/libsycl-fallback-cstring-22661d.o /tmp/icpx-8cfc78/libsycl-fallback-complex-d2d585.o /tmp/icpx-8cfc78/libsycl-fallback-complex-fp64-bc39c6.o /tmp/icpx-8cfc78/libsycl-fallback-cmath-06ecd0.o /tmp/icpx-8cfc78/libsycl-fallback-cmath-fp64-b861e3.o /tmp/icpx-8cfc78/libsycl-fallback-imf-705d66.o /tmp/icpx-8cfc78/libsycl-fallback-imf-fp64-a38881.o /tmp/icpx-8cfc78/libsycl-fallback-imf-bf16-d3d0d9.o /tmp/icpx-8cfc78/libsycl-itt-user-wrappers-6d049d.o /tmp/icpx-8cfc78/libsycl-itt-compiler-wrappers-446a54.o /tmp/icpx-8cfc78/libsycl-itt-stubs-dc429a.o -o /tmp/icpx-8cfc78/example-94d1c5.bc --suppress-warnings
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin/sycl-post-link" -emit-only-kernels-as-entry-points -emit-param-info -symbols -emit-exported-symbols -split-esimd -lower-esimd -O2 -spec-const=default -device-globals -o /tmp/icpx-8cfc78/example-298f95.table /tmp/icpx-8cfc78/example-94d1c5.bc
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/file-table-tform" -extract=Code -drop_titles -o /tmp/icpx-8cfc78/example-9f294b.txt /tmp/icpx-8cfc78/example-298f95.table
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/llvm-foreach" --in-file-list=/tmp/icpx-8cfc78/example-9f294b.txt --in-replace=/tmp/icpx-8cfc78/example-9f294b.txt --out-ext=spv --out-file-list=/tmp/icpx-8cfc78/example-680c35.txt --out-replace=/tmp/icpx-8cfc78/example-680c35.txt --out-dir=/tmp/icpx-8cfc78 -- /opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/llvm-spirv -o /tmp/icpx-8cfc78/example-680c35.txt -spirv-max-version=1.4 -spirv-debug-info-version=ocl-100 -spirv-allow-extra-diexpressions -spirv-allow-unknown-intrinsics=llvm.genx. -spirv-ext=-all,+SPV_EXT_shader_atomic_float_add,+SPV_EXT_shader_atomic_float_min_max,+SPV_KHR_no_integer_wrap_decoration,+SPV_KHR_float_controls,+SPV_KHR_expect_assume,+SPV_KHR_linkonce_odr,+SPV_INTEL_subgroups,+SPV_INTEL_media_block_io,+SPV_INTEL_device_side_avc_motion_estimation,+SPV_INTEL_fpga_loop_controls,+SPV_INTEL_unstructured_loop_controls,+SPV_INTEL_fpga_reg,+SPV_INTEL_blocking_pipes,+SPV_INTEL_function_pointers,+SPV_INTEL_kernel_attributes,+SPV_INTEL_io_pipes,+SPV_INTEL_inline_assembly,+SPV_INTEL_arbitrary_precision_integers,+SPV_INTEL_float_controls2,+SPV_INTEL_vector_compute,+SPV_INTEL_fast_composite,+SPV_INTEL_joint_matrix,+SPV_INTEL_arbitrary_precision_fixed_point,+SPV_INTEL_arbitrary_precision_floating_point,+SPV_INTEL_variable_length_array,+SPV_INTEL_fp_fast_math_mode,+SPV_INTEL_long_constant_composite,+SPV_INTEL_arithmetic_fence,+SPV_INTEL_global_variable_decorations,+SPV_INTEL_task_sequence,+SPV_INTEL_optnone,+SPV_INTEL_token_type,+SPV_INTEL_bfloat16_conversion,+SPV_INTEL_joint_matrix,+SPV_INTEL_hw_thread_queries,+SPV_INTEL_memory_access_aliasing,+SPV_KHR_uniform_group_instructions,+SPV_INTEL_masked_gather_scatter,+SPV_INTEL_tensor_float32_conversion /tmp/icpx-8cfc78/example-9f294b.txt
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/llvm-foreach" --out-ext=aocx --in-file-list=/tmp/icpx-8cfc78/example-680c35.txt --in-replace=/tmp/icpx-8cfc78/example-680c35.txt --out-file-list=/tmp/icpx-8cfc78/example-00243b.aocx --out-replace=/tmp/icpx-8cfc78/example-00243b.aocx -- /opt/intel/oneapi/compiler/2023.1.0/linux/bin/opencl-aot -device=fpga_fast_emu -spv=/tmp/icpx-8cfc78/example-680c35.txt -ir=/tmp/icpx-8cfc78/example-00243b.aocx --bo=-g
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/file-table-tform" -replace=Code,Code -o /tmp/icpx-8cfc78/example-ecfbdb.table /tmp/icpx-8cfc78/example-298f95.table /tmp/icpx-8cfc78/example-00243b.aocx
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-wrapper" -o=/tmp/icpx-8cfc78/wrapper-e8d111.bc -host=x86_64-unknown-linux-gnu -target=spir64_fpga -kind=sycl -batch /tmp/icpx-8cfc78/example-ecfbdb.table
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/llc" -filetype=obj -o /tmp/icpx-8cfc78/example-0049fd.o /tmp/icpx-8cfc78/wrapper-e8d111.bc
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/append-file" "$3" --append=/tmp/icpx-8cfc78/example-footer-90f1ce.h --orig-filename="$3" --output=/tmp/icpx-8cfc78/example-c89c4c.cpp --use-include
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang++" -cc1 -triple x86_64-unknown-linux-gnu -sycl-std=2020 -fsycl-unique-prefix=1334f98bb55c8b61 -aux-triple spir64_fpga-unknown-unknown -fsycl-disable-range-rounding -fintelfpga -include /tmp/icpx-8cfc78/example-header-e5fd7f.h -dependency-filter /tmp/icpx-8cfc78/example-header-e5fd7f.h -fsycl-enable-int-header-diags -fsycl-is-host -emit-obj --mrelax-relocations -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name "$3" -fsycl-use-main-file-name -full-main-file-name "$3" -mrelocation-model static -fveclib=SVML -mframe-pointer=none -menable-no-infs -menable-no-nans -fapprox-func -funsafe-math-optimizations -fno-signed-zeros -mreassociate -freciprocal-math -fdenormal-fp-math=preserve-sign,preserve-sign -ffp-contract=fast -fno-rounding-math -ffast-math -ffinite-math-only -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -mllvm -x86-enable-unaligned-vector-move=true -tune-cpu generic -mllvm -treat-scalable-fixed-error-as-warning -debug-info-kind=limited -dwarf-version=4 -debugger-tuning=gdb   -fcoverage-compilation-dir=/home/rob/git/llvm-sycl-passes/scripts/compilation/example -resource-dir /opt/intel/oneapi/compiler/2023.1.0/linux/lib/clang/16 -internal-isystem /opt/intel/oneapi/compiler/2023.1.0/linux/lib/oclfpga/include -internal-isystem /opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../include/sycl -internal-isystem /opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../include -iquote /home/rob/git/llvm-sycl-passes/scripts/compilation/example -I /home/rob/git/llvm-sycl-passes/include_sycl -I /home/rob/git/llvm-sycl-passes/lsq -D FPGA_EMU -I/opt/intel/oneapi/tbb/2021.9.0/env/../include -I/opt/intel/oneapi/mpi/2021.9.0//include -I/opt/intel/oneapi/mkl/2023.1.0/include -I/opt/intel/oneapi/ippcp/2021.7.0/include -I/opt/intel/oneapi/ipp/2021.8.0/include -I/opt/intel/oneapi/dpl/2022.1.0/linux/include -I/opt/intel/oneapi/dpcpp-ct/2023.1.0/include -I/opt/intel/oneapi/dnnl/2023.1.0/cpu_dpcpp_gpu_dpcpp/include -I/opt/intel/oneapi/dev-utilities/2021.9.0/include -I/opt/intel/oneapi/dal/2023.1.0/include -I/opt/intel/oneapi/ccl/2021.9.0/include/cpu_gpu_dpcpp -internal-isystem /opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../compiler/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward -internal-isystem /opt/intel/oneapi/compiler/2023.1.0/linux/lib/clang/16/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -internal-isystem /opt/intel/oneapi/compiler/2023.1.0/linux/lib/clang/16/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -O2 -std=c++17 -fdeprecated-macro -fdebug-compilation-dir=/home/rob/git/llvm-sycl-passes/scripts/compilation/example -ferror-limit 19 -fheinous-gnu-extensions -fgpu-rdc -fgnuc-version=4.2.1 -no-opaque-pointers -fcxx-exceptions -fexceptions -vectorize-loops -vectorize-slp -dwarf-debug-flags " --driver-mode=g++ --intel -I /home/rob/git/llvm-sycl-passes/include_sycl -I /home/rob/git/llvm-sycl-passes/lsq -fsycl -std=c++17 -O2 -qactypes   -fintelfpga -D FPGA_EMU "$3" -o "$4" -g -fveclib=SVML -fheinous-gnu-extensions" -D__GCC_HAVE_DWARF2_CFI_ASM=1 -fintel-compatibility -fintel-libirc-allowed -mllvm -disable-hir-generate-mkl-call -mllvm -sycl-host -mllvm -loopopt=1 -floopopt-pipeline=light -mllvm -intel-abi-compatible=true -o /tmp/icpx-8cfc78/example-25d490.o -x c++ /tmp/icpx-8cfc78/example-c89c4c.cpp
 "/usr/bin/ld" -z relro --hash-style=gnu --eh-frame-hdr -m elf_x86_64 -dynamic-linker /lib64/ld-linux-x86-64.so.2 -o "$4" /lib/x86_64-linux-gnu/Scrt1.o /lib/x86_64-linux-gnu/crti.o /usr/lib/gcc/x86_64-linux-gnu/11/crtbeginS.o -L/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../compiler/lib/intel64_lin -L/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib -L/opt/intel/oneapi/compiler/2023.1.0/linux/lib/oclfpga/host/linux64/lib -L/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../compiler/lib/intel64_lin -L/usr/lib/gcc/x86_64-linux-gnu/11 -L/usr/lib/gcc/x86_64-linux-gnu/11/../../../../lib64 -L/lib/x86_64-linux-gnu -L/lib/../lib64 -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib64 -L/usr/lib/gcc/x86_64-linux-gnu/11/../../.. -L/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib -L/lib -L/usr/lib -L/opt/intel/oneapi/tbb/2021.9.0/env/../lib/intel64/gcc4.8 -L/opt/intel/oneapi/mpi/2021.9.0//libfabric/lib -L/opt/intel/oneapi/mpi/2021.9.0//lib/release -L/opt/intel/oneapi/mpi/2021.9.0//lib -L/opt/intel/oneapi/mkl/2023.1.0/lib/intel64 -L/opt/intel/oneapi/ippcp/2021.7.0/lib/intel64 -L/opt/intel/oneapi/ipp/2021.8.0/lib/intel64 -L/opt/intel/oneapi/dnnl/2023.1.0/cpu_dpcpp_gpu_dpcpp/lib -L/opt/intel/oneapi/dal/2023.1.0/lib/intel64 -L/opt/intel/oneapi/compiler/2023.1.0/linux/compiler/lib/intel64_lin -L/opt/intel/oneapi/compiler/2023.1.0/linux/lib -L/opt/intel/oneapi/ccl/2021.9.0/lib/cpu_gpu_dpcpp /tmp/icpx-8cfc78/example-25d490.o /tmp/icpx-8cfc78/example-0049fd.o -ldspba_mpir -ldspba_mpfr -lac_types_fixed_point_math_x86 -lac_types_vpfp_library -Bstatic -lsvml -Bdynamic -Bstatic -lirng -Bdynamic -lstdc++ -Bstatic -limf -Bdynamic -lm -lgcc_s -lgcc -Bstatic -lirc -Bdynamic -ldl -lgcc_s -lgcc -lsycl -lsycl-devicelib-host -lOpenCL -lc -lgcc_s -lgcc -Bstatic -lirc_s -Bdynamic /usr/lib/gcc/x86_64-linux-gnu/11/crtendS.o /lib/x86_64-linux-gnu/crtn.o
exit
fi

if [ "$1" == "sim" ]; then
	 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/llvm-link" "$2" -o /tmp/icpx-6d4981/example-776a23.bc --suppress-warnings
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-crt.o -output=/tmp/icpx-6d4981/libsycl-crt-aa7e6c.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-6d4981
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-complex.o -output=/tmp/icpx-6d4981/libsycl-complex-3f5afb.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-6d4981
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-complex-fp64.o -output=/tmp/icpx-6d4981/libsycl-complex-fp64-6892ec.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-6d4981
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-cmath.o -output=/tmp/icpx-6d4981/libsycl-cmath-5decfd.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-6d4981
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-cmath-fp64.o -output=/tmp/icpx-6d4981/libsycl-cmath-fp64-864e06.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-6d4981
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-imf.o -output=/tmp/icpx-6d4981/libsycl-imf-1a1897.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-6d4981
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-imf-fp64.o -output=/tmp/icpx-6d4981/libsycl-imf-fp64-5c6a75.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-6d4981
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-imf-bf16.o -output=/tmp/icpx-6d4981/libsycl-imf-bf16-6ddffc.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-6d4981
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-fallback-cassert.o -output=/tmp/icpx-6d4981/libsycl-fallback-cassert-b59918.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-6d4981
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-fallback-cstring.o -output=/tmp/icpx-6d4981/libsycl-fallback-cstring-69fb8f.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-6d4981
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-fallback-complex.o -output=/tmp/icpx-6d4981/libsycl-fallback-complex-19a223.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-6d4981
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-fallback-complex-fp64.o -output=/tmp/icpx-6d4981/libsycl-fallback-complex-fp64-97f016.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-6d4981
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-fallback-cmath.o -output=/tmp/icpx-6d4981/libsycl-fallback-cmath-573373.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-6d4981
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-fallback-cmath-fp64.o -output=/tmp/icpx-6d4981/libsycl-fallback-cmath-fp64-f288c9.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-6d4981
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-fallback-imf.o -output=/tmp/icpx-6d4981/libsycl-fallback-imf-023fdb.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-6d4981
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-fallback-imf-fp64.o -output=/tmp/icpx-6d4981/libsycl-fallback-imf-fp64-ff4916.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-6d4981
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-fallback-imf-bf16.o -output=/tmp/icpx-6d4981/libsycl-fallback-imf-bf16-cbdbbf.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-6d4981
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-itt-user-wrappers.o -output=/tmp/icpx-6d4981/libsycl-itt-user-wrappers-20653d.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-6d4981
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-itt-compiler-wrappers.o -output=/tmp/icpx-6d4981/libsycl-itt-compiler-wrappers-8200ac.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-6d4981
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -input=/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib/libsycl-itt-stubs.o -output=/tmp/icpx-6d4981/libsycl-itt-stubs-abedac.o -unbundle -allow-missing-bundles -base-temp-dir=/tmp/icpx-6d4981
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/llvm-link" -only-needed /tmp/icpx-6d4981/example-776a23.bc /tmp/icpx-6d4981/libsycl-crt-aa7e6c.o /tmp/icpx-6d4981/libsycl-complex-3f5afb.o /tmp/icpx-6d4981/libsycl-complex-fp64-6892ec.o /tmp/icpx-6d4981/libsycl-cmath-5decfd.o /tmp/icpx-6d4981/libsycl-cmath-fp64-864e06.o /tmp/icpx-6d4981/libsycl-imf-1a1897.o /tmp/icpx-6d4981/libsycl-imf-fp64-5c6a75.o /tmp/icpx-6d4981/libsycl-imf-bf16-6ddffc.o /tmp/icpx-6d4981/libsycl-fallback-cassert-b59918.o /tmp/icpx-6d4981/libsycl-fallback-cstring-69fb8f.o /tmp/icpx-6d4981/libsycl-fallback-complex-19a223.o /tmp/icpx-6d4981/libsycl-fallback-complex-fp64-97f016.o /tmp/icpx-6d4981/libsycl-fallback-cmath-573373.o /tmp/icpx-6d4981/libsycl-fallback-cmath-fp64-f288c9.o /tmp/icpx-6d4981/libsycl-fallback-imf-023fdb.o /tmp/icpx-6d4981/libsycl-fallback-imf-fp64-ff4916.o /tmp/icpx-6d4981/libsycl-fallback-imf-bf16-cbdbbf.o /tmp/icpx-6d4981/libsycl-itt-user-wrappers-20653d.o /tmp/icpx-6d4981/libsycl-itt-compiler-wrappers-8200ac.o /tmp/icpx-6d4981/libsycl-itt-stubs-abedac.o -o /tmp/icpx-6d4981/example-89bc2c.bc --suppress-warnings
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin/sycl-post-link" -emit-only-kernels-as-entry-points -emit-param-info -symbols -emit-exported-symbols -split-esimd -lower-esimd -O2 -spec-const=default -device-globals -o /tmp/icpx-6d4981/example-3e70a2.table /tmp/icpx-6d4981/example-89bc2c.bc
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/file-table-tform" -extract=Code -drop_titles -o /tmp/icpx-6d4981/example-fc3616.txt /tmp/icpx-6d4981/example-3e70a2.table
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/llvm-foreach" --in-file-list=/tmp/icpx-6d4981/example-fc3616.txt --in-replace=/tmp/icpx-6d4981/example-fc3616.txt --out-ext=spv --out-file-list=/tmp/icpx-6d4981/example-3a93a3.txt --out-replace=/tmp/icpx-6d4981/example-3a93a3.txt --out-dir=/tmp/icpx-6d4981 -- /opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/llvm-spirv -o /tmp/icpx-6d4981/example-3a93a3.txt -spirv-max-version=1.4 -spirv-debug-info-version=ocl-100 -spirv-allow-extra-diexpressions -spirv-allow-unknown-intrinsics=llvm.genx. -spirv-ext=-all,+SPV_EXT_shader_atomic_float_add,+SPV_EXT_shader_atomic_float_min_max,+SPV_KHR_no_integer_wrap_decoration,+SPV_KHR_float_controls,+SPV_KHR_expect_assume,+SPV_KHR_linkonce_odr,+SPV_INTEL_subgroups,+SPV_INTEL_media_block_io,+SPV_INTEL_device_side_avc_motion_estimation,+SPV_INTEL_fpga_loop_controls,+SPV_INTEL_unstructured_loop_controls,+SPV_INTEL_fpga_reg,+SPV_INTEL_blocking_pipes,+SPV_INTEL_function_pointers,+SPV_INTEL_kernel_attributes,+SPV_INTEL_io_pipes,+SPV_INTEL_inline_assembly,+SPV_INTEL_arbitrary_precision_integers,+SPV_INTEL_float_controls2,+SPV_INTEL_vector_compute,+SPV_INTEL_fast_composite,+SPV_INTEL_joint_matrix,+SPV_INTEL_arbitrary_precision_fixed_point,+SPV_INTEL_arbitrary_precision_floating_point,+SPV_INTEL_variable_length_array,+SPV_INTEL_fp_fast_math_mode,+SPV_INTEL_long_constant_composite,+SPV_INTEL_arithmetic_fence,+SPV_INTEL_global_variable_decorations,+SPV_INTEL_task_sequence,+SPV_INTEL_optnone,+SPV_INTEL_usm_storage_classes,+SPV_INTEL_runtime_aligned,+SPV_INTEL_fpga_cluster_attributes,+SPV_INTEL_loop_fuse,+SPV_INTEL_fpga_buffer_location,+SPV_INTEL_fpga_invocation_pipelining_attributes,+SPV_INTEL_fpga_dsp_control,+SPV_INTEL_fpga_memory_accesses,+SPV_INTEL_fpga_memory_attributes,-SPV_INTEL_optnone /tmp/icpx-6d4981/example-fc3616.txt
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/llvm-foreach" --out-ext=aocx --in-file-list=/tmp/icpx-6d4981/example-3a93a3.txt --in-replace=/tmp/icpx-6d4981/example-3a93a3.txt --out-file-list=/tmp/icpx-6d4981/example-f4ee19.aocx --out-replace=/tmp/icpx-6d4981/example-f4ee19.aocx --out-increment="$4".prj -- /opt/intel/oneapi/compiler/2023.1.0/linux/lib/oclfpga/bin/aoc -o /tmp/icpx-6d4981/example-f4ee19.aocx /tmp/icpx-6d4981/example-3a93a3.txt -sycl -dep-files=/tmp/icpx-6d4981/example-e272fb.d -output-report-folder="$4".prj -g -simulation -board=intel_a10gx_pac:pac_a10
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/file-table-tform" -replace=Code,Code -o /tmp/icpx-6d4981/example-a92565.table /tmp/icpx-6d4981/example-3e70a2.table /tmp/icpx-6d4981/example-f4ee19.aocx
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang-offload-wrapper" -o=/tmp/icpx-6d4981/wrapper-b99efb.bc -host=x86_64-unknown-linux-gnu -target=spir64_fpga -kind=sycl -batch /tmp/icpx-6d4981/example-a92565.table
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/llc" -filetype=obj -o /tmp/icpx-6d4981/example-19399d.o /tmp/icpx-6d4981/wrapper-b99efb.bc
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/append-file" "$3" --append=/tmp/icpx-6d4981/example-footer-997f6b.h --orig-filename="$3" --output=/tmp/icpx-6d4981/example-525e6f.cpp --use-include
 "/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/clang++" -cc1 -triple x86_64-unknown-linux-gnu -sycl-std=2020 -fsycl-unique-prefix=d468f44d448b3ac0 -aux-triple spir64_fpga-unknown-unknown -fsycl-disable-range-rounding -fintelfpga -include /tmp/icpx-6d4981/example-header-844b55.h -dependency-filter /tmp/icpx-6d4981/example-header-844b55.h -fsycl-enable-int-header-diags -fsycl-is-host -emit-obj --mrelax-relocations -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name "$3" -fsycl-use-main-file-name -full-main-file-name "$3" -mrelocation-model static -fveclib=SVML -mframe-pointer=none -menable-no-infs -menable-no-nans -fapprox-func -funsafe-math-optimizations -fno-signed-zeros -mreassociate -freciprocal-math -fdenormal-fp-math=preserve-sign,preserve-sign -ffp-contract=fast -fno-rounding-math -ffast-math -ffinite-math-only -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -mllvm -x86-enable-unaligned-vector-move=true -tune-cpu generic -mllvm -treat-scalable-fixed-error-as-warning -debug-info-kind=limited -dwarf-version=4 -debugger-tuning=gdb   -fcoverage-compilation-dir=/home/rob/git/llvm-sycl-passes/scripts/compilation/example -resource-dir /opt/intel/oneapi/compiler/2023.1.0/linux/lib/clang/16 -internal-isystem /opt/intel/oneapi/compiler/2023.1.0/linux/lib/oclfpga/include -internal-isystem /opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../include/sycl -internal-isystem /opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../include -iquote /home/rob/git/llvm-sycl-passes/scripts/compilation/example -I /home/rob/git/llvm-sycl-passes/include_sycl -I /home/rob/git/llvm-sycl-passes/lsq -D FPGA_SIM -I/opt/intel/oneapi/tbb/2021.9.0/env/../include -I/opt/intel/oneapi/mpi/2021.9.0//include -I/opt/intel/oneapi/mkl/2023.1.0/include -I/opt/intel/oneapi/ippcp/2021.7.0/include -I/opt/intel/oneapi/ipp/2021.8.0/include -I/opt/intel/oneapi/dpl/2022.1.0/linux/include -I/opt/intel/oneapi/dpcpp-ct/2023.1.0/include -I/opt/intel/oneapi/dnnl/2023.1.0/cpu_dpcpp_gpu_dpcpp/include -I/opt/intel/oneapi/dev-utilities/2021.9.0/include -I/opt/intel/oneapi/dal/2023.1.0/include -I/opt/intel/oneapi/ccl/2021.9.0/include/cpu_gpu_dpcpp -internal-isystem /opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../compiler/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward -internal-isystem /opt/intel/oneapi/compiler/2023.1.0/linux/lib/clang/16/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -internal-isystem /opt/intel/oneapi/compiler/2023.1.0/linux/lib/clang/16/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -O2 -std=c++17 -fdeprecated-macro -fdebug-compilation-dir=/home/rob/git/llvm-sycl-passes/scripts/compilation/example -ferror-limit 19 -fheinous-gnu-extensions -fgpu-rdc -fgnuc-version=4.2.1 -no-opaque-pointers -fcxx-exceptions -fexceptions -vectorize-loops -vectorize-slp -dwarf-debug-flags " --driver-mode=g++ --intel -I /home/rob/git/llvm-sycl-passes/include_sycl -I /home/rob/git/llvm-sycl-passes/lsq -fsycl -std=c++17 -O2 -qactypes   -fintelfpga -D FPGA_SIM -Xssimulation -Xsboard=intel_a10gx_pac:pac_a10 "$3" -o "$4" -g -fveclib=SVML -fheinous-gnu-extensions" -D__GCC_HAVE_DWARF2_CFI_ASM=1 -fintel-compatibility -fintel-libirc-allowed -mllvm -disable-hir-generate-mkl-call -mllvm -sycl-host -mllvm -loopopt=1 -floopopt-pipeline=light -mllvm -intel-abi-compatible=true -o /tmp/icpx-6d4981/example-154035.o -x c++ /tmp/icpx-6d4981/example-525e6f.cpp
 "/usr/bin/ld" -z relro --hash-style=gnu --eh-frame-hdr -m elf_x86_64 -dynamic-linker /lib64/ld-linux-x86-64.so.2 -o "$4" /lib/x86_64-linux-gnu/Scrt1.o /lib/x86_64-linux-gnu/crti.o /usr/lib/gcc/x86_64-linux-gnu/11/crtbeginS.o -L/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../compiler/lib/intel64_lin -L/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib -L/opt/intel/oneapi/compiler/2023.1.0/linux/lib/oclfpga/host/linux64/lib -L/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../compiler/lib/intel64_lin -L/usr/lib/gcc/x86_64-linux-gnu/11 -L/usr/lib/gcc/x86_64-linux-gnu/11/../../../../lib64 -L/lib/x86_64-linux-gnu -L/lib/../lib64 -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib64 -L/usr/lib/gcc/x86_64-linux-gnu/11/../../.. -L/opt/intel/oneapi/compiler/2023.1.0/linux/bin-llvm/../lib -L/lib -L/usr/lib -L/opt/intel/oneapi/tbb/2021.9.0/env/../lib/intel64/gcc4.8 -L/opt/intel/oneapi/mpi/2021.9.0//libfabric/lib -L/opt/intel/oneapi/mpi/2021.9.0//lib/release -L/opt/intel/oneapi/mpi/2021.9.0//lib -L/opt/intel/oneapi/mkl/2023.1.0/lib/intel64 -L/opt/intel/oneapi/ippcp/2021.7.0/lib/intel64 -L/opt/intel/oneapi/ipp/2021.8.0/lib/intel64 -L/opt/intel/oneapi/dnnl/2023.1.0/cpu_dpcpp_gpu_dpcpp/lib -L/opt/intel/oneapi/dal/2023.1.0/lib/intel64 -L/opt/intel/oneapi/compiler/2023.1.0/linux/compiler/lib/intel64_lin -L/opt/intel/oneapi/compiler/2023.1.0/linux/lib -L/opt/intel/oneapi/ccl/2021.9.0/lib/cpu_gpu_dpcpp /tmp/icpx-6d4981/example-154035.o /tmp/icpx-6d4981/example-19399d.o -ldspba_mpir -ldspba_mpfr -lac_types_fixed_point_math_x86 -lac_types_vpfp_library -Bstatic -lsvml -Bdynamic -Bstatic -lirng -Bdynamic -lstdc++ -Bstatic -limf -Bdynamic -lm -lgcc_s -lgcc -Bstatic -lirc -Bdynamic -ldl -lgcc_s -lgcc -lsycl -lsycl-devicelib-host -lOpenCL -lc -lgcc_s -lgcc -Bstatic -lirc_s -Bdynamic /usr/lib/gcc/x86_64-linux-gnu/11/crtendS.o /lib/x86_64-linux-gnu/crtn.o
exit
fi

printf 'Warning: No target matched.'