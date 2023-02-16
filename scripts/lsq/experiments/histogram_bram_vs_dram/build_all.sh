#!/bin/bash


# $1  - target sim/hw

make fpga_$1 FILE=histogram_bram_cache.cpp II=0 # ii=0 means unspecified

make fpga_$1 FILE=histogram_bram_lsq.cpp II=1
make fpga_$1 FILE=histogram_bram_lsq.cpp II=2
make fpga_$1 FILE=histogram_bram_lsq.cpp II=4
make fpga_$1 FILE=histogram_bram_lsq.cpp II=8

make fpga_$1 FILE=histogram_bram.cpp II=0


make fpga_$1 FILE=histogram_dram_lsq.cpp II=1
make fpga_$1 FILE=histogram_dram_lsq.cpp II=2
make fpga_$1 FILE=histogram_dram_lsq.cpp II=4
make fpga_$1 FILE=histogram_dram_lsq.cpp II=8

make fpga_$1 FILE=histogram_dram.cpp II=0

