# scc - Suckless C Compiler
.POSIX:

PROJECTDIR = .

include rules.mk

DIRS  = inc cc1 cc2 driver lib as ar nm objdump ld

all:
	$(FORALL)

clean:
	$(FORALL)

distclean:
	touch config.mk    # we need config.mk for makes in $DIRS
	find . -name makefile | xargs rm -f
	$(FORALL)
	rm -rf rootdir
	rm -f config.mk

dep: config.mk
	$(FORALL)

tests: all
	+cd tests && $(MAKE) -e all

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/
	cp -r rootdir/* $(DESTDIR)$(PREFIX)/
	cd $(DESTDIR)$(PREFIX)/libexec/scc/ && chmod 755 cc*
	cd $(DESTDIR)$(PREFIX)/bin && chmod 755 scc cpp
	mkdir -p $(DESTDIR)$(MANPREFIX)/man1
	sed "s/VERSION/$(VERSION)/g" < doc/scc.1 > $(DESTDIR)$(MANPREFIX)/man1/scc.1
	chmod 644 $(DESTDIR)$(MANPREFIX)/man1/scc.1

uninstall:
	rm -rf $(DESTDIR)$(PREFIX)/libexec/scc/
	rm -rf $(DESTDIR)$(PREFIX)/lib/scc/
	rm -f $(DESTDIR)$(PREFIX)/bin/scc
	rm -f $(DESTDIR)$(PREFIX)/bin/scpp
	rm -f $(DESTDIR)$(MANPREFIX)/man1/scc.1
