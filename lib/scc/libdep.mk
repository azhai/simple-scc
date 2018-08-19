include $(LIBSCC)/objlst.mk

$(LIBDIR)/libscc.a: $(LIBSCC-OBJ)
	+@cd $(LIBSCC) && $(MAKE)
