.POSIX:

PROJECTDIR = .
include $(PROJECTDIR)/scripts/rules.mk

DIRS  = src include/scc/scc tests

all: src

src: dirs include/scc/scc

dirs: $(SCRIPTDIR)/libc-proto
	xargs mkdir -p < $(SCRIPTDIR)/libc-proto
	touch dirs

$(DIRS): FORCE 
	+@cd $@ && $(MAKE)

dep:
	$(FORALL)

clean:
	$(FORALL)
	rm -rf lib bin libexec dirs

distclean: clean
	+@cd include/scc/scc && $(MAKE) distclean

tests: all
