#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT QUIT TERM HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

echo SHELL=/bin/sh > $tmp2
SHELL=foo scc-make -f- <<'EOF'
all:
	@echo $(SHELL)
EOF
