#include <string.h>

#include <arch/sigaction.h>
#include <sys.h>

extern void _sigreturn(void);

int _sigaction(int sig, struct sigaction *sa, struct sigaction *old)
{
	struct sigaction ksa, *p = NULL;

	if (sa) {
		ksa.sa_handler = sa->sa_handler;
		ksa.sa_flags = sa->sa_flags | SA_RESTORER;
		ksa.sa_restorer = _sigreturn;
		memcpy(&ksa.sa_mask, &sa->sa_mask, sizeof(ksa.sa_mask));
		p = &ksa;
	}

	if (__sigaction(sig, p, old, sizeof(ksa.sa_mask)) < 0)
		return -1;

	return 0;
}
