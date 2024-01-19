#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT QUIT TERM HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

cat > $tmp2 <<'EOF'
make: error: <stdin>:4: DEFAULT rule with prerequisites
EOF

scc-make -f- <<EOF > $tmp1 2>&1
.DEFAULT: all
	@echo Hello World!
EOF

diff $tmp1 $tmp2
