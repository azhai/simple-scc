.POSIX:

PROJECTDIR = ../..
LIBDIR = $(PROJECTDIR)/lib/scc
include $(PROJECTDIR)/rules.mk
include $(LIBDIR)/libdep.mk

OBJ = scc.o
HDR = config.h \
      $(PROJECTDIR)/inc/scc.h \
      $(PROJECTDIR)/inc/arg.h \
      $(PROJECTDIR)/inc/syslibs.h \
      $(PROJECTDIR)/inc/ldflags.h

all: scc cpp
	mkdir -p $(PROJECTDIR)/rootdir/bin
	cp scc cpp $(PROJECTDIR)/rootdir/bin/

dep:
	PREFIX=$(PREFIX) USEQBE=$(USEQBE) ./gendep.sh $(TARGETS)

$(OBJ): $(HDR)

scc: $(OBJ) $(LIBDIR)/libscc.a
	$(CC) $(SCC_LDFLAGS) $(OBJ) -lscc -o $@

cpp: cpp.sh config.h
	set -x ;\
	trap "rm -f $$$$.sh" 0 2 3;\
	rm -f $@ ;\
	sed "s%@PREFIX@%$(PREFIX)%" < cpp.sh > $$$$.sh && \
	chmod +x $$$$.sh && \
	mv $$$$.sh $@

clean:
	rm -f $(OBJ)
	rm -f scc
	rm -f scpp

distclean: clean
	rm -f config.h

