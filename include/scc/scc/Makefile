.POSIX:

PROJECTDIR = ../../..
include $(PROJECTDIR)/scripts/rules.mk

SYSHDR =\
	sysld.h\
	sysincludes.h\
	cstd.h\

NODEP = 1

all: cstd.h

cstd.h: cstd-$(STD).h
	cp cstd-$(STD).h $@

config: sysld.h sysincludes.h

sysld.h:
	set -e;\
	rm -f $@;\
	trap "rm -f $$$$.tmp" INT QUIT TERM HUP;\
	sed -e 's/%NOPIE%/"$(NOPIE_LDFLAGS)",/' \
	    -e 's/"",//' sysld.$(LIBPROFILE).h > $$$$.tmp && \
	mv $$$$.tmp $@

sysincludes.h:
	cp sysincludes.$(LIBPROFILE).h $@

clean:
	rm -f cstd.h
