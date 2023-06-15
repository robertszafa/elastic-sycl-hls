#!/bin/bash

###
# Since SYCL 2023, a binary compiled for the simulator needs to be run from the
# directory where the binary resides. Otherwise, there an exception is thrown.
# https://github.com/oneapi-src/oneAPI-samples/issues/1515
###

DIR=`dirname "$1"`
BIN=`basename "$1"`

cd $DIR
CL_CONTEXT_MPSIM_DEVICE_INTELFPGA=1 ./"$BIN" "${@:2}"
"$ELASTIC_SYCL_HLS_DIR"/scripts/get_cycles.py "$BIN".prj
