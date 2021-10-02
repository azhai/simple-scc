typedef unsigned int sigset_t;
typedef struct siginfo siginfo_t;

struct sigaction {
	union {
		void (*__sa_handler)(int);
		void (*__sa_sigaction)(int, siginfo_t *, void *);
	} __sigaction_u;

	sigset_t sa_mask;
	int sa_flags;
};

#define sa_handler      __sigaction_u.__sa_handler
#define sa_sigaction    __sigaction_u.__sa_sigaction
