# scc - Suckless C Compiler
.POSIX:

PROJECTDIR = .

include rules.mk

DIRS  = inc cc1 cc2 driver lib as ar nm objdump ld

all: $(DIRS)

$(DIRS): config $(BINDIR) $(LIBEXEC) FORCE 
	+@cd $@ && $(MAKE) all

$(BINDIR):
	mkdir -p $@

$(LIBEXEC):
	mkdir -p $@

clean dep:
	$(FORALL)

distclean: unconfig
	$(FORALL)
	rm -f config
	rm -rf rootdir

tests: all
	+@cd tests && $(MAKE) -e all

unconfig:
	(echo '/^### Systems/,$$ v/^#/ s/^/#/' ; echo w) | ed -s config.mk
	rm -f config

config:
	(echo '/^# '`uname`'/+;/^###$$/- s/^#//'; echo w) | ed -s config.mk
	touch $@

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
