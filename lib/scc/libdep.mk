include $(LIBSCC)/objlst.mk

$(LIBSCC)/libscc.a: $(LIBSCC-OBJ)
	+cd $(LIBSCC) && $(MAKE)
