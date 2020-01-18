.POSIX:
PROJECTDIR =../../..
include $(PROJECTDIR)/scripts/rules.mk
include ../rules.mk

OBJS =\
	localeconv.$O\
	setlocale.$O\

all: $(OBJS)

$(LIBC): $(OBJS)
	$(MKLIB)

dep: inc-dep

include deps.mk
