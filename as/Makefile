.POSIX:

PROJECTDIR = ..
include $(PROJECTDIR)/rules.mk
include $(LIBSCC)/libdep.mk

OBJ = main.o symbol.o ins.o parser.o expr.o myro.o
MORECFLAGS = -I$(INCDIR)/$(STD)
TARGETS = $(LIBEXEC)/as-amd64 $(LIBEXEC)/as-i386 \
          $(LIBEXEC)/as-i286 $(LIBEXEC)/as-z80

all: $(TARGETS)

$(TARGETS): $(LIBDIR)/libscc.a

dep:
	$(PROJECTDIR)/mkdep.sh

clean:
	rm -f *.o target/*/*.o
	rm -f target/*/*tbl.c
	rm -f as-*
	rm -f $(TARGETS)

include target/amd64.mk
include target/i386.mk
include target/i286.mk
include target/z80.mk
include deps.mk
