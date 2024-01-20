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
volatile sig_atomic_t  stop;

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
	case 'j':
	case 'f':
		getarg(args, argv);
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
	return 1;
}

static void
parseargv(char **argv, char ***targets, int export)
{
	char *s;

	for ( ; *argv; ++argv) {
		s = *argv;
		if (s[0] != '-') {
			if (!assign(s, export))
				break;
			continue;
		}
		while (*++s)
			parseflag(*s, &s, &argv);
	}

	if (targets)
		*targets = argv;
}

static void
parsemakeflags(void)
{
	int c, n;
	char *s, *flags, **arr;

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
		n = 0;
		arr = NULL;
		for (s = strtok(flags, " \t"); s; s = strtok(NULL, " \t")) {
			n++;
			arr = erealloc(arr, sizeof(char *) * (n+1));
			arr[n-1] = s;
			arr[n] = NULL;
		}

		parseargv(arr, NULL, NOEXPORT);
		free(arr);
	}
}

static void
parsemakefiles(char **argv)
{
	char *s, *arg;
	int c, hasmake;

	hasmake = 0;
	for ( ; *argv && **argv == '-'; ++argv) {
		for (s = *argv; c = *s; ++s) {
			if (c == 'f' || c == 'j')
				arg = getarg(&s, &argv);

			if (c == 'f') {
				if (strcmp(arg, "-") == 0)
					arg = NULL;
				parse(arg);
				hasmake = 1;
			}
		}
	}

	if (hasmake)
		return;

	if (parse("makefile"))
		return;
	if (parse("Makefile"))
		return;
}

int
main(int argc, char *argv[])
{
	char *arg0;

	signal(SIGINT, sighandler);
	signal(SIGHUP, sighandler);
	signal(SIGTERM, sighandler);
	signal(SIGQUIT, sighandler);

	arg0 = *argv++;

	inject(defaults);
	parsemakefiles(argv);
	parsemakeflags();
	parseargv(argv, &argv, EXPORT);

	setmacro("MAKE", arg0, NOEXPORT);

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
