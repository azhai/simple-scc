#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT TERM QUIT HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

echo Hello World! > $tmp2

scc-make -f - <<'EOF' > $tmp1 2>&1
# Generic rules
.SUFFIXES:
.SUFFIXES: .c .map .dump .elf .bin\
	   .i .a .o .s .S .ko .ld .tmpl\
	   .pdf .ps .eps .puml\
	   .ms .1 .2 .3 .4 .5 .6 .7\

all:
	@echo Hello World!
EOF

diff $tmp1 $tmp2
