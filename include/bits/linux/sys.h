#define O_RDONLY  0x00000000
#define O_WRONLY  0x00000001
#define O_RDWR    0x00000002

#define O_TRUNC   0x00000200
#define O_APPEND  0x00000400
#define O_CREAT   0x00000040

#define CLOCKS_PER_SEC ((clock_t) 1000000)
#define RUSAGE_SELF 0

typedef int pid_t;

struct sigaction {
	void (*sa_handler)(int);
	int sa_mask;
	int sa_flags;
};

extern pid_t _getpid(void);
extern int _kill(pid_t, int);
extern int _sigaction(int, struct sigaction *, struct sigaction *);
