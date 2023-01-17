i386:
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=i386-linux src/libc src/libcrt

install-i386: i386
	$(SCRIPTDIR)/install -p $(SCRIPTDIR)/proto.i386 $(ROOT)

uninstall-i386:
	$(SCRIPTDIR)/uninstall -p $(SCRIPTDIR)/proto.i386 $(ROOT)
