#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT QUIT TERM HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

cat > $tmp2 <<EOF
Hello World!
EOF

scc-make -f test.mk silent-echo-hello >$tmp1 2>&1

diff $tmp1 $tmp2
