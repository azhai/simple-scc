#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT QUIT TERM HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

echo Hello World! > $tmp2

scc-make -f- hello <<'EOF' > $tmp1 2>&1
FILE = test.mk

include $(FILE)
EOF

diff $tmp1 $tmp2
