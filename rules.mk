include $(PROJECTDIR)/config.mk

INCLUDE    = $(PROJECTDIR)/inc

BINDIR     = $(PROJECTDIR)/root/bin
INCDIR     = $(PROJECTDIR)/root/include/scc
LIBDIR     = $(PROJECTDIR)/root/lib/scc
LIBEXEC    = $(PROJECTDIR)/root/libexec/scc

LIBSCC     = $(PROJECTDIR)/lib/scc
LIBCOFF32  = $(PROJECTDIR)/lib/coff32

SCC_CFLAGS = $(MORECFLAGS) $(SYSCFLAGS) $(STDCFLAGS) -g $(CFLAGS)

SCC_LDFLAGS = $(MORELFLAGS) $(SYSLDFLAGS) -L$(LIBDIR) -g $(LDFLAGS)

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
clean:
distclean: clean
FORCE:
