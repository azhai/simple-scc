.POSIX:
PROJECTDIR =../../../..
include $(PROJECTDIR)/scripts/rules.mk

OBJS = longjmp.o setjmp.o
DIRS = netbsd openbsd dragonfly linux

all: $(OBJS) $(SYS)

$(SYS): FORCE
	+@cd $@ && $(MAKE)

clean:
	$(FORALL)
