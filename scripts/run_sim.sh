#!/bin/bash

DIR=`dirname "$1"`
BIN=`basename "$1"`

cd $DIR
CL_CONTEXT_MPSIM_DEVICE_INTELFPGA=1 ./"$BIN" "${@:2}"
"$ELASTIC_SYCL_HLS_DIR"/scripts/get_cycles.py "$BIN".prj
