-- Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2019.2 (lin64) Build 2708876 Wed Nov  6 21:39:14 MST 2019
-- Date        : Sun Jun 25 13:46:30 2023
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
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_MulnS_0 is
  port (
    A_d0 : out STD_LOGIC_VECTOR ( 31 downto 0 );
    Q : in STD_LOGIC_VECTOR ( 0 to 0 );
    ap_clk : in STD_LOGIC;
    grp_fu_190_p1 : in STD_LOGIC_VECTOR ( 30 downto 0 );
    A : in STD_LOGIC_VECTOR ( 0 to 0 );
    A_q0 : in STD_LOGIC_VECTOR ( 16 downto 0 );
    buff2_reg_0 : in STD_LOGIC_VECTOR ( 14 downto 0 )
  );
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_MulnS_0;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_MulnS_0 is
  signal \A_d0[13]_INST_0_n_1\ : STD_LOGIC;
  signal \A_d0[13]_INST_0_n_2\ : STD_LOGIC;
  signal \A_d0[13]_INST_0_n_3\ : STD_LOGIC;
  signal \A_d0[13]_INST_0_n_4\ : STD_LOGIC;
  signal \A_d0[17]_INST_0_n_1\ : STD_LOGIC;
  signal \A_d0[17]_INST_0_n_2\ : STD_LOGIC;
  signal \A_d0[17]_INST_0_n_3\ : STD_LOGIC;
  signal \A_d0[17]_INST_0_n_4\ : STD_LOGIC;
  signal \A_d0[1]_INST_0_i_1_n_1\ : STD_LOGIC;
  signal \A_d0[1]_INST_0_n_1\ : STD_LOGIC;
  signal \A_d0[1]_INST_0_n_2\ : STD_LOGIC;
  signal \A_d0[1]_INST_0_n_3\ : STD_LOGIC;
  signal \A_d0[1]_INST_0_n_4\ : STD_LOGIC;
  signal \A_d0[21]_INST_0_n_1\ : STD_LOGIC;
  signal \A_d0[21]_INST_0_n_2\ : STD_LOGIC;
  signal \A_d0[21]_INST_0_n_3\ : STD_LOGIC;
  signal \A_d0[21]_INST_0_n_4\ : STD_LOGIC;
  signal \A_d0[25]_INST_0_n_1\ : STD_LOGIC;
  signal \A_d0[25]_INST_0_n_2\ : STD_LOGIC;
  signal \A_d0[25]_INST_0_n_3\ : STD_LOGIC;
  signal \A_d0[25]_INST_0_n_4\ : STD_LOGIC;
  signal \A_d0[29]_INST_0_n_3\ : STD_LOGIC;
  signal \A_d0[29]_INST_0_n_4\ : STD_LOGIC;
  signal \A_d0[5]_INST_0_i_1_n_1\ : STD_LOGIC;
  signal \A_d0[5]_INST_0_i_2_n_1\ : STD_LOGIC;
  signal \A_d0[5]_INST_0_n_1\ : STD_LOGIC;
  signal \A_d0[5]_INST_0_n_2\ : STD_LOGIC;
  signal \A_d0[5]_INST_0_n_3\ : STD_LOGIC;
  signal \A_d0[5]_INST_0_n_4\ : STD_LOGIC;
  signal \A_d0[9]_INST_0_n_1\ : STD_LOGIC;
  signal \A_d0[9]_INST_0_n_2\ : STD_LOGIC;
  signal \A_d0[9]_INST_0_n_3\ : STD_LOGIC;
  signal \A_d0[9]_INST_0_n_4\ : STD_LOGIC;
  signal buff0_reg_n_100 : STD_LOGIC;
  signal buff0_reg_n_101 : STD_LOGIC;
  signal buff0_reg_n_102 : STD_LOGIC;
  signal buff0_reg_n_103 : STD_LOGIC;
  signal buff0_reg_n_104 : STD_LOGIC;
  signal buff0_reg_n_105 : STD_LOGIC;
  signal buff0_reg_n_106 : STD_LOGIC;
  signal buff0_reg_n_107 : STD_LOGIC;
  signal buff0_reg_n_108 : STD_LOGIC;
  signal buff0_reg_n_109 : STD_LOGIC;
  signal buff0_reg_n_110 : STD_LOGIC;
  signal buff0_reg_n_111 : STD_LOGIC;
  signal buff0_reg_n_112 : STD_LOGIC;
  signal buff0_reg_n_113 : STD_LOGIC;
  signal buff0_reg_n_114 : STD_LOGIC;
  signal buff0_reg_n_115 : STD_LOGIC;
  signal buff0_reg_n_116 : STD_LOGIC;
  signal buff0_reg_n_117 : STD_LOGIC;
  signal buff0_reg_n_118 : STD_LOGIC;
  signal buff0_reg_n_119 : STD_LOGIC;
  signal buff0_reg_n_120 : STD_LOGIC;
  signal buff0_reg_n_121 : STD_LOGIC;
  signal buff0_reg_n_122 : STD_LOGIC;
  signal buff0_reg_n_123 : STD_LOGIC;
  signal buff0_reg_n_124 : STD_LOGIC;
  signal buff0_reg_n_125 : STD_LOGIC;
  signal buff0_reg_n_126 : STD_LOGIC;
  signal buff0_reg_n_127 : STD_LOGIC;
  signal buff0_reg_n_128 : STD_LOGIC;
  signal buff0_reg_n_129 : STD_LOGIC;
  signal buff0_reg_n_130 : STD_LOGIC;
  signal buff0_reg_n_131 : STD_LOGIC;
  signal buff0_reg_n_132 : STD_LOGIC;
  signal buff0_reg_n_133 : STD_LOGIC;
  signal buff0_reg_n_134 : STD_LOGIC;
  signal buff0_reg_n_135 : STD_LOGIC;
  signal buff0_reg_n_136 : STD_LOGIC;
  signal buff0_reg_n_137 : STD_LOGIC;
  signal buff0_reg_n_138 : STD_LOGIC;
  signal buff0_reg_n_139 : STD_LOGIC;
  signal buff0_reg_n_140 : STD_LOGIC;
  signal buff0_reg_n_141 : STD_LOGIC;
  signal buff0_reg_n_142 : STD_LOGIC;
  signal buff0_reg_n_143 : STD_LOGIC;
  signal buff0_reg_n_144 : STD_LOGIC;
  signal buff0_reg_n_145 : STD_LOGIC;
  signal buff0_reg_n_146 : STD_LOGIC;
  signal buff0_reg_n_147 : STD_LOGIC;
  signal buff0_reg_n_148 : STD_LOGIC;
  signal buff0_reg_n_149 : STD_LOGIC;
  signal buff0_reg_n_150 : STD_LOGIC;
  signal buff0_reg_n_151 : STD_LOGIC;
  signal buff0_reg_n_152 : STD_LOGIC;
  signal buff0_reg_n_153 : STD_LOGIC;
  signal buff0_reg_n_154 : STD_LOGIC;
  signal buff0_reg_n_59 : STD_LOGIC;
  signal buff0_reg_n_60 : STD_LOGIC;
  signal buff0_reg_n_61 : STD_LOGIC;
  signal buff0_reg_n_62 : STD_LOGIC;
  signal buff0_reg_n_63 : STD_LOGIC;
  signal buff0_reg_n_64 : STD_LOGIC;
  signal buff0_reg_n_65 : STD_LOGIC;
  signal buff0_reg_n_66 : STD_LOGIC;
  signal buff0_reg_n_67 : STD_LOGIC;
  signal buff0_reg_n_68 : STD_LOGIC;
  signal buff0_reg_n_69 : STD_LOGIC;
  signal buff0_reg_n_70 : STD_LOGIC;
  signal buff0_reg_n_71 : STD_LOGIC;
  signal buff0_reg_n_72 : STD_LOGIC;
  signal buff0_reg_n_73 : STD_LOGIC;
  signal buff0_reg_n_74 : STD_LOGIC;
  signal buff0_reg_n_75 : STD_LOGIC;
  signal buff0_reg_n_76 : STD_LOGIC;
  signal buff0_reg_n_77 : STD_LOGIC;
  signal buff0_reg_n_78 : STD_LOGIC;
  signal buff0_reg_n_79 : STD_LOGIC;
  signal buff0_reg_n_80 : STD_LOGIC;
  signal buff0_reg_n_81 : STD_LOGIC;
  signal buff0_reg_n_82 : STD_LOGIC;
  signal buff0_reg_n_83 : STD_LOGIC;
  signal buff0_reg_n_84 : STD_LOGIC;
  signal buff0_reg_n_85 : STD_LOGIC;
  signal buff0_reg_n_86 : STD_LOGIC;
  signal buff0_reg_n_87 : STD_LOGIC;
  signal buff0_reg_n_88 : STD_LOGIC;
  signal buff0_reg_n_89 : STD_LOGIC;
  signal buff0_reg_n_90 : STD_LOGIC;
  signal buff0_reg_n_91 : STD_LOGIC;
  signal buff0_reg_n_92 : STD_LOGIC;
  signal buff0_reg_n_93 : STD_LOGIC;
  signal buff0_reg_n_94 : STD_LOGIC;
  signal buff0_reg_n_95 : STD_LOGIC;
  signal buff0_reg_n_96 : STD_LOGIC;
  signal buff0_reg_n_97 : STD_LOGIC;
  signal buff0_reg_n_98 : STD_LOGIC;
  signal buff0_reg_n_99 : STD_LOGIC;
  signal buff1_reg_n_107 : STD_LOGIC;
  signal buff1_reg_n_108 : STD_LOGIC;
  signal buff1_reg_n_109 : STD_LOGIC;
  signal buff1_reg_n_110 : STD_LOGIC;
  signal buff1_reg_n_111 : STD_LOGIC;
  signal buff1_reg_n_112 : STD_LOGIC;
  signal buff1_reg_n_113 : STD_LOGIC;
  signal buff1_reg_n_114 : STD_LOGIC;
  signal buff1_reg_n_115 : STD_LOGIC;
  signal buff1_reg_n_116 : STD_LOGIC;
  signal buff1_reg_n_117 : STD_LOGIC;
  signal buff1_reg_n_118 : STD_LOGIC;
  signal buff1_reg_n_119 : STD_LOGIC;
  signal buff1_reg_n_120 : STD_LOGIC;
  signal buff1_reg_n_121 : STD_LOGIC;
  signal buff1_reg_n_122 : STD_LOGIC;
  signal buff1_reg_n_123 : STD_LOGIC;
  signal buff1_reg_n_124 : STD_LOGIC;
  signal buff1_reg_n_125 : STD_LOGIC;
  signal buff1_reg_n_126 : STD_LOGIC;
  signal buff1_reg_n_127 : STD_LOGIC;
  signal buff1_reg_n_128 : STD_LOGIC;
  signal buff1_reg_n_129 : STD_LOGIC;
  signal buff1_reg_n_130 : STD_LOGIC;
  signal buff1_reg_n_131 : STD_LOGIC;
  signal buff1_reg_n_132 : STD_LOGIC;
  signal buff1_reg_n_133 : STD_LOGIC;
  signal buff1_reg_n_134 : STD_LOGIC;
  signal buff1_reg_n_135 : STD_LOGIC;
  signal buff1_reg_n_136 : STD_LOGIC;
  signal buff1_reg_n_137 : STD_LOGIC;
  signal buff1_reg_n_138 : STD_LOGIC;
  signal buff1_reg_n_139 : STD_LOGIC;
  signal buff1_reg_n_140 : STD_LOGIC;
  signal buff1_reg_n_141 : STD_LOGIC;
  signal buff1_reg_n_142 : STD_LOGIC;
  signal buff1_reg_n_143 : STD_LOGIC;
  signal buff1_reg_n_144 : STD_LOGIC;
  signal buff1_reg_n_145 : STD_LOGIC;
  signal buff1_reg_n_146 : STD_LOGIC;
  signal buff1_reg_n_147 : STD_LOGIC;
  signal buff1_reg_n_148 : STD_LOGIC;
  signal buff1_reg_n_149 : STD_LOGIC;
  signal buff1_reg_n_150 : STD_LOGIC;
  signal buff1_reg_n_151 : STD_LOGIC;
  signal buff1_reg_n_152 : STD_LOGIC;
  signal buff1_reg_n_153 : STD_LOGIC;
  signal buff1_reg_n_154 : STD_LOGIC;
  signal \buff1_reg_n_1_[0]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[10]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[11]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[12]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[13]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[14]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[15]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[16]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[1]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[2]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[3]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[4]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[5]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[6]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[7]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[8]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[9]\ : STD_LOGIC;
  signal \buff2_reg__0\ : STD_LOGIC_VECTOR ( 31 downto 1 );
  signal buff2_reg_n_59 : STD_LOGIC;
  signal buff2_reg_n_60 : STD_LOGIC;
  signal buff2_reg_n_61 : STD_LOGIC;
  signal buff2_reg_n_62 : STD_LOGIC;
  signal buff2_reg_n_63 : STD_LOGIC;
  signal buff2_reg_n_64 : STD_LOGIC;
  signal buff2_reg_n_65 : STD_LOGIC;
  signal buff2_reg_n_66 : STD_LOGIC;
  signal buff2_reg_n_67 : STD_LOGIC;
  signal buff2_reg_n_68 : STD_LOGIC;
  signal buff2_reg_n_69 : STD_LOGIC;
  signal buff2_reg_n_70 : STD_LOGIC;
  signal buff2_reg_n_71 : STD_LOGIC;
  signal buff2_reg_n_72 : STD_LOGIC;
  signal buff2_reg_n_73 : STD_LOGIC;
  signal buff2_reg_n_74 : STD_LOGIC;
  signal buff2_reg_n_75 : STD_LOGIC;
  signal buff2_reg_n_76 : STD_LOGIC;
  signal buff2_reg_n_77 : STD_LOGIC;
  signal buff2_reg_n_78 : STD_LOGIC;
  signal buff2_reg_n_79 : STD_LOGIC;
  signal buff2_reg_n_80 : STD_LOGIC;
  signal buff2_reg_n_81 : STD_LOGIC;
  signal buff2_reg_n_82 : STD_LOGIC;
  signal buff2_reg_n_83 : STD_LOGIC;
  signal buff2_reg_n_84 : STD_LOGIC;
  signal buff2_reg_n_85 : STD_LOGIC;
  signal buff2_reg_n_86 : STD_LOGIC;
  signal buff2_reg_n_87 : STD_LOGIC;
  signal buff2_reg_n_88 : STD_LOGIC;
  signal buff2_reg_n_89 : STD_LOGIC;
  signal buff2_reg_n_90 : STD_LOGIC;
  signal buff2_reg_n_91 : STD_LOGIC;
  signal \NLW_A_d0[29]_INST_0_CO_UNCONNECTED\ : STD_LOGIC_VECTOR ( 3 downto 2 );
  signal \NLW_A_d0[29]_INST_0_O_UNCONNECTED\ : STD_LOGIC_VECTOR ( 3 to 3 );
  signal NLW_buff0_reg_CARRYCASCOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_MULTSIGNOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_OVERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_PATTERNBDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_PATTERNDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_UNDERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_ACOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 29 downto 0 );
  signal NLW_buff0_reg_BCOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 17 downto 0 );
  signal NLW_buff0_reg_CARRYOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_buff1_reg_CARRYCASCOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_MULTSIGNOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_OVERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_PATTERNBDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_PATTERNDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_UNDERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_ACOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 29 downto 0 );
  signal NLW_buff1_reg_BCOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 17 downto 0 );
  signal NLW_buff1_reg_CARRYOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_buff1_reg_P_UNCONNECTED : STD_LOGIC_VECTOR ( 47 downto 0 );
  signal NLW_buff2_reg_CARRYCASCOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_MULTSIGNOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_OVERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_PATTERNBDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_PATTERNDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_UNDERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_ACOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 29 downto 0 );
  signal NLW_buff2_reg_BCOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 17 downto 0 );
  signal NLW_buff2_reg_CARRYOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_buff2_reg_PCOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 47 downto 0 );
begin
\A_d0[13]_INST_0\: unisim.vcomponents.CARRY4
     port map (
      CI => \A_d0[9]_INST_0_n_1\,
      CO(3) => \A_d0[13]_INST_0_n_1\,
      CO(2) => \A_d0[13]_INST_0_n_2\,
      CO(1) => \A_d0[13]_INST_0_n_3\,
      CO(0) => \A_d0[13]_INST_0_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => A_d0(16 downto 13),
      S(3 downto 0) => \buff2_reg__0\(16 downto 13)
    );
\A_d0[17]_INST_0\: unisim.vcomponents.CARRY4
     port map (
      CI => \A_d0[13]_INST_0_n_1\,
      CO(3) => \A_d0[17]_INST_0_n_1\,
      CO(2) => \A_d0[17]_INST_0_n_2\,
      CO(1) => \A_d0[17]_INST_0_n_3\,
      CO(0) => \A_d0[17]_INST_0_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => A_d0(20 downto 17),
      S(3 downto 0) => \buff2_reg__0\(20 downto 17)
    );
\A_d0[1]_INST_0\: unisim.vcomponents.CARRY4
     port map (
      CI => '0',
      CO(3) => \A_d0[1]_INST_0_n_1\,
      CO(2) => \A_d0[1]_INST_0_n_2\,
      CO(1) => \A_d0[1]_INST_0_n_3\,
      CO(0) => \A_d0[1]_INST_0_n_4\,
      CYINIT => '0',
      DI(3 downto 2) => B"00",
      DI(1) => \buff2_reg__0\(2),
      DI(0) => '0',
      O(3 downto 0) => A_d0(4 downto 1),
      S(3 downto 2) => \buff2_reg__0\(4 downto 3),
      S(1) => \A_d0[1]_INST_0_i_1_n_1\,
      S(0) => \buff2_reg__0\(1)
    );
\A_d0[1]_INST_0_i_1\: unisim.vcomponents.LUT1
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => \buff2_reg__0\(2),
      O => \A_d0[1]_INST_0_i_1_n_1\
    );
\A_d0[21]_INST_0\: unisim.vcomponents.CARRY4
     port map (
      CI => \A_d0[17]_INST_0_n_1\,
      CO(3) => \A_d0[21]_INST_0_n_1\,
      CO(2) => \A_d0[21]_INST_0_n_2\,
      CO(1) => \A_d0[21]_INST_0_n_3\,
      CO(0) => \A_d0[21]_INST_0_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => A_d0(24 downto 21),
      S(3 downto 0) => \buff2_reg__0\(24 downto 21)
    );
\A_d0[25]_INST_0\: unisim.vcomponents.CARRY4
     port map (
      CI => \A_d0[21]_INST_0_n_1\,
      CO(3) => \A_d0[25]_INST_0_n_1\,
      CO(2) => \A_d0[25]_INST_0_n_2\,
      CO(1) => \A_d0[25]_INST_0_n_3\,
      CO(0) => \A_d0[25]_INST_0_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => A_d0(28 downto 25),
      S(3 downto 0) => \buff2_reg__0\(28 downto 25)
    );
\A_d0[29]_INST_0\: unisim.vcomponents.CARRY4
     port map (
      CI => \A_d0[25]_INST_0_n_1\,
      CO(3 downto 2) => \NLW_A_d0[29]_INST_0_CO_UNCONNECTED\(3 downto 2),
      CO(1) => \A_d0[29]_INST_0_n_3\,
      CO(0) => \A_d0[29]_INST_0_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3) => \NLW_A_d0[29]_INST_0_O_UNCONNECTED\(3),
      O(2 downto 0) => A_d0(31 downto 29),
      S(3) => '0',
      S(2 downto 0) => \buff2_reg__0\(31 downto 29)
    );
\A_d0[5]_INST_0\: unisim.vcomponents.CARRY4
     port map (
      CI => \A_d0[1]_INST_0_n_1\,
      CO(3) => \A_d0[5]_INST_0_n_1\,
      CO(2) => \A_d0[5]_INST_0_n_2\,
      CO(1) => \A_d0[5]_INST_0_n_3\,
      CO(0) => \A_d0[5]_INST_0_n_4\,
      CYINIT => '0',
      DI(3 downto 2) => B"00",
      DI(1 downto 0) => \buff2_reg__0\(6 downto 5),
      O(3 downto 0) => A_d0(8 downto 5),
      S(3 downto 2) => \buff2_reg__0\(8 downto 7),
      S(1) => \A_d0[5]_INST_0_i_1_n_1\,
      S(0) => \A_d0[5]_INST_0_i_2_n_1\
    );
\A_d0[5]_INST_0_i_1\: unisim.vcomponents.LUT1
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => \buff2_reg__0\(6),
      O => \A_d0[5]_INST_0_i_1_n_1\
    );
\A_d0[5]_INST_0_i_2\: unisim.vcomponents.LUT1
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => \buff2_reg__0\(5),
      O => \A_d0[5]_INST_0_i_2_n_1\
    );
\A_d0[9]_INST_0\: unisim.vcomponents.CARRY4
     port map (
      CI => \A_d0[5]_INST_0_n_1\,
      CO(3) => \A_d0[9]_INST_0_n_1\,
      CO(2) => \A_d0[9]_INST_0_n_2\,
      CO(1) => \A_d0[9]_INST_0_n_3\,
      CO(0) => \A_d0[9]_INST_0_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => A_d0(12 downto 9),
      S(3 downto 0) => \buff2_reg__0\(12 downto 9)
    );
buff0_reg: unisim.vcomponents.DSP48E1
    generic map(
      ACASCREG => 2,
      ADREG => 1,
      ALUMODEREG => 0,
      AREG => 2,
      AUTORESET_PATDET => "NO_RESET",
      A_INPUT => "DIRECT",
      BCASCREG => 1,
      BREG => 1,
      B_INPUT => "DIRECT",
      CARRYINREG => 0,
      CARRYINSELREG => 0,
      CREG => 0,
      DREG => 1,
      INMODEREG => 0,
      MASK => X"3FFFFFFFFFFF",
      MREG => 0,
      OPMODEREG => 0,
      PATTERN => X"000000000000",
      PREG => 1,
      SEL_MASK => "MASK",
      SEL_PATTERN => "PATTERN",
      USE_DPORT => false,
      USE_MULT => "MULTIPLY",
      USE_PATTERN_DETECT => "NO_PATDET",
      USE_SIMD => "ONE48"
    )
        port map (
      A(29 downto 17) => B"0000000000000",
      A(16 downto 0) => A_q0(16 downto 0),
      ACIN(29 downto 0) => B"000000000000000000000000000000",
      ACOUT(29 downto 0) => NLW_buff0_reg_ACOUT_UNCONNECTED(29 downto 0),
      ALUMODE(3 downto 0) => B"0000",
      B(17) => '0',
      B(16 downto 1) => grp_fu_190_p1(15 downto 0),
      B(0) => A(0),
      BCIN(17 downto 0) => B"000000000000000000",
      BCOUT(17 downto 0) => NLW_buff0_reg_BCOUT_UNCONNECTED(17 downto 0),
      C(47 downto 0) => B"111111111111111111111111111111111111111111111111",
      CARRYCASCIN => '0',
      CARRYCASCOUT => NLW_buff0_reg_CARRYCASCOUT_UNCONNECTED,
      CARRYIN => '0',
      CARRYINSEL(2 downto 0) => B"000",
      CARRYOUT(3 downto 0) => NLW_buff0_reg_CARRYOUT_UNCONNECTED(3 downto 0),
      CEA1 => Q(0),
      CEA2 => '1',
      CEAD => '0',
      CEALUMODE => '0',
      CEB1 => '0',
      CEB2 => '1',
      CEC => '0',
      CECARRYIN => '0',
      CECTRL => '0',
      CED => '0',
      CEINMODE => '0',
      CEM => '0',
      CEP => '1',
      CLK => ap_clk,
      D(24 downto 0) => B"0000000000000000000000000",
      INMODE(4 downto 0) => B"00000",
      MULTSIGNIN => '0',
      MULTSIGNOUT => NLW_buff0_reg_MULTSIGNOUT_UNCONNECTED,
      OPMODE(6 downto 0) => B"0000101",
      OVERFLOW => NLW_buff0_reg_OVERFLOW_UNCONNECTED,
      P(47) => buff0_reg_n_59,
      P(46) => buff0_reg_n_60,
      P(45) => buff0_reg_n_61,
      P(44) => buff0_reg_n_62,
      P(43) => buff0_reg_n_63,
      P(42) => buff0_reg_n_64,
      P(41) => buff0_reg_n_65,
      P(40) => buff0_reg_n_66,
      P(39) => buff0_reg_n_67,
      P(38) => buff0_reg_n_68,
      P(37) => buff0_reg_n_69,
      P(36) => buff0_reg_n_70,
      P(35) => buff0_reg_n_71,
      P(34) => buff0_reg_n_72,
      P(33) => buff0_reg_n_73,
      P(32) => buff0_reg_n_74,
      P(31) => buff0_reg_n_75,
      P(30) => buff0_reg_n_76,
      P(29) => buff0_reg_n_77,
      P(28) => buff0_reg_n_78,
      P(27) => buff0_reg_n_79,
      P(26) => buff0_reg_n_80,
      P(25) => buff0_reg_n_81,
      P(24) => buff0_reg_n_82,
      P(23) => buff0_reg_n_83,
      P(22) => buff0_reg_n_84,
      P(21) => buff0_reg_n_85,
      P(20) => buff0_reg_n_86,
      P(19) => buff0_reg_n_87,
      P(18) => buff0_reg_n_88,
      P(17) => buff0_reg_n_89,
      P(16) => buff0_reg_n_90,
      P(15) => buff0_reg_n_91,
      P(14) => buff0_reg_n_92,
      P(13) => buff0_reg_n_93,
      P(12) => buff0_reg_n_94,
      P(11) => buff0_reg_n_95,
      P(10) => buff0_reg_n_96,
      P(9) => buff0_reg_n_97,
      P(8) => buff0_reg_n_98,
      P(7) => buff0_reg_n_99,
      P(6) => buff0_reg_n_100,
      P(5) => buff0_reg_n_101,
      P(4) => buff0_reg_n_102,
      P(3) => buff0_reg_n_103,
      P(2) => buff0_reg_n_104,
      P(1) => buff0_reg_n_105,
      P(0) => buff0_reg_n_106,
      PATTERNBDETECT => NLW_buff0_reg_PATTERNBDETECT_UNCONNECTED,
      PATTERNDETECT => NLW_buff0_reg_PATTERNDETECT_UNCONNECTED,
      PCIN(47 downto 0) => B"000000000000000000000000000000000000000000000000",
      PCOUT(47) => buff0_reg_n_107,
      PCOUT(46) => buff0_reg_n_108,
      PCOUT(45) => buff0_reg_n_109,
      PCOUT(44) => buff0_reg_n_110,
      PCOUT(43) => buff0_reg_n_111,
      PCOUT(42) => buff0_reg_n_112,
      PCOUT(41) => buff0_reg_n_113,
      PCOUT(40) => buff0_reg_n_114,
      PCOUT(39) => buff0_reg_n_115,
      PCOUT(38) => buff0_reg_n_116,
      PCOUT(37) => buff0_reg_n_117,
      PCOUT(36) => buff0_reg_n_118,
      PCOUT(35) => buff0_reg_n_119,
      PCOUT(34) => buff0_reg_n_120,
      PCOUT(33) => buff0_reg_n_121,
      PCOUT(32) => buff0_reg_n_122,
      PCOUT(31) => buff0_reg_n_123,
      PCOUT(30) => buff0_reg_n_124,
      PCOUT(29) => buff0_reg_n_125,
      PCOUT(28) => buff0_reg_n_126,
      PCOUT(27) => buff0_reg_n_127,
      PCOUT(26) => buff0_reg_n_128,
      PCOUT(25) => buff0_reg_n_129,
      PCOUT(24) => buff0_reg_n_130,
      PCOUT(23) => buff0_reg_n_131,
      PCOUT(22) => buff0_reg_n_132,
      PCOUT(21) => buff0_reg_n_133,
      PCOUT(20) => buff0_reg_n_134,
      PCOUT(19) => buff0_reg_n_135,
      PCOUT(18) => buff0_reg_n_136,
      PCOUT(17) => buff0_reg_n_137,
      PCOUT(16) => buff0_reg_n_138,
      PCOUT(15) => buff0_reg_n_139,
      PCOUT(14) => buff0_reg_n_140,
      PCOUT(13) => buff0_reg_n_141,
      PCOUT(12) => buff0_reg_n_142,
      PCOUT(11) => buff0_reg_n_143,
      PCOUT(10) => buff0_reg_n_144,
      PCOUT(9) => buff0_reg_n_145,
      PCOUT(8) => buff0_reg_n_146,
      PCOUT(7) => buff0_reg_n_147,
      PCOUT(6) => buff0_reg_n_148,
      PCOUT(5) => buff0_reg_n_149,
      PCOUT(4) => buff0_reg_n_150,
      PCOUT(3) => buff0_reg_n_151,
      PCOUT(2) => buff0_reg_n_152,
      PCOUT(1) => buff0_reg_n_153,
      PCOUT(0) => buff0_reg_n_154,
      RSTA => '0',
      RSTALLCARRYIN => '0',
      RSTALUMODE => '0',
      RSTB => '0',
      RSTC => '0',
      RSTCTRL => '0',
      RSTD => '0',
      RSTINMODE => '0',
      RSTM => '0',
      RSTP => '0',
      UNDERFLOW => NLW_buff0_reg_UNDERFLOW_UNCONNECTED
    );
buff1_reg: unisim.vcomponents.DSP48E1
    generic map(
      ACASCREG => 2,
      ADREG => 1,
      ALUMODEREG => 0,
      AREG => 2,
      AUTORESET_PATDET => "NO_RESET",
      A_INPUT => "DIRECT",
      BCASCREG => 1,
      BREG => 1,
      B_INPUT => "DIRECT",
      CARRYINREG => 0,
      CARRYINSELREG => 0,
      CREG => 1,
      DREG => 1,
      INMODEREG => 0,
      MASK => X"3FFFFFFFFFFF",
      MREG => 1,
      OPMODEREG => 0,
      PATTERN => X"000000000000",
      PREG => 1,
      SEL_MASK => "MASK",
      SEL_PATTERN => "PATTERN",
      USE_DPORT => false,
      USE_MULT => "MULTIPLY",
      USE_PATTERN_DETECT => "NO_PATDET",
      USE_SIMD => "ONE48"
    )
        port map (
      A(29 downto 17) => B"0000000000000",
      A(16 downto 0) => A_q0(16 downto 0),
      ACIN(29 downto 0) => B"000000000000000000000000000000",
      ACOUT(29 downto 0) => NLW_buff1_reg_ACOUT_UNCONNECTED(29 downto 0),
      ALUMODE(3 downto 0) => B"0000",
      B(17) => grp_fu_190_p1(30),
      B(16) => grp_fu_190_p1(30),
      B(15) => grp_fu_190_p1(30),
      B(14 downto 0) => grp_fu_190_p1(30 downto 16),
      BCIN(17 downto 0) => B"000000000000000000",
      BCOUT(17 downto 0) => NLW_buff1_reg_BCOUT_UNCONNECTED(17 downto 0),
      C(47 downto 0) => B"111111111111111111111111111111111111111111111111",
      CARRYCASCIN => '0',
      CARRYCASCOUT => NLW_buff1_reg_CARRYCASCOUT_UNCONNECTED,
      CARRYIN => '0',
      CARRYINSEL(2 downto 0) => B"000",
      CARRYOUT(3 downto 0) => NLW_buff1_reg_CARRYOUT_UNCONNECTED(3 downto 0),
      CEA1 => Q(0),
      CEA2 => '1',
      CEAD => '0',
      CEALUMODE => '0',
      CEB1 => '0',
      CEB2 => '1',
      CEC => '0',
      CECARRYIN => '0',
      CECTRL => '0',
      CED => '0',
      CEINMODE => '0',
      CEM => '1',
      CEP => '1',
      CLK => ap_clk,
      D(24 downto 0) => B"0000000000000000000000000",
      INMODE(4 downto 0) => B"00000",
      MULTSIGNIN => '0',
      MULTSIGNOUT => NLW_buff1_reg_MULTSIGNOUT_UNCONNECTED,
      OPMODE(6 downto 0) => B"1010101",
      OVERFLOW => NLW_buff1_reg_OVERFLOW_UNCONNECTED,
      P(47 downto 0) => NLW_buff1_reg_P_UNCONNECTED(47 downto 0),
      PATTERNBDETECT => NLW_buff1_reg_PATTERNBDETECT_UNCONNECTED,
      PATTERNDETECT => NLW_buff1_reg_PATTERNDETECT_UNCONNECTED,
      PCIN(47) => buff0_reg_n_107,
      PCIN(46) => buff0_reg_n_108,
      PCIN(45) => buff0_reg_n_109,
      PCIN(44) => buff0_reg_n_110,
      PCIN(43) => buff0_reg_n_111,
      PCIN(42) => buff0_reg_n_112,
      PCIN(41) => buff0_reg_n_113,
      PCIN(40) => buff0_reg_n_114,
      PCIN(39) => buff0_reg_n_115,
      PCIN(38) => buff0_reg_n_116,
      PCIN(37) => buff0_reg_n_117,
      PCIN(36) => buff0_reg_n_118,
      PCIN(35) => buff0_reg_n_119,
      PCIN(34) => buff0_reg_n_120,
      PCIN(33) => buff0_reg_n_121,
      PCIN(32) => buff0_reg_n_122,
      PCIN(31) => buff0_reg_n_123,
      PCIN(30) => buff0_reg_n_124,
      PCIN(29) => buff0_reg_n_125,
      PCIN(28) => buff0_reg_n_126,
      PCIN(27) => buff0_reg_n_127,
      PCIN(26) => buff0_reg_n_128,
      PCIN(25) => buff0_reg_n_129,
      PCIN(24) => buff0_reg_n_130,
      PCIN(23) => buff0_reg_n_131,
      PCIN(22) => buff0_reg_n_132,
      PCIN(21) => buff0_reg_n_133,
      PCIN(20) => buff0_reg_n_134,
      PCIN(19) => buff0_reg_n_135,
      PCIN(18) => buff0_reg_n_136,
      PCIN(17) => buff0_reg_n_137,
      PCIN(16) => buff0_reg_n_138,
      PCIN(15) => buff0_reg_n_139,
      PCIN(14) => buff0_reg_n_140,
      PCIN(13) => buff0_reg_n_141,
      PCIN(12) => buff0_reg_n_142,
      PCIN(11) => buff0_reg_n_143,
      PCIN(10) => buff0_reg_n_144,
      PCIN(9) => buff0_reg_n_145,
      PCIN(8) => buff0_reg_n_146,
      PCIN(7) => buff0_reg_n_147,
      PCIN(6) => buff0_reg_n_148,
      PCIN(5) => buff0_reg_n_149,
      PCIN(4) => buff0_reg_n_150,
      PCIN(3) => buff0_reg_n_151,
      PCIN(2) => buff0_reg_n_152,
      PCIN(1) => buff0_reg_n_153,
      PCIN(0) => buff0_reg_n_154,
      PCOUT(47) => buff1_reg_n_107,
      PCOUT(46) => buff1_reg_n_108,
      PCOUT(45) => buff1_reg_n_109,
      PCOUT(44) => buff1_reg_n_110,
      PCOUT(43) => buff1_reg_n_111,
      PCOUT(42) => buff1_reg_n_112,
      PCOUT(41) => buff1_reg_n_113,
      PCOUT(40) => buff1_reg_n_114,
      PCOUT(39) => buff1_reg_n_115,
      PCOUT(38) => buff1_reg_n_116,
      PCOUT(37) => buff1_reg_n_117,
      PCOUT(36) => buff1_reg_n_118,
      PCOUT(35) => buff1_reg_n_119,
      PCOUT(34) => buff1_reg_n_120,
      PCOUT(33) => buff1_reg_n_121,
      PCOUT(32) => buff1_reg_n_122,
      PCOUT(31) => buff1_reg_n_123,
      PCOUT(30) => buff1_reg_n_124,
      PCOUT(29) => buff1_reg_n_125,
      PCOUT(28) => buff1_reg_n_126,
      PCOUT(27) => buff1_reg_n_127,
      PCOUT(26) => buff1_reg_n_128,
      PCOUT(25) => buff1_reg_n_129,
      PCOUT(24) => buff1_reg_n_130,
      PCOUT(23) => buff1_reg_n_131,
      PCOUT(22) => buff1_reg_n_132,
      PCOUT(21) => buff1_reg_n_133,
      PCOUT(20) => buff1_reg_n_134,
      PCOUT(19) => buff1_reg_n_135,
      PCOUT(18) => buff1_reg_n_136,
      PCOUT(17) => buff1_reg_n_137,
      PCOUT(16) => buff1_reg_n_138,
      PCOUT(15) => buff1_reg_n_139,
      PCOUT(14) => buff1_reg_n_140,
      PCOUT(13) => buff1_reg_n_141,
      PCOUT(12) => buff1_reg_n_142,
      PCOUT(11) => buff1_reg_n_143,
      PCOUT(10) => buff1_reg_n_144,
      PCOUT(9) => buff1_reg_n_145,
      PCOUT(8) => buff1_reg_n_146,
      PCOUT(7) => buff1_reg_n_147,
      PCOUT(6) => buff1_reg_n_148,
      PCOUT(5) => buff1_reg_n_149,
      PCOUT(4) => buff1_reg_n_150,
      PCOUT(3) => buff1_reg_n_151,
      PCOUT(2) => buff1_reg_n_152,
      PCOUT(1) => buff1_reg_n_153,
      PCOUT(0) => buff1_reg_n_154,
      RSTA => '0',
      RSTALLCARRYIN => '0',
      RSTALUMODE => '0',
      RSTB => '0',
      RSTC => '0',
      RSTCTRL => '0',
      RSTD => '0',
      RSTINMODE => '0',
      RSTM => '0',
      RSTP => '0',
      UNDERFLOW => NLW_buff1_reg_UNDERFLOW_UNCONNECTED
    );
\buff1_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_106,
      Q => \buff1_reg_n_1_[0]\,
      R => '0'
    );
\buff1_reg[10]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_96,
      Q => \buff1_reg_n_1_[10]\,
      R => '0'
    );
\buff1_reg[11]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_95,
      Q => \buff1_reg_n_1_[11]\,
      R => '0'
    );
\buff1_reg[12]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_94,
      Q => \buff1_reg_n_1_[12]\,
      R => '0'
    );
\buff1_reg[13]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_93,
      Q => \buff1_reg_n_1_[13]\,
      R => '0'
    );
\buff1_reg[14]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_92,
      Q => \buff1_reg_n_1_[14]\,
      R => '0'
    );
\buff1_reg[15]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_91,
      Q => \buff1_reg_n_1_[15]\,
      R => '0'
    );
\buff1_reg[16]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_90,
      Q => \buff1_reg_n_1_[16]\,
      R => '0'
    );
\buff1_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_105,
      Q => \buff1_reg_n_1_[1]\,
      R => '0'
    );
\buff1_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_104,
      Q => \buff1_reg_n_1_[2]\,
      R => '0'
    );
\buff1_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_103,
      Q => \buff1_reg_n_1_[3]\,
      R => '0'
    );
\buff1_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_102,
      Q => \buff1_reg_n_1_[4]\,
      R => '0'
    );
\buff1_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_101,
      Q => \buff1_reg_n_1_[5]\,
      R => '0'
    );
\buff1_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_100,
      Q => \buff1_reg_n_1_[6]\,
      R => '0'
    );
\buff1_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_99,
      Q => \buff1_reg_n_1_[7]\,
      R => '0'
    );
\buff1_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_98,
      Q => \buff1_reg_n_1_[8]\,
      R => '0'
    );
\buff1_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_97,
      Q => \buff1_reg_n_1_[9]\,
      R => '0'
    );
buff2_reg: unisim.vcomponents.DSP48E1
    generic map(
      ACASCREG => 2,
      ADREG => 1,
      ALUMODEREG => 0,
      AREG => 2,
      AUTORESET_PATDET => "NO_RESET",
      A_INPUT => "DIRECT",
      BCASCREG => 2,
      BREG => 2,
      B_INPUT => "DIRECT",
      CARRYINREG => 0,
      CARRYINSELREG => 0,
      CREG => 1,
      DREG => 1,
      INMODEREG => 0,
      MASK => X"3FFFFFFFFFFF",
      MREG => 1,
      OPMODEREG => 0,
      PATTERN => X"000000000000",
      PREG => 1,
      SEL_MASK => "MASK",
      SEL_PATTERN => "PATTERN",
      USE_DPORT => false,
      USE_MULT => "MULTIPLY",
      USE_PATTERN_DETECT => "NO_PATDET",
      USE_SIMD => "ONE48"
    )
        port map (
      A(29 downto 17) => B"0000000000000",
      A(16 downto 1) => grp_fu_190_p1(15 downto 0),
      A(0) => A(0),
      ACIN(29 downto 0) => B"000000000000000000000000000000",
      ACOUT(29 downto 0) => NLW_buff2_reg_ACOUT_UNCONNECTED(29 downto 0),
      ALUMODE(3 downto 0) => B"0000",
      B(17) => buff2_reg_0(14),
      B(16) => buff2_reg_0(14),
      B(15) => buff2_reg_0(14),
      B(14 downto 0) => buff2_reg_0(14 downto 0),
      BCIN(17 downto 0) => B"000000000000000000",
      BCOUT(17 downto 0) => NLW_buff2_reg_BCOUT_UNCONNECTED(17 downto 0),
      C(47 downto 0) => B"111111111111111111111111111111111111111111111111",
      CARRYCASCIN => '0',
      CARRYCASCOUT => NLW_buff2_reg_CARRYCASCOUT_UNCONNECTED,
      CARRYIN => '0',
      CARRYINSEL(2 downto 0) => B"000",
      CARRYOUT(3 downto 0) => NLW_buff2_reg_CARRYOUT_UNCONNECTED(3 downto 0),
      CEA1 => '1',
      CEA2 => '1',
      CEAD => '0',
      CEALUMODE => '0',
      CEB1 => '1',
      CEB2 => '1',
      CEC => '0',
      CECARRYIN => '0',
      CECTRL => '0',
      CED => '0',
      CEINMODE => '0',
      CEM => '1',
      CEP => '1',
      CLK => ap_clk,
      D(24 downto 0) => B"0000000000000000000000000",
      INMODE(4 downto 0) => B"00000",
      MULTSIGNIN => '0',
      MULTSIGNOUT => NLW_buff2_reg_MULTSIGNOUT_UNCONNECTED,
      OPMODE(6 downto 0) => B"0010101",
      OVERFLOW => NLW_buff2_reg_OVERFLOW_UNCONNECTED,
      P(47) => buff2_reg_n_59,
      P(46) => buff2_reg_n_60,
      P(45) => buff2_reg_n_61,
      P(44) => buff2_reg_n_62,
      P(43) => buff2_reg_n_63,
      P(42) => buff2_reg_n_64,
      P(41) => buff2_reg_n_65,
      P(40) => buff2_reg_n_66,
      P(39) => buff2_reg_n_67,
      P(38) => buff2_reg_n_68,
      P(37) => buff2_reg_n_69,
      P(36) => buff2_reg_n_70,
      P(35) => buff2_reg_n_71,
      P(34) => buff2_reg_n_72,
      P(33) => buff2_reg_n_73,
      P(32) => buff2_reg_n_74,
      P(31) => buff2_reg_n_75,
      P(30) => buff2_reg_n_76,
      P(29) => buff2_reg_n_77,
      P(28) => buff2_reg_n_78,
      P(27) => buff2_reg_n_79,
      P(26) => buff2_reg_n_80,
      P(25) => buff2_reg_n_81,
      P(24) => buff2_reg_n_82,
      P(23) => buff2_reg_n_83,
      P(22) => buff2_reg_n_84,
      P(21) => buff2_reg_n_85,
      P(20) => buff2_reg_n_86,
      P(19) => buff2_reg_n_87,
      P(18) => buff2_reg_n_88,
      P(17) => buff2_reg_n_89,
      P(16) => buff2_reg_n_90,
      P(15) => buff2_reg_n_91,
      P(14 downto 0) => \buff2_reg__0\(31 downto 17),
      PATTERNBDETECT => NLW_buff2_reg_PATTERNBDETECT_UNCONNECTED,
      PATTERNDETECT => NLW_buff2_reg_PATTERNDETECT_UNCONNECTED,
      PCIN(47) => buff1_reg_n_107,
      PCIN(46) => buff1_reg_n_108,
      PCIN(45) => buff1_reg_n_109,
      PCIN(44) => buff1_reg_n_110,
      PCIN(43) => buff1_reg_n_111,
      PCIN(42) => buff1_reg_n_112,
      PCIN(41) => buff1_reg_n_113,
      PCIN(40) => buff1_reg_n_114,
      PCIN(39) => buff1_reg_n_115,
      PCIN(38) => buff1_reg_n_116,
      PCIN(37) => buff1_reg_n_117,
      PCIN(36) => buff1_reg_n_118,
      PCIN(35) => buff1_reg_n_119,
      PCIN(34) => buff1_reg_n_120,
      PCIN(33) => buff1_reg_n_121,
      PCIN(32) => buff1_reg_n_122,
      PCIN(31) => buff1_reg_n_123,
      PCIN(30) => buff1_reg_n_124,
      PCIN(29) => buff1_reg_n_125,
      PCIN(28) => buff1_reg_n_126,
      PCIN(27) => buff1_reg_n_127,
      PCIN(26) => buff1_reg_n_128,
      PCIN(25) => buff1_reg_n_129,
      PCIN(24) => buff1_reg_n_130,
      PCIN(23) => buff1_reg_n_131,
      PCIN(22) => buff1_reg_n_132,
      PCIN(21) => buff1_reg_n_133,
      PCIN(20) => buff1_reg_n_134,
      PCIN(19) => buff1_reg_n_135,
      PCIN(18) => buff1_reg_n_136,
      PCIN(17) => buff1_reg_n_137,
      PCIN(16) => buff1_reg_n_138,
      PCIN(15) => buff1_reg_n_139,
      PCIN(14) => buff1_reg_n_140,
      PCIN(13) => buff1_reg_n_141,
      PCIN(12) => buff1_reg_n_142,
      PCIN(11) => buff1_reg_n_143,
      PCIN(10) => buff1_reg_n_144,
      PCIN(9) => buff1_reg_n_145,
      PCIN(8) => buff1_reg_n_146,
      PCIN(7) => buff1_reg_n_147,
      PCIN(6) => buff1_reg_n_148,
      PCIN(5) => buff1_reg_n_149,
      PCIN(4) => buff1_reg_n_150,
      PCIN(3) => buff1_reg_n_151,
      PCIN(2) => buff1_reg_n_152,
      PCIN(1) => buff1_reg_n_153,
      PCIN(0) => buff1_reg_n_154,
      PCOUT(47 downto 0) => NLW_buff2_reg_PCOUT_UNCONNECTED(47 downto 0),
      RSTA => '0',
      RSTALLCARRYIN => '0',
      RSTALUMODE => '0',
      RSTB => '0',
      RSTC => '0',
      RSTCTRL => '0',
      RSTD => '0',
      RSTINMODE => '0',
      RSTM => '0',
      RSTP => '0',
      UNDERFLOW => NLW_buff2_reg_UNDERFLOW_UNCONNECTED
    );
\buff2_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[0]\,
      Q => A_d0(0),
      R => '0'
    );
\buff2_reg[10]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[10]\,
      Q => \buff2_reg__0\(10),
      R => '0'
    );
\buff2_reg[11]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[11]\,
      Q => \buff2_reg__0\(11),
      R => '0'
    );
\buff2_reg[12]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[12]\,
      Q => \buff2_reg__0\(12),
      R => '0'
    );
\buff2_reg[13]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[13]\,
      Q => \buff2_reg__0\(13),
      R => '0'
    );
\buff2_reg[14]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[14]\,
      Q => \buff2_reg__0\(14),
      R => '0'
    );
\buff2_reg[15]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[15]\,
      Q => \buff2_reg__0\(15),
      R => '0'
    );
\buff2_reg[16]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[16]\,
      Q => \buff2_reg__0\(16),
      R => '0'
    );
\buff2_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[1]\,
      Q => \buff2_reg__0\(1),
      R => '0'
    );
\buff2_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[2]\,
      Q => \buff2_reg__0\(2),
      R => '0'
    );
\buff2_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[3]\,
      Q => \buff2_reg__0\(3),
      R => '0'
    );
\buff2_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[4]\,
      Q => \buff2_reg__0\(4),
      R => '0'
    );
\buff2_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[5]\,
      Q => \buff2_reg__0\(5),
      R => '0'
    );
\buff2_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[6]\,
      Q => \buff2_reg__0\(6),
      R => '0'
    );
\buff2_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[7]\,
      Q => \buff2_reg__0\(7),
      R => '0'
    );
\buff2_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[8]\,
      Q => \buff2_reg__0\(8),
      R => '0'
    );
\buff2_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[9]\,
      Q => \buff2_reg__0\(9),
      R => '0'
    );
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_MulnS_0_10 is
  port (
    A : out STD_LOGIC_VECTOR ( 16 downto 0 );
    grp_fu_146_p1 : out STD_LOGIC_VECTOR ( 14 downto 0 );
    Q : in STD_LOGIC_VECTOR ( 0 to 0 );
    ap_clk : in STD_LOGIC;
    grp_fu_135_p1 : in STD_LOGIC_VECTOR ( 31 downto 0 );
    A_q0 : in STD_LOGIC_VECTOR ( 16 downto 0 );
    buff2_reg_0 : in STD_LOGIC_VECTOR ( 14 downto 0 )
  );
  attribute ORIG_REF_NAME : string;
  attribute ORIG_REF_NAME of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_MulnS_0_10 : entity is "loop_imperfect_mubkb_MulnS_0";
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_MulnS_0_10;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_MulnS_0_10 is
  signal \buff0_reg_i_1__1_n_1\ : STD_LOGIC;
  signal \buff0_reg_i_1__1_n_2\ : STD_LOGIC;
  signal \buff0_reg_i_1__1_n_3\ : STD_LOGIC;
  signal \buff0_reg_i_1__1_n_4\ : STD_LOGIC;
  signal \buff0_reg_i_2__1_n_1\ : STD_LOGIC;
  signal \buff0_reg_i_2__1_n_2\ : STD_LOGIC;
  signal \buff0_reg_i_2__1_n_3\ : STD_LOGIC;
  signal \buff0_reg_i_2__1_n_4\ : STD_LOGIC;
  signal \buff0_reg_i_3__1_n_1\ : STD_LOGIC;
  signal \buff0_reg_i_3__1_n_2\ : STD_LOGIC;
  signal \buff0_reg_i_3__1_n_3\ : STD_LOGIC;
  signal \buff0_reg_i_3__1_n_4\ : STD_LOGIC;
  signal \buff0_reg_i_4__1_n_1\ : STD_LOGIC;
  signal \buff0_reg_i_4__1_n_2\ : STD_LOGIC;
  signal \buff0_reg_i_4__1_n_3\ : STD_LOGIC;
  signal \buff0_reg_i_4__1_n_4\ : STD_LOGIC;
  signal \buff0_reg_i_5__1_n_1\ : STD_LOGIC;
  signal \buff0_reg_i_6__1_n_1\ : STD_LOGIC;
  signal buff0_reg_n_100 : STD_LOGIC;
  signal buff0_reg_n_101 : STD_LOGIC;
  signal buff0_reg_n_102 : STD_LOGIC;
  signal buff0_reg_n_103 : STD_LOGIC;
  signal buff0_reg_n_104 : STD_LOGIC;
  signal buff0_reg_n_105 : STD_LOGIC;
  signal buff0_reg_n_106 : STD_LOGIC;
  signal buff0_reg_n_107 : STD_LOGIC;
  signal buff0_reg_n_108 : STD_LOGIC;
  signal buff0_reg_n_109 : STD_LOGIC;
  signal buff0_reg_n_110 : STD_LOGIC;
  signal buff0_reg_n_111 : STD_LOGIC;
  signal buff0_reg_n_112 : STD_LOGIC;
  signal buff0_reg_n_113 : STD_LOGIC;
  signal buff0_reg_n_114 : STD_LOGIC;
  signal buff0_reg_n_115 : STD_LOGIC;
  signal buff0_reg_n_116 : STD_LOGIC;
  signal buff0_reg_n_117 : STD_LOGIC;
  signal buff0_reg_n_118 : STD_LOGIC;
  signal buff0_reg_n_119 : STD_LOGIC;
  signal buff0_reg_n_120 : STD_LOGIC;
  signal buff0_reg_n_121 : STD_LOGIC;
  signal buff0_reg_n_122 : STD_LOGIC;
  signal buff0_reg_n_123 : STD_LOGIC;
  signal buff0_reg_n_124 : STD_LOGIC;
  signal buff0_reg_n_125 : STD_LOGIC;
  signal buff0_reg_n_126 : STD_LOGIC;
  signal buff0_reg_n_127 : STD_LOGIC;
  signal buff0_reg_n_128 : STD_LOGIC;
  signal buff0_reg_n_129 : STD_LOGIC;
  signal buff0_reg_n_130 : STD_LOGIC;
  signal buff0_reg_n_131 : STD_LOGIC;
  signal buff0_reg_n_132 : STD_LOGIC;
  signal buff0_reg_n_133 : STD_LOGIC;
  signal buff0_reg_n_134 : STD_LOGIC;
  signal buff0_reg_n_135 : STD_LOGIC;
  signal buff0_reg_n_136 : STD_LOGIC;
  signal buff0_reg_n_137 : STD_LOGIC;
  signal buff0_reg_n_138 : STD_LOGIC;
  signal buff0_reg_n_139 : STD_LOGIC;
  signal buff0_reg_n_140 : STD_LOGIC;
  signal buff0_reg_n_141 : STD_LOGIC;
  signal buff0_reg_n_142 : STD_LOGIC;
  signal buff0_reg_n_143 : STD_LOGIC;
  signal buff0_reg_n_144 : STD_LOGIC;
  signal buff0_reg_n_145 : STD_LOGIC;
  signal buff0_reg_n_146 : STD_LOGIC;
  signal buff0_reg_n_147 : STD_LOGIC;
  signal buff0_reg_n_148 : STD_LOGIC;
  signal buff0_reg_n_149 : STD_LOGIC;
  signal buff0_reg_n_150 : STD_LOGIC;
  signal buff0_reg_n_151 : STD_LOGIC;
  signal buff0_reg_n_152 : STD_LOGIC;
  signal buff0_reg_n_153 : STD_LOGIC;
  signal buff0_reg_n_154 : STD_LOGIC;
  signal buff0_reg_n_59 : STD_LOGIC;
  signal buff0_reg_n_60 : STD_LOGIC;
  signal buff0_reg_n_61 : STD_LOGIC;
  signal buff0_reg_n_62 : STD_LOGIC;
  signal buff0_reg_n_63 : STD_LOGIC;
  signal buff0_reg_n_64 : STD_LOGIC;
  signal buff0_reg_n_65 : STD_LOGIC;
  signal buff0_reg_n_66 : STD_LOGIC;
  signal buff0_reg_n_67 : STD_LOGIC;
  signal buff0_reg_n_68 : STD_LOGIC;
  signal buff0_reg_n_69 : STD_LOGIC;
  signal buff0_reg_n_70 : STD_LOGIC;
  signal buff0_reg_n_71 : STD_LOGIC;
  signal buff0_reg_n_72 : STD_LOGIC;
  signal buff0_reg_n_73 : STD_LOGIC;
  signal buff0_reg_n_74 : STD_LOGIC;
  signal buff0_reg_n_75 : STD_LOGIC;
  signal buff0_reg_n_76 : STD_LOGIC;
  signal buff0_reg_n_77 : STD_LOGIC;
  signal buff0_reg_n_78 : STD_LOGIC;
  signal buff0_reg_n_79 : STD_LOGIC;
  signal buff0_reg_n_80 : STD_LOGIC;
  signal buff0_reg_n_81 : STD_LOGIC;
  signal buff0_reg_n_82 : STD_LOGIC;
  signal buff0_reg_n_83 : STD_LOGIC;
  signal buff0_reg_n_84 : STD_LOGIC;
  signal buff0_reg_n_85 : STD_LOGIC;
  signal buff0_reg_n_86 : STD_LOGIC;
  signal buff0_reg_n_87 : STD_LOGIC;
  signal buff0_reg_n_88 : STD_LOGIC;
  signal buff0_reg_n_89 : STD_LOGIC;
  signal buff0_reg_n_90 : STD_LOGIC;
  signal buff0_reg_n_91 : STD_LOGIC;
  signal buff0_reg_n_92 : STD_LOGIC;
  signal buff0_reg_n_93 : STD_LOGIC;
  signal buff0_reg_n_94 : STD_LOGIC;
  signal buff0_reg_n_95 : STD_LOGIC;
  signal buff0_reg_n_96 : STD_LOGIC;
  signal buff0_reg_n_97 : STD_LOGIC;
  signal buff0_reg_n_98 : STD_LOGIC;
  signal buff0_reg_n_99 : STD_LOGIC;
  signal \buff1_reg_i_1__1_n_3\ : STD_LOGIC;
  signal \buff1_reg_i_1__1_n_4\ : STD_LOGIC;
  signal \buff1_reg_i_2__1_n_1\ : STD_LOGIC;
  signal \buff1_reg_i_2__1_n_2\ : STD_LOGIC;
  signal \buff1_reg_i_2__1_n_3\ : STD_LOGIC;
  signal \buff1_reg_i_2__1_n_4\ : STD_LOGIC;
  signal \buff1_reg_i_3__1_n_1\ : STD_LOGIC;
  signal \buff1_reg_i_3__1_n_2\ : STD_LOGIC;
  signal \buff1_reg_i_3__1_n_3\ : STD_LOGIC;
  signal \buff1_reg_i_3__1_n_4\ : STD_LOGIC;
  signal \buff1_reg_i_4__1_n_1\ : STD_LOGIC;
  signal \buff1_reg_i_4__1_n_2\ : STD_LOGIC;
  signal \buff1_reg_i_4__1_n_3\ : STD_LOGIC;
  signal \buff1_reg_i_4__1_n_4\ : STD_LOGIC;
  signal buff1_reg_n_107 : STD_LOGIC;
  signal buff1_reg_n_108 : STD_LOGIC;
  signal buff1_reg_n_109 : STD_LOGIC;
  signal buff1_reg_n_110 : STD_LOGIC;
  signal buff1_reg_n_111 : STD_LOGIC;
  signal buff1_reg_n_112 : STD_LOGIC;
  signal buff1_reg_n_113 : STD_LOGIC;
  signal buff1_reg_n_114 : STD_LOGIC;
  signal buff1_reg_n_115 : STD_LOGIC;
  signal buff1_reg_n_116 : STD_LOGIC;
  signal buff1_reg_n_117 : STD_LOGIC;
  signal buff1_reg_n_118 : STD_LOGIC;
  signal buff1_reg_n_119 : STD_LOGIC;
  signal buff1_reg_n_120 : STD_LOGIC;
  signal buff1_reg_n_121 : STD_LOGIC;
  signal buff1_reg_n_122 : STD_LOGIC;
  signal buff1_reg_n_123 : STD_LOGIC;
  signal buff1_reg_n_124 : STD_LOGIC;
  signal buff1_reg_n_125 : STD_LOGIC;
  signal buff1_reg_n_126 : STD_LOGIC;
  signal buff1_reg_n_127 : STD_LOGIC;
  signal buff1_reg_n_128 : STD_LOGIC;
  signal buff1_reg_n_129 : STD_LOGIC;
  signal buff1_reg_n_130 : STD_LOGIC;
  signal buff1_reg_n_131 : STD_LOGIC;
  signal buff1_reg_n_132 : STD_LOGIC;
  signal buff1_reg_n_133 : STD_LOGIC;
  signal buff1_reg_n_134 : STD_LOGIC;
  signal buff1_reg_n_135 : STD_LOGIC;
  signal buff1_reg_n_136 : STD_LOGIC;
  signal buff1_reg_n_137 : STD_LOGIC;
  signal buff1_reg_n_138 : STD_LOGIC;
  signal buff1_reg_n_139 : STD_LOGIC;
  signal buff1_reg_n_140 : STD_LOGIC;
  signal buff1_reg_n_141 : STD_LOGIC;
  signal buff1_reg_n_142 : STD_LOGIC;
  signal buff1_reg_n_143 : STD_LOGIC;
  signal buff1_reg_n_144 : STD_LOGIC;
  signal buff1_reg_n_145 : STD_LOGIC;
  signal buff1_reg_n_146 : STD_LOGIC;
  signal buff1_reg_n_147 : STD_LOGIC;
  signal buff1_reg_n_148 : STD_LOGIC;
  signal buff1_reg_n_149 : STD_LOGIC;
  signal buff1_reg_n_150 : STD_LOGIC;
  signal buff1_reg_n_151 : STD_LOGIC;
  signal buff1_reg_n_152 : STD_LOGIC;
  signal buff1_reg_n_153 : STD_LOGIC;
  signal buff1_reg_n_154 : STD_LOGIC;
  signal \buff1_reg_n_1_[0]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[10]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[11]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[12]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[13]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[14]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[15]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[16]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[1]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[2]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[3]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[4]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[5]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[6]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[7]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[8]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[9]\ : STD_LOGIC;
  signal \buff2_reg__0\ : STD_LOGIC_VECTOR ( 31 downto 1 );
  signal buff2_reg_n_59 : STD_LOGIC;
  signal buff2_reg_n_60 : STD_LOGIC;
  signal buff2_reg_n_61 : STD_LOGIC;
  signal buff2_reg_n_62 : STD_LOGIC;
  signal buff2_reg_n_63 : STD_LOGIC;
  signal buff2_reg_n_64 : STD_LOGIC;
  signal buff2_reg_n_65 : STD_LOGIC;
  signal buff2_reg_n_66 : STD_LOGIC;
  signal buff2_reg_n_67 : STD_LOGIC;
  signal buff2_reg_n_68 : STD_LOGIC;
  signal buff2_reg_n_69 : STD_LOGIC;
  signal buff2_reg_n_70 : STD_LOGIC;
  signal buff2_reg_n_71 : STD_LOGIC;
  signal buff2_reg_n_72 : STD_LOGIC;
  signal buff2_reg_n_73 : STD_LOGIC;
  signal buff2_reg_n_74 : STD_LOGIC;
  signal buff2_reg_n_75 : STD_LOGIC;
  signal buff2_reg_n_76 : STD_LOGIC;
  signal buff2_reg_n_77 : STD_LOGIC;
  signal buff2_reg_n_78 : STD_LOGIC;
  signal buff2_reg_n_79 : STD_LOGIC;
  signal buff2_reg_n_80 : STD_LOGIC;
  signal buff2_reg_n_81 : STD_LOGIC;
  signal buff2_reg_n_82 : STD_LOGIC;
  signal buff2_reg_n_83 : STD_LOGIC;
  signal buff2_reg_n_84 : STD_LOGIC;
  signal buff2_reg_n_85 : STD_LOGIC;
  signal buff2_reg_n_86 : STD_LOGIC;
  signal buff2_reg_n_87 : STD_LOGIC;
  signal buff2_reg_n_88 : STD_LOGIC;
  signal buff2_reg_n_89 : STD_LOGIC;
  signal buff2_reg_n_90 : STD_LOGIC;
  signal buff2_reg_n_91 : STD_LOGIC;
  signal NLW_buff0_reg_CARRYCASCOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_MULTSIGNOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_OVERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_PATTERNBDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_PATTERNDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_UNDERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_ACOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 29 downto 0 );
  signal NLW_buff0_reg_BCOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 17 downto 0 );
  signal NLW_buff0_reg_CARRYOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_buff1_reg_CARRYCASCOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_MULTSIGNOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_OVERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_PATTERNBDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_PATTERNDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_UNDERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_ACOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 29 downto 0 );
  signal NLW_buff1_reg_BCOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 17 downto 0 );
  signal NLW_buff1_reg_CARRYOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_buff1_reg_P_UNCONNECTED : STD_LOGIC_VECTOR ( 47 downto 0 );
  signal \NLW_buff1_reg_i_1__1_CO_UNCONNECTED\ : STD_LOGIC_VECTOR ( 3 downto 2 );
  signal \NLW_buff1_reg_i_1__1_O_UNCONNECTED\ : STD_LOGIC_VECTOR ( 3 to 3 );
  signal NLW_buff2_reg_CARRYCASCOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_MULTSIGNOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_OVERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_PATTERNBDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_PATTERNDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_UNDERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_ACOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 29 downto 0 );
  signal NLW_buff2_reg_BCOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 17 downto 0 );
  signal NLW_buff2_reg_CARRYOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_buff2_reg_PCOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 47 downto 0 );
begin
buff0_reg: unisim.vcomponents.DSP48E1
    generic map(
      ACASCREG => 2,
      ADREG => 1,
      ALUMODEREG => 0,
      AREG => 2,
      AUTORESET_PATDET => "NO_RESET",
      A_INPUT => "DIRECT",
      BCASCREG => 1,
      BREG => 1,
      B_INPUT => "DIRECT",
      CARRYINREG => 0,
      CARRYINSELREG => 0,
      CREG => 0,
      DREG => 1,
      INMODEREG => 0,
      MASK => X"3FFFFFFFFFFF",
      MREG => 0,
      OPMODEREG => 0,
      PATTERN => X"000000000000",
      PREG => 1,
      SEL_MASK => "MASK",
      SEL_PATTERN => "PATTERN",
      USE_DPORT => false,
      USE_MULT => "MULTIPLY",
      USE_PATTERN_DETECT => "NO_PATDET",
      USE_SIMD => "ONE48"
    )
        port map (
      A(29 downto 17) => B"0000000000000",
      A(16 downto 0) => A_q0(16 downto 0),
      ACIN(29 downto 0) => B"000000000000000000000000000000",
      ACOUT(29 downto 0) => NLW_buff0_reg_ACOUT_UNCONNECTED(29 downto 0),
      ALUMODE(3 downto 0) => B"0000",
      B(17) => '0',
      B(16 downto 0) => grp_fu_135_p1(16 downto 0),
      BCIN(17 downto 0) => B"000000000000000000",
      BCOUT(17 downto 0) => NLW_buff0_reg_BCOUT_UNCONNECTED(17 downto 0),
      C(47 downto 0) => B"111111111111111111111111111111111111111111111111",
      CARRYCASCIN => '0',
      CARRYCASCOUT => NLW_buff0_reg_CARRYCASCOUT_UNCONNECTED,
      CARRYIN => '0',
      CARRYINSEL(2 downto 0) => B"000",
      CARRYOUT(3 downto 0) => NLW_buff0_reg_CARRYOUT_UNCONNECTED(3 downto 0),
      CEA1 => Q(0),
      CEA2 => '1',
      CEAD => '0',
      CEALUMODE => '0',
      CEB1 => '0',
      CEB2 => '1',
      CEC => '0',
      CECARRYIN => '0',
      CECTRL => '0',
      CED => '0',
      CEINMODE => '0',
      CEM => '0',
      CEP => '1',
      CLK => ap_clk,
      D(24 downto 0) => B"0000000000000000000000000",
      INMODE(4 downto 0) => B"00000",
      MULTSIGNIN => '0',
      MULTSIGNOUT => NLW_buff0_reg_MULTSIGNOUT_UNCONNECTED,
      OPMODE(6 downto 0) => B"0000101",
      OVERFLOW => NLW_buff0_reg_OVERFLOW_UNCONNECTED,
      P(47) => buff0_reg_n_59,
      P(46) => buff0_reg_n_60,
      P(45) => buff0_reg_n_61,
      P(44) => buff0_reg_n_62,
      P(43) => buff0_reg_n_63,
      P(42) => buff0_reg_n_64,
      P(41) => buff0_reg_n_65,
      P(40) => buff0_reg_n_66,
      P(39) => buff0_reg_n_67,
      P(38) => buff0_reg_n_68,
      P(37) => buff0_reg_n_69,
      P(36) => buff0_reg_n_70,
      P(35) => buff0_reg_n_71,
      P(34) => buff0_reg_n_72,
      P(33) => buff0_reg_n_73,
      P(32) => buff0_reg_n_74,
      P(31) => buff0_reg_n_75,
      P(30) => buff0_reg_n_76,
      P(29) => buff0_reg_n_77,
      P(28) => buff0_reg_n_78,
      P(27) => buff0_reg_n_79,
      P(26) => buff0_reg_n_80,
      P(25) => buff0_reg_n_81,
      P(24) => buff0_reg_n_82,
      P(23) => buff0_reg_n_83,
      P(22) => buff0_reg_n_84,
      P(21) => buff0_reg_n_85,
      P(20) => buff0_reg_n_86,
      P(19) => buff0_reg_n_87,
      P(18) => buff0_reg_n_88,
      P(17) => buff0_reg_n_89,
      P(16) => buff0_reg_n_90,
      P(15) => buff0_reg_n_91,
      P(14) => buff0_reg_n_92,
      P(13) => buff0_reg_n_93,
      P(12) => buff0_reg_n_94,
      P(11) => buff0_reg_n_95,
      P(10) => buff0_reg_n_96,
      P(9) => buff0_reg_n_97,
      P(8) => buff0_reg_n_98,
      P(7) => buff0_reg_n_99,
      P(6) => buff0_reg_n_100,
      P(5) => buff0_reg_n_101,
      P(4) => buff0_reg_n_102,
      P(3) => buff0_reg_n_103,
      P(2) => buff0_reg_n_104,
      P(1) => buff0_reg_n_105,
      P(0) => buff0_reg_n_106,
      PATTERNBDETECT => NLW_buff0_reg_PATTERNBDETECT_UNCONNECTED,
      PATTERNDETECT => NLW_buff0_reg_PATTERNDETECT_UNCONNECTED,
      PCIN(47 downto 0) => B"000000000000000000000000000000000000000000000000",
      PCOUT(47) => buff0_reg_n_107,
      PCOUT(46) => buff0_reg_n_108,
      PCOUT(45) => buff0_reg_n_109,
      PCOUT(44) => buff0_reg_n_110,
      PCOUT(43) => buff0_reg_n_111,
      PCOUT(42) => buff0_reg_n_112,
      PCOUT(41) => buff0_reg_n_113,
      PCOUT(40) => buff0_reg_n_114,
      PCOUT(39) => buff0_reg_n_115,
      PCOUT(38) => buff0_reg_n_116,
      PCOUT(37) => buff0_reg_n_117,
      PCOUT(36) => buff0_reg_n_118,
      PCOUT(35) => buff0_reg_n_119,
      PCOUT(34) => buff0_reg_n_120,
      PCOUT(33) => buff0_reg_n_121,
      PCOUT(32) => buff0_reg_n_122,
      PCOUT(31) => buff0_reg_n_123,
      PCOUT(30) => buff0_reg_n_124,
      PCOUT(29) => buff0_reg_n_125,
      PCOUT(28) => buff0_reg_n_126,
      PCOUT(27) => buff0_reg_n_127,
      PCOUT(26) => buff0_reg_n_128,
      PCOUT(25) => buff0_reg_n_129,
      PCOUT(24) => buff0_reg_n_130,
      PCOUT(23) => buff0_reg_n_131,
      PCOUT(22) => buff0_reg_n_132,
      PCOUT(21) => buff0_reg_n_133,
      PCOUT(20) => buff0_reg_n_134,
      PCOUT(19) => buff0_reg_n_135,
      PCOUT(18) => buff0_reg_n_136,
      PCOUT(17) => buff0_reg_n_137,
      PCOUT(16) => buff0_reg_n_138,
      PCOUT(15) => buff0_reg_n_139,
      PCOUT(14) => buff0_reg_n_140,
      PCOUT(13) => buff0_reg_n_141,
      PCOUT(12) => buff0_reg_n_142,
      PCOUT(11) => buff0_reg_n_143,
      PCOUT(10) => buff0_reg_n_144,
      PCOUT(9) => buff0_reg_n_145,
      PCOUT(8) => buff0_reg_n_146,
      PCOUT(7) => buff0_reg_n_147,
      PCOUT(6) => buff0_reg_n_148,
      PCOUT(5) => buff0_reg_n_149,
      PCOUT(4) => buff0_reg_n_150,
      PCOUT(3) => buff0_reg_n_151,
      PCOUT(2) => buff0_reg_n_152,
      PCOUT(1) => buff0_reg_n_153,
      PCOUT(0) => buff0_reg_n_154,
      RSTA => '0',
      RSTALLCARRYIN => '0',
      RSTALUMODE => '0',
      RSTB => '0',
      RSTC => '0',
      RSTCTRL => '0',
      RSTD => '0',
      RSTINMODE => '0',
      RSTM => '0',
      RSTP => '0',
      UNDERFLOW => NLW_buff0_reg_UNDERFLOW_UNCONNECTED
    );
\buff0_reg_i_1__1\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff0_reg_i_2__1_n_1\,
      CO(3) => \buff0_reg_i_1__1_n_1\,
      CO(2) => \buff0_reg_i_1__1_n_2\,
      CO(1) => \buff0_reg_i_1__1_n_3\,
      CO(0) => \buff0_reg_i_1__1_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => A(16 downto 13),
      S(3 downto 0) => \buff2_reg__0\(16 downto 13)
    );
\buff0_reg_i_2__1\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff0_reg_i_3__1_n_1\,
      CO(3) => \buff0_reg_i_2__1_n_1\,
      CO(2) => \buff0_reg_i_2__1_n_2\,
      CO(1) => \buff0_reg_i_2__1_n_3\,
      CO(0) => \buff0_reg_i_2__1_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => A(12 downto 9),
      S(3 downto 0) => \buff2_reg__0\(12 downto 9)
    );
\buff0_reg_i_3__1\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff0_reg_i_4__1_n_1\,
      CO(3) => \buff0_reg_i_3__1_n_1\,
      CO(2) => \buff0_reg_i_3__1_n_2\,
      CO(1) => \buff0_reg_i_3__1_n_3\,
      CO(0) => \buff0_reg_i_3__1_n_4\,
      CYINIT => '0',
      DI(3 downto 1) => B"000",
      DI(0) => \buff2_reg__0\(5),
      O(3 downto 0) => A(8 downto 5),
      S(3 downto 1) => \buff2_reg__0\(8 downto 6),
      S(0) => \buff0_reg_i_5__1_n_1\
    );
\buff0_reg_i_4__1\: unisim.vcomponents.CARRY4
     port map (
      CI => '0',
      CO(3) => \buff0_reg_i_4__1_n_1\,
      CO(2) => \buff0_reg_i_4__1_n_2\,
      CO(1) => \buff0_reg_i_4__1_n_3\,
      CO(0) => \buff0_reg_i_4__1_n_4\,
      CYINIT => '0',
      DI(3 downto 2) => B"00",
      DI(1) => \buff2_reg__0\(2),
      DI(0) => '0',
      O(3 downto 0) => A(4 downto 1),
      S(3 downto 2) => \buff2_reg__0\(4 downto 3),
      S(1) => \buff0_reg_i_6__1_n_1\,
      S(0) => \buff2_reg__0\(1)
    );
\buff0_reg_i_5__1\: unisim.vcomponents.LUT1
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => \buff2_reg__0\(5),
      O => \buff0_reg_i_5__1_n_1\
    );
\buff0_reg_i_6__1\: unisim.vcomponents.LUT1
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => \buff2_reg__0\(2),
      O => \buff0_reg_i_6__1_n_1\
    );
buff1_reg: unisim.vcomponents.DSP48E1
    generic map(
      ACASCREG => 2,
      ADREG => 1,
      ALUMODEREG => 0,
      AREG => 2,
      AUTORESET_PATDET => "NO_RESET",
      A_INPUT => "DIRECT",
      BCASCREG => 1,
      BREG => 1,
      B_INPUT => "DIRECT",
      CARRYINREG => 0,
      CARRYINSELREG => 0,
      CREG => 1,
      DREG => 1,
      INMODEREG => 0,
      MASK => X"3FFFFFFFFFFF",
      MREG => 1,
      OPMODEREG => 0,
      PATTERN => X"000000000000",
      PREG => 1,
      SEL_MASK => "MASK",
      SEL_PATTERN => "PATTERN",
      USE_DPORT => false,
      USE_MULT => "MULTIPLY",
      USE_PATTERN_DETECT => "NO_PATDET",
      USE_SIMD => "ONE48"
    )
        port map (
      A(29 downto 17) => B"0000000000000",
      A(16 downto 0) => A_q0(16 downto 0),
      ACIN(29 downto 0) => B"000000000000000000000000000000",
      ACOUT(29 downto 0) => NLW_buff1_reg_ACOUT_UNCONNECTED(29 downto 0),
      ALUMODE(3 downto 0) => B"0000",
      B(17) => grp_fu_135_p1(31),
      B(16) => grp_fu_135_p1(31),
      B(15) => grp_fu_135_p1(31),
      B(14 downto 0) => grp_fu_135_p1(31 downto 17),
      BCIN(17 downto 0) => B"000000000000000000",
      BCOUT(17 downto 0) => NLW_buff1_reg_BCOUT_UNCONNECTED(17 downto 0),
      C(47 downto 0) => B"111111111111111111111111111111111111111111111111",
      CARRYCASCIN => '0',
      CARRYCASCOUT => NLW_buff1_reg_CARRYCASCOUT_UNCONNECTED,
      CARRYIN => '0',
      CARRYINSEL(2 downto 0) => B"000",
      CARRYOUT(3 downto 0) => NLW_buff1_reg_CARRYOUT_UNCONNECTED(3 downto 0),
      CEA1 => Q(0),
      CEA2 => '1',
      CEAD => '0',
      CEALUMODE => '0',
      CEB1 => '0',
      CEB2 => '1',
      CEC => '0',
      CECARRYIN => '0',
      CECTRL => '0',
      CED => '0',
      CEINMODE => '0',
      CEM => '1',
      CEP => '1',
      CLK => ap_clk,
      D(24 downto 0) => B"0000000000000000000000000",
      INMODE(4 downto 0) => B"00000",
      MULTSIGNIN => '0',
      MULTSIGNOUT => NLW_buff1_reg_MULTSIGNOUT_UNCONNECTED,
      OPMODE(6 downto 0) => B"1010101",
      OVERFLOW => NLW_buff1_reg_OVERFLOW_UNCONNECTED,
      P(47 downto 0) => NLW_buff1_reg_P_UNCONNECTED(47 downto 0),
      PATTERNBDETECT => NLW_buff1_reg_PATTERNBDETECT_UNCONNECTED,
      PATTERNDETECT => NLW_buff1_reg_PATTERNDETECT_UNCONNECTED,
      PCIN(47) => buff0_reg_n_107,
      PCIN(46) => buff0_reg_n_108,
      PCIN(45) => buff0_reg_n_109,
      PCIN(44) => buff0_reg_n_110,
      PCIN(43) => buff0_reg_n_111,
      PCIN(42) => buff0_reg_n_112,
      PCIN(41) => buff0_reg_n_113,
      PCIN(40) => buff0_reg_n_114,
      PCIN(39) => buff0_reg_n_115,
      PCIN(38) => buff0_reg_n_116,
      PCIN(37) => buff0_reg_n_117,
      PCIN(36) => buff0_reg_n_118,
      PCIN(35) => buff0_reg_n_119,
      PCIN(34) => buff0_reg_n_120,
      PCIN(33) => buff0_reg_n_121,
      PCIN(32) => buff0_reg_n_122,
      PCIN(31) => buff0_reg_n_123,
      PCIN(30) => buff0_reg_n_124,
      PCIN(29) => buff0_reg_n_125,
      PCIN(28) => buff0_reg_n_126,
      PCIN(27) => buff0_reg_n_127,
      PCIN(26) => buff0_reg_n_128,
      PCIN(25) => buff0_reg_n_129,
      PCIN(24) => buff0_reg_n_130,
      PCIN(23) => buff0_reg_n_131,
      PCIN(22) => buff0_reg_n_132,
      PCIN(21) => buff0_reg_n_133,
      PCIN(20) => buff0_reg_n_134,
      PCIN(19) => buff0_reg_n_135,
      PCIN(18) => buff0_reg_n_136,
      PCIN(17) => buff0_reg_n_137,
      PCIN(16) => buff0_reg_n_138,
      PCIN(15) => buff0_reg_n_139,
      PCIN(14) => buff0_reg_n_140,
      PCIN(13) => buff0_reg_n_141,
      PCIN(12) => buff0_reg_n_142,
      PCIN(11) => buff0_reg_n_143,
      PCIN(10) => buff0_reg_n_144,
      PCIN(9) => buff0_reg_n_145,
      PCIN(8) => buff0_reg_n_146,
      PCIN(7) => buff0_reg_n_147,
      PCIN(6) => buff0_reg_n_148,
      PCIN(5) => buff0_reg_n_149,
      PCIN(4) => buff0_reg_n_150,
      PCIN(3) => buff0_reg_n_151,
      PCIN(2) => buff0_reg_n_152,
      PCIN(1) => buff0_reg_n_153,
      PCIN(0) => buff0_reg_n_154,
      PCOUT(47) => buff1_reg_n_107,
      PCOUT(46) => buff1_reg_n_108,
      PCOUT(45) => buff1_reg_n_109,
      PCOUT(44) => buff1_reg_n_110,
      PCOUT(43) => buff1_reg_n_111,
      PCOUT(42) => buff1_reg_n_112,
      PCOUT(41) => buff1_reg_n_113,
      PCOUT(40) => buff1_reg_n_114,
      PCOUT(39) => buff1_reg_n_115,
      PCOUT(38) => buff1_reg_n_116,
      PCOUT(37) => buff1_reg_n_117,
      PCOUT(36) => buff1_reg_n_118,
      PCOUT(35) => buff1_reg_n_119,
      PCOUT(34) => buff1_reg_n_120,
      PCOUT(33) => buff1_reg_n_121,
      PCOUT(32) => buff1_reg_n_122,
      PCOUT(31) => buff1_reg_n_123,
      PCOUT(30) => buff1_reg_n_124,
      PCOUT(29) => buff1_reg_n_125,
      PCOUT(28) => buff1_reg_n_126,
      PCOUT(27) => buff1_reg_n_127,
      PCOUT(26) => buff1_reg_n_128,
      PCOUT(25) => buff1_reg_n_129,
      PCOUT(24) => buff1_reg_n_130,
      PCOUT(23) => buff1_reg_n_131,
      PCOUT(22) => buff1_reg_n_132,
      PCOUT(21) => buff1_reg_n_133,
      PCOUT(20) => buff1_reg_n_134,
      PCOUT(19) => buff1_reg_n_135,
      PCOUT(18) => buff1_reg_n_136,
      PCOUT(17) => buff1_reg_n_137,
      PCOUT(16) => buff1_reg_n_138,
      PCOUT(15) => buff1_reg_n_139,
      PCOUT(14) => buff1_reg_n_140,
      PCOUT(13) => buff1_reg_n_141,
      PCOUT(12) => buff1_reg_n_142,
      PCOUT(11) => buff1_reg_n_143,
      PCOUT(10) => buff1_reg_n_144,
      PCOUT(9) => buff1_reg_n_145,
      PCOUT(8) => buff1_reg_n_146,
      PCOUT(7) => buff1_reg_n_147,
      PCOUT(6) => buff1_reg_n_148,
      PCOUT(5) => buff1_reg_n_149,
      PCOUT(4) => buff1_reg_n_150,
      PCOUT(3) => buff1_reg_n_151,
      PCOUT(2) => buff1_reg_n_152,
      PCOUT(1) => buff1_reg_n_153,
      PCOUT(0) => buff1_reg_n_154,
      RSTA => '0',
      RSTALLCARRYIN => '0',
      RSTALUMODE => '0',
      RSTB => '0',
      RSTC => '0',
      RSTCTRL => '0',
      RSTD => '0',
      RSTINMODE => '0',
      RSTM => '0',
      RSTP => '0',
      UNDERFLOW => NLW_buff1_reg_UNDERFLOW_UNCONNECTED
    );
\buff1_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_106,
      Q => \buff1_reg_n_1_[0]\,
      R => '0'
    );
\buff1_reg[10]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_96,
      Q => \buff1_reg_n_1_[10]\,
      R => '0'
    );
\buff1_reg[11]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_95,
      Q => \buff1_reg_n_1_[11]\,
      R => '0'
    );
\buff1_reg[12]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_94,
      Q => \buff1_reg_n_1_[12]\,
      R => '0'
    );
\buff1_reg[13]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_93,
      Q => \buff1_reg_n_1_[13]\,
      R => '0'
    );
\buff1_reg[14]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_92,
      Q => \buff1_reg_n_1_[14]\,
      R => '0'
    );
\buff1_reg[15]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_91,
      Q => \buff1_reg_n_1_[15]\,
      R => '0'
    );
\buff1_reg[16]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_90,
      Q => \buff1_reg_n_1_[16]\,
      R => '0'
    );
\buff1_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_105,
      Q => \buff1_reg_n_1_[1]\,
      R => '0'
    );
\buff1_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_104,
      Q => \buff1_reg_n_1_[2]\,
      R => '0'
    );
\buff1_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_103,
      Q => \buff1_reg_n_1_[3]\,
      R => '0'
    );
\buff1_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_102,
      Q => \buff1_reg_n_1_[4]\,
      R => '0'
    );
\buff1_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_101,
      Q => \buff1_reg_n_1_[5]\,
      R => '0'
    );
\buff1_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_100,
      Q => \buff1_reg_n_1_[6]\,
      R => '0'
    );
\buff1_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_99,
      Q => \buff1_reg_n_1_[7]\,
      R => '0'
    );
\buff1_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_98,
      Q => \buff1_reg_n_1_[8]\,
      R => '0'
    );
\buff1_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_97,
      Q => \buff1_reg_n_1_[9]\,
      R => '0'
    );
\buff1_reg_i_1__1\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff1_reg_i_2__1_n_1\,
      CO(3 downto 2) => \NLW_buff1_reg_i_1__1_CO_UNCONNECTED\(3 downto 2),
      CO(1) => \buff1_reg_i_1__1_n_3\,
      CO(0) => \buff1_reg_i_1__1_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3) => \NLW_buff1_reg_i_1__1_O_UNCONNECTED\(3),
      O(2 downto 0) => grp_fu_146_p1(14 downto 12),
      S(3) => '0',
      S(2 downto 0) => \buff2_reg__0\(31 downto 29)
    );
\buff1_reg_i_2__1\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff1_reg_i_3__1_n_1\,
      CO(3) => \buff1_reg_i_2__1_n_1\,
      CO(2) => \buff1_reg_i_2__1_n_2\,
      CO(1) => \buff1_reg_i_2__1_n_3\,
      CO(0) => \buff1_reg_i_2__1_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_146_p1(11 downto 8),
      S(3 downto 0) => \buff2_reg__0\(28 downto 25)
    );
\buff1_reg_i_3__1\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff1_reg_i_4__1_n_1\,
      CO(3) => \buff1_reg_i_3__1_n_1\,
      CO(2) => \buff1_reg_i_3__1_n_2\,
      CO(1) => \buff1_reg_i_3__1_n_3\,
      CO(0) => \buff1_reg_i_3__1_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_146_p1(7 downto 4),
      S(3 downto 0) => \buff2_reg__0\(24 downto 21)
    );
\buff1_reg_i_4__1\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff0_reg_i_1__1_n_1\,
      CO(3) => \buff1_reg_i_4__1_n_1\,
      CO(2) => \buff1_reg_i_4__1_n_2\,
      CO(1) => \buff1_reg_i_4__1_n_3\,
      CO(0) => \buff1_reg_i_4__1_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_146_p1(3 downto 0),
      S(3 downto 0) => \buff2_reg__0\(20 downto 17)
    );
buff2_reg: unisim.vcomponents.DSP48E1
    generic map(
      ACASCREG => 2,
      ADREG => 1,
      ALUMODEREG => 0,
      AREG => 2,
      AUTORESET_PATDET => "NO_RESET",
      A_INPUT => "DIRECT",
      BCASCREG => 2,
      BREG => 2,
      B_INPUT => "DIRECT",
      CARRYINREG => 0,
      CARRYINSELREG => 0,
      CREG => 1,
      DREG => 1,
      INMODEREG => 0,
      MASK => X"3FFFFFFFFFFF",
      MREG => 1,
      OPMODEREG => 0,
      PATTERN => X"000000000000",
      PREG => 1,
      SEL_MASK => "MASK",
      SEL_PATTERN => "PATTERN",
      USE_DPORT => false,
      USE_MULT => "MULTIPLY",
      USE_PATTERN_DETECT => "NO_PATDET",
      USE_SIMD => "ONE48"
    )
        port map (
      A(29 downto 17) => B"0000000000000",
      A(16 downto 0) => grp_fu_135_p1(16 downto 0),
      ACIN(29 downto 0) => B"000000000000000000000000000000",
      ACOUT(29 downto 0) => NLW_buff2_reg_ACOUT_UNCONNECTED(29 downto 0),
      ALUMODE(3 downto 0) => B"0000",
      B(17) => buff2_reg_0(14),
      B(16) => buff2_reg_0(14),
      B(15) => buff2_reg_0(14),
      B(14 downto 0) => buff2_reg_0(14 downto 0),
      BCIN(17 downto 0) => B"000000000000000000",
      BCOUT(17 downto 0) => NLW_buff2_reg_BCOUT_UNCONNECTED(17 downto 0),
      C(47 downto 0) => B"111111111111111111111111111111111111111111111111",
      CARRYCASCIN => '0',
      CARRYCASCOUT => NLW_buff2_reg_CARRYCASCOUT_UNCONNECTED,
      CARRYIN => '0',
      CARRYINSEL(2 downto 0) => B"000",
      CARRYOUT(3 downto 0) => NLW_buff2_reg_CARRYOUT_UNCONNECTED(3 downto 0),
      CEA1 => '1',
      CEA2 => '1',
      CEAD => '0',
      CEALUMODE => '0',
      CEB1 => '1',
      CEB2 => '1',
      CEC => '0',
      CECARRYIN => '0',
      CECTRL => '0',
      CED => '0',
      CEINMODE => '0',
      CEM => '1',
      CEP => '1',
      CLK => ap_clk,
      D(24 downto 0) => B"0000000000000000000000000",
      INMODE(4 downto 0) => B"00000",
      MULTSIGNIN => '0',
      MULTSIGNOUT => NLW_buff2_reg_MULTSIGNOUT_UNCONNECTED,
      OPMODE(6 downto 0) => B"0010101",
      OVERFLOW => NLW_buff2_reg_OVERFLOW_UNCONNECTED,
      P(47) => buff2_reg_n_59,
      P(46) => buff2_reg_n_60,
      P(45) => buff2_reg_n_61,
      P(44) => buff2_reg_n_62,
      P(43) => buff2_reg_n_63,
      P(42) => buff2_reg_n_64,
      P(41) => buff2_reg_n_65,
      P(40) => buff2_reg_n_66,
      P(39) => buff2_reg_n_67,
      P(38) => buff2_reg_n_68,
      P(37) => buff2_reg_n_69,
      P(36) => buff2_reg_n_70,
      P(35) => buff2_reg_n_71,
      P(34) => buff2_reg_n_72,
      P(33) => buff2_reg_n_73,
      P(32) => buff2_reg_n_74,
      P(31) => buff2_reg_n_75,
      P(30) => buff2_reg_n_76,
      P(29) => buff2_reg_n_77,
      P(28) => buff2_reg_n_78,
      P(27) => buff2_reg_n_79,
      P(26) => buff2_reg_n_80,
      P(25) => buff2_reg_n_81,
      P(24) => buff2_reg_n_82,
      P(23) => buff2_reg_n_83,
      P(22) => buff2_reg_n_84,
      P(21) => buff2_reg_n_85,
      P(20) => buff2_reg_n_86,
      P(19) => buff2_reg_n_87,
      P(18) => buff2_reg_n_88,
      P(17) => buff2_reg_n_89,
      P(16) => buff2_reg_n_90,
      P(15) => buff2_reg_n_91,
      P(14 downto 0) => \buff2_reg__0\(31 downto 17),
      PATTERNBDETECT => NLW_buff2_reg_PATTERNBDETECT_UNCONNECTED,
      PATTERNDETECT => NLW_buff2_reg_PATTERNDETECT_UNCONNECTED,
      PCIN(47) => buff1_reg_n_107,
      PCIN(46) => buff1_reg_n_108,
      PCIN(45) => buff1_reg_n_109,
      PCIN(44) => buff1_reg_n_110,
      PCIN(43) => buff1_reg_n_111,
      PCIN(42) => buff1_reg_n_112,
      PCIN(41) => buff1_reg_n_113,
      PCIN(40) => buff1_reg_n_114,
      PCIN(39) => buff1_reg_n_115,
      PCIN(38) => buff1_reg_n_116,
      PCIN(37) => buff1_reg_n_117,
      PCIN(36) => buff1_reg_n_118,
      PCIN(35) => buff1_reg_n_119,
      PCIN(34) => buff1_reg_n_120,
      PCIN(33) => buff1_reg_n_121,
      PCIN(32) => buff1_reg_n_122,
      PCIN(31) => buff1_reg_n_123,
      PCIN(30) => buff1_reg_n_124,
      PCIN(29) => buff1_reg_n_125,
      PCIN(28) => buff1_reg_n_126,
      PCIN(27) => buff1_reg_n_127,
      PCIN(26) => buff1_reg_n_128,
      PCIN(25) => buff1_reg_n_129,
      PCIN(24) => buff1_reg_n_130,
      PCIN(23) => buff1_reg_n_131,
      PCIN(22) => buff1_reg_n_132,
      PCIN(21) => buff1_reg_n_133,
      PCIN(20) => buff1_reg_n_134,
      PCIN(19) => buff1_reg_n_135,
      PCIN(18) => buff1_reg_n_136,
      PCIN(17) => buff1_reg_n_137,
      PCIN(16) => buff1_reg_n_138,
      PCIN(15) => buff1_reg_n_139,
      PCIN(14) => buff1_reg_n_140,
      PCIN(13) => buff1_reg_n_141,
      PCIN(12) => buff1_reg_n_142,
      PCIN(11) => buff1_reg_n_143,
      PCIN(10) => buff1_reg_n_144,
      PCIN(9) => buff1_reg_n_145,
      PCIN(8) => buff1_reg_n_146,
      PCIN(7) => buff1_reg_n_147,
      PCIN(6) => buff1_reg_n_148,
      PCIN(5) => buff1_reg_n_149,
      PCIN(4) => buff1_reg_n_150,
      PCIN(3) => buff1_reg_n_151,
      PCIN(2) => buff1_reg_n_152,
      PCIN(1) => buff1_reg_n_153,
      PCIN(0) => buff1_reg_n_154,
      PCOUT(47 downto 0) => NLW_buff2_reg_PCOUT_UNCONNECTED(47 downto 0),
      RSTA => '0',
      RSTALLCARRYIN => '0',
      RSTALUMODE => '0',
      RSTB => '0',
      RSTC => '0',
      RSTCTRL => '0',
      RSTD => '0',
      RSTINMODE => '0',
      RSTM => '0',
      RSTP => '0',
      UNDERFLOW => NLW_buff2_reg_UNDERFLOW_UNCONNECTED
    );
\buff2_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[0]\,
      Q => A(0),
      R => '0'
    );
\buff2_reg[10]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[10]\,
      Q => \buff2_reg__0\(10),
      R => '0'
    );
\buff2_reg[11]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[11]\,
      Q => \buff2_reg__0\(11),
      R => '0'
    );
\buff2_reg[12]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[12]\,
      Q => \buff2_reg__0\(12),
      R => '0'
    );
\buff2_reg[13]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[13]\,
      Q => \buff2_reg__0\(13),
      R => '0'
    );
\buff2_reg[14]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[14]\,
      Q => \buff2_reg__0\(14),
      R => '0'
    );
\buff2_reg[15]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[15]\,
      Q => \buff2_reg__0\(15),
      R => '0'
    );
\buff2_reg[16]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[16]\,
      Q => \buff2_reg__0\(16),
      R => '0'
    );
\buff2_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[1]\,
      Q => \buff2_reg__0\(1),
      R => '0'
    );
\buff2_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[2]\,
      Q => \buff2_reg__0\(2),
      R => '0'
    );
\buff2_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[3]\,
      Q => \buff2_reg__0\(3),
      R => '0'
    );
\buff2_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[4]\,
      Q => \buff2_reg__0\(4),
      R => '0'
    );
\buff2_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[5]\,
      Q => \buff2_reg__0\(5),
      R => '0'
    );
\buff2_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[6]\,
      Q => \buff2_reg__0\(6),
      R => '0'
    );
\buff2_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[7]\,
      Q => \buff2_reg__0\(7),
      R => '0'
    );
\buff2_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[8]\,
      Q => \buff2_reg__0\(8),
      R => '0'
    );
\buff2_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[9]\,
      Q => \buff2_reg__0\(9),
      R => '0'
    );
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_MulnS_0_11 is
  port (
    grp_fu_135_p1 : out STD_LOGIC_VECTOR ( 31 downto 0 );
    ap_clk : in STD_LOGIC;
    A_q0 : in STD_LOGIC_VECTOR ( 31 downto 0 )
  );
  attribute ORIG_REF_NAME : string;
  attribute ORIG_REF_NAME of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_MulnS_0_11 : entity is "loop_imperfect_mubkb_MulnS_0";
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_MulnS_0_11;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_MulnS_0_11 is
  signal \buff0_reg_i_1__0_n_1\ : STD_LOGIC;
  signal \buff0_reg_i_1__0_n_2\ : STD_LOGIC;
  signal \buff0_reg_i_1__0_n_3\ : STD_LOGIC;
  signal \buff0_reg_i_1__0_n_4\ : STD_LOGIC;
  signal buff0_reg_i_1_n_1 : STD_LOGIC;
  signal buff0_reg_i_1_n_2 : STD_LOGIC;
  signal buff0_reg_i_1_n_3 : STD_LOGIC;
  signal buff0_reg_i_1_n_4 : STD_LOGIC;
  signal \buff0_reg_i_2__0_n_1\ : STD_LOGIC;
  signal \buff0_reg_i_2__0_n_2\ : STD_LOGIC;
  signal \buff0_reg_i_2__0_n_3\ : STD_LOGIC;
  signal \buff0_reg_i_2__0_n_4\ : STD_LOGIC;
  signal buff0_reg_i_2_n_1 : STD_LOGIC;
  signal buff0_reg_i_2_n_2 : STD_LOGIC;
  signal buff0_reg_i_2_n_3 : STD_LOGIC;
  signal buff0_reg_i_2_n_4 : STD_LOGIC;
  signal \buff0_reg_i_3__0_n_1\ : STD_LOGIC;
  signal \buff0_reg_i_3__0_n_2\ : STD_LOGIC;
  signal \buff0_reg_i_3__0_n_3\ : STD_LOGIC;
  signal \buff0_reg_i_3__0_n_4\ : STD_LOGIC;
  signal buff0_reg_i_3_n_1 : STD_LOGIC;
  signal buff0_reg_i_3_n_2 : STD_LOGIC;
  signal buff0_reg_i_3_n_3 : STD_LOGIC;
  signal buff0_reg_i_3_n_4 : STD_LOGIC;
  signal \buff0_reg_i_4__0_n_1\ : STD_LOGIC;
  signal \buff0_reg_i_4__0_n_2\ : STD_LOGIC;
  signal \buff0_reg_i_4__0_n_3\ : STD_LOGIC;
  signal \buff0_reg_i_4__0_n_4\ : STD_LOGIC;
  signal buff0_reg_i_4_n_1 : STD_LOGIC;
  signal buff0_reg_i_4_n_2 : STD_LOGIC;
  signal buff0_reg_i_4_n_3 : STD_LOGIC;
  signal buff0_reg_i_4_n_4 : STD_LOGIC;
  signal buff0_reg_i_5_n_1 : STD_LOGIC;
  signal \buff0_reg_i_6__0_n_1\ : STD_LOGIC;
  signal buff0_reg_i_6_n_1 : STD_LOGIC;
  signal \buff0_reg_i_7__0_n_1\ : STD_LOGIC;
  signal buff0_reg_i_7_n_1 : STD_LOGIC;
  signal buff0_reg_i_8_n_1 : STD_LOGIC;
  signal buff0_reg_n_100 : STD_LOGIC;
  signal buff0_reg_n_101 : STD_LOGIC;
  signal buff0_reg_n_102 : STD_LOGIC;
  signal buff0_reg_n_103 : STD_LOGIC;
  signal buff0_reg_n_104 : STD_LOGIC;
  signal buff0_reg_n_105 : STD_LOGIC;
  signal buff0_reg_n_106 : STD_LOGIC;
  signal buff0_reg_n_107 : STD_LOGIC;
  signal buff0_reg_n_108 : STD_LOGIC;
  signal buff0_reg_n_109 : STD_LOGIC;
  signal buff0_reg_n_110 : STD_LOGIC;
  signal buff0_reg_n_111 : STD_LOGIC;
  signal buff0_reg_n_112 : STD_LOGIC;
  signal buff0_reg_n_113 : STD_LOGIC;
  signal buff0_reg_n_114 : STD_LOGIC;
  signal buff0_reg_n_115 : STD_LOGIC;
  signal buff0_reg_n_116 : STD_LOGIC;
  signal buff0_reg_n_117 : STD_LOGIC;
  signal buff0_reg_n_118 : STD_LOGIC;
  signal buff0_reg_n_119 : STD_LOGIC;
  signal buff0_reg_n_120 : STD_LOGIC;
  signal buff0_reg_n_121 : STD_LOGIC;
  signal buff0_reg_n_122 : STD_LOGIC;
  signal buff0_reg_n_123 : STD_LOGIC;
  signal buff0_reg_n_124 : STD_LOGIC;
  signal buff0_reg_n_125 : STD_LOGIC;
  signal buff0_reg_n_126 : STD_LOGIC;
  signal buff0_reg_n_127 : STD_LOGIC;
  signal buff0_reg_n_128 : STD_LOGIC;
  signal buff0_reg_n_129 : STD_LOGIC;
  signal buff0_reg_n_130 : STD_LOGIC;
  signal buff0_reg_n_131 : STD_LOGIC;
  signal buff0_reg_n_132 : STD_LOGIC;
  signal buff0_reg_n_133 : STD_LOGIC;
  signal buff0_reg_n_134 : STD_LOGIC;
  signal buff0_reg_n_135 : STD_LOGIC;
  signal buff0_reg_n_136 : STD_LOGIC;
  signal buff0_reg_n_137 : STD_LOGIC;
  signal buff0_reg_n_138 : STD_LOGIC;
  signal buff0_reg_n_139 : STD_LOGIC;
  signal buff0_reg_n_140 : STD_LOGIC;
  signal buff0_reg_n_141 : STD_LOGIC;
  signal buff0_reg_n_142 : STD_LOGIC;
  signal buff0_reg_n_143 : STD_LOGIC;
  signal buff0_reg_n_144 : STD_LOGIC;
  signal buff0_reg_n_145 : STD_LOGIC;
  signal buff0_reg_n_146 : STD_LOGIC;
  signal buff0_reg_n_147 : STD_LOGIC;
  signal buff0_reg_n_148 : STD_LOGIC;
  signal buff0_reg_n_149 : STD_LOGIC;
  signal buff0_reg_n_150 : STD_LOGIC;
  signal buff0_reg_n_151 : STD_LOGIC;
  signal buff0_reg_n_152 : STD_LOGIC;
  signal buff0_reg_n_153 : STD_LOGIC;
  signal buff0_reg_n_154 : STD_LOGIC;
  signal buff0_reg_n_59 : STD_LOGIC;
  signal buff0_reg_n_60 : STD_LOGIC;
  signal buff0_reg_n_61 : STD_LOGIC;
  signal buff0_reg_n_62 : STD_LOGIC;
  signal buff0_reg_n_63 : STD_LOGIC;
  signal buff0_reg_n_64 : STD_LOGIC;
  signal buff0_reg_n_65 : STD_LOGIC;
  signal buff0_reg_n_66 : STD_LOGIC;
  signal buff0_reg_n_67 : STD_LOGIC;
  signal buff0_reg_n_68 : STD_LOGIC;
  signal buff0_reg_n_69 : STD_LOGIC;
  signal buff0_reg_n_70 : STD_LOGIC;
  signal buff0_reg_n_71 : STD_LOGIC;
  signal buff0_reg_n_72 : STD_LOGIC;
  signal buff0_reg_n_73 : STD_LOGIC;
  signal buff0_reg_n_74 : STD_LOGIC;
  signal buff0_reg_n_75 : STD_LOGIC;
  signal buff0_reg_n_76 : STD_LOGIC;
  signal buff0_reg_n_77 : STD_LOGIC;
  signal buff0_reg_n_78 : STD_LOGIC;
  signal buff0_reg_n_79 : STD_LOGIC;
  signal buff0_reg_n_80 : STD_LOGIC;
  signal buff0_reg_n_81 : STD_LOGIC;
  signal buff0_reg_n_82 : STD_LOGIC;
  signal buff0_reg_n_83 : STD_LOGIC;
  signal buff0_reg_n_84 : STD_LOGIC;
  signal buff0_reg_n_85 : STD_LOGIC;
  signal buff0_reg_n_86 : STD_LOGIC;
  signal buff0_reg_n_87 : STD_LOGIC;
  signal buff0_reg_n_88 : STD_LOGIC;
  signal buff0_reg_n_89 : STD_LOGIC;
  signal buff0_reg_n_90 : STD_LOGIC;
  signal buff0_reg_n_91 : STD_LOGIC;
  signal buff0_reg_n_92 : STD_LOGIC;
  signal buff0_reg_n_93 : STD_LOGIC;
  signal buff0_reg_n_94 : STD_LOGIC;
  signal buff0_reg_n_95 : STD_LOGIC;
  signal buff0_reg_n_96 : STD_LOGIC;
  signal buff0_reg_n_97 : STD_LOGIC;
  signal buff0_reg_n_98 : STD_LOGIC;
  signal buff0_reg_n_99 : STD_LOGIC;
  signal \buff1_reg_i_1__0_n_3\ : STD_LOGIC;
  signal \buff1_reg_i_1__0_n_4\ : STD_LOGIC;
  signal \buff1_reg_i_2__0_n_1\ : STD_LOGIC;
  signal \buff1_reg_i_2__0_n_2\ : STD_LOGIC;
  signal \buff1_reg_i_2__0_n_3\ : STD_LOGIC;
  signal \buff1_reg_i_2__0_n_4\ : STD_LOGIC;
  signal buff1_reg_i_2_n_1 : STD_LOGIC;
  signal buff1_reg_i_2_n_2 : STD_LOGIC;
  signal buff1_reg_i_2_n_3 : STD_LOGIC;
  signal buff1_reg_i_2_n_4 : STD_LOGIC;
  signal \buff1_reg_i_3__0_n_1\ : STD_LOGIC;
  signal \buff1_reg_i_3__0_n_2\ : STD_LOGIC;
  signal \buff1_reg_i_3__0_n_3\ : STD_LOGIC;
  signal \buff1_reg_i_3__0_n_4\ : STD_LOGIC;
  signal buff1_reg_i_3_n_1 : STD_LOGIC;
  signal buff1_reg_i_3_n_2 : STD_LOGIC;
  signal buff1_reg_i_3_n_3 : STD_LOGIC;
  signal buff1_reg_i_3_n_4 : STD_LOGIC;
  signal \buff1_reg_i_4__0_n_1\ : STD_LOGIC;
  signal \buff1_reg_i_4__0_n_2\ : STD_LOGIC;
  signal \buff1_reg_i_4__0_n_3\ : STD_LOGIC;
  signal \buff1_reg_i_4__0_n_4\ : STD_LOGIC;
  signal buff1_reg_i_4_n_1 : STD_LOGIC;
  signal buff1_reg_i_4_n_2 : STD_LOGIC;
  signal buff1_reg_i_4_n_3 : STD_LOGIC;
  signal buff1_reg_i_4_n_4 : STD_LOGIC;
  signal buff1_reg_n_107 : STD_LOGIC;
  signal buff1_reg_n_108 : STD_LOGIC;
  signal buff1_reg_n_109 : STD_LOGIC;
  signal buff1_reg_n_110 : STD_LOGIC;
  signal buff1_reg_n_111 : STD_LOGIC;
  signal buff1_reg_n_112 : STD_LOGIC;
  signal buff1_reg_n_113 : STD_LOGIC;
  signal buff1_reg_n_114 : STD_LOGIC;
  signal buff1_reg_n_115 : STD_LOGIC;
  signal buff1_reg_n_116 : STD_LOGIC;
  signal buff1_reg_n_117 : STD_LOGIC;
  signal buff1_reg_n_118 : STD_LOGIC;
  signal buff1_reg_n_119 : STD_LOGIC;
  signal buff1_reg_n_120 : STD_LOGIC;
  signal buff1_reg_n_121 : STD_LOGIC;
  signal buff1_reg_n_122 : STD_LOGIC;
  signal buff1_reg_n_123 : STD_LOGIC;
  signal buff1_reg_n_124 : STD_LOGIC;
  signal buff1_reg_n_125 : STD_LOGIC;
  signal buff1_reg_n_126 : STD_LOGIC;
  signal buff1_reg_n_127 : STD_LOGIC;
  signal buff1_reg_n_128 : STD_LOGIC;
  signal buff1_reg_n_129 : STD_LOGIC;
  signal buff1_reg_n_130 : STD_LOGIC;
  signal buff1_reg_n_131 : STD_LOGIC;
  signal buff1_reg_n_132 : STD_LOGIC;
  signal buff1_reg_n_133 : STD_LOGIC;
  signal buff1_reg_n_134 : STD_LOGIC;
  signal buff1_reg_n_135 : STD_LOGIC;
  signal buff1_reg_n_136 : STD_LOGIC;
  signal buff1_reg_n_137 : STD_LOGIC;
  signal buff1_reg_n_138 : STD_LOGIC;
  signal buff1_reg_n_139 : STD_LOGIC;
  signal buff1_reg_n_140 : STD_LOGIC;
  signal buff1_reg_n_141 : STD_LOGIC;
  signal buff1_reg_n_142 : STD_LOGIC;
  signal buff1_reg_n_143 : STD_LOGIC;
  signal buff1_reg_n_144 : STD_LOGIC;
  signal buff1_reg_n_145 : STD_LOGIC;
  signal buff1_reg_n_146 : STD_LOGIC;
  signal buff1_reg_n_147 : STD_LOGIC;
  signal buff1_reg_n_148 : STD_LOGIC;
  signal buff1_reg_n_149 : STD_LOGIC;
  signal buff1_reg_n_150 : STD_LOGIC;
  signal buff1_reg_n_151 : STD_LOGIC;
  signal buff1_reg_n_152 : STD_LOGIC;
  signal buff1_reg_n_153 : STD_LOGIC;
  signal buff1_reg_n_154 : STD_LOGIC;
  signal \buff1_reg_n_1_[0]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[10]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[11]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[12]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[13]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[14]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[15]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[16]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[1]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[2]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[3]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[4]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[5]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[6]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[7]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[8]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[9]\ : STD_LOGIC;
  signal \buff2_reg__0\ : STD_LOGIC_VECTOR ( 31 downto 0 );
  signal buff2_reg_n_59 : STD_LOGIC;
  signal buff2_reg_n_60 : STD_LOGIC;
  signal buff2_reg_n_61 : STD_LOGIC;
  signal buff2_reg_n_62 : STD_LOGIC;
  signal buff2_reg_n_63 : STD_LOGIC;
  signal buff2_reg_n_64 : STD_LOGIC;
  signal buff2_reg_n_65 : STD_LOGIC;
  signal buff2_reg_n_66 : STD_LOGIC;
  signal buff2_reg_n_67 : STD_LOGIC;
  signal buff2_reg_n_68 : STD_LOGIC;
  signal buff2_reg_n_69 : STD_LOGIC;
  signal buff2_reg_n_70 : STD_LOGIC;
  signal buff2_reg_n_71 : STD_LOGIC;
  signal buff2_reg_n_72 : STD_LOGIC;
  signal buff2_reg_n_73 : STD_LOGIC;
  signal buff2_reg_n_74 : STD_LOGIC;
  signal buff2_reg_n_75 : STD_LOGIC;
  signal buff2_reg_n_76 : STD_LOGIC;
  signal buff2_reg_n_77 : STD_LOGIC;
  signal buff2_reg_n_78 : STD_LOGIC;
  signal buff2_reg_n_79 : STD_LOGIC;
  signal buff2_reg_n_80 : STD_LOGIC;
  signal buff2_reg_n_81 : STD_LOGIC;
  signal buff2_reg_n_82 : STD_LOGIC;
  signal buff2_reg_n_83 : STD_LOGIC;
  signal buff2_reg_n_84 : STD_LOGIC;
  signal buff2_reg_n_85 : STD_LOGIC;
  signal buff2_reg_n_86 : STD_LOGIC;
  signal buff2_reg_n_87 : STD_LOGIC;
  signal buff2_reg_n_88 : STD_LOGIC;
  signal buff2_reg_n_89 : STD_LOGIC;
  signal buff2_reg_n_90 : STD_LOGIC;
  signal buff2_reg_n_91 : STD_LOGIC;
  signal grp_fu_123_p1 : STD_LOGIC_VECTOR ( 31 downto 3 );
  signal NLW_buff0_reg_CARRYCASCOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_MULTSIGNOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_OVERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_PATTERNBDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_PATTERNDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_UNDERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_ACOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 29 downto 0 );
  signal NLW_buff0_reg_BCOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 17 downto 0 );
  signal NLW_buff0_reg_CARRYOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_buff1_reg_CARRYCASCOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_MULTSIGNOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_OVERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_PATTERNBDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_PATTERNDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_UNDERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_ACOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 29 downto 0 );
  signal NLW_buff1_reg_BCOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 17 downto 0 );
  signal NLW_buff1_reg_CARRYOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_buff1_reg_P_UNCONNECTED : STD_LOGIC_VECTOR ( 47 downto 0 );
  signal NLW_buff1_reg_i_1_CO_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_buff1_reg_i_1_O_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 1 );
  signal \NLW_buff1_reg_i_1__0_CO_UNCONNECTED\ : STD_LOGIC_VECTOR ( 3 downto 2 );
  signal \NLW_buff1_reg_i_1__0_O_UNCONNECTED\ : STD_LOGIC_VECTOR ( 3 to 3 );
  signal NLW_buff2_reg_CARRYCASCOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_MULTSIGNOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_OVERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_PATTERNBDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_PATTERNDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_UNDERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_ACOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 29 downto 0 );
  signal NLW_buff2_reg_BCOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 17 downto 0 );
  signal NLW_buff2_reg_CARRYOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_buff2_reg_PCOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 47 downto 0 );
begin
buff0_reg: unisim.vcomponents.DSP48E1
    generic map(
      ACASCREG => 1,
      ADREG => 1,
      ALUMODEREG => 0,
      AREG => 1,
      AUTORESET_PATDET => "NO_RESET",
      A_INPUT => "DIRECT",
      BCASCREG => 1,
      BREG => 1,
      B_INPUT => "DIRECT",
      CARRYINREG => 0,
      CARRYINSELREG => 0,
      CREG => 0,
      DREG => 1,
      INMODEREG => 0,
      MASK => X"3FFFFFFFFFFF",
      MREG => 0,
      OPMODEREG => 0,
      PATTERN => X"000000000000",
      PREG => 1,
      SEL_MASK => "MASK",
      SEL_PATTERN => "PATTERN",
      USE_DPORT => false,
      USE_MULT => "MULTIPLY",
      USE_PATTERN_DETECT => "NO_PATDET",
      USE_SIMD => "ONE48"
    )
        port map (
      A(29 downto 17) => B"0000000000000",
      A(16 downto 0) => A_q0(16 downto 0),
      ACIN(29 downto 0) => B"000000000000000000000000000000",
      ACOUT(29 downto 0) => NLW_buff0_reg_ACOUT_UNCONNECTED(29 downto 0),
      ALUMODE(3 downto 0) => B"0000",
      B(17) => '0',
      B(16 downto 3) => grp_fu_123_p1(16 downto 3),
      B(2 downto 0) => A_q0(2 downto 0),
      BCIN(17 downto 0) => B"000000000000000000",
      BCOUT(17 downto 0) => NLW_buff0_reg_BCOUT_UNCONNECTED(17 downto 0),
      C(47 downto 0) => B"111111111111111111111111111111111111111111111111",
      CARRYCASCIN => '0',
      CARRYCASCOUT => NLW_buff0_reg_CARRYCASCOUT_UNCONNECTED,
      CARRYIN => '0',
      CARRYINSEL(2 downto 0) => B"000",
      CARRYOUT(3 downto 0) => NLW_buff0_reg_CARRYOUT_UNCONNECTED(3 downto 0),
      CEA1 => '0',
      CEA2 => '1',
      CEAD => '0',
      CEALUMODE => '0',
      CEB1 => '0',
      CEB2 => '1',
      CEC => '0',
      CECARRYIN => '0',
      CECTRL => '0',
      CED => '0',
      CEINMODE => '0',
      CEM => '0',
      CEP => '1',
      CLK => ap_clk,
      D(24 downto 0) => B"0000000000000000000000000",
      INMODE(4 downto 0) => B"00000",
      MULTSIGNIN => '0',
      MULTSIGNOUT => NLW_buff0_reg_MULTSIGNOUT_UNCONNECTED,
      OPMODE(6 downto 0) => B"0000101",
      OVERFLOW => NLW_buff0_reg_OVERFLOW_UNCONNECTED,
      P(47) => buff0_reg_n_59,
      P(46) => buff0_reg_n_60,
      P(45) => buff0_reg_n_61,
      P(44) => buff0_reg_n_62,
      P(43) => buff0_reg_n_63,
      P(42) => buff0_reg_n_64,
      P(41) => buff0_reg_n_65,
      P(40) => buff0_reg_n_66,
      P(39) => buff0_reg_n_67,
      P(38) => buff0_reg_n_68,
      P(37) => buff0_reg_n_69,
      P(36) => buff0_reg_n_70,
      P(35) => buff0_reg_n_71,
      P(34) => buff0_reg_n_72,
      P(33) => buff0_reg_n_73,
      P(32) => buff0_reg_n_74,
      P(31) => buff0_reg_n_75,
      P(30) => buff0_reg_n_76,
      P(29) => buff0_reg_n_77,
      P(28) => buff0_reg_n_78,
      P(27) => buff0_reg_n_79,
      P(26) => buff0_reg_n_80,
      P(25) => buff0_reg_n_81,
      P(24) => buff0_reg_n_82,
      P(23) => buff0_reg_n_83,
      P(22) => buff0_reg_n_84,
      P(21) => buff0_reg_n_85,
      P(20) => buff0_reg_n_86,
      P(19) => buff0_reg_n_87,
      P(18) => buff0_reg_n_88,
      P(17) => buff0_reg_n_89,
      P(16) => buff0_reg_n_90,
      P(15) => buff0_reg_n_91,
      P(14) => buff0_reg_n_92,
      P(13) => buff0_reg_n_93,
      P(12) => buff0_reg_n_94,
      P(11) => buff0_reg_n_95,
      P(10) => buff0_reg_n_96,
      P(9) => buff0_reg_n_97,
      P(8) => buff0_reg_n_98,
      P(7) => buff0_reg_n_99,
      P(6) => buff0_reg_n_100,
      P(5) => buff0_reg_n_101,
      P(4) => buff0_reg_n_102,
      P(3) => buff0_reg_n_103,
      P(2) => buff0_reg_n_104,
      P(1) => buff0_reg_n_105,
      P(0) => buff0_reg_n_106,
      PATTERNBDETECT => NLW_buff0_reg_PATTERNBDETECT_UNCONNECTED,
      PATTERNDETECT => NLW_buff0_reg_PATTERNDETECT_UNCONNECTED,
      PCIN(47 downto 0) => B"000000000000000000000000000000000000000000000000",
      PCOUT(47) => buff0_reg_n_107,
      PCOUT(46) => buff0_reg_n_108,
      PCOUT(45) => buff0_reg_n_109,
      PCOUT(44) => buff0_reg_n_110,
      PCOUT(43) => buff0_reg_n_111,
      PCOUT(42) => buff0_reg_n_112,
      PCOUT(41) => buff0_reg_n_113,
      PCOUT(40) => buff0_reg_n_114,
      PCOUT(39) => buff0_reg_n_115,
      PCOUT(38) => buff0_reg_n_116,
      PCOUT(37) => buff0_reg_n_117,
      PCOUT(36) => buff0_reg_n_118,
      PCOUT(35) => buff0_reg_n_119,
      PCOUT(34) => buff0_reg_n_120,
      PCOUT(33) => buff0_reg_n_121,
      PCOUT(32) => buff0_reg_n_122,
      PCOUT(31) => buff0_reg_n_123,
      PCOUT(30) => buff0_reg_n_124,
      PCOUT(29) => buff0_reg_n_125,
      PCOUT(28) => buff0_reg_n_126,
      PCOUT(27) => buff0_reg_n_127,
      PCOUT(26) => buff0_reg_n_128,
      PCOUT(25) => buff0_reg_n_129,
      PCOUT(24) => buff0_reg_n_130,
      PCOUT(23) => buff0_reg_n_131,
      PCOUT(22) => buff0_reg_n_132,
      PCOUT(21) => buff0_reg_n_133,
      PCOUT(20) => buff0_reg_n_134,
      PCOUT(19) => buff0_reg_n_135,
      PCOUT(18) => buff0_reg_n_136,
      PCOUT(17) => buff0_reg_n_137,
      PCOUT(16) => buff0_reg_n_138,
      PCOUT(15) => buff0_reg_n_139,
      PCOUT(14) => buff0_reg_n_140,
      PCOUT(13) => buff0_reg_n_141,
      PCOUT(12) => buff0_reg_n_142,
      PCOUT(11) => buff0_reg_n_143,
      PCOUT(10) => buff0_reg_n_144,
      PCOUT(9) => buff0_reg_n_145,
      PCOUT(8) => buff0_reg_n_146,
      PCOUT(7) => buff0_reg_n_147,
      PCOUT(6) => buff0_reg_n_148,
      PCOUT(5) => buff0_reg_n_149,
      PCOUT(4) => buff0_reg_n_150,
      PCOUT(3) => buff0_reg_n_151,
      PCOUT(2) => buff0_reg_n_152,
      PCOUT(1) => buff0_reg_n_153,
      PCOUT(0) => buff0_reg_n_154,
      RSTA => '0',
      RSTALLCARRYIN => '0',
      RSTALUMODE => '0',
      RSTB => '0',
      RSTC => '0',
      RSTCTRL => '0',
      RSTD => '0',
      RSTINMODE => '0',
      RSTM => '0',
      RSTP => '0',
      UNDERFLOW => NLW_buff0_reg_UNDERFLOW_UNCONNECTED
    );
buff0_reg_i_1: unisim.vcomponents.CARRY4
     port map (
      CI => buff0_reg_i_2_n_1,
      CO(3) => buff0_reg_i_1_n_1,
      CO(2) => buff0_reg_i_1_n_2,
      CO(1) => buff0_reg_i_1_n_3,
      CO(0) => buff0_reg_i_1_n_4,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_123_p1(18 downto 15),
      S(3 downto 0) => A_q0(18 downto 15)
    );
\buff0_reg_i_1__0\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff0_reg_i_2__0_n_1\,
      CO(3) => \buff0_reg_i_1__0_n_1\,
      CO(2) => \buff0_reg_i_1__0_n_2\,
      CO(1) => \buff0_reg_i_1__0_n_3\,
      CO(0) => \buff0_reg_i_1__0_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_135_p1(16 downto 13),
      S(3 downto 0) => \buff2_reg__0\(16 downto 13)
    );
buff0_reg_i_2: unisim.vcomponents.CARRY4
     port map (
      CI => buff0_reg_i_3_n_1,
      CO(3) => buff0_reg_i_2_n_1,
      CO(2) => buff0_reg_i_2_n_2,
      CO(1) => buff0_reg_i_2_n_3,
      CO(0) => buff0_reg_i_2_n_4,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_123_p1(14 downto 11),
      S(3 downto 0) => A_q0(14 downto 11)
    );
\buff0_reg_i_2__0\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff0_reg_i_3__0_n_1\,
      CO(3) => \buff0_reg_i_2__0_n_1\,
      CO(2) => \buff0_reg_i_2__0_n_2\,
      CO(1) => \buff0_reg_i_2__0_n_3\,
      CO(0) => \buff0_reg_i_2__0_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_135_p1(12 downto 9),
      S(3 downto 0) => \buff2_reg__0\(12 downto 9)
    );
buff0_reg_i_3: unisim.vcomponents.CARRY4
     port map (
      CI => buff0_reg_i_4_n_1,
      CO(3) => buff0_reg_i_3_n_1,
      CO(2) => buff0_reg_i_3_n_2,
      CO(1) => buff0_reg_i_3_n_3,
      CO(0) => buff0_reg_i_3_n_4,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_123_p1(10 downto 7),
      S(3 downto 0) => A_q0(10 downto 7)
    );
\buff0_reg_i_3__0\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff0_reg_i_4__0_n_1\,
      CO(3) => \buff0_reg_i_3__0_n_1\,
      CO(2) => \buff0_reg_i_3__0_n_2\,
      CO(1) => \buff0_reg_i_3__0_n_3\,
      CO(0) => \buff0_reg_i_3__0_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_135_p1(8 downto 5),
      S(3 downto 0) => \buff2_reg__0\(8 downto 5)
    );
buff0_reg_i_4: unisim.vcomponents.CARRY4
     port map (
      CI => '0',
      CO(3) => buff0_reg_i_4_n_1,
      CO(2) => buff0_reg_i_4_n_2,
      CO(1) => buff0_reg_i_4_n_3,
      CO(0) => buff0_reg_i_4_n_4,
      CYINIT => '0',
      DI(3 downto 1) => A_q0(6 downto 4),
      DI(0) => '0',
      O(3 downto 0) => grp_fu_123_p1(6 downto 3),
      S(3) => buff0_reg_i_5_n_1,
      S(2) => buff0_reg_i_6_n_1,
      S(1) => buff0_reg_i_7_n_1,
      S(0) => A_q0(3)
    );
\buff0_reg_i_4__0\: unisim.vcomponents.CARRY4
     port map (
      CI => '0',
      CO(3) => \buff0_reg_i_4__0_n_1\,
      CO(2) => \buff0_reg_i_4__0_n_2\,
      CO(1) => \buff0_reg_i_4__0_n_3\,
      CO(0) => \buff0_reg_i_4__0_n_4\,
      CYINIT => \buff2_reg__0\(0),
      DI(3) => \buff2_reg__0\(4),
      DI(2) => '0',
      DI(1 downto 0) => \buff2_reg__0\(2 downto 1),
      O(3 downto 0) => grp_fu_135_p1(4 downto 1),
      S(3) => \buff0_reg_i_6__0_n_1\,
      S(2) => \buff2_reg__0\(3),
      S(1) => \buff0_reg_i_7__0_n_1\,
      S(0) => buff0_reg_i_8_n_1
    );
buff0_reg_i_5: unisim.vcomponents.LUT1
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => A_q0(6),
      O => buff0_reg_i_5_n_1
    );
\buff0_reg_i_5__0\: unisim.vcomponents.LUT1
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => \buff2_reg__0\(0),
      O => grp_fu_135_p1(0)
    );
buff0_reg_i_6: unisim.vcomponents.LUT1
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => A_q0(5),
      O => buff0_reg_i_6_n_1
    );
\buff0_reg_i_6__0\: unisim.vcomponents.LUT1
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => \buff2_reg__0\(4),
      O => \buff0_reg_i_6__0_n_1\
    );
buff0_reg_i_7: unisim.vcomponents.LUT1
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => A_q0(4),
      O => buff0_reg_i_7_n_1
    );
\buff0_reg_i_7__0\: unisim.vcomponents.LUT1
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => \buff2_reg__0\(2),
      O => \buff0_reg_i_7__0_n_1\
    );
buff0_reg_i_8: unisim.vcomponents.LUT1
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => \buff2_reg__0\(1),
      O => buff0_reg_i_8_n_1
    );
buff1_reg: unisim.vcomponents.DSP48E1
    generic map(
      ACASCREG => 1,
      ADREG => 1,
      ALUMODEREG => 0,
      AREG => 1,
      AUTORESET_PATDET => "NO_RESET",
      A_INPUT => "DIRECT",
      BCASCREG => 1,
      BREG => 1,
      B_INPUT => "DIRECT",
      CARRYINREG => 0,
      CARRYINSELREG => 0,
      CREG => 1,
      DREG => 1,
      INMODEREG => 0,
      MASK => X"3FFFFFFFFFFF",
      MREG => 1,
      OPMODEREG => 0,
      PATTERN => X"000000000000",
      PREG => 1,
      SEL_MASK => "MASK",
      SEL_PATTERN => "PATTERN",
      USE_DPORT => false,
      USE_MULT => "MULTIPLY",
      USE_PATTERN_DETECT => "NO_PATDET",
      USE_SIMD => "ONE48"
    )
        port map (
      A(29 downto 17) => B"0000000000000",
      A(16 downto 0) => A_q0(16 downto 0),
      ACIN(29 downto 0) => B"000000000000000000000000000000",
      ACOUT(29 downto 0) => NLW_buff1_reg_ACOUT_UNCONNECTED(29 downto 0),
      ALUMODE(3 downto 0) => B"0000",
      B(17) => grp_fu_123_p1(31),
      B(16) => grp_fu_123_p1(31),
      B(15) => grp_fu_123_p1(31),
      B(14 downto 0) => grp_fu_123_p1(31 downto 17),
      BCIN(17 downto 0) => B"000000000000000000",
      BCOUT(17 downto 0) => NLW_buff1_reg_BCOUT_UNCONNECTED(17 downto 0),
      C(47 downto 0) => B"111111111111111111111111111111111111111111111111",
      CARRYCASCIN => '0',
      CARRYCASCOUT => NLW_buff1_reg_CARRYCASCOUT_UNCONNECTED,
      CARRYIN => '0',
      CARRYINSEL(2 downto 0) => B"000",
      CARRYOUT(3 downto 0) => NLW_buff1_reg_CARRYOUT_UNCONNECTED(3 downto 0),
      CEA1 => '0',
      CEA2 => '1',
      CEAD => '0',
      CEALUMODE => '0',
      CEB1 => '0',
      CEB2 => '1',
      CEC => '0',
      CECARRYIN => '0',
      CECTRL => '0',
      CED => '0',
      CEINMODE => '0',
      CEM => '1',
      CEP => '1',
      CLK => ap_clk,
      D(24 downto 0) => B"0000000000000000000000000",
      INMODE(4 downto 0) => B"00000",
      MULTSIGNIN => '0',
      MULTSIGNOUT => NLW_buff1_reg_MULTSIGNOUT_UNCONNECTED,
      OPMODE(6 downto 0) => B"1010101",
      OVERFLOW => NLW_buff1_reg_OVERFLOW_UNCONNECTED,
      P(47 downto 0) => NLW_buff1_reg_P_UNCONNECTED(47 downto 0),
      PATTERNBDETECT => NLW_buff1_reg_PATTERNBDETECT_UNCONNECTED,
      PATTERNDETECT => NLW_buff1_reg_PATTERNDETECT_UNCONNECTED,
      PCIN(47) => buff0_reg_n_107,
      PCIN(46) => buff0_reg_n_108,
      PCIN(45) => buff0_reg_n_109,
      PCIN(44) => buff0_reg_n_110,
      PCIN(43) => buff0_reg_n_111,
      PCIN(42) => buff0_reg_n_112,
      PCIN(41) => buff0_reg_n_113,
      PCIN(40) => buff0_reg_n_114,
      PCIN(39) => buff0_reg_n_115,
      PCIN(38) => buff0_reg_n_116,
      PCIN(37) => buff0_reg_n_117,
      PCIN(36) => buff0_reg_n_118,
      PCIN(35) => buff0_reg_n_119,
      PCIN(34) => buff0_reg_n_120,
      PCIN(33) => buff0_reg_n_121,
      PCIN(32) => buff0_reg_n_122,
      PCIN(31) => buff0_reg_n_123,
      PCIN(30) => buff0_reg_n_124,
      PCIN(29) => buff0_reg_n_125,
      PCIN(28) => buff0_reg_n_126,
      PCIN(27) => buff0_reg_n_127,
      PCIN(26) => buff0_reg_n_128,
      PCIN(25) => buff0_reg_n_129,
      PCIN(24) => buff0_reg_n_130,
      PCIN(23) => buff0_reg_n_131,
      PCIN(22) => buff0_reg_n_132,
      PCIN(21) => buff0_reg_n_133,
      PCIN(20) => buff0_reg_n_134,
      PCIN(19) => buff0_reg_n_135,
      PCIN(18) => buff0_reg_n_136,
      PCIN(17) => buff0_reg_n_137,
      PCIN(16) => buff0_reg_n_138,
      PCIN(15) => buff0_reg_n_139,
      PCIN(14) => buff0_reg_n_140,
      PCIN(13) => buff0_reg_n_141,
      PCIN(12) => buff0_reg_n_142,
      PCIN(11) => buff0_reg_n_143,
      PCIN(10) => buff0_reg_n_144,
      PCIN(9) => buff0_reg_n_145,
      PCIN(8) => buff0_reg_n_146,
      PCIN(7) => buff0_reg_n_147,
      PCIN(6) => buff0_reg_n_148,
      PCIN(5) => buff0_reg_n_149,
      PCIN(4) => buff0_reg_n_150,
      PCIN(3) => buff0_reg_n_151,
      PCIN(2) => buff0_reg_n_152,
      PCIN(1) => buff0_reg_n_153,
      PCIN(0) => buff0_reg_n_154,
      PCOUT(47) => buff1_reg_n_107,
      PCOUT(46) => buff1_reg_n_108,
      PCOUT(45) => buff1_reg_n_109,
      PCOUT(44) => buff1_reg_n_110,
      PCOUT(43) => buff1_reg_n_111,
      PCOUT(42) => buff1_reg_n_112,
      PCOUT(41) => buff1_reg_n_113,
      PCOUT(40) => buff1_reg_n_114,
      PCOUT(39) => buff1_reg_n_115,
      PCOUT(38) => buff1_reg_n_116,
      PCOUT(37) => buff1_reg_n_117,
      PCOUT(36) => buff1_reg_n_118,
      PCOUT(35) => buff1_reg_n_119,
      PCOUT(34) => buff1_reg_n_120,
      PCOUT(33) => buff1_reg_n_121,
      PCOUT(32) => buff1_reg_n_122,
      PCOUT(31) => buff1_reg_n_123,
      PCOUT(30) => buff1_reg_n_124,
      PCOUT(29) => buff1_reg_n_125,
      PCOUT(28) => buff1_reg_n_126,
      PCOUT(27) => buff1_reg_n_127,
      PCOUT(26) => buff1_reg_n_128,
      PCOUT(25) => buff1_reg_n_129,
      PCOUT(24) => buff1_reg_n_130,
      PCOUT(23) => buff1_reg_n_131,
      PCOUT(22) => buff1_reg_n_132,
      PCOUT(21) => buff1_reg_n_133,
      PCOUT(20) => buff1_reg_n_134,
      PCOUT(19) => buff1_reg_n_135,
      PCOUT(18) => buff1_reg_n_136,
      PCOUT(17) => buff1_reg_n_137,
      PCOUT(16) => buff1_reg_n_138,
      PCOUT(15) => buff1_reg_n_139,
      PCOUT(14) => buff1_reg_n_140,
      PCOUT(13) => buff1_reg_n_141,
      PCOUT(12) => buff1_reg_n_142,
      PCOUT(11) => buff1_reg_n_143,
      PCOUT(10) => buff1_reg_n_144,
      PCOUT(9) => buff1_reg_n_145,
      PCOUT(8) => buff1_reg_n_146,
      PCOUT(7) => buff1_reg_n_147,
      PCOUT(6) => buff1_reg_n_148,
      PCOUT(5) => buff1_reg_n_149,
      PCOUT(4) => buff1_reg_n_150,
      PCOUT(3) => buff1_reg_n_151,
      PCOUT(2) => buff1_reg_n_152,
      PCOUT(1) => buff1_reg_n_153,
      PCOUT(0) => buff1_reg_n_154,
      RSTA => '0',
      RSTALLCARRYIN => '0',
      RSTALUMODE => '0',
      RSTB => '0',
      RSTC => '0',
      RSTCTRL => '0',
      RSTD => '0',
      RSTINMODE => '0',
      RSTM => '0',
      RSTP => '0',
      UNDERFLOW => NLW_buff1_reg_UNDERFLOW_UNCONNECTED
    );
\buff1_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_106,
      Q => \buff1_reg_n_1_[0]\,
      R => '0'
    );
\buff1_reg[10]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_96,
      Q => \buff1_reg_n_1_[10]\,
      R => '0'
    );
\buff1_reg[11]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_95,
      Q => \buff1_reg_n_1_[11]\,
      R => '0'
    );
\buff1_reg[12]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_94,
      Q => \buff1_reg_n_1_[12]\,
      R => '0'
    );
\buff1_reg[13]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_93,
      Q => \buff1_reg_n_1_[13]\,
      R => '0'
    );
\buff1_reg[14]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_92,
      Q => \buff1_reg_n_1_[14]\,
      R => '0'
    );
\buff1_reg[15]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_91,
      Q => \buff1_reg_n_1_[15]\,
      R => '0'
    );
\buff1_reg[16]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_90,
      Q => \buff1_reg_n_1_[16]\,
      R => '0'
    );
\buff1_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_105,
      Q => \buff1_reg_n_1_[1]\,
      R => '0'
    );
\buff1_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_104,
      Q => \buff1_reg_n_1_[2]\,
      R => '0'
    );
\buff1_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_103,
      Q => \buff1_reg_n_1_[3]\,
      R => '0'
    );
\buff1_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_102,
      Q => \buff1_reg_n_1_[4]\,
      R => '0'
    );
\buff1_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_101,
      Q => \buff1_reg_n_1_[5]\,
      R => '0'
    );
\buff1_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_100,
      Q => \buff1_reg_n_1_[6]\,
      R => '0'
    );
\buff1_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_99,
      Q => \buff1_reg_n_1_[7]\,
      R => '0'
    );
\buff1_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_98,
      Q => \buff1_reg_n_1_[8]\,
      R => '0'
    );
\buff1_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_97,
      Q => \buff1_reg_n_1_[9]\,
      R => '0'
    );
buff1_reg_i_1: unisim.vcomponents.CARRY4
     port map (
      CI => buff1_reg_i_2_n_1,
      CO(3 downto 0) => NLW_buff1_reg_i_1_CO_UNCONNECTED(3 downto 0),
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 1) => NLW_buff1_reg_i_1_O_UNCONNECTED(3 downto 1),
      O(0) => grp_fu_123_p1(31),
      S(3 downto 1) => B"000",
      S(0) => A_q0(31)
    );
\buff1_reg_i_1__0\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff1_reg_i_2__0_n_1\,
      CO(3 downto 2) => \NLW_buff1_reg_i_1__0_CO_UNCONNECTED\(3 downto 2),
      CO(1) => \buff1_reg_i_1__0_n_3\,
      CO(0) => \buff1_reg_i_1__0_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3) => \NLW_buff1_reg_i_1__0_O_UNCONNECTED\(3),
      O(2 downto 0) => grp_fu_135_p1(31 downto 29),
      S(3) => '0',
      S(2 downto 0) => \buff2_reg__0\(31 downto 29)
    );
buff1_reg_i_2: unisim.vcomponents.CARRY4
     port map (
      CI => buff1_reg_i_3_n_1,
      CO(3) => buff1_reg_i_2_n_1,
      CO(2) => buff1_reg_i_2_n_2,
      CO(1) => buff1_reg_i_2_n_3,
      CO(0) => buff1_reg_i_2_n_4,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_123_p1(30 downto 27),
      S(3 downto 0) => A_q0(30 downto 27)
    );
\buff1_reg_i_2__0\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff1_reg_i_3__0_n_1\,
      CO(3) => \buff1_reg_i_2__0_n_1\,
      CO(2) => \buff1_reg_i_2__0_n_2\,
      CO(1) => \buff1_reg_i_2__0_n_3\,
      CO(0) => \buff1_reg_i_2__0_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_135_p1(28 downto 25),
      S(3 downto 0) => \buff2_reg__0\(28 downto 25)
    );
buff1_reg_i_3: unisim.vcomponents.CARRY4
     port map (
      CI => buff1_reg_i_4_n_1,
      CO(3) => buff1_reg_i_3_n_1,
      CO(2) => buff1_reg_i_3_n_2,
      CO(1) => buff1_reg_i_3_n_3,
      CO(0) => buff1_reg_i_3_n_4,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_123_p1(26 downto 23),
      S(3 downto 0) => A_q0(26 downto 23)
    );
\buff1_reg_i_3__0\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff1_reg_i_4__0_n_1\,
      CO(3) => \buff1_reg_i_3__0_n_1\,
      CO(2) => \buff1_reg_i_3__0_n_2\,
      CO(1) => \buff1_reg_i_3__0_n_3\,
      CO(0) => \buff1_reg_i_3__0_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_135_p1(24 downto 21),
      S(3 downto 0) => \buff2_reg__0\(24 downto 21)
    );
buff1_reg_i_4: unisim.vcomponents.CARRY4
     port map (
      CI => buff0_reg_i_1_n_1,
      CO(3) => buff1_reg_i_4_n_1,
      CO(2) => buff1_reg_i_4_n_2,
      CO(1) => buff1_reg_i_4_n_3,
      CO(0) => buff1_reg_i_4_n_4,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_123_p1(22 downto 19),
      S(3 downto 0) => A_q0(22 downto 19)
    );
\buff1_reg_i_4__0\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff0_reg_i_1__0_n_1\,
      CO(3) => \buff1_reg_i_4__0_n_1\,
      CO(2) => \buff1_reg_i_4__0_n_2\,
      CO(1) => \buff1_reg_i_4__0_n_3\,
      CO(0) => \buff1_reg_i_4__0_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_135_p1(20 downto 17),
      S(3 downto 0) => \buff2_reg__0\(20 downto 17)
    );
buff2_reg: unisim.vcomponents.DSP48E1
    generic map(
      ACASCREG => 2,
      ADREG => 1,
      ALUMODEREG => 0,
      AREG => 2,
      AUTORESET_PATDET => "NO_RESET",
      A_INPUT => "DIRECT",
      BCASCREG => 2,
      BREG => 2,
      B_INPUT => "DIRECT",
      CARRYINREG => 0,
      CARRYINSELREG => 0,
      CREG => 1,
      DREG => 1,
      INMODEREG => 0,
      MASK => X"3FFFFFFFFFFF",
      MREG => 1,
      OPMODEREG => 0,
      PATTERN => X"000000000000",
      PREG => 1,
      SEL_MASK => "MASK",
      SEL_PATTERN => "PATTERN",
      USE_DPORT => false,
      USE_MULT => "MULTIPLY",
      USE_PATTERN_DETECT => "NO_PATDET",
      USE_SIMD => "ONE48"
    )
        port map (
      A(29 downto 17) => B"0000000000000",
      A(16 downto 3) => grp_fu_123_p1(16 downto 3),
      A(2 downto 0) => A_q0(2 downto 0),
      ACIN(29 downto 0) => B"000000000000000000000000000000",
      ACOUT(29 downto 0) => NLW_buff2_reg_ACOUT_UNCONNECTED(29 downto 0),
      ALUMODE(3 downto 0) => B"0000",
      B(17) => A_q0(31),
      B(16) => A_q0(31),
      B(15) => A_q0(31),
      B(14 downto 0) => A_q0(31 downto 17),
      BCIN(17 downto 0) => B"000000000000000000",
      BCOUT(17 downto 0) => NLW_buff2_reg_BCOUT_UNCONNECTED(17 downto 0),
      C(47 downto 0) => B"111111111111111111111111111111111111111111111111",
      CARRYCASCIN => '0',
      CARRYCASCOUT => NLW_buff2_reg_CARRYCASCOUT_UNCONNECTED,
      CARRYIN => '0',
      CARRYINSEL(2 downto 0) => B"000",
      CARRYOUT(3 downto 0) => NLW_buff2_reg_CARRYOUT_UNCONNECTED(3 downto 0),
      CEA1 => '1',
      CEA2 => '1',
      CEAD => '0',
      CEALUMODE => '0',
      CEB1 => '1',
      CEB2 => '1',
      CEC => '0',
      CECARRYIN => '0',
      CECTRL => '0',
      CED => '0',
      CEINMODE => '0',
      CEM => '1',
      CEP => '1',
      CLK => ap_clk,
      D(24 downto 0) => B"0000000000000000000000000",
      INMODE(4 downto 0) => B"00000",
      MULTSIGNIN => '0',
      MULTSIGNOUT => NLW_buff2_reg_MULTSIGNOUT_UNCONNECTED,
      OPMODE(6 downto 0) => B"0010101",
      OVERFLOW => NLW_buff2_reg_OVERFLOW_UNCONNECTED,
      P(47) => buff2_reg_n_59,
      P(46) => buff2_reg_n_60,
      P(45) => buff2_reg_n_61,
      P(44) => buff2_reg_n_62,
      P(43) => buff2_reg_n_63,
      P(42) => buff2_reg_n_64,
      P(41) => buff2_reg_n_65,
      P(40) => buff2_reg_n_66,
      P(39) => buff2_reg_n_67,
      P(38) => buff2_reg_n_68,
      P(37) => buff2_reg_n_69,
      P(36) => buff2_reg_n_70,
      P(35) => buff2_reg_n_71,
      P(34) => buff2_reg_n_72,
      P(33) => buff2_reg_n_73,
      P(32) => buff2_reg_n_74,
      P(31) => buff2_reg_n_75,
      P(30) => buff2_reg_n_76,
      P(29) => buff2_reg_n_77,
      P(28) => buff2_reg_n_78,
      P(27) => buff2_reg_n_79,
      P(26) => buff2_reg_n_80,
      P(25) => buff2_reg_n_81,
      P(24) => buff2_reg_n_82,
      P(23) => buff2_reg_n_83,
      P(22) => buff2_reg_n_84,
      P(21) => buff2_reg_n_85,
      P(20) => buff2_reg_n_86,
      P(19) => buff2_reg_n_87,
      P(18) => buff2_reg_n_88,
      P(17) => buff2_reg_n_89,
      P(16) => buff2_reg_n_90,
      P(15) => buff2_reg_n_91,
      P(14 downto 0) => \buff2_reg__0\(31 downto 17),
      PATTERNBDETECT => NLW_buff2_reg_PATTERNBDETECT_UNCONNECTED,
      PATTERNDETECT => NLW_buff2_reg_PATTERNDETECT_UNCONNECTED,
      PCIN(47) => buff1_reg_n_107,
      PCIN(46) => buff1_reg_n_108,
      PCIN(45) => buff1_reg_n_109,
      PCIN(44) => buff1_reg_n_110,
      PCIN(43) => buff1_reg_n_111,
      PCIN(42) => buff1_reg_n_112,
      PCIN(41) => buff1_reg_n_113,
      PCIN(40) => buff1_reg_n_114,
      PCIN(39) => buff1_reg_n_115,
      PCIN(38) => buff1_reg_n_116,
      PCIN(37) => buff1_reg_n_117,
      PCIN(36) => buff1_reg_n_118,
      PCIN(35) => buff1_reg_n_119,
      PCIN(34) => buff1_reg_n_120,
      PCIN(33) => buff1_reg_n_121,
      PCIN(32) => buff1_reg_n_122,
      PCIN(31) => buff1_reg_n_123,
      PCIN(30) => buff1_reg_n_124,
      PCIN(29) => buff1_reg_n_125,
      PCIN(28) => buff1_reg_n_126,
      PCIN(27) => buff1_reg_n_127,
      PCIN(26) => buff1_reg_n_128,
      PCIN(25) => buff1_reg_n_129,
      PCIN(24) => buff1_reg_n_130,
      PCIN(23) => buff1_reg_n_131,
      PCIN(22) => buff1_reg_n_132,
      PCIN(21) => buff1_reg_n_133,
      PCIN(20) => buff1_reg_n_134,
      PCIN(19) => buff1_reg_n_135,
      PCIN(18) => buff1_reg_n_136,
      PCIN(17) => buff1_reg_n_137,
      PCIN(16) => buff1_reg_n_138,
      PCIN(15) => buff1_reg_n_139,
      PCIN(14) => buff1_reg_n_140,
      PCIN(13) => buff1_reg_n_141,
      PCIN(12) => buff1_reg_n_142,
      PCIN(11) => buff1_reg_n_143,
      PCIN(10) => buff1_reg_n_144,
      PCIN(9) => buff1_reg_n_145,
      PCIN(8) => buff1_reg_n_146,
      PCIN(7) => buff1_reg_n_147,
      PCIN(6) => buff1_reg_n_148,
      PCIN(5) => buff1_reg_n_149,
      PCIN(4) => buff1_reg_n_150,
      PCIN(3) => buff1_reg_n_151,
      PCIN(2) => buff1_reg_n_152,
      PCIN(1) => buff1_reg_n_153,
      PCIN(0) => buff1_reg_n_154,
      PCOUT(47 downto 0) => NLW_buff2_reg_PCOUT_UNCONNECTED(47 downto 0),
      RSTA => '0',
      RSTALLCARRYIN => '0',
      RSTALUMODE => '0',
      RSTB => '0',
      RSTC => '0',
      RSTCTRL => '0',
      RSTD => '0',
      RSTINMODE => '0',
      RSTM => '0',
      RSTP => '0',
      UNDERFLOW => NLW_buff2_reg_UNDERFLOW_UNCONNECTED
    );
\buff2_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[0]\,
      Q => \buff2_reg__0\(0),
      R => '0'
    );
\buff2_reg[10]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[10]\,
      Q => \buff2_reg__0\(10),
      R => '0'
    );
\buff2_reg[11]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[11]\,
      Q => \buff2_reg__0\(11),
      R => '0'
    );
\buff2_reg[12]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[12]\,
      Q => \buff2_reg__0\(12),
      R => '0'
    );
\buff2_reg[13]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[13]\,
      Q => \buff2_reg__0\(13),
      R => '0'
    );
\buff2_reg[14]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[14]\,
      Q => \buff2_reg__0\(14),
      R => '0'
    );
\buff2_reg[15]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[15]\,
      Q => \buff2_reg__0\(15),
      R => '0'
    );
\buff2_reg[16]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[16]\,
      Q => \buff2_reg__0\(16),
      R => '0'
    );
\buff2_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[1]\,
      Q => \buff2_reg__0\(1),
      R => '0'
    );
\buff2_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[2]\,
      Q => \buff2_reg__0\(2),
      R => '0'
    );
\buff2_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[3]\,
      Q => \buff2_reg__0\(3),
      R => '0'
    );
\buff2_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[4]\,
      Q => \buff2_reg__0\(4),
      R => '0'
    );
\buff2_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[5]\,
      Q => \buff2_reg__0\(5),
      R => '0'
    );
\buff2_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[6]\,
      Q => \buff2_reg__0\(6),
      R => '0'
    );
\buff2_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[7]\,
      Q => \buff2_reg__0\(7),
      R => '0'
    );
\buff2_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[8]\,
      Q => \buff2_reg__0\(8),
      R => '0'
    );
\buff2_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[9]\,
      Q => \buff2_reg__0\(9),
      R => '0'
    );
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_MulnS_0_6 is
  port (
    A : out STD_LOGIC_VECTOR ( 16 downto 0 );
    grp_fu_190_p1 : out STD_LOGIC_VECTOR ( 14 downto 0 );
    Q : in STD_LOGIC_VECTOR ( 0 to 0 );
    ap_clk : in STD_LOGIC;
    grp_fu_179_p1 : in STD_LOGIC_VECTOR ( 31 downto 0 );
    A_q0 : in STD_LOGIC_VECTOR ( 16 downto 0 );
    buff2_reg_0 : in STD_LOGIC_VECTOR ( 14 downto 0 )
  );
  attribute ORIG_REF_NAME : string;
  attribute ORIG_REF_NAME of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_MulnS_0_6 : entity is "loop_imperfect_mubkb_MulnS_0";
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_MulnS_0_6;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_MulnS_0_6 is
  signal \buff0_reg_i_1__5_n_1\ : STD_LOGIC;
  signal \buff0_reg_i_1__5_n_2\ : STD_LOGIC;
  signal \buff0_reg_i_1__5_n_3\ : STD_LOGIC;
  signal \buff0_reg_i_1__5_n_4\ : STD_LOGIC;
  signal \buff0_reg_i_2__5_n_1\ : STD_LOGIC;
  signal \buff0_reg_i_2__5_n_2\ : STD_LOGIC;
  signal \buff0_reg_i_2__5_n_3\ : STD_LOGIC;
  signal \buff0_reg_i_2__5_n_4\ : STD_LOGIC;
  signal \buff0_reg_i_3__5_n_1\ : STD_LOGIC;
  signal \buff0_reg_i_3__5_n_2\ : STD_LOGIC;
  signal \buff0_reg_i_3__5_n_3\ : STD_LOGIC;
  signal \buff0_reg_i_3__5_n_4\ : STD_LOGIC;
  signal \buff0_reg_i_4__5_n_1\ : STD_LOGIC;
  signal \buff0_reg_i_4__5_n_2\ : STD_LOGIC;
  signal \buff0_reg_i_4__5_n_3\ : STD_LOGIC;
  signal \buff0_reg_i_4__5_n_4\ : STD_LOGIC;
  signal \buff0_reg_i_5__5_n_1\ : STD_LOGIC;
  signal \buff0_reg_i_6__5_n_1\ : STD_LOGIC;
  signal buff0_reg_n_100 : STD_LOGIC;
  signal buff0_reg_n_101 : STD_LOGIC;
  signal buff0_reg_n_102 : STD_LOGIC;
  signal buff0_reg_n_103 : STD_LOGIC;
  signal buff0_reg_n_104 : STD_LOGIC;
  signal buff0_reg_n_105 : STD_LOGIC;
  signal buff0_reg_n_106 : STD_LOGIC;
  signal buff0_reg_n_107 : STD_LOGIC;
  signal buff0_reg_n_108 : STD_LOGIC;
  signal buff0_reg_n_109 : STD_LOGIC;
  signal buff0_reg_n_110 : STD_LOGIC;
  signal buff0_reg_n_111 : STD_LOGIC;
  signal buff0_reg_n_112 : STD_LOGIC;
  signal buff0_reg_n_113 : STD_LOGIC;
  signal buff0_reg_n_114 : STD_LOGIC;
  signal buff0_reg_n_115 : STD_LOGIC;
  signal buff0_reg_n_116 : STD_LOGIC;
  signal buff0_reg_n_117 : STD_LOGIC;
  signal buff0_reg_n_118 : STD_LOGIC;
  signal buff0_reg_n_119 : STD_LOGIC;
  signal buff0_reg_n_120 : STD_LOGIC;
  signal buff0_reg_n_121 : STD_LOGIC;
  signal buff0_reg_n_122 : STD_LOGIC;
  signal buff0_reg_n_123 : STD_LOGIC;
  signal buff0_reg_n_124 : STD_LOGIC;
  signal buff0_reg_n_125 : STD_LOGIC;
  signal buff0_reg_n_126 : STD_LOGIC;
  signal buff0_reg_n_127 : STD_LOGIC;
  signal buff0_reg_n_128 : STD_LOGIC;
  signal buff0_reg_n_129 : STD_LOGIC;
  signal buff0_reg_n_130 : STD_LOGIC;
  signal buff0_reg_n_131 : STD_LOGIC;
  signal buff0_reg_n_132 : STD_LOGIC;
  signal buff0_reg_n_133 : STD_LOGIC;
  signal buff0_reg_n_134 : STD_LOGIC;
  signal buff0_reg_n_135 : STD_LOGIC;
  signal buff0_reg_n_136 : STD_LOGIC;
  signal buff0_reg_n_137 : STD_LOGIC;
  signal buff0_reg_n_138 : STD_LOGIC;
  signal buff0_reg_n_139 : STD_LOGIC;
  signal buff0_reg_n_140 : STD_LOGIC;
  signal buff0_reg_n_141 : STD_LOGIC;
  signal buff0_reg_n_142 : STD_LOGIC;
  signal buff0_reg_n_143 : STD_LOGIC;
  signal buff0_reg_n_144 : STD_LOGIC;
  signal buff0_reg_n_145 : STD_LOGIC;
  signal buff0_reg_n_146 : STD_LOGIC;
  signal buff0_reg_n_147 : STD_LOGIC;
  signal buff0_reg_n_148 : STD_LOGIC;
  signal buff0_reg_n_149 : STD_LOGIC;
  signal buff0_reg_n_150 : STD_LOGIC;
  signal buff0_reg_n_151 : STD_LOGIC;
  signal buff0_reg_n_152 : STD_LOGIC;
  signal buff0_reg_n_153 : STD_LOGIC;
  signal buff0_reg_n_154 : STD_LOGIC;
  signal buff0_reg_n_59 : STD_LOGIC;
  signal buff0_reg_n_60 : STD_LOGIC;
  signal buff0_reg_n_61 : STD_LOGIC;
  signal buff0_reg_n_62 : STD_LOGIC;
  signal buff0_reg_n_63 : STD_LOGIC;
  signal buff0_reg_n_64 : STD_LOGIC;
  signal buff0_reg_n_65 : STD_LOGIC;
  signal buff0_reg_n_66 : STD_LOGIC;
  signal buff0_reg_n_67 : STD_LOGIC;
  signal buff0_reg_n_68 : STD_LOGIC;
  signal buff0_reg_n_69 : STD_LOGIC;
  signal buff0_reg_n_70 : STD_LOGIC;
  signal buff0_reg_n_71 : STD_LOGIC;
  signal buff0_reg_n_72 : STD_LOGIC;
  signal buff0_reg_n_73 : STD_LOGIC;
  signal buff0_reg_n_74 : STD_LOGIC;
  signal buff0_reg_n_75 : STD_LOGIC;
  signal buff0_reg_n_76 : STD_LOGIC;
  signal buff0_reg_n_77 : STD_LOGIC;
  signal buff0_reg_n_78 : STD_LOGIC;
  signal buff0_reg_n_79 : STD_LOGIC;
  signal buff0_reg_n_80 : STD_LOGIC;
  signal buff0_reg_n_81 : STD_LOGIC;
  signal buff0_reg_n_82 : STD_LOGIC;
  signal buff0_reg_n_83 : STD_LOGIC;
  signal buff0_reg_n_84 : STD_LOGIC;
  signal buff0_reg_n_85 : STD_LOGIC;
  signal buff0_reg_n_86 : STD_LOGIC;
  signal buff0_reg_n_87 : STD_LOGIC;
  signal buff0_reg_n_88 : STD_LOGIC;
  signal buff0_reg_n_89 : STD_LOGIC;
  signal buff0_reg_n_90 : STD_LOGIC;
  signal buff0_reg_n_91 : STD_LOGIC;
  signal buff0_reg_n_92 : STD_LOGIC;
  signal buff0_reg_n_93 : STD_LOGIC;
  signal buff0_reg_n_94 : STD_LOGIC;
  signal buff0_reg_n_95 : STD_LOGIC;
  signal buff0_reg_n_96 : STD_LOGIC;
  signal buff0_reg_n_97 : STD_LOGIC;
  signal buff0_reg_n_98 : STD_LOGIC;
  signal buff0_reg_n_99 : STD_LOGIC;
  signal \buff1_reg_i_1__5_n_3\ : STD_LOGIC;
  signal \buff1_reg_i_1__5_n_4\ : STD_LOGIC;
  signal \buff1_reg_i_2__5_n_1\ : STD_LOGIC;
  signal \buff1_reg_i_2__5_n_2\ : STD_LOGIC;
  signal \buff1_reg_i_2__5_n_3\ : STD_LOGIC;
  signal \buff1_reg_i_2__5_n_4\ : STD_LOGIC;
  signal \buff1_reg_i_3__5_n_1\ : STD_LOGIC;
  signal \buff1_reg_i_3__5_n_2\ : STD_LOGIC;
  signal \buff1_reg_i_3__5_n_3\ : STD_LOGIC;
  signal \buff1_reg_i_3__5_n_4\ : STD_LOGIC;
  signal \buff1_reg_i_4__3_n_1\ : STD_LOGIC;
  signal \buff1_reg_i_4__3_n_2\ : STD_LOGIC;
  signal \buff1_reg_i_4__3_n_3\ : STD_LOGIC;
  signal \buff1_reg_i_4__3_n_4\ : STD_LOGIC;
  signal buff1_reg_n_107 : STD_LOGIC;
  signal buff1_reg_n_108 : STD_LOGIC;
  signal buff1_reg_n_109 : STD_LOGIC;
  signal buff1_reg_n_110 : STD_LOGIC;
  signal buff1_reg_n_111 : STD_LOGIC;
  signal buff1_reg_n_112 : STD_LOGIC;
  signal buff1_reg_n_113 : STD_LOGIC;
  signal buff1_reg_n_114 : STD_LOGIC;
  signal buff1_reg_n_115 : STD_LOGIC;
  signal buff1_reg_n_116 : STD_LOGIC;
  signal buff1_reg_n_117 : STD_LOGIC;
  signal buff1_reg_n_118 : STD_LOGIC;
  signal buff1_reg_n_119 : STD_LOGIC;
  signal buff1_reg_n_120 : STD_LOGIC;
  signal buff1_reg_n_121 : STD_LOGIC;
  signal buff1_reg_n_122 : STD_LOGIC;
  signal buff1_reg_n_123 : STD_LOGIC;
  signal buff1_reg_n_124 : STD_LOGIC;
  signal buff1_reg_n_125 : STD_LOGIC;
  signal buff1_reg_n_126 : STD_LOGIC;
  signal buff1_reg_n_127 : STD_LOGIC;
  signal buff1_reg_n_128 : STD_LOGIC;
  signal buff1_reg_n_129 : STD_LOGIC;
  signal buff1_reg_n_130 : STD_LOGIC;
  signal buff1_reg_n_131 : STD_LOGIC;
  signal buff1_reg_n_132 : STD_LOGIC;
  signal buff1_reg_n_133 : STD_LOGIC;
  signal buff1_reg_n_134 : STD_LOGIC;
  signal buff1_reg_n_135 : STD_LOGIC;
  signal buff1_reg_n_136 : STD_LOGIC;
  signal buff1_reg_n_137 : STD_LOGIC;
  signal buff1_reg_n_138 : STD_LOGIC;
  signal buff1_reg_n_139 : STD_LOGIC;
  signal buff1_reg_n_140 : STD_LOGIC;
  signal buff1_reg_n_141 : STD_LOGIC;
  signal buff1_reg_n_142 : STD_LOGIC;
  signal buff1_reg_n_143 : STD_LOGIC;
  signal buff1_reg_n_144 : STD_LOGIC;
  signal buff1_reg_n_145 : STD_LOGIC;
  signal buff1_reg_n_146 : STD_LOGIC;
  signal buff1_reg_n_147 : STD_LOGIC;
  signal buff1_reg_n_148 : STD_LOGIC;
  signal buff1_reg_n_149 : STD_LOGIC;
  signal buff1_reg_n_150 : STD_LOGIC;
  signal buff1_reg_n_151 : STD_LOGIC;
  signal buff1_reg_n_152 : STD_LOGIC;
  signal buff1_reg_n_153 : STD_LOGIC;
  signal buff1_reg_n_154 : STD_LOGIC;
  signal \buff1_reg_n_1_[0]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[10]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[11]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[12]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[13]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[14]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[15]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[16]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[1]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[2]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[3]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[4]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[5]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[6]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[7]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[8]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[9]\ : STD_LOGIC;
  signal \buff2_reg__0\ : STD_LOGIC_VECTOR ( 31 downto 1 );
  signal buff2_reg_n_59 : STD_LOGIC;
  signal buff2_reg_n_60 : STD_LOGIC;
  signal buff2_reg_n_61 : STD_LOGIC;
  signal buff2_reg_n_62 : STD_LOGIC;
  signal buff2_reg_n_63 : STD_LOGIC;
  signal buff2_reg_n_64 : STD_LOGIC;
  signal buff2_reg_n_65 : STD_LOGIC;
  signal buff2_reg_n_66 : STD_LOGIC;
  signal buff2_reg_n_67 : STD_LOGIC;
  signal buff2_reg_n_68 : STD_LOGIC;
  signal buff2_reg_n_69 : STD_LOGIC;
  signal buff2_reg_n_70 : STD_LOGIC;
  signal buff2_reg_n_71 : STD_LOGIC;
  signal buff2_reg_n_72 : STD_LOGIC;
  signal buff2_reg_n_73 : STD_LOGIC;
  signal buff2_reg_n_74 : STD_LOGIC;
  signal buff2_reg_n_75 : STD_LOGIC;
  signal buff2_reg_n_76 : STD_LOGIC;
  signal buff2_reg_n_77 : STD_LOGIC;
  signal buff2_reg_n_78 : STD_LOGIC;
  signal buff2_reg_n_79 : STD_LOGIC;
  signal buff2_reg_n_80 : STD_LOGIC;
  signal buff2_reg_n_81 : STD_LOGIC;
  signal buff2_reg_n_82 : STD_LOGIC;
  signal buff2_reg_n_83 : STD_LOGIC;
  signal buff2_reg_n_84 : STD_LOGIC;
  signal buff2_reg_n_85 : STD_LOGIC;
  signal buff2_reg_n_86 : STD_LOGIC;
  signal buff2_reg_n_87 : STD_LOGIC;
  signal buff2_reg_n_88 : STD_LOGIC;
  signal buff2_reg_n_89 : STD_LOGIC;
  signal buff2_reg_n_90 : STD_LOGIC;
  signal buff2_reg_n_91 : STD_LOGIC;
  signal NLW_buff0_reg_CARRYCASCOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_MULTSIGNOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_OVERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_PATTERNBDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_PATTERNDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_UNDERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_ACOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 29 downto 0 );
  signal NLW_buff0_reg_BCOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 17 downto 0 );
  signal NLW_buff0_reg_CARRYOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_buff1_reg_CARRYCASCOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_MULTSIGNOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_OVERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_PATTERNBDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_PATTERNDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_UNDERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_ACOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 29 downto 0 );
  signal NLW_buff1_reg_BCOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 17 downto 0 );
  signal NLW_buff1_reg_CARRYOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_buff1_reg_P_UNCONNECTED : STD_LOGIC_VECTOR ( 47 downto 0 );
  signal \NLW_buff1_reg_i_1__5_CO_UNCONNECTED\ : STD_LOGIC_VECTOR ( 3 downto 2 );
  signal \NLW_buff1_reg_i_1__5_O_UNCONNECTED\ : STD_LOGIC_VECTOR ( 3 to 3 );
  signal NLW_buff2_reg_CARRYCASCOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_MULTSIGNOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_OVERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_PATTERNBDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_PATTERNDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_UNDERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_ACOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 29 downto 0 );
  signal NLW_buff2_reg_BCOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 17 downto 0 );
  signal NLW_buff2_reg_CARRYOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_buff2_reg_PCOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 47 downto 0 );
begin
buff0_reg: unisim.vcomponents.DSP48E1
    generic map(
      ACASCREG => 2,
      ADREG => 1,
      ALUMODEREG => 0,
      AREG => 2,
      AUTORESET_PATDET => "NO_RESET",
      A_INPUT => "DIRECT",
      BCASCREG => 1,
      BREG => 1,
      B_INPUT => "DIRECT",
      CARRYINREG => 0,
      CARRYINSELREG => 0,
      CREG => 0,
      DREG => 1,
      INMODEREG => 0,
      MASK => X"3FFFFFFFFFFF",
      MREG => 0,
      OPMODEREG => 0,
      PATTERN => X"000000000000",
      PREG => 1,
      SEL_MASK => "MASK",
      SEL_PATTERN => "PATTERN",
      USE_DPORT => false,
      USE_MULT => "MULTIPLY",
      USE_PATTERN_DETECT => "NO_PATDET",
      USE_SIMD => "ONE48"
    )
        port map (
      A(29 downto 17) => B"0000000000000",
      A(16 downto 0) => A_q0(16 downto 0),
      ACIN(29 downto 0) => B"000000000000000000000000000000",
      ACOUT(29 downto 0) => NLW_buff0_reg_ACOUT_UNCONNECTED(29 downto 0),
      ALUMODE(3 downto 0) => B"0000",
      B(17) => '0',
      B(16 downto 0) => grp_fu_179_p1(16 downto 0),
      BCIN(17 downto 0) => B"000000000000000000",
      BCOUT(17 downto 0) => NLW_buff0_reg_BCOUT_UNCONNECTED(17 downto 0),
      C(47 downto 0) => B"111111111111111111111111111111111111111111111111",
      CARRYCASCIN => '0',
      CARRYCASCOUT => NLW_buff0_reg_CARRYCASCOUT_UNCONNECTED,
      CARRYIN => '0',
      CARRYINSEL(2 downto 0) => B"000",
      CARRYOUT(3 downto 0) => NLW_buff0_reg_CARRYOUT_UNCONNECTED(3 downto 0),
      CEA1 => Q(0),
      CEA2 => '1',
      CEAD => '0',
      CEALUMODE => '0',
      CEB1 => '0',
      CEB2 => '1',
      CEC => '0',
      CECARRYIN => '0',
      CECTRL => '0',
      CED => '0',
      CEINMODE => '0',
      CEM => '0',
      CEP => '1',
      CLK => ap_clk,
      D(24 downto 0) => B"0000000000000000000000000",
      INMODE(4 downto 0) => B"00000",
      MULTSIGNIN => '0',
      MULTSIGNOUT => NLW_buff0_reg_MULTSIGNOUT_UNCONNECTED,
      OPMODE(6 downto 0) => B"0000101",
      OVERFLOW => NLW_buff0_reg_OVERFLOW_UNCONNECTED,
      P(47) => buff0_reg_n_59,
      P(46) => buff0_reg_n_60,
      P(45) => buff0_reg_n_61,
      P(44) => buff0_reg_n_62,
      P(43) => buff0_reg_n_63,
      P(42) => buff0_reg_n_64,
      P(41) => buff0_reg_n_65,
      P(40) => buff0_reg_n_66,
      P(39) => buff0_reg_n_67,
      P(38) => buff0_reg_n_68,
      P(37) => buff0_reg_n_69,
      P(36) => buff0_reg_n_70,
      P(35) => buff0_reg_n_71,
      P(34) => buff0_reg_n_72,
      P(33) => buff0_reg_n_73,
      P(32) => buff0_reg_n_74,
      P(31) => buff0_reg_n_75,
      P(30) => buff0_reg_n_76,
      P(29) => buff0_reg_n_77,
      P(28) => buff0_reg_n_78,
      P(27) => buff0_reg_n_79,
      P(26) => buff0_reg_n_80,
      P(25) => buff0_reg_n_81,
      P(24) => buff0_reg_n_82,
      P(23) => buff0_reg_n_83,
      P(22) => buff0_reg_n_84,
      P(21) => buff0_reg_n_85,
      P(20) => buff0_reg_n_86,
      P(19) => buff0_reg_n_87,
      P(18) => buff0_reg_n_88,
      P(17) => buff0_reg_n_89,
      P(16) => buff0_reg_n_90,
      P(15) => buff0_reg_n_91,
      P(14) => buff0_reg_n_92,
      P(13) => buff0_reg_n_93,
      P(12) => buff0_reg_n_94,
      P(11) => buff0_reg_n_95,
      P(10) => buff0_reg_n_96,
      P(9) => buff0_reg_n_97,
      P(8) => buff0_reg_n_98,
      P(7) => buff0_reg_n_99,
      P(6) => buff0_reg_n_100,
      P(5) => buff0_reg_n_101,
      P(4) => buff0_reg_n_102,
      P(3) => buff0_reg_n_103,
      P(2) => buff0_reg_n_104,
      P(1) => buff0_reg_n_105,
      P(0) => buff0_reg_n_106,
      PATTERNBDETECT => NLW_buff0_reg_PATTERNBDETECT_UNCONNECTED,
      PATTERNDETECT => NLW_buff0_reg_PATTERNDETECT_UNCONNECTED,
      PCIN(47 downto 0) => B"000000000000000000000000000000000000000000000000",
      PCOUT(47) => buff0_reg_n_107,
      PCOUT(46) => buff0_reg_n_108,
      PCOUT(45) => buff0_reg_n_109,
      PCOUT(44) => buff0_reg_n_110,
      PCOUT(43) => buff0_reg_n_111,
      PCOUT(42) => buff0_reg_n_112,
      PCOUT(41) => buff0_reg_n_113,
      PCOUT(40) => buff0_reg_n_114,
      PCOUT(39) => buff0_reg_n_115,
      PCOUT(38) => buff0_reg_n_116,
      PCOUT(37) => buff0_reg_n_117,
      PCOUT(36) => buff0_reg_n_118,
      PCOUT(35) => buff0_reg_n_119,
      PCOUT(34) => buff0_reg_n_120,
      PCOUT(33) => buff0_reg_n_121,
      PCOUT(32) => buff0_reg_n_122,
      PCOUT(31) => buff0_reg_n_123,
      PCOUT(30) => buff0_reg_n_124,
      PCOUT(29) => buff0_reg_n_125,
      PCOUT(28) => buff0_reg_n_126,
      PCOUT(27) => buff0_reg_n_127,
      PCOUT(26) => buff0_reg_n_128,
      PCOUT(25) => buff0_reg_n_129,
      PCOUT(24) => buff0_reg_n_130,
      PCOUT(23) => buff0_reg_n_131,
      PCOUT(22) => buff0_reg_n_132,
      PCOUT(21) => buff0_reg_n_133,
      PCOUT(20) => buff0_reg_n_134,
      PCOUT(19) => buff0_reg_n_135,
      PCOUT(18) => buff0_reg_n_136,
      PCOUT(17) => buff0_reg_n_137,
      PCOUT(16) => buff0_reg_n_138,
      PCOUT(15) => buff0_reg_n_139,
      PCOUT(14) => buff0_reg_n_140,
      PCOUT(13) => buff0_reg_n_141,
      PCOUT(12) => buff0_reg_n_142,
      PCOUT(11) => buff0_reg_n_143,
      PCOUT(10) => buff0_reg_n_144,
      PCOUT(9) => buff0_reg_n_145,
      PCOUT(8) => buff0_reg_n_146,
      PCOUT(7) => buff0_reg_n_147,
      PCOUT(6) => buff0_reg_n_148,
      PCOUT(5) => buff0_reg_n_149,
      PCOUT(4) => buff0_reg_n_150,
      PCOUT(3) => buff0_reg_n_151,
      PCOUT(2) => buff0_reg_n_152,
      PCOUT(1) => buff0_reg_n_153,
      PCOUT(0) => buff0_reg_n_154,
      RSTA => '0',
      RSTALLCARRYIN => '0',
      RSTALUMODE => '0',
      RSTB => '0',
      RSTC => '0',
      RSTCTRL => '0',
      RSTD => '0',
      RSTINMODE => '0',
      RSTM => '0',
      RSTP => '0',
      UNDERFLOW => NLW_buff0_reg_UNDERFLOW_UNCONNECTED
    );
\buff0_reg_i_1__5\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff0_reg_i_2__5_n_1\,
      CO(3) => \buff0_reg_i_1__5_n_1\,
      CO(2) => \buff0_reg_i_1__5_n_2\,
      CO(1) => \buff0_reg_i_1__5_n_3\,
      CO(0) => \buff0_reg_i_1__5_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => A(16 downto 13),
      S(3 downto 0) => \buff2_reg__0\(16 downto 13)
    );
\buff0_reg_i_2__5\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff0_reg_i_3__5_n_1\,
      CO(3) => \buff0_reg_i_2__5_n_1\,
      CO(2) => \buff0_reg_i_2__5_n_2\,
      CO(1) => \buff0_reg_i_2__5_n_3\,
      CO(0) => \buff0_reg_i_2__5_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => A(12 downto 9),
      S(3 downto 0) => \buff2_reg__0\(12 downto 9)
    );
\buff0_reg_i_3__5\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff0_reg_i_4__5_n_1\,
      CO(3) => \buff0_reg_i_3__5_n_1\,
      CO(2) => \buff0_reg_i_3__5_n_2\,
      CO(1) => \buff0_reg_i_3__5_n_3\,
      CO(0) => \buff0_reg_i_3__5_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => A(8 downto 5),
      S(3 downto 0) => \buff2_reg__0\(8 downto 5)
    );
\buff0_reg_i_4__5\: unisim.vcomponents.CARRY4
     port map (
      CI => '0',
      CO(3) => \buff0_reg_i_4__5_n_1\,
      CO(2) => \buff0_reg_i_4__5_n_2\,
      CO(1) => \buff0_reg_i_4__5_n_3\,
      CO(0) => \buff0_reg_i_4__5_n_4\,
      CYINIT => '0',
      DI(3) => \buff2_reg__0\(4),
      DI(2) => '0',
      DI(1) => \buff2_reg__0\(2),
      DI(0) => '0',
      O(3 downto 0) => A(4 downto 1),
      S(3) => \buff0_reg_i_5__5_n_1\,
      S(2) => \buff2_reg__0\(3),
      S(1) => \buff0_reg_i_6__5_n_1\,
      S(0) => \buff2_reg__0\(1)
    );
\buff0_reg_i_5__5\: unisim.vcomponents.LUT1
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => \buff2_reg__0\(4),
      O => \buff0_reg_i_5__5_n_1\
    );
\buff0_reg_i_6__5\: unisim.vcomponents.LUT1
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => \buff2_reg__0\(2),
      O => \buff0_reg_i_6__5_n_1\
    );
buff1_reg: unisim.vcomponents.DSP48E1
    generic map(
      ACASCREG => 2,
      ADREG => 1,
      ALUMODEREG => 0,
      AREG => 2,
      AUTORESET_PATDET => "NO_RESET",
      A_INPUT => "DIRECT",
      BCASCREG => 1,
      BREG => 1,
      B_INPUT => "DIRECT",
      CARRYINREG => 0,
      CARRYINSELREG => 0,
      CREG => 1,
      DREG => 1,
      INMODEREG => 0,
      MASK => X"3FFFFFFFFFFF",
      MREG => 1,
      OPMODEREG => 0,
      PATTERN => X"000000000000",
      PREG => 1,
      SEL_MASK => "MASK",
      SEL_PATTERN => "PATTERN",
      USE_DPORT => false,
      USE_MULT => "MULTIPLY",
      USE_PATTERN_DETECT => "NO_PATDET",
      USE_SIMD => "ONE48"
    )
        port map (
      A(29 downto 17) => B"0000000000000",
      A(16 downto 0) => A_q0(16 downto 0),
      ACIN(29 downto 0) => B"000000000000000000000000000000",
      ACOUT(29 downto 0) => NLW_buff1_reg_ACOUT_UNCONNECTED(29 downto 0),
      ALUMODE(3 downto 0) => B"0000",
      B(17) => grp_fu_179_p1(31),
      B(16) => grp_fu_179_p1(31),
      B(15) => grp_fu_179_p1(31),
      B(14 downto 0) => grp_fu_179_p1(31 downto 17),
      BCIN(17 downto 0) => B"000000000000000000",
      BCOUT(17 downto 0) => NLW_buff1_reg_BCOUT_UNCONNECTED(17 downto 0),
      C(47 downto 0) => B"111111111111111111111111111111111111111111111111",
      CARRYCASCIN => '0',
      CARRYCASCOUT => NLW_buff1_reg_CARRYCASCOUT_UNCONNECTED,
      CARRYIN => '0',
      CARRYINSEL(2 downto 0) => B"000",
      CARRYOUT(3 downto 0) => NLW_buff1_reg_CARRYOUT_UNCONNECTED(3 downto 0),
      CEA1 => Q(0),
      CEA2 => '1',
      CEAD => '0',
      CEALUMODE => '0',
      CEB1 => '0',
      CEB2 => '1',
      CEC => '0',
      CECARRYIN => '0',
      CECTRL => '0',
      CED => '0',
      CEINMODE => '0',
      CEM => '1',
      CEP => '1',
      CLK => ap_clk,
      D(24 downto 0) => B"0000000000000000000000000",
      INMODE(4 downto 0) => B"00000",
      MULTSIGNIN => '0',
      MULTSIGNOUT => NLW_buff1_reg_MULTSIGNOUT_UNCONNECTED,
      OPMODE(6 downto 0) => B"1010101",
      OVERFLOW => NLW_buff1_reg_OVERFLOW_UNCONNECTED,
      P(47 downto 0) => NLW_buff1_reg_P_UNCONNECTED(47 downto 0),
      PATTERNBDETECT => NLW_buff1_reg_PATTERNBDETECT_UNCONNECTED,
      PATTERNDETECT => NLW_buff1_reg_PATTERNDETECT_UNCONNECTED,
      PCIN(47) => buff0_reg_n_107,
      PCIN(46) => buff0_reg_n_108,
      PCIN(45) => buff0_reg_n_109,
      PCIN(44) => buff0_reg_n_110,
      PCIN(43) => buff0_reg_n_111,
      PCIN(42) => buff0_reg_n_112,
      PCIN(41) => buff0_reg_n_113,
      PCIN(40) => buff0_reg_n_114,
      PCIN(39) => buff0_reg_n_115,
      PCIN(38) => buff0_reg_n_116,
      PCIN(37) => buff0_reg_n_117,
      PCIN(36) => buff0_reg_n_118,
      PCIN(35) => buff0_reg_n_119,
      PCIN(34) => buff0_reg_n_120,
      PCIN(33) => buff0_reg_n_121,
      PCIN(32) => buff0_reg_n_122,
      PCIN(31) => buff0_reg_n_123,
      PCIN(30) => buff0_reg_n_124,
      PCIN(29) => buff0_reg_n_125,
      PCIN(28) => buff0_reg_n_126,
      PCIN(27) => buff0_reg_n_127,
      PCIN(26) => buff0_reg_n_128,
      PCIN(25) => buff0_reg_n_129,
      PCIN(24) => buff0_reg_n_130,
      PCIN(23) => buff0_reg_n_131,
      PCIN(22) => buff0_reg_n_132,
      PCIN(21) => buff0_reg_n_133,
      PCIN(20) => buff0_reg_n_134,
      PCIN(19) => buff0_reg_n_135,
      PCIN(18) => buff0_reg_n_136,
      PCIN(17) => buff0_reg_n_137,
      PCIN(16) => buff0_reg_n_138,
      PCIN(15) => buff0_reg_n_139,
      PCIN(14) => buff0_reg_n_140,
      PCIN(13) => buff0_reg_n_141,
      PCIN(12) => buff0_reg_n_142,
      PCIN(11) => buff0_reg_n_143,
      PCIN(10) => buff0_reg_n_144,
      PCIN(9) => buff0_reg_n_145,
      PCIN(8) => buff0_reg_n_146,
      PCIN(7) => buff0_reg_n_147,
      PCIN(6) => buff0_reg_n_148,
      PCIN(5) => buff0_reg_n_149,
      PCIN(4) => buff0_reg_n_150,
      PCIN(3) => buff0_reg_n_151,
      PCIN(2) => buff0_reg_n_152,
      PCIN(1) => buff0_reg_n_153,
      PCIN(0) => buff0_reg_n_154,
      PCOUT(47) => buff1_reg_n_107,
      PCOUT(46) => buff1_reg_n_108,
      PCOUT(45) => buff1_reg_n_109,
      PCOUT(44) => buff1_reg_n_110,
      PCOUT(43) => buff1_reg_n_111,
      PCOUT(42) => buff1_reg_n_112,
      PCOUT(41) => buff1_reg_n_113,
      PCOUT(40) => buff1_reg_n_114,
      PCOUT(39) => buff1_reg_n_115,
      PCOUT(38) => buff1_reg_n_116,
      PCOUT(37) => buff1_reg_n_117,
      PCOUT(36) => buff1_reg_n_118,
      PCOUT(35) => buff1_reg_n_119,
      PCOUT(34) => buff1_reg_n_120,
      PCOUT(33) => buff1_reg_n_121,
      PCOUT(32) => buff1_reg_n_122,
      PCOUT(31) => buff1_reg_n_123,
      PCOUT(30) => buff1_reg_n_124,
      PCOUT(29) => buff1_reg_n_125,
      PCOUT(28) => buff1_reg_n_126,
      PCOUT(27) => buff1_reg_n_127,
      PCOUT(26) => buff1_reg_n_128,
      PCOUT(25) => buff1_reg_n_129,
      PCOUT(24) => buff1_reg_n_130,
      PCOUT(23) => buff1_reg_n_131,
      PCOUT(22) => buff1_reg_n_132,
      PCOUT(21) => buff1_reg_n_133,
      PCOUT(20) => buff1_reg_n_134,
      PCOUT(19) => buff1_reg_n_135,
      PCOUT(18) => buff1_reg_n_136,
      PCOUT(17) => buff1_reg_n_137,
      PCOUT(16) => buff1_reg_n_138,
      PCOUT(15) => buff1_reg_n_139,
      PCOUT(14) => buff1_reg_n_140,
      PCOUT(13) => buff1_reg_n_141,
      PCOUT(12) => buff1_reg_n_142,
      PCOUT(11) => buff1_reg_n_143,
      PCOUT(10) => buff1_reg_n_144,
      PCOUT(9) => buff1_reg_n_145,
      PCOUT(8) => buff1_reg_n_146,
      PCOUT(7) => buff1_reg_n_147,
      PCOUT(6) => buff1_reg_n_148,
      PCOUT(5) => buff1_reg_n_149,
      PCOUT(4) => buff1_reg_n_150,
      PCOUT(3) => buff1_reg_n_151,
      PCOUT(2) => buff1_reg_n_152,
      PCOUT(1) => buff1_reg_n_153,
      PCOUT(0) => buff1_reg_n_154,
      RSTA => '0',
      RSTALLCARRYIN => '0',
      RSTALUMODE => '0',
      RSTB => '0',
      RSTC => '0',
      RSTCTRL => '0',
      RSTD => '0',
      RSTINMODE => '0',
      RSTM => '0',
      RSTP => '0',
      UNDERFLOW => NLW_buff1_reg_UNDERFLOW_UNCONNECTED
    );
\buff1_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_106,
      Q => \buff1_reg_n_1_[0]\,
      R => '0'
    );
\buff1_reg[10]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_96,
      Q => \buff1_reg_n_1_[10]\,
      R => '0'
    );
\buff1_reg[11]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_95,
      Q => \buff1_reg_n_1_[11]\,
      R => '0'
    );
\buff1_reg[12]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_94,
      Q => \buff1_reg_n_1_[12]\,
      R => '0'
    );
\buff1_reg[13]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_93,
      Q => \buff1_reg_n_1_[13]\,
      R => '0'
    );
\buff1_reg[14]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_92,
      Q => \buff1_reg_n_1_[14]\,
      R => '0'
    );
\buff1_reg[15]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_91,
      Q => \buff1_reg_n_1_[15]\,
      R => '0'
    );
\buff1_reg[16]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_90,
      Q => \buff1_reg_n_1_[16]\,
      R => '0'
    );
\buff1_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_105,
      Q => \buff1_reg_n_1_[1]\,
      R => '0'
    );
\buff1_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_104,
      Q => \buff1_reg_n_1_[2]\,
      R => '0'
    );
\buff1_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_103,
      Q => \buff1_reg_n_1_[3]\,
      R => '0'
    );
\buff1_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_102,
      Q => \buff1_reg_n_1_[4]\,
      R => '0'
    );
\buff1_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_101,
      Q => \buff1_reg_n_1_[5]\,
      R => '0'
    );
\buff1_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_100,
      Q => \buff1_reg_n_1_[6]\,
      R => '0'
    );
\buff1_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_99,
      Q => \buff1_reg_n_1_[7]\,
      R => '0'
    );
\buff1_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_98,
      Q => \buff1_reg_n_1_[8]\,
      R => '0'
    );
\buff1_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_97,
      Q => \buff1_reg_n_1_[9]\,
      R => '0'
    );
\buff1_reg_i_1__5\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff1_reg_i_2__5_n_1\,
      CO(3 downto 2) => \NLW_buff1_reg_i_1__5_CO_UNCONNECTED\(3 downto 2),
      CO(1) => \buff1_reg_i_1__5_n_3\,
      CO(0) => \buff1_reg_i_1__5_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3) => \NLW_buff1_reg_i_1__5_O_UNCONNECTED\(3),
      O(2 downto 0) => grp_fu_190_p1(14 downto 12),
      S(3) => '0',
      S(2 downto 0) => \buff2_reg__0\(31 downto 29)
    );
\buff1_reg_i_2__5\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff1_reg_i_3__5_n_1\,
      CO(3) => \buff1_reg_i_2__5_n_1\,
      CO(2) => \buff1_reg_i_2__5_n_2\,
      CO(1) => \buff1_reg_i_2__5_n_3\,
      CO(0) => \buff1_reg_i_2__5_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_190_p1(11 downto 8),
      S(3 downto 0) => \buff2_reg__0\(28 downto 25)
    );
\buff1_reg_i_3__5\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff1_reg_i_4__3_n_1\,
      CO(3) => \buff1_reg_i_3__5_n_1\,
      CO(2) => \buff1_reg_i_3__5_n_2\,
      CO(1) => \buff1_reg_i_3__5_n_3\,
      CO(0) => \buff1_reg_i_3__5_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_190_p1(7 downto 4),
      S(3 downto 0) => \buff2_reg__0\(24 downto 21)
    );
\buff1_reg_i_4__3\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff0_reg_i_1__5_n_1\,
      CO(3) => \buff1_reg_i_4__3_n_1\,
      CO(2) => \buff1_reg_i_4__3_n_2\,
      CO(1) => \buff1_reg_i_4__3_n_3\,
      CO(0) => \buff1_reg_i_4__3_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_190_p1(3 downto 0),
      S(3 downto 0) => \buff2_reg__0\(20 downto 17)
    );
buff2_reg: unisim.vcomponents.DSP48E1
    generic map(
      ACASCREG => 2,
      ADREG => 1,
      ALUMODEREG => 0,
      AREG => 2,
      AUTORESET_PATDET => "NO_RESET",
      A_INPUT => "DIRECT",
      BCASCREG => 2,
      BREG => 2,
      B_INPUT => "DIRECT",
      CARRYINREG => 0,
      CARRYINSELREG => 0,
      CREG => 1,
      DREG => 1,
      INMODEREG => 0,
      MASK => X"3FFFFFFFFFFF",
      MREG => 1,
      OPMODEREG => 0,
      PATTERN => X"000000000000",
      PREG => 1,
      SEL_MASK => "MASK",
      SEL_PATTERN => "PATTERN",
      USE_DPORT => false,
      USE_MULT => "MULTIPLY",
      USE_PATTERN_DETECT => "NO_PATDET",
      USE_SIMD => "ONE48"
    )
        port map (
      A(29 downto 17) => B"0000000000000",
      A(16 downto 0) => grp_fu_179_p1(16 downto 0),
      ACIN(29 downto 0) => B"000000000000000000000000000000",
      ACOUT(29 downto 0) => NLW_buff2_reg_ACOUT_UNCONNECTED(29 downto 0),
      ALUMODE(3 downto 0) => B"0000",
      B(17) => buff2_reg_0(14),
      B(16) => buff2_reg_0(14),
      B(15) => buff2_reg_0(14),
      B(14 downto 0) => buff2_reg_0(14 downto 0),
      BCIN(17 downto 0) => B"000000000000000000",
      BCOUT(17 downto 0) => NLW_buff2_reg_BCOUT_UNCONNECTED(17 downto 0),
      C(47 downto 0) => B"111111111111111111111111111111111111111111111111",
      CARRYCASCIN => '0',
      CARRYCASCOUT => NLW_buff2_reg_CARRYCASCOUT_UNCONNECTED,
      CARRYIN => '0',
      CARRYINSEL(2 downto 0) => B"000",
      CARRYOUT(3 downto 0) => NLW_buff2_reg_CARRYOUT_UNCONNECTED(3 downto 0),
      CEA1 => '1',
      CEA2 => '1',
      CEAD => '0',
      CEALUMODE => '0',
      CEB1 => '1',
      CEB2 => '1',
      CEC => '0',
      CECARRYIN => '0',
      CECTRL => '0',
      CED => '0',
      CEINMODE => '0',
      CEM => '1',
      CEP => '1',
      CLK => ap_clk,
      D(24 downto 0) => B"0000000000000000000000000",
      INMODE(4 downto 0) => B"00000",
      MULTSIGNIN => '0',
      MULTSIGNOUT => NLW_buff2_reg_MULTSIGNOUT_UNCONNECTED,
      OPMODE(6 downto 0) => B"0010101",
      OVERFLOW => NLW_buff2_reg_OVERFLOW_UNCONNECTED,
      P(47) => buff2_reg_n_59,
      P(46) => buff2_reg_n_60,
      P(45) => buff2_reg_n_61,
      P(44) => buff2_reg_n_62,
      P(43) => buff2_reg_n_63,
      P(42) => buff2_reg_n_64,
      P(41) => buff2_reg_n_65,
      P(40) => buff2_reg_n_66,
      P(39) => buff2_reg_n_67,
      P(38) => buff2_reg_n_68,
      P(37) => buff2_reg_n_69,
      P(36) => buff2_reg_n_70,
      P(35) => buff2_reg_n_71,
      P(34) => buff2_reg_n_72,
      P(33) => buff2_reg_n_73,
      P(32) => buff2_reg_n_74,
      P(31) => buff2_reg_n_75,
      P(30) => buff2_reg_n_76,
      P(29) => buff2_reg_n_77,
      P(28) => buff2_reg_n_78,
      P(27) => buff2_reg_n_79,
      P(26) => buff2_reg_n_80,
      P(25) => buff2_reg_n_81,
      P(24) => buff2_reg_n_82,
      P(23) => buff2_reg_n_83,
      P(22) => buff2_reg_n_84,
      P(21) => buff2_reg_n_85,
      P(20) => buff2_reg_n_86,
      P(19) => buff2_reg_n_87,
      P(18) => buff2_reg_n_88,
      P(17) => buff2_reg_n_89,
      P(16) => buff2_reg_n_90,
      P(15) => buff2_reg_n_91,
      P(14 downto 0) => \buff2_reg__0\(31 downto 17),
      PATTERNBDETECT => NLW_buff2_reg_PATTERNBDETECT_UNCONNECTED,
      PATTERNDETECT => NLW_buff2_reg_PATTERNDETECT_UNCONNECTED,
      PCIN(47) => buff1_reg_n_107,
      PCIN(46) => buff1_reg_n_108,
      PCIN(45) => buff1_reg_n_109,
      PCIN(44) => buff1_reg_n_110,
      PCIN(43) => buff1_reg_n_111,
      PCIN(42) => buff1_reg_n_112,
      PCIN(41) => buff1_reg_n_113,
      PCIN(40) => buff1_reg_n_114,
      PCIN(39) => buff1_reg_n_115,
      PCIN(38) => buff1_reg_n_116,
      PCIN(37) => buff1_reg_n_117,
      PCIN(36) => buff1_reg_n_118,
      PCIN(35) => buff1_reg_n_119,
      PCIN(34) => buff1_reg_n_120,
      PCIN(33) => buff1_reg_n_121,
      PCIN(32) => buff1_reg_n_122,
      PCIN(31) => buff1_reg_n_123,
      PCIN(30) => buff1_reg_n_124,
      PCIN(29) => buff1_reg_n_125,
      PCIN(28) => buff1_reg_n_126,
      PCIN(27) => buff1_reg_n_127,
      PCIN(26) => buff1_reg_n_128,
      PCIN(25) => buff1_reg_n_129,
      PCIN(24) => buff1_reg_n_130,
      PCIN(23) => buff1_reg_n_131,
      PCIN(22) => buff1_reg_n_132,
      PCIN(21) => buff1_reg_n_133,
      PCIN(20) => buff1_reg_n_134,
      PCIN(19) => buff1_reg_n_135,
      PCIN(18) => buff1_reg_n_136,
      PCIN(17) => buff1_reg_n_137,
      PCIN(16) => buff1_reg_n_138,
      PCIN(15) => buff1_reg_n_139,
      PCIN(14) => buff1_reg_n_140,
      PCIN(13) => buff1_reg_n_141,
      PCIN(12) => buff1_reg_n_142,
      PCIN(11) => buff1_reg_n_143,
      PCIN(10) => buff1_reg_n_144,
      PCIN(9) => buff1_reg_n_145,
      PCIN(8) => buff1_reg_n_146,
      PCIN(7) => buff1_reg_n_147,
      PCIN(6) => buff1_reg_n_148,
      PCIN(5) => buff1_reg_n_149,
      PCIN(4) => buff1_reg_n_150,
      PCIN(3) => buff1_reg_n_151,
      PCIN(2) => buff1_reg_n_152,
      PCIN(1) => buff1_reg_n_153,
      PCIN(0) => buff1_reg_n_154,
      PCOUT(47 downto 0) => NLW_buff2_reg_PCOUT_UNCONNECTED(47 downto 0),
      RSTA => '0',
      RSTALLCARRYIN => '0',
      RSTALUMODE => '0',
      RSTB => '0',
      RSTC => '0',
      RSTCTRL => '0',
      RSTD => '0',
      RSTINMODE => '0',
      RSTM => '0',
      RSTP => '0',
      UNDERFLOW => NLW_buff2_reg_UNDERFLOW_UNCONNECTED
    );
\buff2_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[0]\,
      Q => A(0),
      R => '0'
    );
\buff2_reg[10]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[10]\,
      Q => \buff2_reg__0\(10),
      R => '0'
    );
\buff2_reg[11]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[11]\,
      Q => \buff2_reg__0\(11),
      R => '0'
    );
\buff2_reg[12]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[12]\,
      Q => \buff2_reg__0\(12),
      R => '0'
    );
\buff2_reg[13]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[13]\,
      Q => \buff2_reg__0\(13),
      R => '0'
    );
\buff2_reg[14]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[14]\,
      Q => \buff2_reg__0\(14),
      R => '0'
    );
\buff2_reg[15]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[15]\,
      Q => \buff2_reg__0\(15),
      R => '0'
    );
\buff2_reg[16]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[16]\,
      Q => \buff2_reg__0\(16),
      R => '0'
    );
\buff2_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[1]\,
      Q => \buff2_reg__0\(1),
      R => '0'
    );
\buff2_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[2]\,
      Q => \buff2_reg__0\(2),
      R => '0'
    );
\buff2_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[3]\,
      Q => \buff2_reg__0\(3),
      R => '0'
    );
\buff2_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[4]\,
      Q => \buff2_reg__0\(4),
      R => '0'
    );
\buff2_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[5]\,
      Q => \buff2_reg__0\(5),
      R => '0'
    );
\buff2_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[6]\,
      Q => \buff2_reg__0\(6),
      R => '0'
    );
\buff2_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[7]\,
      Q => \buff2_reg__0\(7),
      R => '0'
    );
\buff2_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[8]\,
      Q => \buff2_reg__0\(8),
      R => '0'
    );
\buff2_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[9]\,
      Q => \buff2_reg__0\(9),
      R => '0'
    );
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_MulnS_0_7 is
  port (
    grp_fu_179_p1 : out STD_LOGIC_VECTOR ( 31 downto 0 );
    Q : in STD_LOGIC_VECTOR ( 0 to 0 );
    ap_clk : in STD_LOGIC;
    grp_fu_168_p1 : in STD_LOGIC_VECTOR ( 31 downto 0 );
    A_q0 : in STD_LOGIC_VECTOR ( 16 downto 0 );
    buff2_reg_0 : in STD_LOGIC_VECTOR ( 14 downto 0 )
  );
  attribute ORIG_REF_NAME : string;
  attribute ORIG_REF_NAME of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_MulnS_0_7 : entity is "loop_imperfect_mubkb_MulnS_0";
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_MulnS_0_7;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_MulnS_0_7 is
  signal \buff0_reg_i_1__4_n_1\ : STD_LOGIC;
  signal \buff0_reg_i_1__4_n_2\ : STD_LOGIC;
  signal \buff0_reg_i_1__4_n_3\ : STD_LOGIC;
  signal \buff0_reg_i_1__4_n_4\ : STD_LOGIC;
  signal \buff0_reg_i_2__4_n_1\ : STD_LOGIC;
  signal \buff0_reg_i_2__4_n_2\ : STD_LOGIC;
  signal \buff0_reg_i_2__4_n_3\ : STD_LOGIC;
  signal \buff0_reg_i_2__4_n_4\ : STD_LOGIC;
  signal \buff0_reg_i_3__4_n_1\ : STD_LOGIC;
  signal \buff0_reg_i_3__4_n_2\ : STD_LOGIC;
  signal \buff0_reg_i_3__4_n_3\ : STD_LOGIC;
  signal \buff0_reg_i_3__4_n_4\ : STD_LOGIC;
  signal \buff0_reg_i_4__4_n_1\ : STD_LOGIC;
  signal \buff0_reg_i_4__4_n_2\ : STD_LOGIC;
  signal \buff0_reg_i_4__4_n_3\ : STD_LOGIC;
  signal \buff0_reg_i_4__4_n_4\ : STD_LOGIC;
  signal \buff0_reg_i_5__4_n_1\ : STD_LOGIC;
  signal \buff0_reg_i_5__4_n_2\ : STD_LOGIC;
  signal \buff0_reg_i_5__4_n_3\ : STD_LOGIC;
  signal \buff0_reg_i_5__4_n_4\ : STD_LOGIC;
  signal \buff0_reg_i_6__4_n_1\ : STD_LOGIC;
  signal buff0_reg_n_100 : STD_LOGIC;
  signal buff0_reg_n_101 : STD_LOGIC;
  signal buff0_reg_n_102 : STD_LOGIC;
  signal buff0_reg_n_103 : STD_LOGIC;
  signal buff0_reg_n_104 : STD_LOGIC;
  signal buff0_reg_n_105 : STD_LOGIC;
  signal buff0_reg_n_106 : STD_LOGIC;
  signal buff0_reg_n_107 : STD_LOGIC;
  signal buff0_reg_n_108 : STD_LOGIC;
  signal buff0_reg_n_109 : STD_LOGIC;
  signal buff0_reg_n_110 : STD_LOGIC;
  signal buff0_reg_n_111 : STD_LOGIC;
  signal buff0_reg_n_112 : STD_LOGIC;
  signal buff0_reg_n_113 : STD_LOGIC;
  signal buff0_reg_n_114 : STD_LOGIC;
  signal buff0_reg_n_115 : STD_LOGIC;
  signal buff0_reg_n_116 : STD_LOGIC;
  signal buff0_reg_n_117 : STD_LOGIC;
  signal buff0_reg_n_118 : STD_LOGIC;
  signal buff0_reg_n_119 : STD_LOGIC;
  signal buff0_reg_n_120 : STD_LOGIC;
  signal buff0_reg_n_121 : STD_LOGIC;
  signal buff0_reg_n_122 : STD_LOGIC;
  signal buff0_reg_n_123 : STD_LOGIC;
  signal buff0_reg_n_124 : STD_LOGIC;
  signal buff0_reg_n_125 : STD_LOGIC;
  signal buff0_reg_n_126 : STD_LOGIC;
  signal buff0_reg_n_127 : STD_LOGIC;
  signal buff0_reg_n_128 : STD_LOGIC;
  signal buff0_reg_n_129 : STD_LOGIC;
  signal buff0_reg_n_130 : STD_LOGIC;
  signal buff0_reg_n_131 : STD_LOGIC;
  signal buff0_reg_n_132 : STD_LOGIC;
  signal buff0_reg_n_133 : STD_LOGIC;
  signal buff0_reg_n_134 : STD_LOGIC;
  signal buff0_reg_n_135 : STD_LOGIC;
  signal buff0_reg_n_136 : STD_LOGIC;
  signal buff0_reg_n_137 : STD_LOGIC;
  signal buff0_reg_n_138 : STD_LOGIC;
  signal buff0_reg_n_139 : STD_LOGIC;
  signal buff0_reg_n_140 : STD_LOGIC;
  signal buff0_reg_n_141 : STD_LOGIC;
  signal buff0_reg_n_142 : STD_LOGIC;
  signal buff0_reg_n_143 : STD_LOGIC;
  signal buff0_reg_n_144 : STD_LOGIC;
  signal buff0_reg_n_145 : STD_LOGIC;
  signal buff0_reg_n_146 : STD_LOGIC;
  signal buff0_reg_n_147 : STD_LOGIC;
  signal buff0_reg_n_148 : STD_LOGIC;
  signal buff0_reg_n_149 : STD_LOGIC;
  signal buff0_reg_n_150 : STD_LOGIC;
  signal buff0_reg_n_151 : STD_LOGIC;
  signal buff0_reg_n_152 : STD_LOGIC;
  signal buff0_reg_n_153 : STD_LOGIC;
  signal buff0_reg_n_154 : STD_LOGIC;
  signal buff0_reg_n_59 : STD_LOGIC;
  signal buff0_reg_n_60 : STD_LOGIC;
  signal buff0_reg_n_61 : STD_LOGIC;
  signal buff0_reg_n_62 : STD_LOGIC;
  signal buff0_reg_n_63 : STD_LOGIC;
  signal buff0_reg_n_64 : STD_LOGIC;
  signal buff0_reg_n_65 : STD_LOGIC;
  signal buff0_reg_n_66 : STD_LOGIC;
  signal buff0_reg_n_67 : STD_LOGIC;
  signal buff0_reg_n_68 : STD_LOGIC;
  signal buff0_reg_n_69 : STD_LOGIC;
  signal buff0_reg_n_70 : STD_LOGIC;
  signal buff0_reg_n_71 : STD_LOGIC;
  signal buff0_reg_n_72 : STD_LOGIC;
  signal buff0_reg_n_73 : STD_LOGIC;
  signal buff0_reg_n_74 : STD_LOGIC;
  signal buff0_reg_n_75 : STD_LOGIC;
  signal buff0_reg_n_76 : STD_LOGIC;
  signal buff0_reg_n_77 : STD_LOGIC;
  signal buff0_reg_n_78 : STD_LOGIC;
  signal buff0_reg_n_79 : STD_LOGIC;
  signal buff0_reg_n_80 : STD_LOGIC;
  signal buff0_reg_n_81 : STD_LOGIC;
  signal buff0_reg_n_82 : STD_LOGIC;
  signal buff0_reg_n_83 : STD_LOGIC;
  signal buff0_reg_n_84 : STD_LOGIC;
  signal buff0_reg_n_85 : STD_LOGIC;
  signal buff0_reg_n_86 : STD_LOGIC;
  signal buff0_reg_n_87 : STD_LOGIC;
  signal buff0_reg_n_88 : STD_LOGIC;
  signal buff0_reg_n_89 : STD_LOGIC;
  signal buff0_reg_n_90 : STD_LOGIC;
  signal buff0_reg_n_91 : STD_LOGIC;
  signal buff0_reg_n_92 : STD_LOGIC;
  signal buff0_reg_n_93 : STD_LOGIC;
  signal buff0_reg_n_94 : STD_LOGIC;
  signal buff0_reg_n_95 : STD_LOGIC;
  signal buff0_reg_n_96 : STD_LOGIC;
  signal buff0_reg_n_97 : STD_LOGIC;
  signal buff0_reg_n_98 : STD_LOGIC;
  signal buff0_reg_n_99 : STD_LOGIC;
  signal \buff1_reg_i_1__4_n_2\ : STD_LOGIC;
  signal \buff1_reg_i_1__4_n_3\ : STD_LOGIC;
  signal \buff1_reg_i_1__4_n_4\ : STD_LOGIC;
  signal \buff1_reg_i_2__4_n_1\ : STD_LOGIC;
  signal \buff1_reg_i_2__4_n_2\ : STD_LOGIC;
  signal \buff1_reg_i_2__4_n_3\ : STD_LOGIC;
  signal \buff1_reg_i_2__4_n_4\ : STD_LOGIC;
  signal \buff1_reg_i_3__4_n_1\ : STD_LOGIC;
  signal \buff1_reg_i_3__4_n_2\ : STD_LOGIC;
  signal \buff1_reg_i_3__4_n_3\ : STD_LOGIC;
  signal \buff1_reg_i_3__4_n_4\ : STD_LOGIC;
  signal buff1_reg_n_107 : STD_LOGIC;
  signal buff1_reg_n_108 : STD_LOGIC;
  signal buff1_reg_n_109 : STD_LOGIC;
  signal buff1_reg_n_110 : STD_LOGIC;
  signal buff1_reg_n_111 : STD_LOGIC;
  signal buff1_reg_n_112 : STD_LOGIC;
  signal buff1_reg_n_113 : STD_LOGIC;
  signal buff1_reg_n_114 : STD_LOGIC;
  signal buff1_reg_n_115 : STD_LOGIC;
  signal buff1_reg_n_116 : STD_LOGIC;
  signal buff1_reg_n_117 : STD_LOGIC;
  signal buff1_reg_n_118 : STD_LOGIC;
  signal buff1_reg_n_119 : STD_LOGIC;
  signal buff1_reg_n_120 : STD_LOGIC;
  signal buff1_reg_n_121 : STD_LOGIC;
  signal buff1_reg_n_122 : STD_LOGIC;
  signal buff1_reg_n_123 : STD_LOGIC;
  signal buff1_reg_n_124 : STD_LOGIC;
  signal buff1_reg_n_125 : STD_LOGIC;
  signal buff1_reg_n_126 : STD_LOGIC;
  signal buff1_reg_n_127 : STD_LOGIC;
  signal buff1_reg_n_128 : STD_LOGIC;
  signal buff1_reg_n_129 : STD_LOGIC;
  signal buff1_reg_n_130 : STD_LOGIC;
  signal buff1_reg_n_131 : STD_LOGIC;
  signal buff1_reg_n_132 : STD_LOGIC;
  signal buff1_reg_n_133 : STD_LOGIC;
  signal buff1_reg_n_134 : STD_LOGIC;
  signal buff1_reg_n_135 : STD_LOGIC;
  signal buff1_reg_n_136 : STD_LOGIC;
  signal buff1_reg_n_137 : STD_LOGIC;
  signal buff1_reg_n_138 : STD_LOGIC;
  signal buff1_reg_n_139 : STD_LOGIC;
  signal buff1_reg_n_140 : STD_LOGIC;
  signal buff1_reg_n_141 : STD_LOGIC;
  signal buff1_reg_n_142 : STD_LOGIC;
  signal buff1_reg_n_143 : STD_LOGIC;
  signal buff1_reg_n_144 : STD_LOGIC;
  signal buff1_reg_n_145 : STD_LOGIC;
  signal buff1_reg_n_146 : STD_LOGIC;
  signal buff1_reg_n_147 : STD_LOGIC;
  signal buff1_reg_n_148 : STD_LOGIC;
  signal buff1_reg_n_149 : STD_LOGIC;
  signal buff1_reg_n_150 : STD_LOGIC;
  signal buff1_reg_n_151 : STD_LOGIC;
  signal buff1_reg_n_152 : STD_LOGIC;
  signal buff1_reg_n_153 : STD_LOGIC;
  signal buff1_reg_n_154 : STD_LOGIC;
  signal \buff1_reg_n_1_[0]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[10]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[11]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[12]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[13]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[14]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[15]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[16]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[1]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[2]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[3]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[4]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[5]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[6]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[7]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[8]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[9]\ : STD_LOGIC;
  signal \buff2_reg__0\ : STD_LOGIC_VECTOR ( 31 downto 0 );
  signal buff2_reg_n_59 : STD_LOGIC;
  signal buff2_reg_n_60 : STD_LOGIC;
  signal buff2_reg_n_61 : STD_LOGIC;
  signal buff2_reg_n_62 : STD_LOGIC;
  signal buff2_reg_n_63 : STD_LOGIC;
  signal buff2_reg_n_64 : STD_LOGIC;
  signal buff2_reg_n_65 : STD_LOGIC;
  signal buff2_reg_n_66 : STD_LOGIC;
  signal buff2_reg_n_67 : STD_LOGIC;
  signal buff2_reg_n_68 : STD_LOGIC;
  signal buff2_reg_n_69 : STD_LOGIC;
  signal buff2_reg_n_70 : STD_LOGIC;
  signal buff2_reg_n_71 : STD_LOGIC;
  signal buff2_reg_n_72 : STD_LOGIC;
  signal buff2_reg_n_73 : STD_LOGIC;
  signal buff2_reg_n_74 : STD_LOGIC;
  signal buff2_reg_n_75 : STD_LOGIC;
  signal buff2_reg_n_76 : STD_LOGIC;
  signal buff2_reg_n_77 : STD_LOGIC;
  signal buff2_reg_n_78 : STD_LOGIC;
  signal buff2_reg_n_79 : STD_LOGIC;
  signal buff2_reg_n_80 : STD_LOGIC;
  signal buff2_reg_n_81 : STD_LOGIC;
  signal buff2_reg_n_82 : STD_LOGIC;
  signal buff2_reg_n_83 : STD_LOGIC;
  signal buff2_reg_n_84 : STD_LOGIC;
  signal buff2_reg_n_85 : STD_LOGIC;
  signal buff2_reg_n_86 : STD_LOGIC;
  signal buff2_reg_n_87 : STD_LOGIC;
  signal buff2_reg_n_88 : STD_LOGIC;
  signal buff2_reg_n_89 : STD_LOGIC;
  signal buff2_reg_n_90 : STD_LOGIC;
  signal buff2_reg_n_91 : STD_LOGIC;
  signal NLW_buff0_reg_CARRYCASCOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_MULTSIGNOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_OVERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_PATTERNBDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_PATTERNDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_UNDERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_ACOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 29 downto 0 );
  signal NLW_buff0_reg_BCOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 17 downto 0 );
  signal NLW_buff0_reg_CARRYOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_buff1_reg_CARRYCASCOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_MULTSIGNOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_OVERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_PATTERNBDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_PATTERNDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_UNDERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_ACOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 29 downto 0 );
  signal NLW_buff1_reg_BCOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 17 downto 0 );
  signal NLW_buff1_reg_CARRYOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_buff1_reg_P_UNCONNECTED : STD_LOGIC_VECTOR ( 47 downto 0 );
  signal \NLW_buff1_reg_i_1__4_CO_UNCONNECTED\ : STD_LOGIC_VECTOR ( 3 to 3 );
  signal NLW_buff2_reg_CARRYCASCOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_MULTSIGNOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_OVERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_PATTERNBDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_PATTERNDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_UNDERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_ACOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 29 downto 0 );
  signal NLW_buff2_reg_BCOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 17 downto 0 );
  signal NLW_buff2_reg_CARRYOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_buff2_reg_PCOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 47 downto 0 );
begin
buff0_reg: unisim.vcomponents.DSP48E1
    generic map(
      ACASCREG => 2,
      ADREG => 1,
      ALUMODEREG => 0,
      AREG => 2,
      AUTORESET_PATDET => "NO_RESET",
      A_INPUT => "DIRECT",
      BCASCREG => 1,
      BREG => 1,
      B_INPUT => "DIRECT",
      CARRYINREG => 0,
      CARRYINSELREG => 0,
      CREG => 0,
      DREG => 1,
      INMODEREG => 0,
      MASK => X"3FFFFFFFFFFF",
      MREG => 0,
      OPMODEREG => 0,
      PATTERN => X"000000000000",
      PREG => 1,
      SEL_MASK => "MASK",
      SEL_PATTERN => "PATTERN",
      USE_DPORT => false,
      USE_MULT => "MULTIPLY",
      USE_PATTERN_DETECT => "NO_PATDET",
      USE_SIMD => "ONE48"
    )
        port map (
      A(29 downto 17) => B"0000000000000",
      A(16 downto 0) => A_q0(16 downto 0),
      ACIN(29 downto 0) => B"000000000000000000000000000000",
      ACOUT(29 downto 0) => NLW_buff0_reg_ACOUT_UNCONNECTED(29 downto 0),
      ALUMODE(3 downto 0) => B"0000",
      B(17) => '0',
      B(16 downto 0) => grp_fu_168_p1(16 downto 0),
      BCIN(17 downto 0) => B"000000000000000000",
      BCOUT(17 downto 0) => NLW_buff0_reg_BCOUT_UNCONNECTED(17 downto 0),
      C(47 downto 0) => B"111111111111111111111111111111111111111111111111",
      CARRYCASCIN => '0',
      CARRYCASCOUT => NLW_buff0_reg_CARRYCASCOUT_UNCONNECTED,
      CARRYIN => '0',
      CARRYINSEL(2 downto 0) => B"000",
      CARRYOUT(3 downto 0) => NLW_buff0_reg_CARRYOUT_UNCONNECTED(3 downto 0),
      CEA1 => Q(0),
      CEA2 => '1',
      CEAD => '0',
      CEALUMODE => '0',
      CEB1 => '0',
      CEB2 => '1',
      CEC => '0',
      CECARRYIN => '0',
      CECTRL => '0',
      CED => '0',
      CEINMODE => '0',
      CEM => '0',
      CEP => '1',
      CLK => ap_clk,
      D(24 downto 0) => B"0000000000000000000000000",
      INMODE(4 downto 0) => B"00000",
      MULTSIGNIN => '0',
      MULTSIGNOUT => NLW_buff0_reg_MULTSIGNOUT_UNCONNECTED,
      OPMODE(6 downto 0) => B"0000101",
      OVERFLOW => NLW_buff0_reg_OVERFLOW_UNCONNECTED,
      P(47) => buff0_reg_n_59,
      P(46) => buff0_reg_n_60,
      P(45) => buff0_reg_n_61,
      P(44) => buff0_reg_n_62,
      P(43) => buff0_reg_n_63,
      P(42) => buff0_reg_n_64,
      P(41) => buff0_reg_n_65,
      P(40) => buff0_reg_n_66,
      P(39) => buff0_reg_n_67,
      P(38) => buff0_reg_n_68,
      P(37) => buff0_reg_n_69,
      P(36) => buff0_reg_n_70,
      P(35) => buff0_reg_n_71,
      P(34) => buff0_reg_n_72,
      P(33) => buff0_reg_n_73,
      P(32) => buff0_reg_n_74,
      P(31) => buff0_reg_n_75,
      P(30) => buff0_reg_n_76,
      P(29) => buff0_reg_n_77,
      P(28) => buff0_reg_n_78,
      P(27) => buff0_reg_n_79,
      P(26) => buff0_reg_n_80,
      P(25) => buff0_reg_n_81,
      P(24) => buff0_reg_n_82,
      P(23) => buff0_reg_n_83,
      P(22) => buff0_reg_n_84,
      P(21) => buff0_reg_n_85,
      P(20) => buff0_reg_n_86,
      P(19) => buff0_reg_n_87,
      P(18) => buff0_reg_n_88,
      P(17) => buff0_reg_n_89,
      P(16) => buff0_reg_n_90,
      P(15) => buff0_reg_n_91,
      P(14) => buff0_reg_n_92,
      P(13) => buff0_reg_n_93,
      P(12) => buff0_reg_n_94,
      P(11) => buff0_reg_n_95,
      P(10) => buff0_reg_n_96,
      P(9) => buff0_reg_n_97,
      P(8) => buff0_reg_n_98,
      P(7) => buff0_reg_n_99,
      P(6) => buff0_reg_n_100,
      P(5) => buff0_reg_n_101,
      P(4) => buff0_reg_n_102,
      P(3) => buff0_reg_n_103,
      P(2) => buff0_reg_n_104,
      P(1) => buff0_reg_n_105,
      P(0) => buff0_reg_n_106,
      PATTERNBDETECT => NLW_buff0_reg_PATTERNBDETECT_UNCONNECTED,
      PATTERNDETECT => NLW_buff0_reg_PATTERNDETECT_UNCONNECTED,
      PCIN(47 downto 0) => B"000000000000000000000000000000000000000000000000",
      PCOUT(47) => buff0_reg_n_107,
      PCOUT(46) => buff0_reg_n_108,
      PCOUT(45) => buff0_reg_n_109,
      PCOUT(44) => buff0_reg_n_110,
      PCOUT(43) => buff0_reg_n_111,
      PCOUT(42) => buff0_reg_n_112,
      PCOUT(41) => buff0_reg_n_113,
      PCOUT(40) => buff0_reg_n_114,
      PCOUT(39) => buff0_reg_n_115,
      PCOUT(38) => buff0_reg_n_116,
      PCOUT(37) => buff0_reg_n_117,
      PCOUT(36) => buff0_reg_n_118,
      PCOUT(35) => buff0_reg_n_119,
      PCOUT(34) => buff0_reg_n_120,
      PCOUT(33) => buff0_reg_n_121,
      PCOUT(32) => buff0_reg_n_122,
      PCOUT(31) => buff0_reg_n_123,
      PCOUT(30) => buff0_reg_n_124,
      PCOUT(29) => buff0_reg_n_125,
      PCOUT(28) => buff0_reg_n_126,
      PCOUT(27) => buff0_reg_n_127,
      PCOUT(26) => buff0_reg_n_128,
      PCOUT(25) => buff0_reg_n_129,
      PCOUT(24) => buff0_reg_n_130,
      PCOUT(23) => buff0_reg_n_131,
      PCOUT(22) => buff0_reg_n_132,
      PCOUT(21) => buff0_reg_n_133,
      PCOUT(20) => buff0_reg_n_134,
      PCOUT(19) => buff0_reg_n_135,
      PCOUT(18) => buff0_reg_n_136,
      PCOUT(17) => buff0_reg_n_137,
      PCOUT(16) => buff0_reg_n_138,
      PCOUT(15) => buff0_reg_n_139,
      PCOUT(14) => buff0_reg_n_140,
      PCOUT(13) => buff0_reg_n_141,
      PCOUT(12) => buff0_reg_n_142,
      PCOUT(11) => buff0_reg_n_143,
      PCOUT(10) => buff0_reg_n_144,
      PCOUT(9) => buff0_reg_n_145,
      PCOUT(8) => buff0_reg_n_146,
      PCOUT(7) => buff0_reg_n_147,
      PCOUT(6) => buff0_reg_n_148,
      PCOUT(5) => buff0_reg_n_149,
      PCOUT(4) => buff0_reg_n_150,
      PCOUT(3) => buff0_reg_n_151,
      PCOUT(2) => buff0_reg_n_152,
      PCOUT(1) => buff0_reg_n_153,
      PCOUT(0) => buff0_reg_n_154,
      RSTA => '0',
      RSTALLCARRYIN => '0',
      RSTALUMODE => '0',
      RSTB => '0',
      RSTC => '0',
      RSTCTRL => '0',
      RSTD => '0',
      RSTINMODE => '0',
      RSTM => '0',
      RSTP => '0',
      UNDERFLOW => NLW_buff0_reg_UNDERFLOW_UNCONNECTED
    );
\buff0_reg_i_1__4\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff0_reg_i_2__4_n_1\,
      CO(3) => \buff0_reg_i_1__4_n_1\,
      CO(2) => \buff0_reg_i_1__4_n_2\,
      CO(1) => \buff0_reg_i_1__4_n_3\,
      CO(0) => \buff0_reg_i_1__4_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_179_p1(19 downto 16),
      S(3 downto 0) => \buff2_reg__0\(19 downto 16)
    );
\buff0_reg_i_2__4\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff0_reg_i_3__4_n_1\,
      CO(3) => \buff0_reg_i_2__4_n_1\,
      CO(2) => \buff0_reg_i_2__4_n_2\,
      CO(1) => \buff0_reg_i_2__4_n_3\,
      CO(0) => \buff0_reg_i_2__4_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_179_p1(15 downto 12),
      S(3 downto 0) => \buff2_reg__0\(15 downto 12)
    );
\buff0_reg_i_3__4\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff0_reg_i_4__4_n_1\,
      CO(3) => \buff0_reg_i_3__4_n_1\,
      CO(2) => \buff0_reg_i_3__4_n_2\,
      CO(1) => \buff0_reg_i_3__4_n_3\,
      CO(0) => \buff0_reg_i_3__4_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_179_p1(11 downto 8),
      S(3 downto 0) => \buff2_reg__0\(11 downto 8)
    );
\buff0_reg_i_4__4\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff0_reg_i_5__4_n_1\,
      CO(3) => \buff0_reg_i_4__4_n_1\,
      CO(2) => \buff0_reg_i_4__4_n_2\,
      CO(1) => \buff0_reg_i_4__4_n_3\,
      CO(0) => \buff0_reg_i_4__4_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_179_p1(7 downto 4),
      S(3 downto 0) => \buff2_reg__0\(7 downto 4)
    );
\buff0_reg_i_5__4\: unisim.vcomponents.CARRY4
     port map (
      CI => '0',
      CO(3) => \buff0_reg_i_5__4_n_1\,
      CO(2) => \buff0_reg_i_5__4_n_2\,
      CO(1) => \buff0_reg_i_5__4_n_3\,
      CO(0) => \buff0_reg_i_5__4_n_4\,
      CYINIT => '0',
      DI(3 downto 2) => B"00",
      DI(1) => \buff2_reg__0\(1),
      DI(0) => '0',
      O(3 downto 0) => grp_fu_179_p1(3 downto 0),
      S(3 downto 2) => \buff2_reg__0\(3 downto 2),
      S(1) => \buff0_reg_i_6__4_n_1\,
      S(0) => \buff2_reg__0\(0)
    );
\buff0_reg_i_6__4\: unisim.vcomponents.LUT1
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => \buff2_reg__0\(1),
      O => \buff0_reg_i_6__4_n_1\
    );
buff1_reg: unisim.vcomponents.DSP48E1
    generic map(
      ACASCREG => 2,
      ADREG => 1,
      ALUMODEREG => 0,
      AREG => 2,
      AUTORESET_PATDET => "NO_RESET",
      A_INPUT => "DIRECT",
      BCASCREG => 1,
      BREG => 1,
      B_INPUT => "DIRECT",
      CARRYINREG => 0,
      CARRYINSELREG => 0,
      CREG => 1,
      DREG => 1,
      INMODEREG => 0,
      MASK => X"3FFFFFFFFFFF",
      MREG => 1,
      OPMODEREG => 0,
      PATTERN => X"000000000000",
      PREG => 1,
      SEL_MASK => "MASK",
      SEL_PATTERN => "PATTERN",
      USE_DPORT => false,
      USE_MULT => "MULTIPLY",
      USE_PATTERN_DETECT => "NO_PATDET",
      USE_SIMD => "ONE48"
    )
        port map (
      A(29 downto 17) => B"0000000000000",
      A(16 downto 0) => A_q0(16 downto 0),
      ACIN(29 downto 0) => B"000000000000000000000000000000",
      ACOUT(29 downto 0) => NLW_buff1_reg_ACOUT_UNCONNECTED(29 downto 0),
      ALUMODE(3 downto 0) => B"0000",
      B(17) => grp_fu_168_p1(31),
      B(16) => grp_fu_168_p1(31),
      B(15) => grp_fu_168_p1(31),
      B(14 downto 0) => grp_fu_168_p1(31 downto 17),
      BCIN(17 downto 0) => B"000000000000000000",
      BCOUT(17 downto 0) => NLW_buff1_reg_BCOUT_UNCONNECTED(17 downto 0),
      C(47 downto 0) => B"111111111111111111111111111111111111111111111111",
      CARRYCASCIN => '0',
      CARRYCASCOUT => NLW_buff1_reg_CARRYCASCOUT_UNCONNECTED,
      CARRYIN => '0',
      CARRYINSEL(2 downto 0) => B"000",
      CARRYOUT(3 downto 0) => NLW_buff1_reg_CARRYOUT_UNCONNECTED(3 downto 0),
      CEA1 => Q(0),
      CEA2 => '1',
      CEAD => '0',
      CEALUMODE => '0',
      CEB1 => '0',
      CEB2 => '1',
      CEC => '0',
      CECARRYIN => '0',
      CECTRL => '0',
      CED => '0',
      CEINMODE => '0',
      CEM => '1',
      CEP => '1',
      CLK => ap_clk,
      D(24 downto 0) => B"0000000000000000000000000",
      INMODE(4 downto 0) => B"00000",
      MULTSIGNIN => '0',
      MULTSIGNOUT => NLW_buff1_reg_MULTSIGNOUT_UNCONNECTED,
      OPMODE(6 downto 0) => B"1010101",
      OVERFLOW => NLW_buff1_reg_OVERFLOW_UNCONNECTED,
      P(47 downto 0) => NLW_buff1_reg_P_UNCONNECTED(47 downto 0),
      PATTERNBDETECT => NLW_buff1_reg_PATTERNBDETECT_UNCONNECTED,
      PATTERNDETECT => NLW_buff1_reg_PATTERNDETECT_UNCONNECTED,
      PCIN(47) => buff0_reg_n_107,
      PCIN(46) => buff0_reg_n_108,
      PCIN(45) => buff0_reg_n_109,
      PCIN(44) => buff0_reg_n_110,
      PCIN(43) => buff0_reg_n_111,
      PCIN(42) => buff0_reg_n_112,
      PCIN(41) => buff0_reg_n_113,
      PCIN(40) => buff0_reg_n_114,
      PCIN(39) => buff0_reg_n_115,
      PCIN(38) => buff0_reg_n_116,
      PCIN(37) => buff0_reg_n_117,
      PCIN(36) => buff0_reg_n_118,
      PCIN(35) => buff0_reg_n_119,
      PCIN(34) => buff0_reg_n_120,
      PCIN(33) => buff0_reg_n_121,
      PCIN(32) => buff0_reg_n_122,
      PCIN(31) => buff0_reg_n_123,
      PCIN(30) => buff0_reg_n_124,
      PCIN(29) => buff0_reg_n_125,
      PCIN(28) => buff0_reg_n_126,
      PCIN(27) => buff0_reg_n_127,
      PCIN(26) => buff0_reg_n_128,
      PCIN(25) => buff0_reg_n_129,
      PCIN(24) => buff0_reg_n_130,
      PCIN(23) => buff0_reg_n_131,
      PCIN(22) => buff0_reg_n_132,
      PCIN(21) => buff0_reg_n_133,
      PCIN(20) => buff0_reg_n_134,
      PCIN(19) => buff0_reg_n_135,
      PCIN(18) => buff0_reg_n_136,
      PCIN(17) => buff0_reg_n_137,
      PCIN(16) => buff0_reg_n_138,
      PCIN(15) => buff0_reg_n_139,
      PCIN(14) => buff0_reg_n_140,
      PCIN(13) => buff0_reg_n_141,
      PCIN(12) => buff0_reg_n_142,
      PCIN(11) => buff0_reg_n_143,
      PCIN(10) => buff0_reg_n_144,
      PCIN(9) => buff0_reg_n_145,
      PCIN(8) => buff0_reg_n_146,
      PCIN(7) => buff0_reg_n_147,
      PCIN(6) => buff0_reg_n_148,
      PCIN(5) => buff0_reg_n_149,
      PCIN(4) => buff0_reg_n_150,
      PCIN(3) => buff0_reg_n_151,
      PCIN(2) => buff0_reg_n_152,
      PCIN(1) => buff0_reg_n_153,
      PCIN(0) => buff0_reg_n_154,
      PCOUT(47) => buff1_reg_n_107,
      PCOUT(46) => buff1_reg_n_108,
      PCOUT(45) => buff1_reg_n_109,
      PCOUT(44) => buff1_reg_n_110,
      PCOUT(43) => buff1_reg_n_111,
      PCOUT(42) => buff1_reg_n_112,
      PCOUT(41) => buff1_reg_n_113,
      PCOUT(40) => buff1_reg_n_114,
      PCOUT(39) => buff1_reg_n_115,
      PCOUT(38) => buff1_reg_n_116,
      PCOUT(37) => buff1_reg_n_117,
      PCOUT(36) => buff1_reg_n_118,
      PCOUT(35) => buff1_reg_n_119,
      PCOUT(34) => buff1_reg_n_120,
      PCOUT(33) => buff1_reg_n_121,
      PCOUT(32) => buff1_reg_n_122,
      PCOUT(31) => buff1_reg_n_123,
      PCOUT(30) => buff1_reg_n_124,
      PCOUT(29) => buff1_reg_n_125,
      PCOUT(28) => buff1_reg_n_126,
      PCOUT(27) => buff1_reg_n_127,
      PCOUT(26) => buff1_reg_n_128,
      PCOUT(25) => buff1_reg_n_129,
      PCOUT(24) => buff1_reg_n_130,
      PCOUT(23) => buff1_reg_n_131,
      PCOUT(22) => buff1_reg_n_132,
      PCOUT(21) => buff1_reg_n_133,
      PCOUT(20) => buff1_reg_n_134,
      PCOUT(19) => buff1_reg_n_135,
      PCOUT(18) => buff1_reg_n_136,
      PCOUT(17) => buff1_reg_n_137,
      PCOUT(16) => buff1_reg_n_138,
      PCOUT(15) => buff1_reg_n_139,
      PCOUT(14) => buff1_reg_n_140,
      PCOUT(13) => buff1_reg_n_141,
      PCOUT(12) => buff1_reg_n_142,
      PCOUT(11) => buff1_reg_n_143,
      PCOUT(10) => buff1_reg_n_144,
      PCOUT(9) => buff1_reg_n_145,
      PCOUT(8) => buff1_reg_n_146,
      PCOUT(7) => buff1_reg_n_147,
      PCOUT(6) => buff1_reg_n_148,
      PCOUT(5) => buff1_reg_n_149,
      PCOUT(4) => buff1_reg_n_150,
      PCOUT(3) => buff1_reg_n_151,
      PCOUT(2) => buff1_reg_n_152,
      PCOUT(1) => buff1_reg_n_153,
      PCOUT(0) => buff1_reg_n_154,
      RSTA => '0',
      RSTALLCARRYIN => '0',
      RSTALUMODE => '0',
      RSTB => '0',
      RSTC => '0',
      RSTCTRL => '0',
      RSTD => '0',
      RSTINMODE => '0',
      RSTM => '0',
      RSTP => '0',
      UNDERFLOW => NLW_buff1_reg_UNDERFLOW_UNCONNECTED
    );
\buff1_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_106,
      Q => \buff1_reg_n_1_[0]\,
      R => '0'
    );
\buff1_reg[10]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_96,
      Q => \buff1_reg_n_1_[10]\,
      R => '0'
    );
\buff1_reg[11]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_95,
      Q => \buff1_reg_n_1_[11]\,
      R => '0'
    );
\buff1_reg[12]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_94,
      Q => \buff1_reg_n_1_[12]\,
      R => '0'
    );
\buff1_reg[13]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_93,
      Q => \buff1_reg_n_1_[13]\,
      R => '0'
    );
\buff1_reg[14]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_92,
      Q => \buff1_reg_n_1_[14]\,
      R => '0'
    );
\buff1_reg[15]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_91,
      Q => \buff1_reg_n_1_[15]\,
      R => '0'
    );
\buff1_reg[16]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_90,
      Q => \buff1_reg_n_1_[16]\,
      R => '0'
    );
\buff1_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_105,
      Q => \buff1_reg_n_1_[1]\,
      R => '0'
    );
\buff1_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_104,
      Q => \buff1_reg_n_1_[2]\,
      R => '0'
    );
\buff1_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_103,
      Q => \buff1_reg_n_1_[3]\,
      R => '0'
    );
\buff1_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_102,
      Q => \buff1_reg_n_1_[4]\,
      R => '0'
    );
\buff1_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_101,
      Q => \buff1_reg_n_1_[5]\,
      R => '0'
    );
\buff1_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_100,
      Q => \buff1_reg_n_1_[6]\,
      R => '0'
    );
\buff1_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_99,
      Q => \buff1_reg_n_1_[7]\,
      R => '0'
    );
\buff1_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_98,
      Q => \buff1_reg_n_1_[8]\,
      R => '0'
    );
\buff1_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_97,
      Q => \buff1_reg_n_1_[9]\,
      R => '0'
    );
\buff1_reg_i_1__4\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff1_reg_i_2__4_n_1\,
      CO(3) => \NLW_buff1_reg_i_1__4_CO_UNCONNECTED\(3),
      CO(2) => \buff1_reg_i_1__4_n_2\,
      CO(1) => \buff1_reg_i_1__4_n_3\,
      CO(0) => \buff1_reg_i_1__4_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_179_p1(31 downto 28),
      S(3 downto 0) => \buff2_reg__0\(31 downto 28)
    );
\buff1_reg_i_2__4\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff1_reg_i_3__4_n_1\,
      CO(3) => \buff1_reg_i_2__4_n_1\,
      CO(2) => \buff1_reg_i_2__4_n_2\,
      CO(1) => \buff1_reg_i_2__4_n_3\,
      CO(0) => \buff1_reg_i_2__4_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_179_p1(27 downto 24),
      S(3 downto 0) => \buff2_reg__0\(27 downto 24)
    );
\buff1_reg_i_3__4\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff0_reg_i_1__4_n_1\,
      CO(3) => \buff1_reg_i_3__4_n_1\,
      CO(2) => \buff1_reg_i_3__4_n_2\,
      CO(1) => \buff1_reg_i_3__4_n_3\,
      CO(0) => \buff1_reg_i_3__4_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_179_p1(23 downto 20),
      S(3 downto 0) => \buff2_reg__0\(23 downto 20)
    );
buff2_reg: unisim.vcomponents.DSP48E1
    generic map(
      ACASCREG => 2,
      ADREG => 1,
      ALUMODEREG => 0,
      AREG => 2,
      AUTORESET_PATDET => "NO_RESET",
      A_INPUT => "DIRECT",
      BCASCREG => 2,
      BREG => 2,
      B_INPUT => "DIRECT",
      CARRYINREG => 0,
      CARRYINSELREG => 0,
      CREG => 1,
      DREG => 1,
      INMODEREG => 0,
      MASK => X"3FFFFFFFFFFF",
      MREG => 1,
      OPMODEREG => 0,
      PATTERN => X"000000000000",
      PREG => 1,
      SEL_MASK => "MASK",
      SEL_PATTERN => "PATTERN",
      USE_DPORT => false,
      USE_MULT => "MULTIPLY",
      USE_PATTERN_DETECT => "NO_PATDET",
      USE_SIMD => "ONE48"
    )
        port map (
      A(29 downto 17) => B"0000000000000",
      A(16 downto 0) => grp_fu_168_p1(16 downto 0),
      ACIN(29 downto 0) => B"000000000000000000000000000000",
      ACOUT(29 downto 0) => NLW_buff2_reg_ACOUT_UNCONNECTED(29 downto 0),
      ALUMODE(3 downto 0) => B"0000",
      B(17) => buff2_reg_0(14),
      B(16) => buff2_reg_0(14),
      B(15) => buff2_reg_0(14),
      B(14 downto 0) => buff2_reg_0(14 downto 0),
      BCIN(17 downto 0) => B"000000000000000000",
      BCOUT(17 downto 0) => NLW_buff2_reg_BCOUT_UNCONNECTED(17 downto 0),
      C(47 downto 0) => B"111111111111111111111111111111111111111111111111",
      CARRYCASCIN => '0',
      CARRYCASCOUT => NLW_buff2_reg_CARRYCASCOUT_UNCONNECTED,
      CARRYIN => '0',
      CARRYINSEL(2 downto 0) => B"000",
      CARRYOUT(3 downto 0) => NLW_buff2_reg_CARRYOUT_UNCONNECTED(3 downto 0),
      CEA1 => '1',
      CEA2 => '1',
      CEAD => '0',
      CEALUMODE => '0',
      CEB1 => '1',
      CEB2 => '1',
      CEC => '0',
      CECARRYIN => '0',
      CECTRL => '0',
      CED => '0',
      CEINMODE => '0',
      CEM => '1',
      CEP => '1',
      CLK => ap_clk,
      D(24 downto 0) => B"0000000000000000000000000",
      INMODE(4 downto 0) => B"00000",
      MULTSIGNIN => '0',
      MULTSIGNOUT => NLW_buff2_reg_MULTSIGNOUT_UNCONNECTED,
      OPMODE(6 downto 0) => B"0010101",
      OVERFLOW => NLW_buff2_reg_OVERFLOW_UNCONNECTED,
      P(47) => buff2_reg_n_59,
      P(46) => buff2_reg_n_60,
      P(45) => buff2_reg_n_61,
      P(44) => buff2_reg_n_62,
      P(43) => buff2_reg_n_63,
      P(42) => buff2_reg_n_64,
      P(41) => buff2_reg_n_65,
      P(40) => buff2_reg_n_66,
      P(39) => buff2_reg_n_67,
      P(38) => buff2_reg_n_68,
      P(37) => buff2_reg_n_69,
      P(36) => buff2_reg_n_70,
      P(35) => buff2_reg_n_71,
      P(34) => buff2_reg_n_72,
      P(33) => buff2_reg_n_73,
      P(32) => buff2_reg_n_74,
      P(31) => buff2_reg_n_75,
      P(30) => buff2_reg_n_76,
      P(29) => buff2_reg_n_77,
      P(28) => buff2_reg_n_78,
      P(27) => buff2_reg_n_79,
      P(26) => buff2_reg_n_80,
      P(25) => buff2_reg_n_81,
      P(24) => buff2_reg_n_82,
      P(23) => buff2_reg_n_83,
      P(22) => buff2_reg_n_84,
      P(21) => buff2_reg_n_85,
      P(20) => buff2_reg_n_86,
      P(19) => buff2_reg_n_87,
      P(18) => buff2_reg_n_88,
      P(17) => buff2_reg_n_89,
      P(16) => buff2_reg_n_90,
      P(15) => buff2_reg_n_91,
      P(14 downto 0) => \buff2_reg__0\(31 downto 17),
      PATTERNBDETECT => NLW_buff2_reg_PATTERNBDETECT_UNCONNECTED,
      PATTERNDETECT => NLW_buff2_reg_PATTERNDETECT_UNCONNECTED,
      PCIN(47) => buff1_reg_n_107,
      PCIN(46) => buff1_reg_n_108,
      PCIN(45) => buff1_reg_n_109,
      PCIN(44) => buff1_reg_n_110,
      PCIN(43) => buff1_reg_n_111,
      PCIN(42) => buff1_reg_n_112,
      PCIN(41) => buff1_reg_n_113,
      PCIN(40) => buff1_reg_n_114,
      PCIN(39) => buff1_reg_n_115,
      PCIN(38) => buff1_reg_n_116,
      PCIN(37) => buff1_reg_n_117,
      PCIN(36) => buff1_reg_n_118,
      PCIN(35) => buff1_reg_n_119,
      PCIN(34) => buff1_reg_n_120,
      PCIN(33) => buff1_reg_n_121,
      PCIN(32) => buff1_reg_n_122,
      PCIN(31) => buff1_reg_n_123,
      PCIN(30) => buff1_reg_n_124,
      PCIN(29) => buff1_reg_n_125,
      PCIN(28) => buff1_reg_n_126,
      PCIN(27) => buff1_reg_n_127,
      PCIN(26) => buff1_reg_n_128,
      PCIN(25) => buff1_reg_n_129,
      PCIN(24) => buff1_reg_n_130,
      PCIN(23) => buff1_reg_n_131,
      PCIN(22) => buff1_reg_n_132,
      PCIN(21) => buff1_reg_n_133,
      PCIN(20) => buff1_reg_n_134,
      PCIN(19) => buff1_reg_n_135,
      PCIN(18) => buff1_reg_n_136,
      PCIN(17) => buff1_reg_n_137,
      PCIN(16) => buff1_reg_n_138,
      PCIN(15) => buff1_reg_n_139,
      PCIN(14) => buff1_reg_n_140,
      PCIN(13) => buff1_reg_n_141,
      PCIN(12) => buff1_reg_n_142,
      PCIN(11) => buff1_reg_n_143,
      PCIN(10) => buff1_reg_n_144,
      PCIN(9) => buff1_reg_n_145,
      PCIN(8) => buff1_reg_n_146,
      PCIN(7) => buff1_reg_n_147,
      PCIN(6) => buff1_reg_n_148,
      PCIN(5) => buff1_reg_n_149,
      PCIN(4) => buff1_reg_n_150,
      PCIN(3) => buff1_reg_n_151,
      PCIN(2) => buff1_reg_n_152,
      PCIN(1) => buff1_reg_n_153,
      PCIN(0) => buff1_reg_n_154,
      PCOUT(47 downto 0) => NLW_buff2_reg_PCOUT_UNCONNECTED(47 downto 0),
      RSTA => '0',
      RSTALLCARRYIN => '0',
      RSTALUMODE => '0',
      RSTB => '0',
      RSTC => '0',
      RSTCTRL => '0',
      RSTD => '0',
      RSTINMODE => '0',
      RSTM => '0',
      RSTP => '0',
      UNDERFLOW => NLW_buff2_reg_UNDERFLOW_UNCONNECTED
    );
\buff2_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[0]\,
      Q => \buff2_reg__0\(0),
      R => '0'
    );
\buff2_reg[10]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[10]\,
      Q => \buff2_reg__0\(10),
      R => '0'
    );
\buff2_reg[11]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[11]\,
      Q => \buff2_reg__0\(11),
      R => '0'
    );
\buff2_reg[12]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[12]\,
      Q => \buff2_reg__0\(12),
      R => '0'
    );
\buff2_reg[13]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[13]\,
      Q => \buff2_reg__0\(13),
      R => '0'
    );
\buff2_reg[14]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[14]\,
      Q => \buff2_reg__0\(14),
      R => '0'
    );
\buff2_reg[15]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[15]\,
      Q => \buff2_reg__0\(15),
      R => '0'
    );
\buff2_reg[16]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[16]\,
      Q => \buff2_reg__0\(16),
      R => '0'
    );
\buff2_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[1]\,
      Q => \buff2_reg__0\(1),
      R => '0'
    );
\buff2_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[2]\,
      Q => \buff2_reg__0\(2),
      R => '0'
    );
\buff2_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[3]\,
      Q => \buff2_reg__0\(3),
      R => '0'
    );
\buff2_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[4]\,
      Q => \buff2_reg__0\(4),
      R => '0'
    );
\buff2_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[5]\,
      Q => \buff2_reg__0\(5),
      R => '0'
    );
\buff2_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[6]\,
      Q => \buff2_reg__0\(6),
      R => '0'
    );
\buff2_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[7]\,
      Q => \buff2_reg__0\(7),
      R => '0'
    );
\buff2_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[8]\,
      Q => \buff2_reg__0\(8),
      R => '0'
    );
\buff2_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[9]\,
      Q => \buff2_reg__0\(9),
      R => '0'
    );
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_MulnS_0_8 is
  port (
    grp_fu_168_p1 : out STD_LOGIC_VECTOR ( 31 downto 0 );
    Q : in STD_LOGIC_VECTOR ( 0 to 0 );
    ap_clk : in STD_LOGIC;
    grp_fu_157_p1 : in STD_LOGIC_VECTOR ( 31 downto 0 );
    A_q0 : in STD_LOGIC_VECTOR ( 16 downto 0 );
    buff2_reg_0 : in STD_LOGIC_VECTOR ( 14 downto 0 )
  );
  attribute ORIG_REF_NAME : string;
  attribute ORIG_REF_NAME of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_MulnS_0_8 : entity is "loop_imperfect_mubkb_MulnS_0";
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_MulnS_0_8;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_MulnS_0_8 is
  signal buff0_reg_i_10_n_1 : STD_LOGIC;
  signal buff0_reg_i_11_n_1 : STD_LOGIC;
  signal \buff0_reg_i_1__3_n_1\ : STD_LOGIC;
  signal \buff0_reg_i_1__3_n_2\ : STD_LOGIC;
  signal \buff0_reg_i_1__3_n_3\ : STD_LOGIC;
  signal \buff0_reg_i_1__3_n_4\ : STD_LOGIC;
  signal \buff0_reg_i_2__3_n_1\ : STD_LOGIC;
  signal \buff0_reg_i_2__3_n_2\ : STD_LOGIC;
  signal \buff0_reg_i_2__3_n_3\ : STD_LOGIC;
  signal \buff0_reg_i_2__3_n_4\ : STD_LOGIC;
  signal \buff0_reg_i_3__3_n_1\ : STD_LOGIC;
  signal \buff0_reg_i_3__3_n_2\ : STD_LOGIC;
  signal \buff0_reg_i_3__3_n_3\ : STD_LOGIC;
  signal \buff0_reg_i_3__3_n_4\ : STD_LOGIC;
  signal \buff0_reg_i_4__3_n_1\ : STD_LOGIC;
  signal \buff0_reg_i_4__3_n_2\ : STD_LOGIC;
  signal \buff0_reg_i_4__3_n_3\ : STD_LOGIC;
  signal \buff0_reg_i_4__3_n_4\ : STD_LOGIC;
  signal \buff0_reg_i_6__3_n_1\ : STD_LOGIC;
  signal \buff0_reg_i_7__2_n_1\ : STD_LOGIC;
  signal \buff0_reg_i_8__1_n_1\ : STD_LOGIC;
  signal buff0_reg_i_9_n_1 : STD_LOGIC;
  signal buff0_reg_n_100 : STD_LOGIC;
  signal buff0_reg_n_101 : STD_LOGIC;
  signal buff0_reg_n_102 : STD_LOGIC;
  signal buff0_reg_n_103 : STD_LOGIC;
  signal buff0_reg_n_104 : STD_LOGIC;
  signal buff0_reg_n_105 : STD_LOGIC;
  signal buff0_reg_n_106 : STD_LOGIC;
  signal buff0_reg_n_107 : STD_LOGIC;
  signal buff0_reg_n_108 : STD_LOGIC;
  signal buff0_reg_n_109 : STD_LOGIC;
  signal buff0_reg_n_110 : STD_LOGIC;
  signal buff0_reg_n_111 : STD_LOGIC;
  signal buff0_reg_n_112 : STD_LOGIC;
  signal buff0_reg_n_113 : STD_LOGIC;
  signal buff0_reg_n_114 : STD_LOGIC;
  signal buff0_reg_n_115 : STD_LOGIC;
  signal buff0_reg_n_116 : STD_LOGIC;
  signal buff0_reg_n_117 : STD_LOGIC;
  signal buff0_reg_n_118 : STD_LOGIC;
  signal buff0_reg_n_119 : STD_LOGIC;
  signal buff0_reg_n_120 : STD_LOGIC;
  signal buff0_reg_n_121 : STD_LOGIC;
  signal buff0_reg_n_122 : STD_LOGIC;
  signal buff0_reg_n_123 : STD_LOGIC;
  signal buff0_reg_n_124 : STD_LOGIC;
  signal buff0_reg_n_125 : STD_LOGIC;
  signal buff0_reg_n_126 : STD_LOGIC;
  signal buff0_reg_n_127 : STD_LOGIC;
  signal buff0_reg_n_128 : STD_LOGIC;
  signal buff0_reg_n_129 : STD_LOGIC;
  signal buff0_reg_n_130 : STD_LOGIC;
  signal buff0_reg_n_131 : STD_LOGIC;
  signal buff0_reg_n_132 : STD_LOGIC;
  signal buff0_reg_n_133 : STD_LOGIC;
  signal buff0_reg_n_134 : STD_LOGIC;
  signal buff0_reg_n_135 : STD_LOGIC;
  signal buff0_reg_n_136 : STD_LOGIC;
  signal buff0_reg_n_137 : STD_LOGIC;
  signal buff0_reg_n_138 : STD_LOGIC;
  signal buff0_reg_n_139 : STD_LOGIC;
  signal buff0_reg_n_140 : STD_LOGIC;
  signal buff0_reg_n_141 : STD_LOGIC;
  signal buff0_reg_n_142 : STD_LOGIC;
  signal buff0_reg_n_143 : STD_LOGIC;
  signal buff0_reg_n_144 : STD_LOGIC;
  signal buff0_reg_n_145 : STD_LOGIC;
  signal buff0_reg_n_146 : STD_LOGIC;
  signal buff0_reg_n_147 : STD_LOGIC;
  signal buff0_reg_n_148 : STD_LOGIC;
  signal buff0_reg_n_149 : STD_LOGIC;
  signal buff0_reg_n_150 : STD_LOGIC;
  signal buff0_reg_n_151 : STD_LOGIC;
  signal buff0_reg_n_152 : STD_LOGIC;
  signal buff0_reg_n_153 : STD_LOGIC;
  signal buff0_reg_n_154 : STD_LOGIC;
  signal buff0_reg_n_59 : STD_LOGIC;
  signal buff0_reg_n_60 : STD_LOGIC;
  signal buff0_reg_n_61 : STD_LOGIC;
  signal buff0_reg_n_62 : STD_LOGIC;
  signal buff0_reg_n_63 : STD_LOGIC;
  signal buff0_reg_n_64 : STD_LOGIC;
  signal buff0_reg_n_65 : STD_LOGIC;
  signal buff0_reg_n_66 : STD_LOGIC;
  signal buff0_reg_n_67 : STD_LOGIC;
  signal buff0_reg_n_68 : STD_LOGIC;
  signal buff0_reg_n_69 : STD_LOGIC;
  signal buff0_reg_n_70 : STD_LOGIC;
  signal buff0_reg_n_71 : STD_LOGIC;
  signal buff0_reg_n_72 : STD_LOGIC;
  signal buff0_reg_n_73 : STD_LOGIC;
  signal buff0_reg_n_74 : STD_LOGIC;
  signal buff0_reg_n_75 : STD_LOGIC;
  signal buff0_reg_n_76 : STD_LOGIC;
  signal buff0_reg_n_77 : STD_LOGIC;
  signal buff0_reg_n_78 : STD_LOGIC;
  signal buff0_reg_n_79 : STD_LOGIC;
  signal buff0_reg_n_80 : STD_LOGIC;
  signal buff0_reg_n_81 : STD_LOGIC;
  signal buff0_reg_n_82 : STD_LOGIC;
  signal buff0_reg_n_83 : STD_LOGIC;
  signal buff0_reg_n_84 : STD_LOGIC;
  signal buff0_reg_n_85 : STD_LOGIC;
  signal buff0_reg_n_86 : STD_LOGIC;
  signal buff0_reg_n_87 : STD_LOGIC;
  signal buff0_reg_n_88 : STD_LOGIC;
  signal buff0_reg_n_89 : STD_LOGIC;
  signal buff0_reg_n_90 : STD_LOGIC;
  signal buff0_reg_n_91 : STD_LOGIC;
  signal buff0_reg_n_92 : STD_LOGIC;
  signal buff0_reg_n_93 : STD_LOGIC;
  signal buff0_reg_n_94 : STD_LOGIC;
  signal buff0_reg_n_95 : STD_LOGIC;
  signal buff0_reg_n_96 : STD_LOGIC;
  signal buff0_reg_n_97 : STD_LOGIC;
  signal buff0_reg_n_98 : STD_LOGIC;
  signal buff0_reg_n_99 : STD_LOGIC;
  signal \buff1_reg_i_1__3_n_3\ : STD_LOGIC;
  signal \buff1_reg_i_1__3_n_4\ : STD_LOGIC;
  signal \buff1_reg_i_2__3_n_1\ : STD_LOGIC;
  signal \buff1_reg_i_2__3_n_2\ : STD_LOGIC;
  signal \buff1_reg_i_2__3_n_3\ : STD_LOGIC;
  signal \buff1_reg_i_2__3_n_4\ : STD_LOGIC;
  signal \buff1_reg_i_3__3_n_1\ : STD_LOGIC;
  signal \buff1_reg_i_3__3_n_2\ : STD_LOGIC;
  signal \buff1_reg_i_3__3_n_3\ : STD_LOGIC;
  signal \buff1_reg_i_3__3_n_4\ : STD_LOGIC;
  signal \buff1_reg_i_4__2_n_1\ : STD_LOGIC;
  signal \buff1_reg_i_4__2_n_2\ : STD_LOGIC;
  signal \buff1_reg_i_4__2_n_3\ : STD_LOGIC;
  signal \buff1_reg_i_4__2_n_4\ : STD_LOGIC;
  signal buff1_reg_n_107 : STD_LOGIC;
  signal buff1_reg_n_108 : STD_LOGIC;
  signal buff1_reg_n_109 : STD_LOGIC;
  signal buff1_reg_n_110 : STD_LOGIC;
  signal buff1_reg_n_111 : STD_LOGIC;
  signal buff1_reg_n_112 : STD_LOGIC;
  signal buff1_reg_n_113 : STD_LOGIC;
  signal buff1_reg_n_114 : STD_LOGIC;
  signal buff1_reg_n_115 : STD_LOGIC;
  signal buff1_reg_n_116 : STD_LOGIC;
  signal buff1_reg_n_117 : STD_LOGIC;
  signal buff1_reg_n_118 : STD_LOGIC;
  signal buff1_reg_n_119 : STD_LOGIC;
  signal buff1_reg_n_120 : STD_LOGIC;
  signal buff1_reg_n_121 : STD_LOGIC;
  signal buff1_reg_n_122 : STD_LOGIC;
  signal buff1_reg_n_123 : STD_LOGIC;
  signal buff1_reg_n_124 : STD_LOGIC;
  signal buff1_reg_n_125 : STD_LOGIC;
  signal buff1_reg_n_126 : STD_LOGIC;
  signal buff1_reg_n_127 : STD_LOGIC;
  signal buff1_reg_n_128 : STD_LOGIC;
  signal buff1_reg_n_129 : STD_LOGIC;
  signal buff1_reg_n_130 : STD_LOGIC;
  signal buff1_reg_n_131 : STD_LOGIC;
  signal buff1_reg_n_132 : STD_LOGIC;
  signal buff1_reg_n_133 : STD_LOGIC;
  signal buff1_reg_n_134 : STD_LOGIC;
  signal buff1_reg_n_135 : STD_LOGIC;
  signal buff1_reg_n_136 : STD_LOGIC;
  signal buff1_reg_n_137 : STD_LOGIC;
  signal buff1_reg_n_138 : STD_LOGIC;
  signal buff1_reg_n_139 : STD_LOGIC;
  signal buff1_reg_n_140 : STD_LOGIC;
  signal buff1_reg_n_141 : STD_LOGIC;
  signal buff1_reg_n_142 : STD_LOGIC;
  signal buff1_reg_n_143 : STD_LOGIC;
  signal buff1_reg_n_144 : STD_LOGIC;
  signal buff1_reg_n_145 : STD_LOGIC;
  signal buff1_reg_n_146 : STD_LOGIC;
  signal buff1_reg_n_147 : STD_LOGIC;
  signal buff1_reg_n_148 : STD_LOGIC;
  signal buff1_reg_n_149 : STD_LOGIC;
  signal buff1_reg_n_150 : STD_LOGIC;
  signal buff1_reg_n_151 : STD_LOGIC;
  signal buff1_reg_n_152 : STD_LOGIC;
  signal buff1_reg_n_153 : STD_LOGIC;
  signal buff1_reg_n_154 : STD_LOGIC;
  signal \buff1_reg_n_1_[0]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[10]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[11]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[12]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[13]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[14]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[15]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[16]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[1]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[2]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[3]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[4]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[5]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[6]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[7]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[8]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[9]\ : STD_LOGIC;
  signal \buff2_reg__0\ : STD_LOGIC_VECTOR ( 31 downto 0 );
  signal buff2_reg_n_59 : STD_LOGIC;
  signal buff2_reg_n_60 : STD_LOGIC;
  signal buff2_reg_n_61 : STD_LOGIC;
  signal buff2_reg_n_62 : STD_LOGIC;
  signal buff2_reg_n_63 : STD_LOGIC;
  signal buff2_reg_n_64 : STD_LOGIC;
  signal buff2_reg_n_65 : STD_LOGIC;
  signal buff2_reg_n_66 : STD_LOGIC;
  signal buff2_reg_n_67 : STD_LOGIC;
  signal buff2_reg_n_68 : STD_LOGIC;
  signal buff2_reg_n_69 : STD_LOGIC;
  signal buff2_reg_n_70 : STD_LOGIC;
  signal buff2_reg_n_71 : STD_LOGIC;
  signal buff2_reg_n_72 : STD_LOGIC;
  signal buff2_reg_n_73 : STD_LOGIC;
  signal buff2_reg_n_74 : STD_LOGIC;
  signal buff2_reg_n_75 : STD_LOGIC;
  signal buff2_reg_n_76 : STD_LOGIC;
  signal buff2_reg_n_77 : STD_LOGIC;
  signal buff2_reg_n_78 : STD_LOGIC;
  signal buff2_reg_n_79 : STD_LOGIC;
  signal buff2_reg_n_80 : STD_LOGIC;
  signal buff2_reg_n_81 : STD_LOGIC;
  signal buff2_reg_n_82 : STD_LOGIC;
  signal buff2_reg_n_83 : STD_LOGIC;
  signal buff2_reg_n_84 : STD_LOGIC;
  signal buff2_reg_n_85 : STD_LOGIC;
  signal buff2_reg_n_86 : STD_LOGIC;
  signal buff2_reg_n_87 : STD_LOGIC;
  signal buff2_reg_n_88 : STD_LOGIC;
  signal buff2_reg_n_89 : STD_LOGIC;
  signal buff2_reg_n_90 : STD_LOGIC;
  signal buff2_reg_n_91 : STD_LOGIC;
  signal NLW_buff0_reg_CARRYCASCOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_MULTSIGNOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_OVERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_PATTERNBDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_PATTERNDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_UNDERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_ACOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 29 downto 0 );
  signal NLW_buff0_reg_BCOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 17 downto 0 );
  signal NLW_buff0_reg_CARRYOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_buff1_reg_CARRYCASCOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_MULTSIGNOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_OVERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_PATTERNBDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_PATTERNDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_UNDERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_ACOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 29 downto 0 );
  signal NLW_buff1_reg_BCOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 17 downto 0 );
  signal NLW_buff1_reg_CARRYOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_buff1_reg_P_UNCONNECTED : STD_LOGIC_VECTOR ( 47 downto 0 );
  signal \NLW_buff1_reg_i_1__3_CO_UNCONNECTED\ : STD_LOGIC_VECTOR ( 3 downto 2 );
  signal \NLW_buff1_reg_i_1__3_O_UNCONNECTED\ : STD_LOGIC_VECTOR ( 3 to 3 );
  signal NLW_buff2_reg_CARRYCASCOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_MULTSIGNOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_OVERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_PATTERNBDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_PATTERNDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_UNDERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_ACOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 29 downto 0 );
  signal NLW_buff2_reg_BCOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 17 downto 0 );
  signal NLW_buff2_reg_CARRYOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_buff2_reg_PCOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 47 downto 0 );
begin
buff0_reg: unisim.vcomponents.DSP48E1
    generic map(
      ACASCREG => 2,
      ADREG => 1,
      ALUMODEREG => 0,
      AREG => 2,
      AUTORESET_PATDET => "NO_RESET",
      A_INPUT => "DIRECT",
      BCASCREG => 1,
      BREG => 1,
      B_INPUT => "DIRECT",
      CARRYINREG => 0,
      CARRYINSELREG => 0,
      CREG => 0,
      DREG => 1,
      INMODEREG => 0,
      MASK => X"3FFFFFFFFFFF",
      MREG => 0,
      OPMODEREG => 0,
      PATTERN => X"000000000000",
      PREG => 1,
      SEL_MASK => "MASK",
      SEL_PATTERN => "PATTERN",
      USE_DPORT => false,
      USE_MULT => "MULTIPLY",
      USE_PATTERN_DETECT => "NO_PATDET",
      USE_SIMD => "ONE48"
    )
        port map (
      A(29 downto 17) => B"0000000000000",
      A(16 downto 0) => A_q0(16 downto 0),
      ACIN(29 downto 0) => B"000000000000000000000000000000",
      ACOUT(29 downto 0) => NLW_buff0_reg_ACOUT_UNCONNECTED(29 downto 0),
      ALUMODE(3 downto 0) => B"0000",
      B(17) => '0',
      B(16 downto 0) => grp_fu_157_p1(16 downto 0),
      BCIN(17 downto 0) => B"000000000000000000",
      BCOUT(17 downto 0) => NLW_buff0_reg_BCOUT_UNCONNECTED(17 downto 0),
      C(47 downto 0) => B"111111111111111111111111111111111111111111111111",
      CARRYCASCIN => '0',
      CARRYCASCOUT => NLW_buff0_reg_CARRYCASCOUT_UNCONNECTED,
      CARRYIN => '0',
      CARRYINSEL(2 downto 0) => B"000",
      CARRYOUT(3 downto 0) => NLW_buff0_reg_CARRYOUT_UNCONNECTED(3 downto 0),
      CEA1 => Q(0),
      CEA2 => '1',
      CEAD => '0',
      CEALUMODE => '0',
      CEB1 => '0',
      CEB2 => '1',
      CEC => '0',
      CECARRYIN => '0',
      CECTRL => '0',
      CED => '0',
      CEINMODE => '0',
      CEM => '0',
      CEP => '1',
      CLK => ap_clk,
      D(24 downto 0) => B"0000000000000000000000000",
      INMODE(4 downto 0) => B"00000",
      MULTSIGNIN => '0',
      MULTSIGNOUT => NLW_buff0_reg_MULTSIGNOUT_UNCONNECTED,
      OPMODE(6 downto 0) => B"0000101",
      OVERFLOW => NLW_buff0_reg_OVERFLOW_UNCONNECTED,
      P(47) => buff0_reg_n_59,
      P(46) => buff0_reg_n_60,
      P(45) => buff0_reg_n_61,
      P(44) => buff0_reg_n_62,
      P(43) => buff0_reg_n_63,
      P(42) => buff0_reg_n_64,
      P(41) => buff0_reg_n_65,
      P(40) => buff0_reg_n_66,
      P(39) => buff0_reg_n_67,
      P(38) => buff0_reg_n_68,
      P(37) => buff0_reg_n_69,
      P(36) => buff0_reg_n_70,
      P(35) => buff0_reg_n_71,
      P(34) => buff0_reg_n_72,
      P(33) => buff0_reg_n_73,
      P(32) => buff0_reg_n_74,
      P(31) => buff0_reg_n_75,
      P(30) => buff0_reg_n_76,
      P(29) => buff0_reg_n_77,
      P(28) => buff0_reg_n_78,
      P(27) => buff0_reg_n_79,
      P(26) => buff0_reg_n_80,
      P(25) => buff0_reg_n_81,
      P(24) => buff0_reg_n_82,
      P(23) => buff0_reg_n_83,
      P(22) => buff0_reg_n_84,
      P(21) => buff0_reg_n_85,
      P(20) => buff0_reg_n_86,
      P(19) => buff0_reg_n_87,
      P(18) => buff0_reg_n_88,
      P(17) => buff0_reg_n_89,
      P(16) => buff0_reg_n_90,
      P(15) => buff0_reg_n_91,
      P(14) => buff0_reg_n_92,
      P(13) => buff0_reg_n_93,
      P(12) => buff0_reg_n_94,
      P(11) => buff0_reg_n_95,
      P(10) => buff0_reg_n_96,
      P(9) => buff0_reg_n_97,
      P(8) => buff0_reg_n_98,
      P(7) => buff0_reg_n_99,
      P(6) => buff0_reg_n_100,
      P(5) => buff0_reg_n_101,
      P(4) => buff0_reg_n_102,
      P(3) => buff0_reg_n_103,
      P(2) => buff0_reg_n_104,
      P(1) => buff0_reg_n_105,
      P(0) => buff0_reg_n_106,
      PATTERNBDETECT => NLW_buff0_reg_PATTERNBDETECT_UNCONNECTED,
      PATTERNDETECT => NLW_buff0_reg_PATTERNDETECT_UNCONNECTED,
      PCIN(47 downto 0) => B"000000000000000000000000000000000000000000000000",
      PCOUT(47) => buff0_reg_n_107,
      PCOUT(46) => buff0_reg_n_108,
      PCOUT(45) => buff0_reg_n_109,
      PCOUT(44) => buff0_reg_n_110,
      PCOUT(43) => buff0_reg_n_111,
      PCOUT(42) => buff0_reg_n_112,
      PCOUT(41) => buff0_reg_n_113,
      PCOUT(40) => buff0_reg_n_114,
      PCOUT(39) => buff0_reg_n_115,
      PCOUT(38) => buff0_reg_n_116,
      PCOUT(37) => buff0_reg_n_117,
      PCOUT(36) => buff0_reg_n_118,
      PCOUT(35) => buff0_reg_n_119,
      PCOUT(34) => buff0_reg_n_120,
      PCOUT(33) => buff0_reg_n_121,
      PCOUT(32) => buff0_reg_n_122,
      PCOUT(31) => buff0_reg_n_123,
      PCOUT(30) => buff0_reg_n_124,
      PCOUT(29) => buff0_reg_n_125,
      PCOUT(28) => buff0_reg_n_126,
      PCOUT(27) => buff0_reg_n_127,
      PCOUT(26) => buff0_reg_n_128,
      PCOUT(25) => buff0_reg_n_129,
      PCOUT(24) => buff0_reg_n_130,
      PCOUT(23) => buff0_reg_n_131,
      PCOUT(22) => buff0_reg_n_132,
      PCOUT(21) => buff0_reg_n_133,
      PCOUT(20) => buff0_reg_n_134,
      PCOUT(19) => buff0_reg_n_135,
      PCOUT(18) => buff0_reg_n_136,
      PCOUT(17) => buff0_reg_n_137,
      PCOUT(16) => buff0_reg_n_138,
      PCOUT(15) => buff0_reg_n_139,
      PCOUT(14) => buff0_reg_n_140,
      PCOUT(13) => buff0_reg_n_141,
      PCOUT(12) => buff0_reg_n_142,
      PCOUT(11) => buff0_reg_n_143,
      PCOUT(10) => buff0_reg_n_144,
      PCOUT(9) => buff0_reg_n_145,
      PCOUT(8) => buff0_reg_n_146,
      PCOUT(7) => buff0_reg_n_147,
      PCOUT(6) => buff0_reg_n_148,
      PCOUT(5) => buff0_reg_n_149,
      PCOUT(4) => buff0_reg_n_150,
      PCOUT(3) => buff0_reg_n_151,
      PCOUT(2) => buff0_reg_n_152,
      PCOUT(1) => buff0_reg_n_153,
      PCOUT(0) => buff0_reg_n_154,
      RSTA => '0',
      RSTALLCARRYIN => '0',
      RSTALUMODE => '0',
      RSTB => '0',
      RSTC => '0',
      RSTCTRL => '0',
      RSTD => '0',
      RSTINMODE => '0',
      RSTM => '0',
      RSTP => '0',
      UNDERFLOW => NLW_buff0_reg_UNDERFLOW_UNCONNECTED
    );
buff0_reg_i_10: unisim.vcomponents.LUT1
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => \buff2_reg__0\(2),
      O => buff0_reg_i_10_n_1
    );
buff0_reg_i_11: unisim.vcomponents.LUT1
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => \buff2_reg__0\(1),
      O => buff0_reg_i_11_n_1
    );
\buff0_reg_i_1__3\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff0_reg_i_2__3_n_1\,
      CO(3) => \buff0_reg_i_1__3_n_1\,
      CO(2) => \buff0_reg_i_1__3_n_2\,
      CO(1) => \buff0_reg_i_1__3_n_3\,
      CO(0) => \buff0_reg_i_1__3_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_168_p1(16 downto 13),
      S(3 downto 0) => \buff2_reg__0\(16 downto 13)
    );
\buff0_reg_i_2__3\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff0_reg_i_3__3_n_1\,
      CO(3) => \buff0_reg_i_2__3_n_1\,
      CO(2) => \buff0_reg_i_2__3_n_2\,
      CO(1) => \buff0_reg_i_2__3_n_3\,
      CO(0) => \buff0_reg_i_2__3_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_168_p1(12 downto 9),
      S(3 downto 0) => \buff2_reg__0\(12 downto 9)
    );
\buff0_reg_i_3__3\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff0_reg_i_4__3_n_1\,
      CO(3) => \buff0_reg_i_3__3_n_1\,
      CO(2) => \buff0_reg_i_3__3_n_2\,
      CO(1) => \buff0_reg_i_3__3_n_3\,
      CO(0) => \buff0_reg_i_3__3_n_4\,
      CYINIT => '0',
      DI(3 downto 2) => B"00",
      DI(1 downto 0) => \buff2_reg__0\(6 downto 5),
      O(3 downto 0) => grp_fu_168_p1(8 downto 5),
      S(3 downto 2) => \buff2_reg__0\(8 downto 7),
      S(1) => \buff0_reg_i_6__3_n_1\,
      S(0) => \buff0_reg_i_7__2_n_1\
    );
\buff0_reg_i_4__3\: unisim.vcomponents.CARRY4
     port map (
      CI => '0',
      CO(3) => \buff0_reg_i_4__3_n_1\,
      CO(2) => \buff0_reg_i_4__3_n_2\,
      CO(1) => \buff0_reg_i_4__3_n_3\,
      CO(0) => \buff0_reg_i_4__3_n_4\,
      CYINIT => \buff2_reg__0\(0),
      DI(3 downto 0) => \buff2_reg__0\(4 downto 1),
      O(3 downto 0) => grp_fu_168_p1(4 downto 1),
      S(3) => \buff0_reg_i_8__1_n_1\,
      S(2) => buff0_reg_i_9_n_1,
      S(1) => buff0_reg_i_10_n_1,
      S(0) => buff0_reg_i_11_n_1
    );
\buff0_reg_i_5__3\: unisim.vcomponents.LUT1
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => \buff2_reg__0\(0),
      O => grp_fu_168_p1(0)
    );
\buff0_reg_i_6__3\: unisim.vcomponents.LUT1
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => \buff2_reg__0\(6),
      O => \buff0_reg_i_6__3_n_1\
    );
\buff0_reg_i_7__2\: unisim.vcomponents.LUT1
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => \buff2_reg__0\(5),
      O => \buff0_reg_i_7__2_n_1\
    );
\buff0_reg_i_8__1\: unisim.vcomponents.LUT1
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => \buff2_reg__0\(4),
      O => \buff0_reg_i_8__1_n_1\
    );
buff0_reg_i_9: unisim.vcomponents.LUT1
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => \buff2_reg__0\(3),
      O => buff0_reg_i_9_n_1
    );
buff1_reg: unisim.vcomponents.DSP48E1
    generic map(
      ACASCREG => 2,
      ADREG => 1,
      ALUMODEREG => 0,
      AREG => 2,
      AUTORESET_PATDET => "NO_RESET",
      A_INPUT => "DIRECT",
      BCASCREG => 1,
      BREG => 1,
      B_INPUT => "DIRECT",
      CARRYINREG => 0,
      CARRYINSELREG => 0,
      CREG => 1,
      DREG => 1,
      INMODEREG => 0,
      MASK => X"3FFFFFFFFFFF",
      MREG => 1,
      OPMODEREG => 0,
      PATTERN => X"000000000000",
      PREG => 1,
      SEL_MASK => "MASK",
      SEL_PATTERN => "PATTERN",
      USE_DPORT => false,
      USE_MULT => "MULTIPLY",
      USE_PATTERN_DETECT => "NO_PATDET",
      USE_SIMD => "ONE48"
    )
        port map (
      A(29 downto 17) => B"0000000000000",
      A(16 downto 0) => A_q0(16 downto 0),
      ACIN(29 downto 0) => B"000000000000000000000000000000",
      ACOUT(29 downto 0) => NLW_buff1_reg_ACOUT_UNCONNECTED(29 downto 0),
      ALUMODE(3 downto 0) => B"0000",
      B(17) => grp_fu_157_p1(31),
      B(16) => grp_fu_157_p1(31),
      B(15) => grp_fu_157_p1(31),
      B(14 downto 0) => grp_fu_157_p1(31 downto 17),
      BCIN(17 downto 0) => B"000000000000000000",
      BCOUT(17 downto 0) => NLW_buff1_reg_BCOUT_UNCONNECTED(17 downto 0),
      C(47 downto 0) => B"111111111111111111111111111111111111111111111111",
      CARRYCASCIN => '0',
      CARRYCASCOUT => NLW_buff1_reg_CARRYCASCOUT_UNCONNECTED,
      CARRYIN => '0',
      CARRYINSEL(2 downto 0) => B"000",
      CARRYOUT(3 downto 0) => NLW_buff1_reg_CARRYOUT_UNCONNECTED(3 downto 0),
      CEA1 => Q(0),
      CEA2 => '1',
      CEAD => '0',
      CEALUMODE => '0',
      CEB1 => '0',
      CEB2 => '1',
      CEC => '0',
      CECARRYIN => '0',
      CECTRL => '0',
      CED => '0',
      CEINMODE => '0',
      CEM => '1',
      CEP => '1',
      CLK => ap_clk,
      D(24 downto 0) => B"0000000000000000000000000",
      INMODE(4 downto 0) => B"00000",
      MULTSIGNIN => '0',
      MULTSIGNOUT => NLW_buff1_reg_MULTSIGNOUT_UNCONNECTED,
      OPMODE(6 downto 0) => B"1010101",
      OVERFLOW => NLW_buff1_reg_OVERFLOW_UNCONNECTED,
      P(47 downto 0) => NLW_buff1_reg_P_UNCONNECTED(47 downto 0),
      PATTERNBDETECT => NLW_buff1_reg_PATTERNBDETECT_UNCONNECTED,
      PATTERNDETECT => NLW_buff1_reg_PATTERNDETECT_UNCONNECTED,
      PCIN(47) => buff0_reg_n_107,
      PCIN(46) => buff0_reg_n_108,
      PCIN(45) => buff0_reg_n_109,
      PCIN(44) => buff0_reg_n_110,
      PCIN(43) => buff0_reg_n_111,
      PCIN(42) => buff0_reg_n_112,
      PCIN(41) => buff0_reg_n_113,
      PCIN(40) => buff0_reg_n_114,
      PCIN(39) => buff0_reg_n_115,
      PCIN(38) => buff0_reg_n_116,
      PCIN(37) => buff0_reg_n_117,
      PCIN(36) => buff0_reg_n_118,
      PCIN(35) => buff0_reg_n_119,
      PCIN(34) => buff0_reg_n_120,
      PCIN(33) => buff0_reg_n_121,
      PCIN(32) => buff0_reg_n_122,
      PCIN(31) => buff0_reg_n_123,
      PCIN(30) => buff0_reg_n_124,
      PCIN(29) => buff0_reg_n_125,
      PCIN(28) => buff0_reg_n_126,
      PCIN(27) => buff0_reg_n_127,
      PCIN(26) => buff0_reg_n_128,
      PCIN(25) => buff0_reg_n_129,
      PCIN(24) => buff0_reg_n_130,
      PCIN(23) => buff0_reg_n_131,
      PCIN(22) => buff0_reg_n_132,
      PCIN(21) => buff0_reg_n_133,
      PCIN(20) => buff0_reg_n_134,
      PCIN(19) => buff0_reg_n_135,
      PCIN(18) => buff0_reg_n_136,
      PCIN(17) => buff0_reg_n_137,
      PCIN(16) => buff0_reg_n_138,
      PCIN(15) => buff0_reg_n_139,
      PCIN(14) => buff0_reg_n_140,
      PCIN(13) => buff0_reg_n_141,
      PCIN(12) => buff0_reg_n_142,
      PCIN(11) => buff0_reg_n_143,
      PCIN(10) => buff0_reg_n_144,
      PCIN(9) => buff0_reg_n_145,
      PCIN(8) => buff0_reg_n_146,
      PCIN(7) => buff0_reg_n_147,
      PCIN(6) => buff0_reg_n_148,
      PCIN(5) => buff0_reg_n_149,
      PCIN(4) => buff0_reg_n_150,
      PCIN(3) => buff0_reg_n_151,
      PCIN(2) => buff0_reg_n_152,
      PCIN(1) => buff0_reg_n_153,
      PCIN(0) => buff0_reg_n_154,
      PCOUT(47) => buff1_reg_n_107,
      PCOUT(46) => buff1_reg_n_108,
      PCOUT(45) => buff1_reg_n_109,
      PCOUT(44) => buff1_reg_n_110,
      PCOUT(43) => buff1_reg_n_111,
      PCOUT(42) => buff1_reg_n_112,
      PCOUT(41) => buff1_reg_n_113,
      PCOUT(40) => buff1_reg_n_114,
      PCOUT(39) => buff1_reg_n_115,
      PCOUT(38) => buff1_reg_n_116,
      PCOUT(37) => buff1_reg_n_117,
      PCOUT(36) => buff1_reg_n_118,
      PCOUT(35) => buff1_reg_n_119,
      PCOUT(34) => buff1_reg_n_120,
      PCOUT(33) => buff1_reg_n_121,
      PCOUT(32) => buff1_reg_n_122,
      PCOUT(31) => buff1_reg_n_123,
      PCOUT(30) => buff1_reg_n_124,
      PCOUT(29) => buff1_reg_n_125,
      PCOUT(28) => buff1_reg_n_126,
      PCOUT(27) => buff1_reg_n_127,
      PCOUT(26) => buff1_reg_n_128,
      PCOUT(25) => buff1_reg_n_129,
      PCOUT(24) => buff1_reg_n_130,
      PCOUT(23) => buff1_reg_n_131,
      PCOUT(22) => buff1_reg_n_132,
      PCOUT(21) => buff1_reg_n_133,
      PCOUT(20) => buff1_reg_n_134,
      PCOUT(19) => buff1_reg_n_135,
      PCOUT(18) => buff1_reg_n_136,
      PCOUT(17) => buff1_reg_n_137,
      PCOUT(16) => buff1_reg_n_138,
      PCOUT(15) => buff1_reg_n_139,
      PCOUT(14) => buff1_reg_n_140,
      PCOUT(13) => buff1_reg_n_141,
      PCOUT(12) => buff1_reg_n_142,
      PCOUT(11) => buff1_reg_n_143,
      PCOUT(10) => buff1_reg_n_144,
      PCOUT(9) => buff1_reg_n_145,
      PCOUT(8) => buff1_reg_n_146,
      PCOUT(7) => buff1_reg_n_147,
      PCOUT(6) => buff1_reg_n_148,
      PCOUT(5) => buff1_reg_n_149,
      PCOUT(4) => buff1_reg_n_150,
      PCOUT(3) => buff1_reg_n_151,
      PCOUT(2) => buff1_reg_n_152,
      PCOUT(1) => buff1_reg_n_153,
      PCOUT(0) => buff1_reg_n_154,
      RSTA => '0',
      RSTALLCARRYIN => '0',
      RSTALUMODE => '0',
      RSTB => '0',
      RSTC => '0',
      RSTCTRL => '0',
      RSTD => '0',
      RSTINMODE => '0',
      RSTM => '0',
      RSTP => '0',
      UNDERFLOW => NLW_buff1_reg_UNDERFLOW_UNCONNECTED
    );
\buff1_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_106,
      Q => \buff1_reg_n_1_[0]\,
      R => '0'
    );
\buff1_reg[10]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_96,
      Q => \buff1_reg_n_1_[10]\,
      R => '0'
    );
\buff1_reg[11]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_95,
      Q => \buff1_reg_n_1_[11]\,
      R => '0'
    );
\buff1_reg[12]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_94,
      Q => \buff1_reg_n_1_[12]\,
      R => '0'
    );
\buff1_reg[13]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_93,
      Q => \buff1_reg_n_1_[13]\,
      R => '0'
    );
\buff1_reg[14]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_92,
      Q => \buff1_reg_n_1_[14]\,
      R => '0'
    );
\buff1_reg[15]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_91,
      Q => \buff1_reg_n_1_[15]\,
      R => '0'
    );
\buff1_reg[16]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_90,
      Q => \buff1_reg_n_1_[16]\,
      R => '0'
    );
\buff1_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_105,
      Q => \buff1_reg_n_1_[1]\,
      R => '0'
    );
\buff1_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_104,
      Q => \buff1_reg_n_1_[2]\,
      R => '0'
    );
\buff1_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_103,
      Q => \buff1_reg_n_1_[3]\,
      R => '0'
    );
\buff1_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_102,
      Q => \buff1_reg_n_1_[4]\,
      R => '0'
    );
\buff1_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_101,
      Q => \buff1_reg_n_1_[5]\,
      R => '0'
    );
\buff1_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_100,
      Q => \buff1_reg_n_1_[6]\,
      R => '0'
    );
\buff1_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_99,
      Q => \buff1_reg_n_1_[7]\,
      R => '0'
    );
\buff1_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_98,
      Q => \buff1_reg_n_1_[8]\,
      R => '0'
    );
\buff1_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_97,
      Q => \buff1_reg_n_1_[9]\,
      R => '0'
    );
\buff1_reg_i_1__3\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff1_reg_i_2__3_n_1\,
      CO(3 downto 2) => \NLW_buff1_reg_i_1__3_CO_UNCONNECTED\(3 downto 2),
      CO(1) => \buff1_reg_i_1__3_n_3\,
      CO(0) => \buff1_reg_i_1__3_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3) => \NLW_buff1_reg_i_1__3_O_UNCONNECTED\(3),
      O(2 downto 0) => grp_fu_168_p1(31 downto 29),
      S(3) => '0',
      S(2 downto 0) => \buff2_reg__0\(31 downto 29)
    );
\buff1_reg_i_2__3\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff1_reg_i_3__3_n_1\,
      CO(3) => \buff1_reg_i_2__3_n_1\,
      CO(2) => \buff1_reg_i_2__3_n_2\,
      CO(1) => \buff1_reg_i_2__3_n_3\,
      CO(0) => \buff1_reg_i_2__3_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_168_p1(28 downto 25),
      S(3 downto 0) => \buff2_reg__0\(28 downto 25)
    );
\buff1_reg_i_3__3\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff1_reg_i_4__2_n_1\,
      CO(3) => \buff1_reg_i_3__3_n_1\,
      CO(2) => \buff1_reg_i_3__3_n_2\,
      CO(1) => \buff1_reg_i_3__3_n_3\,
      CO(0) => \buff1_reg_i_3__3_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_168_p1(24 downto 21),
      S(3 downto 0) => \buff2_reg__0\(24 downto 21)
    );
\buff1_reg_i_4__2\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff0_reg_i_1__3_n_1\,
      CO(3) => \buff1_reg_i_4__2_n_1\,
      CO(2) => \buff1_reg_i_4__2_n_2\,
      CO(1) => \buff1_reg_i_4__2_n_3\,
      CO(0) => \buff1_reg_i_4__2_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_168_p1(20 downto 17),
      S(3 downto 0) => \buff2_reg__0\(20 downto 17)
    );
buff2_reg: unisim.vcomponents.DSP48E1
    generic map(
      ACASCREG => 2,
      ADREG => 1,
      ALUMODEREG => 0,
      AREG => 2,
      AUTORESET_PATDET => "NO_RESET",
      A_INPUT => "DIRECT",
      BCASCREG => 2,
      BREG => 2,
      B_INPUT => "DIRECT",
      CARRYINREG => 0,
      CARRYINSELREG => 0,
      CREG => 1,
      DREG => 1,
      INMODEREG => 0,
      MASK => X"3FFFFFFFFFFF",
      MREG => 1,
      OPMODEREG => 0,
      PATTERN => X"000000000000",
      PREG => 1,
      SEL_MASK => "MASK",
      SEL_PATTERN => "PATTERN",
      USE_DPORT => false,
      USE_MULT => "MULTIPLY",
      USE_PATTERN_DETECT => "NO_PATDET",
      USE_SIMD => "ONE48"
    )
        port map (
      A(29 downto 17) => B"0000000000000",
      A(16 downto 0) => grp_fu_157_p1(16 downto 0),
      ACIN(29 downto 0) => B"000000000000000000000000000000",
      ACOUT(29 downto 0) => NLW_buff2_reg_ACOUT_UNCONNECTED(29 downto 0),
      ALUMODE(3 downto 0) => B"0000",
      B(17) => buff2_reg_0(14),
      B(16) => buff2_reg_0(14),
      B(15) => buff2_reg_0(14),
      B(14 downto 0) => buff2_reg_0(14 downto 0),
      BCIN(17 downto 0) => B"000000000000000000",
      BCOUT(17 downto 0) => NLW_buff2_reg_BCOUT_UNCONNECTED(17 downto 0),
      C(47 downto 0) => B"111111111111111111111111111111111111111111111111",
      CARRYCASCIN => '0',
      CARRYCASCOUT => NLW_buff2_reg_CARRYCASCOUT_UNCONNECTED,
      CARRYIN => '0',
      CARRYINSEL(2 downto 0) => B"000",
      CARRYOUT(3 downto 0) => NLW_buff2_reg_CARRYOUT_UNCONNECTED(3 downto 0),
      CEA1 => '1',
      CEA2 => '1',
      CEAD => '0',
      CEALUMODE => '0',
      CEB1 => '1',
      CEB2 => '1',
      CEC => '0',
      CECARRYIN => '0',
      CECTRL => '0',
      CED => '0',
      CEINMODE => '0',
      CEM => '1',
      CEP => '1',
      CLK => ap_clk,
      D(24 downto 0) => B"0000000000000000000000000",
      INMODE(4 downto 0) => B"00000",
      MULTSIGNIN => '0',
      MULTSIGNOUT => NLW_buff2_reg_MULTSIGNOUT_UNCONNECTED,
      OPMODE(6 downto 0) => B"0010101",
      OVERFLOW => NLW_buff2_reg_OVERFLOW_UNCONNECTED,
      P(47) => buff2_reg_n_59,
      P(46) => buff2_reg_n_60,
      P(45) => buff2_reg_n_61,
      P(44) => buff2_reg_n_62,
      P(43) => buff2_reg_n_63,
      P(42) => buff2_reg_n_64,
      P(41) => buff2_reg_n_65,
      P(40) => buff2_reg_n_66,
      P(39) => buff2_reg_n_67,
      P(38) => buff2_reg_n_68,
      P(37) => buff2_reg_n_69,
      P(36) => buff2_reg_n_70,
      P(35) => buff2_reg_n_71,
      P(34) => buff2_reg_n_72,
      P(33) => buff2_reg_n_73,
      P(32) => buff2_reg_n_74,
      P(31) => buff2_reg_n_75,
      P(30) => buff2_reg_n_76,
      P(29) => buff2_reg_n_77,
      P(28) => buff2_reg_n_78,
      P(27) => buff2_reg_n_79,
      P(26) => buff2_reg_n_80,
      P(25) => buff2_reg_n_81,
      P(24) => buff2_reg_n_82,
      P(23) => buff2_reg_n_83,
      P(22) => buff2_reg_n_84,
      P(21) => buff2_reg_n_85,
      P(20) => buff2_reg_n_86,
      P(19) => buff2_reg_n_87,
      P(18) => buff2_reg_n_88,
      P(17) => buff2_reg_n_89,
      P(16) => buff2_reg_n_90,
      P(15) => buff2_reg_n_91,
      P(14 downto 0) => \buff2_reg__0\(31 downto 17),
      PATTERNBDETECT => NLW_buff2_reg_PATTERNBDETECT_UNCONNECTED,
      PATTERNDETECT => NLW_buff2_reg_PATTERNDETECT_UNCONNECTED,
      PCIN(47) => buff1_reg_n_107,
      PCIN(46) => buff1_reg_n_108,
      PCIN(45) => buff1_reg_n_109,
      PCIN(44) => buff1_reg_n_110,
      PCIN(43) => buff1_reg_n_111,
      PCIN(42) => buff1_reg_n_112,
      PCIN(41) => buff1_reg_n_113,
      PCIN(40) => buff1_reg_n_114,
      PCIN(39) => buff1_reg_n_115,
      PCIN(38) => buff1_reg_n_116,
      PCIN(37) => buff1_reg_n_117,
      PCIN(36) => buff1_reg_n_118,
      PCIN(35) => buff1_reg_n_119,
      PCIN(34) => buff1_reg_n_120,
      PCIN(33) => buff1_reg_n_121,
      PCIN(32) => buff1_reg_n_122,
      PCIN(31) => buff1_reg_n_123,
      PCIN(30) => buff1_reg_n_124,
      PCIN(29) => buff1_reg_n_125,
      PCIN(28) => buff1_reg_n_126,
      PCIN(27) => buff1_reg_n_127,
      PCIN(26) => buff1_reg_n_128,
      PCIN(25) => buff1_reg_n_129,
      PCIN(24) => buff1_reg_n_130,
      PCIN(23) => buff1_reg_n_131,
      PCIN(22) => buff1_reg_n_132,
      PCIN(21) => buff1_reg_n_133,
      PCIN(20) => buff1_reg_n_134,
      PCIN(19) => buff1_reg_n_135,
      PCIN(18) => buff1_reg_n_136,
      PCIN(17) => buff1_reg_n_137,
      PCIN(16) => buff1_reg_n_138,
      PCIN(15) => buff1_reg_n_139,
      PCIN(14) => buff1_reg_n_140,
      PCIN(13) => buff1_reg_n_141,
      PCIN(12) => buff1_reg_n_142,
      PCIN(11) => buff1_reg_n_143,
      PCIN(10) => buff1_reg_n_144,
      PCIN(9) => buff1_reg_n_145,
      PCIN(8) => buff1_reg_n_146,
      PCIN(7) => buff1_reg_n_147,
      PCIN(6) => buff1_reg_n_148,
      PCIN(5) => buff1_reg_n_149,
      PCIN(4) => buff1_reg_n_150,
      PCIN(3) => buff1_reg_n_151,
      PCIN(2) => buff1_reg_n_152,
      PCIN(1) => buff1_reg_n_153,
      PCIN(0) => buff1_reg_n_154,
      PCOUT(47 downto 0) => NLW_buff2_reg_PCOUT_UNCONNECTED(47 downto 0),
      RSTA => '0',
      RSTALLCARRYIN => '0',
      RSTALUMODE => '0',
      RSTB => '0',
      RSTC => '0',
      RSTCTRL => '0',
      RSTD => '0',
      RSTINMODE => '0',
      RSTM => '0',
      RSTP => '0',
      UNDERFLOW => NLW_buff2_reg_UNDERFLOW_UNCONNECTED
    );
\buff2_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[0]\,
      Q => \buff2_reg__0\(0),
      R => '0'
    );
\buff2_reg[10]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[10]\,
      Q => \buff2_reg__0\(10),
      R => '0'
    );
\buff2_reg[11]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[11]\,
      Q => \buff2_reg__0\(11),
      R => '0'
    );
\buff2_reg[12]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[12]\,
      Q => \buff2_reg__0\(12),
      R => '0'
    );
\buff2_reg[13]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[13]\,
      Q => \buff2_reg__0\(13),
      R => '0'
    );
\buff2_reg[14]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[14]\,
      Q => \buff2_reg__0\(14),
      R => '0'
    );
\buff2_reg[15]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[15]\,
      Q => \buff2_reg__0\(15),
      R => '0'
    );
\buff2_reg[16]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[16]\,
      Q => \buff2_reg__0\(16),
      R => '0'
    );
\buff2_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[1]\,
      Q => \buff2_reg__0\(1),
      R => '0'
    );
\buff2_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[2]\,
      Q => \buff2_reg__0\(2),
      R => '0'
    );
\buff2_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[3]\,
      Q => \buff2_reg__0\(3),
      R => '0'
    );
\buff2_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[4]\,
      Q => \buff2_reg__0\(4),
      R => '0'
    );
\buff2_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[5]\,
      Q => \buff2_reg__0\(5),
      R => '0'
    );
\buff2_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[6]\,
      Q => \buff2_reg__0\(6),
      R => '0'
    );
\buff2_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[7]\,
      Q => \buff2_reg__0\(7),
      R => '0'
    );
\buff2_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[8]\,
      Q => \buff2_reg__0\(8),
      R => '0'
    );
\buff2_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[9]\,
      Q => \buff2_reg__0\(9),
      R => '0'
    );
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_MulnS_0_9 is
  port (
    grp_fu_157_p1 : out STD_LOGIC_VECTOR ( 31 downto 0 );
    Q : in STD_LOGIC_VECTOR ( 0 to 0 );
    ap_clk : in STD_LOGIC;
    grp_fu_146_p1 : in STD_LOGIC_VECTOR ( 30 downto 0 );
    A : in STD_LOGIC_VECTOR ( 0 to 0 );
    A_q0 : in STD_LOGIC_VECTOR ( 16 downto 0 );
    buff2_reg_0 : in STD_LOGIC_VECTOR ( 14 downto 0 )
  );
  attribute ORIG_REF_NAME : string;
  attribute ORIG_REF_NAME of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_MulnS_0_9 : entity is "loop_imperfect_mubkb_MulnS_0";
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_MulnS_0_9;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_MulnS_0_9 is
  signal \buff0_reg_i_1__2_n_1\ : STD_LOGIC;
  signal \buff0_reg_i_1__2_n_2\ : STD_LOGIC;
  signal \buff0_reg_i_1__2_n_3\ : STD_LOGIC;
  signal \buff0_reg_i_1__2_n_4\ : STD_LOGIC;
  signal \buff0_reg_i_2__2_n_1\ : STD_LOGIC;
  signal \buff0_reg_i_2__2_n_2\ : STD_LOGIC;
  signal \buff0_reg_i_2__2_n_3\ : STD_LOGIC;
  signal \buff0_reg_i_2__2_n_4\ : STD_LOGIC;
  signal \buff0_reg_i_3__2_n_1\ : STD_LOGIC;
  signal \buff0_reg_i_3__2_n_2\ : STD_LOGIC;
  signal \buff0_reg_i_3__2_n_3\ : STD_LOGIC;
  signal \buff0_reg_i_3__2_n_4\ : STD_LOGIC;
  signal \buff0_reg_i_4__2_n_1\ : STD_LOGIC;
  signal \buff0_reg_i_4__2_n_2\ : STD_LOGIC;
  signal \buff0_reg_i_4__2_n_3\ : STD_LOGIC;
  signal \buff0_reg_i_4__2_n_4\ : STD_LOGIC;
  signal \buff0_reg_i_5__2_n_1\ : STD_LOGIC;
  signal \buff0_reg_i_5__2_n_2\ : STD_LOGIC;
  signal \buff0_reg_i_5__2_n_3\ : STD_LOGIC;
  signal \buff0_reg_i_5__2_n_4\ : STD_LOGIC;
  signal \buff0_reg_i_6__2_n_1\ : STD_LOGIC;
  signal \buff0_reg_i_7__1_n_1\ : STD_LOGIC;
  signal \buff0_reg_i_8__0_n_1\ : STD_LOGIC;
  signal buff0_reg_n_100 : STD_LOGIC;
  signal buff0_reg_n_101 : STD_LOGIC;
  signal buff0_reg_n_102 : STD_LOGIC;
  signal buff0_reg_n_103 : STD_LOGIC;
  signal buff0_reg_n_104 : STD_LOGIC;
  signal buff0_reg_n_105 : STD_LOGIC;
  signal buff0_reg_n_106 : STD_LOGIC;
  signal buff0_reg_n_107 : STD_LOGIC;
  signal buff0_reg_n_108 : STD_LOGIC;
  signal buff0_reg_n_109 : STD_LOGIC;
  signal buff0_reg_n_110 : STD_LOGIC;
  signal buff0_reg_n_111 : STD_LOGIC;
  signal buff0_reg_n_112 : STD_LOGIC;
  signal buff0_reg_n_113 : STD_LOGIC;
  signal buff0_reg_n_114 : STD_LOGIC;
  signal buff0_reg_n_115 : STD_LOGIC;
  signal buff0_reg_n_116 : STD_LOGIC;
  signal buff0_reg_n_117 : STD_LOGIC;
  signal buff0_reg_n_118 : STD_LOGIC;
  signal buff0_reg_n_119 : STD_LOGIC;
  signal buff0_reg_n_120 : STD_LOGIC;
  signal buff0_reg_n_121 : STD_LOGIC;
  signal buff0_reg_n_122 : STD_LOGIC;
  signal buff0_reg_n_123 : STD_LOGIC;
  signal buff0_reg_n_124 : STD_LOGIC;
  signal buff0_reg_n_125 : STD_LOGIC;
  signal buff0_reg_n_126 : STD_LOGIC;
  signal buff0_reg_n_127 : STD_LOGIC;
  signal buff0_reg_n_128 : STD_LOGIC;
  signal buff0_reg_n_129 : STD_LOGIC;
  signal buff0_reg_n_130 : STD_LOGIC;
  signal buff0_reg_n_131 : STD_LOGIC;
  signal buff0_reg_n_132 : STD_LOGIC;
  signal buff0_reg_n_133 : STD_LOGIC;
  signal buff0_reg_n_134 : STD_LOGIC;
  signal buff0_reg_n_135 : STD_LOGIC;
  signal buff0_reg_n_136 : STD_LOGIC;
  signal buff0_reg_n_137 : STD_LOGIC;
  signal buff0_reg_n_138 : STD_LOGIC;
  signal buff0_reg_n_139 : STD_LOGIC;
  signal buff0_reg_n_140 : STD_LOGIC;
  signal buff0_reg_n_141 : STD_LOGIC;
  signal buff0_reg_n_142 : STD_LOGIC;
  signal buff0_reg_n_143 : STD_LOGIC;
  signal buff0_reg_n_144 : STD_LOGIC;
  signal buff0_reg_n_145 : STD_LOGIC;
  signal buff0_reg_n_146 : STD_LOGIC;
  signal buff0_reg_n_147 : STD_LOGIC;
  signal buff0_reg_n_148 : STD_LOGIC;
  signal buff0_reg_n_149 : STD_LOGIC;
  signal buff0_reg_n_150 : STD_LOGIC;
  signal buff0_reg_n_151 : STD_LOGIC;
  signal buff0_reg_n_152 : STD_LOGIC;
  signal buff0_reg_n_153 : STD_LOGIC;
  signal buff0_reg_n_154 : STD_LOGIC;
  signal buff0_reg_n_59 : STD_LOGIC;
  signal buff0_reg_n_60 : STD_LOGIC;
  signal buff0_reg_n_61 : STD_LOGIC;
  signal buff0_reg_n_62 : STD_LOGIC;
  signal buff0_reg_n_63 : STD_LOGIC;
  signal buff0_reg_n_64 : STD_LOGIC;
  signal buff0_reg_n_65 : STD_LOGIC;
  signal buff0_reg_n_66 : STD_LOGIC;
  signal buff0_reg_n_67 : STD_LOGIC;
  signal buff0_reg_n_68 : STD_LOGIC;
  signal buff0_reg_n_69 : STD_LOGIC;
  signal buff0_reg_n_70 : STD_LOGIC;
  signal buff0_reg_n_71 : STD_LOGIC;
  signal buff0_reg_n_72 : STD_LOGIC;
  signal buff0_reg_n_73 : STD_LOGIC;
  signal buff0_reg_n_74 : STD_LOGIC;
  signal buff0_reg_n_75 : STD_LOGIC;
  signal buff0_reg_n_76 : STD_LOGIC;
  signal buff0_reg_n_77 : STD_LOGIC;
  signal buff0_reg_n_78 : STD_LOGIC;
  signal buff0_reg_n_79 : STD_LOGIC;
  signal buff0_reg_n_80 : STD_LOGIC;
  signal buff0_reg_n_81 : STD_LOGIC;
  signal buff0_reg_n_82 : STD_LOGIC;
  signal buff0_reg_n_83 : STD_LOGIC;
  signal buff0_reg_n_84 : STD_LOGIC;
  signal buff0_reg_n_85 : STD_LOGIC;
  signal buff0_reg_n_86 : STD_LOGIC;
  signal buff0_reg_n_87 : STD_LOGIC;
  signal buff0_reg_n_88 : STD_LOGIC;
  signal buff0_reg_n_89 : STD_LOGIC;
  signal buff0_reg_n_90 : STD_LOGIC;
  signal buff0_reg_n_91 : STD_LOGIC;
  signal buff0_reg_n_92 : STD_LOGIC;
  signal buff0_reg_n_93 : STD_LOGIC;
  signal buff0_reg_n_94 : STD_LOGIC;
  signal buff0_reg_n_95 : STD_LOGIC;
  signal buff0_reg_n_96 : STD_LOGIC;
  signal buff0_reg_n_97 : STD_LOGIC;
  signal buff0_reg_n_98 : STD_LOGIC;
  signal buff0_reg_n_99 : STD_LOGIC;
  signal \buff1_reg_i_1__2_n_2\ : STD_LOGIC;
  signal \buff1_reg_i_1__2_n_3\ : STD_LOGIC;
  signal \buff1_reg_i_1__2_n_4\ : STD_LOGIC;
  signal \buff1_reg_i_2__2_n_1\ : STD_LOGIC;
  signal \buff1_reg_i_2__2_n_2\ : STD_LOGIC;
  signal \buff1_reg_i_2__2_n_3\ : STD_LOGIC;
  signal \buff1_reg_i_2__2_n_4\ : STD_LOGIC;
  signal \buff1_reg_i_3__2_n_1\ : STD_LOGIC;
  signal \buff1_reg_i_3__2_n_2\ : STD_LOGIC;
  signal \buff1_reg_i_3__2_n_3\ : STD_LOGIC;
  signal \buff1_reg_i_3__2_n_4\ : STD_LOGIC;
  signal buff1_reg_n_107 : STD_LOGIC;
  signal buff1_reg_n_108 : STD_LOGIC;
  signal buff1_reg_n_109 : STD_LOGIC;
  signal buff1_reg_n_110 : STD_LOGIC;
  signal buff1_reg_n_111 : STD_LOGIC;
  signal buff1_reg_n_112 : STD_LOGIC;
  signal buff1_reg_n_113 : STD_LOGIC;
  signal buff1_reg_n_114 : STD_LOGIC;
  signal buff1_reg_n_115 : STD_LOGIC;
  signal buff1_reg_n_116 : STD_LOGIC;
  signal buff1_reg_n_117 : STD_LOGIC;
  signal buff1_reg_n_118 : STD_LOGIC;
  signal buff1_reg_n_119 : STD_LOGIC;
  signal buff1_reg_n_120 : STD_LOGIC;
  signal buff1_reg_n_121 : STD_LOGIC;
  signal buff1_reg_n_122 : STD_LOGIC;
  signal buff1_reg_n_123 : STD_LOGIC;
  signal buff1_reg_n_124 : STD_LOGIC;
  signal buff1_reg_n_125 : STD_LOGIC;
  signal buff1_reg_n_126 : STD_LOGIC;
  signal buff1_reg_n_127 : STD_LOGIC;
  signal buff1_reg_n_128 : STD_LOGIC;
  signal buff1_reg_n_129 : STD_LOGIC;
  signal buff1_reg_n_130 : STD_LOGIC;
  signal buff1_reg_n_131 : STD_LOGIC;
  signal buff1_reg_n_132 : STD_LOGIC;
  signal buff1_reg_n_133 : STD_LOGIC;
  signal buff1_reg_n_134 : STD_LOGIC;
  signal buff1_reg_n_135 : STD_LOGIC;
  signal buff1_reg_n_136 : STD_LOGIC;
  signal buff1_reg_n_137 : STD_LOGIC;
  signal buff1_reg_n_138 : STD_LOGIC;
  signal buff1_reg_n_139 : STD_LOGIC;
  signal buff1_reg_n_140 : STD_LOGIC;
  signal buff1_reg_n_141 : STD_LOGIC;
  signal buff1_reg_n_142 : STD_LOGIC;
  signal buff1_reg_n_143 : STD_LOGIC;
  signal buff1_reg_n_144 : STD_LOGIC;
  signal buff1_reg_n_145 : STD_LOGIC;
  signal buff1_reg_n_146 : STD_LOGIC;
  signal buff1_reg_n_147 : STD_LOGIC;
  signal buff1_reg_n_148 : STD_LOGIC;
  signal buff1_reg_n_149 : STD_LOGIC;
  signal buff1_reg_n_150 : STD_LOGIC;
  signal buff1_reg_n_151 : STD_LOGIC;
  signal buff1_reg_n_152 : STD_LOGIC;
  signal buff1_reg_n_153 : STD_LOGIC;
  signal buff1_reg_n_154 : STD_LOGIC;
  signal \buff1_reg_n_1_[0]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[10]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[11]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[12]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[13]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[14]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[15]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[16]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[1]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[2]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[3]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[4]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[5]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[6]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[7]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[8]\ : STD_LOGIC;
  signal \buff1_reg_n_1_[9]\ : STD_LOGIC;
  signal \buff2_reg__0\ : STD_LOGIC_VECTOR ( 31 downto 0 );
  signal buff2_reg_n_59 : STD_LOGIC;
  signal buff2_reg_n_60 : STD_LOGIC;
  signal buff2_reg_n_61 : STD_LOGIC;
  signal buff2_reg_n_62 : STD_LOGIC;
  signal buff2_reg_n_63 : STD_LOGIC;
  signal buff2_reg_n_64 : STD_LOGIC;
  signal buff2_reg_n_65 : STD_LOGIC;
  signal buff2_reg_n_66 : STD_LOGIC;
  signal buff2_reg_n_67 : STD_LOGIC;
  signal buff2_reg_n_68 : STD_LOGIC;
  signal buff2_reg_n_69 : STD_LOGIC;
  signal buff2_reg_n_70 : STD_LOGIC;
  signal buff2_reg_n_71 : STD_LOGIC;
  signal buff2_reg_n_72 : STD_LOGIC;
  signal buff2_reg_n_73 : STD_LOGIC;
  signal buff2_reg_n_74 : STD_LOGIC;
  signal buff2_reg_n_75 : STD_LOGIC;
  signal buff2_reg_n_76 : STD_LOGIC;
  signal buff2_reg_n_77 : STD_LOGIC;
  signal buff2_reg_n_78 : STD_LOGIC;
  signal buff2_reg_n_79 : STD_LOGIC;
  signal buff2_reg_n_80 : STD_LOGIC;
  signal buff2_reg_n_81 : STD_LOGIC;
  signal buff2_reg_n_82 : STD_LOGIC;
  signal buff2_reg_n_83 : STD_LOGIC;
  signal buff2_reg_n_84 : STD_LOGIC;
  signal buff2_reg_n_85 : STD_LOGIC;
  signal buff2_reg_n_86 : STD_LOGIC;
  signal buff2_reg_n_87 : STD_LOGIC;
  signal buff2_reg_n_88 : STD_LOGIC;
  signal buff2_reg_n_89 : STD_LOGIC;
  signal buff2_reg_n_90 : STD_LOGIC;
  signal buff2_reg_n_91 : STD_LOGIC;
  signal NLW_buff0_reg_CARRYCASCOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_MULTSIGNOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_OVERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_PATTERNBDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_PATTERNDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_UNDERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff0_reg_ACOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 29 downto 0 );
  signal NLW_buff0_reg_BCOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 17 downto 0 );
  signal NLW_buff0_reg_CARRYOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_buff1_reg_CARRYCASCOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_MULTSIGNOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_OVERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_PATTERNBDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_PATTERNDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_UNDERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff1_reg_ACOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 29 downto 0 );
  signal NLW_buff1_reg_BCOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 17 downto 0 );
  signal NLW_buff1_reg_CARRYOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_buff1_reg_P_UNCONNECTED : STD_LOGIC_VECTOR ( 47 downto 0 );
  signal \NLW_buff1_reg_i_1__2_CO_UNCONNECTED\ : STD_LOGIC_VECTOR ( 3 to 3 );
  signal NLW_buff2_reg_CARRYCASCOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_MULTSIGNOUT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_OVERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_PATTERNBDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_PATTERNDETECT_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_UNDERFLOW_UNCONNECTED : STD_LOGIC;
  signal NLW_buff2_reg_ACOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 29 downto 0 );
  signal NLW_buff2_reg_BCOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 17 downto 0 );
  signal NLW_buff2_reg_CARRYOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal NLW_buff2_reg_PCOUT_UNCONNECTED : STD_LOGIC_VECTOR ( 47 downto 0 );
begin
buff0_reg: unisim.vcomponents.DSP48E1
    generic map(
      ACASCREG => 2,
      ADREG => 1,
      ALUMODEREG => 0,
      AREG => 2,
      AUTORESET_PATDET => "NO_RESET",
      A_INPUT => "DIRECT",
      BCASCREG => 1,
      BREG => 1,
      B_INPUT => "DIRECT",
      CARRYINREG => 0,
      CARRYINSELREG => 0,
      CREG => 0,
      DREG => 1,
      INMODEREG => 0,
      MASK => X"3FFFFFFFFFFF",
      MREG => 0,
      OPMODEREG => 0,
      PATTERN => X"000000000000",
      PREG => 1,
      SEL_MASK => "MASK",
      SEL_PATTERN => "PATTERN",
      USE_DPORT => false,
      USE_MULT => "MULTIPLY",
      USE_PATTERN_DETECT => "NO_PATDET",
      USE_SIMD => "ONE48"
    )
        port map (
      A(29 downto 17) => B"0000000000000",
      A(16 downto 0) => A_q0(16 downto 0),
      ACIN(29 downto 0) => B"000000000000000000000000000000",
      ACOUT(29 downto 0) => NLW_buff0_reg_ACOUT_UNCONNECTED(29 downto 0),
      ALUMODE(3 downto 0) => B"0000",
      B(17) => '0',
      B(16 downto 1) => grp_fu_146_p1(15 downto 0),
      B(0) => A(0),
      BCIN(17 downto 0) => B"000000000000000000",
      BCOUT(17 downto 0) => NLW_buff0_reg_BCOUT_UNCONNECTED(17 downto 0),
      C(47 downto 0) => B"111111111111111111111111111111111111111111111111",
      CARRYCASCIN => '0',
      CARRYCASCOUT => NLW_buff0_reg_CARRYCASCOUT_UNCONNECTED,
      CARRYIN => '0',
      CARRYINSEL(2 downto 0) => B"000",
      CARRYOUT(3 downto 0) => NLW_buff0_reg_CARRYOUT_UNCONNECTED(3 downto 0),
      CEA1 => Q(0),
      CEA2 => '1',
      CEAD => '0',
      CEALUMODE => '0',
      CEB1 => '0',
      CEB2 => '1',
      CEC => '0',
      CECARRYIN => '0',
      CECTRL => '0',
      CED => '0',
      CEINMODE => '0',
      CEM => '0',
      CEP => '1',
      CLK => ap_clk,
      D(24 downto 0) => B"0000000000000000000000000",
      INMODE(4 downto 0) => B"00000",
      MULTSIGNIN => '0',
      MULTSIGNOUT => NLW_buff0_reg_MULTSIGNOUT_UNCONNECTED,
      OPMODE(6 downto 0) => B"0000101",
      OVERFLOW => NLW_buff0_reg_OVERFLOW_UNCONNECTED,
      P(47) => buff0_reg_n_59,
      P(46) => buff0_reg_n_60,
      P(45) => buff0_reg_n_61,
      P(44) => buff0_reg_n_62,
      P(43) => buff0_reg_n_63,
      P(42) => buff0_reg_n_64,
      P(41) => buff0_reg_n_65,
      P(40) => buff0_reg_n_66,
      P(39) => buff0_reg_n_67,
      P(38) => buff0_reg_n_68,
      P(37) => buff0_reg_n_69,
      P(36) => buff0_reg_n_70,
      P(35) => buff0_reg_n_71,
      P(34) => buff0_reg_n_72,
      P(33) => buff0_reg_n_73,
      P(32) => buff0_reg_n_74,
      P(31) => buff0_reg_n_75,
      P(30) => buff0_reg_n_76,
      P(29) => buff0_reg_n_77,
      P(28) => buff0_reg_n_78,
      P(27) => buff0_reg_n_79,
      P(26) => buff0_reg_n_80,
      P(25) => buff0_reg_n_81,
      P(24) => buff0_reg_n_82,
      P(23) => buff0_reg_n_83,
      P(22) => buff0_reg_n_84,
      P(21) => buff0_reg_n_85,
      P(20) => buff0_reg_n_86,
      P(19) => buff0_reg_n_87,
      P(18) => buff0_reg_n_88,
      P(17) => buff0_reg_n_89,
      P(16) => buff0_reg_n_90,
      P(15) => buff0_reg_n_91,
      P(14) => buff0_reg_n_92,
      P(13) => buff0_reg_n_93,
      P(12) => buff0_reg_n_94,
      P(11) => buff0_reg_n_95,
      P(10) => buff0_reg_n_96,
      P(9) => buff0_reg_n_97,
      P(8) => buff0_reg_n_98,
      P(7) => buff0_reg_n_99,
      P(6) => buff0_reg_n_100,
      P(5) => buff0_reg_n_101,
      P(4) => buff0_reg_n_102,
      P(3) => buff0_reg_n_103,
      P(2) => buff0_reg_n_104,
      P(1) => buff0_reg_n_105,
      P(0) => buff0_reg_n_106,
      PATTERNBDETECT => NLW_buff0_reg_PATTERNBDETECT_UNCONNECTED,
      PATTERNDETECT => NLW_buff0_reg_PATTERNDETECT_UNCONNECTED,
      PCIN(47 downto 0) => B"000000000000000000000000000000000000000000000000",
      PCOUT(47) => buff0_reg_n_107,
      PCOUT(46) => buff0_reg_n_108,
      PCOUT(45) => buff0_reg_n_109,
      PCOUT(44) => buff0_reg_n_110,
      PCOUT(43) => buff0_reg_n_111,
      PCOUT(42) => buff0_reg_n_112,
      PCOUT(41) => buff0_reg_n_113,
      PCOUT(40) => buff0_reg_n_114,
      PCOUT(39) => buff0_reg_n_115,
      PCOUT(38) => buff0_reg_n_116,
      PCOUT(37) => buff0_reg_n_117,
      PCOUT(36) => buff0_reg_n_118,
      PCOUT(35) => buff0_reg_n_119,
      PCOUT(34) => buff0_reg_n_120,
      PCOUT(33) => buff0_reg_n_121,
      PCOUT(32) => buff0_reg_n_122,
      PCOUT(31) => buff0_reg_n_123,
      PCOUT(30) => buff0_reg_n_124,
      PCOUT(29) => buff0_reg_n_125,
      PCOUT(28) => buff0_reg_n_126,
      PCOUT(27) => buff0_reg_n_127,
      PCOUT(26) => buff0_reg_n_128,
      PCOUT(25) => buff0_reg_n_129,
      PCOUT(24) => buff0_reg_n_130,
      PCOUT(23) => buff0_reg_n_131,
      PCOUT(22) => buff0_reg_n_132,
      PCOUT(21) => buff0_reg_n_133,
      PCOUT(20) => buff0_reg_n_134,
      PCOUT(19) => buff0_reg_n_135,
      PCOUT(18) => buff0_reg_n_136,
      PCOUT(17) => buff0_reg_n_137,
      PCOUT(16) => buff0_reg_n_138,
      PCOUT(15) => buff0_reg_n_139,
      PCOUT(14) => buff0_reg_n_140,
      PCOUT(13) => buff0_reg_n_141,
      PCOUT(12) => buff0_reg_n_142,
      PCOUT(11) => buff0_reg_n_143,
      PCOUT(10) => buff0_reg_n_144,
      PCOUT(9) => buff0_reg_n_145,
      PCOUT(8) => buff0_reg_n_146,
      PCOUT(7) => buff0_reg_n_147,
      PCOUT(6) => buff0_reg_n_148,
      PCOUT(5) => buff0_reg_n_149,
      PCOUT(4) => buff0_reg_n_150,
      PCOUT(3) => buff0_reg_n_151,
      PCOUT(2) => buff0_reg_n_152,
      PCOUT(1) => buff0_reg_n_153,
      PCOUT(0) => buff0_reg_n_154,
      RSTA => '0',
      RSTALLCARRYIN => '0',
      RSTALUMODE => '0',
      RSTB => '0',
      RSTC => '0',
      RSTCTRL => '0',
      RSTD => '0',
      RSTINMODE => '0',
      RSTM => '0',
      RSTP => '0',
      UNDERFLOW => NLW_buff0_reg_UNDERFLOW_UNCONNECTED
    );
\buff0_reg_i_1__2\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff0_reg_i_2__2_n_1\,
      CO(3) => \buff0_reg_i_1__2_n_1\,
      CO(2) => \buff0_reg_i_1__2_n_2\,
      CO(1) => \buff0_reg_i_1__2_n_3\,
      CO(0) => \buff0_reg_i_1__2_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_157_p1(19 downto 16),
      S(3 downto 0) => \buff2_reg__0\(19 downto 16)
    );
\buff0_reg_i_2__2\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff0_reg_i_3__2_n_1\,
      CO(3) => \buff0_reg_i_2__2_n_1\,
      CO(2) => \buff0_reg_i_2__2_n_2\,
      CO(1) => \buff0_reg_i_2__2_n_3\,
      CO(0) => \buff0_reg_i_2__2_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_157_p1(15 downto 12),
      S(3 downto 0) => \buff2_reg__0\(15 downto 12)
    );
\buff0_reg_i_3__2\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff0_reg_i_4__2_n_1\,
      CO(3) => \buff0_reg_i_3__2_n_1\,
      CO(2) => \buff0_reg_i_3__2_n_2\,
      CO(1) => \buff0_reg_i_3__2_n_3\,
      CO(0) => \buff0_reg_i_3__2_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_157_p1(11 downto 8),
      S(3 downto 0) => \buff2_reg__0\(11 downto 8)
    );
\buff0_reg_i_4__2\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff0_reg_i_5__2_n_1\,
      CO(3) => \buff0_reg_i_4__2_n_1\,
      CO(2) => \buff0_reg_i_4__2_n_2\,
      CO(1) => \buff0_reg_i_4__2_n_3\,
      CO(0) => \buff0_reg_i_4__2_n_4\,
      CYINIT => '0',
      DI(3) => '0',
      DI(2) => \buff2_reg__0\(6),
      DI(1) => '0',
      DI(0) => \buff2_reg__0\(4),
      O(3 downto 0) => grp_fu_157_p1(7 downto 4),
      S(3) => \buff2_reg__0\(7),
      S(2) => \buff0_reg_i_6__2_n_1\,
      S(1) => \buff2_reg__0\(5),
      S(0) => \buff0_reg_i_7__1_n_1\
    );
\buff0_reg_i_5__2\: unisim.vcomponents.CARRY4
     port map (
      CI => '0',
      CO(3) => \buff0_reg_i_5__2_n_1\,
      CO(2) => \buff0_reg_i_5__2_n_2\,
      CO(1) => \buff0_reg_i_5__2_n_3\,
      CO(0) => \buff0_reg_i_5__2_n_4\,
      CYINIT => '0',
      DI(3 downto 2) => B"00",
      DI(1) => \buff2_reg__0\(1),
      DI(0) => '0',
      O(3 downto 0) => grp_fu_157_p1(3 downto 0),
      S(3 downto 2) => \buff2_reg__0\(3 downto 2),
      S(1) => \buff0_reg_i_8__0_n_1\,
      S(0) => \buff2_reg__0\(0)
    );
\buff0_reg_i_6__2\: unisim.vcomponents.LUT1
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => \buff2_reg__0\(6),
      O => \buff0_reg_i_6__2_n_1\
    );
\buff0_reg_i_7__1\: unisim.vcomponents.LUT1
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => \buff2_reg__0\(4),
      O => \buff0_reg_i_7__1_n_1\
    );
\buff0_reg_i_8__0\: unisim.vcomponents.LUT1
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => \buff2_reg__0\(1),
      O => \buff0_reg_i_8__0_n_1\
    );
buff1_reg: unisim.vcomponents.DSP48E1
    generic map(
      ACASCREG => 2,
      ADREG => 1,
      ALUMODEREG => 0,
      AREG => 2,
      AUTORESET_PATDET => "NO_RESET",
      A_INPUT => "DIRECT",
      BCASCREG => 1,
      BREG => 1,
      B_INPUT => "DIRECT",
      CARRYINREG => 0,
      CARRYINSELREG => 0,
      CREG => 1,
      DREG => 1,
      INMODEREG => 0,
      MASK => X"3FFFFFFFFFFF",
      MREG => 1,
      OPMODEREG => 0,
      PATTERN => X"000000000000",
      PREG => 1,
      SEL_MASK => "MASK",
      SEL_PATTERN => "PATTERN",
      USE_DPORT => false,
      USE_MULT => "MULTIPLY",
      USE_PATTERN_DETECT => "NO_PATDET",
      USE_SIMD => "ONE48"
    )
        port map (
      A(29 downto 17) => B"0000000000000",
      A(16 downto 0) => A_q0(16 downto 0),
      ACIN(29 downto 0) => B"000000000000000000000000000000",
      ACOUT(29 downto 0) => NLW_buff1_reg_ACOUT_UNCONNECTED(29 downto 0),
      ALUMODE(3 downto 0) => B"0000",
      B(17) => grp_fu_146_p1(30),
      B(16) => grp_fu_146_p1(30),
      B(15) => grp_fu_146_p1(30),
      B(14 downto 0) => grp_fu_146_p1(30 downto 16),
      BCIN(17 downto 0) => B"000000000000000000",
      BCOUT(17 downto 0) => NLW_buff1_reg_BCOUT_UNCONNECTED(17 downto 0),
      C(47 downto 0) => B"111111111111111111111111111111111111111111111111",
      CARRYCASCIN => '0',
      CARRYCASCOUT => NLW_buff1_reg_CARRYCASCOUT_UNCONNECTED,
      CARRYIN => '0',
      CARRYINSEL(2 downto 0) => B"000",
      CARRYOUT(3 downto 0) => NLW_buff1_reg_CARRYOUT_UNCONNECTED(3 downto 0),
      CEA1 => Q(0),
      CEA2 => '1',
      CEAD => '0',
      CEALUMODE => '0',
      CEB1 => '0',
      CEB2 => '1',
      CEC => '0',
      CECARRYIN => '0',
      CECTRL => '0',
      CED => '0',
      CEINMODE => '0',
      CEM => '1',
      CEP => '1',
      CLK => ap_clk,
      D(24 downto 0) => B"0000000000000000000000000",
      INMODE(4 downto 0) => B"00000",
      MULTSIGNIN => '0',
      MULTSIGNOUT => NLW_buff1_reg_MULTSIGNOUT_UNCONNECTED,
      OPMODE(6 downto 0) => B"1010101",
      OVERFLOW => NLW_buff1_reg_OVERFLOW_UNCONNECTED,
      P(47 downto 0) => NLW_buff1_reg_P_UNCONNECTED(47 downto 0),
      PATTERNBDETECT => NLW_buff1_reg_PATTERNBDETECT_UNCONNECTED,
      PATTERNDETECT => NLW_buff1_reg_PATTERNDETECT_UNCONNECTED,
      PCIN(47) => buff0_reg_n_107,
      PCIN(46) => buff0_reg_n_108,
      PCIN(45) => buff0_reg_n_109,
      PCIN(44) => buff0_reg_n_110,
      PCIN(43) => buff0_reg_n_111,
      PCIN(42) => buff0_reg_n_112,
      PCIN(41) => buff0_reg_n_113,
      PCIN(40) => buff0_reg_n_114,
      PCIN(39) => buff0_reg_n_115,
      PCIN(38) => buff0_reg_n_116,
      PCIN(37) => buff0_reg_n_117,
      PCIN(36) => buff0_reg_n_118,
      PCIN(35) => buff0_reg_n_119,
      PCIN(34) => buff0_reg_n_120,
      PCIN(33) => buff0_reg_n_121,
      PCIN(32) => buff0_reg_n_122,
      PCIN(31) => buff0_reg_n_123,
      PCIN(30) => buff0_reg_n_124,
      PCIN(29) => buff0_reg_n_125,
      PCIN(28) => buff0_reg_n_126,
      PCIN(27) => buff0_reg_n_127,
      PCIN(26) => buff0_reg_n_128,
      PCIN(25) => buff0_reg_n_129,
      PCIN(24) => buff0_reg_n_130,
      PCIN(23) => buff0_reg_n_131,
      PCIN(22) => buff0_reg_n_132,
      PCIN(21) => buff0_reg_n_133,
      PCIN(20) => buff0_reg_n_134,
      PCIN(19) => buff0_reg_n_135,
      PCIN(18) => buff0_reg_n_136,
      PCIN(17) => buff0_reg_n_137,
      PCIN(16) => buff0_reg_n_138,
      PCIN(15) => buff0_reg_n_139,
      PCIN(14) => buff0_reg_n_140,
      PCIN(13) => buff0_reg_n_141,
      PCIN(12) => buff0_reg_n_142,
      PCIN(11) => buff0_reg_n_143,
      PCIN(10) => buff0_reg_n_144,
      PCIN(9) => buff0_reg_n_145,
      PCIN(8) => buff0_reg_n_146,
      PCIN(7) => buff0_reg_n_147,
      PCIN(6) => buff0_reg_n_148,
      PCIN(5) => buff0_reg_n_149,
      PCIN(4) => buff0_reg_n_150,
      PCIN(3) => buff0_reg_n_151,
      PCIN(2) => buff0_reg_n_152,
      PCIN(1) => buff0_reg_n_153,
      PCIN(0) => buff0_reg_n_154,
      PCOUT(47) => buff1_reg_n_107,
      PCOUT(46) => buff1_reg_n_108,
      PCOUT(45) => buff1_reg_n_109,
      PCOUT(44) => buff1_reg_n_110,
      PCOUT(43) => buff1_reg_n_111,
      PCOUT(42) => buff1_reg_n_112,
      PCOUT(41) => buff1_reg_n_113,
      PCOUT(40) => buff1_reg_n_114,
      PCOUT(39) => buff1_reg_n_115,
      PCOUT(38) => buff1_reg_n_116,
      PCOUT(37) => buff1_reg_n_117,
      PCOUT(36) => buff1_reg_n_118,
      PCOUT(35) => buff1_reg_n_119,
      PCOUT(34) => buff1_reg_n_120,
      PCOUT(33) => buff1_reg_n_121,
      PCOUT(32) => buff1_reg_n_122,
      PCOUT(31) => buff1_reg_n_123,
      PCOUT(30) => buff1_reg_n_124,
      PCOUT(29) => buff1_reg_n_125,
      PCOUT(28) => buff1_reg_n_126,
      PCOUT(27) => buff1_reg_n_127,
      PCOUT(26) => buff1_reg_n_128,
      PCOUT(25) => buff1_reg_n_129,
      PCOUT(24) => buff1_reg_n_130,
      PCOUT(23) => buff1_reg_n_131,
      PCOUT(22) => buff1_reg_n_132,
      PCOUT(21) => buff1_reg_n_133,
      PCOUT(20) => buff1_reg_n_134,
      PCOUT(19) => buff1_reg_n_135,
      PCOUT(18) => buff1_reg_n_136,
      PCOUT(17) => buff1_reg_n_137,
      PCOUT(16) => buff1_reg_n_138,
      PCOUT(15) => buff1_reg_n_139,
      PCOUT(14) => buff1_reg_n_140,
      PCOUT(13) => buff1_reg_n_141,
      PCOUT(12) => buff1_reg_n_142,
      PCOUT(11) => buff1_reg_n_143,
      PCOUT(10) => buff1_reg_n_144,
      PCOUT(9) => buff1_reg_n_145,
      PCOUT(8) => buff1_reg_n_146,
      PCOUT(7) => buff1_reg_n_147,
      PCOUT(6) => buff1_reg_n_148,
      PCOUT(5) => buff1_reg_n_149,
      PCOUT(4) => buff1_reg_n_150,
      PCOUT(3) => buff1_reg_n_151,
      PCOUT(2) => buff1_reg_n_152,
      PCOUT(1) => buff1_reg_n_153,
      PCOUT(0) => buff1_reg_n_154,
      RSTA => '0',
      RSTALLCARRYIN => '0',
      RSTALUMODE => '0',
      RSTB => '0',
      RSTC => '0',
      RSTCTRL => '0',
      RSTD => '0',
      RSTINMODE => '0',
      RSTM => '0',
      RSTP => '0',
      UNDERFLOW => NLW_buff1_reg_UNDERFLOW_UNCONNECTED
    );
\buff1_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_106,
      Q => \buff1_reg_n_1_[0]\,
      R => '0'
    );
\buff1_reg[10]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_96,
      Q => \buff1_reg_n_1_[10]\,
      R => '0'
    );
\buff1_reg[11]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_95,
      Q => \buff1_reg_n_1_[11]\,
      R => '0'
    );
\buff1_reg[12]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_94,
      Q => \buff1_reg_n_1_[12]\,
      R => '0'
    );
\buff1_reg[13]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_93,
      Q => \buff1_reg_n_1_[13]\,
      R => '0'
    );
\buff1_reg[14]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_92,
      Q => \buff1_reg_n_1_[14]\,
      R => '0'
    );
\buff1_reg[15]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_91,
      Q => \buff1_reg_n_1_[15]\,
      R => '0'
    );
\buff1_reg[16]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_90,
      Q => \buff1_reg_n_1_[16]\,
      R => '0'
    );
\buff1_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_105,
      Q => \buff1_reg_n_1_[1]\,
      R => '0'
    );
\buff1_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_104,
      Q => \buff1_reg_n_1_[2]\,
      R => '0'
    );
\buff1_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_103,
      Q => \buff1_reg_n_1_[3]\,
      R => '0'
    );
\buff1_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_102,
      Q => \buff1_reg_n_1_[4]\,
      R => '0'
    );
\buff1_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_101,
      Q => \buff1_reg_n_1_[5]\,
      R => '0'
    );
\buff1_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_100,
      Q => \buff1_reg_n_1_[6]\,
      R => '0'
    );
\buff1_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_99,
      Q => \buff1_reg_n_1_[7]\,
      R => '0'
    );
\buff1_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_98,
      Q => \buff1_reg_n_1_[8]\,
      R => '0'
    );
\buff1_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => buff0_reg_n_97,
      Q => \buff1_reg_n_1_[9]\,
      R => '0'
    );
\buff1_reg_i_1__2\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff1_reg_i_2__2_n_1\,
      CO(3) => \NLW_buff1_reg_i_1__2_CO_UNCONNECTED\(3),
      CO(2) => \buff1_reg_i_1__2_n_2\,
      CO(1) => \buff1_reg_i_1__2_n_3\,
      CO(0) => \buff1_reg_i_1__2_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_157_p1(31 downto 28),
      S(3 downto 0) => \buff2_reg__0\(31 downto 28)
    );
\buff1_reg_i_2__2\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff1_reg_i_3__2_n_1\,
      CO(3) => \buff1_reg_i_2__2_n_1\,
      CO(2) => \buff1_reg_i_2__2_n_2\,
      CO(1) => \buff1_reg_i_2__2_n_3\,
      CO(0) => \buff1_reg_i_2__2_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_157_p1(27 downto 24),
      S(3 downto 0) => \buff2_reg__0\(27 downto 24)
    );
\buff1_reg_i_3__2\: unisim.vcomponents.CARRY4
     port map (
      CI => \buff0_reg_i_1__2_n_1\,
      CO(3) => \buff1_reg_i_3__2_n_1\,
      CO(2) => \buff1_reg_i_3__2_n_2\,
      CO(1) => \buff1_reg_i_3__2_n_3\,
      CO(0) => \buff1_reg_i_3__2_n_4\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => grp_fu_157_p1(23 downto 20),
      S(3 downto 0) => \buff2_reg__0\(23 downto 20)
    );
buff2_reg: unisim.vcomponents.DSP48E1
    generic map(
      ACASCREG => 2,
      ADREG => 1,
      ALUMODEREG => 0,
      AREG => 2,
      AUTORESET_PATDET => "NO_RESET",
      A_INPUT => "DIRECT",
      BCASCREG => 2,
      BREG => 2,
      B_INPUT => "DIRECT",
      CARRYINREG => 0,
      CARRYINSELREG => 0,
      CREG => 1,
      DREG => 1,
      INMODEREG => 0,
      MASK => X"3FFFFFFFFFFF",
      MREG => 1,
      OPMODEREG => 0,
      PATTERN => X"000000000000",
      PREG => 1,
      SEL_MASK => "MASK",
      SEL_PATTERN => "PATTERN",
      USE_DPORT => false,
      USE_MULT => "MULTIPLY",
      USE_PATTERN_DETECT => "NO_PATDET",
      USE_SIMD => "ONE48"
    )
        port map (
      A(29 downto 17) => B"0000000000000",
      A(16 downto 1) => grp_fu_146_p1(15 downto 0),
      A(0) => A(0),
      ACIN(29 downto 0) => B"000000000000000000000000000000",
      ACOUT(29 downto 0) => NLW_buff2_reg_ACOUT_UNCONNECTED(29 downto 0),
      ALUMODE(3 downto 0) => B"0000",
      B(17) => buff2_reg_0(14),
      B(16) => buff2_reg_0(14),
      B(15) => buff2_reg_0(14),
      B(14 downto 0) => buff2_reg_0(14 downto 0),
      BCIN(17 downto 0) => B"000000000000000000",
      BCOUT(17 downto 0) => NLW_buff2_reg_BCOUT_UNCONNECTED(17 downto 0),
      C(47 downto 0) => B"111111111111111111111111111111111111111111111111",
      CARRYCASCIN => '0',
      CARRYCASCOUT => NLW_buff2_reg_CARRYCASCOUT_UNCONNECTED,
      CARRYIN => '0',
      CARRYINSEL(2 downto 0) => B"000",
      CARRYOUT(3 downto 0) => NLW_buff2_reg_CARRYOUT_UNCONNECTED(3 downto 0),
      CEA1 => '1',
      CEA2 => '1',
      CEAD => '0',
      CEALUMODE => '0',
      CEB1 => '1',
      CEB2 => '1',
      CEC => '0',
      CECARRYIN => '0',
      CECTRL => '0',
      CED => '0',
      CEINMODE => '0',
      CEM => '1',
      CEP => '1',
      CLK => ap_clk,
      D(24 downto 0) => B"0000000000000000000000000",
      INMODE(4 downto 0) => B"00000",
      MULTSIGNIN => '0',
      MULTSIGNOUT => NLW_buff2_reg_MULTSIGNOUT_UNCONNECTED,
      OPMODE(6 downto 0) => B"0010101",
      OVERFLOW => NLW_buff2_reg_OVERFLOW_UNCONNECTED,
      P(47) => buff2_reg_n_59,
      P(46) => buff2_reg_n_60,
      P(45) => buff2_reg_n_61,
      P(44) => buff2_reg_n_62,
      P(43) => buff2_reg_n_63,
      P(42) => buff2_reg_n_64,
      P(41) => buff2_reg_n_65,
      P(40) => buff2_reg_n_66,
      P(39) => buff2_reg_n_67,
      P(38) => buff2_reg_n_68,
      P(37) => buff2_reg_n_69,
      P(36) => buff2_reg_n_70,
      P(35) => buff2_reg_n_71,
      P(34) => buff2_reg_n_72,
      P(33) => buff2_reg_n_73,
      P(32) => buff2_reg_n_74,
      P(31) => buff2_reg_n_75,
      P(30) => buff2_reg_n_76,
      P(29) => buff2_reg_n_77,
      P(28) => buff2_reg_n_78,
      P(27) => buff2_reg_n_79,
      P(26) => buff2_reg_n_80,
      P(25) => buff2_reg_n_81,
      P(24) => buff2_reg_n_82,
      P(23) => buff2_reg_n_83,
      P(22) => buff2_reg_n_84,
      P(21) => buff2_reg_n_85,
      P(20) => buff2_reg_n_86,
      P(19) => buff2_reg_n_87,
      P(18) => buff2_reg_n_88,
      P(17) => buff2_reg_n_89,
      P(16) => buff2_reg_n_90,
      P(15) => buff2_reg_n_91,
      P(14 downto 0) => \buff2_reg__0\(31 downto 17),
      PATTERNBDETECT => NLW_buff2_reg_PATTERNBDETECT_UNCONNECTED,
      PATTERNDETECT => NLW_buff2_reg_PATTERNDETECT_UNCONNECTED,
      PCIN(47) => buff1_reg_n_107,
      PCIN(46) => buff1_reg_n_108,
      PCIN(45) => buff1_reg_n_109,
      PCIN(44) => buff1_reg_n_110,
      PCIN(43) => buff1_reg_n_111,
      PCIN(42) => buff1_reg_n_112,
      PCIN(41) => buff1_reg_n_113,
      PCIN(40) => buff1_reg_n_114,
      PCIN(39) => buff1_reg_n_115,
      PCIN(38) => buff1_reg_n_116,
      PCIN(37) => buff1_reg_n_117,
      PCIN(36) => buff1_reg_n_118,
      PCIN(35) => buff1_reg_n_119,
      PCIN(34) => buff1_reg_n_120,
      PCIN(33) => buff1_reg_n_121,
      PCIN(32) => buff1_reg_n_122,
      PCIN(31) => buff1_reg_n_123,
      PCIN(30) => buff1_reg_n_124,
      PCIN(29) => buff1_reg_n_125,
      PCIN(28) => buff1_reg_n_126,
      PCIN(27) => buff1_reg_n_127,
      PCIN(26) => buff1_reg_n_128,
      PCIN(25) => buff1_reg_n_129,
      PCIN(24) => buff1_reg_n_130,
      PCIN(23) => buff1_reg_n_131,
      PCIN(22) => buff1_reg_n_132,
      PCIN(21) => buff1_reg_n_133,
      PCIN(20) => buff1_reg_n_134,
      PCIN(19) => buff1_reg_n_135,
      PCIN(18) => buff1_reg_n_136,
      PCIN(17) => buff1_reg_n_137,
      PCIN(16) => buff1_reg_n_138,
      PCIN(15) => buff1_reg_n_139,
      PCIN(14) => buff1_reg_n_140,
      PCIN(13) => buff1_reg_n_141,
      PCIN(12) => buff1_reg_n_142,
      PCIN(11) => buff1_reg_n_143,
      PCIN(10) => buff1_reg_n_144,
      PCIN(9) => buff1_reg_n_145,
      PCIN(8) => buff1_reg_n_146,
      PCIN(7) => buff1_reg_n_147,
      PCIN(6) => buff1_reg_n_148,
      PCIN(5) => buff1_reg_n_149,
      PCIN(4) => buff1_reg_n_150,
      PCIN(3) => buff1_reg_n_151,
      PCIN(2) => buff1_reg_n_152,
      PCIN(1) => buff1_reg_n_153,
      PCIN(0) => buff1_reg_n_154,
      PCOUT(47 downto 0) => NLW_buff2_reg_PCOUT_UNCONNECTED(47 downto 0),
      RSTA => '0',
      RSTALLCARRYIN => '0',
      RSTALUMODE => '0',
      RSTB => '0',
      RSTC => '0',
      RSTCTRL => '0',
      RSTD => '0',
      RSTINMODE => '0',
      RSTM => '0',
      RSTP => '0',
      UNDERFLOW => NLW_buff2_reg_UNDERFLOW_UNCONNECTED
    );
\buff2_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[0]\,
      Q => \buff2_reg__0\(0),
      R => '0'
    );
\buff2_reg[10]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[10]\,
      Q => \buff2_reg__0\(10),
      R => '0'
    );
\buff2_reg[11]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[11]\,
      Q => \buff2_reg__0\(11),
      R => '0'
    );
\buff2_reg[12]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[12]\,
      Q => \buff2_reg__0\(12),
      R => '0'
    );
\buff2_reg[13]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[13]\,
      Q => \buff2_reg__0\(13),
      R => '0'
    );
\buff2_reg[14]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[14]\,
      Q => \buff2_reg__0\(14),
      R => '0'
    );
\buff2_reg[15]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[15]\,
      Q => \buff2_reg__0\(15),
      R => '0'
    );
\buff2_reg[16]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[16]\,
      Q => \buff2_reg__0\(16),
      R => '0'
    );
\buff2_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[1]\,
      Q => \buff2_reg__0\(1),
      R => '0'
    );
\buff2_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[2]\,
      Q => \buff2_reg__0\(2),
      R => '0'
    );
\buff2_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[3]\,
      Q => \buff2_reg__0\(3),
      R => '0'
    );
\buff2_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[4]\,
      Q => \buff2_reg__0\(4),
      R => '0'
    );
\buff2_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[5]\,
      Q => \buff2_reg__0\(5),
      R => '0'
    );
\buff2_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[6]\,
      Q => \buff2_reg__0\(6),
      R => '0'
    );
\buff2_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[7]\,
      Q => \buff2_reg__0\(7),
      R => '0'
    );
\buff2_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[8]\,
      Q => \buff2_reg__0\(8),
      R => '0'
    );
\buff2_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => '1',
      D => \buff1_reg_n_1_[9]\,
      Q => \buff2_reg__0\(9),
      R => '0'
    );
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb is
  port (
    grp_fu_135_p1 : out STD_LOGIC_VECTOR ( 31 downto 0 );
    ap_clk : in STD_LOGIC;
    A_q0 : in STD_LOGIC_VECTOR ( 31 downto 0 )
  );
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb is
begin
loop_imperfect_mubkb_MulnS_0_U: entity work.decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_MulnS_0_11
     port map (
      A_q0(31 downto 0) => A_q0(31 downto 0),
      ap_clk => ap_clk,
      grp_fu_135_p1(31 downto 0) => grp_fu_135_p1(31 downto 0)
    );
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_0 is
  port (
    A : out STD_LOGIC_VECTOR ( 16 downto 0 );
    grp_fu_146_p1 : out STD_LOGIC_VECTOR ( 14 downto 0 );
    Q : in STD_LOGIC_VECTOR ( 0 to 0 );
    ap_clk : in STD_LOGIC;
    grp_fu_135_p1 : in STD_LOGIC_VECTOR ( 31 downto 0 );
    A_q0 : in STD_LOGIC_VECTOR ( 16 downto 0 );
    buff2_reg : in STD_LOGIC_VECTOR ( 14 downto 0 )
  );
  attribute ORIG_REF_NAME : string;
  attribute ORIG_REF_NAME of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_0 : entity is "loop_imperfect_mubkb";
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_0;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_0 is
begin
loop_imperfect_mubkb_MulnS_0_U: entity work.decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_MulnS_0_10
     port map (
      A(16 downto 0) => A(16 downto 0),
      A_q0(16 downto 0) => A_q0(16 downto 0),
      Q(0) => Q(0),
      ap_clk => ap_clk,
      buff2_reg_0(14 downto 0) => buff2_reg(14 downto 0),
      grp_fu_135_p1(31 downto 0) => grp_fu_135_p1(31 downto 0),
      grp_fu_146_p1(14 downto 0) => grp_fu_146_p1(14 downto 0)
    );
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_1 is
  port (
    grp_fu_157_p1 : out STD_LOGIC_VECTOR ( 31 downto 0 );
    Q : in STD_LOGIC_VECTOR ( 0 to 0 );
    ap_clk : in STD_LOGIC;
    grp_fu_146_p1 : in STD_LOGIC_VECTOR ( 30 downto 0 );
    A : in STD_LOGIC_VECTOR ( 0 to 0 );
    A_q0 : in STD_LOGIC_VECTOR ( 16 downto 0 );
    buff2_reg : in STD_LOGIC_VECTOR ( 14 downto 0 )
  );
  attribute ORIG_REF_NAME : string;
  attribute ORIG_REF_NAME of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_1 : entity is "loop_imperfect_mubkb";
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_1;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_1 is
begin
loop_imperfect_mubkb_MulnS_0_U: entity work.decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_MulnS_0_9
     port map (
      A(0) => A(0),
      A_q0(16 downto 0) => A_q0(16 downto 0),
      Q(0) => Q(0),
      ap_clk => ap_clk,
      buff2_reg_0(14 downto 0) => buff2_reg(14 downto 0),
      grp_fu_146_p1(30 downto 0) => grp_fu_146_p1(30 downto 0),
      grp_fu_157_p1(31 downto 0) => grp_fu_157_p1(31 downto 0)
    );
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_2 is
  port (
    grp_fu_168_p1 : out STD_LOGIC_VECTOR ( 31 downto 0 );
    Q : in STD_LOGIC_VECTOR ( 0 to 0 );
    ap_clk : in STD_LOGIC;
    grp_fu_157_p1 : in STD_LOGIC_VECTOR ( 31 downto 0 );
    A_q0 : in STD_LOGIC_VECTOR ( 16 downto 0 );
    buff2_reg : in STD_LOGIC_VECTOR ( 14 downto 0 )
  );
  attribute ORIG_REF_NAME : string;
  attribute ORIG_REF_NAME of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_2 : entity is "loop_imperfect_mubkb";
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_2;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_2 is
begin
loop_imperfect_mubkb_MulnS_0_U: entity work.decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_MulnS_0_8
     port map (
      A_q0(16 downto 0) => A_q0(16 downto 0),
      Q(0) => Q(0),
      ap_clk => ap_clk,
      buff2_reg_0(14 downto 0) => buff2_reg(14 downto 0),
      grp_fu_157_p1(31 downto 0) => grp_fu_157_p1(31 downto 0),
      grp_fu_168_p1(31 downto 0) => grp_fu_168_p1(31 downto 0)
    );
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_3 is
  port (
    grp_fu_179_p1 : out STD_LOGIC_VECTOR ( 31 downto 0 );
    Q : in STD_LOGIC_VECTOR ( 0 to 0 );
    ap_clk : in STD_LOGIC;
    grp_fu_168_p1 : in STD_LOGIC_VECTOR ( 31 downto 0 );
    A_q0 : in STD_LOGIC_VECTOR ( 16 downto 0 );
    buff2_reg : in STD_LOGIC_VECTOR ( 14 downto 0 )
  );
  attribute ORIG_REF_NAME : string;
  attribute ORIG_REF_NAME of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_3 : entity is "loop_imperfect_mubkb";
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_3;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_3 is
begin
loop_imperfect_mubkb_MulnS_0_U: entity work.decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_MulnS_0_7
     port map (
      A_q0(16 downto 0) => A_q0(16 downto 0),
      Q(0) => Q(0),
      ap_clk => ap_clk,
      buff2_reg_0(14 downto 0) => buff2_reg(14 downto 0),
      grp_fu_168_p1(31 downto 0) => grp_fu_168_p1(31 downto 0),
      grp_fu_179_p1(31 downto 0) => grp_fu_179_p1(31 downto 0)
    );
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_4 is
  port (
    A : out STD_LOGIC_VECTOR ( 16 downto 0 );
    grp_fu_190_p1 : out STD_LOGIC_VECTOR ( 14 downto 0 );
    Q : in STD_LOGIC_VECTOR ( 0 to 0 );
    ap_clk : in STD_LOGIC;
    grp_fu_179_p1 : in STD_LOGIC_VECTOR ( 31 downto 0 );
    A_q0 : in STD_LOGIC_VECTOR ( 16 downto 0 );
    buff2_reg : in STD_LOGIC_VECTOR ( 14 downto 0 )
  );
  attribute ORIG_REF_NAME : string;
  attribute ORIG_REF_NAME of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_4 : entity is "loop_imperfect_mubkb";
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_4;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_4 is
begin
loop_imperfect_mubkb_MulnS_0_U: entity work.decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_MulnS_0_6
     port map (
      A(16 downto 0) => A(16 downto 0),
      A_q0(16 downto 0) => A_q0(16 downto 0),
      Q(0) => Q(0),
      ap_clk => ap_clk,
      buff2_reg_0(14 downto 0) => buff2_reg(14 downto 0),
      grp_fu_179_p1(31 downto 0) => grp_fu_179_p1(31 downto 0),
      grp_fu_190_p1(14 downto 0) => grp_fu_190_p1(14 downto 0)
    );
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_5 is
  port (
    A_d0 : out STD_LOGIC_VECTOR ( 31 downto 0 );
    Q : in STD_LOGIC_VECTOR ( 0 to 0 );
    ap_clk : in STD_LOGIC;
    grp_fu_190_p1 : in STD_LOGIC_VECTOR ( 30 downto 0 );
    A : in STD_LOGIC_VECTOR ( 0 to 0 );
    A_q0 : in STD_LOGIC_VECTOR ( 16 downto 0 );
    buff2_reg : in STD_LOGIC_VECTOR ( 14 downto 0 )
  );
  attribute ORIG_REF_NAME : string;
  attribute ORIG_REF_NAME of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_5 : entity is "loop_imperfect_mubkb";
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_5;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_5 is
begin
loop_imperfect_mubkb_MulnS_0_U: entity work.decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_MulnS_0
     port map (
      A(0) => A(0),
      A_d0(31 downto 0) => A_d0(31 downto 0),
      A_q0(16 downto 0) => A_q0(16 downto 0),
      Q(0) => Q(0),
      ap_clk => ap_clk,
      buff2_reg_0(14 downto 0) => buff2_reg(14 downto 0),
      grp_fu_190_p1(30 downto 0) => grp_fu_190_p1(30 downto 0)
    );
end STRUCTURE;
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
    A_we0 : out STD_LOGIC;
    A_d0 : out STD_LOGIC_VECTOR ( 31 downto 0 );
    A_q0 : in STD_LOGIC_VECTOR ( 31 downto 0 );
    b_address0 : out STD_LOGIC_VECTOR ( 9 downto 0 );
    b_ce0 : out STD_LOGIC;
    b_q0 : in STD_LOGIC_VECTOR ( 31 downto 0 )
  );
  attribute ap_ST_fsm_state1 : integer;
  attribute ap_ST_fsm_state1 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is 1;
  attribute ap_ST_fsm_state10 : integer;
  attribute ap_ST_fsm_state10 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is 512;
  attribute ap_ST_fsm_state11 : integer;
  attribute ap_ST_fsm_state11 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is 1024;
  attribute ap_ST_fsm_state12 : integer;
  attribute ap_ST_fsm_state12 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is 2048;
  attribute ap_ST_fsm_state13 : integer;
  attribute ap_ST_fsm_state13 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is 4096;
  attribute ap_ST_fsm_state14 : integer;
  attribute ap_ST_fsm_state14 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is 8192;
  attribute ap_ST_fsm_state15 : integer;
  attribute ap_ST_fsm_state15 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is 16384;
  attribute ap_ST_fsm_state16 : integer;
  attribute ap_ST_fsm_state16 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is 32768;
  attribute ap_ST_fsm_state17 : integer;
  attribute ap_ST_fsm_state17 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is 65536;
  attribute ap_ST_fsm_state18 : integer;
  attribute ap_ST_fsm_state18 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is 131072;
  attribute ap_ST_fsm_state19 : integer;
  attribute ap_ST_fsm_state19 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is 262144;
  attribute ap_ST_fsm_state2 : integer;
  attribute ap_ST_fsm_state2 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is 2;
  attribute ap_ST_fsm_state20 : integer;
  attribute ap_ST_fsm_state20 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is 524288;
  attribute ap_ST_fsm_state21 : integer;
  attribute ap_ST_fsm_state21 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is 1048576;
  attribute ap_ST_fsm_state22 : integer;
  attribute ap_ST_fsm_state22 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is 2097152;
  attribute ap_ST_fsm_state23 : integer;
  attribute ap_ST_fsm_state23 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is 4194304;
  attribute ap_ST_fsm_state24 : integer;
  attribute ap_ST_fsm_state24 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is 8388608;
  attribute ap_ST_fsm_state25 : integer;
  attribute ap_ST_fsm_state25 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is 16777216;
  attribute ap_ST_fsm_state26 : integer;
  attribute ap_ST_fsm_state26 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is 33554432;
  attribute ap_ST_fsm_state27 : integer;
  attribute ap_ST_fsm_state27 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is 67108864;
  attribute ap_ST_fsm_state28 : integer;
  attribute ap_ST_fsm_state28 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is 134217728;
  attribute ap_ST_fsm_state29 : integer;
  attribute ap_ST_fsm_state29 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is 268435456;
  attribute ap_ST_fsm_state3 : integer;
  attribute ap_ST_fsm_state3 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is 4;
  attribute ap_ST_fsm_state30 : integer;
  attribute ap_ST_fsm_state30 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is 536870912;
  attribute ap_ST_fsm_state31 : integer;
  attribute ap_ST_fsm_state31 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is 1073741824;
  attribute ap_ST_fsm_state32 : integer;
  attribute ap_ST_fsm_state32 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is -2147483648;
  attribute ap_ST_fsm_state4 : integer;
  attribute ap_ST_fsm_state4 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is 8;
  attribute ap_ST_fsm_state5 : integer;
  attribute ap_ST_fsm_state5 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is 16;
  attribute ap_ST_fsm_state6 : integer;
  attribute ap_ST_fsm_state6 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is 32;
  attribute ap_ST_fsm_state7 : integer;
  attribute ap_ST_fsm_state7 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is 64;
  attribute ap_ST_fsm_state8 : integer;
  attribute ap_ST_fsm_state8 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is 128;
  attribute ap_ST_fsm_state9 : integer;
  attribute ap_ST_fsm_state9 of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is 256;
  attribute hls_module : string;
  attribute hls_module of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect : entity is "yes";
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect is
  signal \^a_we0\ : STD_LOGIC;
  signal \ap_CS_fsm[31]_i_1_n_1\ : STD_LOGIC;
  signal \ap_CS_fsm[31]_i_2_n_1\ : STD_LOGIC;
  signal \ap_CS_fsm_reg_n_1_[0]\ : STD_LOGIC;
  signal \ap_CS_fsm_reg_n_1_[10]\ : STD_LOGIC;
  signal \ap_CS_fsm_reg_n_1_[11]\ : STD_LOGIC;
  signal \ap_CS_fsm_reg_n_1_[12]\ : STD_LOGIC;
  signal \ap_CS_fsm_reg_n_1_[13]\ : STD_LOGIC;
  signal \ap_CS_fsm_reg_n_1_[14]\ : STD_LOGIC;
  signal \ap_CS_fsm_reg_n_1_[15]\ : STD_LOGIC;
  signal \ap_CS_fsm_reg_n_1_[16]\ : STD_LOGIC;
  signal \ap_CS_fsm_reg_n_1_[17]\ : STD_LOGIC;
  signal \ap_CS_fsm_reg_n_1_[18]\ : STD_LOGIC;
  signal \ap_CS_fsm_reg_n_1_[19]\ : STD_LOGIC;
  signal \ap_CS_fsm_reg_n_1_[20]\ : STD_LOGIC;
  signal \ap_CS_fsm_reg_n_1_[21]\ : STD_LOGIC;
  signal \ap_CS_fsm_reg_n_1_[22]\ : STD_LOGIC;
  signal \ap_CS_fsm_reg_n_1_[23]\ : STD_LOGIC;
  signal \ap_CS_fsm_reg_n_1_[24]\ : STD_LOGIC;
  signal \ap_CS_fsm_reg_n_1_[25]\ : STD_LOGIC;
  signal \ap_CS_fsm_reg_n_1_[26]\ : STD_LOGIC;
  signal \ap_CS_fsm_reg_n_1_[27]\ : STD_LOGIC;
  signal \ap_CS_fsm_reg_n_1_[28]\ : STD_LOGIC;
  signal \ap_CS_fsm_reg_n_1_[29]\ : STD_LOGIC;
  signal \ap_CS_fsm_reg_n_1_[3]\ : STD_LOGIC;
  signal \ap_CS_fsm_reg_n_1_[4]\ : STD_LOGIC;
  signal \ap_CS_fsm_reg_n_1_[5]\ : STD_LOGIC;
  signal \ap_CS_fsm_reg_n_1_[6]\ : STD_LOGIC;
  signal \ap_CS_fsm_reg_n_1_[7]\ : STD_LOGIC;
  signal \ap_CS_fsm_reg_n_1_[8]\ : STD_LOGIC;
  signal \ap_CS_fsm_reg_n_1_[9]\ : STD_LOGIC;
  signal ap_CS_fsm_state3 : STD_LOGIC;
  signal ap_NS_fsm : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal \^ap_done\ : STD_LOGIC;
  signal \^b_address0\ : STD_LOGIC_VECTOR ( 9 downto 0 );
  signal \^b_ce0\ : STD_LOGIC;
  signal b_load_reg_240 : STD_LOGIC_VECTOR ( 9 downto 0 );
  signal d_reg_224 : STD_LOGIC_VECTOR ( 31 downto 17 );
  signal grp_fu_135_p1 : STD_LOGIC_VECTOR ( 31 downto 0 );
  signal grp_fu_146_p1 : STD_LOGIC_VECTOR ( 31 downto 1 );
  signal grp_fu_157_p1 : STD_LOGIC_VECTOR ( 31 downto 0 );
  signal grp_fu_168_p1 : STD_LOGIC_VECTOR ( 31 downto 0 );
  signal grp_fu_179_p1 : STD_LOGIC_VECTOR ( 31 downto 0 );
  signal grp_fu_190_p1 : STD_LOGIC_VECTOR ( 31 downto 1 );
  signal i_0_reg_88 : STD_LOGIC;
  signal i_fu_105_p2 : STD_LOGIC_VECTOR ( 9 downto 0 );
  signal i_reg_209 : STD_LOGIC_VECTOR ( 9 downto 0 );
  signal \i_reg_209[9]_i_2_n_1\ : STD_LOGIC;
  signal \loop_imperfect_mubkb_MulnS_0_U/buff2_reg__0\ : STD_LOGIC_VECTOR ( 0 to 0 );
  signal \loop_imperfect_mubkb_MulnS_0_U/buff2_reg__0_0\ : STD_LOGIC_VECTOR ( 0 to 0 );
  attribute SOFT_HLUTNM : string;
  attribute SOFT_HLUTNM of \A_address0[0]_INST_0\ : label is "soft_lutpair3";
  attribute SOFT_HLUTNM of \A_address0[1]_INST_0\ : label is "soft_lutpair4";
  attribute SOFT_HLUTNM of \A_address0[2]_INST_0\ : label is "soft_lutpair5";
  attribute SOFT_HLUTNM of \A_address0[3]_INST_0\ : label is "soft_lutpair7";
  attribute SOFT_HLUTNM of \A_address0[4]_INST_0\ : label is "soft_lutpair8";
  attribute SOFT_HLUTNM of \A_address0[5]_INST_0\ : label is "soft_lutpair8";
  attribute SOFT_HLUTNM of \A_address0[6]_INST_0\ : label is "soft_lutpair9";
  attribute SOFT_HLUTNM of \A_address0[7]_INST_0\ : label is "soft_lutpair10";
  attribute SOFT_HLUTNM of \A_address0[8]_INST_0\ : label is "soft_lutpair10";
  attribute SOFT_HLUTNM of \A_address0[9]_INST_0\ : label is "soft_lutpair9";
  attribute SOFT_HLUTNM of A_ce0_INST_0 : label is "soft_lutpair7";
  attribute SOFT_HLUTNM of \ap_CS_fsm[1]_i_1\ : label is "soft_lutpair2";
  attribute FSM_ENCODING : string;
  attribute FSM_ENCODING of \ap_CS_fsm_reg[0]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[10]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[11]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[12]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[13]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[14]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[15]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[16]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[17]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[18]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[19]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[1]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[20]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[21]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[22]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[23]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[24]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[25]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[26]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[27]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[28]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[29]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[2]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[30]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[31]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[3]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[4]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[5]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[6]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[7]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[8]\ : label is "none";
  attribute FSM_ENCODING of \ap_CS_fsm_reg[9]\ : label is "none";
  attribute SOFT_HLUTNM of ap_idle_INST_0 : label is "soft_lutpair2";
  attribute SOFT_HLUTNM of \i_reg_209[0]_i_1\ : label is "soft_lutpair3";
  attribute SOFT_HLUTNM of \i_reg_209[1]_i_1\ : label is "soft_lutpair4";
  attribute SOFT_HLUTNM of \i_reg_209[2]_i_1\ : label is "soft_lutpair5";
  attribute SOFT_HLUTNM of \i_reg_209[3]_i_1\ : label is "soft_lutpair1";
  attribute SOFT_HLUTNM of \i_reg_209[4]_i_1\ : label is "soft_lutpair1";
  attribute SOFT_HLUTNM of \i_reg_209[6]_i_1\ : label is "soft_lutpair6";
  attribute SOFT_HLUTNM of \i_reg_209[7]_i_1\ : label is "soft_lutpair6";
  attribute SOFT_HLUTNM of \i_reg_209[8]_i_1\ : label is "soft_lutpair0";
  attribute SOFT_HLUTNM of \i_reg_209[9]_i_1\ : label is "soft_lutpair0";
begin
  A_we0 <= \^a_we0\;
  ap_done <= \^ap_done\;
  ap_ready <= \^ap_done\;
  b_address0(9 downto 0) <= \^b_address0\(9 downto 0);
  b_ce0 <= \^b_ce0\;
\A_address0[0]_INST_0\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"AC"
    )
        port map (
      I0 => b_load_reg_240(0),
      I1 => \^b_address0\(0),
      I2 => \^a_we0\,
      O => A_address0(0)
    );
\A_address0[1]_INST_0\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"AC"
    )
        port map (
      I0 => b_load_reg_240(1),
      I1 => \^b_address0\(1),
      I2 => \^a_we0\,
      O => A_address0(1)
    );
\A_address0[2]_INST_0\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"AC"
    )
        port map (
      I0 => b_load_reg_240(2),
      I1 => \^b_address0\(2),
      I2 => \^a_we0\,
      O => A_address0(2)
    );
\A_address0[3]_INST_0\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"AC"
    )
        port map (
      I0 => b_load_reg_240(3),
      I1 => \^b_address0\(3),
      I2 => \^a_we0\,
      O => A_address0(3)
    );
\A_address0[4]_INST_0\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"AC"
    )
        port map (
      I0 => b_load_reg_240(4),
      I1 => \^b_address0\(4),
      I2 => \^a_we0\,
      O => A_address0(4)
    );
\A_address0[5]_INST_0\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"AC"
    )
        port map (
      I0 => b_load_reg_240(5),
      I1 => \^b_address0\(5),
      I2 => \^a_we0\,
      O => A_address0(5)
    );
\A_address0[6]_INST_0\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"AC"
    )
        port map (
      I0 => b_load_reg_240(6),
      I1 => \^b_address0\(6),
      I2 => \^a_we0\,
      O => A_address0(6)
    );
\A_address0[7]_INST_0\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"AC"
    )
        port map (
      I0 => b_load_reg_240(7),
      I1 => \^b_address0\(7),
      I2 => \^a_we0\,
      O => A_address0(7)
    );
\A_address0[8]_INST_0\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"AC"
    )
        port map (
      I0 => b_load_reg_240(8),
      I1 => \^b_address0\(8),
      I2 => \^a_we0\,
      O => A_address0(8)
    );
\A_address0[9]_INST_0\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"AC"
    )
        port map (
      I0 => b_load_reg_240(9),
      I1 => \^b_address0\(9),
      I2 => \^a_we0\,
      O => A_address0(9)
    );
A_ce0_INST_0: unisim.vcomponents.LUT2
    generic map(
      INIT => X"E"
    )
        port map (
      I0 => \^b_ce0\,
      I1 => \^a_we0\,
      O => A_ce0
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
\ap_CS_fsm[1]_i_1\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"EA"
    )
        port map (
      I0 => \^a_we0\,
      I1 => ap_start,
      I2 => \ap_CS_fsm_reg_n_1_[0]\,
      O => ap_NS_fsm(1)
    );
\ap_CS_fsm[2]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"8AAAAAAAAAAAAAAA"
    )
        port map (
      I0 => \^b_ce0\,
      I1 => \ap_CS_fsm[31]_i_2_n_1\,
      I2 => \^b_address0\(6),
      I3 => \^b_address0\(7),
      I4 => \^b_address0\(8),
      I5 => \^b_address0\(9),
      O => ap_NS_fsm(2)
    );
\ap_CS_fsm[31]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"2000000000000000"
    )
        port map (
      I0 => \^b_ce0\,
      I1 => \ap_CS_fsm[31]_i_2_n_1\,
      I2 => \^b_address0\(6),
      I3 => \^b_address0\(7),
      I4 => \^b_address0\(8),
      I5 => \^b_address0\(9),
      O => \ap_CS_fsm[31]_i_1_n_1\
    );
\ap_CS_fsm[31]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFFFFFFFFFFFFF7"
    )
        port map (
      I0 => \^b_address0\(5),
      I1 => \^b_address0\(3),
      I2 => \^b_address0\(4),
      I3 => \^b_address0\(0),
      I4 => \^b_address0\(1),
      I5 => \^b_address0\(2),
      O => \ap_CS_fsm[31]_i_2_n_1\
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
\ap_CS_fsm_reg[10]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => \ap_CS_fsm_reg_n_1_[9]\,
      Q => \ap_CS_fsm_reg_n_1_[10]\,
      R => ap_rst
    );
\ap_CS_fsm_reg[11]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => \ap_CS_fsm_reg_n_1_[10]\,
      Q => \ap_CS_fsm_reg_n_1_[11]\,
      R => ap_rst
    );
\ap_CS_fsm_reg[12]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => \ap_CS_fsm_reg_n_1_[11]\,
      Q => \ap_CS_fsm_reg_n_1_[12]\,
      R => ap_rst
    );
\ap_CS_fsm_reg[13]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => \ap_CS_fsm_reg_n_1_[12]\,
      Q => \ap_CS_fsm_reg_n_1_[13]\,
      R => ap_rst
    );
\ap_CS_fsm_reg[14]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => \ap_CS_fsm_reg_n_1_[13]\,
      Q => \ap_CS_fsm_reg_n_1_[14]\,
      R => ap_rst
    );
\ap_CS_fsm_reg[15]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => \ap_CS_fsm_reg_n_1_[14]\,
      Q => \ap_CS_fsm_reg_n_1_[15]\,
      R => ap_rst
    );
\ap_CS_fsm_reg[16]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => \ap_CS_fsm_reg_n_1_[15]\,
      Q => \ap_CS_fsm_reg_n_1_[16]\,
      R => ap_rst
    );
\ap_CS_fsm_reg[17]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => \ap_CS_fsm_reg_n_1_[16]\,
      Q => \ap_CS_fsm_reg_n_1_[17]\,
      R => ap_rst
    );
\ap_CS_fsm_reg[18]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => \ap_CS_fsm_reg_n_1_[17]\,
      Q => \ap_CS_fsm_reg_n_1_[18]\,
      R => ap_rst
    );
\ap_CS_fsm_reg[19]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => \ap_CS_fsm_reg_n_1_[18]\,
      Q => \ap_CS_fsm_reg_n_1_[19]\,
      R => ap_rst
    );
\ap_CS_fsm_reg[1]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => ap_NS_fsm(1),
      Q => \^b_ce0\,
      R => ap_rst
    );
\ap_CS_fsm_reg[20]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => \ap_CS_fsm_reg_n_1_[19]\,
      Q => \ap_CS_fsm_reg_n_1_[20]\,
      R => ap_rst
    );
\ap_CS_fsm_reg[21]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => \ap_CS_fsm_reg_n_1_[20]\,
      Q => \ap_CS_fsm_reg_n_1_[21]\,
      R => ap_rst
    );
\ap_CS_fsm_reg[22]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => \ap_CS_fsm_reg_n_1_[21]\,
      Q => \ap_CS_fsm_reg_n_1_[22]\,
      R => ap_rst
    );
\ap_CS_fsm_reg[23]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => \ap_CS_fsm_reg_n_1_[22]\,
      Q => \ap_CS_fsm_reg_n_1_[23]\,
      R => ap_rst
    );
\ap_CS_fsm_reg[24]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => \ap_CS_fsm_reg_n_1_[23]\,
      Q => \ap_CS_fsm_reg_n_1_[24]\,
      R => ap_rst
    );
\ap_CS_fsm_reg[25]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => \ap_CS_fsm_reg_n_1_[24]\,
      Q => \ap_CS_fsm_reg_n_1_[25]\,
      R => ap_rst
    );
\ap_CS_fsm_reg[26]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => \ap_CS_fsm_reg_n_1_[25]\,
      Q => \ap_CS_fsm_reg_n_1_[26]\,
      R => ap_rst
    );
\ap_CS_fsm_reg[27]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => \ap_CS_fsm_reg_n_1_[26]\,
      Q => \ap_CS_fsm_reg_n_1_[27]\,
      R => ap_rst
    );
\ap_CS_fsm_reg[28]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => \ap_CS_fsm_reg_n_1_[27]\,
      Q => \ap_CS_fsm_reg_n_1_[28]\,
      R => ap_rst
    );
\ap_CS_fsm_reg[29]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => \ap_CS_fsm_reg_n_1_[28]\,
      Q => \ap_CS_fsm_reg_n_1_[29]\,
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
\ap_CS_fsm_reg[30]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => \ap_CS_fsm_reg_n_1_[29]\,
      Q => \^a_we0\,
      R => ap_rst
    );
\ap_CS_fsm_reg[31]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => \ap_CS_fsm[31]_i_1_n_1\,
      Q => \^ap_done\,
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
      Q => \ap_CS_fsm_reg_n_1_[3]\,
      R => ap_rst
    );
\ap_CS_fsm_reg[4]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => \ap_CS_fsm_reg_n_1_[3]\,
      Q => \ap_CS_fsm_reg_n_1_[4]\,
      R => ap_rst
    );
\ap_CS_fsm_reg[5]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => \ap_CS_fsm_reg_n_1_[4]\,
      Q => \ap_CS_fsm_reg_n_1_[5]\,
      R => ap_rst
    );
\ap_CS_fsm_reg[6]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => \ap_CS_fsm_reg_n_1_[5]\,
      Q => \ap_CS_fsm_reg_n_1_[6]\,
      R => ap_rst
    );
\ap_CS_fsm_reg[7]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => \ap_CS_fsm_reg_n_1_[6]\,
      Q => \ap_CS_fsm_reg_n_1_[7]\,
      R => ap_rst
    );
\ap_CS_fsm_reg[8]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => \ap_CS_fsm_reg_n_1_[7]\,
      Q => \ap_CS_fsm_reg_n_1_[8]\,
      R => ap_rst
    );
\ap_CS_fsm_reg[9]\: unisim.vcomponents.FDRE
    generic map(
      INIT => '0'
    )
        port map (
      C => ap_clk,
      CE => '1',
      D => \ap_CS_fsm_reg_n_1_[8]\,
      Q => \ap_CS_fsm_reg_n_1_[9]\,
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
\b_load_reg_240_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => b_q0(0),
      Q => b_load_reg_240(0),
      R => '0'
    );
\b_load_reg_240_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => b_q0(1),
      Q => b_load_reg_240(1),
      R => '0'
    );
\b_load_reg_240_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => b_q0(2),
      Q => b_load_reg_240(2),
      R => '0'
    );
\b_load_reg_240_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => b_q0(3),
      Q => b_load_reg_240(3),
      R => '0'
    );
\b_load_reg_240_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => b_q0(4),
      Q => b_load_reg_240(4),
      R => '0'
    );
\b_load_reg_240_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => b_q0(5),
      Q => b_load_reg_240(5),
      R => '0'
    );
\b_load_reg_240_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => b_q0(6),
      Q => b_load_reg_240(6),
      R => '0'
    );
\b_load_reg_240_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => b_q0(7),
      Q => b_load_reg_240(7),
      R => '0'
    );
\b_load_reg_240_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => b_q0(8),
      Q => b_load_reg_240(8),
      R => '0'
    );
\b_load_reg_240_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => b_q0(9),
      Q => b_load_reg_240(9),
      R => '0'
    );
\d_reg_224_reg[17]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => A_q0(17),
      Q => d_reg_224(17),
      R => '0'
    );
\d_reg_224_reg[18]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => A_q0(18),
      Q => d_reg_224(18),
      R => '0'
    );
\d_reg_224_reg[19]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => A_q0(19),
      Q => d_reg_224(19),
      R => '0'
    );
\d_reg_224_reg[20]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => A_q0(20),
      Q => d_reg_224(20),
      R => '0'
    );
\d_reg_224_reg[21]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => A_q0(21),
      Q => d_reg_224(21),
      R => '0'
    );
\d_reg_224_reg[22]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => A_q0(22),
      Q => d_reg_224(22),
      R => '0'
    );
\d_reg_224_reg[23]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => A_q0(23),
      Q => d_reg_224(23),
      R => '0'
    );
\d_reg_224_reg[24]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => A_q0(24),
      Q => d_reg_224(24),
      R => '0'
    );
\d_reg_224_reg[25]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => A_q0(25),
      Q => d_reg_224(25),
      R => '0'
    );
\d_reg_224_reg[26]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => A_q0(26),
      Q => d_reg_224(26),
      R => '0'
    );
\d_reg_224_reg[27]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => A_q0(27),
      Q => d_reg_224(27),
      R => '0'
    );
\d_reg_224_reg[28]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => A_q0(28),
      Q => d_reg_224(28),
      R => '0'
    );
\d_reg_224_reg[29]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => A_q0(29),
      Q => d_reg_224(29),
      R => '0'
    );
\d_reg_224_reg[30]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => A_q0(30),
      Q => d_reg_224(30),
      R => '0'
    );
\d_reg_224_reg[31]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => ap_CS_fsm_state3,
      D => A_q0(31),
      Q => d_reg_224(31),
      R => '0'
    );
\i_0_reg_88[9]_i_1\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"08"
    )
        port map (
      I0 => \ap_CS_fsm_reg_n_1_[0]\,
      I1 => ap_start,
      I2 => \^a_we0\,
      O => i_0_reg_88
    );
\i_0_reg_88_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^a_we0\,
      D => i_reg_209(0),
      Q => \^b_address0\(0),
      R => i_0_reg_88
    );
\i_0_reg_88_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^a_we0\,
      D => i_reg_209(1),
      Q => \^b_address0\(1),
      R => i_0_reg_88
    );
\i_0_reg_88_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^a_we0\,
      D => i_reg_209(2),
      Q => \^b_address0\(2),
      R => i_0_reg_88
    );
\i_0_reg_88_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^a_we0\,
      D => i_reg_209(3),
      Q => \^b_address0\(3),
      R => i_0_reg_88
    );
\i_0_reg_88_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^a_we0\,
      D => i_reg_209(4),
      Q => \^b_address0\(4),
      R => i_0_reg_88
    );
\i_0_reg_88_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^a_we0\,
      D => i_reg_209(5),
      Q => \^b_address0\(5),
      R => i_0_reg_88
    );
\i_0_reg_88_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^a_we0\,
      D => i_reg_209(6),
      Q => \^b_address0\(6),
      R => i_0_reg_88
    );
\i_0_reg_88_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^a_we0\,
      D => i_reg_209(7),
      Q => \^b_address0\(7),
      R => i_0_reg_88
    );
\i_0_reg_88_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^a_we0\,
      D => i_reg_209(8),
      Q => \^b_address0\(8),
      R => i_0_reg_88
    );
\i_0_reg_88_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^a_we0\,
      D => i_reg_209(9),
      Q => \^b_address0\(9),
      R => i_0_reg_88
    );
\i_reg_209[0]_i_1\: unisim.vcomponents.LUT1
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => \^b_address0\(0),
      O => i_fu_105_p2(0)
    );
\i_reg_209[1]_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => \^b_address0\(0),
      I1 => \^b_address0\(1),
      O => i_fu_105_p2(1)
    );
\i_reg_209[2]_i_1\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"78"
    )
        port map (
      I0 => \^b_address0\(0),
      I1 => \^b_address0\(1),
      I2 => \^b_address0\(2),
      O => i_fu_105_p2(2)
    );
\i_reg_209[3]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"7F80"
    )
        port map (
      I0 => \^b_address0\(1),
      I1 => \^b_address0\(0),
      I2 => \^b_address0\(2),
      I3 => \^b_address0\(3),
      O => i_fu_105_p2(3)
    );
\i_reg_209[4]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"7FFF8000"
    )
        port map (
      I0 => \^b_address0\(2),
      I1 => \^b_address0\(0),
      I2 => \^b_address0\(1),
      I3 => \^b_address0\(3),
      I4 => \^b_address0\(4),
      O => i_fu_105_p2(4)
    );
\i_reg_209[5]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"7FFFFFFF80000000"
    )
        port map (
      I0 => \^b_address0\(3),
      I1 => \^b_address0\(1),
      I2 => \^b_address0\(0),
      I3 => \^b_address0\(2),
      I4 => \^b_address0\(4),
      I5 => \^b_address0\(5),
      O => i_fu_105_p2(5)
    );
\i_reg_209[6]_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => \i_reg_209[9]_i_2_n_1\,
      I1 => \^b_address0\(6),
      O => i_fu_105_p2(6)
    );
\i_reg_209[7]_i_1\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"78"
    )
        port map (
      I0 => \i_reg_209[9]_i_2_n_1\,
      I1 => \^b_address0\(6),
      I2 => \^b_address0\(7),
      O => i_fu_105_p2(7)
    );
\i_reg_209[8]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"7F80"
    )
        port map (
      I0 => \^b_address0\(6),
      I1 => \i_reg_209[9]_i_2_n_1\,
      I2 => \^b_address0\(7),
      I3 => \^b_address0\(8),
      O => i_fu_105_p2(8)
    );
\i_reg_209[9]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"7FFF8000"
    )
        port map (
      I0 => \^b_address0\(7),
      I1 => \i_reg_209[9]_i_2_n_1\,
      I2 => \^b_address0\(6),
      I3 => \^b_address0\(8),
      I4 => \^b_address0\(9),
      O => i_fu_105_p2(9)
    );
\i_reg_209[9]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"8000000000000000"
    )
        port map (
      I0 => \^b_address0\(5),
      I1 => \^b_address0\(3),
      I2 => \^b_address0\(1),
      I3 => \^b_address0\(0),
      I4 => \^b_address0\(2),
      I5 => \^b_address0\(4),
      O => \i_reg_209[9]_i_2_n_1\
    );
\i_reg_209_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^b_ce0\,
      D => i_fu_105_p2(0),
      Q => i_reg_209(0),
      R => '0'
    );
\i_reg_209_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^b_ce0\,
      D => i_fu_105_p2(1),
      Q => i_reg_209(1),
      R => '0'
    );
\i_reg_209_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^b_ce0\,
      D => i_fu_105_p2(2),
      Q => i_reg_209(2),
      R => '0'
    );
\i_reg_209_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^b_ce0\,
      D => i_fu_105_p2(3),
      Q => i_reg_209(3),
      R => '0'
    );
\i_reg_209_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^b_ce0\,
      D => i_fu_105_p2(4),
      Q => i_reg_209(4),
      R => '0'
    );
\i_reg_209_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^b_ce0\,
      D => i_fu_105_p2(5),
      Q => i_reg_209(5),
      R => '0'
    );
\i_reg_209_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^b_ce0\,
      D => i_fu_105_p2(6),
      Q => i_reg_209(6),
      R => '0'
    );
\i_reg_209_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^b_ce0\,
      D => i_fu_105_p2(7),
      Q => i_reg_209(7),
      R => '0'
    );
\i_reg_209_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^b_ce0\,
      D => i_fu_105_p2(8),
      Q => i_reg_209(8),
      R => '0'
    );
\i_reg_209_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => ap_clk,
      CE => \^b_ce0\,
      D => i_fu_105_p2(9),
      Q => i_reg_209(9),
      R => '0'
    );
loop_imperfect_mubkb_U1: entity work.decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb
     port map (
      A_q0(31 downto 0) => A_q0(31 downto 0),
      ap_clk => ap_clk,
      grp_fu_135_p1(31 downto 0) => grp_fu_135_p1(31 downto 0)
    );
loop_imperfect_mubkb_U2: entity work.decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_0
     port map (
      A(16 downto 1) => grp_fu_146_p1(16 downto 1),
      A(0) => \loop_imperfect_mubkb_MulnS_0_U/buff2_reg__0\(0),
      A_q0(16 downto 0) => A_q0(16 downto 0),
      Q(0) => ap_CS_fsm_state3,
      ap_clk => ap_clk,
      buff2_reg(14 downto 0) => d_reg_224(31 downto 17),
      grp_fu_135_p1(31 downto 0) => grp_fu_135_p1(31 downto 0),
      grp_fu_146_p1(14 downto 0) => grp_fu_146_p1(31 downto 17)
    );
loop_imperfect_mubkb_U3: entity work.decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_1
     port map (
      A(0) => \loop_imperfect_mubkb_MulnS_0_U/buff2_reg__0\(0),
      A_q0(16 downto 0) => A_q0(16 downto 0),
      Q(0) => ap_CS_fsm_state3,
      ap_clk => ap_clk,
      buff2_reg(14 downto 0) => d_reg_224(31 downto 17),
      grp_fu_146_p1(30 downto 0) => grp_fu_146_p1(31 downto 1),
      grp_fu_157_p1(31 downto 0) => grp_fu_157_p1(31 downto 0)
    );
loop_imperfect_mubkb_U4: entity work.decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_2
     port map (
      A_q0(16 downto 0) => A_q0(16 downto 0),
      Q(0) => ap_CS_fsm_state3,
      ap_clk => ap_clk,
      buff2_reg(14 downto 0) => d_reg_224(31 downto 17),
      grp_fu_157_p1(31 downto 0) => grp_fu_157_p1(31 downto 0),
      grp_fu_168_p1(31 downto 0) => grp_fu_168_p1(31 downto 0)
    );
loop_imperfect_mubkb_U5: entity work.decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_3
     port map (
      A_q0(16 downto 0) => A_q0(16 downto 0),
      Q(0) => ap_CS_fsm_state3,
      ap_clk => ap_clk,
      buff2_reg(14 downto 0) => d_reg_224(31 downto 17),
      grp_fu_168_p1(31 downto 0) => grp_fu_168_p1(31 downto 0),
      grp_fu_179_p1(31 downto 0) => grp_fu_179_p1(31 downto 0)
    );
loop_imperfect_mubkb_U6: entity work.decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_4
     port map (
      A(16 downto 1) => grp_fu_190_p1(16 downto 1),
      A(0) => \loop_imperfect_mubkb_MulnS_0_U/buff2_reg__0_0\(0),
      A_q0(16 downto 0) => A_q0(16 downto 0),
      Q(0) => ap_CS_fsm_state3,
      ap_clk => ap_clk,
      buff2_reg(14 downto 0) => d_reg_224(31 downto 17),
      grp_fu_179_p1(31 downto 0) => grp_fu_179_p1(31 downto 0),
      grp_fu_190_p1(14 downto 0) => grp_fu_190_p1(31 downto 17)
    );
loop_imperfect_mubkb_U7: entity work.decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect_mubkb_5
     port map (
      A(0) => \loop_imperfect_mubkb_MulnS_0_U/buff2_reg__0_0\(0),
      A_d0(31 downto 0) => A_d0(31 downto 0),
      A_q0(16 downto 0) => A_q0(16 downto 0),
      Q(0) => ap_CS_fsm_state3,
      ap_clk => ap_clk,
      buff2_reg(14 downto 0) => d_reg_224(31 downto 17),
      grp_fu_190_p1(30 downto 0) => grp_fu_190_p1(31 downto 1)
    );
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix is
  port (
    A_ce0 : out STD_LOGIC;
    A_we0 : out STD_LOGIC;
    b_ce0 : out STD_LOGIC;
    ap_clk : in STD_LOGIC;
    ap_rst : in STD_LOGIC;
    ap_start : in STD_LOGIC;
    ap_done : out STD_LOGIC;
    ap_idle : out STD_LOGIC;
    ap_ready : out STD_LOGIC;
    A_address0 : out STD_LOGIC_VECTOR ( 9 downto 0 );
    A_d0 : out STD_LOGIC_VECTOR ( 31 downto 0 );
    A_q0 : in STD_LOGIC_VECTOR ( 31 downto 0 );
    b_address0 : out STD_LOGIC_VECTOR ( 9 downto 0 );
    b_q0 : in STD_LOGIC_VECTOR ( 31 downto 0 )
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
  attribute ap_ST_fsm_state1 : integer;
  attribute ap_ST_fsm_state1 of inst : label is 1;
  attribute ap_ST_fsm_state10 : integer;
  attribute ap_ST_fsm_state10 of inst : label is 512;
  attribute ap_ST_fsm_state11 : integer;
  attribute ap_ST_fsm_state11 of inst : label is 1024;
  attribute ap_ST_fsm_state12 : integer;
  attribute ap_ST_fsm_state12 of inst : label is 2048;
  attribute ap_ST_fsm_state13 : integer;
  attribute ap_ST_fsm_state13 of inst : label is 4096;
  attribute ap_ST_fsm_state14 : integer;
  attribute ap_ST_fsm_state14 of inst : label is 8192;
  attribute ap_ST_fsm_state15 : integer;
  attribute ap_ST_fsm_state15 of inst : label is 16384;
  attribute ap_ST_fsm_state16 : integer;
  attribute ap_ST_fsm_state16 of inst : label is 32768;
  attribute ap_ST_fsm_state17 : integer;
  attribute ap_ST_fsm_state17 of inst : label is 65536;
  attribute ap_ST_fsm_state18 : integer;
  attribute ap_ST_fsm_state18 of inst : label is 131072;
  attribute ap_ST_fsm_state19 : integer;
  attribute ap_ST_fsm_state19 of inst : label is 262144;
  attribute ap_ST_fsm_state2 : integer;
  attribute ap_ST_fsm_state2 of inst : label is 2;
  attribute ap_ST_fsm_state20 : integer;
  attribute ap_ST_fsm_state20 of inst : label is 524288;
  attribute ap_ST_fsm_state21 : integer;
  attribute ap_ST_fsm_state21 of inst : label is 1048576;
  attribute ap_ST_fsm_state22 : integer;
  attribute ap_ST_fsm_state22 of inst : label is 2097152;
  attribute ap_ST_fsm_state23 : integer;
  attribute ap_ST_fsm_state23 of inst : label is 4194304;
  attribute ap_ST_fsm_state24 : integer;
  attribute ap_ST_fsm_state24 of inst : label is 8388608;
  attribute ap_ST_fsm_state25 : integer;
  attribute ap_ST_fsm_state25 of inst : label is 16777216;
  attribute ap_ST_fsm_state26 : integer;
  attribute ap_ST_fsm_state26 of inst : label is 33554432;
  attribute ap_ST_fsm_state27 : integer;
  attribute ap_ST_fsm_state27 of inst : label is 67108864;
  attribute ap_ST_fsm_state28 : integer;
  attribute ap_ST_fsm_state28 of inst : label is 134217728;
  attribute ap_ST_fsm_state29 : integer;
  attribute ap_ST_fsm_state29 of inst : label is 268435456;
  attribute ap_ST_fsm_state3 : integer;
  attribute ap_ST_fsm_state3 of inst : label is 4;
  attribute ap_ST_fsm_state30 : integer;
  attribute ap_ST_fsm_state30 of inst : label is 536870912;
  attribute ap_ST_fsm_state31 : integer;
  attribute ap_ST_fsm_state31 of inst : label is 1073741824;
  attribute ap_ST_fsm_state32 : integer;
  attribute ap_ST_fsm_state32 of inst : label is -2147483648;
  attribute ap_ST_fsm_state4 : integer;
  attribute ap_ST_fsm_state4 of inst : label is 8;
  attribute ap_ST_fsm_state5 : integer;
  attribute ap_ST_fsm_state5 of inst : label is 16;
  attribute ap_ST_fsm_state6 : integer;
  attribute ap_ST_fsm_state6 of inst : label is 32;
  attribute ap_ST_fsm_state7 : integer;
  attribute ap_ST_fsm_state7 of inst : label is 64;
  attribute ap_ST_fsm_state8 : integer;
  attribute ap_ST_fsm_state8 of inst : label is 128;
  attribute ap_ST_fsm_state9 : integer;
  attribute ap_ST_fsm_state9 of inst : label is 256;
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
  attribute X_INTERFACE_INFO of A_d0 : signal is "xilinx.com:signal:data:1.0 A_d0 DATA";
  attribute X_INTERFACE_PARAMETER of A_d0 : signal is "XIL_INTERFACENAME A_d0, LAYERED_METADATA undef";
  attribute X_INTERFACE_INFO of A_q0 : signal is "xilinx.com:signal:data:1.0 A_q0 DATA";
  attribute X_INTERFACE_PARAMETER of A_q0 : signal is "XIL_INTERFACENAME A_q0, LAYERED_METADATA undef";
  attribute X_INTERFACE_INFO of b_address0 : signal is "xilinx.com:signal:data:1.0 b_address0 DATA";
  attribute X_INTERFACE_PARAMETER of b_address0 : signal is "XIL_INTERFACENAME b_address0, LAYERED_METADATA undef";
  attribute X_INTERFACE_INFO of b_q0 : signal is "xilinx.com:signal:data:1.0 b_q0 DATA";
  attribute X_INTERFACE_PARAMETER of b_q0 : signal is "XIL_INTERFACENAME b_q0, LAYERED_METADATA undef";
begin
inst: entity work.decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect
     port map (
      A_address0(9 downto 0) => A_address0(9 downto 0),
      A_ce0 => A_ce0,
      A_d0(31 downto 0) => A_d0(31 downto 0),
      A_q0(31 downto 0) => A_q0(31 downto 0),
      A_we0 => A_we0,
      ap_clk => ap_clk,
      ap_done => ap_done,
      ap_idle => ap_idle,
      ap_ready => ap_ready,
      ap_rst => ap_rst,
      ap_start => ap_start,
      b_address0(9 downto 0) => b_address0(9 downto 0),
      b_ce0 => b_ce0,
      b_q0(31 downto 0) => b_q0(31 downto 0)
    );
end STRUCTURE;
