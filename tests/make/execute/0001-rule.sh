#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT QUIT HUP TERM
tmp1=tmp1.$$
tmp2=tmp2.$$

scc-make hello > $tmp1 2>&1
echo Hello World! > $tmp2

diff $tmp1 $tmp2
