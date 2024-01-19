#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT TERM QUIT HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

echo Hello World!> $tmp2

scc-make -f- all <<'EOF' > $tmp1 2>&1
VAR = guy

all:
	@echo Hello $(VAR)!
VAR = World
EOF

diff $tmp1 $tmp2
