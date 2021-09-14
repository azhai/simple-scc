.POSIX:

PROJECTDIR = ../../../..
DIRS = \
	linux\

include $(PROJECTDIR)/scripts/rules.mk
include ../../rules.mk

OBJS =\
	longjmp.$O\
	setjmp.$O\

all: $(OBJS) $(SYS)

include deps.mk
