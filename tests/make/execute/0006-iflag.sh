#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT QUIT HUP TERM

tmp1=tmp1.$$
tmp2=tmp2.$$

cat <<EOF > $tmp2
no-valid-program
sh: 1: no-valid-program: not found
-i
EOF

scc-make -if test.mk error print-makeflags > $tmp1 2>&1

diff $tmp1 $tmp2