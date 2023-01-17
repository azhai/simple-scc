.POSIX:

DIRS  =\
	src\
	src/libc\
	src/libcrt\
	include/scc/scc\
	tests\

PROJECTDIR = .
include scripts/rules.mk

ROOT = $(DESTDIR)$(PREFIX)
NODEP = 1

all: toolchain libc

config:
	cd include/scc/scc && $(MAKE) $@

install: all
	$(SCRIPTDIR)/install $(ROOT)
	+@$(MAKE) install-`uname -m`

uninstall:
	$(SCRIPTDIR)/uninstall $(ROOT)
	+@$(MAKE) uninstall-`uname -m`

toolchain: config dirs include/scc/scc
	+@$(MAKE) `$(SCRIPTDIR)/config -c` src

libc: config dirs
	+@$(MAKE) `$(SCRIPTDIR)/config -c` `uname -m`

libcrt: config dirs
	+@$(MAKE) `$(SCRIPTDIR)/config -c` src/libcrt

dirs: $(SCRIPTDIR)/libc-dirs
	xargs mkdir -p < $(SCRIPTDIR)/libc-dirs
	touch dirs

clean:
	xargs rm -rf < $(SCRIPTDIR)/libc-dirs
	find . -name '*.gcno' -o -name '*.gcda' | xargs rm -f
	rm -rf dirs coverage

distclean: clean
	rm -f include/scc/scc/sysld.h
	rm -f include/scc/scc/sysincludes.h

include scripts/amd64.mk
include scripts/arm.mk
include scripts/arm64.mk
include scripts/i386.mk
include scripts/ppc.mk
