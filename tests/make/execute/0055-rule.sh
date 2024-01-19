#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT QUIT TERM HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

cat > $tmp2 <<EOF
Hello World!
bye
fail
EOF

(set -e
 scc-make -if- <<'EOF'
.POSIX:

all:
	@echo Hello World!;\
	false;\
	echo bye
EOF
echo fail) > $tmp1 2> /dev/null

diff $tmp1 $tmp2
