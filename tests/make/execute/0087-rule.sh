#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT TERM QUIT HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

echo '#error line' >$tmp2

scc-make -f- <<'EOF' > $tmp1 2>&1
all:
	@echo '#error line'
EOF

diff $tmp1 $tmp2
