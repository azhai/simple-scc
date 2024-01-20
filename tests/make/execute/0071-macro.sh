#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT QUIT TERM

tmp1=tmp1.$$
tmp2=tmp2.$$

echo CC=pcc > $tmp2

CC=dcc MAKEFLAGS='-S CC=gcc' scc-make -f- CC=pcc <<'EOF' > $tmp1 2>&1
CC=tcc

all:
	@echo CC=$(CC)
EOF

diff $tmp1 $tmp2
