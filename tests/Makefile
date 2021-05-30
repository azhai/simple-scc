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

all clean:
	for i in $(DIRS);\
	do \
		cd $$i;\
		$(MAKE) $@;\
		cd -;\
	done

dep:
