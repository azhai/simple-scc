.POSIX:

DIRS =\
	ar/execute\
	nm/execute\
	as/execute\
	ld/execute\
	size/execute\
	strip/execute\
	libc/execute\
	cc/execute cc/error\

PROJECTDIR = ..
include $(PROJECTDIR)/scripts/rules.mk

all:
	@for i in $(DIRS);\
	do \
		cd $$i;\
		$(MAKE) tests;\
		cd -;\
	done | grep -e PASS -e FAIL -e SKIP;\

dep:
