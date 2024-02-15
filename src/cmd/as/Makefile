.POSIX:

PROJECTDIR = ../../..
include $(PROJECTDIR)/scripts/rules.mk

MORE_CFLAGS = -I$(INCDIR)/$(STD)

OBJS = \
	binfmt.o\
	expr.o\
	ins.o\
	main.o\
	parser.o\
	symbol.o\

TARGET =\
	$(BINDIR)/scc-as\
	$(LIBEXEC)/scc/as-amd64\
	$(LIBEXEC)/scc/as-i286\
	$(LIBEXEC)/scc/as-i386\
	$(LIBEXEC)/scc/as-powerpc\
	$(LIBEXEC)/scc/as-powerpc64\
	$(LIBEXEC)/scc/as-z80 \

MORE_LDLIBS =  -lscc -lmach

all: $(TARGET)

$(TARGET): $(LIBSCC) $(LIBMACH)

$(BINDIR)/scc-as: as
	cp as $@

genhash.o: ../../libscc/genhash.c
	$(HOSTCC) -c ../../libscc/genhash.c

lexh.o: lexh.c
	$(HOSTCC) -c lexh.c

lexh: lexh.o genhash.o
	$(HOSTCC) -o $@ lexh.o genhash.o

clean:
	rm -f target/*/*.o target/*/*tbl.c lexh
	rm -f as

include target/powerpc/powerpc64.mk
include target/powerpc/powerpc.mk
include target/x86/amd64.mk
include target/x86/i386.mk
include target/x86/i286.mk
include target/x80/z80.mk
include deps.mk
