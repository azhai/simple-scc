#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT QUIT TERM HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

cat >$tmp2 <<EOF
Hello World! Bye World!
EOF

scc-make -f- <<EOF > $tmp1 2>&1
hello:
	@echo Hello World!\
	 Bye World!
EOF

diff $tmp1 $tmp2