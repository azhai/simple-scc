.POSIX:

PROJECTDIR = ..
include $(PROJECTDIR)/scripts/rules.mk

LIBS  = libscc libc libcrt libmach
DIRS  = cmd $(LIBS)

all: cmd

cmd: $(LIBS)

$(DIRS): FORCE
	+@cd $@ && $(MAKE)

dep clean:
	$(FORALL)
