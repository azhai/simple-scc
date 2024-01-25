#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT QUIT HUP TERM

tmp1=tmp1.$$
tmp2=tmp2.$$

cat <<EOF > $tmp2
-k
no-valid-program
sh: no-valid-program: not found
make: warning: target error: error 32512
Hello World!
make: warning: target follow not remade because of errors
EOF

(set -e
 scc-make -kf test.mk print-makeflags follow
 echo fail) 2>&1 |
sed 's/:.*: *no-/: no-/' > $tmp1

diff $tmp1 $tmp2
