#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT QUIT TERM QUIT

tmp1=tmp1.$$
tmp2=tmp2.$$

cat > $tmp2 <<EOF
make: error: don't know how to make target1
EOF

scc-make -f - target1 <<EOF > $tmp1 2>&1
#esto es un comentario \
target1: ; echo hello
target2: ; echo bye
EOF

diff $tmp1 $tmp2
