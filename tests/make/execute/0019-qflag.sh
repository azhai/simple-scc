#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT QUIT TERM HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

cat > $tmp2 <<EOF
-q
pass
EOF

(set -e
 MAKEFLAGS=-q scc-make -f test.mk print-makeflags-always
 echo pass) > $tmp1 2>&1

diff $tmp1 $tmp2
