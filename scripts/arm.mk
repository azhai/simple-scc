arm:
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=arm-linux libc libcrt

install-arm: arm
	$(SCRIPTDIR)/install -p $(SCRIPTDIR)/proto.arm $(ROOT)

uninstall-arm:
	$(SCRIPTDIR)/uninstall -p $(SCRIPTDIR)/proto.arm $(ROOT)
