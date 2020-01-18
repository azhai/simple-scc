.POSIX:
PROJECTDIR =../../../..
include $(PROJECTDIR)/scripts/rules.mk
include ../../rules.mk

OBJS = longjmp.$O setjmp.$O
DIRS = linux

all: $(LIBC) $(SYS)

$(LIBCLST): $(OBJS)
	$(MKLST)

$(SYS): FORCE
	+@cd $@ && $(MAKE)

dep clean:
	$(FORALL)
