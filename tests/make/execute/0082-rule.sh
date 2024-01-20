#!/bin/sh

trap 'rm -f $tmp1 $tmp2 f.c f.o f' EXIT INT QUIT TERM HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

echo hello > f.c

cat > $tmp2 <<EOF
make: error: don't know how to make f
EOF

scc-make -sf- <<'EOF' > $tmp1 2>&1

.SUFFIXES:

.c:
	echo hello > $@

all: f
EOF

diff $tmp1 $tmp2
