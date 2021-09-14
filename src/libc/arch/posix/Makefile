.POSIX:
PROJECTDIR =../../../..
RULES = user
include $(PROJECTDIR)/scripts/rules.mk
include $(SRCDIR)/libc/rules.mk

OBJS=\
	_getheap.$O\
	_open.$O\
	_systime.$O\
	_tzone.$O\
	clock.$O\
	getenv.$O\
	raise.$O\
	signal.$O\
	time.$O\

all: $(OBJS)

include deps.mk
