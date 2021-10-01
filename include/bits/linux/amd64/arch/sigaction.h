#define SA_RESTORER   0x04000000

struct sigaction {
        void (*sa_handler)(int);
        unsigned long sa_flags;
        void (*sa_restorer)(void);
        unsigned sa_mask[2];
};

extern int __sigaction(int, struct sigaction *, struct sigaction *, size_t);
