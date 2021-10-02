#include <stddef.h>
#include <sys.h>

typedef struct ucontext_t ucontext_t;

extern int _sigaction2(int,
                       struct sigaction *, struct sigaction *,
                       const void *,
                       int);

extern int _setcontext(ucontext_t *);

int
_sigaction(int sig, struct sigaction *new, struct sigaction *old)
{
	return _sigaction2(sig, new, old, _setcontext, 2);
}
