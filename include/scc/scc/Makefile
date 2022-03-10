.POSIX:

PROJECTDIR = ../../..
include $(PROJECTDIR)/scripts/rules.mk

SYSHDR =\
	ldflags.h\
	syscrts.h\
	sysincludes.h\
	syslibs.h\
	cstd.h\

NODEP = 1

all: $(SYSHDR)

cstd.h: cstd-$(STD).h
	cp cstd-$(STD).h $@

ldflags.h:
	set -e;\
	rm -f $@;\
	trap "rm -f $$$$.tmp" INT QUIT TERM HUP;\
	sed 's/%NOPIE%/"$(NOPIE_LDFLAGS)",/' ldflags.def.h | \
	sed 's/"",//' > $$$$.tmp && \
	mv $$$$.tmp $@

syscrts.h:
	cp syscrts.def.h $@

sysincludes.h:
	cp sysincludes.def.h $@

syslibs.h:
	cp syslibs.def.h $@

clean:
	rm -f $(SYSHDR)
