.POSIX:

PROJECTDIR = ../..
include $(PROJECTDIR)/rules.mk
include $(LIBSCC)/libdep.mk
include $(INCDIR)/incdep.mk

# SYSLST is a list of backend-arch-abi-sys. First
# element of the list becomes the default target

SYSLST  = amd64-sysv-linux-elf z80-scc-none-none \
          i386-sysv-linux-elf amd64-sysv-openbsd-elf

TARGETS = $(BINDIR)/scc $(BINDIR)/scpp

all: $(TARGETS)

$(BINDIR)/scc: scc
	cp scc $@

$(BINDIR)/scpp: cpp
	cp cpp $@

scc: scc.o $(LIBSCC)/libscc.a
	$(CC) $(SCC_LDFLAGS) scc.o -lscc -o $@

cpp: cpp.sh config.h
	set -x ;\
	trap "rm -f $$$$.sh" 0 2 3;\
	rm -f $@ ;\
	sed "s%@PREFIX@%$(PREFIX)%" < cpp.sh > $$$$.sh && \
	chmod +x $$$$.sh && \
	mv $$$$.sh $@

config.h:
	PREFIX=$(PREFIX) ./config.sh $(SYSLST)

dep:
	$(PROJECTDIR)/mkdep.sh

clean:
	rm -f scc scpp *.o
	rm -f $(TARGETS)
	rm -f config.h

include deps.mk
