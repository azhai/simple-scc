#!/bin/sh

trap 'rm -f $tmp1 $tmp2 t? t' EXIT INT TERM QUIT HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

cat > $tmp2 <<EOF
touch t
touch t1
touch t2
EOF

scc-make -f- <<'EOF' > $tmp1 2>&1
all: t1 t2

t1 t2: t
t1 t2 t:
	touch $@
EOF

diff $tmp1 $tmp2
