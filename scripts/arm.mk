arm:
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=arm-linux src/libc src/libcrt

install-arm: arm
	$(SCRIPTDIR)/install -p $(SCRIPTDIR)/proto.arm $(ROOT)

uninstall-arm:
	$(SCRIPTDIR)/uninstall -p $(SCRIPTDIR)/proto.arm $(ROOT)
