#!/bin/sh
lli=${LLVMINTERP-lli}
exec $lli \
    /home/dynamatic/vecTrans/proj_loop_imperfect/solution1/.autopilot/db/a.g.bc ${1+"$@"}
