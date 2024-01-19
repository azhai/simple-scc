#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT QUIT TERM HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

echo Hello World! > $tmp2

echo 'all: ; @echo Hello World!' |
scc-make -f- > $tmp1 2>&1

diff $tmp1 $tmp2
