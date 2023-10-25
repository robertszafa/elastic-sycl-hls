Maximize
  NxS_e0 + NxS_e2 + NxS_e3 + NxS_e5 + NxS_e6 + NxS_e7
Subject to
  -S_BB_1 + S_e_0 = 0
  -S_BB_1 = 0
  -S_BB_2 + S_e_2 = 0
  -S_BB_2 + S_e_0 = 0
  -S_BB_3 + S_e_3 = 0
  -S_BB_3 + S_e_2 + S_e_6 = 0
  -S_BB_4 + S_e_5 = 0
  -S_BB_4 + S_e_3 = 0
  -S_BB_5 + S_e_6 + S_e_7 = 0
  -S_BB_5 + S_e_5 = 0
  -S_BB_6 = 0
  -S_BB_6 + S_e_7 = 0
  NxS_e0 - N <= 0
  -1000 S_e_0 + NxS_e0 <= 0
  -1000 S_e_0 + NxS_e0 - N >= -1000
  -999 S_e_0 - N >= -1000
  NxS_e2 - N <= 0
  -1000 S_e_2 + NxS_e2 <= 0
  -1000 S_e_2 + NxS_e2 - N >= -1000
  -999 S_e_2 - N >= -1000
  NxS_e3 - N <= 0
  -1000 S_e_3 + NxS_e3 <= 0
  -1000 S_e_3 + NxS_e3 - N >= -1000
  -N >= -1000
  NxS_e5 - N <= 0
  -1000 S_e_5 + NxS_e5 <= 0
  -1000 S_e_5 + NxS_e5 - N >= -1000
  -N >= -1000
  NxS_e6 - N <= 0
  -1000 S_e_6 + NxS_e6 <= 0
  -1000 S_e_6 + NxS_e6 - N >= -1000
  -S_e_6 - N >= -1000
  NxS_e7 - N <= 0
  -1000 S_e_7 + NxS_e7 <= 0
  -1000 S_e_7 + NxS_e7 - N >= -1000
  -999 S_e_7 - N >= -1000
  S_e_6 = 1
Bounds
  0 <= NxS_e0 <= 1
  0 <= NxS_e2 <= 1
  0 <= NxS_e3 <= 1000
  0 <= NxS_e5 <= 1000
  0 <= NxS_e6 <= 999
  0 <= NxS_e7 <= 1
  0 <= N <= 1000
General
  N
Binary
  S_BB_1 S_BB_2 S_BB_3 S_BB_4 S_BB_5 S_BB_6 S_e_0 S_e_2 S_e_3 S_e_5 S_e_6 S_e_7
End
