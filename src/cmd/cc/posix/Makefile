.POSIX:

PREFIX = /usr/local
PROJECTDIR = ../../../..
include $(PROJECTDIR)/scripts/rules.mk

# SYSLST is a list of arch-abi-sys-format.
# By default the driver uses the configuration
# inhered from the environment, that in a normal
# build is derived from the host configuration.

SYSLST  =\
	amd64-sysv-linux-elf\
	z80-scc-none-none\
	i386-sysv-linux-elf\
	amd64-sysv-openbsd-elf\

TARGETS =\
	$(BINDIR)/scc-cc\
	$(BINDIR)/scc-cpp\
	$(BINDIR)/scc\

all: $(TARGETS)

$(BINDIR)/scc-cc: $(LIBSCC) cc.o
	$(CC) $(PROJ_LDFLAGS) cc.o -lscc -o $@

$(BINDIR)/scc-cpp: scc.sh
	cp scc.sh $@
	chmod +x $@

$(BINDIR)/scc: scc.sh
	cp scc.sh $@
	chmod +x $@

config.h:
	set -e;\
	rm -f $@;\
	trap "rm -f $$$$.h" EXIT QUIT HUP TERM;\
	(echo '#define PREFIX "$(PREFIX)"';\
	 echo '#define ARCH "$(ARCH)"';\
	 echo '#define SYS  "$(SYS)"';\
	 echo '#define ABI  "$(ABI)"';\
	 echo '#define FORMAT "$(FORMAT)"') > $$$$.h && mv $$$$.h $@

clean:
	rm -f scc scpp *.o
	rm -f $(TARGETS)
	rm -f config.h

include deps.mk
