#!/bin/sh
lli=${LLVMINTERP-lli}
exec $lli \
    /home/dynamatic/bnn/proj_loop_imperfect/solution1/.autopilot/db/a.g.bc ${1+"$@"}
