#!/bin/bash

KERNEL=histogram_bram

export LSQ_SIZE=2
./elastic_pass.sh hw experiments/$KERNEL.cpp
unset LSQ_SIZE
export LSQ_SIZE=4
./elastic_pass.sh hw experiments/$KERNEL.cpp
unset LSQ_SIZE
export LSQ_SIZE=8
./elastic_pass.sh hw experiments/$KERNEL.cpp
unset LSQ_SIZE
export LSQ_SIZE=16
./elastic_pass.sh hw experiments/$KERNEL.cpp
unset LSQ_SIZE
export LSQ_SIZE=32
./elastic_pass.sh hw experiments/$KERNEL.cpp 
unset LSQ_SIZE
export LSQ_SIZE=64
./elastic_pass.sh hw experiments/$KERNEL.cpp
unset LSQ_SIZE
export LSQ_SIZE=128
./elastic_pass.sh hw experiments/$KERNEL.cpp
unset LSQ_SIZE
export LSQ_SIZE=256
./elastic_pass.sh hw experiments/$KERNEL.cpp

unset LSQ_SIZE

