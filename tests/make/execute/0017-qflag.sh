#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT QUIT TERM HUP

tmp1=tmp1.$$
tmp2=tmp3.$$

touch $tmp2

(set -e
 scc-make -qf test.mk error
 echo fail) > $tmp1 2>&1

diff $tmp1 $tmp2
