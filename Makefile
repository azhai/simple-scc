.POSIX:

PROJECTDIR = .
include $(PROJECTDIR)/scripts/rules.mk

DIRS  = src include/scc/scc tests
ENV   = $(SCRIPTDIR)/env.sh

all: src

src: dirs include/scc/scc

dirs: $(SCRIPTDIR)/libc-proto
	xargs mkdir -p < $(SCRIPTDIR)/libc-proto
	touch dirs

$(DIRS): $(ENV) FORCE
	. $(ENV) && cd $@ && $(MAKE)

$(ENV):
	@rm -f $@; \
	trap 'r=$?;rm -f $$$$.tmp;exit $r' EXIT HUP INT QUIT TERM; \
	(echo PATH=$$PWD/scripts:$$PATH:.; \
	 echo NM=\"$(NM)\"; \
	 echo AR=\"$(AR)\"; \
	 echo RL=\"$(RL)\"; \
	 echo RLFLAGS=\"$(RLFLAGS)\"; \
	 echo export PATH RLFLAGS NM AR RL) > $$$$.tmp && mv $$$$.tmp $@

dep:
	$(FORALL)

clean:
	$(FORALL)
	rm -rf lib bin libexec dirs $(ENV)

distclean: clean
	+@cd include/scc/scc && $(MAKE) distclean

tests: all
