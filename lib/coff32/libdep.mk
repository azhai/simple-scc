include $(LIBCOFF32)/objlst.mk

$(LIBCOFF32)/libcoff32.a: $(LIBCOFF32-OBJ)
	+cd $(LIBCOFF32) && $(MAKE)
