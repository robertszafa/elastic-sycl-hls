Maximize
  NxS_e0 + NxS_e1 + NxS_e2
Subject to
  -S_BB_1 + S_e_0 = 0
  -S_BB_1 = 0
  -S_BB_2 + S_e_1 + S_e_2 = 0
  -S_BB_2 + S_e_0 + S_e_1 = 0
  -S_BB_3 = 0
  -S_BB_3 + S_e_2 = 0
  NxS_e0 - N <= 0
  -999 S_e_0 + NxS_e0 <= 0
  -999 S_e_0 + NxS_e0 - N >= -999
  -998 S_e_0 - N >= -999
  NxS_e1 - N <= 0
  -999 S_e_1 + NxS_e1 <= 0
  -999 S_e_1 + NxS_e1 - N >= -999
  -N >= -999
  NxS_e2 - N <= 0
  -999 S_e_2 + NxS_e2 <= 0
  -999 S_e_2 + NxS_e2 - N >= -999
  -998 S_e_2 - N >= -999
  S_e_1 = 1
Bounds
  0 <= NxS_e0 <= 1
  0 <= NxS_e1 <= 999
  0 <= NxS_e2 <= 1
  0 <= N <= 999
General
  N
Binary
  S_BB_1 S_BB_2 S_BB_3 S_e_0 S_e_1 S_e_2
End
