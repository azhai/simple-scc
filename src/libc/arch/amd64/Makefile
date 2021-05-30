.POSIX:

DIRS =\
	netbsd\
	openbsd\
	dragonfly\
	linux\
	darwin\

PROJECTDIR =../../../..
include $(PROJECTDIR)/scripts/rules.mk
include ../../rules.mk

OBJS =\
	longjmp.$O\
	setjmp.$O\
	memcpy.$O\

all: $(OBJS) $(SYS)

include deps.mk
