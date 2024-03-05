#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT QUIT TERM

tmp1=tmp1.$$
tmp2=tmp2.$$

echo CC=c99 > $tmp2

CC=dcc scc-make -f- <<'EOF' > $tmp1 2>&1
all:
	@echo CC=$(CC)
EOF

diff $tmp1 $tmp2