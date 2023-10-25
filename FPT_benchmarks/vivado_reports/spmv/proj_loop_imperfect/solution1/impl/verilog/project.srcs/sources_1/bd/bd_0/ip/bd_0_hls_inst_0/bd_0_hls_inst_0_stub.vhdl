-- Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2019.2 (lin64) Build 2708876 Wed Nov  6 21:39:14 MST 2019
-- Date        : Sun Jun 25 13:38:16 2023
-- Host        : dynamatic-VirtualBox running 64-bit Ubuntu 18.04.3 LTS
-- Command     : write_vhdl -force -mode synth_stub
--               /home/dynamatic/spmv/proj_loop_imperfect/solution1/impl/verilog/project.srcs/sources_1/bd/bd_0/ip/bd_0_hls_inst_0/bd_0_hls_inst_0_stub.vhdl
-- Design      : bd_0_hls_inst_0
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xc7k160tfbg484-1
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity bd_0_hls_inst_0 is
  Port ( 
    matrix_ce0 : out STD_LOGIC;
    matrix_we0 : out STD_LOGIC;
    matrix_ce1 : out STD_LOGIC;
    matrix_we1 : out STD_LOGIC;
    row_ce0 : out STD_LOGIC;
    row_ce1 : out STD_LOGIC;
    col_ce0 : out STD_LOGIC;
    col_ce1 : out STD_LOGIC;
    a_ce0 : out STD_LOGIC;
    a_ce1 : out STD_LOGIC;
    ap_clk : in STD_LOGIC;
    ap_rst : in STD_LOGIC;
    ap_start : in STD_LOGIC;
    ap_done : out STD_LOGIC;
    ap_idle : out STD_LOGIC;
    ap_ready : out STD_LOGIC;
    matrix_address0 : out STD_LOGIC_VECTOR ( 8 downto 0 );
    matrix_d0 : out STD_LOGIC_VECTOR ( 31 downto 0 );
    matrix_q0 : in STD_LOGIC_VECTOR ( 31 downto 0 );
    matrix_address1 : out STD_LOGIC_VECTOR ( 8 downto 0 );
    matrix_d1 : out STD_LOGIC_VECTOR ( 31 downto 0 );
    matrix_q1 : in STD_LOGIC_VECTOR ( 31 downto 0 );
    row_address0 : out STD_LOGIC_VECTOR ( 8 downto 0 );
    row_q0 : in STD_LOGIC_VECTOR ( 31 downto 0 );
    row_address1 : out STD_LOGIC_VECTOR ( 8 downto 0 );
    row_q1 : in STD_LOGIC_VECTOR ( 31 downto 0 );
    col_address0 : out STD_LOGIC_VECTOR ( 8 downto 0 );
    col_q0 : in STD_LOGIC_VECTOR ( 31 downto 0 );
    col_address1 : out STD_LOGIC_VECTOR ( 8 downto 0 );
    col_q1 : in STD_LOGIC_VECTOR ( 31 downto 0 );
    a_address0 : out STD_LOGIC_VECTOR ( 8 downto 0 );
    a_q0 : in STD_LOGIC_VECTOR ( 31 downto 0 );
    a_address1 : out STD_LOGIC_VECTOR ( 8 downto 0 );
    a_q1 : in STD_LOGIC_VECTOR ( 31 downto 0 )
  );

end bd_0_hls_inst_0;

architecture stub of bd_0_hls_inst_0 is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "matrix_ce0,matrix_we0,matrix_ce1,matrix_we1,row_ce0,row_ce1,col_ce0,col_ce1,a_ce0,a_ce1,ap_clk,ap_rst,ap_start,ap_done,ap_idle,ap_ready,matrix_address0[8:0],matrix_d0[31:0],matrix_q0[31:0],matrix_address1[8:0],matrix_d1[31:0],matrix_q1[31:0],row_address0[8:0],row_q0[31:0],row_address1[8:0],row_q1[31:0],col_address0[8:0],col_q0[31:0],col_address1[8:0],col_q1[31:0],a_address0[8:0],a_q0[31:0],a_address1[8:0],a_q1[31:0]";
attribute X_CORE_INFO : string;
attribute X_CORE_INFO of stub : architecture is "loop_imperfect,Vivado 2019.2";
begin
end;
