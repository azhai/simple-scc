#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT TERM QUIT HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

cat >$tmp2 <<EOF
Hello World!  
EOF

scc-make -f- <<'EOF' > $tmp1 2>&1
VAR = Hello World!  # This is a comment
all:
	@echo '$(VAR)'
EOF

diff $tmp1 $tmp2
