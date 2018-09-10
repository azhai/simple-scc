include $(LIBCOFF32)/objlst.mk

$(LIBDIR)/libcoff32.a: $(LIBCOFF32-OBJ)
	+cd $(LIBCOFF32) && $(MAKE)
