include objs/common-objs.mk

OBJS =\
	$(COMMON_OBJS)\
	arch/amd64/openbsd/_brk.$O\
	arch/amd64/openbsd/_close.$O\
	arch/amd64/openbsd/_exit.$O\
	arch/amd64/openbsd/_getpid.$O\
	arch/amd64/openbsd/_gettimeofday.$O\
	arch/amd64/openbsd/_kill.$O\
	arch/amd64/openbsd/_lseek.$O\
	arch/amd64/openbsd/_open.$O\
	arch/amd64/openbsd/_read.$O\
	arch/amd64/openbsd/_sigaction.$O\
	arch/amd64/openbsd/_wait4.$O\
	arch/amd64/openbsd/_write.$O\
	arch/amd64/openbsd/crt.$O\
	arch/amd64/openbsd/_sys_errlist.$O\
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
	arch/posix/_getheap.$O\
	arch/posix/_open.$O\
	arch/posix/_systime.$O\
	arch/posix/_tzone.$O\
	arch/posix/clock.$O\
	arch/posix/getenv.$O\
	arch/posix/raise.$O\
	arch/posix/signal.$O\
	arch/posix/time.$O\
	string/strlen.$O\
