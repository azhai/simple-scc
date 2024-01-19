#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT QUIT TERM HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

echo Hello World! > $tmp2

scc-make -f- <<'EOF' > $tmp1 2>&1
F = World
G = Hello

all:
	@echo $G $(F)!
EOF

diff $tmp1 $tmp2
