.POSIX:

PROJECTDIR = ../../..
include $(PROJECTDIR)/scripts/rules.mk

MORE_CFLAGS = -I$(INCDIR)/$(STD)

OBJS = \
	main.o\
	symbol.o\
	ins.o\
	parser.o\
	expr.o\

TARGET =\
	$(LIBEXEC)/scc/as-powerpc64\
	$(LIBEXEC)/scc/as-powerpc\
	$(LIBEXEC)/scc/as-amd64\
	$(LIBEXEC)/scc/as-i386\
	$(LIBEXEC)/scc/as-i286\
	$(LIBEXEC)/scc/as-z80 \

all: $(TARGET)

$(TARGET): $(LIBSCC)

dep: inc-dep

clean:
	rm -f target/*/*.o target/*/*tbl.c

include target/powerpc/powerpc64.mk
include target/powerpc/powerpc.mk
include target/x86/amd64.mk
include target/x86/i386.mk
include target/x86/i286.mk
include target/x80/z80.mk
include deps.mk
