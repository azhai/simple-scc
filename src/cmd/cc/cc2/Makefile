.POSIX:

PROJECTDIR = ../../../..
include $(PROJECTDIR)/scripts/rules.mk

MORE_CPPFLAGS = -I$(INCDIR)/$(STD) -I$(CMDDIR) -I.

OBJS =\
	main.o\
	parser.o\
	peep.o\
	symbol.o\
	node.o\
	code.o\
	optm.o\

TARGET  =\
	$(LIBEXEC)/scc/cc2-amd64-sysv\
	$(LIBEXEC)/scc/cc2-i386-sysv\
	$(LIBEXEC)/scc/cc2-qbe_amd64-sysv\
	$(LIBEXEC)/scc/cc2-qbe_arm64-sysv\
	$(LIBEXEC)/scc/cc2-z80-scc\

all: $(TARGET)

main.o: error.h

error.h: cc2.h
	rm -f $@;\
	trap 'rm -f $$$$.h' EXIT INT QUIT TERM HUP;\
	awk -f generror.awk cc2.h > $$$$.h && mv $$$$.h $@

clean:
	rm -f target/*/*.o error.h

include target/amd64-sysv/target.mk
include target/i386-sysv/target.mk
include target/qbe_amd64-sysv/target.mk
include target/qbe_arm64-sysv/target.mk
include target/z80-scc/target.mk
include deps.mk
