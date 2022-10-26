#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/cstd.h>
#include <scc/scc.h>
#include "cc1.h"

int yytoken;
struct yystype yylval;
char yytext[STRINGSIZ+3];
unsigned short yylen;
int lexmode = CCMODE;
unsigned lineno;
char filenam[FILENAME_MAX];

int namespace = NS_IDEN;
static int safe;
Input *input;

void
setloc(char *fname, unsigned line)
{
	size_t len;

	if (fname) {
		if ((len = strlen(fname)) >= FILENAME_MAX)
			die("cc1: %s: file name too long", fname);
		memmove(filenam, fname, len);
		filenam[len] = '\0';

		/*
		 * There are cases where we want to call setloc()
		 * with the data in input, and then we have t be
		 * careful about freeing input->filenam
		 */
		if (fname != input->filenam) {
			free(input->filenam);
			input->filenam = xstrdup(fname);
		}
	}

	lineno = input->lineno = line;
}

int
addinput(int type, void *arg, int fail)
{
	FILE *fp;
	char *extp, *fname, *buffer, *infile;
	int infileln;
	Macro *mp;
	Symbol *sym;
	Input *newip, *curip = input;

	if (curip)
		curip->lineno = lineno;

	switch (type) {
	case IMACRO:
		fp = NULL;
		mp = arg;
		sym = mp->sym;
		fname = mp->fname;
		buffer = mp->buffer;
		DBG("INPUT: expanding macro %s", sym->name);
		break;
	case IPARAM:
		fp = NULL;
		mp = NULL;
		buffer = arg;
		fname = filenam;
		DBG("INPUT: macro parameter '%s'", buffer);
		break;
	case IFILE:
		fname = arg;
		mp = NULL;
		buffer = NULL;

		if ((fp = fopen(fname, "r")) == NULL) {
			if (!fail)
				return 0;
			die("cc1: %s: %s", fname, strerror(errno));
		}
		if (curip && onlyheader) {
			infile = curip->filenam;
			infileln = strlen(infile);
			if (extp = strrchr(infile, '.'))
				infileln -= strlen(extp);
			printf("%.*s.o: %s %s\n",
			       infileln, infile, infile, fname);
		}
		lineno = 0;
		DBG("INPUT: file input '%s'", fname);
		break;
	case ISTDIN:
		fp = stdin;
		mp = NULL;
		fname = "<stdin>";
		buffer = NULL;
		lineno = 0;
		DBG("INPUT: file input 'stdin'");
		break;
	default:
		abort();
	}

	if (!buffer) {
		buffer = xmalloc(INPUTSIZ);
		buffer[0] = '\0';
	} else {
		buffer = xstrdup(buffer);
	}

	newip = xmalloc(sizeof(*newip));
	newip->next = curip;
	newip->macro = mp;
	newip->p = newip->begin = newip->line = buffer;
	newip->filenam = NULL;
	newip->lineno = 0;
	newip->fp = fp;
	newip->flags = type;
	input = newip;

	setloc(fname, lineno);
	return 1;
}

void
delinput(void)
{
	Input *ip = input;

	switch (ip->flags & ITYPE) {
	case IFILE:
		DBG("INPUT: file finished '%s'", ip->filenam);
		if (fclose(ip->fp))
			die("cc1: %s: %s", ip->filenam, strerror(errno));
		break;
	case IMACRO:
		DBG("INPUT: macro %s finished", ip->macro->sym->name);
		delmacro(ip->macro);
		break;
	case IPARAM:
		DBG("INPUT: macro param finished");
		break;
	case ISTDIN:
		DBG("INPUT: stdin finished");
		break;
	default:
		abort();
	}

	input = ip->next;
	free(ip->filenam);
	free(ip->line);
	free(ip);
	if (input)
		setloc(input->filenam, input->lineno);
}

static void
newline(void)
{
	if (++lineno == 0)
		die("cc1: %s: file too long", filenam);
}

/*
 * Read the next character from the input file, counting number of lines
 * and joining lines escaped with \
 */
static int
readchar(void)
{
	FILE *fp = input->fp;
	int c;

repeat:
	switch (c = getc(fp)) {
	case '\\':
		if ((c = getc(fp)) == '\n') {
			newline();
			goto repeat;
		}
		ungetc(c, fp);
		c = '\\';
		break;
	case '\n':
		newline();
		break;
	case EOF:
		break;
	default:
		if (!isprint(c) && !ispunct(c) && !isspace(c))
			warn("invalid input character. The shame of UB is yours");
		break;
	}

	return c;
}

/*
 * discard a C comment. This function is only called from readline
 * because it is impossible to have a comment in a macro, because
 * comments are always discarded before processing any cpp directive
 */
static void
comment(int type)
{
	int c;

repeat:
	while ((c = readchar()) != EOF && c != type)
		;

	if (c == EOF) {
		errorp("unterminated comment");
		return;
	}

	if (type == '*' && (c = readchar()) != '/')
		goto repeat;
}

/*
 * readline is used to read a full logic line from a file.
 * It discards comments and check that the line fits in
 * the input buffer
 */
static int
readline(void)
{
	char *bp, *lim;
	int c, peekc = 0, delim = 0;

	if (feof(input->fp)) {
		input->flags |= IEOF;
		*input->p = '\0';
		return 0;
	}

	*input->line = '\0';
	lim = &input->line[INPUTSIZ-1];
	for (bp = input->line; bp < lim-1; *bp++ = c) {
		c = (peekc) ? peekc : readchar();
		peekc = 0;
		if (c == '\n' || c == EOF)
			break;
		if (c == '\\') {
			peekc = readchar();
			if (peekc == '\n' || peekc == EOF)
				continue;
			if (bp == lim-2)
				break;
			*bp++ = c;
			c = peekc;
			peekc = 0;
			continue;
		}

		if (delim && c == delim)
			delim = 0;
		else if (!delim && (c == '"' || c == '\''))
			delim = c;
		if (c != '/' || delim)
			continue;

		/* check for /* or // */
		peekc = readchar();
		if (peekc != '*' && peekc != '/')
			continue;

		if (peekc == '/') {
			comment('\n');
			break;
		} else {
			comment('*');
			c = ' ';
		}
		peekc = 0;
	}

	input->begin = input->p = input->line;
	if (bp == lim-1) {
		errorp("line too long");
		--bp;
	}
	*bp++ = '\n';
	*bp = '\0';

	return 1;
}

/*
 * moreinput gets more bytes to be passed to the lexer.
 * It can take more bytes from macro expansions or
 * directly reading from files. When a cpp directive
 * is processed the line is discarded because it must not
 * be passed to the lexer
 */
static int
moreinput(void)
{
	int wasexpand = 0;

repeat:
	if (!input)
		return 0;

	if (*input->p == '\0') {
		int t = input->flags & ITYPE;
		if (t == IPARAM) {
			input->flags |= IEOF;
			return 0;
		}
		if (t == IMACRO) {
			wasexpand = 1;
			input->flags |= IEOF;
		}
		if (input->flags & IEOF) {
			delinput();
			goto repeat;
		}
		if (!readline()) {
			*input->p = '\0';
			goto repeat;
		}
		if (cpp())
			goto repeat;
	}

	if (onlycpp && !wasexpand)
		ppragmaln();
	return 1;
}

static void
tok2str(void)
{
	if ((yylen = input->p - input->begin) > INTIDENTSIZ)
		error("token too big");
	memcpy(yytext, input->begin, yylen);
	yytext[yylen] = '\0';
	input->begin = input->p;
}

static Symbol *
readint(char *s, int base, int sign, Symbol *sym)
{
	Type *tp = sym->type;
	struct limits *lim;
	TUINT u, val, max;
	int c;

	lim = getlimits(tp);
	max = lim->max.i;
	if (*s == '0')
		++s;
	if (toupper(*s) == 'X')
		++s;

	for (u = 0; isxdigit(c = *s++); u = u*base + val) {
		static char letters[] = "0123456789ABCDEF";
		val = strchr(letters, toupper(c)) - letters;
	repeat:
		if (u <= max/base && u*base <= max - val)
			continue;
		if (tp->prop & TSIGNED) {
			if (tp == inttype)
				tp = (base==10) ? longtype : uinttype;
			else if (tp == longtype)
				tp = (base==10) ? llongtype : ulongtype;
			else
				goto overflow;
		} else {
			if (tp == uinttype)
				tp = (sign==UNSIGNED) ? ulongtype : longtype;
			else if (tp == ulongtype)
				tp = (sign==UNSIGNED) ? ullongtype : llongtype;
			else
				goto overflow;
		}
		sym->type = tp;
		lim = getlimits(tp);
		max = lim->max.i;
		goto repeat;
	}

	if (tp->prop & TSIGNED)
		sym->u.i = u;
	else
		sym->u.u = u;

	return sym;

overflow:
	errorp("overflow in integer constant");
	return sym;
}

static int
integer(char *s, int base)
{
	Type *tp;
	Symbol *sym;
	unsigned size, sign;

	for (size = sign = 0; ; ++input->p) {
		switch (toupper(*input->p)) {
		case 'L':
			if (size == LLONG)
				goto wrong_type;
			size = (size == LONG) ? LLONG : LONG;
			continue;
		case 'U':
			if (sign == UNSIGNED)
				goto wrong_type;
			sign = UNSIGNED;
			continue;
		default:
			goto convert;
		wrong_type:
			error("invalid suffix in integer constant");
		}
	}

convert:
	tp = ctype(INT, sign, size);
	sym = newsym(NS_IDEN, NULL);
	sym->type = tp;
	sym->flags |= SCONSTANT;
	yylval.sym = readint(s, base, sign, sym);
	return CONSTANT;
}

static char *
digits(int base)
{
	char *p;
	int c;

	for (p = input->p; c = *p; ++p) {
		switch (base) {
		case 8:
			if (!strchr("01234567", c))
				goto end;
			break;
		case 10:
			if (!isdigit(c))
				goto end;
			break;
		case 16:
			if (!isxdigit(c))
				goto end;
			break;
		}
	}
end:
	input->p = p;
	tok2str();
	return yytext;
}

static int
number(void)
{
	int base;

	if (*input->p != '0') {
		base = 10;
	} else {
		if (toupper(*++input->p) == 'X') {
			++input->p;
			base = 16;
		} else {
			base = 8;
		}
	}

	return integer(digits(base), base);
}

static int
escape(void)
{
	int c, base;

	switch (*++input->p) {
	case 'a':
		return '\a';
	case 'b':
		return '\b';
	case 'f':
		return '\f';
	case 'n':
		return '\n';
	case 'r':
		return '\r';
	case 't':
		return '\t';
	case 'v':
		return '\v';
	case '"':
		return '"';
	case '\'':
		return '\'';
	case '\\':
		return '\\';
	case '\?':
		return '\?';
	case 'u':
		/*
		 * FIXME: universal constants are not correctly handled
		 */
		if (!isdigit(*++input->p))
			warn("incorrect digit for numerical character constant");
		base = 10;
		break;
	case 'x':
		if (!isxdigit(*++input->p))
			warn("\\x used with no following hex digits");
		base = 16;
		break;
	case '0':
		if (!strchr("01234567", *++input->p))
			warn("\\0 used with no following octal digits");
		base = 8;
		break;
	default:
		warn("unknown escape sequence");
		return ' ';
	}
	errno = 0;
	/* FIXME: We don't check that there is an actual number */
	c = strtoul(input->p, &input->p, base);
	if (errno || c > 255)
		warn("character constant out of range");
	--input->p;
	return c;
}

static int
character(void)
{
	int c;
	Symbol *sym;

	if ((c = *++input->p) == '\\')
		c = escape();
	else
		c = *input->p;
	++input->p;
	if (*input->p != '\'')
		errorp("invalid character constant");
	else
		++input->p;

	sym = newsym(NS_IDEN, NULL);
	sym->u.i = c;
	sym->type = inttype;
	yylval.sym = sym;
	tok2str();
	return CONSTANT;
}

/*
 * string() parses a constant string, and convert all the
 * escape sequences into single characters. This behaviour
 * is correct except when we parse a #define, where we want
 * to preserve the literal content of the string. In that
 * case cpp.c:/^define( sets the variable disescape to
 * disable converting escape sequences into characters.
 */
static int
string(void)
{
	char *bp = yytext;
	int c, esc;

	*bp++ = '"';
	esc = 0;
	for (++input->p; ; ++input->p) {
		c = *input->p;

		if (c == '"' && !esc)
			break;

		if (c == '\0') {
			errorp("missing terminating '\"' character");
			break;
		}

		if (c == '\\' && !esc && disescape)
			esc = 1;
		else
			esc = 0;

		if (c == '\\' && !esc)
			c = escape();

		if (bp == &yytext[STRINGSIZ+1]) {
			/* too long, ignore everything until next quote */
			for (++input->p; *input->p != '"'; ++input->p) {
				if (*input->p == '\\')
					++input->p;
				if (*input->p == '\0')
					break;
			}
			--bp;
			errorp("string too long");
			break;
		}
		*bp++ = c;
	}

	input->begin = ++input->p;
	*bp = '\0';

	yylen = bp - yytext + 1;
	yylval.sym = newstring(yytext+1, yylen-1);
	*bp++ = '"';
	*bp = '\0';
	return STRING;
}

static int
iden(void)
{
	Symbol *sym;
	char *p, *begin;

	begin = input->p;
	for (p = begin; isalnum(*p) || *p == '_'; ++p)
		;
	input->p = p;
	tok2str();
	if ((sym = lookup(NS_CPP, yytext, NOALLOC)) != NULL) {
		if (expand(sym))
			return next();
	}
	sym = lookup(namespace, yytext, ALLOC);
	yylval.sym = sym;
	if (sym->flags & SCONSTANT)
		return CONSTANT;
	if (sym->token != IDEN)
		yylval.token = sym->u.token;
	return sym->token;
}

static int
follow(int expect, int ifyes, int ifno)
{
	if (*input->p++ == expect)
		return ifyes;
	--input->p;
	return ifno;
}

static int
minus(void)
{
	switch (*input->p++) {
	case '-':
		return DEC;
	case '>':
		return INDIR;
	case '=':
		return SUB_EQ;
	default:
		--input->p;
		return '-';
	}
}

static int
plus(void)
{
	switch (*input->p++) {
	case '+':
		return INC;
	case '=':
		return ADD_EQ;
	default:
		--input->p;
		return '+';
	}
}

static int
relational(int op, int equal, int shift, int assig)
{
	int c;

	if ((c = *input->p++) == '=')
		return equal;
	if (c == op)
		return follow('=', assig, shift);
	--input->p;
	return op;
}

static int
logic(int op, int equal, int logic)
{
	int c;

	if ((c = *input->p++) == '=')
		return equal;
	if (c == op)
		return logic;
	--input->p;
	return op;
}

static int
dot(void)
{
	int c;

	if ((c = *input->p) != '.')
		return '.';
	if ((c = *++input->p) != '.')
		error("incorrect token '..'");
	++input->p;
	return ELLIPSIS;
}

static int
operator(void)
{
	int t;

	switch (t = *input->p++) {
	case '<':
		t = relational('<', LE, SHL, SHL_EQ);
		break;
	case '>':
		t = relational('>', GE, SHR, SHR_EQ);
		break;
	case '&':
		t = logic('&', AND_EQ, AND);
		break;
	case '|':
		t = logic('|', OR_EQ, OR);
		break;
	case '=':
		t = follow('=', EQ, '=');
		break;
	case '^':
		t = follow('=', XOR_EQ, '^');
		break;
	case '*':
		t = follow('=', MUL_EQ, '*');
		break;
	case '/':
		t = follow('=', DIV_EQ, '/');
		break;
	case '%':
		t = follow('=', MOD_EQ, '%');
		break;
	case '!':
		t = follow('=', NE, '!');
		break;
	case '#':
		t = follow('#', CONCAT, STRINGIZE);
		break;
	case '-':
		t = minus();
		break;
	case '+':
		t = plus();
		break;
	case '.':
		t = dot();
		break;
	}
	tok2str();
	return t;
}

/* TODO: Ensure that namespace is NS_IDEN after a recovery */

/*
 * skip all the spaces until the next token. When we are in
 * CPPMODE \n is not considered a whitespace
 */
static int
skipspaces(void)
{
	int c;

	if (!input)
		return EOF;

	for (;;) {
		switch (c = *input->p) {
		case '\n':
			if (lexmode == CPPMODE)
				goto return_byte;
			++input->p;
		case '\0':
			if (!moreinput())
				return EOF;
			break;
		case ' ':
		case '\t':
		case '\v':
		case '\r':
		case '\f':
			++input->p;
			break;
		default:
			goto return_byte;
		}
	}

return_byte:
	input->begin = input->p;
	return c;
}

int
next(void)
{
	int c;

	if ((c = skipspaces()) == EOF)
		yytoken = EOFTOK;
	else if (isalpha(c) || c == '_')
		yytoken = iden();
	else if (isdigit(c))
		yytoken = number();
	else if (c == '"')
		yytoken = string();
	else if (c == '\'')
		yytoken = character();
	else
		yytoken = operator();

	if (yytoken == EOFTOK) {
		strcpy(yytext, "<EOF>");
		if (cppctx)
			errorp("#endif expected");
	}

	DBG("TOKEN %s", yytext);
	return yytoken;
}

void
expect(int tok)
{
	if (yytoken != tok) {
		if (isgraph(tok))
			errorp("expected '%c' before '%s'", tok, yytext);
		else
			errorp("unexpected '%s'", yytext);
	} else {
		next();
	}
}

int
ahead(void)
{
	skipspaces();
	return *input->begin;
}

void
setsafe(int type)
{
	safe = type;
}

void
discard(void)
{
	extern jmp_buf recover;
	int c;

	if (!input)
		return;

	for (c = yytoken; ; c = *input->p++) {
		switch (safe) {
		case END_COMP:
			if (c == '}')
				goto jump;
			goto semicolon;
		case END_COND:
			if (c == ')')
				goto jump;
			break;
		case END_LDECL:
			if (c == ',')
				goto jump;
		case END_DECL:
		semicolon:
			if (c == ';')
				goto jump;
			break;
		}
		if (c == '\0' && !moreinput())
			exit(1);
	}
jump:
	input->begin = input->p;
	yytoken = c;
	yytext[0] = c;
	yytext[1] = '\0';
	exit(EXIT_FAILURE);

	/*
	 * FIXME: We don't have a proper recover mechanism at this moment
	 * and we don't set the recover point ever, so executing this
	 * longjmp will generate surely a segmentation fault, so it does
	 * not make sense to do it. We just exit until we can find time
	 * to solve this problem.
	 */
	longjmp(recover, 1);
}
