arm64:
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=arm64-linux libc libcrt

install-arm64: arm64
	$(SCRIPTDIR)/install -p $(SCRIPTDIR)/proto.arm64 $(ROOT)

uninstall-arm64:
	$(SCRIPTDIR)/uninstall -p $(SCRIPTDIR)/proto.arm64 $(ROOT)
