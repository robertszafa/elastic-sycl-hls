#!/bin/sh
lli=${LLVMINTERP-lli}
exec $lli \
    /home/dynamatic/loop_imperfect/proj_loop_imperfect/solution1/.autopilot/db/a.g.bc ${1+"$@"}
