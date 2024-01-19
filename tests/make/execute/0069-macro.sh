#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT TERM QUIT HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

echo Hello World! > $tmp2

scc-make -f- <<'EOF' > $tmp1 2>&1
VAR1 = one
VAR = Hello $(VAR1)
VAR1 = two

all:
	@echo $(VAR)!
VAR1 = World

EOF

diff $tmp1 $tmp2
