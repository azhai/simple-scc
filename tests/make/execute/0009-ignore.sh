#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT QUIT HUP TERM

tmp1=tmp1.$$
tmp2=tmp2.$$

cat <<EOF > $tmp2
no-valid-program
sh: 1: no-valid-program: not found
make: error: don't know how to make hello
EOF

scc-make -f - error hello > $tmp1 2>&1 <<EOF
.IGNORE:

error:
	no-valid-program
EOF

diff $tmp1 $tmp2
