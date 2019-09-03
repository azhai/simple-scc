.POSIX:
PROJECTDIR =../../../..
include $(PROJECTDIR)/scripts/rules.mk
include ../../rules.mk

DIRS = netbsd openbsd dragonfly linux darwin

all: $(OBJS) $(SYS)

$(SYS): FORCE
	+@cd $@ && $(MAKE)

clean:
	$(FORALL)
