#!/bin/sh

trap 'rm -f $tmp1 $tmp2 f.c f.o' EXIT INT TERM QUIT HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

echo dude > f.c
echo hello > $tmp2

(scc-make -f- <<'EOF'
f.o:
	@echo hello > $@

all: f.o
EOF
cat f.o) > $tmp1 2>&1

diff $tmp1 $tmp2
