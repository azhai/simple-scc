.POSIX:

PREFIX = /usr/local
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

config.h:
	set -e;\
	rm -f $@;\
	trap "rm -f $$$$.h" EXIT QUIT HUP TERM;\
	(echo '#define PREFIX "$(PREFIX)"';\
	 echo '#define ARCH "$(ARCH)"';\
	 echo '#define SYS  "$(SYS)"';\
	 echo '#define ABI  "$(ABI)"';\
	 echo '#define LIBPREFIX "$(LIBPREFIX)"';\
	 echo '#define FORMAT "$(FORMAT)"') > $$$$.h && mv $$$$.h $@

clean:
	rm -f scc scpp *.o
	rm -f $(TARGETS)
	rm -f config.h

include deps.mk
