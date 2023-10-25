-- Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2019.2 (lin64) Build 2708876 Wed Nov  6 21:39:14 MST 2019
-- Date        : Fri Jun 23 13:46:44 2023
-- Host        : dynamatic-VirtualBox running 64-bit Ubuntu 18.04.3 LTS
-- Command     : write_vhdl -force -mode synth_stub -rename_top decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix -prefix
--               decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_ bd_0_hls_inst_0_stub.vhdl
-- Design      : bd_0_hls_inst_0
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xc7k160tfbg484-1
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix is
  Port ( 
    buffer_r_ce0 : out STD_LOGIC;
    buffer_r_we0 : out STD_LOGIC;
    buffer_r_ce1 : out STD_LOGIC;
    buffer_r_we1 : out STD_LOGIC;
    M_ce0 : out STD_LOGIC;
    M_ce1 : out STD_LOGIC;
    ap_clk : in STD_LOGIC;
    ap_rst : in STD_LOGIC;
    ap_start : in STD_LOGIC;
    ap_done : out STD_LOGIC;
    ap_idle : out STD_LOGIC;
    ap_ready : out STD_LOGIC;
    buffer_r_address0 : out STD_LOGIC_VECTOR ( 11 downto 0 );
    buffer_r_d0 : out STD_LOGIC_VECTOR ( 31 downto 0 );
    buffer_r_q0 : in STD_LOGIC_VECTOR ( 31 downto 0 );
    buffer_r_address1 : out STD_LOGIC_VECTOR ( 11 downto 0 );
    buffer_r_d1 : out STD_LOGIC_VECTOR ( 31 downto 0 );
    buffer_r_q1 : in STD_LOGIC_VECTOR ( 31 downto 0 );
    M_address0 : out STD_LOGIC_VECTOR ( 11 downto 0 );
    M_q0 : in STD_LOGIC_VECTOR ( 31 downto 0 );
    M_address1 : out STD_LOGIC_VECTOR ( 11 downto 0 );
    M_q1 : in STD_LOGIC_VECTOR ( 31 downto 0 )
  );

end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix;

architecture stub of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "buffer_r_ce0,buffer_r_we0,buffer_r_ce1,buffer_r_we1,M_ce0,M_ce1,ap_clk,ap_rst,ap_start,ap_done,ap_idle,ap_ready,buffer_r_address0[11:0],buffer_r_d0[31:0],buffer_r_q0[31:0],buffer_r_address1[11:0],buffer_r_d1[31:0],buffer_r_q1[31:0],M_address0[11:0],M_q0[31:0],M_address1[11:0],M_q1[31:0]";
attribute X_CORE_INFO : string;
attribute X_CORE_INFO of stub : architecture is "loop_imperfect,Vivado 2019.2";
begin
end;
