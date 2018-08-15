.POSIX:

PROJECTDIR = ..

include $(PROJECTDIR)/rules.mk
include $(LIBDIR)/libdep.mk

MOREFLAGS = -I$(PROJECTDIR)/inc/$(STD) $(CC1_CFLAGS)

OBJ = types.o decl.o lex.o error.o symbol.o main.o expr.o \
      code.o stmt.o cpp.o fold.o init.o builtin.o

all:
	cp cc1-* $(PROJECTDIR)/rootdir/libexec/scc/

dep:
	$(PROJECTDIR)/mkdep.sh

clean:
	rm -f *.o
	rm -f target/*/*.o
	rm -f cc1-*

distclean: clean

include target/amd64-sysv/arch.mk
include target/arm64-sysv/arch.mk
include target/i386-sysv/arch.mk
include target/z80-scc/arch.mk
include deps.mk
