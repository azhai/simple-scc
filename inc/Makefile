.POSIX:

PROJECTDIR = ..

include $(PROJECTDIR)/rules.mk

HDR = ldflags.h syscrts.h sysincludes.h syslibs.h

all: config

config: $(HDR)

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
	rm -f $(HDR)
