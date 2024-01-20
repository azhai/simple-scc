#!/bin/sh

trap 'rm -f f.c f.o f' EXIT INT QUIT TERM HUP

echo hello > f.c

scc-make -sf- <<'EOF'
.c:
	echo hello > $@

all: f
EOF

diff f.c f
