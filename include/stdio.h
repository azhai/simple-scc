#ifndef _STDIO_H
#define _STDIO_H

#define _NEED_NULL
#define _NEED_SIZET
#define _NEED_VA_LIST
#include <sys/stdio.h>
#include <arch/cdefs.h>

#ifndef FOPEN_MAX
#define FOPEN_MAX 12
#endif

#define EOF     -1
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

/**
 * enum _file_flags - internal FILE macros used by stdio
 * @_IOWRITE: write only stream
 * @_IOREAD: read only stream
 * @_IORW: read and write stream
 * @_IOEOF: mark of end of file in the stream
 * @_IOERR: mark of error in the stream
 * @_IOSTRG: string stream
 * @_IOTXT: text stream
 * @_IOFBF: full buffered stream
 * @_IOLBF: line buffered stream
 * @_IONBF: non buffered stream
 * @_IOALLOC: stream with a dynamic allocated buffer
 */
enum _file_flags {
	_IOWRITE = (1 << 0),
	_IOREAD =  (1 << 1),
	_IORW =    (1 << 2),
	_IOEOF =   (1 << 3),
	_IOERR =   (1 << 4),
	_IOSTRG =  (1 << 5),
	_IOTXT =   (1 << 6),
	_IOFBF =   (1 << 7),
	_IOLBF =   (1 << 8),
	_IONBF =   (1 << 9),
	_IOALLOC = (1 <<10),
};

/**
 * struct FILE - opaque structure containing information about a file
 * @fd: file descriptor
 * @buf: pointer to i/o buffer
 * @rp: read pointer
 * @wp: write pointer
 * @lp: write pointer used when line-buffering
 * @len: actual length of buffer
 * @flags: file open mode
 * @unbuf: tiny buffer for unbuffered i/o
 */
typedef struct {
	int fd;
	unsigned char *buf;
	unsigned char *rp;
	unsigned char *wp;
	unsigned char *lp;
	size_t len;
	unsigned short flags;
	unsigned char unbuf[1];
} FILE;

extern FILE __iob[FOPEN_MAX];

#define	stdin  (&__iob[0])
#define	stdout (&__iob[1])
#define	stderr (&__iob[2])

extern int remove(const char *);
extern int rename(const char *, const char *);
extern FILE *tmpfile(void);
extern char *tmpnam(char *);
extern int fclose(FILE *);
extern int fflush(FILE *);
extern FILE *fopen(const char *restrict, const char *restrict);
extern FILE *freopen(const char *restrict, const char *restrict,
                     FILE *restrict);
extern void setbuf(FILE *restrict, char *restrict);
extern int setvbuf(FILE *restrict, char *restrict, int, size_t);
extern int fprintf(FILE *restrict, const char *restrict, ...);
extern int fscanf(FILE *restrict, const char *restrict, ...);
extern int printf(const char *restrict, ...);
extern int scanf(const char *restrict, ...);
extern int snprintf(char *restrict, size_t, const char *restrict, ...);
extern int sprintf(char *restrict, const char *restrict, ...);
extern int sscanf(const char *restrict, const char *restrict, ...);

extern int vfprintf(FILE *restrict, const char *restrict, __va_list);
extern int vfscanf(FILE *restrict, const char *restrict, __va_list);
extern int vprintf(const char *restrict, __va_list);
extern int vscanf(const char *restrict, __va_list);
extern int vsnprintf(char *restrict, size_t, const char *restrict, __va_list);
extern int vsprintf(char *restrict, const char *restrict, __va_list);
extern int vsscanf(const char *restrict, const char *restrict, __va_list);

extern int fgetc(FILE *);
extern char *fgets(char *restrict, int, FILE *restrict);
extern int fputc(int, FILE *);
extern int fputs(const char *restrict, FILE *restrict);
extern int getc(FILE *);
extern int getchar(void);
extern char *gets(char *);
extern int putc(int, FILE *);
extern int putchar(int);
extern int puts(const char *);
extern int ungetc(int, FILE *);
extern size_t fread(void *restrict, size_t, size_t, FILE *restrict);
extern size_t fwrite(const void *restrict, size_t, size_t, FILE *restrict);
extern int fseek(FILE *, long int, int);
extern long int ftell(FILE *);
extern void rewind(FILE *);
extern void clearerr(FILE *);
extern int feof(FILE *);
extern int ferror(FILE *);
extern void perror(const char *);

extern int __getc(FILE *);
extern int __putc(int, FILE *);

#define getc(fp)            ((fp)->rp >= (fp)->wp ? __getc(fp) : *(fp)->rp++)
#define putc(c, fp)         ((fp)->wp >= (fp)->rp ? __putc(c,fp) : (*(fp)->wp++ = c))
#define ferror(fp)          ((fp)->flags & _IOERR)
#define feof(fp)            ((fp)->flags & _IOEOF)
#define clearerr(fp)        (void) ((fp)->flags &= ~(_IOERR|_IOEOF))
#define getchar()           getc(stdin)
#define putchar(c)          putc((c), stdout)
#define setbuf(fp, b)       (void) setvbuf(fp, b, b ? _IOFBF:_IONBF, BUFSIZ)

#endif
