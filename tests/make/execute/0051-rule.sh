#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT QUIT TERM HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

echo Hello World! > $tmp2

(set -e
 scc-make -f- <<'EOF'
.POSIX:

all:
	@echo Hello World!;\
	false;\
	echo bye
EOF
echo fail) > $tmp1 2> /dev/null

diff $tmp1 $tmp2
