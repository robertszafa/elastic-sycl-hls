#!/bin/bash


# $1  - target sim/hw

A_SIZE=10000
DATA_DISTR=1

GET_CYCLES="grep time | tr -dc '0-9' | cut -c 5-"


if [ "$1" == "sim" ]; then
  printf "\n\n--- BRAM\n"
  ./bin/histogram_bram_0.fpga_sim $A_SIZE $DATA_DISTR && printf "\n-- histogram_bram_lsq baseline:\n" && head simulation_raw.json | eval $GET_CYCLES

  ./bin/histogram_bram_cache_0.fpga_sim $A_SIZE $DATA_DISTR && printf "\n-- histogram_bram_cache_0:\n" && head simulation_raw.json |  eval $GET_CYCLES

  ./bin/histogram_bram_lsq_1.fpga_sim $A_SIZE $DATA_DISTR && printf "\n-- histogram_bram_lsq_1:\n" && head simulation_raw.json | eval $GET_CYCLES
  ./bin/histogram_bram_lsq_2.fpga_sim $A_SIZE $DATA_DISTR && printf "\n-- histogram_bram_lsq_2:\n" && head simulation_raw.json | eval $GET_CYCLES
  ./bin/histogram_bram_lsq_4.fpga_sim $A_SIZE $DATA_DISTR && printf "\n-- histogram_bram_lsq_4:\n" && head simulation_raw.json | eval $GET_CYCLES
  ./bin/histogram_bram_lsq_8.fpga_sim $A_SIZE $DATA_DISTR && printf "\n-- histogram_bram_lsq_8:\n" && head simulation_raw.json | eval $GET_CYCLES


  printf "\n\n--- DRAM\n"
  ./bin/histogram_dram_0.fpga_sim $A_SIZE $DATA_DISTR && printf "\n-- histogram_dram baseline:\n" && head simulation_raw.json | eval $GET_CYCLES

  ./bin/histogram_dram_lsq_1.fpga_sim $A_SIZE $DATA_DISTR && printf "\n-- histogram_bram_lsq_1:\n" && head simulation_raw.json | eval $GET_CYCLES
  ./bin/histogram_dram_lsq_2.fpga_sim $A_SIZE $DATA_DISTR && printf "\n-- histogram_bram_lsq_2:\n" && head simulation_raw.json | eval $GET_CYCLES
  ./bin/histogram_dram_lsq_4.fpga_sim $A_SIZE $DATA_DISTR && printf "\n-- histogram_bram_lsq_4:\n" && head simulation_raw.json | eval $GET_CYCLES
  ./bin/histogram_dram_lsq_8.fpga_sim $A_SIZE $DATA_DISTR && printf "\n-- histogram_bram_lsq_8:\n" && head simulation_raw.json | eval $GET_CYCLES

fi


if [ "$1" == "hw" ]; then
  printf "\n\n--- BRAM\n"
  printf "\n-- histogram_bram_lsq baseline:\n" 
  ./bin/histogram_bram_0.fpga_hw $A_SIZE $DATA_DISTR 

  printf "\n-- histogram_bram_cache_0:\n"  
  ./bin/histogram_bram_cache_0.fpga_hw $A_SIZE $DATA_DISTR 

  printf "\n-- histogram_bram_lsq_1:\n" 
  ./bin/histogram_bram_lsq_1.fpga_hw $A_SIZE $DATA_DISTR 
  printf "\n-- histogram_bram_lsq_2:\n" 
  ./bin/histogram_bram_lsq_2.fpga_hw $A_SIZE $DATA_DISTR 
  printf "\n-- histogram_bram_lsq_4:\n" 
  ./bin/histogram_bram_lsq_4.fpga_hw $A_SIZE $DATA_DISTR 
  printf "\n-- histogram_bram_lsq_8:\n" 
  ./bin/histogram_bram_lsq_8.fpga_hw $A_SIZE $DATA_DISTR 


  printf "\n\n--- DRAM\n"
  printf "\n-- histogram_dram baseline:\n" 
  ./bin/histogram_dram_0.fpga_hw $A_SIZE $DATA_DISTR 

  printf "\n-- histogram_bram_lsq_1:\n" 
  ./bin/histogram_dram_lsq_1.fpga_hw $A_SIZE $DATA_DISTR 
  printf "\n-- histogram_bram_lsq_2:\n" 
  ./bin/histogram_dram_lsq_2.fpga_hw $A_SIZE $DATA_DISTR 
  printf "\n-- histogram_bram_lsq_4:\n" 
  ./bin/histogram_dram_lsq_4.fpga_hw $A_SIZE $DATA_DISTR 
  printf "\n-- histogram_bram_lsq_8:\n" 
  ./bin/histogram_dram_lsq_8.fpga_hw $A_SIZE $DATA_DISTR 

fi

