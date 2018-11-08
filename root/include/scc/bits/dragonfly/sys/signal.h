typedef int sig_atomic_t;

#define SIG_ERR    ((void (*)(int))-1)
#define SIG_DFL    ((void (*)(int)) 0)
#define SIG_IGN    ((void (*)(int)) 1)

#define SIGINT      2
#define SIGILL      4
#define SIGABRT     6
#define SIGFPE      8
#define SIGSEGV    11
#define SIGTERM    15
