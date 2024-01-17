#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT QUIT HUP TERM

tmp1=tmp1.$$
tmp2=tmp2.$$

cat <<EOF > $tmp2
sh: 1: no-valid-program: not found
-i
no-valid-program
EOF

MAKEFLAGS=-i scc-make -f test.mk error print-makeflags > $tmp1 2>&1

diff -u $tmp1 $tmp2
