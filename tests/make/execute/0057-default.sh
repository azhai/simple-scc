#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT QUIT TERM HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

echo make: error: DEFAULT rule with prerequisites > $tmp2

scc-make -f- <<EOF > $tmp1 2>&1
.DEFAULT: all
	@echo Hello World!
EOF

diff $tmp1 $tmp2
