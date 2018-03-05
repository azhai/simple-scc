include $(PROJECTDIR)/config.mk

INCDIR  = $(PROJECTDIR)/inc/

SCC_CFLAGS = $(MOREFLAGS) \
             $(SYSCFLAGS) \
             -g \
             $(CFLAGS)

SCC_LDFLAGS = -L$(LIBDIR) -g $(LDFLAGS)

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
