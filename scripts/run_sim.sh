#!/bin/bash

# $1 - emu / sim / hw
# $2 - filename to compile

DIR=`dirname "$1"`
BIN=`basename "$1"`

cd $DIR
CL_CONTEXT_MPSIM_DEVICE_INTELFPGA=1 ./"$BIN" "${@:2}"
/home/rob/git/llvm-sycl-passes/scripts/get_cycles.py "$BIN".prj
