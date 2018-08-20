.POSIX:

PROJECTDIR = ..

include $(PROJECTDIR)/rules.mk

DIRS = scc coff32 crt c

all: $(DIRS)

$(DIRS): FORCE
	+@cd $@ && $(MAKE)

dep clean distclean:
	$(FORALL)
