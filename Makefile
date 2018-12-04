.POSIX:

PROJECTDIR = .
include $(PROJECTDIR)/scripts/rules.mk

DIRS  = src include/scc/scc tests

all: src

src: dirs include/scc/scc

dirs: $(SCRIPTDIR)/libc-proto
	xargs mkdir -p < $(SCRIPTDIR)/libc-proto
	touch dirs

$(DIRS): FORCE 
	+@cd $@ && $(MAKE)

dep:
	$(FORALL)

clean:
	$(FORALL)
	rm -rf lib bin libexec dirs

distclean: clean
	+@cd include/scc/scc && $(MAKE) distclean

tests: all

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/
	cp -r root/* $(DESTDIR)$(PREFIX)/
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
