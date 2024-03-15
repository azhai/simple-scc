aarch64 arm64:
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=arm64-linux libc libcrt

install-arm64 install-aarch64: arm64
	$(SCRIPTDIR)/install -p $(SCRIPTDIR)/proto.arm64 $(ROOT)

uninstall-arm64 uninstall-aarch64:
	$(SCRIPTDIR)/uninstall -p $(SCRIPTDIR)/proto.arm64 $(ROOT)
