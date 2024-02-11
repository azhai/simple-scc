.POSIX:

PROJECTDIR = ../../../..
include $(PROJECTDIR)/scripts/rules.mk

TARGETS =\
	$(BINDIR)/scc-cc\
	$(BINDIR)/scc-cpp\
	$(BINDIR)/scc\

all: $(TARGETS)

$(BINDIR)/scc-cc: $(LIBSCC) cc.o
	$(CC) $(PROJ_LDFLAGS) cc.o -lscc $(PROJ_LDLIBS) -o $@

$(BINDIR)/scc-cpp: scc.sh
	cp scc.sh $@
	chmod +x $@

$(BINDIR)/scc: scc.sh
	cp scc.sh $@
	chmod +x $@

clean:
	rm -f scc scpp *.o
	rm -f $(TARGETS)

include deps.mk
