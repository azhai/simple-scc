extern void *_brk(void *addr);
extern int _open(const char *path, int flags);
extern int _close(int fd);
extern int _read(int fd, void *buf, size_t n);
extern int _write(int fd, void *buf, size_t n);
extern int _lseek(int fd, long off, int whence);
extern void _Exit(int status);
extern int _access(char *path, int mode);

/* auxiliar functions */

#ifdef stdin
extern FILE *_fpopen(const char * restrict fname,
                     const char * restrict mode,
                     FILE * restrict fp);
#endif

#define O_WRONLY  1
#define O_RDONLY  2
#define O_RDWR    4
#define O_TRUNC   8
#define O_APPEND 16
#define O_CREAT  32
