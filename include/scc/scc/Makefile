.POSIX:

PROJECTDIR = ../../..
include $(PROJECTDIR)/scripts/rules.mk

SYSHDR =\
	sysld.h\
	sysincludes.h\
	cstd.h\

NODEP = 1

all: $(SYSHDR)

cstd.h: cstd-$(STD).h
	cp cstd-$(STD).h $@

sysld.h:
	set -e;\
	rm -f $@;\
	trap "rm -f $$$$.tmp" INT QUIT TERM HUP;\
	sed -e 's/%NOPIE%/"$(NOPIE_LDFLAGS)",/' \
	    -e 's/"",//'  sysld.def.h > $$$$.tmp && \
	mv $$$$.tmp $@

sysincludes.h:
	cp sysincludes.def.h $@

clean:
	rm -f $(SYSHDR)
