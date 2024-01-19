#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "make.h"

#define MAXREPL  30
#define TABSIZ   64
#define MAXTOKEN 256
#define ITEM     128

typedef struct macro Macro;

enum inputype {
	FTFILE,
	FTEXPAN,
};

struct loc {
	char *fname;
	int lineno;
};

struct input {
	int siz;
	int type;

	FILE *fp;
	struct loc loc;

	int pos;
	char *buf;

	struct input *prev;
};

struct macro {
	char *name;
	char *value;

	struct macro *next;
};

static struct input *input;
static char token[MAXTOKEN];
static int tok;
static Macro *htab[TABSIZ];

void
dumpmacros(void)
{
	Macro **pp, *p;

	for (pp = htab; pp < &htab[TABSIZ]; ++pp) {
		for (p = *pp; p; p = p->next)
			printf("%s = %s\n", p->name, p->value);
	}
}

static Macro *
lookup(char *name)
{
	Macro *mp;
	int h = hash(name) & TABSIZ-1;

	for (mp = htab[h]; mp && strcmp(mp->name, name); mp = mp->next)
		;

	if (mp)
		return mp;

	mp = emalloc(sizeof(*mp));
	mp->name = estrdup(name);
	mp->value = NULL;
	mp->next = htab[h];
	htab[h] = mp;

	return mp;
}

void
setmacro(char *name, char *val, int export)
{
	int n;
	char *buf;
	Macro *mp;

	mp = lookup(name);
	free(mp->value);
	mp->value = estrdup(val);

	if (export && strcmp(name, "SHELL") != 0) {
		n = snprintf(NULL, 0, "%s=%s", name, val);
		buf = emalloc(n+1);
		snprintf(buf, n+1, "%s=%s", name, val);
		putenv(buf);
	}
}

char *
getmacro(char *name)
{
	int hide;
	char *s, *t;
	Macro *mp = lookup(name);

	hide = 0;
	if (!strcmp(name, "SHELL") || !strcmp(name, "MAKEFLAGS"))
		hide = 1;

	s = mp->value;
	if (!s && !hide)
		s = getenv(name);
	if (!s)
		s = "";

	if (eflag && !hide) {
		t = getenv(name);
		if (t)
			s = t;
	}

	return s;
}

static struct loc *
getloc(void)
{
	struct input *ip;

	for (ip = input; ip && ip->type != FTFILE; ip = ip->prev)
		;
	if (!ip)
		return NULL;

	return &ip->loc;
}


void
error(char *fmt, ...)
{
	va_list va;
	struct loc *loc;

	fprintf(stderr, "make: error: ");
	if ((loc = getloc()) != NULL)
		fprintf(stderr, "%s:%d: ", loc->fname, loc->lineno);

	va_start(va, fmt);
	vfprintf(stderr, fmt, va);
	va_end(va);
	putc('\n', stderr);

	exit(EXIT_FAILURE);
}

void
warning(char *fmt, ...)
{
	va_list va;
	struct loc *loc;

	fprintf(stderr, "make: warning: ");
	if ((loc = getloc()) != NULL)
		fprintf(stderr, "%s:%d: ", loc->fname, loc->lineno);

	va_start(va, fmt);
	vfprintf(stderr, fmt, va);
	va_end(va);
	putc('\n', stderr);
}

static void
pop(void)
{
	struct input *ip = input->prev;

	if (input->type == FTFILE) {
		fclose(input->fp);
		free(input->loc.fname);
	}
	free(input->buf);
	free(input);

	input = ip;
}

static void
push(int type, ...)
{
	int len, pos;
	FILE *fp = NULL;
	char *buf, *s, *fname = NULL;
	va_list va;
	struct input *ip;

	va_start(va, type);
	switch (type) {
	case FTFILE:
		s = va_arg(va, char *);
		fp = va_arg(va, FILE *);
		fname = estrdup(s);
		buf = emalloc(BUFSIZ);
		pos = len = BUFSIZ;
		break;
	case FTEXPAN:
		s = va_arg(va, char *);
		buf = estrdup(s);
		pos = 0;
		len = strlen(s);
		break;
	}
	va_end(va);

	ip = emalloc(sizeof(*ip));
	ip->siz = len;
	ip->buf = buf;
	ip->type = type;
	ip->fp = fp;
	ip->loc.fname = fname;
	ip->loc.lineno = 1;
	ip->pos = pos;
	ip->prev = input;

	input = ip;
}

static char *
trim(char *s)
{
	size_t len;

	while (isspace(*s))
		s++;

	for (len = strlen(s); len > 0 && isspace(s[len-1]); --len)
		s[len-1] = '\0';

	return s;
}

static void
include(char *s)
{
	int len;
	FILE *fp;
	char *fil, *t;

	s = trim(s);
	fil = expandstring(s, NULL);

	t = trim(fil);
	if (strlen(t) != 0) {
		debug("including '%s'", t);
		if ((fp = fopen(t, "r")) == NULL)
			error("opening %s:%s", t, strerror(errno));
		push(FTFILE, t, fp);
	}

end:
	free(fil);
}

static void
comment(FILE *fp)
{
	int c;

	while ((c = getc(fp)) != EOF && c != '\n') {
		if (c == '\\' && getc(fp) == EOF)
			break;
	}
}

static char *
nextline(void)
{
	int c;
	FILE *fp;
	char *s, *lim;

	assert(input->type == FTFILE);

repeat:
	fp = input->fp;
	if (feof(fp))
		return NULL;

	lim = &input->buf[input->siz];
	for (s = input->buf; s < lim; *s++ = c) {
		c = getc(fp);
		if (c == '\n' || c == EOF) {
			input->loc.lineno++;
			*s++ = '\n';
			break;
		}
		if (c == '#') {
			comment(fp);
			*s++ = '\n';
			break;
		}
		if (c > UCHAR_MAX || c < 0)
			error("invalid character '%c' (%d)", c, c);
	}


	if (s == lim)
		error("too long line");
	if (ferror(fp))
		error(strerror(errno));
	*s = '\0';

	if (!strcmp(input->buf, ""))
		goto repeat;

	if (strncmp(input->buf, "include", 7) == 0) {
		input->pos = input->siz;
		include(input->buf+7);
		goto repeat;
	}

	input->pos = 0;


	return input->buf;
}

static int
empty(struct input *ip)
{
	return ip->pos == ip->siz || ip->buf[ip->pos] == '\0';
}

static int
moreinput(void)
{
	while (input) {
		if (!empty(input))
			break;

		switch (input->type) {
		case FTEXPAN:
			pop();
			break;
		case FTFILE:
			if (!nextline())
				pop();
			break;
		}
	}

	return input != NULL;
}

static int
nextc(void)
{
	if (!moreinput())
		return EOF;

	return input->buf[input->pos++];
}

static int
back(int c)
{
	if (c == EOF)
		return c;
	assert(input->pos > 0);
	input->buf[--input->pos] = c;
}

static void
skipspaces(void)
{
	int c;

	for (c = nextc(); c == ' ' || c == '\t'; c = nextc())
		;
	back(c);
}

static int
validchar(int c)
{
	if (c == EOF)
		return 0;
	return c == '.' || c == '/' || c == '_' || c == '-' || isalnum(c);
}

static int
item(void)
{
	int c;
	char *s;

	for (s = token; s < &token[MAXTOKEN] - 1; *s++ = c) {
		c = nextc();
		if (!validchar(c))
			break;
	}

	if (s >= &token[MAXTOKEN] - 1)
		error("token too long");
	if (s == token)
		error("invalid empty token");
	*s = '\0';
	back(c);

	return ITEM;
}

static void
expandmacro(char *name, char *repl, char *to)
{
	int pos, siz, replsiz, tosiz;
	char *s, *t, *p, *buf;

	s = expandstring(getmacro(name), NULL);

	pos = 0;
	buf = NULL;
	tosiz = strlen(to);
	replsiz = strlen(repl);

	t = s;
	for (pos = 0; *t; pos += siz) {
		if (replsiz > 0 && strncmp(t, repl, replsiz) == 0) {
			siz = tosiz;
			p = to;
			t += replsiz;
		} else {
			siz = 1;
			p = t;
			t++;
		}

		buf = erealloc(buf, pos + siz + 1);
		memcpy(buf+pos, p, siz);
	}

	if (pos > 0) {
		buf[pos] = '\0';
		push(FTEXPAN, buf);
		free(buf);
	}
	free(s);
}

static void
expandsimple(Target *tp)
{
	char *s;
	Target **p;
	int len, c;

	switch (c = nextc()) {
	case '@':
		if (!tp || !tp->target)
			return;
		push(FTEXPAN, tp->target);
		break;
	case '<':
		if (!tp || !tp->req)
			return;
		push(FTEXPAN, tp->req);
		break;
	case '*':
		if (!tp || !tp->target)
			return;
		s = strrchr(tp->target, '.');
		if (!s) {
			push(FTEXPAN, tp->target);
			return;
		}

		len = s - tp->target;
		s = emalloc(len+1);
		memcpy(s, tp->target, len);
		s[len] = '\0';
		push(FTEXPAN, s);
		free(s);
		break;
	case '?':
		if (!tp)
			return;

		if (tp->req && stamp(tp->req) > tp->stamp) {
			push(FTEXPAN, " ");
			push(FTEXPAN, tp->req);
		}

		for (p = tp->deps; p && *p; ++p) {
			if (stamp((*p)->name) > tp->stamp) {
				push(FTEXPAN, " ");
				push(FTEXPAN, (*p)->name);
			}
		}
		break;
	default:
		token[0] = c;
		token[1] = '\0';
		expandmacro(token, "", "");
		break;
	}
}

static void
expansion(Target *tp)
{
	int delim, c, repli, toi, namei, st;
	char name[MAXTOKEN], repl[MAXREPL], to[MAXREPL];

	c = nextc();
	if (c == '(')
		delim = ')';
	else if (c == '{')
		delim = '}';
	else
		delim = 0;

	if (!delim) {
		back(c);
		expandsimple(tp);
	} else {
		st = namei = repli = toi = 0;
		while ((c = nextc()) != EOF) {
			if (c == delim)
				break;

			switch (st) {
			case 0:
				if (c == ':') {
					st = 1;
					continue;
				}
				if (!validchar(c))
					error("invalid macro name in expansion");
				if (namei == MAXTOKEN-1)
					error("expansion text too long");
				name[namei++] = c;
				break;
			case 1:
				if (c == '=') {
					st = 2;
					continue;
				}
				if (repli == MAXREPL-1)
					error("macro replacement too big");
				repl[repli++] = c;
				break;
			case 2:
				if (toi == MAXREPL-1)
					error("macro substiturion too big");
				to[toi++] = c;
				break;
			}
		}

		if (c == EOF)
			error("found eof while parsing expansion");
		if (st > 0 && (namei == 0 ||  repli == 0 || to == 0))
			error("invalid macro expansion");

		name[namei] = '\0';
		repl[repli] = '\0';
		to[toi] = '\0';
		expandmacro(name, repl, to);
	}
}

/*
 * Horrible hack to do string expansion.
 * We cannot use normal push and nextc because that
 * would consume characters of the current file too.
 * For that reason it cleans the input and it recovers
 * it later.
 */
char *
expandstring(char *line, Target *tp)
{
	int c, n;
	char *s;
	struct input *ip = input;

	input = NULL;
	push(FTEXPAN, line);

	n = 0;
	s = NULL;
	while ((c = nextc()) != EOF) {
		if (c == '$') {
			if ((c = nextc()) != '$') {
				back(c);
				expansion(tp);
				continue;
			}
		}

		s = erealloc(s, ++n);
		s[n-1] = c;
	}

	s = erealloc(s, n+1);
	s[n] = '\0';
	input = ip;

	return s;
}

static int
readchar(void)
{
	int c;

	while ((c = nextc()) != EOF) {
		if (c == ' ' || c == '\t')
			continue;
		if (c == '\\') {
			if ((c = nextc()) == '\n')
				continue;
			back(c);
			c = '\\';
		}
		break;
	}

	return c;
}

static int
next(void)
{
	int c;

repeat:
	c = readchar();

	switch (c) {
	case EOF:
		strcpy(token, "<EOF>");
		tok = EOF;
		break;
	case '$':
		if ((c = nextc()) == '$')
			goto single;
		back(c);
		expansion(NULL);
		goto repeat;
	case ';':
	case ':':
	case '=':
	case '\n':
	single:
		token[0] = c;
		token[1] = '\0';
		tok = c;
		break;
	default:
		if (!validchar(c))
			error("unexpected character '%c'", c);
		back(c);
		tok = item();
		break;
	}

	return tok;
}

static char *
readmacrodef(void)
{
	int n, c;
	char *line;

	n = 0;
	line = NULL;
	while ((c = nextc()) != EOF) {
		line = erealloc(line, n+1);
		if (c == '\n')
			break;
		if (c == '\\') {
			if ((c = nextc()) != '\n') {
				back(c);
				c = '\\';
			} else {
				skipspaces();
				c = ' ';
			}
		}

		line[n++] = c;
	}
	if (c == EOF)
		error("EOF while looking for end of line");
	line[n] = '\0';

	return line;
}

static char *
readcmd(void)
{
	int n, c;
	char *line;

	skipspaces();

	n = 0;
	line = NULL;
	while ((c = nextc()) != EOF) {
		line = erealloc(line, n+1);
		if (c == '\n')
			break;
		if (c == '\\') {
			if ((c = nextc()) == '\n') {
				if ((c = nextc()) != '\t')
					back(c);
				continue;
			}
			back(c);
			c = '\\';
		}
		line[n++] = c;
	}
	if (c == EOF)
		error("EOF while looking for end of command");
	line[n] = '\0';

	return line;
}

static void
rule(char *targets[], int ntargets)
{
	int c, i, j, ndeps, nactions;
	char **actions, **deps = NULL;

	if (ntargets == 0)
		error("missing target");

	for (ndeps = 0; next() == ITEM; ++ndeps) {
		deps = erealloc(deps, (ndeps+1) * sizeof(char *));
		deps[ndeps] = estrdup(token);
	}

	if (tok != '\n' && tok != ';')
		error("garbage at the end of the line");

	nactions = 0;
	actions = NULL;
	if (tok == ';') {
		nactions++;
		actions = erealloc(actions, nactions * sizeof(char *));
		actions[nactions-1] = readcmd();
	}

	while ((c = nextc()) == '\t') {
		nactions++;
		actions = erealloc(actions, nactions * sizeof(char *));
		actions[nactions-1] = readcmd();
	}
	back(c);

	for (i = 0; i < ntargets; i++) {
		addtarget(targets[i], ndeps);
		for (j = 0; j < ndeps; j++)
			adddep(targets[i], deps[j]);
		if (nactions > 0)
			addrule(targets[i], actions, nactions);
	}

	for (i = 0; i < ndeps; i++)
		free(deps[i]);
	free(deps);

	for (i = 0; i < nactions; i++)
		free(actions[i]);
	free(actions);
}

static void
assign(char *macros[], int n)
{
	int len, c;
	char *defs;

	if (n != 1)
		error("invalid macro definition");

	skipspaces();
	defs = readmacrodef();
	setmacro(*macros, defs, NOEXPORT);
	free(defs);
}

void
parseinput(void)
{
	int i, n;
	char **targets;

	while (moreinput()) {
		n = 0;
		targets = NULL;

		next();
		if (tok == '\n')
			continue;

		while (tok == ITEM) {
			n++;
			targets = erealloc(targets, n * sizeof(char *));
			targets[n-1] = estrdup(token);
			next();
		}

		switch (tok) {
		case ':':
			rule(targets, n);
			break;
		case '=':
			assign(targets, n);
			break;
		default:
			error("unexpected token '%s'(%d)", token, tok);
		}

		for (i = 0; i < n; i++)
			free(targets[i]);
		free(targets);
	}
}

int
parse(char *fname)
{
	FILE *fp;

	if (!fname) {
		fp = stdin;
		fname = "<stdin>";
	} else if ((fp = fopen(fname, "r")) == NULL) {
		return 0;
	}

	debug("parsing %s", fname);
	push(FTFILE, fname, fp);
	parseinput();

	return 1;
}

void
inject(char *s)
{
	push(FTEXPAN, s);
	parseinput();
}
