.POSIX:

PROJECTDIR = ../../..
include $(PROJECTDIR)/scripts/rules.mk

SYSHDR = ldflags.h\
         syscrts.h\
         sysincludes.h\
         syslibs.h \
         cstd.h \

all: $(SYSHDR)

cstd.h: cstd-$(STD).h
	cp cstd-$(STD).h $@

ldflags.h:
	rm -f $@ ; \
	trap "rm -f $$$$.h" 0 2 3; \
	cat < ldflags.def.h > $$$$.h; \
	mv $$$$.h $@

syscrts.h:
	rm -f $@ ; \
	trap "rm -f $$$$.h" 0 2 3; \
	sed 's,PREFIX,"$(PREFIX)",g' < syscrts.def.h > $$$$.h && \
	mv $$$$.h $@

sysincludes.h:
	rm -f $@ ; \
	trap "rm -f $$$$.h" 0 2 3; \
	sed 's,PREFIX,"$(PREFIX)",g' < sysincludes.def.h > $$$$.h && \
	mv $$$$.h $@

syslibs.h:
	rm -f $@ ; \
	trap "rm -f $$$$.h" 0 2 3; \
	sed 's,PREFIX,"$(PREFIX)",g' < syslibs.def.h > $$$$.h && \
	mv $$$$.h $@

distclean:
	rm -f $(SYSHDR)
