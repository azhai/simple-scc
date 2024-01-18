#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT QUIT TERM HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

echo > $tmp2

MAKEFLAGS='-S CC=pcc' scc-make -f test.mk print-env-cc > $tmp1 2>&1

diff $tmp1 $tmp2
