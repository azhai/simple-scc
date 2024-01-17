#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT QUIT INT HUP TERM

tmp1=tmp1.$$
tmp2=tmp2.$$

cat <<EOF > $tmp2
echo Hello World!
Hello World!
EOF

scc-make -nf test.mk print-hidden-hello > $tmp1 2>&1

diff $tmp1 $tmp2 
