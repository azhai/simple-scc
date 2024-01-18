#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT QUIT TERM HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

echo pcc > $tmp2

scc-make -f test.mk CC=pcc print-env-cc > $tmp1 2>&1

diff $tmp1 $tmp2
