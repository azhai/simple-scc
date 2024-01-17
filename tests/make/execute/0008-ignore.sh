#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT QUIT HUP TERM

tmp1=tmp1.$$
tmp2=tmp2.$$

cat <<EOF > $tmp2
no-valid-program
sh: 1: no-valid-program: not found
Hello World!
EOF

scc-make -f test.mk ignored-error hello > $tmp1 2>&1

diff $tmp1 $tmp2
