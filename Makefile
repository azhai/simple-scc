.POSIX:

DIRS  =\
	src\
	src/libc\
	src/libcrt\
	include/scc/scc\
	tests\

PROJECTDIR = .
include $(PROJECTDIR)/scripts/rules.mk

PREFIX= /usr/local
NODEP = 1

all:
	+@$(MAKE) `$(SCRIPTDIR)/config` toolchain
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-linux libc libcrt
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-openbsd libc libcrt
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-netbsd libc libcrt
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-dragonfly libc libcrt
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-darwin libc libcrt

toolchain: dirs src

libc: dirs src/libc
libcrt: dirs src/libcrt

src: include/scc/scc

dirs: $(SCRIPTDIR)/libc-proto
	xargs mkdir -p < $(SCRIPTDIR)/libc-proto
	touch dirs

install: all
	$(SCRIPTDIR)/install $(PREFIX)

clean:
	xargs rm -rf < $(SCRIPTDIR)/libc-proto
	rm -f dirs

tests: all
