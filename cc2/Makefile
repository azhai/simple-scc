.POSIX:

PROJECTDIR = ..
include $(PROJECTDIR)/rules.mk
include $(LIBSCC)/libdep.mk

MORECFLAGS = -I$(PROJECTDIR)/inc/$(STD)

OBJ = main.o parser.o peep.o symbol.o node.o code.o optm.o

TARGETS  = $(LIBEXEC)/cc2-amd64-sysv  $(LIBEXEC)/cc2-i386-sysv \
           $(LIBEXEC)/cc2-qbe_amd64-sysv $(LIBEXEC)/cc2-z80-scc

all: $(TARGETS)

dep:
	$(PROJECTDIR)/mkdep.sh

error.h: cc2.h
	rm -f $@; trap 'rm -f $$$$.h' EXIT INT QUIT ;\
	awk -f generror.awk cc2.h > $$$$.h && mv $$$$.h $@

clean:
	rm -f *.o error.h
	rm -f target/*/*.o
	rm -f $(TARGETS)

include target/amd64-sysv/target.mk
include target/i386-sysv/target.mk
include target/qbe_amd64-sysv/target.mk
include target/qbe_arm64-sysv/target.mk
include target/z80-scc/target.mk
include deps.mk
