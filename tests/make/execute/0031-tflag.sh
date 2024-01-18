#!/bin/sh

trap 'rm -f $tmp1 $tmp2 print-makeflags-always' EXIT INT TERM QUIT HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

cat > $tmp2 <<EOF
-t
touch print-makeflags-always
EOF

MAKEFLAGS=-t scc-make -f test.mk print-makeflags-always > $tmp1 2>&1

diff -u $tmp1 $tmp2 && test -f print-makeflags-always 
