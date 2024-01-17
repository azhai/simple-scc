#!/bin/sh

trap 'rm -f $tmp1 $tmp2 $tmp3' EXIT INT QUIT HUP TERM

tmp1=tmp1.$$
tmp2=tmp2.$$
tmp3=tmp3.$$

cat <<EOF > $tmp2
EOF

cat <<EOF > $tmp3
.IGNORE:

error:
	no-valid-program
EOF

cat <<EOF > $tmp2
no-valid-program
sh: 1: no-valid-program: not found
make: error: don't know how to make hello
EOF

scc-make -f $tmp3 error hello > $tmp1 2>&1

diff $tmp1 $tmp2
