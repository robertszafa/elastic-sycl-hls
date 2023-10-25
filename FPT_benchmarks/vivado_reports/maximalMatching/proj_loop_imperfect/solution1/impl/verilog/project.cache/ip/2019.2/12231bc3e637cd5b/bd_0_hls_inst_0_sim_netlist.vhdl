-- Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2019.2 (lin64) Build 2708876 Wed Nov  6 21:39:14 MST 2019
-- Date        : Fri Jun 23 11:57:07 2023
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
    vertices_address0 : out STD_LOGIC_VECTOR ( 10 downto 0 );
    vertices_ce0 : out STD_LOGIC;
    vertices_we0 : out STD_LOGIC;
    vertices_d0 : out STD_LOGIC_VECTOR ( 31 downto 0 );
    vertices_q0 : in STD_LOGIC_VECTOR ( 31 downto 0 );
    vertices_address1 : out STD_LOGIC_VECTOR ( 10 downto 0 );
    vertices_ce1 : out STD_LOGIC;
    vertices_we1 : out STD_LOGIC;
    vertices_d1 : out STD_LOGIC_VECTOR ( 31 downto 0 );
    vertices_q1 : in STD_LOGIC_VECTOR ( 31 downto 0 );
    edges_address0 : out STD_LOGIC_VECTOR ( 10 downto 0 );
    edges_ce0 : out STD_LOGIC;
    edges_q0 : in STD_LOGIC_VECTOR ( 31 downto 0 );
    edges_address1 : out STD_LOGIC_VECTOR ( 10 downto 0 );
    edges_ce1 : out STD_LOGIC;
    edges_q1 : in STD_LOGIC_VECTOR ( 31 downto 0 )
  );
  attribute ap_ST_fsm_state1 : string;
  attribute ap_ST_fsm_state1 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is "7'b0000001";
  attribute ap_ST_fsm_state2 : string;
  attribute ap_ST_fsm_state2 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is "7'b0000010";
  attribute ap_ST_fsm_state3 : string;
  attribute ap_ST_fsm_state3 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is "7'b0000100";
  attribute ap_ST_fsm_state4 : string;
  attribute ap_ST_fsm_state4 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is "7'b0001000";
  attribute ap_ST_fsm_state5 : string;
  attribute ap_ST_fsm_state5 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is "7'b0010000";
  attribute ap_ST_fsm_state6 : string;
  attribute ap_ST_fsm_state6 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is "7'b0100000";
  attribute ap_ST_fsm_state7 : string;
  attribute ap_ST_fsm_state7 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is "7'b1000000";
  attribute hls_module : string;
  attribute hls_module of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is "yes";
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect is
  signal \<const0>\ : STD_LOGIC;
  signal \<const1>\ : STD_LOGIC;
  signal \ap_CS_fsm[0]_i_2_n_1\ : STD_LOGIC;
  signal \ap_CS_fsm[0]_i_3_n_1\ : STD_LOGIC;
  signal \ap_CS_fsm_reg_n_1_[0]\ : STD_LOGIC;
  signal ap_CS_fsm_state3 : STD_LOGIC;
  signal ap_CS_fsm_state4 : STD_LOGIC;
  signal ap_CS_fsm_state5 : STD_LOGIC;
  signal ap_CS_fsm_state6 : STD_LOGIC;
  signal ap_CS_fsm_state7 : STD_LOGIC;
  signal ap_NS_fsm : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal \^ap_ready\ : STD_LOGIC;
  signal ap_ready_INST_0_i_1_n_1 : STD_LOGIC;
  signal ap_ready_INST_0_i_2_n_1 : STD_LOGIC;
  signal \^edges_address1\ : STD_LOGIC_VECTOR ( 10 downto 1 );
  signal \^edges_ce1\ : STD_LOGIC;
  signal i_0_reg_78 : STD_LOGIC;
  signal i_fu_95_p2 : STD_LOGIC_VECTOR ( 9 downto 0 );
  signal i_reg_148 : STD_LOGIC_VECTOR ( 9 downto 0 );
  signal \i_reg_148[8]_i_2_n_1\ : STD_LOGIC;
  signal \i_reg_148[9]_i_2_n_1\ : STD_LOGIC;
  signal icmp_ln108_reg_185 : STD_LOGIC;
  signal \icmp_ln108_reg_185[0]_i_1_n_1\ : STD_LOGIC;
  signal \icmp_ln108_reg_185[0]_i_2_n_1\ : STD_LOGIC;
  signal \icmp_ln108_reg_185[0]_i_3_n_1\ : STD_LOGIC;
  signal \icmp_ln108_reg_185[0]_i_4_n_1\ : STD_LOGIC;
  signal \^vertices_d0\ : STD_LOGIC_VECTOR ( 31 downto 0 );
  signal \^vertices_d1\ : STD_LOGIC_VECTOR ( 31 downto 0 );
  signal vertices_we0_INST_0_i_10_n_1 : STD_LOGIC;
  signal vertices_we0_INST_0_i_11_n_1 : STD_LOGIC;
  signal vertices_we0_INST_0_i_12_n_1 : STD_LOGIC;
  signal vertices_we0_INST_0_i_13_n_1 : STD_LOGIC;
  signal vertices_we0_INST_0_i_14_n_1 : STD_LOGIC;
  signal vertices_we0_INST_0_i_15_n_1 : STD_LOGIC;
  signal vertices_we0_INST_0_i_16_n_1 : STD_LOGIC;
  signal vertices_we0_INST_0_i_17_n_1 : STD_LOGIC;
  signal vertices_we0_INST_0_i_18_n_1 : STD_LOGIC;
  signal vertices_we0_INST_0_i_19_n_1 : STD_LOGIC;
  signal vertices_we0_INST_0_i_1_n_1 : STD_LOGIC;
  signal vertices_we0_INST_0_i_2_n_1 : STD_LOGIC;
  signal vertices_we0_INST_0_i_3_n_1 : STD_LOGIC;
  signal vertices_we0_INST_0_i_4_n_1 : STD_LOGIC;
  signal vertices_we0_INST_0_i_5_n_1 : STD_LOGIC;
  signal vertices_we0_INST_0_i_6_n_1 : STD_LOGIC;
  signal vertices_we0_INST_0_i_7_n_1 : STD_LOGIC;
  signal vertices_we0_INST_0_i_8_n_1 : STD_LOGIC;
  signal vertices_we0_INST_0_i_9_n_1 : STD_LOGIC;
  attribute SOFT_HLUTNM : string;
  attribute SOFT_HLUTNM of \ap_CS_fsm[0]_i_1\ : label is "soft_lutpair2";
  attribute SOFT_HLUTNM of \ap_CS_fsm[1]_i_1\ : label is "soft_lutpair8";
  attribute SOFT_HLUTNM of \ap_CS_fsm[2]_i_1\ : label is "soft_lutpair1";
  attribute FSM_ENCODING : string;
  attribute FSM_ENCODING of \ap_CS_fsm_reg[0]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[1]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[2]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[3]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[4]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[5]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[6]\ : label is "none";
  attribute SOFT_HLUTNM of ap_idle_INST_0 : label is "soft_lutpair2";
  attribute SOFT_HLUTNM of ap_ready_INST_0 : label is "soft_lutpair1";
  attribute SOFT_HLUTNM of ap_ready_INST_0_i_2 : label is "soft_lutpair7";
  attribute SOFT_HLUTNM of \i_reg_148[0]_i_1\ : label is "soft_lutpair9";
  attribute SOFT_HLUTNM of \i_reg_148[1]_i_1\ : label is "soft_lutpair9";
  attribute SOFT_HLUTNM of \i_reg_148[2]_i_1\ : label is "soft_lutpair6";
  attribute SOFT_HLUTNM of \i_reg_148[3]_i_1\ : label is "soft_lutpair6";
  attribute SOFT_HLUTNM of \i_reg_148[4]_i_1\ : label is "soft_lutpair3";
  attribute SOFT_HLUTNM of \i_reg_148[7]_i_1\ : label is "soft_lutpair5";
  attribute SOFT_HLUTNM of \i_reg_148[8]_i_2\ : label is "soft_lutpair3";
  attribute SOFT_HLUTNM of \i_reg_148[9]_i_1\ : label is "soft_lutpair7";
  attribute SOFT_HLUTNM of \i_reg_148[9]_i_2\ : label is "soft_lutpair5";
  attribute SOFT_HLUTNM of \icmp_ln108_reg_185[0]_i_3\ : label is "soft_lutpair0";
  attribute SOFT_HLUTNM of \icmp_ln108_reg_185[0]_i_4\ : label is "soft_lutpair4";
  attribute SOFT_HLUTNM of vertices_ce1_INST_0 : label is "soft_lutpair8";
  attribute SOFT_HLUTNM of vertices_we0_INST_0_i_3 : label is "soft_lutpair4";
  attribute SOFT_HLUTNM of vertices_we0_INST_0_i_4 : label is "soft_lutpair0";
begin
  ap_done <= \^ap_ready\;
  ap_ready <= \^ap_ready\;
  edges_address0(10 downto 1) <= \^edges_address1\(10 downto 1);
  edges_address0(0) <= \<const0>\;
  edges_address1(10 downto 1) <= \^edges_address1\(10 downto 1);
  edges_address1(0) <= \<const1>\;
  edges_ce0 <= \^edges_ce1\;
  edges_ce1 <= \^edges_ce1\;
  vertices_d0(31 downto 0) <= \^vertices_d0\(31 downto 0);
  vertices_d1(31 downto 0) <= \^vertices_d1\(31 downto 0);
GND: unisim.vcomponents.GND
     port map (
      G => \<const0>\
    );
VCC: unisim.vcomponents.VCC
     port map (
      P => \<const1>\
    );
\ap_CS_fsm[0]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"44444F44"
    )
        port map (
      I0 => ap_start,
      I1 => \ap_CS_fsm_reg_n_1_[0]\,
      I2 => \ap_CS_fsm[0]_i_2_n_1\,
      I3 => \ap_CS_fsm[0]_i_3_n_1\,
      I4 => ap_ready_INST_0_i_1_n_1,
      O => ap_NS_fsm(0)
    );
\ap_CS_fsm[0]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFFFFFFFFFFFFFE"
    )
        port map (
      I0 => ap_CS_fsm_state6,
      I1 => ap_CS_fsm_state4,
      I2 => \ap_CS_fsm_reg_n_1_[0]\,
      I3 => ap_CS_fsm_state5,
      I4 => ap_CS_fsm_state7,
      I5 => ap_CS_fsm_state3,
      O => \ap_CS_fsm[0]_i_2_n_1\
    );
\ap_CS_fsm[0]_i_3\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"8000"
    )
        port map (
      I0 => \^edges_address1\(10),
      I1 => \^edges_ce1\,
      I2 => \^edges_address1\(7),
      I3 => \^edges_address1\(4),
      O => \ap_CS_fsm[0]_i_3_n_1\
    );
\ap_CS_fsm[1]_i_1\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"EA"
    )
        port map (
      I0 => ap_CS_fsm_state7,
      I1 => \ap_CS_fsm_reg_n_1_[0]\,
      I2 => ap_start,
      O => ap_NS_fsm(1)
    );
\ap_CS_fsm[2]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"B0F0F0F0"
    )
        port map (
      I0 => ap_ready_INST_0_i_1_n_1,
      I1 => \^edges_address1\(10),
      I2 => \^edges_ce1\,
      I3 => \^edges_address1\(7),
      I4 => \^edges_address1\(4),
      O => ap_NS_fsm(2)
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
      Q => \^edges_ce1\,
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
      Q => ap_CS_fsm_state3,
      R => ap_rst
    );
\ap_CS_fsm_reg[3]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => ap_CS_fsm_state3,
      Q => ap_CS_fsm_state4,
      R => ap_rst
    );
\ap_CS_fsm_reg[4]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => ap_CS_fsm_state4,
      Q => ap_CS_fsm_state5,
      R => ap_rst
    );
\ap_CS_fsm_reg[5]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => ap_CS_fsm_state5,
      Q => ap_CS_fsm_state6,
      R => ap_rst
    );
\ap_CS_fsm_reg[6]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => ap_CS_fsm_state6,
      Q => ap_CS_fsm_state7,
      R => ap_rst
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
ap_ready_INST_0: unisim.vcomponents.LUT5
    generic map(
      INIT => X"00008000"
    )
        port map (
      I0 => \^edges_address1\(4),
      I1 => \^edges_address1\(7),
      I2 => \^edges_ce1\,
      I3 => \^edges_address1\(10),
      I4 => ap_ready_INST_0_i_1_n_1,
      O => \^ap_ready\
    );
ap_ready_INST_0_i_1: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFFFFFFFFFFFFEF"
    )
        port map (
      I0 => \^edges_address1\(5),
      I1 => \^edges_address1\(3),
      I2 => \^edges_address1\(6),
      I3 => ap_ready_INST_0_i_2_n_1,
      I4 => \^edges_address1\(1),
      I5 => \^edges_address1\(2),
      O => ap_ready_INST_0_i_1_n_1
    );
ap_ready_INST_0_i_2: unisim.vcomponents.LUT2
    generic map(
      INIT => X"7"
    )
        port map (
      I0 => \^edges_address1\(8),
      I1 => \^edges_address1\(9),
      O => ap_ready_INST_0_i_2_n_1
    );
\i_0_reg_78[9]_i_1\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"08"
    )
        port map (
      I0 => \ap_CS_fsm_reg_n_1_[0]\,
      I1 => ap_start,
      I2 => ap_CS_fsm_state7,
      O => i_0_reg_78
    );
\i_0_reg_78_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state7,
      D => i_reg_148(0),
      Q => \^edges_address1\(1),
      R => i_0_reg_78
    );
\i_0_reg_78_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state7,
      D => i_reg_148(1),
      Q => \^edges_address1\(2),
      R => i_0_reg_78
    );
\i_0_reg_78_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state7,
      D => i_reg_148(2),
      Q => \^edges_address1\(3),
      R => i_0_reg_78
    );
\i_0_reg_78_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state7,
      D => i_reg_148(3),
      Q => \^edges_address1\(4),
      R => i_0_reg_78
    );
\i_0_reg_78_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state7,
      D => i_reg_148(4),
      Q => \^edges_address1\(5),
      R => i_0_reg_78
    );
\i_0_reg_78_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state7,
      D => i_reg_148(5),
      Q => \^edges_address1\(6),
      R => i_0_reg_78
    );
\i_0_reg_78_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state7,
      D => i_reg_148(6),
      Q => \^edges_address1\(7),
      R => i_0_reg_78
    );
\i_0_reg_78_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state7,
      D => i_reg_148(7),
      Q => \^edges_address1\(8),
      R => i_0_reg_78
    );
\i_0_reg_78_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state7,
      D => i_reg_148(8),
      Q => \^edges_address1\(9),
      R => i_0_reg_78
    );
\i_0_reg_78_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state7,
      D => i_reg_148(9),
      Q => \^edges_address1\(10),
      R => i_0_reg_78
    );
\i_reg_148[0]_i_1\: unisim.vcomponents.LUT1
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => \^edges_address1\(1),
      O => i_fu_95_p2(0)
    );
\i_reg_148[1]_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => \^edges_address1\(1),
      I1 => \^edges_address1\(2),
      O => i_fu_95_p2(1)
    );
\i_reg_148[2]_i_1\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"6A"
    )
        port map (
      I0 => \^edges_address1\(3),
      I1 => \^edges_address1\(1),
      I2 => \^edges_address1\(2),
      O => i_fu_95_p2(2)
    );
\i_reg_148[3]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"7F80"
    )
        port map (
      I0 => \^edges_address1\(2),
      I1 => \^edges_address1\(1),
      I2 => \^edges_address1\(3),
      I3 => \^edges_address1\(4),
      O => i_fu_95_p2(3)
    );
\i_reg_148[4]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"6AAAAAAA"
    )
        port map (
      I0 => \^edges_address1\(5),
      I1 => \^edges_address1\(2),
      I2 => \^edges_address1\(1),
      I3 => \^edges_address1\(3),
      I4 => \^edges_address1\(4),
      O => i_fu_95_p2(4)
    );
\i_reg_148[5]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"6AAAAAAAAAAAAAAA"
    )
        port map (
      I0 => \^edges_address1\(6),
      I1 => \^edges_address1\(4),
      I2 => \^edges_address1\(3),
      I3 => \^edges_address1\(1),
      I4 => \^edges_address1\(2),
      I5 => \^edges_address1\(5),
      O => i_fu_95_p2(5)
    );
\i_reg_148[6]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"6AAA"
    )
        port map (
      I0 => \^edges_address1\(7),
      I1 => \^edges_address1\(5),
      I2 => \i_reg_148[8]_i_2_n_1\,
      I3 => \^edges_address1\(6),
      O => i_fu_95_p2(6)
    );
\i_reg_148[7]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"6AAAAAAA"
    )
        port map (
      I0 => \^edges_address1\(8),
      I1 => \^edges_address1\(6),
      I2 => \i_reg_148[8]_i_2_n_1\,
      I3 => \^edges_address1\(5),
      I4 => \^edges_address1\(7),
      O => i_fu_95_p2(7)
    );
\i_reg_148[8]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"6AAAAAAAAAAAAAAA"
    )
        port map (
      I0 => \^edges_address1\(9),
      I1 => \^edges_address1\(7),
      I2 => \^edges_address1\(5),
      I3 => \i_reg_148[8]_i_2_n_1\,
      I4 => \^edges_address1\(6),
      I5 => \^edges_address1\(8),
      O => i_fu_95_p2(8)
    );
\i_reg_148[8]_i_2\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"8000"
    )
        port map (
      I0 => \^edges_address1\(4),
      I1 => \^edges_address1\(3),
      I2 => \^edges_address1\(1),
      I3 => \^edges_address1\(2),
      O => \i_reg_148[8]_i_2_n_1\
    );
\i_reg_148[9]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"6AAA"
    )
        port map (
      I0 => \^edges_address1\(10),
      I1 => \^edges_address1\(8),
      I2 => \^edges_address1\(9),
      I3 => \i_reg_148[9]_i_2_n_1\,
      O => i_fu_95_p2(9)
    );
\i_reg_148[9]_i_2\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"8000"
    )
        port map (
      I0 => \^edges_address1\(7),
      I1 => \^edges_address1\(5),
      I2 => \i_reg_148[8]_i_2_n_1\,
      I3 => \^edges_address1\(6),
      O => \i_reg_148[9]_i_2_n_1\
    );
\i_reg_148_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^edges_ce1\,
      D => i_fu_95_p2(0),
      Q => i_reg_148(0),
      R => '0'
    );
\i_reg_148_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^edges_ce1\,
      D => i_fu_95_p2(1),
      Q => i_reg_148(1),
      R => '0'
    );
\i_reg_148_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^edges_ce1\,
      D => i_fu_95_p2(2),
      Q => i_reg_148(2),
      R => '0'
    );
\i_reg_148_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^edges_ce1\,
      D => i_fu_95_p2(3),
      Q => i_reg_148(3),
      R => '0'
    );
\i_reg_148_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^edges_ce1\,
      D => i_fu_95_p2(4),
      Q => i_reg_148(4),
      R => '0'
    );
\i_reg_148_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^edges_ce1\,
      D => i_fu_95_p2(5),
      Q => i_reg_148(5),
      R => '0'
    );
\i_reg_148_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^edges_ce1\,
      D => i_fu_95_p2(6),
      Q => i_reg_148(6),
      R => '0'
    );
\i_reg_148_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^edges_ce1\,
      D => i_fu_95_p2(7),
      Q => i_reg_148(7),
      R => '0'
    );
\i_reg_148_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^edges_ce1\,
      D => i_fu_95_p2(8),
      Q => i_reg_148(8),
      R => '0'
    );
\i_reg_148_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^edges_ce1\,
      D => i_fu_95_p2(9),
      Q => i_reg_148(9),
      R => '0'
    );
\icmp_ln108_reg_185[0]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0004FFFF00040000"
    )
        port map (
      I0 => vertices_we0_INST_0_i_5_n_1,
      I1 => \icmp_ln108_reg_185[0]_i_2_n_1\,
      I2 => vertices_we0_INST_0_i_2_n_1,
      I3 => vertices_we0_INST_0_i_1_n_1,
      I4 => ap_CS_fsm_state6,
      I5 => icmp_ln108_reg_185,
      O => \icmp_ln108_reg_185[0]_i_1_n_1\
    );
\icmp_ln108_reg_185[0]_i_2\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"0004"
    )
        port map (
      I0 => vertices_we0_INST_0_i_15_n_1,
      I1 => \icmp_ln108_reg_185[0]_i_3_n_1\,
      I2 => vertices_we0_INST_0_i_14_n_1,
      I3 => \icmp_ln108_reg_185[0]_i_4_n_1\,
      O => \icmp_ln108_reg_185[0]_i_2_n_1\
    );
\icmp_ln108_reg_185[0]_i_3\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"0001"
    )
        port map (
      I0 => \^vertices_d1\(12),
      I1 => \^vertices_d1\(0),
      I2 => \^vertices_d0\(30),
      I3 => \^vertices_d1\(15),
      O => \icmp_ln108_reg_185[0]_i_3_n_1\
    );
\icmp_ln108_reg_185[0]_i_4\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FFFE"
    )
        port map (
      I0 => \^vertices_d0\(27),
      I1 => \^vertices_d0\(18),
      I2 => \^vertices_d0\(24),
      I3 => \^vertices_d0\(5),
      O => \icmp_ln108_reg_185[0]_i_4_n_1\
    );
\icmp_ln108_reg_185_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \icmp_ln108_reg_185[0]_i_1_n_1\,
      Q => icmp_ln108_reg_185,
      R => '0'
    );
\v1_reg_173_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q0(0),
      Q => \^vertices_d1\(0),
      R => '0'
    );
\v1_reg_173_reg[10]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q0(10),
      Q => \^vertices_d1\(10),
      R => '0'
    );
\v1_reg_173_reg[11]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q0(11),
      Q => \^vertices_d1\(11),
      R => '0'
    );
\v1_reg_173_reg[12]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q0(12),
      Q => \^vertices_d1\(12),
      R => '0'
    );
\v1_reg_173_reg[13]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q0(13),
      Q => \^vertices_d1\(13),
      R => '0'
    );
\v1_reg_173_reg[14]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q0(14),
      Q => \^vertices_d1\(14),
      R => '0'
    );
\v1_reg_173_reg[15]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q0(15),
      Q => \^vertices_d1\(15),
      R => '0'
    );
\v1_reg_173_reg[16]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q0(16),
      Q => \^vertices_d1\(16),
      R => '0'
    );
\v1_reg_173_reg[17]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q0(17),
      Q => \^vertices_d1\(17),
      R => '0'
    );
\v1_reg_173_reg[18]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q0(18),
      Q => \^vertices_d1\(18),
      R => '0'
    );
\v1_reg_173_reg[19]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q0(19),
      Q => \^vertices_d1\(19),
      R => '0'
    );
\v1_reg_173_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q0(1),
      Q => \^vertices_d1\(1),
      R => '0'
    );
\v1_reg_173_reg[20]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q0(20),
      Q => \^vertices_d1\(20),
      R => '0'
    );
\v1_reg_173_reg[21]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q0(21),
      Q => \^vertices_d1\(21),
      R => '0'
    );
\v1_reg_173_reg[22]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q0(22),
      Q => \^vertices_d1\(22),
      R => '0'
    );
\v1_reg_173_reg[23]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q0(23),
      Q => \^vertices_d1\(23),
      R => '0'
    );
\v1_reg_173_reg[24]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q0(24),
      Q => \^vertices_d1\(24),
      R => '0'
    );
\v1_reg_173_reg[25]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q0(25),
      Q => \^vertices_d1\(25),
      R => '0'
    );
\v1_reg_173_reg[26]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q0(26),
      Q => \^vertices_d1\(26),
      R => '0'
    );
\v1_reg_173_reg[27]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q0(27),
      Q => \^vertices_d1\(27),
      R => '0'
    );
\v1_reg_173_reg[28]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q0(28),
      Q => \^vertices_d1\(28),
      R => '0'
    );
\v1_reg_173_reg[29]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q0(29),
      Q => \^vertices_d1\(29),
      R => '0'
    );
\v1_reg_173_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q0(2),
      Q => \^vertices_d1\(2),
      R => '0'
    );
\v1_reg_173_reg[30]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q0(30),
      Q => \^vertices_d1\(30),
      R => '0'
    );
\v1_reg_173_reg[31]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q0(31),
      Q => \^vertices_d1\(31),
      R => '0'
    );
\v1_reg_173_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q0(3),
      Q => \^vertices_d1\(3),
      R => '0'
    );
\v1_reg_173_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q0(4),
      Q => \^vertices_d1\(4),
      R => '0'
    );
\v1_reg_173_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q0(5),
      Q => \^vertices_d1\(5),
      R => '0'
    );
\v1_reg_173_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q0(6),
      Q => \^vertices_d1\(6),
      R => '0'
    );
\v1_reg_173_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q0(7),
      Q => \^vertices_d1\(7),
      R => '0'
    );
\v1_reg_173_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q0(8),
      Q => \^vertices_d1\(8),
      R => '0'
    );
\v1_reg_173_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q0(9),
      Q => \^vertices_d1\(9),
      R => '0'
    );
\v2_reg_179_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q1(0),
      Q => \^vertices_d0\(0),
      R => '0'
    );
\v2_reg_179_reg[10]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q1(10),
      Q => \^vertices_d0\(10),
      R => '0'
    );
\v2_reg_179_reg[11]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q1(11),
      Q => \^vertices_d0\(11),
      R => '0'
    );
\v2_reg_179_reg[12]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q1(12),
      Q => \^vertices_d0\(12),
      R => '0'
    );
\v2_reg_179_reg[13]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q1(13),
      Q => \^vertices_d0\(13),
      R => '0'
    );
\v2_reg_179_reg[14]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q1(14),
      Q => \^vertices_d0\(14),
      R => '0'
    );
\v2_reg_179_reg[15]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q1(15),
      Q => \^vertices_d0\(15),
      R => '0'
    );
\v2_reg_179_reg[16]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q1(16),
      Q => \^vertices_d0\(16),
      R => '0'
    );
\v2_reg_179_reg[17]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q1(17),
      Q => \^vertices_d0\(17),
      R => '0'
    );
\v2_reg_179_reg[18]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q1(18),
      Q => \^vertices_d0\(18),
      R => '0'
    );
\v2_reg_179_reg[19]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q1(19),
      Q => \^vertices_d0\(19),
      R => '0'
    );
\v2_reg_179_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q1(1),
      Q => \^vertices_d0\(1),
      R => '0'
    );
\v2_reg_179_reg[20]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q1(20),
      Q => \^vertices_d0\(20),
      R => '0'
    );
\v2_reg_179_reg[21]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q1(21),
      Q => \^vertices_d0\(21),
      R => '0'
    );
\v2_reg_179_reg[22]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q1(22),
      Q => \^vertices_d0\(22),
      R => '0'
    );
\v2_reg_179_reg[23]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q1(23),
      Q => \^vertices_d0\(23),
      R => '0'
    );
\v2_reg_179_reg[24]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q1(24),
      Q => \^vertices_d0\(24),
      R => '0'
    );
\v2_reg_179_reg[25]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q1(25),
      Q => \^vertices_d0\(25),
      R => '0'
    );
\v2_reg_179_reg[26]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q1(26),
      Q => \^vertices_d0\(26),
      R => '0'
    );
\v2_reg_179_reg[27]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q1(27),
      Q => \^vertices_d0\(27),
      R => '0'
    );
\v2_reg_179_reg[28]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q1(28),
      Q => \^vertices_d0\(28),
      R => '0'
    );
\v2_reg_179_reg[29]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q1(29),
      Q => \^vertices_d0\(29),
      R => '0'
    );
\v2_reg_179_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q1(2),
      Q => \^vertices_d0\(2),
      R => '0'
    );
\v2_reg_179_reg[30]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q1(30),
      Q => \^vertices_d0\(30),
      R => '0'
    );
\v2_reg_179_reg[31]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q1(31),
      Q => \^vertices_d0\(31),
      R => '0'
    );
\v2_reg_179_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q1(3),
      Q => \^vertices_d0\(3),
      R => '0'
    );
\v2_reg_179_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q1(4),
      Q => \^vertices_d0\(4),
      R => '0'
    );
\v2_reg_179_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q1(5),
      Q => \^vertices_d0\(5),
      R => '0'
    );
\v2_reg_179_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q1(6),
      Q => \^vertices_d0\(6),
      R => '0'
    );
\v2_reg_179_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q1(7),
      Q => \^vertices_d0\(7),
      R => '0'
    );
\v2_reg_179_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q1(8),
      Q => \^vertices_d0\(8),
      R => '0'
    );
\v2_reg_179_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state5,
      D => vertices_q1(9),
      Q => \^vertices_d0\(9),
      R => '0'
    );
\vertices_addr_1_reg_168_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => edges_q1(0),
      Q => vertices_address1(0),
      R => '0'
    );
\vertices_addr_1_reg_168_reg[10]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => edges_q1(10),
      Q => vertices_address1(10),
      R => '0'
    );
\vertices_addr_1_reg_168_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => edges_q1(1),
      Q => vertices_address1(1),
      R => '0'
    );
\vertices_addr_1_reg_168_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => edges_q1(2),
      Q => vertices_address1(2),
      R => '0'
    );
\vertices_addr_1_reg_168_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => edges_q1(3),
      Q => vertices_address1(3),
      R => '0'
    );
\vertices_addr_1_reg_168_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => edges_q1(4),
      Q => vertices_address1(4),
      R => '0'
    );
\vertices_addr_1_reg_168_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => edges_q1(5),
      Q => vertices_address1(5),
      R => '0'
    );
\vertices_addr_1_reg_168_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => edges_q1(6),
      Q => vertices_address1(6),
      R => '0'
    );
\vertices_addr_1_reg_168_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => edges_q1(7),
      Q => vertices_address1(7),
      R => '0'
    );
\vertices_addr_1_reg_168_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => edges_q1(8),
      Q => vertices_address1(8),
      R => '0'
    );
\vertices_addr_1_reg_168_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => edges_q1(9),
      Q => vertices_address1(9),
      R => '0'
    );
\vertices_addr_reg_163_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => edges_q0(0),
      Q => vertices_address0(0),
      R => '0'
    );
\vertices_addr_reg_163_reg[10]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => edges_q0(10),
      Q => vertices_address0(10),
      R => '0'
    );
\vertices_addr_reg_163_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => edges_q0(1),
      Q => vertices_address0(1),
      R => '0'
    );
\vertices_addr_reg_163_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => edges_q0(2),
      Q => vertices_address0(2),
      R => '0'
    );
\vertices_addr_reg_163_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => edges_q0(3),
      Q => vertices_address0(3),
      R => '0'
    );
\vertices_addr_reg_163_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => edges_q0(4),
      Q => vertices_address0(4),
      R => '0'
    );
\vertices_addr_reg_163_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => edges_q0(5),
      Q => vertices_address0(5),
      R => '0'
    );
\vertices_addr_reg_163_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => edges_q0(6),
      Q => vertices_address0(6),
      R => '0'
    );
\vertices_addr_reg_163_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => edges_q0(7),
      Q => vertices_address0(7),
      R => '0'
    );
\vertices_addr_reg_163_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => edges_q0(8),
      Q => vertices_address0(8),
      R => '0'
    );
\vertices_addr_reg_163_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => edges_q0(9),
      Q => vertices_address0(9),
      R => '0'
    );
vertices_ce0_INST_0: unisim.vcomponents.LUT2
    generic map(
      INIT => X"E"
    )
        port map (
      I0 => ap_CS_fsm_state4,
      I1 => ap_CS_fsm_state6,
      O => vertices_ce0
    );
vertices_ce1_INST_0: unisim.vcomponents.LUT2
    generic map(
      INIT => X"E"
    )
        port map (
      I0 => ap_CS_fsm_state7,
      I1 => ap_CS_fsm_state4,
      O => vertices_ce1
    );
vertices_we0_INST_0: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0000000000020000"
    )
        port map (
      I0 => ap_CS_fsm_state6,
      I1 => vertices_we0_INST_0_i_1_n_1,
      I2 => vertices_we0_INST_0_i_2_n_1,
      I3 => vertices_we0_INST_0_i_3_n_1,
      I4 => vertices_we0_INST_0_i_4_n_1,
      I5 => vertices_we0_INST_0_i_5_n_1,
      O => vertices_we0
    );
vertices_we0_INST_0_i_1: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FFFE"
    )
        port map (
      I0 => vertices_we0_INST_0_i_6_n_1,
      I1 => vertices_we0_INST_0_i_7_n_1,
      I2 => vertices_we0_INST_0_i_8_n_1,
      I3 => vertices_we0_INST_0_i_9_n_1,
      O => vertices_we0_INST_0_i_1_n_1
    );
vertices_we0_INST_0_i_10: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FFFE"
    )
        port map (
      I0 => \^vertices_d0\(28),
      I1 => \^vertices_d0\(19),
      I2 => \^vertices_d0\(6),
      I3 => \^vertices_d0\(3),
      O => vertices_we0_INST_0_i_10_n_1
    );
vertices_we0_INST_0_i_11: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FFFE"
    )
        port map (
      I0 => \^vertices_d0\(31),
      I1 => \^vertices_d0\(16),
      I2 => \^vertices_d0\(26),
      I3 => \^vertices_d0\(25),
      O => vertices_we0_INST_0_i_11_n_1
    );
vertices_we0_INST_0_i_12: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FFFE"
    )
        port map (
      I0 => \^vertices_d1\(30),
      I1 => \^vertices_d1\(17),
      I2 => \^vertices_d1\(27),
      I3 => \^vertices_d1\(24),
      O => vertices_we0_INST_0_i_12_n_1
    );
vertices_we0_INST_0_i_13: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FFFE"
    )
        port map (
      I0 => \^vertices_d1\(7),
      I1 => \^vertices_d1\(2),
      I2 => \^vertices_d1\(28),
      I3 => \^vertices_d1\(4),
      O => vertices_we0_INST_0_i_13_n_1
    );
vertices_we0_INST_0_i_14: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FFFE"
    )
        port map (
      I0 => \^vertices_d0\(17),
      I1 => \^vertices_d0\(11),
      I2 => \^vertices_d0\(29),
      I3 => \^vertices_d0\(8),
      O => vertices_we0_INST_0_i_14_n_1
    );
vertices_we0_INST_0_i_15: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FFFE"
    )
        port map (
      I0 => \^vertices_d1\(14),
      I1 => \^vertices_d1\(5),
      I2 => \^vertices_d1\(13),
      I3 => \^vertices_d1\(1),
      O => vertices_we0_INST_0_i_15_n_1
    );
vertices_we0_INST_0_i_16: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FFFE"
    )
        port map (
      I0 => \^vertices_d1\(22),
      I1 => \^vertices_d1\(8),
      I2 => \^vertices_d1\(23),
      I3 => \^vertices_d1\(20),
      O => vertices_we0_INST_0_i_16_n_1
    );
vertices_we0_INST_0_i_17: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FFFE"
    )
        port map (
      I0 => \^vertices_d1\(21),
      I1 => \^vertices_d1\(11),
      I2 => \^vertices_d1\(29),
      I3 => \^vertices_d1\(18),
      O => vertices_we0_INST_0_i_17_n_1
    );
vertices_we0_INST_0_i_18: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FFFE"
    )
        port map (
      I0 => \^vertices_d0\(23),
      I1 => \^vertices_d0\(9),
      I2 => \^vertices_d0\(22),
      I3 => \^vertices_d0\(21),
      O => vertices_we0_INST_0_i_18_n_1
    );
vertices_we0_INST_0_i_19: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FFFE"
    )
        port map (
      I0 => \^vertices_d0\(20),
      I1 => \^vertices_d0\(10),
      I2 => \^vertices_d1\(6),
      I3 => \^vertices_d1\(3),
      O => vertices_we0_INST_0_i_19_n_1
    );
vertices_we0_INST_0_i_2: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FFFE"
    )
        port map (
      I0 => vertices_we0_INST_0_i_10_n_1,
      I1 => vertices_we0_INST_0_i_11_n_1,
      I2 => vertices_we0_INST_0_i_12_n_1,
      I3 => vertices_we0_INST_0_i_13_n_1,
      O => vertices_we0_INST_0_i_2_n_1
    );
vertices_we0_INST_0_i_3: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFFFFFE"
    )
        port map (
      I0 => \^vertices_d0\(5),
      I1 => \^vertices_d0\(24),
      I2 => \^vertices_d0\(18),
      I3 => \^vertices_d0\(27),
      I4 => vertices_we0_INST_0_i_14_n_1,
      O => vertices_we0_INST_0_i_3_n_1
    );
vertices_we0_INST_0_i_4: unisim.vcomponents.LUT5
    generic map(
      INIT => X"00000001"
    )
        port map (
      I0 => \^vertices_d1\(15),
      I1 => \^vertices_d0\(30),
      I2 => \^vertices_d1\(0),
      I3 => \^vertices_d1\(12),
      I4 => vertices_we0_INST_0_i_15_n_1,
      O => vertices_we0_INST_0_i_4_n_1
    );
vertices_we0_INST_0_i_5: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FFFE"
    )
        port map (
      I0 => vertices_we0_INST_0_i_16_n_1,
      I1 => vertices_we0_INST_0_i_17_n_1,
      I2 => vertices_we0_INST_0_i_18_n_1,
      I3 => vertices_we0_INST_0_i_19_n_1,
      O => vertices_we0_INST_0_i_5_n_1
    );
vertices_we0_INST_0_i_6: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FFFE"
    )
        port map (
      I0 => \^vertices_d1\(26),
      I1 => \^vertices_d1\(19),
      I2 => \^vertices_d1\(31),
      I3 => \^vertices_d1\(25),
      O => vertices_we0_INST_0_i_6_n_1
    );
vertices_we0_INST_0_i_7: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FFFE"
    )
        port map (
      I0 => \^vertices_d1\(16),
      I1 => \^vertices_d1\(10),
      I2 => \^vertices_d0\(15),
      I3 => \^vertices_d1\(9),
      O => vertices_we0_INST_0_i_7_n_1
    );
vertices_we0_INST_0_i_8: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FFFE"
    )
        port map (
      I0 => \^vertices_d0\(12),
      I1 => \^vertices_d0\(0),
      I2 => \^vertices_d0\(13),
      I3 => \^vertices_d0\(1),
      O => vertices_we0_INST_0_i_8_n_1
    );
vertices_we0_INST_0_i_9: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FFFE"
    )
        port map (
      I0 => \^vertices_d0\(14),
      I1 => \^vertices_d0\(7),
      I2 => \^vertices_d0\(4),
      I3 => \^vertices_d0\(2),
      O => vertices_we0_INST_0_i_9_n_1
    );
vertices_we1_INST_0: unisim.vcomponents.LUT2
    generic map(
      INIT => X"8"
    )
        port map (
      I0 => ap_CS_fsm_state7,
      I1 => icmp_ln108_reg_185,
      O => vertices_we1
    );
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix is
  port (
    vertices_ce0 : out STD_LOGIC;
    vertices_we0 : out STD_LOGIC;
    vertices_ce1 : out STD_LOGIC;
    vertices_we1 : out STD_LOGIC;
    edges_ce0 : out STD_LOGIC;
    edges_ce1 : out STD_LOGIC;
    ap_clk : in STD_LOGIC;
    ap_rst : in STD_LOGIC;
    ap_start : in STD_LOGIC;
    ap_done : out STD_LOGIC;
    ap_idle : out STD_LOGIC;
    ap_ready : out STD_LOGIC;
    vertices_address0 : out STD_LOGIC_VECTOR ( 10 downto 0 );
    vertices_d0 : out STD_LOGIC_VECTOR ( 31 downto 0 );
    vertices_q0 : in STD_LOGIC_VECTOR ( 31 downto 0 );
    vertices_address1 : out STD_LOGIC_VECTOR ( 10 downto 0 );
    vertices_d1 : out STD_LOGIC_VECTOR ( 31 downto 0 );
    vertices_q1 : in STD_LOGIC_VECTOR ( 31 downto 0 );
    edges_address0 : out STD_LOGIC_VECTOR ( 10 downto 0 );
    edges_q0 : in STD_LOGIC_VECTOR ( 31 downto 0 );
    edges_address1 : out STD_LOGIC_VECTOR ( 10 downto 0 );
    edges_q1 : in STD_LOGIC_VECTOR ( 31 downto 0 )
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
  attribute ap_ST_fsm_state1 : string;
  attribute ap_ST_fsm_state1 of inst : label is "7'b0000001";
  attribute ap_ST_fsm_state2 : string;
  attribute ap_ST_fsm_state2 of inst : label is "7'b0000010";
  attribute ap_ST_fsm_state3 : string;
  attribute ap_ST_fsm_state3 of inst : label is "7'b0000100";
  attribute ap_ST_fsm_state4 : string;
  attribute ap_ST_fsm_state4 of inst : label is "7'b0001000";
  attribute ap_ST_fsm_state5 : string;
  attribute ap_ST_fsm_state5 of inst : label is "7'b0010000";
  attribute ap_ST_fsm_state6 : string;
  attribute ap_ST_fsm_state6 of inst : label is "7'b0100000";
  attribute ap_ST_fsm_state7 : string;
  attribute ap_ST_fsm_state7 of inst : label is "7'b1000000";
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
  attribute X_INTERFACE_INFO of edges_address0 : signal is "xilinx.com:signal:data:1.0 edges_address0 DATA";
  attribute X_INTERFACE_PARAMETER of edges_address0 : signal is "XIL_INTERFACENAME edges_address0, LAYERED_METADATA undef";
  attribute X_INTERFACE_INFO of edges_address1 : signal is "xilinx.com:signal:data:1.0 edges_address1 DATA";
  attribute X_INTERFACE_PARAMETER of edges_address1 : signal is "XIL_INTERFACENAME edges_address1, LAYERED_METADATA undef";
  attribute X_INTERFACE_INFO of edges_q0 : signal is "xilinx.com:signal:data:1.0 edges_q0 DATA";
  attribute X_INTERFACE_PARAMETER of edges_q0 : signal is "XIL_INTERFACENAME edges_q0, LAYERED_METADATA undef";
  attribute X_INTERFACE_INFO of edges_q1 : signal is "xilinx.com:signal:data:1.0 edges_q1 DATA";
  attribute X_INTERFACE_PARAMETER of edges_q1 : signal is "XIL_INTERFACENAME edges_q1, LAYERED_METADATA undef";
  attribute X_INTERFACE_INFO of vertices_address0 : signal is "xilinx.com:signal:data:1.0 vertices_address0 DATA";
  attribute X_INTERFACE_PARAMETER of vertices_address0 : signal is "XIL_INTERFACENAME vertices_address0, LAYERED_METADATA undef";
  attribute X_INTERFACE_INFO of vertices_address1 : signal is "xilinx.com:signal:data:1.0 vertices_address1 DATA";
  attribute X_INTERFACE_PARAMETER of vertices_address1 : signal is "XIL_INTERFACENAME vertices_address1, LAYERED_METADATA undef";
  attribute X_INTERFACE_INFO of vertices_d0 : signal is "xilinx.com:signal:data:1.0 vertices_d0 DATA";
  attribute X_INTERFACE_PARAMETER of vertices_d0 : signal is "XIL_INTERFACENAME vertices_d0, LAYERED_METADATA undef";
  attribute X_INTERFACE_INFO of vertices_d1 : signal is "xilinx.com:signal:data:1.0 vertices_d1 DATA";
  attribute X_INTERFACE_PARAMETER of vertices_d1 : signal is "XIL_INTERFACENAME vertices_d1, LAYERED_METADATA undef";
  attribute X_INTERFACE_INFO of vertices_q0 : signal is "xilinx.com:signal:data:1.0 vertices_q0 DATA";
  attribute X_INTERFACE_PARAMETER of vertices_q0 : signal is "XIL_INTERFACENAME vertices_q0, LAYERED_METADATA undef";
  attribute X_INTERFACE_INFO of vertices_q1 : signal is "xilinx.com:signal:data:1.0 vertices_q1 DATA";
  attribute X_INTERFACE_PARAMETER of vertices_q1 : signal is "XIL_INTERFACENAME vertices_q1, LAYERED_METADATA undef";
begin
inst: entity work.decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect
     port map (
      ap_clk => ap_clk,
      ap_done => ap_done,
      ap_idle => ap_idle,
      ap_ready => ap_ready,
      ap_rst => ap_rst,
      ap_start => ap_start,
      edges_address0(10 downto 0) => edges_address0(10 downto 0),
      edges_address1(10 downto 0) => edges_address1(10 downto 0),
      edges_ce0 => edges_ce0,
      edges_ce1 => edges_ce1,
      edges_q0(31 downto 0) => edges_q0(31 downto 0),
      edges_q1(31 downto 0) => edges_q1(31 downto 0),
      vertices_address0(10 downto 0) => vertices_address0(10 downto 0),
      vertices_address1(10 downto 0) => vertices_address1(10 downto 0),
      vertices_ce0 => vertices_ce0,
      vertices_ce1 => vertices_ce1,
      vertices_d0(31 downto 0) => vertices_d0(31 downto 0),
      vertices_d1(31 downto 0) => vertices_d1(31 downto 0),
      vertices_q0(31 downto 0) => vertices_q0(31 downto 0),
      vertices_q1(31 downto 0) => vertices_q1(31 downto 0),
      vertices_we0 => vertices_we0,
      vertices_we1 => vertices_we1
    );
end STRUCTURE;
