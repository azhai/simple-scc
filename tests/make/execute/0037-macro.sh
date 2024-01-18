#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT QUIT TERM HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

echo ../../../bin/scc-make > $tmp2

../../../bin/scc-make -f test.mk print-make > $tmp1 2>&1

diff $tmp1 $tmp2
