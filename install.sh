#!/bin/bash

set -e

if [ x"${ELASTIC_SYCL_HLS_DIR}" == "x" ]; then
  echo "The {ELASTIC_SYCL_HLS_DIR} env variable is not set."
  exit
fi

CORES_FOR_MAKE=$((`nproc`-2))

echo "Building llvm/sycl from github.com/intel/llvm.git ..."
# The LLVM version should match the version used to build the SYCL compiler: 
# e.g. for intel-basekit-2023.1.0 the LLVM intel/sycl commit hash is 756ba26161. 
git clone https://github.com/intel/llvm.git && cd llvm && git checkout 756ba26161
mkdir -p $ELASTIC_SYCL_HLS_DIR/llvm/build && rm -rf $ELASTIC_SYCL_HLS_DIR/llvm/build/*
cd $ELASTIC_SYCL_HLS_DIR/llvm/build && \
cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_ASSERTIONS=ON -DLLVM_TARGETS_TO_BUILD=X86 \
      -DLLVM_EXTERNAL_PROJECTS="llvm-spirv;clang-tools-extra" \
      -DLLVM_ENABLE_PROJECTS="clang;llvm-spirv;clang-tools-extra" -DLLVM_BUILD_TOOLS=ON \
      -DLLVM_EXTERNAL_LLVM_SPIRV_SOURCE_DIR=$ELASTIC_SYCL_HLS_DIR/llvm/llvm-spirv\
      -DCMAKE_INSTALL_PREFIX=./install -DLLVM_ENABLE_DOXYGEN=OFF -DLLVM_ENABLE_SPHINX=OFF\
      -DBUILD_SHARED_LIBS=OFF -DLLVM_ENABLE_LLD=OFF $ELASTIC_SYCL_HLS_DIR/llvm/llvm
make -j $CORES_FOR_MAKE
echo "Finished building llvm."

echo "Building elastic-sycl-hls"
mkdir -p $ELASTIC_SYCL_HLS_DIR/build && rm -rf $ELASTIC_SYCL_HLS_DIR/build/*
cd $ELASTIC_SYCL_HLS_DIR/build && cmake .. && make -j $CORES_FOR_MAKE
cd $ELASTIC_SYCL_HLS_DIR
echo "Finished building elastic-sycl-hls."

# Our compiler uses latency-insensitive channels and BRAM IPs provided by Altera. 
# We change the channel write-to-read latency to 1 cycle, and we change the 
# read-during-write behaviour of dual-port BRAM to return "old data", i.e. 
# data before the store. The pipe change is purely for improved preformance, 
# the BRAM change is needed to gurantee correctness of our BRAM LSQ.
echo "Parameterizing Altera OpenCL IP (Pipes and BRAM)."
mkdir -p $ELASTIC_SYCL_HLS_DIR/ip
cp $INTELFPGAOCLSDKROOT/ip/acl_channel_fifo.v $ELASTIC_SYCL_HLS_DIR/ip
cp $INTELFPGAOCLSDKROOT/ip/acl_mem1x.v $ELASTIC_SYCL_HLS_DIR/ip
sed -i 's/RDW_MODE\s*==/"OLD_DATA"==/g;s/= RDW_MODE/= "OLD_DATA"/g' $ELASTIC_SYCL_HLS_DIR/ip/acl_mem1x.v
sed -i 's/INTER_KERNEL_PIPELINING/0/g;s/0 = 0/INTER_KERNEL_PIPELINING = 0/g;s/(ALLOW_HIGH_SPEED_FIFO_USAGE ? "hs" : "ms")/("ll")/g' $ELASTIC_SYCL_HLS_DIR/ip/acl_channel_fifo.v
