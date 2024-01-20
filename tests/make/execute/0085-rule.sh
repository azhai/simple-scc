#!/bin/sh

trap 'rm -f $tmp1 $tmp2 t1.c t?.o t2' EXIT INT TERM HUP QUIT

tmp1=tmp1.$$
tmp2=tmp2.$$

cat > $tmp2 <<EOF
t1
t2
t2
EOF

echo Hello > t1.c

scc-make -f- <<'EOF' > $tmp1 2>&1
.c.o:
	@echo $* > $@

all: t1.o t2.o t2
	@cat t1.o t2.o t2

t2.o:
	@echo $* > $@

t2:
	@echo $* > $@
EOF

diff $tmp1 $tmp2
