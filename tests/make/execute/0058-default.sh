#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT QUIT TERM HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

echo Hello World! > $tmp2

scc-make -f- no-target <<EOF > $tmp1 2>&1
.DEFAULT:
	@echo Hello World!
EOF

diff $tmp1 $tmp2
