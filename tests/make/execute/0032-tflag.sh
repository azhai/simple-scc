#!/bin/sh

trap 'rm -f $tmp1 $tmp2 file1' EXIT INT TERM QUIT HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

touch $tmp2

scc-make -stf test.mk file1 > $tmp1 2>&1

diff $tmp1 $tmp2 && test -f file1
