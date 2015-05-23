
#include <ctype.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/sizes.h"
#include "../inc/cc.h"
#include "cc1.h"

/* TODO: preprocessor error must not rise recover */

static char *argp;
static unsigned arglen;
static Symbol *lastmacro;

static void
nextcpp(void)
{
	next();
	if (yytoken == EOFTOK) {
		error("unterminated argument list invoking macro \"%s\"",
		      lastmacro->name);
	}
	if (yylen + 1 > arglen) {
		error("argument overflow invoking macro \"%s\"",
		      lastmacro->name);
	}
	memcpy(argp, yytext, yylen);
	argp += yylen;
	*argp++ = ' ';
	arglen -= yylen + 1;
}

static void
paren(void)
{
	for (;;) {
		nextcpp();
		switch (yytoken) {
		case ')':
			return;
		case '(':
			paren();
			break;
		}
	}
}

static void
parameter(void)
{
	for (;;) {
		nextcpp();
		switch (yytoken) {
		case ')':
		case ',':
			argp -= 2;
			*argp++ = '\0';
			return;
		case '(':
			paren();
			break;
		}
	}
}

static bool
parsepars(char *buffer, char **listp, int nargs)
{
	int n;

	if (nargs == -1)
		return 1;

	if (ahead() != '(')
		return 0;
	next();

	n = 0;
	argp = buffer;
	arglen = INPUTSIZ;
	if (ahead() != ')') {
		do {
			*listp++ = argp;
			parameter();
		} while (++n < NR_MACROARG && yytoken == ',');
	}

	if (n == NR_MACROARG)
		error("too much parameters in macro \"%s\"", lastmacro->name);
	if (n != nargs) {
		error("macro \"%s\" passed %d arguments, but it takes %d",
		      lastmacro->name, n, nargs);
	}
	return 1;
}

/*
 * sym->u.s is a string with the following format:
 * 	dd#string
 * where dd is the number of arguments of the macro
 * (-1 if it is a macro without arguments), and string
 * is the macro definition, where @dd@ indicates the
 * parameter number dd
 */
bool
expand(Symbol *sym)
{
	unsigned len;
	char *arglist[NR_MACROARG], buffer[INPUTSIZ];
	char c, *bp, *arg, *s = sym->u.s;

	lastmacro = sym;
	if (!parsepars(buffer, arglist, atoi(s)))
		return 0;

	bp = addinput(NULL);
	len = INPUTSIZ-1;
	for (s += 3; c = *s; ++s) {
		if (c != '@') {
			if (len-- == 0)
				goto expansion_too_long;
			*bp++ = c;
		} else {
			unsigned size;

			arg = arglist[atoi(++s)];
			size = strlen(arg);
			if (size > len)
				goto expansion_too_long;
			memcpy(bp, arg, size);
			bp += size;
			len -= size;
			s += 2;
		}
	}
	*bp = '\0';
	return 1;

expansion_too_long:
	error("expansion of macro \"%s\" is too long", lastmacro->name);
}

/*
 * Parse an argument list (par0, par1, ...) and creates
 * an array with pointers to all the arguments in the
 * list
 */
static char *
parseargs(char *s, char *args[NR_MACROARG], int *nargs)
{
	int n;
	size_t len;
	char *endp, c;

	n = -1;
	if (*s != '(')
		goto set_nargs;
	n = 0;
	while (isspace(*s++))
		/* nothing */;
	if (*s == ')')
		goto set_nargs;

	for (n = 1; n <= NR_MACROARG; ++n) {
		while (isspace(*s))
			++s;
		if (!isalnum(*s) && *s != '_')
			error("macro arguments must be identifiers");
		for (endp = s; isalnum(*endp) || *endp == '_'; ++endp)
			/* nothing */;
		if ((len = endp - s) > IDENTSIZ)
			error("macro argument too long");
		*args++ = s;
		for (s = endp; isspace(*s); ++s)
			*s = '\0';
		c = *s;
		*s++ = '\0';
		if (c == ')')
			break;
		if (c == ',')
			continue;
		else
			error("macro parameters must be comma-separated");
	}
	if (n > NR_MACROARG)
		error("too much parameters in macro");

set_nargs:
	*nargs = n;
	return s;
}
/*
 * Copy a define string, and substitute formal arguments of the
 * macro into strings in the form @XX, where XX is the position
 * of the argument in the argument list.
 */
static char *
copydefine(char *s, char *args[], char *buff, int bufsiz, int nargs)
{
	unsigned ncopy, n;
	size_t len;
	char arroba[6], *par, *endp, **bp;

	while (*s && bufsiz > 0) {
		if (!isalnum(*s) && *s != '_') {
			--bufsiz;
			*buff++ = *s++;
			continue;
		}
		/*
		 * found an identifier, is it one of the macro arguments?
		 */
		for (endp = s+1; isalnum(*endp) || *endp == '_'; ++endp)
			/* nothing */;
		len = endp - s;
		for (bp =args, n = 0; n < nargs; ++bp, n++) {
			if (strncmp(s, *bp, len))
				continue;
			sprintf(arroba, "@%02d@", n);
			break;
		}
		if (n == nargs)
			par = s, ncopy = len;
		else
			par = arroba, ncopy = 4;

		if ((bufsiz -= ncopy) < 0)
			goto too_long;
		memcpy(buff, par, ncopy);
		buff += ncopy;
		s = endp;
	}

	if (*s == '\0') {
		*buff = '\0';
		return s;
	}

too_long:
	error("macro definition too long");
}

static char *
mkdefine(char *s, Symbol *sym)
{
	int nargs;
	char *args[NR_MACROARG], buff[LINESIZ+1];

	s = parseargs(s, args, &nargs);
	sprintf(buff, "%02d#", nargs);

	while (isspace(*s))
		++s;

	if (*s != '\0')
		s = copydefine(s, args, buff+3, LINESIZ-3, nargs);
	sym->u.s = xstrdup(buff);

	return s;
}

static char *
define(char *s)
{
	extern char yytext[];
	char *t;
	size_t len;
	Symbol *sym;

	if (!isalnum(*s) && *s != '_')
		goto bad_define;
	for (t = s; isalnum(*t) || *t == '_'; ++t)
		/* nothing */;
	if ((len = t - s) > IDENTSIZ)
		goto too_long;
	strncpy(yytext, s, len);
	yytext[len] = '\0';
	sym = lookup(NS_CPP);
	if ((sym->flags & ISDEFINED) && sym->ns == NS_CPP) {
		warn("'%s' redefined", yytext);
		free(sym->u.s);
	}
	sym->flags |= ISDEFINED;
	sym->ns = NS_CPP;

	for (s = t; isspace(*s); ++s)
		/* nothing */;
	for (t = s + strlen(s); isspace(*--t); *t = '\0')
		/* nothing */;
	return mkdefine(s, sym);

too_long:
	error("macro identifier too long");
bad_define:
	error("macro names must be identifiers");
}

static char *
include(char *s)
{
	char fname[FILENAME_MAX], delim, c, *p;
	size_t len;

	if ((c = *s++) == '>')
		delim = '>';
	else if (c == '"')
		delim = '"';
	else
		goto bad_include;

	for (p = s; (c = *p) && c != delim; ++p)
		/* nothing */;
	if (c == '\0')
		goto bad_include;

	len = p - s;
	if (delim == '"') {
		if (len >= FILENAME_MAX)
			goto too_long;
		strncpy(fname, s, len);
		fname[len] = '\0';
		if (!addinput(fname))
			goto not_found;
	} else {
		abort();
	}

	return p+1;

not_found:
	error("included file '%s' not found", fname);
too_long:
	error("file name in include too long");
bad_include:
	error("#include expects \"FILENAME\" or <FILENAME>");
}

static char *
line(char *s)
{
	char *p, *q;

	if (!isdigit(*s))
		goto bad_line;
	for (p = s; isdigit(*p); ++p)
		/* nothing */;
	switch (*p) {
	case ' ':
	case '\t':
		while (isspace(*p))
			++p;
		if (*p != '"')
			goto bad_line;
		for (q = p+1; *q && *q != '"'; ++q)
			/* nothing */;
		if (*q == '\0')
			goto bad_line;
		*q = '\0';
		setfname(p);
		p = q+1;
		/* passthrough */
	case '\0':
		setfline(atoi(s)-1);
		return p;
	default:
		goto bad_file;
	}

bad_file:
	error("second parameter of #line is not a valid filename");
bad_line:
	error("first parameter of #line is not a positive integer");
}

static char *
pragma(char *s)
{
	while (*s)
		++s;
	return s;
}

static char *
usererr(char *s)
{
	fprintf(stderr, "%s:%u:error: #error %s\n", getfname(), getfline(), s);
	exit(-1);
}

char *
preprocessor(char *p)
{
	char *q;
	unsigned short n;
	static struct {
		char *name;
		char *(*fun)(char *);
	} *bp, cmds[] =  {
		"define", define,
		"include", include,
		"line", line,
		"pragma", pragma,
		"error", usererr,
		NULL, NULL
	};

	while (isspace(*p))
		++p;
	if (*p != '#')
		return p;
	for (++p; isspace(*p); ++p)
		/* nothing */;
	if (*p == '\0')
		return p;
	for (q = p; isalpha(*q); ++q)
		/* nothing */;
	if ((n = q - p) == 0)
		goto incorrect;
	while (isspace(*q))
		++q;
	for (bp = cmds; bp->name; ++bp) {
		if (strncmp(bp->name, p, n))
			continue;
		q = (*bp->fun)(q);
		while (isspace(*q++))
			/* nothing */;
		if (*q != '\0')
			error("trailing characters after preprocessor directive");
		return q;
	}
incorrect:
	error("incorrect preprocessor directive");
}
