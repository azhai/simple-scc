.POSIX:
PROJECTDIR =../../../..
include $(PROJECTDIR)/scripts/rules.mk
include ../../rules.mk

OBJS = longjmp.$O setjmp.$O
DIRS = netbsd openbsd dragonfly linux darwin

all: $(LIBC) $(SYS)

$(LIBC): $(OBJS)
	$(MKLST)

$(SYS): FORCE
	+@cd $@ && $(MAKE)

dep clean:
	$(FORALL)
