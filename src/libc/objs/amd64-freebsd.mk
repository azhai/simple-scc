include objs/common-objs.mk

OBJS =\
	$(COMMON_OBJS)\
	arch/amd64/freebsd/_exit.$O\
	arch/amd64/freebsd/crt.$O\
	arch/amd64/longjmp.$O\
	arch/amd64/memchr.$O\
	arch/amd64/memcmp.$O\
	arch/amd64/memcpy.$O\
	arch/amd64/memmove.$O\
	arch/amd64/memset.$O\
	arch/amd64/setjmp.$O\
	arch/amd64/strchr.$O\
	arch/amd64/strcmp.$O\
	arch/amd64/strcpy.$O\
	arch/posix/_open.$O\
	arch/posix/_systime.$O\
	arch/posix/_tzone.$O\
	arch/posix/clock.$O\
	arch/posix/getenv.$O\
	arch/posix/raise.$O\
	arch/posix/signal.$O\
	arch/posix/system.$O\
	arch/posix/time.$O\
	arch/posix/tmpfile.$O\
	string/strlen.$O\
