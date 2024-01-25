#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT QUIT TERM HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

cat > $tmp2 <<EOF
-S -k
no-valid-program
sh: no-valid-program: not found
make: warning: target error: error 32512
Hello World!
make: warning: target follow not remade because of errors
EOF

MAKEFLAGS=-S scc-make -kf test.mk print-makeflags follow 2>&1 |
sed 's/:.*: *no-/: no-/' > $tmp1

diff $tmp1 $tmp2
