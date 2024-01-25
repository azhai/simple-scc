#!/bin/sh

trap 'rm -f $tmp1 $tmp2 f f1 f2 f3' EXIT INT TERM QUIT HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

echo f3 > $tmp2

touch -t 200711121015 f1 f2
touch -t 200711121016 f
touch -t 200711121017 f3

(scc-make -f- > $tmp1 2>&1) <<EOF
f: f1 f2 f3
	@echo $?
EOF

diff $tmp1 $tmp2
