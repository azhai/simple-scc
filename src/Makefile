.POSIX:

LIBS  =\
	libcrt\
	libscc\
	libmach\

DIRS  =\
	cmd\
	$(LIBS)\

PROJECTDIR = ..
include $(PROJECTDIR)/scripts/rules.mk

all: cmd

cmd: $(LIBS)

dep clean:
	$(FORALL)
