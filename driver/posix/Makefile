.POSIX:

PROJECTDIR = ../..
include $(PROJECTDIR)/rules.mk
include $(LIBSCC)/libdep.mk

all: scc cpp
	cp scc cpp $(PROJECTDIR)/rootdir/bin/

scc: scc.o $(LIBSCC)/libscc.a
	$(CC) $(SCC_LDFLAGS) scc.o -lscc -o $@

cpp: cpp.sh config.h
	set -x ;\
	trap "rm -f $$$$.sh" 0 2 3;\
	rm -f $@ ;\
	sed "s%@PREFIX@%$(PREFIX)%" < cpp.sh > $$$$.sh && \
	chmod +x $$$$.sh && \
	mv $$$$.sh $@

dep:
	PREFIX=$(PREFIX) USEQBE=$(USEQBE) ./config.sh $(TARGETS)
	$(PROJECTDIR)/mkdep.sh

clean:
	rm -f scc scpp *.o

distclean:
	rm -f config.h

include deps.mk
