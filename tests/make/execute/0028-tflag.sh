#!/bin/sh

trap 'rm -f $tmp1 $tmp2 file1' EXIT INT TERM QUIT HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

cat > $tmp2 <<EOF
touch file1
EOF

scc-make -tf test.mk file1 > $tmp1 2>&1

diff $tmp1 $tmp2 && test -f file1
