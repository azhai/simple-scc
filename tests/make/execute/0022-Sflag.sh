#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT QUIT TERM HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

cat > $tmp2 <<EOF
-k -S
no-valid-program
sh: 1: no-valid-program: not found
make: error: target error: error 32512
EOF

MAKEFLAGS=-k scc-make -Sf test.mk print-makeflags follow >$tmp1 2>&1

diff $tmp1 $tmp2
