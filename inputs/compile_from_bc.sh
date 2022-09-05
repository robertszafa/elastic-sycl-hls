# exit when any command fails
set -e

# Link with the (compiled) to .bc instantiation of store_q? At this point we should know the template params into store_q from the opt pass:
    # template <typename ld_idx_pipes, typename ld_val_pipes, int num_lds, typename T_idx_tag_pair,
    #           typename st_idx_pipe, typename st_val_pipe, typename end_signal_pipe, 
    #           bool FORWARD=true, int QUEUE_SIZE=8, int ST_LATENCY=12, typename T_val>
    # StoreQueue(queue &q, device_ptr<T_val> data) {
# 
# Also, amend the /tmp/ex1-header-587890.h to include any newly created kernels.
"/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/llvm-link" "$1" -o /tmp/ex1-38b379.bc

 "/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -inputs=/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/../lib/libsycl-crt.o -outputs=/tmp/libsycl-crt-0c174b.o -unbundle -allow-missing-bundles
 "/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -inputs=/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/../lib/libsycl-complex.o -outputs=/tmp/libsycl-complex-70d09b.o -unbundle -allow-missing-bundles
 "/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -inputs=/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/../lib/libsycl-complex-fp64.o -outputs=/tmp/libsycl-complex-fp64-c4a8f7.o -unbundle -allow-missing-bundles
 "/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -inputs=/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/../lib/libsycl-cmath.o -outputs=/tmp/libsycl-cmath-839ae8.o -unbundle -allow-missing-bundles
 "/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -inputs=/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/../lib/libsycl-cmath-fp64.o -outputs=/tmp/libsycl-cmath-fp64-710247.o -unbundle -allow-missing-bundles
 "/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -inputs=/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/../lib/libsycl-fallback-cassert.o -outputs=/tmp/libsycl-fallback-cassert-b43dc8.o -unbundle -allow-missing-bundles
 "/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -inputs=/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/../lib/libsycl-fallback-cstring.o -outputs=/tmp/libsycl-fallback-cstring-838541.o -unbundle -allow-missing-bundles
 "/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -inputs=/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/../lib/libsycl-fallback-complex.o -outputs=/tmp/libsycl-fallback-complex-106b85.o -unbundle -allow-missing-bundles
 "/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -inputs=/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/../lib/libsycl-fallback-complex-fp64.o -outputs=/tmp/libsycl-fallback-complex-fp64-209ca7.o -unbundle -allow-missing-bundles
 "/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -inputs=/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/../lib/libsycl-fallback-cmath.o -outputs=/tmp/libsycl-fallback-cmath-41843d.o -unbundle -allow-missing-bundles
 "/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/clang-offload-bundler" -type=o -targets=sycl-spir64_fpga-unknown-unknown -inputs=/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/../lib/libsycl-fallback-cmath-fp64.o -outputs=/tmp/libsycl-fallback-cmath-fp64-cf1fcd.o -unbundle -allow-missing-bundles
 "/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/llvm-link" -only-needed /tmp/ex1-38b379.bc /tmp/libsycl-crt-0c174b.o /tmp/libsycl-complex-70d09b.o /tmp/libsycl-complex-fp64-c4a8f7.o /tmp/libsycl-cmath-839ae8.o /tmp/libsycl-cmath-fp64-710247.o /tmp/libsycl-fallback-cassert-b43dc8.o /tmp/libsycl-fallback-cstring-838541.o /tmp/libsycl-fallback-complex-106b85.o /tmp/libsycl-fallback-complex-fp64-209ca7.o /tmp/libsycl-fallback-cmath-41843d.o /tmp/libsycl-fallback-cmath-fp64-cf1fcd.o -o /tmp/ex1-741095.bc --suppress-warnings

# Device bc  -o /tmp/ex1-741095.bc 

 "/opt/intel/oneapi/compiler/2022.1.0/linux/bin/sycl-post-link" -emit-only-kernels-as-entry-points -emit-param-info -symbols -emit-exported-symbols -split-esimd -lower-esimd -O2 -spec-const=default -o /tmp/ex1-1a575d.table /tmp/ex1-741095.bc
 "/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/file-table-tform" -extract=Code -drop_titles -o /tmp/ex1-d8d647.txt /tmp/ex1-1a575d.table
 "/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/llvm-foreach" --in-file-list=/tmp/ex1-d8d647.txt --in-replace=/tmp/ex1-d8d647.txt --out-ext=spv --out-file-list=/tmp/ex1-d895cc.txt --out-replace=/tmp/ex1-d895cc.txt -- /opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/llvm-spirv -o /tmp/ex1-d895cc.txt -spirv-max-version=1.3 -spirv-debug-info-version=ocl-100 -spirv-allow-extra-diexpressions -spirv-allow-unknown-intrinsics=llvm.genx. -spirv-ext=-all,+SPV_EXT_shader_atomic_float_add,+SPV_EXT_shader_atomic_float_min_max,+SPV_KHR_no_integer_wrap_decoration,+SPV_KHR_float_controls,+SPV_KHR_expect_assume,+SPV_INTEL_subgroups,+SPV_INTEL_media_block_io,+SPV_INTEL_device_side_avc_motion_estimation,+SPV_INTEL_fpga_loop_controls,+SPV_INTEL_fpga_memory_attributes,+SPV_INTEL_fpga_memory_accesses,+SPV_INTEL_unstructured_loop_controls,+SPV_INTEL_fpga_reg,+SPV_INTEL_blocking_pipes,+SPV_INTEL_function_pointers,+SPV_INTEL_kernel_attributes,+SPV_INTEL_io_pipes,+SPV_INTEL_inline_assembly,+SPV_INTEL_arbitrary_precision_integers,+SPV_INTEL_float_controls2,+SPV_INTEL_vector_compute,+SPV_INTEL_fast_composite,+SPV_INTEL_fpga_buffer_location,+SPV_INTEL_joint_matrix,+SPV_INTEL_arbitrary_precision_fixed_point,+SPV_INTEL_arbitrary_precision_floating_point,+SPV_INTEL_arbitrary_precision_floating_point,+SPV_INTEL_variable_length_array,+SPV_INTEL_fp_fast_math_mode,+SPV_INTEL_fpga_cluster_attributes,+SPV_INTEL_loop_fuse,+SPV_INTEL_long_constant_composite,+SPV_INTEL_fpga_invocation_pipelining_attributes,+SPV_INTEL_fpga_dsp_control,+SPV_INTEL_arithmetic_fence,+SPV_INTEL_runtime_aligned,+SPV_INTEL_task_sequence,+SPV_INTEL_optnone,+SPV_INTEL_token_type,+SPV_INTEL_bfloat16_conversion,+SPV_INTEL_joint_matrix,+SPV_INTEL_hw_thread_queries,+SPV_INTEL_memory_access_aliasing /tmp/ex1-d8d647.txt
 "/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/llvm-foreach" --out-ext=aocx --in-file-list=/tmp/ex1-d895cc.txt --in-replace=/tmp/ex1-d895cc.txt --out-file-list=/tmp/ex1-28b487.aocx --out-replace=/tmp/ex1-28b487.aocx -- /opt/intel/oneapi/compiler/2022.1.0/linux/bin/opencl-aot -device=fpga_fast_emu -spv=/tmp/ex1-d895cc.txt -ir=/tmp/ex1-28b487.aocx --bo=-g
 "/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/file-table-tform" -replace=Code,Code -o /tmp/ex1-1fb2f5.table /tmp/ex1-1a575d.table /tmp/ex1-28b487.aocx
 "/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/clang-offload-wrapper" -o=/tmp/wrapper-95d6b1.bc -host=x86_64-unknown-linux-gnu -target=spir64_fpga -kind=sycl -batch /tmp/ex1-1fb2f5.table
 "/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/llc" -filetype=obj -o /tmp/ex1-71fc85.o /tmp/wrapper-95d6b1.bc


# # This is where we need to inject kernel calls:
 "/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/append-file" ex1/ex1.cpp --append=/tmp/ex1-footer-c298eb.h --orig-filename=ex1/ex1.cpp --output=/tmp/ex1-e897e5.cpp --use-include


# #  TODO: how to inject opt pass here??

 "/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/clang++" \
-cc1 \
-triple x86_64-unknown-linux-gnu \
-sycl-std=2020 \
-fsycl-unnamed-lambda \
-fsycl-unique-prefix=ff49426ca9aefeed \
-fsycl-disable-range-rounding \
-include /tmp/ex1-header-587890.h \
-dependency-filter /tmp/ex1-header-587890.h \
-fsycl-enable-int-header-diags \
-fsycl-is-host \
-emit-obj \
--mrelax-relocations \
-disable-free \
-clear-ast-before-backend \
-disable-llvm-verifier \
-discard-value-names \
-mrelocation-model static \
-fveclib=SVML \
-mframe-pointer=none \
-menable-no-infs \
-menable-no-nans \
-menable-unsafe-fp-math \
-fno-signed-zeros \
-mreassociate \
-freciprocal-math \
-fdenormal-fp-math=preserve-sign,preserve-sign \
-ffp-contract=fast \
-fno-rounding-math \
-ffast-math \
-ffinite-math-only \
-mconstructor-aliases \
-funwind-tables=2 \
-target-cpu x86-64 \
-mllvm \
-x86-enable-unaligned-vector-move=true \
-tune-cpu generic \
-debug-info-kind=limited \
-dwarf-version=4 \
-debugger-tuning=gdb \
-fcoverage-compilation-dir=/home/rob/git/llvm-sycl-passes/inputs/ex1 \
-resource-dir /opt/intel/oneapi/compiler/2022.1.0/linux/lib/clang/14.0.0 \
-internal-isystem /opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/../include/sycl \
-internal-isystem /opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/../include \
-iquote /home/rob/git/llvm-sycl-passes/inputs/ex1 \
-D FPGA_EMULATOR=1 -I/opt/intel/oneapi/vpl/2022.1.0/include  -I/opt/intel/oneapi/tbb/2021.6.0/env/../include  -I/opt/intel/oneapi/mpi/2021.6.0//include  -I/opt/intel/oneapi/mkl/2022.1.0/include  -I/opt/intel/oneapi/ippcp/2021.6.0/include  -I/opt/intel/oneapi/ipp/2021.6.0/include  -I/opt/intel/oneapi/dpl/2021.7.0/linux/include  -I/opt/intel/oneapi/dpcpp-ct/2022.1.0/include  -I/opt/intel/oneapi/dnnl/2022.1.0/cpu_dpcpp_gpu_dpcpp/include  -I/opt/intel/oneapi/dev-utilities/2021.6.0/include  -I/opt/intel/oneapi/dal/2021.6.0/include  -I/opt/intel/oneapi/ccl/2021.6.0/include/cpu_gpu_dpcpp  -internal-isystem /opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/../compiler/include  -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9  -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9  -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/backward  -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9  -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9  -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/backward  -internal-isystem /opt/intel/oneapi/compiler/2022.1.0/linux/lib/clang/14.0.0/include  -internal-isystem /usr/local/include  -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include  -internal-externc-isystem /usr/include/x86_64-linux-gnu  -internal-externc-isystem /include  -internal-externc-isystem /usr/include  -internal-isystem /opt/intel/oneapi/compiler/2022.1.0/linux/lib/clang/14.0.0/include  -internal-isystem /usr/local/include  -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include  -internal-externc-isystem /usr/include/x86_64-linux-gnu  -internal-externc-isystem /include  -internal-externc-isystem /usr/include \
-O2 \
-std=c++17 \
-fdeprecated-macro \
-fdebug-compilation-dir=/home/rob/git/llvm-sycl-passes/inputs/ex1 \
-ferror-limit 19 \
-fheinous-gnu-extensions \
-fgnuc-version=4.2.1 \
-fcxx-exceptions \
-fexceptions \
-fcolor-diagnostics \
-vectorize-loops \
-vectorize-slp \
-D__GCC_HAVE_DWARF2_CFI_ASM=1 \
-fintel-compatibility \
-mllvm \
-disable-hir-generate-mkl-call \
-mllvm \
-intel-libirc-allowed \
-mllvm \
-loopopt=0 \
-floopopt-pipeline=none \
-mllvm \
-enable-lv \
-o /tmp/ex1-30ef46.o \
-x c++ /tmp/ex1-e897e5.cpp
  

 "/usr/bin/ld" -z relro --hash-style=gnu --eh-frame-hdr -m elf_x86_64 -dynamic-linker /lib64/ld-linux-x86-64.so.2 -o ex1/ex1.fpga_emu /lib/x86_64-linux-gnu/crt1.o /lib/x86_64-linux-gnu/crti.o /usr/lib/gcc/x86_64-linux-gnu/9/crtbegin.o -L/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/../compiler/lib/intel64_lin -L/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/../lib -L/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/../compiler/lib/intel64_lin -L/usr/lib/gcc/x86_64-linux-gnu/9 -L/usr/lib/gcc/x86_64-linux-gnu/9/../../../../lib64 -L/lib/x86_64-linux-gnu -L/lib/../lib64 -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib64 -L/usr/lib/gcc/x86_64-linux-gnu/9/../../.. -L/opt/intel/oneapi/compiler/2022.1.0/linux/bin-llvm/../lib -L/lib -L/usr/lib -L/opt/intel/oneapi/vpl/2022.1.0/lib -L/opt/intel/oneapi/tbb/2021.6.0/env/../lib/intel64/gcc4.8 -L/opt/intel/oneapi/mpi/2021.6.0//libfabric/lib -L/opt/intel/oneapi/mpi/2021.6.0//lib/release -L/opt/intel/oneapi/mpi/2021.6.0//lib -L/opt/intel/oneapi/mkl/2022.1.0/lib/intel64 -L/opt/intel/oneapi/ippcp/2021.6.0/lib/intel64 -L/opt/intel/oneapi/ipp/2021.6.0/lib/intel64 -L/opt/intel/oneapi/dnnl/2022.1.0/cpu_dpcpp_gpu_dpcpp/lib -L/opt/intel/oneapi/dal/2021.6.0/lib/intel64 -L/opt/intel/oneapi/compiler/2022.1.0/linux/compiler/lib/intel64_lin -L/opt/intel/oneapi/compiler/2022.1.0/linux/lib -L/opt/intel/oneapi/ccl/2021.6.0/lib/cpu_gpu_dpcpp /tmp/ex1-30ef46.o /tmp/ex1-71fc85.o -Bstatic -lsvml -Bdynamic -Bstatic -lirng -Bdynamic -lstdc++ -Bstatic -limf -Bdynamic -lm -lgcc_s -lgcc -Bstatic -lirc -Bdynamic -ldl -lgcc_s -lgcc -lsycl -lOpenCL -lc -lgcc_s -lgcc -Bstatic -lirc_s -Bdynamic /usr/lib/gcc/x86_64-linux-gnu/9/crtend.o /lib/x86_64-linux-gnu/crtn.o

echo "done"