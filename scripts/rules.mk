CONF=amd64-linux
TOOL=unix
include $(PROJECTDIR)/config/$(CONF).mk
include $(PROJECTDIR)/config/toolchain/$(TOOL).mk

LIBDIR     = $(PROJECTDIR)/lib/scc
SCRIPTDIR  = $(PROJECTDIR)/scripts
INCDIR     = $(PROJECTDIR)/include

BINDIR     = $(PROJECTDIR)/bin
LIBEXEC    = $(PROJECTDIR)/libexec/scc
CRTDIR     = $(PROJECTDIR)/lib/scc
LIBCDIR    = $(CRTDIR)/$(ARCH)-$(SYS)/
ENVIRON    = $(SCRIPTDIR)/env.sh

INCLUDE    = -I$(INCDIR)/scc \

CC = $(CROSS_COMPILE)$(COMP)
AS = $(CROSS_COMPILE)$(ASM)
LD = $(CROSS_COMPILE)$(LINKER)
RL = $(CROSS_COMPILE)$(RANLIB)
AR = $(CROSS_COMPILE)$(ARCHIVE)

SCC_CFLAGS = $(MORECFLAGS) \
             $(TOOLCFLAGS) \
             $(SYSCFLAGS) \
             $(INCLUDE) \
             -g \
             $(CFLAGS)

SCC_LDFLAGS = $(MORELFLAGS) \
              $(TOOLLDFLAGS) \
              $(SYSLDFLAGS) \
              -L$(LIBDIR) \
              -g \
              $(LDFLAGS)

SCC_ASFLAGS = $(MOREASFLAGS) \
              $(TOOLASFLAGS) \
              $(SYSASFLAGS) \
              $(ASFLAGS)

# helper macro to run over all the directories
FORALL = +@set -e ;\
	pwd=$$PWD; \
	. $(ENVIRON); \
	for i in $(DIRS); \
	do \
		cd $$i; \
		$(MAKE) $@; \
		cd $$pwd; \
	done

.o:
	$(CC) $(SCC_LDFLAGS) -o $@ $< $(LIBS)

.s.o:
	$(AS) $(SCC_ASFLAGS) $< -o $@

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
