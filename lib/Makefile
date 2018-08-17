.POSIX:

PROJECTDIR = ..

include $(PROJECTDIR)/rules.mk

DIRS = scc

all: $(DIRS)

FORCE:

$(DIRS): FORCE
	+@cd $@ && $(MAKE)

dep clean distclean:
	$(FORALL)
