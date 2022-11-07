ppc:
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=ppc-linux libc libcrt

install-ppc: ppc
	$(SCRIPTDIR)/install -p $(SCRIPTDIR)/proto.ppc $(ROOT)

uninstall-ppc:
	$(SCRIPTDIR)/uninstall -p $(SCRIPTDIR)/proto.ppc $(ROOT)
