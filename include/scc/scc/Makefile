.POSIX:

PREFIX = /usr/local
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

config: sys.h config.h

config.h:
	rm -f $@;\
	trap "rm -f $$$$.h" EXIT INT QUIT HUP TERM;\
	(echo '#define PREFIX "$(PREFIX)"';\
	 echo '#define ARCH "$(ARCH)"';\
	 echo '#define SYS  "$(SYS)"';\
	 echo '#define ABI  "$(ABI)"';\
	 echo '#define LIBPREFIX "$(LIBPREFIX)"';\
	 echo '#define FORMAT "$(FORMAT)"') > $$$$.h && mv $$$$.h $@

sys.h: sys-$(LIBPROFILE).h
	rm -f $@;\
	trap "rm -f $$$$.tmp" INT QUIT TERM HUP;\
	sed -e 's/%NOPIE%/"$(NOPIE_LDFLAGS)",/' \
	    -e 's/"",//' sys-$(LIBPROFILE).h > $$$$.tmp && \
	mv $$$$.tmp $@

clean:
	rm -f cstd.h
