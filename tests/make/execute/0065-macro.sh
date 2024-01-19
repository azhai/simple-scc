#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT TERM QUIT HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

echo Hello World!> $tmp2

scc-make -f- all <<'EOF' > $tmp1 2>&1
VAR = VAR2

$(VAR) = World

all:
	@echo Hello $(VAR2)!
EOF

diff $tmp1 $tmp2
