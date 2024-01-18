#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT QUIT TERM HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

echo "make: error: don't know how to make no-file" > $tmp2

scc-make no-file > $tmp1 2>&1

diff $tmp1 $tmp2
