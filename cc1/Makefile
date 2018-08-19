.POSIX:

PROJECTDIR = ..

include $(PROJECTDIR)/rules.mk
include $(LIBSCC)/libdep.mk

MORECFLAGS = -I$(INCDIR)/$(STD) $(STDCFLAGS)

OBJ = types.o decl.o lex.o error.o symbol.o main.o expr.o \
      code.o stmt.o cpp.o fold.o init.o builtin.o

TARGETS   = $(LIBEXEC)/cc1-amd64-sysv $(LIBEXEC)/cc1-arm64-sysv \
            $(LIBEXEC)/cc1-i386-sysv  $(LIBEXEC)/cc1-z80-scc

all: $(TARGETS)

dep:
	$(PROJECTDIR)/mkdep.sh

clean:
	rm -f *.o
	rm -f target/*/*.o
	rm -f cc1-*
	rm -f $(TARGETS)

include target/amd64-sysv/arch.mk
include target/arm64-sysv/arch.mk
include target/i386-sysv/arch.mk
include target/z80-scc/arch.mk
include deps.mk
