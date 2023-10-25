-- Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2019.2 (lin64) Build 2708876 Wed Nov  6 21:39:14 MST 2019
-- Date        : Wed Jul  5 16:28:03 2023
-- Host        : dynamatic-VirtualBox running 64-bit Ubuntu 18.04.3 LTS
-- Command     : write_vhdl -force -mode synth_stub
--               /home/dynamatic/floydWarshall/proj_loop_imperfect/solution1/impl/verilog/project.srcs/sources_1/bd/bd_0/ip/bd_0_hls_inst_0/bd_0_hls_inst_0_stub.vhdl
-- Design      : bd_0_hls_inst_0
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xc7k160tfbg484-1
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity bd_0_hls_inst_0 is
  Port ( 
    dist_ce0 : out STD_LOGIC;
    dist_ce1 : out STD_LOGIC;
    dist_we1 : out STD_LOGIC;
    ap_clk : in STD_LOGIC;
    ap_rst : in STD_LOGIC;
    ap_start : in STD_LOGIC;
    ap_done : out STD_LOGIC;
    ap_idle : out STD_LOGIC;
    ap_ready : out STD_LOGIC;
    dist_address0 : out STD_LOGIC_VECTOR ( 6 downto 0 );
    dist_q0 : in STD_LOGIC_VECTOR ( 31 downto 0 );
    dist_address1 : out STD_LOGIC_VECTOR ( 6 downto 0 );
    dist_d1 : out STD_LOGIC_VECTOR ( 31 downto 0 );
    dist_q1 : in STD_LOGIC_VECTOR ( 31 downto 0 )
  );

end bd_0_hls_inst_0;

architecture stub of bd_0_hls_inst_0 is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "dist_ce0,dist_ce1,dist_we1,ap_clk,ap_rst,ap_start,ap_done,ap_idle,ap_ready,dist_address0[6:0],dist_q0[31:0],dist_address1[6:0],dist_d1[31:0],dist_q1[31:0]";
attribute X_CORE_INFO : string;
attribute X_CORE_INFO of stub : architecture is "loop_imperfect,Vivado 2019.2";
begin
end;
