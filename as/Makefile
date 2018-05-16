.POSIX:

PROJECTDIR = ..
include $(PROJECTDIR)/rules.mk
include $(LIBDIR)/libdep.mk

OBJ = main.o symbol.o ins.o parser.o expr.o myro.o
HDR = $(PROJECTDIR)/inc/scc.h as.h
MOREFLAGS = -I$(PROJECTDIR)/inc/$(STD) $(AS_CFLAGS)

all: as-amd64 as-i386 as-z80

$(OBJ): $(HDR)

as: $(OBJ)
	$(CC) $(SCC_LDFLAGS) $(OBJ) -lscc -o $@

myro.o: $(PROJECTDIR)/inc/myro.h

dep:
	$(PROJECTDIR)/mkdep.sh

clean:
	rm -f *.o target/*/*.o
	rm -f target/*/*tbl.c
	rm -f as-*
	rm -f *.log

distclean: clean
	rm -f makefile

include target/amd64.mk
include target/i386.mk
include target/z80.mk

#deps
expr.c: as.h
expr.c: ../inc/scc.h
ins.c: as.h
ins.c: ../inc/scc.h
main.c: as.h
main.c: ../inc/arg.h
main.c: ../inc/scc.h
myro.c: as.h
myro.c: ../inc/myro.h
myro.c: ../inc/scc.h
parser.c: as.h
parser.c: ../inc/scc.h
symbol.c: as.h
symbol.c: ../inc/scc.h
