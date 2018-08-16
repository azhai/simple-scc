.POSIX:

PROJECTDIR = ..
include $(PROJECTDIR)/rules.mk
include $(LIBSCC)/libdep.mk

OBJ = main.o symbol.o ins.o parser.o expr.o myro.o
MOREFLAGS = -I$(INCDIR)/$(STD) $(AS_CFLAGS)

all: as-amd64 as-i386 as-i286 as-z80
	cp as-* $(PROJECTDIR)/rootdir/bin

dep:
	$(PROJECTDIR)/mkdep.sh

clean:
	rm -f *.o target/*/*.o
	rm -f target/*/*tbl.c
	rm -f as-*

include target/amd64.mk
include target/i386.mk
include target/i286.mk
include target/z80.mk
include deps.mk
