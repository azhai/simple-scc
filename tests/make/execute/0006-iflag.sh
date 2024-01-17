#!/bin/sh

set -e

trap 'rm -f $tmp1 $tmp2 $tmp3' EXIT INT QUIT HUP TERM

tmp1=tmp1.$$
tmp2=tmp2.$$
tmp3=tmp3.$$

cat <<EOF > $tmp3
.IGNORE:

error:
	no-valid-program
EOF

cat <<EOF > $tmp2
sh: 1: no-valid-program: not found
-i
no-valid-program
sh: 1: no-valid-program: not found
-i
no-valid-program
sh: 1: no-valid-program: not found
no-valid-program
sh: 1: no-valid-program: not found
no-valid-program
Hello World!
EOF

(scc-make -if test.mk error print-makeflags
 MAKEFLAGS=-i scc-make -f test.mk error print-makeflags
 scc-make -f test.mk ignored-error
 scc-make -f $tmp3 error
 scc-make hello) > $tmp1 2>&1

diff $tmp1 $tmp2
