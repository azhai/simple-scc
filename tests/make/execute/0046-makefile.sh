#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT QUIT TERM HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

echo Bye world! > $tmp2

scc-make -f test.mk -f - hello <<EOF > $tmp1 2>&1
hello:
	@echo Bye world!
EOF

diff $tmp1 $tmp2
