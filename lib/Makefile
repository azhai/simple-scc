.POSIX:

PROJECTDIR = ..

include $(PROJECTDIR)/rules.mk

DIRS = scc coff32

all: $(DIRS)

$(DIRS): FORCE
	+@cd $@ && $(MAKE)

dep clean distclean:
	$(FORALL)
