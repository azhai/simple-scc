.POSIX:

DIRS  =\
	src\
	src/libc\
	include/scc/scc\
	tests\

PROJECTDIR = .
include $(PROJECTDIR)/scripts/rules.mk

PREFIX= /usr/local

all:
	+@$(MAKE) `$(SCRIPTDIR)/config` toolchain
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-linux libc
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-openbsd libc
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-netbsd libc
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-dragonfly libc

toolchain: dirs src

libc: dirs src/libc

src: include/scc/scc

dirs: $(SCRIPTDIR)/libc-proto
	xargs mkdir -p < $(SCRIPTDIR)/libc-proto
	touch dirs

dep:
	$(FORALL)

install: all
	$(SCRIPTDIR)/install $(PREFIX)

distclean: clean
	$(MAKE) $(ENVIRON)
	$(FORALL)
	rm -f $(ENVIRON)

clean: $(ENVIRON)
	$(FORALL)
	xargs rm -rf < $(SCRIPTDIR)/libc-proto
	rm -f dirs $(ENVIRON)

tests: all
