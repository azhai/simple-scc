.POSIX:
PROJECTDIR =../../../..
include $(PROJECTDIR)/scripts/rules.mk
include ../../rules.mk

OBJS = longjmp.$O setjmp.$O
DIRS = netbsd openbsd dragonfly linux darwin

all: $(LIBC) $(SYS)

$(LIBCLST): $(OBJS)
	$(MKLST)

$(SYS): FORCE
	+@cd $@ && $(MAKE)

clean:
	$(FORALL)
