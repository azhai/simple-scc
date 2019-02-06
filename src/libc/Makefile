.POSIX:
PROJECTDIR =../..
include $(PROJECTDIR)/scripts/rules.mk
include rules.mk

DIRS = arch\
       assert\
       ctype\
       locale\
       stdio\
       stdlib\
       string\
       time\

all: $(DIRS) objlst.mk
	+@$(MAKE) -f Makefile.libc

$(DIRS): FORCE
	+@cd $@ && $(MAKE)

objlst.mk: $(DIRS) FORCE
	mklst $@

clean:
	$(FORALL)
	rm -f objlst.mk
