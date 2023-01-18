x86_64 amd64:
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-linux src/libc src/libcrt
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-openbsd src/libc src/libcrt
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-netbsd src/libc src/libcrt
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-dragonfly src/libc src/libcrt
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-darwin src/libc src/libcrt
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-freebsd src/libc src/libcrt

install-x86_64 install-amd64: amd64
	$(SCRIPTDIR)/install -p $(SCRIPTDIR)/proto.amd64 $(ROOT)

uninstall-x86_64 uninstall-amd64:
	$(SCRIPTDIR)/uninstall -p $(SCRIPTDIR)/proto.amd64 $(ROOT)
