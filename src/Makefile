.POSIX:

PROJECTDIR = ..
include $(PROJECTDIR)/scripts/rules.mk

TOOLS = cc1 cc2 ld as nm objdump
LIBS  = libscc libcoff32
DIRS  = $(TOOLS) $(LIBS) libc libcrt

all: $(TOOLS) crt

crt: libcrt libc $(TOOLS)

$(TOOLS): $(LIBS)

$(DIRS): FORCE
	+@cd $@ && $(MAKE)

dep clean:
	$(FORALL)
