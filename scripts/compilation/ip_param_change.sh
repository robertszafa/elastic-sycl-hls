#!/bin/bash

# This script changes a few parameters in the Intel OpenCL SDK FPGA IP.
# It does so for the intermediate files stored in $TMP_DIR/example. The original
# $INTELFPGASDKROOT/ip files are left unchanged.
# 
# We change the SYCL pipe write-to-read latency and the read-during-write 
# behaviour of BRAM.


TMP_DIR=${TMPDIR:-${TEMP:-${TMP:-/tmp}}}

until [ -f "$TMP_DIR/example-*/ip/acl_mem_1x.v" ]
do
  # Change the behaviour of read during write in bram to get the old data.
  sed -i 's/RDW_MODE/"DONT_CARE"/g;s/"DONT_CARE" = "DONT_CARE"/RDW_MODE = "DONT_CARE"/g' $TMP_DIR/example-*/ip/acl_mem_1x.v  > /dev/null  2>&1

  # No inter kernel pipelining when using acl channels (a.k.a. SYCL pipes.)
  # Change write-to-read latency to 1 cycles, i.e. use "ll" FIFO style. 
  sed -i 's/INTER_KERNEL_PIPELINING/0/g;s/0 = 0/INTER_KERNEL_PIPELINING = 0/g;s/(ALLOW_HIGH_SPEED_FIFO_USAGE ? "hs" : "ms")/("ll")/g' $TMP_DIR/example-*/ip/acl_channel_fifo.v  > /dev/null  2>&1
done
