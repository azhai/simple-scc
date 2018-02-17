.POSIX:

PROJECTDIR = ..
# fallback case if DRIVER isn't defined
DRIVER = posix

include $(PROJECTDIR)/rules.mk

all dep clean distclean:
	+cd $(DRIVER) && $(MAKE) $@
