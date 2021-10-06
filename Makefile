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
	+@PREFIX=$(PREFIX) $(MAKE) `$(SCRIPTDIR)/config` toolchain
	+@$(MAKE) `$(SCRIPTDIR)/config` `uname -m`

x86_64 amd64: FORCE
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-linux libc libcrt
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-openbsd libc libcrt
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-netbsd libc libcrt
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-dragonfly libc libcrt
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-darwin libc libcrt

i386: FORCE
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=i386-linux libc libcrt

ppc: FORCE
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=ppc-linux libc libcrt

arm: FORCE
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=arm-linux libc libcrt

arm64: FORCE
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=arm64-linux libc libcrt

toolchain: dirs src

libc: dirs src/libc
libcrt: dirs src/libcrt

src: include/scc/scc

dirs: $(SCRIPTDIR)/libc-proto
	xargs mkdir -p < $(SCRIPTDIR)/libc-proto
	touch dirs

install: all
	$(SCRIPTDIR)/install $(PREFIX)

uninstall:
	$(SCRIPTDIR)/uninstall $(PREFIX)

clean:
	xargs rm -rf < $(SCRIPTDIR)/libc-proto
	rm -f dirs

tests: all
