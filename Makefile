.POSIX:

DIRS  =\
	src\
	src/libc\
	include/scc/scc\
	tests\

PROJECTDIR = .
include $(PROJECTDIR)/scripts/rules.mk

PREFIX= /usr/local
NODEP = 1

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

install: all
	$(SCRIPTDIR)/install $(PREFIX)

clean:
	xargs rm -rf < $(SCRIPTDIR)/libc-proto
	rm -f dirs

tests: all
