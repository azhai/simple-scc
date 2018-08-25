#ifdef stdin
extern FILE *_fpopen(const char * restrict fname,
                     const char * restrict mode,
                     FILE * restrict fp);
#endif

/* TODO: Move to system headers */
#define O_RDONLY  0x00000000
#define O_WRONLY  0x00000001
#define O_RDWR    0x00000002

#define O_TRUNC   0x00000400
#define O_APPEND  0x00000008
#define O_CREAT   0x00000200
