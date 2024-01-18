#!/bin/sh

trap 'rm -f $tmp1 $tmp2 file.c file.o' EXIT QUIT INT TERM HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

cat > $tmp2 <<EOF
-r
EOF
touch file.c

scc-make -rf test.mk file.o print-makeflags > $tmp1 2>&1

diff $tmp1 $tmp2
