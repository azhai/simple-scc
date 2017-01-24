# scc - Suckless C Compiler
# See LICENSE file for copyright and license details.
.POSIX:

include config.mk

DIRS  = lib cc1 cc2 driver/$(DRIVER)

all: driver/$(DRIVER)/scc
	for i in $(ARCHS); \
	do \
		$(MAKE) $$i || exit; \
	done

driver/$(DRIVER)/scc: bin
	cd driver/$(DRIVER)/ && $(MAKE) scc
	ln -f driver/$(DRIVER)/scc bin/scc

$(ARCHS): bin
	for i in cc1 cc2; \
	do \
		(cd $$i; \
		ARCH=$@ $(MAKE) -e $$i-$@ || exit); \
	done
	ln -f cc1/cc1-$@ bin/
	ln -f cc2/cc2-$@ bin/

bin:
	mkdir -p bin

tests: all
	cd tests/execute && $(MAKE) -e tests

install: all
	mkdir -p $(DESTDIR)/$(PREFIX)/libexec/scc/
	mkdir -p $(DESTDIR)/$(PREFIX)/bin/
	mkdir -p $(DESTDIR)/$(PREFIX)/include/scc/
	cp -f bin/cc?-* $(DESTDIR)/$(PREFIX)/libexec/scc/
	cp -f bin/cc1-$(ARCH) $(DESTDIR)/$(PREFIX)/bin/cpp
	cp -f bin/scc $(DESTDIR)/$(PREFIX)/bin/
	cp -fr libc/include/* $(DESTDIR)/$(PREFIX)/include/scc/
	find $(DESTDIR)/$(PREFIX)/include/scc/ -type f | xargs chmod 644
	cd $(DESTDIR)/$(PREFIX)/libexec/scc/ && chmod 755 cc* && strip cc*
	cd $(DESTDIR)/$(PREFIX)/bin && chmod 755 cpp scc && strip cpp scc

uninstall:
	rm -rf $(DESTDIR)/$(PREFIX)/include/scc/
	rm -rf $(DESTDIR)/$(PREFIX)/libexec/scc/
	rm -f $(DESTDIR)/$(PREFIX)/bin/scc
	rm -f $(DESTDIR)/$(PREFIX)/bin/cpp

clean-helper:
	for i in $(DIRS); \
	do \
		(cd $$i && $(MAKE) -e clean || exit); \
	done

clean:
	for i in $(ARCHS); \
	do \
		ARCH=$$i $(MAKE) -e clean-helper || exit; \
	done
	rm -f bin/cc* bin/scc

distclean: clean
	rm -rf bin
	rm -f inc/sysincludes.h
