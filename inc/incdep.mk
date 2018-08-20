include $(INCLUDE)/inclst.mk

debug:
	echo $(SYSHDR)

$(SYSHDR):
	+@cd $(INCLUDE) && $(MAKE)
