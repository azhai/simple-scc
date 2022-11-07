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

all: config
	+@$(MAKE) `$(SCRIPTDIR)/config -c` toolchain
	+@$(MAKE) `$(SCRIPTDIR)/config` `uname -m`

config:
	cd include/scc/scc && $(MAKE) $@

install: all
	$(SCRIPTDIR)/install $(ROOT)
	+@$(MAKE) install-`uname -m`

uninstall:
	$(SCRIPTDIR)/uninstall $(ROOT)
	+@$(MAKE) uninstall-`uname -m`

toolchain: src
libc: src/libc
libcrt: src/libcrt
src: dirs include/scc/scc
src/libc: dirs
src/libcrt: dirs

dirs: $(SCRIPTDIR)/libc-dirs
	xargs mkdir -p < $(SCRIPTDIR)/libc-dirs
	touch dirs

clean:
	xargs rm -rf < $(SCRIPTDIR)/libc-dirs
	rm -f dirs

distclean: clean
	rm -f include/scc/scc/sysld.h
	rm -f include/scc/scc/sysincludes.h

include scripts/amd64.mk
include scripts/arm.mk
include scripts/arm64.mk
include scripts/i386.mk
include scripts/ppc.mk
