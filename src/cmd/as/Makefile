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

clean:
	rm -f target/*/*.o target/*/*tbl.c lexh

genhash.o: ../../libscc/genhash.c
	$(HOSTCC) -c ../../libscc/genhash.c

lexh.o: lexh.c
	$(HOSTCC) -c lexh.c

lexh: lexh.o genhash.o
	$(HOSTCC) -o $@ lexh.o genhash.o

include target/powerpc/powerpc64.mk
include target/powerpc/powerpc.mk
include target/x86/amd64.mk
include target/x86/i386.mk
include target/x86/i286.mk
include target/x80/z80.mk
include deps.mk
