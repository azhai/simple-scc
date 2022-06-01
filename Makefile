.POSIX:

DIRS  =\
	src\
	src/libc\
	src/libcrt\
	include/scc/scc\
	tests\

PROJECTDIR = .
include $(PROJECTDIR)/scripts/rules.mk

ROOT = $(DESTDIR)$(PREFIX)
NODEP = 1

all:
	+@$(MAKE) `$(SCRIPTDIR)/config -c` toolchain
	+@$(MAKE) `$(SCRIPTDIR)/config` `uname -m`

install:
	$(SCRIPTDIR)/install $(ROOT)
	+@$(MAKE) install-`uname -m`

uninstall:
	$(SCRIPTDIR)/uninstall $(ROOT)
	+@$(MAKE) uninstall-`uname -m`

#############################################################
# x86_64 rules

x86_64 amd64:
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-linux libc libcrt
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-openbsd libc libcrt
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-netbsd libc libcrt
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-dragonfly libc libcrt
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-darwin libc libcrt

install-x86_64 install-amd64: amd64
	$(SCRIPTDIR)/install -p $(SCRIPTDIR)/proto.amd64 $(ROOT)

uninstall-x86_64 uninstall-amd64:
	$(SCRIPTDIR)/uninstall -p $(SCRIPTDIR)/proto.amd64 $(ROOT)

#############################################################
# i386 rules

i386:
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=i386-linux libc libcrt

install-i386: i386
	$(SCRIPTDIR)/install -p $(SCRIPTDIR)/proto.i386 $(ROOT)

uninstall-i386:
	$(SCRIPTDIR)/uninstall -p $(SCRIPTDIR)/proto.i386 $(ROOT)

#############################################################
# ppc rules

ppc:
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=ppc-linux libc libcrt

install-ppc: ppc
	$(SCRIPTDIR)/install -p $(SCRIPTDIR)/proto.ppc $(ROOT)

uninstall-ppc:
	$(SCRIPTDIR)/uninstall -p $(SCRIPTDIR)/proto.ppc $(ROOT)

#############################################################
# arm rules

arm:
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=arm-linux libc libcrt

install-arm: arm
	$(SCRIPTDIR)/install -p $(SCRIPTDIR)/proto.arm $(ROOT)

uninstall-arm:
	$(SCRIPTDIR)/uninstall -p $(SCRIPTDIR)/proto.arm $(ROOT)

#############################################################
# arm64 rules

arm64:
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=arm64-linux libc libcrt

install-arm64: arm64
	$(SCRIPTDIR)/install -p $(SCRIPTDIR)/proto.arm64 $(ROOT)

uninstall-arm64:
	$(SCRIPTDIR)/uninstall -p $(SCRIPTDIR)/proto.arm64 $(ROOT)

#############################################################

toolchain: src
libc: src/libc
libcrt: src/libcrt

tests: all
src: include/scc/scc

src: dirs
src/libc: dirs
src/libcrt: dirs

dirs: $(SCRIPTDIR)/libc-dirs
	xargs mkdir -p < $(SCRIPTDIR)/libc-dirs
	touch dirs

clean:
	xargs rm -rf < $(SCRIPTDIR)/libc-dirs
	rm -f dirs
