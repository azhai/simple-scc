#!/bin/sh

trap 'rm -f $tmp1 $tmp2 makefile' EXIT INT QUIT TERM HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

cat > makefile <<EOF
hello:
	@echo Hello World!, makefile!
EOF

cat > $tmp2 <<EOF
Hello World!, makefile!
EOF

scc-make > $tmp1 2>&1

diff $tmp1 $tmp2
