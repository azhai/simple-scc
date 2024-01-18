#!/bin/sh

trap 'rm -f $tmp1 $tmp2 file.o file.c' EXIT INT TERM QUIT HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

touch file.c

cat > $tmp2 <<EOF
touch file.c
touch file.o
EOF

scc-make -tf test.mk file.o > $tmp1 2>&1

diff $tmp1 $tmp2 && test -f file.c -a -f file.o
