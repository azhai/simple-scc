#!/bin/sh

trap 'rm $tmp1 $tmp2' EXIT INT QUIT TERM HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

echo Hello World! > $tmp2

scc-make -f - <<'EOF' > $tmp1
all:
	@echo Hello World!
# blank line
EOF

diff $tmp1 $tmp2
