enum {
	SUN,
	MON,
	TUE,
	WED,
	THU,
	FRI,
	SAT
};

#define JAN 0
#define FEB 1
#define DEC 11

#define FEBDAYS(y) ((_daysyear(y) == 366) ? 29 : 28)
#define EPOCH 1970
#define MINYEAR 1900
#define SECMIN 60
#define SECHOUR (60 * SECMIN)    /* 3600 */
#define SECDAY (24 * SECHOUR)   /* 86400 */

struct tm;

struct tzone {
	char *name;
	int gmtoff;
	int isdst;
};

extern void *_getheap(void);
extern int _dtoi(char c);


#ifdef stdin
extern int _allocbuf(FILE *);
extern int _flsbuf(FILE *);
extern FILE *_fpopen(const char * restrict, const char *restrict,
                     FILE *restrict);
#endif

#ifdef _TIME_H
extern time_t _tzstdoff, _tzdstoff;
extern time_t _tzstart, _tzend;

extern time_t _systime(struct tm *);
#endif

extern void _tzset(void);
extern int _daysyear(int);
extern int _newyear(int);

extern int _tzjulian;
extern int _daysmon[12];
extern char *_tzname[2];
extern struct tzone tzones[];

extern void (*_exitf[_ATEXIT_MAX])(void);
extern unsigned _exitn;
