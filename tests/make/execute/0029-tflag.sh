#!/bin/sh

trap 'rm -f $tmp1 $tmp2 file2' EXIT INT TERM QUIT HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

cat > $tmp2 <<EOF
EOF

scc-make -tf test.mk file2 > $tmp1 2>&1

diff $tmp1 $tmp2 && ! test -f file2
