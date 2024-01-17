#include <errno.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "make.h"

#ifndef SIGINT
#define SIGINT -1
#endif

#ifndef SIGTERM
#define SIGTERM -1
#endif

#ifndef SIGQUIT
#define SIGQUIT -1
#endif

#ifndef SIGHUP
#define SIGHUP -1
#endif

int kflag, dflag, nflag, iflag, sflag;
int eflag, pflag, tflag, qflag;
int exitstatus;
sig_atomic_t  stop;

static int hasmake;

void
debug(char *fmt, ...)
{
	va_list va;

	if (!dflag)
		return;

	va_start(va, fmt);
	vfprintf(stdout, fmt, va);
	fputc('\n', stdout);
	va_end(va);
}

int
hash(char *name)
{
	int c;
	unsigned h = 5381;

	while (c = *name++)
		h = h*33 ^ c;

	return h;
}

void *
emalloc(size_t siz)
{
	void *p;

	if ((p = malloc(siz)) == NULL) {
		perror("make");
		exit(EXIT_FAILURE);
	}

	return p;
}

void *
erealloc(void *p, size_t siz)
{
	if ((p = realloc(p, siz)) == NULL) {
		perror("make");
		exit(EXIT_FAILURE);
	}

	return p;
}

char *
estrdup(char *s)
{
	size_t len;

	len = strlen(s) + 1;
	return memcpy(emalloc(len), s, len);
}

static void
sighandler(int signo)
{
	stop = signo;
}

static void
parsedefault(void)
{
	if (parse("makefile"))
		return;
	if (parse("Makefile"))
		return;
}

static void
usage(void)
{
	fputs("usage: make [-eiknprSstd] [-f file] [-j jobs] "
	      "[macro=value ...] [target ...]\n",
	      stderr);
	exit(EXIT_FAILURE);
}

static char *
getarg(char **args, char ***argv)
{
	char *s;

	if ((*args)[1]) {
		s = (*args) + 1;
		*args += strlen(*args) - 1;
		return s;
	}

	if (!argv)
		usage();

	if ((*argv)[1] == NULL)
		usage();
	(*argv)++;

	return **argv;
}

static void
appendmakeflags(char *text)
{
	int n;
	char *s, *t;

	s = getmacro("MAKEFLAGS");
	n = snprintf(NULL, 0, "%s %s", s, text);

	t = emalloc(n+1);
	snprintf(t, n+1, "%s %s", s, text);
	setmacro("MAKEFLAGS", t, EXPORT);

	free(t);
}

static void
parseflag(int flag, char **args, char ***argv)
{
	char *arg;

	switch (flag) {
	case 'f':
		arg = getarg(args, argv);
		if (strcmp(arg, "-") == 0)
			arg = NULL;
		if (!parse(arg))
			error("%s: %s", arg, strerror(errno));
		hasmake = 1;
		break;
	case 'e':
		eflag = 1;
		appendmakeflags("-e");
		break;
	case 'i':
		iflag = 1;
		appendmakeflags("-i");
		break;
	case 'k':
		kflag = 1;
		appendmakeflags("-k");
		break;
	case 'n':
		nflag = 1;
		appendmakeflags("-n");
		break;
	case 'p':
		pflag = 1;
		break;
	case 'q':
		qflag = 1;
		appendmakeflags("-q");
		break;
	case 'r':
		addtarget(".SUFFIXES", 0);
		appendmakeflags("-r");
		break;	
	case 'S':
		kflag = 0;
		appendmakeflags("-S");
		break;
	case 's':
		sflag = 1;
		appendmakeflags("-s");
		break;
	case 't':
		tflag = 1;
		appendmakeflags("-t");
		break;
	case 'd':
		dflag = 1;
		appendmakeflags("-d");
		break;
	case 'j':
	default:
		usage();
	}
}

static int
assign(char *s, int export)
{
	int pos;
	char *t;

	if ((t = strchr(s, '=')) == NULL)
		return 0;

	pos = t - s;

	appendmakeflags(s);
	t = estrdup(s); 
	t[pos] = '\0';

	setmacro(t, t+pos+1, export);
	free(t);
}

static void
parseargv(char ***argv, int export)
{
	char *s;

	for ( ; **argv; ++*argv) {
		s = **argv;
		if (s[0] != '-') {
			if (!assign(s, export))
				break;
			continue;
		}
		while (*++s)
			parseflag(*s, &s, argv);
	}
}



static void
parsemakeflags(void)
{
	size_t len1, len2, n;
	char *s, *t, **oargv, **argv, *flags;

	if ((flags = getenv("MAKEFLAGS")) == NULL)
		return;

	setmacro("MAKEFLAGS", "", EXPORT);

	while (*flags == ' ' || *flags == '\t')
		flags++;

	if (flags[0] != '-') {
		while (*flags) {
			parseflag(*flags, &flags, NULL);
			flags++;
		}
	} else {
		argv = emalloc(sizeof(char *) * 2);
		argv[0] = flags;
		argv[1] = NULL;

		s = flags;
		for (n = 2; ; ++s) {
			len1 = strcspn(s, " \t");
			if (s[len1] == '\0')
				break;
			len2 = strspn(s+len1, " \t");
			s[len1] = '\0';
			s += len1 + len2;

			argv = erealloc(argv, sizeof(char *) * (n+1));
			argv[n-1]  = s;
			argv[n] = NULL;
		}

		oargv = argv;
		parseargv(&argv, NOEXPORT);
		if (*argv != NULL)
			error("invalid MAKEFLAGS variable");
		free(oargv);
	}
}

int
main(int argc, char *argv[])
{
	signal(SIGINT, sighandler);
	signal(SIGHUP, sighandler);
	signal(SIGTERM, sighandler);
	signal(SIGQUIT, sighandler);

	inject(defaults);
	parsemakeflags();

	++argv;
	parseargv(&argv, EXPORT);

	if (!hasmake)
		parsedefault();

	if (pflag) {
		dumpmacros();
		dumprules();
	}

	if (!*argv) {
		build(NULL);
	} else {
		while (*argv)
			build(*argv++);
	}

	exit(exitstatus);

	return 0;
}
