enum DEP_DIR {
  BACK, // First load, then store in a loop.
  FORWARD, // First store, then load in a loop.
};

template <int MEM_ID> struct DepInfo;

// gemm
template <> struct DepInfo<0> {
  static constexpr int NUM_LOADS = 2;
  static constexpr int NUM_STORES = 2;
  static constexpr int LOOP_DEPTH = 3;

  static constexpr int COMMON_LOOP_DEPTH[NUM_LOADS][NUM_STORES] = {{1, 0},
                                                                   {0, 2}};
  static constexpr int COMMON_STORE_LOOP_DEPTH[NUM_STORES][NUM_STORES] = {
      {1, 0}, {0, 2}};
  static constexpr DEP_DIR LOAD_TO_STORE_DEP_DIR[NUM_LOADS][NUM_STORES] = {
      {BACK, BACK}, {FORWARD, BACK}};
  static constexpr int LOAD_LOOP_DEPTH[NUM_LOADS] = {1, 2};
  static constexpr int STORE_LOOP_DEPTH[NUM_STORES] = {1, 2};

  static constexpr bool LOAD_IS_MAX_ITER_NEEDED[NUM_LOADS][LOOP_DEPTH] = {
      {false, false, false},
      {false, true, false},
  };
  static constexpr bool STORE_IS_MAX_ITER_NEEDED[NUM_STORES][LOOP_DEPTH] = {
      {false, false, false},
      {false, true, false},
  };
  static constexpr bool ARE_IN_SAME_LOOP[NUM_LOADS][NUM_STORES] = {
      {true, false},
      {false, true},
  };
};

// test_fusion_war2
template <> struct DepInfo<1> {
  static constexpr int NUM_LOADS = 1;
  static constexpr int NUM_STORES = 1;
  static constexpr int LOOP_DEPTH = 3;

  static constexpr int COMMON_LOOP_DEPTH[NUM_LOADS][NUM_STORES] = {{0}};
  static constexpr int COMMON_STORE_LOOP_DEPTH[NUM_STORES][NUM_STORES] = {{1}};
  static constexpr DEP_DIR LOAD_TO_STORE_DEP_DIR[NUM_LOADS][NUM_STORES] = {
      {BACK}};
  static constexpr int LOAD_LOOP_DEPTH[NUM_LOADS] = {2};
  static constexpr int STORE_LOOP_DEPTH[NUM_STORES] = {1};

  static constexpr bool LOAD_IS_MAX_ITER_NEEDED[NUM_LOADS][LOOP_DEPTH] = {
      {false, true, false}};
  static constexpr bool STORE_IS_MAX_ITER_NEEDED[NUM_STORES][LOOP_DEPTH] = {
      {true, false}};
  static constexpr bool ARE_IN_SAME_LOOP[NUM_LOADS][NUM_STORES] = {{false}};
};

// test_fusion_waw2
template <> struct DepInfo<2> {
  static constexpr int NUM_LOADS = 1;
  static constexpr int NUM_STORES = 3;
  static constexpr int LOOP_DEPTH = 3;

  static constexpr int COMMON_LOOP_DEPTH[NUM_LOADS][NUM_STORES] = {{0, 0, 1}};
  static constexpr int COMMON_STORE_LOOP_DEPTH[NUM_STORES][NUM_STORES] = {
      {2, 0, 0},
      {0, 1, 0},
      {0, 0, 1},
  };
  static constexpr DEP_DIR LOAD_TO_STORE_DEP_DIR[NUM_LOADS][NUM_STORES] = {
      {FORWARD, FORWARD, BACK}};
  static constexpr int LOAD_LOOP_DEPTH[NUM_LOADS] = {1};
  static constexpr int STORE_LOOP_DEPTH[NUM_STORES] = {2, 1, 1};

  static constexpr bool LOAD_IS_MAX_ITER_NEEDED[NUM_LOADS][LOOP_DEPTH] = {
      {true, false, false}};
  static constexpr bool STORE_IS_MAX_ITER_NEEDED[NUM_STORES][LOOP_DEPTH] = {
      {true, true, false}, {true, false, false}, {true, false, false}};
  static constexpr bool ARE_IN_SAME_LOOP[NUM_LOADS][NUM_STORES] = {
      {false, false, true},
  };
};

// 2mm
template <> struct DepInfo<3> {
  static constexpr int NUM_LOADS = 2;
  static constexpr int NUM_STORES = 1;
  static constexpr int LOOP_DEPTH = 3;

  static constexpr int COMMON_LOOP_DEPTH[NUM_LOADS][NUM_STORES] = {{1}, {-1}};
  static constexpr int COMMON_STORE_LOOP_DEPTH[NUM_STORES][NUM_STORES] = {{1}};
  static constexpr DEP_DIR LOAD_TO_STORE_DEP_DIR[NUM_LOADS][NUM_STORES] = {
      {BACK}, {FORWARD}};
  static constexpr int LOAD_LOOP_DEPTH[NUM_LOADS] = {1, 2};
  static constexpr int STORE_LOOP_DEPTH[NUM_STORES] = {1};

  static constexpr bool LOAD_IS_MAX_ITER_NEEDED[NUM_LOADS][LOOP_DEPTH] = {
      {false, false, false}, {false, true, false}};
  static constexpr bool STORE_IS_MAX_ITER_NEEDED[NUM_STORES][LOOP_DEPTH] = {
      {false, false, false}};
  static constexpr bool ARE_IN_SAME_LOOP[NUM_LOADS][NUM_STORES] = {
      {true},
      {false},
  };
};


// page rank p_new
template <> struct DepInfo<4> {
  static constexpr int NUM_LOADS = 2;
  static constexpr int NUM_STORES = 2;
  static constexpr int LOOP_DEPTH = 3;

  static constexpr int LOAD_LOOP_DEPTH[NUM_LOADS] = {2, 1};
  static constexpr int STORE_LOOP_DEPTH[NUM_STORES] = {1, 2};
  static constexpr int COMMON_LOOP_DEPTH[NUM_LOADS][NUM_STORES] = {{0, 2},
                                                                   {0, 0}};
  static constexpr int COMMON_STORE_LOOP_DEPTH[NUM_STORES][NUM_STORES] = {
      {1, 0}, {0, 2}};
  static constexpr DEP_DIR LOAD_TO_STORE_DEP_DIR[NUM_LOADS][NUM_STORES] = {
      {FORWARD, BACK}, {FORWARD, FORWARD}};

  static constexpr bool LOAD_IS_MAX_ITER_NEEDED[NUM_LOADS][LOOP_DEPTH] = {
      {true, true, false},
      {true, false, false}
  };
  static constexpr bool STORE_IS_MAX_ITER_NEEDED[NUM_STORES][LOOP_DEPTH] = {
      {true, false, false},
      {true, true, false}
  };
  static constexpr bool ARE_IN_SAME_LOOP[NUM_LOADS][NUM_STORES] = {
      {false, true},
      {false, false}
  };
};

// page rank p
template <> struct DepInfo<5> {
  static constexpr int NUM_LOADS = 1;
  static constexpr int NUM_STORES = 1;
  static constexpr int LOOP_DEPTH = 3;

  static constexpr int LOAD_LOOP_DEPTH[NUM_LOADS] = {2};
  static constexpr int STORE_LOOP_DEPTH[NUM_STORES] = {1};
  static constexpr int COMMON_LOOP_DEPTH[NUM_LOADS][NUM_STORES] = {{0}};
  static constexpr int COMMON_STORE_LOOP_DEPTH[NUM_STORES][NUM_STORES] = {{1}};

  static constexpr DEP_DIR LOAD_TO_STORE_DEP_DIR[NUM_LOADS][NUM_STORES] = {
      {BACK}};

  static constexpr bool LOAD_IS_MAX_ITER_NEEDED[NUM_LOADS][LOOP_DEPTH] = {
      {true, false, true},
  };
  static constexpr bool STORE_IS_MAX_ITER_NEEDED[NUM_STORES][LOOP_DEPTH] = {
      {true, false, false},
  };
  static constexpr bool ARE_IN_SAME_LOOP[NUM_LOADS][NUM_STORES] = {{false}};
};

// lud
template <> struct DepInfo<6> {
  static constexpr int NUM_LOADS = 7;
  static constexpr int NUM_STORES = 2;
  static constexpr int LOOP_DEPTH = 3;

  static constexpr int LOAD_LOOP_DEPTH[NUM_LOADS] = {1, 2, 2, 1, 1, 2, 2};
  static constexpr int STORE_LOOP_DEPTH[NUM_STORES] = {1, 1};
  static constexpr int COMMON_LOOP_DEPTH[NUM_LOADS][NUM_STORES] = {
      {1, 0}, {1, 0}, {1, 0}, {1, 0}, {0, 1}, {0, 1}, {0, 1}};
  static constexpr int COMMON_STORE_LOOP_DEPTH[NUM_STORES][NUM_STORES] = {
      {1, 0},
      {0, 1},
  };
  static constexpr DEP_DIR LOAD_TO_STORE_DEP_DIR[NUM_LOADS][NUM_STORES] = {
      {BACK, BACK},
      {BACK, BACK},
      {BACK, BACK},
      {BACK, BACK},
      {FORWARD, BACK},
      {FORWARD, BACK},
      {FORWARD, BACK}
  };
  static constexpr bool LOAD_IS_MAX_ITER_NEEDED[NUM_LOADS][LOOP_DEPTH] = {
      {false, false, false}, {false, true, false}, {true, true, false},
      {true, false, false},

      {false, false, false}, {false, true, false}, {true, true, false},
  };
  static constexpr bool STORE_IS_MAX_ITER_NEEDED[NUM_STORES][LOOP_DEPTH] = {
      {false, false, false},
      {false, false, false},
  };
  static constexpr bool ARE_IN_SAME_LOOP[NUM_LOADS][NUM_STORES] = {
      {true, false}, {false, false}, {false, false}, {true, false},

      {false, true}, {false, false}, {false, false},
  };
};
