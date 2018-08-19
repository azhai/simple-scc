include $(PROJECTDIR)/config.mk

BINDIR     = $(PROJECTDIR)/rootdir/bin
INCDIR     = $(PROJECTDIR)/inc
LIBDIR     = $(PROJECTDIR)/rootdir/lib
LIBEXEC    = $(PROJECTDIR)/rootdir/libexec/scc
LIBSCC     = $(PROJECTDIR)/lib/scc
LIBCOFF32  = $(PROJECTDIR)/lib/coff32

SCC_CFLAGS = $(MORECFLAGS) $(SYSCFLAGS) $(STDCFLAGS) -g $(CFLAGS)

SCC_LDFLAGS = $(MORELFLAGS) -L$(LIBDIR) -g $(LDFLAGS)

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
