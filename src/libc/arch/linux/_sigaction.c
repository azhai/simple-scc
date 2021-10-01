#include <string.h>

#include <arch/sigaction.h>
#include <sys.h>

extern void _sigreturn(void);

int _sigaction(int sig, struct sigaction *sa, struct sigaction *old)
{
	int r;
	struct sigaction ksa, kold;

	if (sa) {
		ksa.sa_handler = sa->sa_handler;
		ksa.sa_flags = sa->sa_flags | SA_RESTORER;
		ksa.sa_restorer = _sigreturn;
		memcpy(&ksa.sa_mask, &sa->sa_mask, sizeof(ksa.sa_mask));
	}

	r = __sigaction(sig,
	                sa ? &ksa : NULL,
	                old ? &kold : NULL,
	                sizeof(ksa.sa_mask));

	if (r != 0)
		return -1;


	if (old) {
		old->sa_handler = kold.sa_handler;
		old->sa_flags = kold.sa_flags;
		memcpy(&old->sa_mask, &kold.sa_mask, sizeof kold.sa_mask);
	}

	return 0;
}
