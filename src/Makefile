.POSIX:

PROJECTDIR = ..
include $(PROJECTDIR)/scripts/rules.mk

TOOLS = cc1 cc2 ld as nm objdump
LIBS  = libscc libc libcrt libmach
DIRS  = $(TOOLS) $(LIBS)

all: $(TOOLS)

$(TOOLS): $(LIBS)

$(DIRS): FORCE
	+@cd $@ && $(MAKE)

dep clean:
	$(FORALL)
