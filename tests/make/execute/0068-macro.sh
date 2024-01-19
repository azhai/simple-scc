#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT TERM QUIT HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

echo file1.o file2.o > $tmp2

scc-make -f- <<'EOF' > $tmp1 2>&1
SRC = file1.c file2.c

all:
	@echo $(SRC:.c=.o)
EOF

diff $tmp1 $tmp2
