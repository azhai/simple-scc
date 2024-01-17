#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT QUIT HUP TERM

tmp1=tmp1.$$
tmp2=tmp2.$$

cat <<EOF > $tmp2
sh: 1: no-valid-program: not found
make: warning: target error: error 32512
Hello World!
make: warning: target follow not remade because of errors
no-valid-program
sh: 1: no-valid-program: not found
make: warning: target error: error 32512
Hello World!
make: warning: target follow not remade because of errors
-k
no-valid-program
EOF

(set -e
 scc-make -kf test.mk follow
 echo fail) > $tmp1 2>&1

(set -e
 MAKEFLAGS=-k scc-make  -f test.mk follow print-makeflags
 echo fail) >> $tmp1 2>&1

diff $tmp1 $tmp2
