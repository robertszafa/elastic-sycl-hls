-- Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2019.2 (lin64) Build 2708876 Wed Nov  6 21:39:14 MST 2019
-- Date        : Thu Jul  6 15:28:56 2023
-- Host        : dynamatic-VirtualBox running 64-bit Ubuntu 18.04.3 LTS
-- Command     : write_vhdl -force -mode funcsim -rename_top decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix -prefix
--               decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_ bd_0_hls_inst_0_sim_netlist.vhdl
-- Design      : bd_0_hls_inst_0
-- Purpose     : This VHDL netlist is a functional simulation representation of the design and should not be modified or
--               synthesized. This netlist cannot be used for SDF annotated simulation.
-- Device      : xc7k160tfbg484-1
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect is
  port (
    ap_clk : in STD_LOGIC;
    ap_rst : in STD_LOGIC;
    ap_start : in STD_LOGIC;
    ap_done : out STD_LOGIC;
    ap_idle : out STD_LOGIC;
    ap_ready : out STD_LOGIC;
    A_address0 : out STD_LOGIC_VECTOR ( 9 downto 0 );
    A_ce0 : out STD_LOGIC;
    A_q0 : in STD_LOGIC_VECTOR ( 31 downto 0 );
    hist_address0 : out STD_LOGIC_VECTOR ( 9 downto 0 );
    hist_ce0 : out STD_LOGIC;
    hist_we0 : out STD_LOGIC;
    hist_d0 : out STD_LOGIC_VECTOR ( 31 downto 0 );
    hist_q0 : in STD_LOGIC_VECTOR ( 31 downto 0 );
    C_address0 : out STD_LOGIC_VECTOR ( 9 downto 0 );
    C_ce0 : out STD_LOGIC;
    C_q0 : in STD_LOGIC_VECTOR ( 31 downto 0 )
  );
  attribute ap_ST_fsm_pp0_stage0 : string;
  attribute ap_ST_fsm_pp0_stage0 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is "4'b0010";
  attribute ap_ST_fsm_pp0_stage1 : string;
  attribute ap_ST_fsm_pp0_stage1 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is "4'b0100";
  attribute ap_ST_fsm_state1 : string;
  attribute ap_ST_fsm_state1 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is "4'b0001";
  attribute ap_ST_fsm_state6 : string;
  attribute ap_ST_fsm_state6 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is "4'b1000";
  attribute hls_module : string;
  attribute hls_module of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is "yes";
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect is
  signal \^c_address0\ : STD_LOGIC_VECTOR ( 9 downto 0 );
  signal \^c_ce0\ : STD_LOGIC;
  signal \ap_CS_fsm[2]_i_2_n_1\ : STD_LOGIC;
  signal \ap_CS_fsm[2]_i_3_n_1\ : STD_LOGIC;
  signal \ap_CS_fsm[3]_i_2_n_1\ : STD_LOGIC;
  signal \ap_CS_fsm[3]_i_3_n_1\ : STD_LOGIC;
  signal \ap_CS_fsm[3]_i_4_n_1\ : STD_LOGIC;
  signal ap_CS_fsm_pp0_stage0 : STD_LOGIC;
  signal ap_CS_fsm_pp0_stage1 : STD_LOGIC;
  signal \ap_CS_fsm_reg_n_1_[0]\ : STD_LOGIC;
  signal ap_NS_fsm : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal ap_NS_fsm14_out : STD_LOGIC;
  signal \^ap_done\ : STD_LOGIC;
  signal ap_enable_reg_pp0_iter0 : STD_LOGIC;
  signal ap_enable_reg_pp0_iter0_i_1_n_1 : STD_LOGIC;
  signal ap_enable_reg_pp0_iter1_i_1_n_1 : STD_LOGIC;
  signal ap_enable_reg_pp0_iter1_reg_n_1 : STD_LOGIC;
  signal ap_phi_mux_i_0_phi_fu_84_p41 : STD_LOGIC;
  signal hist_addr_reg_1500 : STD_LOGIC;
  signal \hist_d0[0]_INST_0_i_1_n_1\ : STD_LOGIC;
  signal \hist_d0[0]_INST_0_i_2_n_1\ : STD_LOGIC;
  signal \hist_d0[0]_INST_0_i_3_n_1\ : STD_LOGIC;
  signal \hist_d0[0]_INST_0_i_4_n_1\ : STD_LOGIC;
  signal \hist_d0[0]_INST_0_n_1\ : STD_LOGIC;
  signal \hist_d0[0]_INST_0_n_2\ : STD_LOGIC;
  signal \hist_d0[0]_INST_0_n_3\ : STD_LOGIC;
  signal \hist_d0[0]_INST_0_n_4\ : STD_LOGIC;
  signal \hist_d0[12]_INST_0_i_1_n_1\ : STD_LOGIC;
  signal \hist_d0[12]_INST_0_i_2_n_1\ : STD_LOGIC;
  signal \hist_d0[12]_INST_0_i_3_n_1\ : STD_LOGIC;
  signal \hist_d0[12]_INST_0_i_4_n_1\ : STD_LOGIC;
  signal \hist_d0[12]_INST_0_n_1\ : STD_LOGIC;
  signal \hist_d0[12]_INST_0_n_2\ : STD_LOGIC;
  signal \hist_d0[12]_INST_0_n_3\ : STD_LOGIC;
  signal \hist_d0[12]_INST_0_n_4\ : STD_LOGIC;
  signal \hist_d0[16]_INST_0_i_1_n_1\ : STD_LOGIC;
  signal \hist_d0[16]_INST_0_i_2_n_1\ : STD_LOGIC;
  signal \hist_d0[16]_INST_0_i_3_n_1\ : STD_LOGIC;
  signal \hist_d0[16]_INST_0_i_4_n_1\ : STD_LOGIC;
  signal \hist_d0[16]_INST_0_n_1\ : STD_LOGIC;
  signal \hist_d0[16]_INST_0_n_2\ : STD_LOGIC;
  signal \hist_d0[16]_INST_0_n_3\ : STD_LOGIC;
  signal \hist_d0[16]_INST_0_n_4\ : STD_LOGIC;
  signal \hist_d0[20]_INST_0_i_1_n_1\ : STD_LOGIC;
  signal \hist_d0[20]_INST_0_i_2_n_1\ : STD_LOGIC;
  signal \hist_d0[20]_INST_0_i_3_n_1\ : STD_LOGIC;
  signal \hist_d0[20]_INST_0_i_4_n_1\ : STD_LOGIC;
  signal \hist_d0[20]_INST_0_n_1\ : STD_LOGIC;
  signal \hist_d0[20]_INST_0_n_2\ : STD_LOGIC;
  signal \hist_d0[20]_INST_0_n_3\ : STD_LOGIC;
  signal \hist_d0[20]_INST_0_n_4\ : STD_LOGIC;
  signal \hist_d0[24]_INST_0_i_1_n_1\ : STD_LOGIC;
  signal \hist_d0[24]_INST_0_i_2_n_1\ : STD_LOGIC;
  signal \hist_d0[24]_INST_0_i_3_n_1\ : STD_LOGIC;
  signal \hist_d0[24]_INST_0_i_4_n_1\ : STD_LOGIC;
  signal \hist_d0[24]_INST_0_n_1\ : STD_LOGIC;
  signal \hist_d0[24]_INST_0_n_2\ : STD_LOGIC;
  signal \hist_d0[24]_INST_0_n_3\ : STD_LOGIC;
  signal \hist_d0[24]_INST_0_n_4\ : STD_LOGIC;
  signal \hist_d0[28]_INST_0_i_1_n_1\ : STD_LOGIC;
  signal \hist_d0[28]_INST_0_i_2_n_1\ : STD_LOGIC;
  signal \hist_d0[28]_INST_0_i_3_n_1\ : STD_LOGIC;
  signal \hist_d0[28]_INST_0_i_4_n_1\ : STD_LOGIC;
  signal \hist_d0[28]_INST_0_n_2\ : STD_LOGIC;
  signal \hist_d0[28]_INST_0_n_3\ : STD_LOGIC;
  signal \hist_d0[28]_INST_0_n_4\ : STD_LOGIC;
  signal \hist_d0[4]_INST_0_i_1_n_1\ : STD_LOGIC;
  signal \hist_d0[4]_INST_0_i_2_n_1\ : STD_LOGIC;
  signal \hist_d0[4]_INST_0_i_3_n_1\ : STD_LOGIC;
  signal \hist_d0[4]_INST_0_i_4_n_1\ : STD_LOGIC;
  signal \hist_d0[4]_INST_0_n_1\ : STD_LOGIC;
  signal \hist_d0[4]_INST_0_n_2\ : STD_LOGIC;
  signal \hist_d0[4]_INST_0_n_3\ : STD_LOGIC;
  signal \hist_d0[4]_INST_0_n_4\ : STD_LOGIC;
  signal \hist_d0[8]_INST_0_i_1_n_1\ : STD_LOGIC;
  signal \hist_d0[8]_INST_0_i_2_n_1\ : STD_LOGIC;
  signal \hist_d0[8]_INST_0_i_3_n_1\ : STD_LOGIC;
  signal \hist_d0[8]_INST_0_i_4_n_1\ : STD_LOGIC;
  signal \hist_d0[8]_INST_0_n_1\ : STD_LOGIC;
  signal \hist_d0[8]_INST_0_n_2\ : STD_LOGIC;
  signal \hist_d0[8]_INST_0_n_3\ : STD_LOGIC;
  signal \hist_d0[8]_INST_0_n_4\ : STD_LOGIC;
  signal hist_we0_INST_0_i_10_n_1 : STD_LOGIC;
  signal hist_we0_INST_0_i_11_n_1 : STD_LOGIC;
  signal hist_we0_INST_0_i_11_n_2 : STD_LOGIC;
  signal hist_we0_INST_0_i_11_n_3 : STD_LOGIC;
  signal hist_we0_INST_0_i_11_n_4 : STD_LOGIC;
  signal hist_we0_INST_0_i_12_n_1 : STD_LOGIC;
  signal hist_we0_INST_0_i_13_n_1 : STD_LOGIC;
  signal hist_we0_INST_0_i_14_n_1 : STD_LOGIC;
  signal hist_we0_INST_0_i_15_n_1 : STD_LOGIC;
  signal hist_we0_INST_0_i_16_n_1 : STD_LOGIC;
  signal hist_we0_INST_0_i_17_n_1 : STD_LOGIC;
  signal hist_we0_INST_0_i_18_n_1 : STD_LOGIC;
  signal hist_we0_INST_0_i_19_n_1 : STD_LOGIC;
  signal hist_we0_INST_0_i_1_n_2 : STD_LOGIC;
  signal hist_we0_INST_0_i_1_n_3 : STD_LOGIC;
  signal hist_we0_INST_0_i_1_n_4 : STD_LOGIC;
  signal hist_we0_INST_0_i_20_n_1 : STD_LOGIC;
  signal hist_we0_INST_0_i_20_n_2 : STD_LOGIC;
  signal hist_we0_INST_0_i_20_n_3 : STD_LOGIC;
  signal hist_we0_INST_0_i_20_n_4 : STD_LOGIC;
  signal hist_we0_INST_0_i_21_n_1 : STD_LOGIC;
  signal hist_we0_INST_0_i_22_n_1 : STD_LOGIC;
  signal hist_we0_INST_0_i_23_n_1 : STD_LOGIC;
  signal hist_we0_INST_0_i_24_n_1 : STD_LOGIC;
  signal hist_we0_INST_0_i_25_n_1 : STD_LOGIC;
  signal hist_we0_INST_0_i_26_n_1 : STD_LOGIC;
  signal hist_we0_INST_0_i_27_n_1 : STD_LOGIC;
  signal hist_we0_INST_0_i_28_n_1 : STD_LOGIC;
  signal hist_we0_INST_0_i_29_n_1 : STD_LOGIC;
  signal hist_we0_INST_0_i_2_n_1 : STD_LOGIC;
  signal hist_we0_INST_0_i_2_n_2 : STD_LOGIC;
  signal hist_we0_INST_0_i_2_n_3 : STD_LOGIC;
  signal hist_we0_INST_0_i_2_n_4 : STD_LOGIC;
  signal hist_we0_INST_0_i_30_n_1 : STD_LOGIC;
  signal hist_we0_INST_0_i_31_n_1 : STD_LOGIC;
  signal hist_we0_INST_0_i_32_n_1 : STD_LOGIC;
  signal hist_we0_INST_0_i_33_n_1 : STD_LOGIC;
  signal hist_we0_INST_0_i_34_n_1 : STD_LOGIC;
  signal hist_we0_INST_0_i_35_n_1 : STD_LOGIC;
  signal hist_we0_INST_0_i_36_n_1 : STD_LOGIC;
  signal hist_we0_INST_0_i_3_n_1 : STD_LOGIC;
  signal hist_we0_INST_0_i_4_n_1 : STD_LOGIC;
  signal hist_we0_INST_0_i_5_n_1 : STD_LOGIC;
  signal hist_we0_INST_0_i_6_n_1 : STD_LOGIC;
  signal hist_we0_INST_0_i_7_n_1 : STD_LOGIC;
  signal hist_we0_INST_0_i_8_n_1 : STD_LOGIC;
  signal hist_we0_INST_0_i_9_n_1 : STD_LOGIC;
  signal i_0_reg_80 : STD_LOGIC;
  signal \i_0_reg_80_reg_n_1_[0]\ : STD_LOGIC;
  signal \i_0_reg_80_reg_n_1_[1]\ : STD_LOGIC;
  signal \i_0_reg_80_reg_n_1_[2]\ : STD_LOGIC;
  signal \i_0_reg_80_reg_n_1_[3]\ : STD_LOGIC;
  signal \i_0_reg_80_reg_n_1_[4]\ : STD_LOGIC;
  signal \i_0_reg_80_reg_n_1_[5]\ : STD_LOGIC;
  signal \i_0_reg_80_reg_n_1_[6]\ : STD_LOGIC;
  signal \i_0_reg_80_reg_n_1_[7]\ : STD_LOGIC;
  signal \i_0_reg_80_reg_n_1_[8]\ : STD_LOGIC;
  signal \i_0_reg_80_reg_n_1_[9]\ : STD_LOGIC;
  signal i_fu_97_p2 : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal \i_reg_130[4]_i_2_n_1\ : STD_LOGIC;
  signal \i_reg_130[5]_i_2_n_1\ : STD_LOGIC;
  signal \i_reg_130[6]_i_1_n_1\ : STD_LOGIC;
  signal \i_reg_130[7]_i_2_n_1\ : STD_LOGIC;
  signal \i_reg_130[8]_i_1_n_1\ : STD_LOGIC;
  signal \i_reg_130[8]_i_2_n_1\ : STD_LOGIC;
  signal \i_reg_130[9]_i_1_n_1\ : STD_LOGIC;
  signal \i_reg_130[9]_i_2_n_1\ : STD_LOGIC;
  signal \i_reg_130[9]_i_3_n_1\ : STD_LOGIC;
  signal \i_reg_130[9]_i_4_n_1\ : STD_LOGIC;
  signal \i_reg_130[9]_i_5_n_1\ : STD_LOGIC;
  signal i_reg_130_reg : STD_LOGIC_VECTOR ( 9 downto 0 );
  signal icmp_ln102_fu_114_p2 : STD_LOGIC;
  signal icmp_ln97_reg_126 : STD_LOGIC;
  signal \icmp_ln97_reg_126[0]_i_1_n_1\ : STD_LOGIC;
  signal wt_reg_145 : STD_LOGIC_VECTOR ( 31 downto 0 );
  signal \NLW_hist_d0[28]_INST_0_CO_UNCONNECTED\ : STD_LOGIC_VECTOR ( 3 to 3 );
  signal NLW_hist_we0_INST_0_i_1_O_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_hist_we0_INST_0_i_11_O_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_hist_we0_INST_0_i_2_O_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_hist_we0_INST_0_i_20_O_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  attribute SOFT_HLUTNM : string;
  attribute SOFT_HLUTNM of \C_address0[0]_INST_0\ : label is "soft_lutpair3";
  attribute SOFT_HLUTNM of \C_address0[3]_INST_0\ : label is "soft_lutpair0";
  attribute SOFT_HLUTNM of \C_address0[5]_INST_0\ : label is "soft_lutpair1";
  attribute SOFT_HLUTNM of \C_address0[6]_INST_0\ : label is "soft_lutpair2";
  attribute SOFT_HLUTNM of \C_address0[7]_INST_0\ : label is "soft_lutpair6";
  attribute SOFT_HLUTNM of \C_address0[8]_INST_0\ : label is "soft_lutpair4";
  attribute SOFT_HLUTNM of \C_address0[9]_INST_0\ : label is "soft_lutpair5";
  attribute SOFT_HLUTNM of \ap_CS_fsm[0]_i_1\ : label is "soft_lutpair9";
  attribute SOFT_HLUTNM of \ap_CS_fsm[1]_i_1\ : label is "soft_lutpair7";
  attribute SOFT_HLUTNM of \ap_CS_fsm[3]_i_4\ : label is "soft_lutpair2";
  attribute FSM_ENCODING : string;
  attribute FSM_ENCODING of \ap_CS_fsm_reg[0]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[1]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[2]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[3]\ : label is "none";
  attribute SOFT_HLUTNM of ap_enable_reg_pp0_iter0_i_2 : label is "soft_lutpair7";
  attribute SOFT_HLUTNM of ap_idle_INST_0 : label is "soft_lutpair9";
  attribute SOFT_HLUTNM of hist_ce0_INST_0 : label is "soft_lutpair8";
  attribute SOFT_HLUTNM of hist_we0_INST_0 : label is "soft_lutpair8";
  attribute SOFT_HLUTNM of \i_reg_130[0]_i_1\ : label is "soft_lutpair3";
  attribute SOFT_HLUTNM of \i_reg_130[7]_i_2\ : label is "soft_lutpair1";
  attribute SOFT_HLUTNM of \i_reg_130[8]_i_2\ : label is "soft_lutpair0";
  attribute SOFT_HLUTNM of \i_reg_130[9]_i_2\ : label is "soft_lutpair5";
  attribute SOFT_HLUTNM of \i_reg_130[9]_i_3\ : label is "soft_lutpair4";
  attribute SOFT_HLUTNM of \i_reg_130[9]_i_4\ : label is "soft_lutpair6";
begin
  A_address0(9 downto 0) <= \^c_address0\(9 downto 0);
  A_ce0 <= \^c_ce0\;
  C_address0(9 downto 0) <= \^c_address0\(9 downto 0);
  C_ce0 <= \^c_ce0\;
  ap_done <= \^ap_done\;
  ap_ready <= \^ap_done\;
\C_address0[0]_INST_0\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"BAAA8AAA"
    )
        port map (
      I0 => \i_0_reg_80_reg_n_1_[0]\,
      I1 => icmp_ln97_reg_126,
      I2 => ap_CS_fsm_pp0_stage0,
      I3 => ap_enable_reg_pp0_iter1_reg_n_1,
      I4 => i_reg_130_reg(0),
      O => \^c_address0\(0)
    );
\C_address0[1]_INST_0\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"BAAA8AAA"
    )
        port map (
      I0 => \i_0_reg_80_reg_n_1_[1]\,
      I1 => icmp_ln97_reg_126,
      I2 => ap_CS_fsm_pp0_stage0,
      I3 => ap_enable_reg_pp0_iter1_reg_n_1,
      I4 => i_reg_130_reg(1),
      O => \^c_address0\(1)
    );
\C_address0[2]_INST_0\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"BAAA8AAA"
    )
        port map (
      I0 => \i_0_reg_80_reg_n_1_[2]\,
      I1 => icmp_ln97_reg_126,
      I2 => ap_CS_fsm_pp0_stage0,
      I3 => ap_enable_reg_pp0_iter1_reg_n_1,
      I4 => i_reg_130_reg(2),
      O => \^c_address0\(2)
    );
\C_address0[3]_INST_0\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"BAAA8AAA"
    )
        port map (
      I0 => \i_0_reg_80_reg_n_1_[3]\,
      I1 => icmp_ln97_reg_126,
      I2 => ap_CS_fsm_pp0_stage0,
      I3 => ap_enable_reg_pp0_iter1_reg_n_1,
      I4 => i_reg_130_reg(3),
      O => \^c_address0\(3)
    );
\C_address0[4]_INST_0\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"BAAA8AAA"
    )
        port map (
      I0 => \i_0_reg_80_reg_n_1_[4]\,
      I1 => icmp_ln97_reg_126,
      I2 => ap_CS_fsm_pp0_stage0,
      I3 => ap_enable_reg_pp0_iter1_reg_n_1,
      I4 => i_reg_130_reg(4),
      O => \^c_address0\(4)
    );
\C_address0[5]_INST_0\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFBF0080"
    )
        port map (
      I0 => i_reg_130_reg(5),
      I1 => ap_enable_reg_pp0_iter1_reg_n_1,
      I2 => ap_CS_fsm_pp0_stage0,
      I3 => icmp_ln97_reg_126,
      I4 => \i_0_reg_80_reg_n_1_[5]\,
      O => \^c_address0\(5)
    );
\C_address0[6]_INST_0\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFBF0080"
    )
        port map (
      I0 => i_reg_130_reg(6),
      I1 => ap_enable_reg_pp0_iter1_reg_n_1,
      I2 => ap_CS_fsm_pp0_stage0,
      I3 => icmp_ln97_reg_126,
      I4 => \i_0_reg_80_reg_n_1_[6]\,
      O => \^c_address0\(6)
    );
\C_address0[7]_INST_0\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFBF0080"
    )
        port map (
      I0 => i_reg_130_reg(7),
      I1 => ap_enable_reg_pp0_iter1_reg_n_1,
      I2 => ap_CS_fsm_pp0_stage0,
      I3 => icmp_ln97_reg_126,
      I4 => \i_0_reg_80_reg_n_1_[7]\,
      O => \^c_address0\(7)
    );
\C_address0[8]_INST_0\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFBF0080"
    )
        port map (
      I0 => i_reg_130_reg(8),
      I1 => ap_enable_reg_pp0_iter1_reg_n_1,
      I2 => ap_CS_fsm_pp0_stage0,
      I3 => icmp_ln97_reg_126,
      I4 => \i_0_reg_80_reg_n_1_[8]\,
      O => \^c_address0\(8)
    );
\C_address0[9]_INST_0\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFBF0080"
    )
        port map (
      I0 => i_reg_130_reg(9),
      I1 => ap_enable_reg_pp0_iter1_reg_n_1,
      I2 => ap_CS_fsm_pp0_stage0,
      I3 => icmp_ln97_reg_126,
      I4 => \i_0_reg_80_reg_n_1_[9]\,
      O => \^c_address0\(9)
    );
C_ce0_INST_0: unisim.vcomponents.LUT2
    generic map(
      INIT => X"8"
    )
        port map (
      I0 => ap_CS_fsm_pp0_stage0,
      I1 => ap_enable_reg_pp0_iter0,
      O => \^c_ce0\
    );
\ap_CS_fsm[0]_i_1\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"BA"
    )
        port map (
      I0 => \^ap_done\,
      I1 => ap_start,
      I2 => \ap_CS_fsm_reg_n_1_[0]\,
      O => ap_NS_fsm(0)
    );
\ap_CS_fsm[1]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"F8F888F8"
    )
        port map (
      I0 => ap_start,
      I1 => \ap_CS_fsm_reg_n_1_[0]\,
      I2 => ap_CS_fsm_pp0_stage1,
      I3 => ap_enable_reg_pp0_iter1_reg_n_1,
      I4 => ap_enable_reg_pp0_iter0,
      O => ap_NS_fsm(1)
    );
\ap_CS_fsm[2]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"AAAAAAAAAAAA8AAA"
    )
        port map (
      I0 => ap_CS_fsm_pp0_stage0,
      I1 => ap_enable_reg_pp0_iter1_reg_n_1,
      I2 => ap_enable_reg_pp0_iter0,
      I3 => \ap_CS_fsm[3]_i_3_n_1\,
      I4 => \ap_CS_fsm[2]_i_2_n_1\,
      I5 => \ap_CS_fsm[2]_i_3_n_1\,
      O => ap_NS_fsm(2)
    );
\ap_CS_fsm[2]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFFFFFFEFEEEFFF"
    )
        port map (
      I0 => \^c_address0\(1),
      I1 => \^c_address0\(0),
      I2 => \i_0_reg_80_reg_n_1_[9]\,
      I3 => \i_reg_130[8]_i_2_n_1\,
      I4 => i_reg_130_reg(9),
      I5 => \i_reg_130[9]_i_3_n_1\,
      O => \ap_CS_fsm[2]_i_2_n_1\
    );
\ap_CS_fsm[2]_i_3\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"335FFF5F"
    )
        port map (
      I0 => i_reg_130_reg(5),
      I1 => \i_0_reg_80_reg_n_1_[5]\,
      I2 => i_reg_130_reg(6),
      I3 => \i_reg_130[8]_i_2_n_1\,
      I4 => \i_0_reg_80_reg_n_1_[6]\,
      O => \ap_CS_fsm[2]_i_3_n_1\
    );
\ap_CS_fsm[3]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"00AA300000AA0000"
    )
        port map (
      I0 => ap_CS_fsm_pp0_stage1,
      I1 => \ap_CS_fsm[3]_i_2_n_1\,
      I2 => \ap_CS_fsm[3]_i_3_n_1\,
      I3 => ap_enable_reg_pp0_iter0,
      I4 => ap_enable_reg_pp0_iter1_reg_n_1,
      I5 => ap_CS_fsm_pp0_stage0,
      O => ap_NS_fsm(3)
    );
\ap_CS_fsm[3]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFFFFFFFFFFFFFE"
    )
        port map (
      I0 => \ap_CS_fsm[3]_i_4_n_1\,
      I1 => \i_reg_130[7]_i_2_n_1\,
      I2 => \i_reg_130[9]_i_3_n_1\,
      I3 => \i_reg_130[9]_i_2_n_1\,
      I4 => \^c_address0\(0),
      I5 => \^c_address0\(1),
      O => \ap_CS_fsm[3]_i_2_n_1\
    );
\ap_CS_fsm[3]_i_3\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"000000000000B800"
    )
        port map (
      I0 => \i_0_reg_80_reg_n_1_[7]\,
      I1 => \i_reg_130[8]_i_2_n_1\,
      I2 => i_reg_130_reg(7),
      I3 => \^c_address0\(3),
      I4 => \^c_address0\(2),
      I5 => \^c_address0\(4),
      O => \ap_CS_fsm[3]_i_3_n_1\
    );
\ap_CS_fsm[3]_i_4\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"45557555"
    )
        port map (
      I0 => \i_0_reg_80_reg_n_1_[6]\,
      I1 => icmp_ln97_reg_126,
      I2 => ap_CS_fsm_pp0_stage0,
      I3 => ap_enable_reg_pp0_iter1_reg_n_1,
      I4 => i_reg_130_reg(6),
      O => \ap_CS_fsm[3]_i_4_n_1\
    );
\ap_CS_fsm_reg[0]\: unisim.vcomponents.FDSE
    generic map(
      INIT => '1'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => ap_NS_fsm(0),
      Q => \ap_CS_fsm_reg_n_1_[0]\,
      S => ap_rst
    );
\ap_CS_fsm_reg[1]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => ap_NS_fsm(1),
      Q => ap_CS_fsm_pp0_stage0,
      R => ap_rst
    );
\ap_CS_fsm_reg[2]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => ap_NS_fsm(2),
      Q => ap_CS_fsm_pp0_stage1,
      R => ap_rst
    );
\ap_CS_fsm_reg[3]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => ap_NS_fsm(3),
      Q => \^ap_done\,
      R => ap_rst
    );
ap_enable_reg_pp0_iter0_i_1: unisim.vcomponents.LUT6
    generic map(
      INIT => X"5454005454545454"
    )
        port map (
      I0 => ap_rst,
      I1 => ap_NS_fsm14_out,
      I2 => ap_enable_reg_pp0_iter0,
      I3 => \ap_CS_fsm[3]_i_3_n_1\,
      I4 => \ap_CS_fsm[3]_i_2_n_1\,
      I5 => ap_CS_fsm_pp0_stage0,
      O => ap_enable_reg_pp0_iter0_i_1_n_1
    );
ap_enable_reg_pp0_iter0_i_2: unisim.vcomponents.LUT2
    generic map(
      INIT => X"8"
    )
        port map (
      I0 => \ap_CS_fsm_reg_n_1_[0]\,
      I1 => ap_start,
      O => ap_NS_fsm14_out
    );
ap_enable_reg_pp0_iter0_reg: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => ap_enable_reg_pp0_iter0_i_1_n_1,
      Q => ap_enable_reg_pp0_iter0,
      R => '0'
    );
ap_enable_reg_pp0_iter1_i_1: unisim.vcomponents.LUT6
    generic map(
      INIT => X"00000000F077F000"
    )
        port map (
      I0 => \ap_CS_fsm_reg_n_1_[0]\,
      I1 => ap_start,
      I2 => ap_enable_reg_pp0_iter0,
      I3 => ap_CS_fsm_pp0_stage1,
      I4 => ap_enable_reg_pp0_iter1_reg_n_1,
      I5 => ap_rst,
      O => ap_enable_reg_pp0_iter1_i_1_n_1
    );
ap_enable_reg_pp0_iter1_reg: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => ap_enable_reg_pp0_iter1_i_1_n_1,
      Q => ap_enable_reg_pp0_iter1_reg_n_1,
      R => '0'
    );
ap_idle_INST_0: unisim.vcomponents.LUT2
    generic map(
      INIT => X"2"
    )
        port map (
      I0 => \ap_CS_fsm_reg_n_1_[0]\,
      I1 => ap_start,
      O => ap_idle
    );
\hist_addr_reg_150[9]_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"2"
    )
        port map (
      I0 => ap_CS_fsm_pp0_stage1,
      I1 => icmp_ln97_reg_126,
      O => hist_addr_reg_1500
    );
\hist_addr_reg_150_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => A_q0(0),
      Q => hist_address0(0),
      R => '0'
    );
\hist_addr_reg_150_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => A_q0(1),
      Q => hist_address0(1),
      R => '0'
    );
\hist_addr_reg_150_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => A_q0(2),
      Q => hist_address0(2),
      R => '0'
    );
\hist_addr_reg_150_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => A_q0(3),
      Q => hist_address0(3),
      R => '0'
    );
\hist_addr_reg_150_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => A_q0(4),
      Q => hist_address0(4),
      R => '0'
    );
\hist_addr_reg_150_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => A_q0(5),
      Q => hist_address0(5),
      R => '0'
    );
\hist_addr_reg_150_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => A_q0(6),
      Q => hist_address0(6),
      R => '0'
    );
\hist_addr_reg_150_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => A_q0(7),
      Q => hist_address0(7),
      R => '0'
    );
\hist_addr_reg_150_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => A_q0(8),
      Q => hist_address0(8),
      R => '0'
    );
\hist_addr_reg_150_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => A_q0(9),
      Q => hist_address0(9),
      R => '0'
    );
hist_ce0_INST_0: unisim.vcomponents.LUT3
    generic map(
      INIT => X"E0"
    )
        port map (
      I0 => ap_CS_fsm_pp0_stage1,
      I1 => ap_CS_fsm_pp0_stage0,
      I2 => ap_enable_reg_pp0_iter1_reg_n_1,
      O => hist_ce0
    );
\hist_d0[0]_INST_0\: unisim.vcomponents.CARRY4
     port map (
      CI => '0',
      CO(3) => \hist_d0[0]_INST_0_n_1\,
      CO(2) => \hist_d0[0]_INST_0_n_2\,
      CO(1) => \hist_d0[0]_INST_0_n_3\,
      CO(0) => \hist_d0[0]_INST_0_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => hist_q0(3 downto 0),
      O(3 downto 0) => hist_d0(3 downto 0),
      S(3) => \hist_d0[0]_INST_0_i_1_n_1\,
      S(2) => \hist_d0[0]_INST_0_i_2_n_1\,
      S(1) => \hist_d0[0]_INST_0_i_3_n_1\,
      S(0) => \hist_d0[0]_INST_0_i_4_n_1\
    );
\hist_d0[0]_INST_0_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => hist_q0(3),
      I1 => wt_reg_145(3),
      O => \hist_d0[0]_INST_0_i_1_n_1\
    );
\hist_d0[0]_INST_0_i_2\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => hist_q0(2),
      I1 => wt_reg_145(2),
      O => \hist_d0[0]_INST_0_i_2_n_1\
    );
\hist_d0[0]_INST_0_i_3\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => hist_q0(1),
      I1 => wt_reg_145(1),
      O => \hist_d0[0]_INST_0_i_3_n_1\
    );
\hist_d0[0]_INST_0_i_4\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => hist_q0(0),
      I1 => wt_reg_145(0),
      O => \hist_d0[0]_INST_0_i_4_n_1\
    );
\hist_d0[12]_INST_0\: unisim.vcomponents.CARRY4
     port map (
      CI => \hist_d0[8]_INST_0_n_1\,
      CO(3) => \hist_d0[12]_INST_0_n_1\,
      CO(2) => \hist_d0[12]_INST_0_n_2\,
      CO(1) => \hist_d0[12]_INST_0_n_3\,
      CO(0) => \hist_d0[12]_INST_0_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => hist_q0(15 downto 12),
      O(3 downto 0) => hist_d0(15 downto 12),
      S(3) => \hist_d0[12]_INST_0_i_1_n_1\,
      S(2) => \hist_d0[12]_INST_0_i_2_n_1\,
      S(1) => \hist_d0[12]_INST_0_i_3_n_1\,
      S(0) => \hist_d0[12]_INST_0_i_4_n_1\
    );
\hist_d0[12]_INST_0_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => hist_q0(15),
      I1 => wt_reg_145(15),
      O => \hist_d0[12]_INST_0_i_1_n_1\
    );
\hist_d0[12]_INST_0_i_2\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => hist_q0(14),
      I1 => wt_reg_145(14),
      O => \hist_d0[12]_INST_0_i_2_n_1\
    );
\hist_d0[12]_INST_0_i_3\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => hist_q0(13),
      I1 => wt_reg_145(13),
      O => \hist_d0[12]_INST_0_i_3_n_1\
    );
\hist_d0[12]_INST_0_i_4\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => hist_q0(12),
      I1 => wt_reg_145(12),
      O => \hist_d0[12]_INST_0_i_4_n_1\
    );
\hist_d0[16]_INST_0\: unisim.vcomponents.CARRY4
     port map (
      CI => \hist_d0[12]_INST_0_n_1\,
      CO(3) => \hist_d0[16]_INST_0_n_1\,
      CO(2) => \hist_d0[16]_INST_0_n_2\,
      CO(1) => \hist_d0[16]_INST_0_n_3\,
      CO(0) => \hist_d0[16]_INST_0_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => hist_q0(19 downto 16),
      O(3 downto 0) => hist_d0(19 downto 16),
      S(3) => \hist_d0[16]_INST_0_i_1_n_1\,
      S(2) => \hist_d0[16]_INST_0_i_2_n_1\,
      S(1) => \hist_d0[16]_INST_0_i_3_n_1\,
      S(0) => \hist_d0[16]_INST_0_i_4_n_1\
    );
\hist_d0[16]_INST_0_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => hist_q0(19),
      I1 => wt_reg_145(19),
      O => \hist_d0[16]_INST_0_i_1_n_1\
    );
\hist_d0[16]_INST_0_i_2\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => hist_q0(18),
      I1 => wt_reg_145(18),
      O => \hist_d0[16]_INST_0_i_2_n_1\
    );
\hist_d0[16]_INST_0_i_3\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => hist_q0(17),
      I1 => wt_reg_145(17),
      O => \hist_d0[16]_INST_0_i_3_n_1\
    );
\hist_d0[16]_INST_0_i_4\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => hist_q0(16),
      I1 => wt_reg_145(16),
      O => \hist_d0[16]_INST_0_i_4_n_1\
    );
\hist_d0[20]_INST_0\: unisim.vcomponents.CARRY4
     port map (
      CI => \hist_d0[16]_INST_0_n_1\,
      CO(3) => \hist_d0[20]_INST_0_n_1\,
      CO(2) => \hist_d0[20]_INST_0_n_2\,
      CO(1) => \hist_d0[20]_INST_0_n_3\,
      CO(0) => \hist_d0[20]_INST_0_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => hist_q0(23 downto 20),
      O(3 downto 0) => hist_d0(23 downto 20),
      S(3) => \hist_d0[20]_INST_0_i_1_n_1\,
      S(2) => \hist_d0[20]_INST_0_i_2_n_1\,
      S(1) => \hist_d0[20]_INST_0_i_3_n_1\,
      S(0) => \hist_d0[20]_INST_0_i_4_n_1\
    );
\hist_d0[20]_INST_0_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => hist_q0(23),
      I1 => wt_reg_145(23),
      O => \hist_d0[20]_INST_0_i_1_n_1\
    );
\hist_d0[20]_INST_0_i_2\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => hist_q0(22),
      I1 => wt_reg_145(22),
      O => \hist_d0[20]_INST_0_i_2_n_1\
    );
\hist_d0[20]_INST_0_i_3\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => hist_q0(21),
      I1 => wt_reg_145(21),
      O => \hist_d0[20]_INST_0_i_3_n_1\
    );
\hist_d0[20]_INST_0_i_4\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => hist_q0(20),
      I1 => wt_reg_145(20),
      O => \hist_d0[20]_INST_0_i_4_n_1\
    );
\hist_d0[24]_INST_0\: unisim.vcomponents.CARRY4
     port map (
      CI => \hist_d0[20]_INST_0_n_1\,
      CO(3) => \hist_d0[24]_INST_0_n_1\,
      CO(2) => \hist_d0[24]_INST_0_n_2\,
      CO(1) => \hist_d0[24]_INST_0_n_3\,
      CO(0) => \hist_d0[24]_INST_0_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => hist_q0(27 downto 24),
      O(3 downto 0) => hist_d0(27 downto 24),
      S(3) => \hist_d0[24]_INST_0_i_1_n_1\,
      S(2) => \hist_d0[24]_INST_0_i_2_n_1\,
      S(1) => \hist_d0[24]_INST_0_i_3_n_1\,
      S(0) => \hist_d0[24]_INST_0_i_4_n_1\
    );
\hist_d0[24]_INST_0_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => hist_q0(27),
      I1 => wt_reg_145(27),
      O => \hist_d0[24]_INST_0_i_1_n_1\
    );
\hist_d0[24]_INST_0_i_2\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => hist_q0(26),
      I1 => wt_reg_145(26),
      O => \hist_d0[24]_INST_0_i_2_n_1\
    );
\hist_d0[24]_INST_0_i_3\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => hist_q0(25),
      I1 => wt_reg_145(25),
      O => \hist_d0[24]_INST_0_i_3_n_1\
    );
\hist_d0[24]_INST_0_i_4\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => hist_q0(24),
      I1 => wt_reg_145(24),
      O => \hist_d0[24]_INST_0_i_4_n_1\
    );
\hist_d0[28]_INST_0\: unisim.vcomponents.CARRY4
     port map (
      CI => \hist_d0[24]_INST_0_n_1\,
      CO(3) => \NLW_hist_d0[28]_INST_0_CO_UNCONNECTED\(3),
      CO(2) => \hist_d0[28]_INST_0_n_2\,
      CO(1) => \hist_d0[28]_INST_0_n_3\,
      CO(0) => \hist_d0[28]_INST_0_n_4\,
      CYINIT => '0',
      DI(3) => '0',
      DI(2 downto 0) => hist_q0(30 downto 28),
      O(3 downto 0) => hist_d0(31 downto 28),
      S(3) => \hist_d0[28]_INST_0_i_1_n_1\,
      S(2) => \hist_d0[28]_INST_0_i_2_n_1\,
      S(1) => \hist_d0[28]_INST_0_i_3_n_1\,
      S(0) => \hist_d0[28]_INST_0_i_4_n_1\
    );
\hist_d0[28]_INST_0_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => hist_q0(31),
      I1 => wt_reg_145(31),
      O => \hist_d0[28]_INST_0_i_1_n_1\
    );
\hist_d0[28]_INST_0_i_2\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => hist_q0(30),
      I1 => wt_reg_145(30),
      O => \hist_d0[28]_INST_0_i_2_n_1\
    );
\hist_d0[28]_INST_0_i_3\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => hist_q0(29),
      I1 => wt_reg_145(29),
      O => \hist_d0[28]_INST_0_i_3_n_1\
    );
\hist_d0[28]_INST_0_i_4\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => hist_q0(28),
      I1 => wt_reg_145(28),
      O => \hist_d0[28]_INST_0_i_4_n_1\
    );
\hist_d0[4]_INST_0\: unisim.vcomponents.CARRY4
     port map (
      CI => \hist_d0[0]_INST_0_n_1\,
      CO(3) => \hist_d0[4]_INST_0_n_1\,
      CO(2) => \hist_d0[4]_INST_0_n_2\,
      CO(1) => \hist_d0[4]_INST_0_n_3\,
      CO(0) => \hist_d0[4]_INST_0_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => hist_q0(7 downto 4),
      O(3 downto 0) => hist_d0(7 downto 4),
      S(3) => \hist_d0[4]_INST_0_i_1_n_1\,
      S(2) => \hist_d0[4]_INST_0_i_2_n_1\,
      S(1) => \hist_d0[4]_INST_0_i_3_n_1\,
      S(0) => \hist_d0[4]_INST_0_i_4_n_1\
    );
\hist_d0[4]_INST_0_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => hist_q0(7),
      I1 => wt_reg_145(7),
      O => \hist_d0[4]_INST_0_i_1_n_1\
    );
\hist_d0[4]_INST_0_i_2\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => hist_q0(6),
      I1 => wt_reg_145(6),
      O => \hist_d0[4]_INST_0_i_2_n_1\
    );
\hist_d0[4]_INST_0_i_3\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => hist_q0(5),
      I1 => wt_reg_145(5),
      O => \hist_d0[4]_INST_0_i_3_n_1\
    );
\hist_d0[4]_INST_0_i_4\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => hist_q0(4),
      I1 => wt_reg_145(4),
      O => \hist_d0[4]_INST_0_i_4_n_1\
    );
\hist_d0[8]_INST_0\: unisim.vcomponents.CARRY4
     port map (
      CI => \hist_d0[4]_INST_0_n_1\,
      CO(3) => \hist_d0[8]_INST_0_n_1\,
      CO(2) => \hist_d0[8]_INST_0_n_2\,
      CO(1) => \hist_d0[8]_INST_0_n_3\,
      CO(0) => \hist_d0[8]_INST_0_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => hist_q0(11 downto 8),
      O(3 downto 0) => hist_d0(11 downto 8),
      S(3) => \hist_d0[8]_INST_0_i_1_n_1\,
      S(2) => \hist_d0[8]_INST_0_i_2_n_1\,
      S(1) => \hist_d0[8]_INST_0_i_3_n_1\,
      S(0) => \hist_d0[8]_INST_0_i_4_n_1\
    );
\hist_d0[8]_INST_0_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => hist_q0(11),
      I1 => wt_reg_145(11),
      O => \hist_d0[8]_INST_0_i_1_n_1\
    );
\hist_d0[8]_INST_0_i_2\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => hist_q0(10),
      I1 => wt_reg_145(10),
      O => \hist_d0[8]_INST_0_i_2_n_1\
    );
\hist_d0[8]_INST_0_i_3\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => hist_q0(9),
      I1 => wt_reg_145(9),
      O => \hist_d0[8]_INST_0_i_3_n_1\
    );
\hist_d0[8]_INST_0_i_4\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => hist_q0(8),
      I1 => wt_reg_145(8),
      O => \hist_d0[8]_INST_0_i_4_n_1\
    );
hist_we0_INST_0: unisim.vcomponents.LUT3
    generic map(
      INIT => X"80"
    )
        port map (
      I0 => icmp_ln102_fu_114_p2,
      I1 => ap_CS_fsm_pp0_stage1,
      I2 => ap_enable_reg_pp0_iter1_reg_n_1,
      O => hist_we0
    );
hist_we0_INST_0_i_1: unisim.vcomponents.CARRY4
     port map (
      CI => hist_we0_INST_0_i_2_n_1,
      CO(3) => icmp_ln102_fu_114_p2,
      CO(2) => hist_we0_INST_0_i_1_n_2,
      CO(1) => hist_we0_INST_0_i_1_n_3,
      CO(0) => hist_we0_INST_0_i_1_n_4,
      CYINIT => '0',
      DI(3) => hist_we0_INST_0_i_3_n_1,
      DI(2) => hist_we0_INST_0_i_4_n_1,
      DI(1) => hist_we0_INST_0_i_5_n_1,
      DI(0) => hist_we0_INST_0_i_6_n_1,
      O(3 downto 0) => NLW_hist_we0_INST_0_i_1_O_UNCONNECTED(3 downto 0),
      S(3) => hist_we0_INST_0_i_7_n_1,
      S(2) => hist_we0_INST_0_i_8_n_1,
      S(1) => hist_we0_INST_0_i_9_n_1,
      S(0) => hist_we0_INST_0_i_10_n_1
    );
hist_we0_INST_0_i_10: unisim.vcomponents.LUT2
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => hist_q0(25),
      I1 => hist_q0(24),
      O => hist_we0_INST_0_i_10_n_1
    );
hist_we0_INST_0_i_11: unisim.vcomponents.CARRY4
     port map (
      CI => hist_we0_INST_0_i_20_n_1,
      CO(3) => hist_we0_INST_0_i_11_n_1,
      CO(2) => hist_we0_INST_0_i_11_n_2,
      CO(1) => hist_we0_INST_0_i_11_n_3,
      CO(0) => hist_we0_INST_0_i_11_n_4,
      CYINIT => '0',
      DI(3) => hist_we0_INST_0_i_21_n_1,
      DI(2) => hist_we0_INST_0_i_22_n_1,
      DI(1) => hist_we0_INST_0_i_23_n_1,
      DI(0) => hist_we0_INST_0_i_24_n_1,
      O(3 downto 0) => NLW_hist_we0_INST_0_i_11_O_UNCONNECTED(3 downto 0),
      S(3) => hist_we0_INST_0_i_25_n_1,
      S(2) => hist_we0_INST_0_i_26_n_1,
      S(1) => hist_we0_INST_0_i_27_n_1,
      S(0) => hist_we0_INST_0_i_28_n_1
    );
hist_we0_INST_0_i_12: unisim.vcomponents.LUT2
    generic map(
      INIT => X"E"
    )
        port map (
      I0 => hist_q0(22),
      I1 => hist_q0(23),
      O => hist_we0_INST_0_i_12_n_1
    );
hist_we0_INST_0_i_13: unisim.vcomponents.LUT2
    generic map(
      INIT => X"E"
    )
        port map (
      I0 => hist_q0(20),
      I1 => hist_q0(21),
      O => hist_we0_INST_0_i_13_n_1
    );
hist_we0_INST_0_i_14: unisim.vcomponents.LUT2
    generic map(
      INIT => X"E"
    )
        port map (
      I0 => hist_q0(18),
      I1 => hist_q0(19),
      O => hist_we0_INST_0_i_14_n_1
    );
hist_we0_INST_0_i_15: unisim.vcomponents.LUT2
    generic map(
      INIT => X"E"
    )
        port map (
      I0 => hist_q0(16),
      I1 => hist_q0(17),
      O => hist_we0_INST_0_i_15_n_1
    );
hist_we0_INST_0_i_16: unisim.vcomponents.LUT2
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => hist_q0(23),
      I1 => hist_q0(22),
      O => hist_we0_INST_0_i_16_n_1
    );
hist_we0_INST_0_i_17: unisim.vcomponents.LUT2
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => hist_q0(21),
      I1 => hist_q0(20),
      O => hist_we0_INST_0_i_17_n_1
    );
hist_we0_INST_0_i_18: unisim.vcomponents.LUT2
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => hist_q0(19),
      I1 => hist_q0(18),
      O => hist_we0_INST_0_i_18_n_1
    );
hist_we0_INST_0_i_19: unisim.vcomponents.LUT2
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => hist_q0(17),
      I1 => hist_q0(16),
      O => hist_we0_INST_0_i_19_n_1
    );
hist_we0_INST_0_i_2: unisim.vcomponents.CARRY4
     port map (
      CI => hist_we0_INST_0_i_11_n_1,
      CO(3) => hist_we0_INST_0_i_2_n_1,
      CO(2) => hist_we0_INST_0_i_2_n_2,
      CO(1) => hist_we0_INST_0_i_2_n_3,
      CO(0) => hist_we0_INST_0_i_2_n_4,
      CYINIT => '0',
      DI(3) => hist_we0_INST_0_i_12_n_1,
      DI(2) => hist_we0_INST_0_i_13_n_1,
      DI(1) => hist_we0_INST_0_i_14_n_1,
      DI(0) => hist_we0_INST_0_i_15_n_1,
      O(3 downto 0) => NLW_hist_we0_INST_0_i_2_O_UNCONNECTED(3 downto 0),
      S(3) => hist_we0_INST_0_i_16_n_1,
      S(2) => hist_we0_INST_0_i_17_n_1,
      S(1) => hist_we0_INST_0_i_18_n_1,
      S(0) => hist_we0_INST_0_i_19_n_1
    );
hist_we0_INST_0_i_20: unisim.vcomponents.CARRY4
     port map (
      CI => '0',
      CO(3) => hist_we0_INST_0_i_20_n_1,
      CO(2) => hist_we0_INST_0_i_20_n_2,
      CO(1) => hist_we0_INST_0_i_20_n_3,
      CO(0) => hist_we0_INST_0_i_20_n_4,
      CYINIT => '0',
      DI(3) => hist_we0_INST_0_i_29_n_1,
      DI(2) => hist_we0_INST_0_i_30_n_1,
      DI(1) => hist_we0_INST_0_i_31_n_1,
      DI(0) => hist_we0_INST_0_i_32_n_1,
      O(3 downto 0) => NLW_hist_we0_INST_0_i_20_O_UNCONNECTED(3 downto 0),
      S(3) => hist_we0_INST_0_i_33_n_1,
      S(2) => hist_we0_INST_0_i_34_n_1,
      S(1) => hist_we0_INST_0_i_35_n_1,
      S(0) => hist_we0_INST_0_i_36_n_1
    );
hist_we0_INST_0_i_21: unisim.vcomponents.LUT2
    generic map(
      INIT => X"E"
    )
        port map (
      I0 => hist_q0(14),
      I1 => hist_q0(15),
      O => hist_we0_INST_0_i_21_n_1
    );
hist_we0_INST_0_i_22: unisim.vcomponents.LUT2
    generic map(
      INIT => X"E"
    )
        port map (
      I0 => hist_q0(12),
      I1 => hist_q0(13),
      O => hist_we0_INST_0_i_22_n_1
    );
hist_we0_INST_0_i_23: unisim.vcomponents.LUT2
    generic map(
      INIT => X"E"
    )
        port map (
      I0 => hist_q0(10),
      I1 => hist_q0(11),
      O => hist_we0_INST_0_i_23_n_1
    );
hist_we0_INST_0_i_24: unisim.vcomponents.LUT2
    generic map(
      INIT => X"E"
    )
        port map (
      I0 => hist_q0(8),
      I1 => hist_q0(9),
      O => hist_we0_INST_0_i_24_n_1
    );
hist_we0_INST_0_i_25: unisim.vcomponents.LUT2
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => hist_q0(15),
      I1 => hist_q0(14),
      O => hist_we0_INST_0_i_25_n_1
    );
hist_we0_INST_0_i_26: unisim.vcomponents.LUT2
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => hist_q0(13),
      I1 => hist_q0(12),
      O => hist_we0_INST_0_i_26_n_1
    );
hist_we0_INST_0_i_27: unisim.vcomponents.LUT2
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => hist_q0(11),
      I1 => hist_q0(10),
      O => hist_we0_INST_0_i_27_n_1
    );
hist_we0_INST_0_i_28: unisim.vcomponents.LUT2
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => hist_q0(9),
      I1 => hist_q0(8),
      O => hist_we0_INST_0_i_28_n_1
    );
hist_we0_INST_0_i_29: unisim.vcomponents.LUT2
    generic map(
      INIT => X"E"
    )
        port map (
      I0 => hist_q0(6),
      I1 => hist_q0(7),
      O => hist_we0_INST_0_i_29_n_1
    );
hist_we0_INST_0_i_3: unisim.vcomponents.LUT2
    generic map(
      INIT => X"2"
    )
        port map (
      I0 => hist_q0(30),
      I1 => hist_q0(31),
      O => hist_we0_INST_0_i_3_n_1
    );
hist_we0_INST_0_i_30: unisim.vcomponents.LUT2
    generic map(
      INIT => X"E"
    )
        port map (
      I0 => hist_q0(4),
      I1 => hist_q0(5),
      O => hist_we0_INST_0_i_30_n_1
    );
hist_we0_INST_0_i_31: unisim.vcomponents.LUT2
    generic map(
      INIT => X"E"
    )
        port map (
      I0 => hist_q0(2),
      I1 => hist_q0(3),
      O => hist_we0_INST_0_i_31_n_1
    );
hist_we0_INST_0_i_32: unisim.vcomponents.LUT2
    generic map(
      INIT => X"E"
    )
        port map (
      I0 => hist_q0(0),
      I1 => hist_q0(1),
      O => hist_we0_INST_0_i_32_n_1
    );
hist_we0_INST_0_i_33: unisim.vcomponents.LUT2
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => hist_q0(7),
      I1 => hist_q0(6),
      O => hist_we0_INST_0_i_33_n_1
    );
hist_we0_INST_0_i_34: unisim.vcomponents.LUT2
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => hist_q0(5),
      I1 => hist_q0(4),
      O => hist_we0_INST_0_i_34_n_1
    );
hist_we0_INST_0_i_35: unisim.vcomponents.LUT2
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => hist_q0(3),
      I1 => hist_q0(2),
      O => hist_we0_INST_0_i_35_n_1
    );
hist_we0_INST_0_i_36: unisim.vcomponents.LUT2
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => hist_q0(1),
      I1 => hist_q0(0),
      O => hist_we0_INST_0_i_36_n_1
    );
hist_we0_INST_0_i_4: unisim.vcomponents.LUT2
    generic map(
      INIT => X"E"
    )
        port map (
      I0 => hist_q0(28),
      I1 => hist_q0(29),
      O => hist_we0_INST_0_i_4_n_1
    );
hist_we0_INST_0_i_5: unisim.vcomponents.LUT2
    generic map(
      INIT => X"E"
    )
        port map (
      I0 => hist_q0(26),
      I1 => hist_q0(27),
      O => hist_we0_INST_0_i_5_n_1
    );
hist_we0_INST_0_i_6: unisim.vcomponents.LUT2
    generic map(
      INIT => X"E"
    )
        port map (
      I0 => hist_q0(24),
      I1 => hist_q0(25),
      O => hist_we0_INST_0_i_6_n_1
    );
hist_we0_INST_0_i_7: unisim.vcomponents.LUT2
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => hist_q0(31),
      I1 => hist_q0(30),
      O => hist_we0_INST_0_i_7_n_1
    );
hist_we0_INST_0_i_8: unisim.vcomponents.LUT2
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => hist_q0(29),
      I1 => hist_q0(28),
      O => hist_we0_INST_0_i_8_n_1
    );
hist_we0_INST_0_i_9: unisim.vcomponents.LUT2
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => hist_q0(27),
      I1 => hist_q0(26),
      O => hist_we0_INST_0_i_9_n_1
    );
\i_0_reg_80[9]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"88880888"
    )
        port map (
      I0 => ap_start,
      I1 => \ap_CS_fsm_reg_n_1_[0]\,
      I2 => ap_enable_reg_pp0_iter1_reg_n_1,
      I3 => ap_CS_fsm_pp0_stage0,
      I4 => icmp_ln97_reg_126,
      O => i_0_reg_80
    );
\i_0_reg_80[9]_i_2\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"08"
    )
        port map (
      I0 => ap_enable_reg_pp0_iter1_reg_n_1,
      I1 => ap_CS_fsm_pp0_stage0,
      I2 => icmp_ln97_reg_126,
      O => ap_phi_mux_i_0_phi_fu_84_p41
    );
\i_0_reg_80_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_phi_mux_i_0_phi_fu_84_p41,
      D => i_reg_130_reg(0),
      Q => \i_0_reg_80_reg_n_1_[0]\,
      R => i_0_reg_80
    );
\i_0_reg_80_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_phi_mux_i_0_phi_fu_84_p41,
      D => i_reg_130_reg(1),
      Q => \i_0_reg_80_reg_n_1_[1]\,
      R => i_0_reg_80
    );
\i_0_reg_80_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_phi_mux_i_0_phi_fu_84_p41,
      D => i_reg_130_reg(2),
      Q => \i_0_reg_80_reg_n_1_[2]\,
      R => i_0_reg_80
    );
\i_0_reg_80_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_phi_mux_i_0_phi_fu_84_p41,
      D => i_reg_130_reg(3),
      Q => \i_0_reg_80_reg_n_1_[3]\,
      R => i_0_reg_80
    );
\i_0_reg_80_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_phi_mux_i_0_phi_fu_84_p41,
      D => i_reg_130_reg(4),
      Q => \i_0_reg_80_reg_n_1_[4]\,
      R => i_0_reg_80
    );
\i_0_reg_80_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_phi_mux_i_0_phi_fu_84_p41,
      D => i_reg_130_reg(5),
      Q => \i_0_reg_80_reg_n_1_[5]\,
      R => i_0_reg_80
    );
\i_0_reg_80_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_phi_mux_i_0_phi_fu_84_p41,
      D => i_reg_130_reg(6),
      Q => \i_0_reg_80_reg_n_1_[6]\,
      R => i_0_reg_80
    );
\i_0_reg_80_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_phi_mux_i_0_phi_fu_84_p41,
      D => i_reg_130_reg(7),
      Q => \i_0_reg_80_reg_n_1_[7]\,
      R => i_0_reg_80
    );
\i_0_reg_80_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_phi_mux_i_0_phi_fu_84_p41,
      D => i_reg_130_reg(8),
      Q => \i_0_reg_80_reg_n_1_[8]\,
      R => i_0_reg_80
    );
\i_0_reg_80_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_phi_mux_i_0_phi_fu_84_p41,
      D => i_reg_130_reg(9),
      Q => \i_0_reg_80_reg_n_1_[9]\,
      R => i_0_reg_80
    );
\i_reg_130[0]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"4575"
    )
        port map (
      I0 => \i_0_reg_80_reg_n_1_[0]\,
      I1 => icmp_ln97_reg_126,
      I2 => ap_enable_reg_pp0_iter1_reg_n_1,
      I3 => i_reg_130_reg(0),
      O => i_fu_97_p2(0)
    );
\i_reg_130[1]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"335ACC5A"
    )
        port map (
      I0 => i_reg_130_reg(0),
      I1 => \i_0_reg_80_reg_n_1_[0]\,
      I2 => i_reg_130_reg(1),
      I3 => \i_reg_130[8]_i_2_n_1\,
      I4 => \i_0_reg_80_reg_n_1_[1]\,
      O => i_fu_97_p2(1)
    );
\i_reg_130[2]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"5A5A66AAAAAA66AA"
    )
        port map (
      I0 => \^c_address0\(2),
      I1 => i_reg_130_reg(0),
      I2 => \i_0_reg_80_reg_n_1_[0]\,
      I3 => i_reg_130_reg(1),
      I4 => \i_reg_130[8]_i_2_n_1\,
      I5 => \i_0_reg_80_reg_n_1_[1]\,
      O => i_fu_97_p2(2)
    );
\i_reg_130[3]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"9A999599"
    )
        port map (
      I0 => \i_reg_130[4]_i_2_n_1\,
      I1 => \i_0_reg_80_reg_n_1_[3]\,
      I2 => icmp_ln97_reg_126,
      I3 => ap_enable_reg_pp0_iter1_reg_n_1,
      I4 => i_reg_130_reg(3),
      O => i_fu_97_p2(3)
    );
\i_reg_130[4]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"C3CCA5A5C3CCAAAA"
    )
        port map (
      I0 => i_reg_130_reg(4),
      I1 => \i_0_reg_80_reg_n_1_[4]\,
      I2 => \i_reg_130[4]_i_2_n_1\,
      I3 => \i_0_reg_80_reg_n_1_[3]\,
      I4 => \i_reg_130[8]_i_2_n_1\,
      I5 => i_reg_130_reg(3),
      O => i_fu_97_p2(4)
    );
\i_reg_130[4]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"47CF77FFFFFFFFFF"
    )
        port map (
      I0 => \i_0_reg_80_reg_n_1_[1]\,
      I1 => \i_reg_130[8]_i_2_n_1\,
      I2 => i_reg_130_reg(1),
      I3 => \i_0_reg_80_reg_n_1_[0]\,
      I4 => i_reg_130_reg(0),
      I5 => \^c_address0\(2),
      O => \i_reg_130[4]_i_2_n_1\
    );
\i_reg_130[5]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"3CCC5A5A3CCCAAAA"
    )
        port map (
      I0 => i_reg_130_reg(5),
      I1 => \i_0_reg_80_reg_n_1_[5]\,
      I2 => \i_reg_130[5]_i_2_n_1\,
      I3 => \i_0_reg_80_reg_n_1_[4]\,
      I4 => \i_reg_130[8]_i_2_n_1\,
      I5 => i_reg_130_reg(4),
      O => i_fu_97_p2(5)
    );
\i_reg_130[5]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"E200000000000000"
    )
        port map (
      I0 => i_reg_130_reg(3),
      I1 => \i_reg_130[8]_i_2_n_1\,
      I2 => \i_0_reg_80_reg_n_1_[3]\,
      I3 => \^c_address0\(2),
      I4 => \^c_address0\(0),
      I5 => \^c_address0\(1),
      O => \i_reg_130[5]_i_2_n_1\
    );
\i_reg_130[6]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"C3CCA5A5C3CCAAAA"
    )
        port map (
      I0 => i_reg_130_reg(6),
      I1 => \i_0_reg_80_reg_n_1_[6]\,
      I2 => \i_reg_130[9]_i_5_n_1\,
      I3 => \i_0_reg_80_reg_n_1_[5]\,
      I4 => \i_reg_130[8]_i_2_n_1\,
      I5 => i_reg_130_reg(5),
      O => \i_reg_130[6]_i_1_n_1\
    );
\i_reg_130[7]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"5656565555555655"
    )
        port map (
      I0 => \i_reg_130[9]_i_4_n_1\,
      I1 => \i_reg_130[9]_i_5_n_1\,
      I2 => \i_reg_130[7]_i_2_n_1\,
      I3 => i_reg_130_reg(6),
      I4 => \i_reg_130[8]_i_2_n_1\,
      I5 => \i_0_reg_80_reg_n_1_[6]\,
      O => i_fu_97_p2(7)
    );
\i_reg_130[7]_i_2\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"45557555"
    )
        port map (
      I0 => \i_0_reg_80_reg_n_1_[5]\,
      I1 => icmp_ln97_reg_126,
      I2 => ap_CS_fsm_pp0_stage0,
      I3 => ap_enable_reg_pp0_iter1_reg_n_1,
      I4 => i_reg_130_reg(5),
      O => \i_reg_130[7]_i_2_n_1\
    );
\i_reg_130[8]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"E2E2E2E2E2E2E21D"
    )
        port map (
      I0 => i_reg_130_reg(8),
      I1 => \i_reg_130[8]_i_2_n_1\,
      I2 => \i_0_reg_80_reg_n_1_[8]\,
      I3 => \ap_CS_fsm[2]_i_3_n_1\,
      I4 => \i_reg_130[9]_i_5_n_1\,
      I5 => \i_reg_130[9]_i_4_n_1\,
      O => \i_reg_130[8]_i_1_n_1\
    );
\i_reg_130[8]_i_2\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"BF"
    )
        port map (
      I0 => icmp_ln97_reg_126,
      I1 => ap_CS_fsm_pp0_stage0,
      I2 => ap_enable_reg_pp0_iter1_reg_n_1,
      O => \i_reg_130[8]_i_2_n_1\
    );
\i_reg_130[9]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"55555556"
    )
        port map (
      I0 => \i_reg_130[9]_i_2_n_1\,
      I1 => \i_reg_130[9]_i_3_n_1\,
      I2 => \i_reg_130[9]_i_4_n_1\,
      I3 => \i_reg_130[9]_i_5_n_1\,
      I4 => \ap_CS_fsm[2]_i_3_n_1\,
      O => \i_reg_130[9]_i_1_n_1\
    );
\i_reg_130[9]_i_2\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"45557555"
    )
        port map (
      I0 => \i_0_reg_80_reg_n_1_[9]\,
      I1 => icmp_ln97_reg_126,
      I2 => ap_CS_fsm_pp0_stage0,
      I3 => ap_enable_reg_pp0_iter1_reg_n_1,
      I4 => i_reg_130_reg(9),
      O => \i_reg_130[9]_i_2_n_1\
    );
\i_reg_130[9]_i_3\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"45557555"
    )
        port map (
      I0 => \i_0_reg_80_reg_n_1_[8]\,
      I1 => icmp_ln97_reg_126,
      I2 => ap_CS_fsm_pp0_stage0,
      I3 => ap_enable_reg_pp0_iter1_reg_n_1,
      I4 => i_reg_130_reg(8),
      O => \i_reg_130[9]_i_3_n_1\
    );
\i_reg_130[9]_i_4\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"45557555"
    )
        port map (
      I0 => \i_0_reg_80_reg_n_1_[7]\,
      I1 => icmp_ln97_reg_126,
      I2 => ap_CS_fsm_pp0_stage0,
      I3 => ap_enable_reg_pp0_iter1_reg_n_1,
      I4 => i_reg_130_reg(7),
      O => \i_reg_130[9]_i_4_n_1\
    );
\i_reg_130[9]_i_5\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"7FFFFFFF"
    )
        port map (
      I0 => \^c_address0\(4),
      I1 => \^c_address0\(1),
      I2 => \^c_address0\(0),
      I3 => \^c_address0\(2),
      I4 => \^c_address0\(3),
      O => \i_reg_130[9]_i_5_n_1\
    );
\i_reg_130_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^c_ce0\,
      D => i_fu_97_p2(0),
      Q => i_reg_130_reg(0),
      R => '0'
    );
\i_reg_130_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^c_ce0\,
      D => i_fu_97_p2(1),
      Q => i_reg_130_reg(1),
      R => '0'
    );
\i_reg_130_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^c_ce0\,
      D => i_fu_97_p2(2),
      Q => i_reg_130_reg(2),
      R => '0'
    );
\i_reg_130_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^c_ce0\,
      D => i_fu_97_p2(3),
      Q => i_reg_130_reg(3),
      R => '0'
    );
\i_reg_130_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^c_ce0\,
      D => i_fu_97_p2(4),
      Q => i_reg_130_reg(4),
      R => '0'
    );
\i_reg_130_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^c_ce0\,
      D => i_fu_97_p2(5),
      Q => i_reg_130_reg(5),
      R => '0'
    );
\i_reg_130_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^c_ce0\,
      D => \i_reg_130[6]_i_1_n_1\,
      Q => i_reg_130_reg(6),
      R => '0'
    );
\i_reg_130_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^c_ce0\,
      D => i_fu_97_p2(7),
      Q => i_reg_130_reg(7),
      R => '0'
    );
\i_reg_130_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^c_ce0\,
      D => \i_reg_130[8]_i_1_n_1\,
      Q => i_reg_130_reg(8),
      R => '0'
    );
\i_reg_130_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^c_ce0\,
      D => \i_reg_130[9]_i_1_n_1\,
      Q => i_reg_130_reg(9),
      R => '0'
    );
\icmp_ln97_reg_126[0]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"02FF0200"
    )
        port map (
      I0 => \ap_CS_fsm[3]_i_3_n_1\,
      I1 => \ap_CS_fsm[2]_i_2_n_1\,
      I2 => \ap_CS_fsm[2]_i_3_n_1\,
      I3 => ap_CS_fsm_pp0_stage0,
      I4 => icmp_ln97_reg_126,
      O => \icmp_ln97_reg_126[0]_i_1_n_1\
    );
\icmp_ln97_reg_126_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \icmp_ln97_reg_126[0]_i_1_n_1\,
      Q => icmp_ln97_reg_126,
      R => '0'
    );
\wt_reg_145_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => C_q0(0),
      Q => wt_reg_145(0),
      R => '0'
    );
\wt_reg_145_reg[10]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => C_q0(10),
      Q => wt_reg_145(10),
      R => '0'
    );
\wt_reg_145_reg[11]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => C_q0(11),
      Q => wt_reg_145(11),
      R => '0'
    );
\wt_reg_145_reg[12]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => C_q0(12),
      Q => wt_reg_145(12),
      R => '0'
    );
\wt_reg_145_reg[13]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => C_q0(13),
      Q => wt_reg_145(13),
      R => '0'
    );
\wt_reg_145_reg[14]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => C_q0(14),
      Q => wt_reg_145(14),
      R => '0'
    );
\wt_reg_145_reg[15]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => C_q0(15),
      Q => wt_reg_145(15),
      R => '0'
    );
\wt_reg_145_reg[16]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => C_q0(16),
      Q => wt_reg_145(16),
      R => '0'
    );
\wt_reg_145_reg[17]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => C_q0(17),
      Q => wt_reg_145(17),
      R => '0'
    );
\wt_reg_145_reg[18]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => C_q0(18),
      Q => wt_reg_145(18),
      R => '0'
    );
\wt_reg_145_reg[19]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => C_q0(19),
      Q => wt_reg_145(19),
      R => '0'
    );
\wt_reg_145_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => C_q0(1),
      Q => wt_reg_145(1),
      R => '0'
    );
\wt_reg_145_reg[20]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => C_q0(20),
      Q => wt_reg_145(20),
      R => '0'
    );
\wt_reg_145_reg[21]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => C_q0(21),
      Q => wt_reg_145(21),
      R => '0'
    );
\wt_reg_145_reg[22]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => C_q0(22),
      Q => wt_reg_145(22),
      R => '0'
    );
\wt_reg_145_reg[23]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => C_q0(23),
      Q => wt_reg_145(23),
      R => '0'
    );
\wt_reg_145_reg[24]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => C_q0(24),
      Q => wt_reg_145(24),
      R => '0'
    );
\wt_reg_145_reg[25]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => C_q0(25),
      Q => wt_reg_145(25),
      R => '0'
    );
\wt_reg_145_reg[26]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => C_q0(26),
      Q => wt_reg_145(26),
      R => '0'
    );
\wt_reg_145_reg[27]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => C_q0(27),
      Q => wt_reg_145(27),
      R => '0'
    );
\wt_reg_145_reg[28]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => C_q0(28),
      Q => wt_reg_145(28),
      R => '0'
    );
\wt_reg_145_reg[29]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => C_q0(29),
      Q => wt_reg_145(29),
      R => '0'
    );
\wt_reg_145_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => C_q0(2),
      Q => wt_reg_145(2),
      R => '0'
    );
\wt_reg_145_reg[30]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => C_q0(30),
      Q => wt_reg_145(30),
      R => '0'
    );
\wt_reg_145_reg[31]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => C_q0(31),
      Q => wt_reg_145(31),
      R => '0'
    );
\wt_reg_145_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => C_q0(3),
      Q => wt_reg_145(3),
      R => '0'
    );
\wt_reg_145_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => C_q0(4),
      Q => wt_reg_145(4),
      R => '0'
    );
\wt_reg_145_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => C_q0(5),
      Q => wt_reg_145(5),
      R => '0'
    );
\wt_reg_145_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => C_q0(6),
      Q => wt_reg_145(6),
      R => '0'
    );
\wt_reg_145_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => C_q0(7),
      Q => wt_reg_145(7),
      R => '0'
    );
\wt_reg_145_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => C_q0(8),
      Q => wt_reg_145(8),
      R => '0'
    );
\wt_reg_145_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => hist_addr_reg_1500,
      D => C_q0(9),
      Q => wt_reg_145(9),
      R => '0'
    );
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix is
  port (
    A_ce0 : out STD_LOGIC;
    hist_ce0 : out STD_LOGIC;
    hist_we0 : out STD_LOGIC;
    C_ce0 : out STD_LOGIC;
    ap_clk : in STD_LOGIC;
    ap_rst : in STD_LOGIC;
    ap_start : in STD_LOGIC;
    ap_done : out STD_LOGIC;
    ap_idle : out STD_LOGIC;
    ap_ready : out STD_LOGIC;
    A_address0 : out STD_LOGIC_VECTOR ( 9 downto 0 );
    A_q0 : in STD_LOGIC_VECTOR ( 31 downto 0 );
    hist_address0 : out STD_LOGIC_VECTOR ( 9 downto 0 );
    hist_d0 : out STD_LOGIC_VECTOR ( 31 downto 0 );
    hist_q0 : in STD_LOGIC_VECTOR ( 31 downto 0 );
    C_address0 : out STD_LOGIC_VECTOR ( 9 downto 0 );
    C_q0 : in STD_LOGIC_VECTOR ( 31 downto 0 )
  );
  attribute NotValidForBitStream : boolean;
  attribute NotValidForBitStream of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix : entity is true;
  attribute CHECK_LICENSE_TYPE : string;
  attribute CHECK_LICENSE_TYPE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix : entity is "bd_0_hls_inst_0,loop_imperfect,{}";
  attribute DowngradeIPIdentifiedWarnings : string;
  attribute DowngradeIPIdentifiedWarnings of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix : entity is "yes";
  attribute IP_DEFINITION_SOURCE : string;
  attribute IP_DEFINITION_SOURCE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix : entity is "HLS";
  attribute X_CORE_INFO : string;
  attribute X_CORE_INFO of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix : entity is "loop_imperfect,Vivado 2019.2";
  attribute hls_module : string;
  attribute hls_module of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix : entity is "yes";
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix is
  attribute ap_ST_fsm_pp0_stage0 : string;
  attribute ap_ST_fsm_pp0_stage0 of inst : label is "4'b0010";
  attribute ap_ST_fsm_pp0_stage1 : string;
  attribute ap_ST_fsm_pp0_stage1 of inst : label is "4'b0100";
  attribute ap_ST_fsm_state1 : string;
  attribute ap_ST_fsm_state1 of inst : label is "4'b0001";
  attribute ap_ST_fsm_state6 : string;
  attribute ap_ST_fsm_state6 of inst : label is "4'b1000";
  attribute X_INTERFACE_INFO : string;
  attribute X_INTERFACE_INFO of ap_clk : signal is "xilinx.com:signal:clock:1.0 ap_clk CLK";
  attribute X_INTERFACE_PARAMETER : string;
  attribute X_INTERFACE_PARAMETER of ap_clk : signal is "XIL_INTERFACENAME ap_clk, ASSOCIATED_RESET ap_rst, FREQ_HZ 250000000.0, PHASE 0.000, CLK_DOMAIN bd_0_ap_clk_0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of ap_done : signal is "xilinx.com:interface:acc_handshake:1.0 ap_ctrl done";
  attribute X_INTERFACE_INFO of ap_idle : signal is "xilinx.com:interface:acc_handshake:1.0 ap_ctrl idle";
  attribute X_INTERFACE_INFO of ap_ready : signal is "xilinx.com:interface:acc_handshake:1.0 ap_ctrl ready";
  attribute X_INTERFACE_INFO of ap_rst : signal is "xilinx.com:signal:reset:1.0 ap_rst RST";
  attribute X_INTERFACE_PARAMETER of ap_rst : signal is "XIL_INTERFACENAME ap_rst, POLARITY ACTIVE_HIGH, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of ap_start : signal is "xilinx.com:interface:acc_handshake:1.0 ap_ctrl start";
  attribute X_INTERFACE_INFO of A_address0 : signal is "xilinx.com:signal:data:1.0 A_address0 DATA";
  attribute X_INTERFACE_PARAMETER of A_address0 : signal is "XIL_INTERFACENAME A_address0, LAYERED_METADATA undef";
  attribute X_INTERFACE_INFO of A_q0 : signal is "xilinx.com:signal:data:1.0 A_q0 DATA";
  attribute X_INTERFACE_PARAMETER of A_q0 : signal is "XIL_INTERFACENAME A_q0, LAYERED_METADATA undef";
  attribute X_INTERFACE_INFO of C_address0 : signal is "xilinx.com:signal:data:1.0 C_address0 DATA";
  attribute X_INTERFACE_PARAMETER of C_address0 : signal is "XIL_INTERFACENAME C_address0, LAYERED_METADATA undef";
  attribute X_INTERFACE_INFO of C_q0 : signal is "xilinx.com:signal:data:1.0 C_q0 DATA";
  attribute X_INTERFACE_PARAMETER of C_q0 : signal is "XIL_INTERFACENAME C_q0, LAYERED_METADATA undef";
  attribute X_INTERFACE_INFO of hist_address0 : signal is "xilinx.com:signal:data:1.0 hist_address0 DATA";
  attribute X_INTERFACE_PARAMETER of hist_address0 : signal is "XIL_INTERFACENAME hist_address0, LAYERED_METADATA undef";
  attribute X_INTERFACE_INFO of hist_d0 : signal is "xilinx.com:signal:data:1.0 hist_d0 DATA";
  attribute X_INTERFACE_PARAMETER of hist_d0 : signal is "XIL_INTERFACENAME hist_d0, LAYERED_METADATA undef";
  attribute X_INTERFACE_INFO of hist_q0 : signal is "xilinx.com:signal:data:1.0 hist_q0 DATA";
  attribute X_INTERFACE_PARAMETER of hist_q0 : signal is "XIL_INTERFACENAME hist_q0, LAYERED_METADATA undef";
begin
inst: entity work.decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect
     port map (
      A_address0(9 downto 0) => A_address0(9 downto 0),
      A_ce0 => A_ce0,
      A_q0(31 downto 0) => A_q0(31 downto 0),
      C_address0(9 downto 0) => C_address0(9 downto 0),
      C_ce0 => C_ce0,
      C_q0(31 downto 0) => C_q0(31 downto 0),
      ap_clk => ap_clk,
      ap_done => ap_done,
      ap_idle => ap_idle,
      ap_ready => ap_ready,
      ap_rst => ap_rst,
      ap_start => ap_start,
      hist_address0(9 downto 0) => hist_address0(9 downto 0),
      hist_ce0 => hist_ce0,
      hist_d0(31 downto 0) => hist_d0(31 downto 0),
      hist_q0(31 downto 0) => hist_q0(31 downto 0),
      hist_we0 => hist_we0
    );
end STRUCTURE;
