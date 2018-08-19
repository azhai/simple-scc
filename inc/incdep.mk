include $(INCDIR)/inclst.mk

debug:
	echo $(SYSHDR)

$(SYSHDR):
	+@cd $(INCDIR) && $(MAKE)
