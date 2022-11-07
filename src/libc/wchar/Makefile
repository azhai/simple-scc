.POSIX:
PROJECTDIR =../../..
include $(PROJECTDIR)/scripts/rules.mk
include ../rules.mk

OBJS =\
	mbrlen.$O\
	mbrtowc.$O\
	mbsrtowcs.$O\
	wcrtomb.$O\
	wcslen.$O\
	wcwidth.$O\
	_validutf8.$O\

all: $(OBJS)

include deps.mk
