CONF=amd64-linux
include $(PROJECTDIR)/config/$(CONF).mk

LIBDIR     = $(PROJECTDIR)/lib/scc
SCRIPTDIR  = $(PROJECTDIR)/scripts
INCDIR     = $(PROJECTDIR)/include

BINDIR     = $(PROJECTDIR)/bin
LIBEXEC    = $(PROJECTDIR)/libexec/scc
CRTDIR     = $(PROJECTDIR)/lib/scc

INCLUDE    = -I$(INCDIR)/scc \

RL	   = $(RANLIB)

SCC_CFLAGS = $(MORECFLAGS) \
             $(SYSCFLAGS) \
             $(INCLUDE) \
             -g \
             $(CFLAGS)

SCC_LDFLAGS = $(MORELFLAGS) \
              $(SYSLDFLAGS) \
              -L$(LIBDIR) \
              -g \
              $(LDFLAGS)

# helper macro to run over all the directories
FORALL = +@set -e ;\
	pwd=$$PWD; \
	for i in $(DIRS); \
	do \
		cd $$i; \
		$(MAKE) $@; \
		cd $$pwd; \
	done

.s.o:
	$(AS) $< -o $@

.c.o:
	$(CC) $(SCC_CFLAGS) -o $@ -c $<

all:

dep:

inc-dep: FORCE
	$(SCRIPTDIR)/mkdep

clean: clean-helper

clean-helper:
	rm -f *.o $(OBJS) $(TARGET)

FORCE:
