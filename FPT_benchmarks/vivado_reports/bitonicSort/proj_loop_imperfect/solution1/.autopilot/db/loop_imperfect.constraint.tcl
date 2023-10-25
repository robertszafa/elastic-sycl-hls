set clock_constraint { \
    name clk \
    module loop_imperfect \
    port ap_clk \
    period 4 \
    uncertainty 0.5 \
}

set all_path {}

set false_path {}

