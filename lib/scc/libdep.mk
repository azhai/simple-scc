include $(LIBDIR)/objlst.mk

$(LIBDIR)/libscc.a: $(LIB-OBJ)
	+cd $(LIBDIR) && $(MAKE)
