.POSIX:

PROJECTDIR = .
include $(PROJECTDIR)/scripts/rules.mk

PREFIX= /usr/local
DIRS  = src include/scc/scc tests

all: src

src: dirs include/scc/scc

dirs: $(SCRIPTDIR)/libc-proto
	xargs mkdir -p < $(SCRIPTDIR)/libc-proto
	touch dirs

$(DIRS): $(ENVIRON) FORCE
	+@. $(ENVIRON) && cd $@ && $(MAKE)

$(ENVIRON):
	@rm -f $@; \
	trap 'r=$?;rm -f $$$$.tmp;exit $r' EXIT HUP INT QUIT TERM; \
	echo PATH=$$PATH:$$PWD/$(SCRIPTDIR):. > $$$$.tmp && \
	echo PREFIX=\"$(PREFIX)\" >> $$$$.tmp && \
	echo NM=\"$(NM)\" >> $$$$.tmp && \
	echo AR=\"$(AR)\" >> $$$$.tmp && \
	echo RL=\"$(RL)\" >> $$$$.tmp && \
	echo STD=\"$(STD)\" >> $$$$.tmp && \
	echo ARFLAGS=\"$(ARFLAGS)\" >> $$$$.tmp && \
	echo RLFLAGS=\"$(RLFLAGS)\" >> $$$$.tmp && \
	echo export PATH STD ARFLAGS RLFLAGS NM AR RL >> $$$$.tmp && \
	mv $$$$.tmp $@

dep:
	$(FORALL)

install: all
	$(SCRIPTDIR)/install $(PREFIX)

distclean: clean
	$(MAKE) $(ENVIRON)
	$(FORALL)
	rm -f $(ENVIRON)

clean: $(ENVIRON)
	$(FORALL)
	xargs rm -rf < $(SCRIPTDIR)/libc-proto
	rm -f dirs $(ENVIRON)

tests: all
