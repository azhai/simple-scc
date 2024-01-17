#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT QUIT HUP TERM

tmp1=tmp1.$$
tmp2=tmp2.$$

cat > $tmp2 <<'EOF'

FC = fort77
YFLAGS = 
ARFLAGS = -rv
LDFLAGS = 
LEX = lex
MAKEFLAGS = 
CFLAGS = -O
YACC = yacc
CC = c99
LFLAGS = 
AR = ar
SHELL = /bin/sh
FFLAGS = -O 1
.f.a:
	${FC} -c ${FFLAGS} $<
	${AR} ${ARFLAGS} $@ $*.o
	rm -f $*.o

.l.o:
	${LEX} ${LFLAGS} $<
	${CC} ${CFLAGS} -c lex.yy.c
	rm -f lex.yy.c
	mv lex.yy.o $@

.c.a:
	${CC} -c ${CFLAGS} $<
	${AR} ${ARFLAGS} $@ $*.o
	rm -f $*.o

.c:
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $<

.c.o:
	${CC} ${CFLAGS} -c $<

.l.c:
	${LEX} ${LFLAGS} $<
	mv lex.yy.c $@

.f.o:
	${FC} ${FFLAGS} -c $<

.f:
	${FC} ${FFLAGS} ${LDFLAGS} -o $@ $<

.y.o:
	${YACC} ${YFLAGS} $<
	${CC} ${CFLAGS} -c y.tab.c
	rm -f y.tab.c
	mv y.tab.o $@

.y.c:
	${YACC} ${YFLAGS} $<
	mv y.tab.c $@

.SUFFIXES: .o .c .y .l .a .sh .f

all:
	@echo $(MAKEFLAGS)

.sh:
	cp $< $@
	chmod a+x $@

EOF

scc-make -pf - > $tmp1 2>&1 <<'EOF'
all:
	@echo $(MAKEFLAGS)
EOF

diff $tmp1 $tmp2
