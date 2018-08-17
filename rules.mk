include $(PROJECTDIR)/config.mk

BINDIR  = $(PROJECTDIR)/rootdir/bin
INCDIR  = $(PROJECTDIR)/inc/
LIBEXEC = $(PROJECTDIR)/rootdir/libexec/scc/
LIBSCC  = $(PROJECTDIR)/lib/scc

SCC_CFLAGS = $(MOREFLAGS) \
             $(SYSCFLAGS) \
             -g \
             $(CFLAGS)

SCC_LDFLAGS = -L$(LIBSCC) -g $(LDFLAGS)

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

.c:
	$(CC) $(SCC_CFLAGS) $(SCC_LDFLAGS) -o $@ $<

all:
dep:
distclean: clean
