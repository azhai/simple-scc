.POSIX:

DIRS =\
	arch\
	assert\
	ctype\
	locale\
	stdio\
	stdlib\
	string\
	time\

PROJECTDIR =../..
include $(PROJECTDIR)/scripts/rules.mk
include rules.mk


all: $(LIBC)

$(LIBC): $(DIRS)
	$(AR) $(PROJ_ARFLAGS) $@ `cat $(LIBCLST)`
	$(RL) $(PROJ_RLFLAGS) $@

rm-lst: FORCE
	rm -f $(LIBCLST)

dep:
	$(FORALL)

clean: rm-lst
	$(FORALL)
