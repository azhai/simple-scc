x86_64 amd64:
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-linux libc libcrt
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-openbsd libc libcrt
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-netbsd libc libcrt
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-dragonfly libc libcrt
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-darwin libc libcrt

install-x86_64 install-amd64: amd64
	$(SCRIPTDIR)/install -p $(SCRIPTDIR)/proto.amd64 $(ROOT)

uninstall-x86_64 uninstall-amd64:
	$(SCRIPTDIR)/uninstall -p $(SCRIPTDIR)/proto.amd64 $(ROOT)
