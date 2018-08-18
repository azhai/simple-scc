.POSIX:

PROJECTDIR = ..

include $(PROJECTDIR)/rules.mk
include inclst.mk

all: config

config: $(SYSHDR)

$(INCDIR)/ldflags.h:
	rm -f $@ ; \
	trap "rm -f $$$$.h" 0 2 3; \
	cat < ldflags.def.h > $$$$.h; \
	mv $$$$.h $@

$(INCDIR)/syscrts.h:
	rm -f $@ ; \
	trap "rm -f $$$$.h" 0 2 3; \
	sed 's,PREFIX,"$(PREFIX)",g' < syscrts.def.h > $$$$.h && \
	mv $$$$.h $@

$(INCDIR)/sysincludes.h:
	rm -f $@ ; \
	trap "rm -f $$$$.h" 0 2 3; \
	sed 's,PREFIX,"$(PREFIX)",g' < sysincludes.def.h > $$$$.h && \
	mv $$$$.h $@

$(INCDIR)/syslibs.h:
	rm -f $@ ; \
	trap "rm -f $$$$.h" 0 2 3; \
	sed 's,PREFIX,"$(PREFIX)",g' < syslibs.def.h > $$$$.h && \
	mv $$$$.h $@

distclean:
	rm -f $(SYSHDR)
