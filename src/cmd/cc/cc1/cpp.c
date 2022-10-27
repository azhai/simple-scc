#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <scc/cstd.h>
#include <scc/scc.h>
#include "cc1.h"

static unsigned ncmdlines;
static Symbol *symline, *symfile;
static unsigned char ifstatus[NR_COND];
static int cppoff;
static struct items dirinclude;

unsigned cppctx;
int disexpand;
int disescape;

void
defdefine(char *name, char *val, char *source)
{
	char buffer[INPUTSIZ+1];
	char *def, *fmt = "#define %s %s\n";
	Symbol *sym = &(Symbol) {
		.name = name,
		.flags = SDECLARED,
	};

	if (!val)
		val = "";
	if (strlen(fmt) + strlen(name) + strlen(val) > INPUTSIZ) {
		errorp("macro definition '%s' too big", name);
		return;
	}

	sprintf(buffer, fmt, name, val);
	lineno = ++ncmdlines;

	addinput(IPARAM, buffer, FAIL);
	cpp();
	delinput();
}

void
undefmacro(char *s)
{
	killsym(lookup(NS_CPP, s, NOALLOC));
}

void
icpp(void)
{
	struct tm *tm;
	time_t t;
	static char sdate[14], stime[11];
	static struct {
		char *name;
		char *value;
	} *bp, list[] = {
		{"__STDC__", "1"},
		{"__STDC_HOSTED__", "1"},
		{"__SCC__", "1"},
		{"__DATE__", sdate},
		{"__TIME__", stime},
		{"__STDC_VERSION__", STDC_VERSION},
		{"__LINE__", NULL},
		{"__FILE__", NULL},
		{NULL, NULL}
	};

	t = time(NULL);
	tm = localtime(&t);
	strftime(sdate, sizeof(sdate), "\"%b %d %Y\"", tm);
	strftime(stime, sizeof(stime), "\"%H:%M:%S\"", tm);

	for (bp = list; bp->name; ++bp)
		defdefine(bp->name, bp->value, "built-in");

	symline = lookup(NS_CPP, "__LINE__", ALLOC);
	symfile = lookup(NS_CPP, "__FILE__", ALLOC);

	ncmdlines = 0;
}

static void
nextcpp(Macro *mp)
{
	next();
	if (yytoken == EOFTOK)
		error("unterminated argument list invoking macro \"%s\"",
		      mp->sym->name);
	if (yytoken == IDEN)
		yylval.sym->flags |= SUSED;
}

static void
paren(Macro *mp)
{
	for (;;) {
		nextcpp(mp);
		switch (yytoken) {
		case ')':
			return;
		case '(':
			paren(mp);
			break;
		}
	}
}

static char *
parameter(Macro *mp)
{
	int siz;
	char *s, *begin, *end;
	Input *ip = input;

	begin = input->begin;
	for (;;) {
		nextcpp(mp);
		switch (yytoken) {
		case ')':
		case ',':
			/* remove ","  or ")"*/
			end = input->begin - 1;
			while (end > begin && isspace(end[-1]))
				--end;
			while (begin < end && isspace(begin[0]))
				++begin;

			siz = end - begin;
			s = memcpy(xmalloc(siz+1), begin, siz);
			s[siz] = '\0';
			return s;
		case '(':
			paren(mp);
			break;
		}
	}
}

static int
parsepars(Macro *mp)
{
	int n;

	if (mp->npars == -1)
		return 1;
	if (ahead() != '(')
		return 0;

	disexpand = 1;
	next();
	n = 0;

	if (mp->npars == 0 && ahead() == ')') {
		next();
	} else {
		do {
			mp->arglist = xrealloc(mp->arglist, (n+1)*sizeof(char *));
			mp->arglist[n] = parameter(mp);
		} while (++n < NR_MACROARG && yytoken == ',');
	}

	if (yytoken != ')')
		error("incorrect macro function-alike invocation");
	disexpand = 0;

	if (n == NR_MACROARG)
		error("too many parameters in macro \"%s\"", mp->sym->name);
	if (n != mp->npars) {
		error("macro \"%s\" received %d arguments, but it takes %d",
		      mp->sym->name, n, mp->npars);
	}

	return 1;
}

static int
expandarg(char *arg, char *buf, int bufsiz)
{
	int siz;
	char *s = buf;

	addinput(IPARAM, arg, FAIL);
	for (siz = 0; next() != EOFTOK; siz += yylen+1) {
		if (yylen > bufsiz-2) {
			siz = -1;
			break;
		}
		memcpy(buf, yytext, yylen);
		bufsiz -= yylen + 1;
		buf += yylen;
		*buf++ = ' ';
	}
	*buf = '\0';

	delinput();

	DBG("MACRO parameter '%s' expanded to '%s'", arg, s);

	return siz;
}

static int
copymacro(Macro *mp)
{
	int delim, c, esc;
	char *s, *p, *arg, *bp;
	int size, bufsiz;

	if (mp->sym == symfile)
		return sprintf(mp->buffer, "\"%s\" ", filenam);
	if (mp->sym == symline)
		return sprintf(mp->buffer, "%d ", lineno);

	bp = mp->buffer;
	bufsiz = mp->bufsiz;
	for (s = mp->def; c = *s; ++s) {
		switch (c) {
		case '\'':
			delim = '\'';
			goto search_delim;
		case '\"':
			delim = '"';
		search_delim:
			esc = 0;
			p = s;
			for (++s; c = *s; ++s) {
				if (c == '\\' && !esc)
					esc = 1;
				else if (c == delim &&!esc)
					break;
				else
					esc = 0;
			}
			size = s - p + 1;
			if (size > bufsiz)
				goto expansion_too_long;
			memcpy(bp, p, size);
			bufsiz -= size;
			bp += size;
			break;
		case CONCAT:
			/* token concatenation operator */
			while (bp[-1] == ' ')
				--bp, ++bufsiz;
			while (s[1] == ' ')
				++s;
			break;
		case STRINGIZE:
			/* stringfier operator */
			arg = mp->arglist[atoi(s += 2)];
			s += 2;

			if (bufsiz < 3)
				goto expansion_too_long;

			*bp++ = '"';
			while ((c = *arg++) != '\0') {
				if (c == '"') {
					if (bufsiz < 3)
						goto expansion_too_long;
					*bp++ = '\\';
					*bp++ = '"';
					bufsiz -= 2;
				} else {
					if (bufsiz < 2)
						goto expansion_too_long;
					*bp++ = c;
					bufsiz--;
				}
			}
			*bp++ = '"';

			break;
		case MACROPAR:
			/* parameter substitution */
			arg = mp->arglist[atoi(++s)];
			size = expandarg(arg, bp, bufsiz);
			if (size < 0)
				goto expansion_too_long;
			bp += size;
			bufsiz -= size;
			s += 2;
			break;
		default:
			if (bufsiz-- == 0)
				goto expansion_too_long;
			*bp++ = c;
			break;
		}
	}
	*bp = '\0';

	return bp - mp->buffer;

expansion_too_long:
	error("macro expansion of \"%s\" too long", mp->sym->name);
}

static void
addhideset(Input *ip,  Symbol *sym)
{
	Symbol **set;
	Symbol **p;

	set = ip->macro->hideset;
	for (p = set; p < &set[NR_MACROARG] && *p; ++p) {
		if (*p == sym)
			return;
	}

	if (p == &set[NR_MACROARG])
		error("too complex macro expansion");

	*p = sym;
	DBG("MACRO Adding %s to hideset of %s",
	    sym->name, ip->macro->sym->name);
}

static void
hide(Symbol *sym)
{
	DBG("SYM: hidding symbol %s %d", sym->name, sym->hide);
	sym->hide = 1;
}

static void
unhide(Symbol *sym)
{
	DBG("SYM: unhidding symbol %s %d", sym->name, sym->hide);
	sym->hide = 0;
}

void
delmacro(Macro *mp)
{
	int i;
	Symbol **p;

	if (!mp)
		return;

	if (mp->arglist) {
		for (i = 0; i < mp->npars; i++)
			free(mp->arglist[i]);
	}

	for (p = mp->hideset; p < &mp->hideset[NR_MACROARG] && *p; ++p)
		unhide(*p);

	free(mp->arglist);
	free(mp);
}

Macro *
newmacro(Symbol *sym)
{
	Macro *mp;

	mp = xmalloc(sizeof(*mp));
	*mp = (Macro) {0};
	mp->sym = sym;
	mp->def = sym->u.s + 3;
	if (sym->u.s)
		mp->npars = atoi(sym->u.s);

	return mp;
}

int
expand(Symbol *sym)
{
	int siz;
	Macro *mp;
	Input *ip;
	Symbol **p;

	DBG("MACRO '%s' detected disexpand=%d hide=%d",
	    sym->name, disexpand, sym->hide);

	if (disexpand || sym->hide || sym->token != IDEN)
		return 0;

	mp = newmacro(sym);
	mp->fname = filenam;

	if (!parsepars(mp)) {
		delmacro(mp);
		return 0;
	}

	addinput(IMACRO, mp, FAIL);
	mp->buffer = input->line;
	mp->bufsiz = INPUTSIZ-1;

	siz = copymacro(mp);
	mp->buffer[siz] = '\0';

	for (ip = input; ip; ip = ip->next) {
                if ((ip->flags & ITYPE) == IMACRO)
			addhideset(ip, sym);
	}

	for (p = mp->hideset; p < &mp->hideset[NR_MACROARG] && *p; ++p)
		hide(*p);

	DBG("MACRO '%s' expanded to :'%s'", mp->sym->name, mp->buffer);

	return 1;
}

static int
getpars(Symbol *args[NR_MACROARG])
{
	int n, c;
	Symbol *sym;

	c = *input->p;
	next();
	if (c != '(')
		return -1;

	/* skip the '(' */
	next();
	if (accept(')'))
		return 0;

	n = 0;
	do {
		if (n == NR_MACROARG) {
			cpperror("too many parameters in macro");
			return NR_MACROARG;
		}
		if (accept(ELLIPSIS)) {
			args[n++] = NULL;
			break;
		}
		if (yytoken != IDEN) {
			cpperror("macro arguments must be identifiers");
			return NR_MACROARG;
		}
		sym = install(NS_IDEN, yylval.sym);
		sym->flags |= SUSED;
		args[n++] = sym;
		next();
	} while (accept(','));
	expect(')');

	return n;
}

static int
getdefs(Symbol *args[NR_MACROARG], int nargs, char *bp, size_t bufsiz)
{
	Symbol **argp;
	int siz;
	size_t len;
	int prevc = 0, ispar;

	if (yytoken == CONCAT) {
		cpperror("'##' cannot appear at either ends of a macro expansion");
		return 0;
	}

	for (;;) {
		ispar = 0;
		if (yytoken == IDEN && nargs >= 0) {
			for (argp = args; argp < &args[nargs]; ++argp) {
				if (*argp == yylval.sym)
					break;
			}
			if (argp != &args[nargs]) {
				siz = argp - args;
				sprintf(yytext,
				        "%c%02d%c", MACROPAR, siz, MACROPAR);
				ispar = 1;
			}
		}
		if (prevc == STRINGIZE && !ispar) {
			cpperror("'#' is not followed by a macro parameter");
			return 0;
		}
		if (yytoken == '\n')
			break;

		if ((len = strlen(yytext)) >= bufsiz) {
			cpperror("macro too long");
			return 0;
		}
		if (yytoken == CONCAT || yytoken == STRINGIZE) {
			*bp++ = yytoken;
			 --bufsiz;
		} else {
			memcpy(bp, yytext, len);
			bp += len;
			bufsiz -= len;
		}
		if ((prevc = yytoken) != STRINGIZE) {
			*bp++ = ' ';
			--bufsiz;
		}
		next();
	}
	*bp = '\0';
	return 1;
}

static void
define(void)
{
	Symbol *sym,*args[NR_MACROARG];
	char buff[LINESIZ+1];
	int n;

	if (cppoff)
		return;

	disescape = 1;
	namespace = NS_CPP;
	next();

	if (yytoken != IDEN) {
		cpperror("macro names must be identifiers");
		return;
	}
	sym = yylval.sym;
	if (sym->flags & SDECLARED) {
		warn("'%s' redefined", yytext);
		free(sym->u.s);
	} else {
		sym = install(NS_CPP, sym);
		sym->flags |= SDECLARED|SSTRING;
	}

	namespace = NS_IDEN;       /* Avoid polution in NS_CPP */
	if ((n = getpars(args)) == NR_MACROARG)
		goto delete;
	if (n > 0 && !args[n-1])  /* it is a variadic function */
		--n;

	sprintf(buff, "%02d#", n);
	if (!getdefs(args, n, buff+3, LINESIZ-3))
		goto delete;
	sym->u.s = xstrdup(buff);
	DBG("MACRO '%s' defined as '%s'", sym->name, buff);
	return;

delete:
	killsym(sym);
}

void
incdir(char *dir)
{
	if (!dir || *dir == '\0')
		die("cc1: incorrect -I flag");
	newitem(&dirinclude, dir);
}

static int
includefile(char *dir, char *file, size_t filelen)
{
	size_t dirlen;
	char path[FILENAME_MAX];

	if (!dir) {
		dirlen = 0;
		if (filelen > FILENAME_MAX-1)
			return 0;
	} else {
		dirlen = strlen(dir);
		if (dirlen + filelen > FILENAME_MAX-2)
			return 0;
		memcpy(path, dir, dirlen);
		if (dir[dirlen-1] != '/')
			path[dirlen++] = '/';
	}
	memcpy(path+dirlen, file, filelen);
	path[dirlen + filelen] = '\0';

	return addinput(IFILE, path, NOFAIL);
}

static char *
cwd(char *buf)
{
	char *p, *s = filenam;
	size_t len;

	if ((p = strrchr(s, '/')) == NULL)
		return NULL;
	if ((len = p - s) >= FILENAME_MAX)
		die("cc1: current work directory too long");
	memcpy(buf, s, len);
	buf[len] = '\0';
	return buf;
}

static void
include(void)
{
	char dir[FILENAME_MAX], file[FILENAME_MAX], *p, **bp;
	size_t filelen;
	int n;

	if (cppoff)
		return;

	disexpand = 0;
	namespace = NS_IDEN;
	next();

	switch (*yytext) {
	case '<':
		if ((p = strchr(input->begin, '>')) == NULL || p[-1] == '<')
			goto bad_include;
		filelen = p - input->begin;
		if (filelen >= FILENAME_MAX)
			goto too_long;
		memcpy(file, input->begin, filelen);
		file[filelen] = '\0';

		input->begin = input->p = p+1;
		if (next() != '\n')
			goto trailing_characters;

		break;
	case '"':
		if (yylen < 3)
			goto bad_include;
		filelen = yylen-2;
		if (filelen >= FILENAME_MAX)
			goto too_long;
		memcpy(file, yytext+1, filelen);
		file[filelen] = '\0';

		if (next() != '\n')
			goto trailing_characters;

		if (includefile(cwd(dir), file, filelen))
			goto its_done;
		break;
	default:
		goto bad_include;
	}

	n = dirinclude.n;
	for (bp = dirinclude.s; n--; ++bp) {
		if (includefile(*bp, file, filelen))
			goto its_done;
	}
	cpperror("included file '%s' not found", file);

its_done:
	return;

trailing_characters:
	cpperror("trailing characters after preprocessor directive");
	return;

too_long:
	cpperror("too long file name in #include");
	return;

bad_include:
	cpperror("#include expects \"FILENAME\" or <FILENAME>");
	return;
}

static void
line(void)
{
	long n;
	char *endp, *fname;

	if (cppoff)
		return;

	disexpand = 0;
	next();
	n = strtol(yytext, &endp, 10);
	if (n <= 0 || n > USHRT_MAX || *endp != '\0') {
		cpperror("first parameter of #line is not a positive integer");
		return;
	}

	next();
	if (yytoken == '\n') {
		fname = NULL;
	} else {
		if (*yytext != '\"' || yylen == 1) {
			cpperror("second parameter of #line is not a valid filename");
			return;
		}
		fname = yylval.sym->u.s;
	}
	setloc(fname, n - 1);
	if (yytoken != '\n')
		next();
}

static void
pragma(void)
{
	if (cppoff)
		return;
	next();
	warn("ignoring pragma '%s'", yytext);
	*input->p = '\0';
	next();
}

static void
usererr(void)
{
	if (cppoff)
		return;
	cpperror("#error %s", input->p);
	next();
}


Node *
defined(void)
{
	Symbol *sym;
	int paren;

	disexpand = 1;
	next();
	paren = accept('(');
	if (yytoken != IDEN && yytoken != TYPEIDEN)
		cpperror("operator 'defined' requires an identifier");
	if (yytoken == TYPEIDEN || !(yylval.sym->flags & SDECLARED))
		sym = zero;
	else
		sym = one;
	disexpand = 0;
	next();
	if (paren)
		expect(')');
	return constnode(sym);
}

static void
ifclause(int negate, int isifdef)
{
	Symbol *sym;
	unsigned n;
	int status;
	Node *expr;

	if (cppctx == NR_COND-1)
		error("too many nesting levels of conditional inclusion");

	n = cppctx++;
	namespace = NS_CPP;
	next();

	if (isifdef) {
		if (yytoken != IDEN) {
			cpperror("no macro name given in #%s directive",
			         (negate) ? "ifndef" : "ifdef");
			return;
		}
		sym = yylval.sym;
		next();
		status = (sym->flags & SDECLARED) != 0;
		if (!status)
			killsym(sym);
	} else {
		/* TODO: catch recovery here */
		if ((expr = constexpr()) == NULL) {
			cpperror("parameter of #if is not an integer constant expression");
			return;
		}
		status = expr->sym->u.i != 0;
		freetree(expr);
	}

	if (negate)
		status = !status;
	if ((ifstatus[n] = status) == 0)
		++cppoff;
}

static void
cppif(void)
{
	disexpand = 0;
	ifclause(0, 0);
}

static void
ifdef(void)
{
	ifclause(0, 1);
}

static void
ifndef(void)
{
	ifclause(1, 1);
}

static void
elseclause(void)
{
	int status;

	if (cppctx == 0) {
		cpperror("#else without #ifdef/ifndef");
		return;
	}

	status = ifstatus[cppctx-1];
	ifstatus[cppctx-1] = !status;
	cppoff += (status) ? 1 : -1;
}

static void
cppelse(void)
{
	elseclause();
	next();
}

static void
elif(void)
{
	if (cppctx == 0) {
		cpperror("#elif without #ifdef/ifndef");
		return;
	}

	elseclause();
	if (ifstatus[cppctx-1]) {
		--cppctx;
		cppif();
	}
}

static void
endif(void)
{
	if (cppctx == 0)
		error("#endif without #if");
	if (!ifstatus[--cppctx])
		--cppoff;
	next();
}

static void
undef(void)
{
	if (cppoff)
		return;

	namespace = NS_CPP;
	next();
	if (yytoken != IDEN) {
		error("no macro name given in #undef directive");
		return;
	}
	killsym(yylval.sym);
	next();
}

int
cpp(void)
{
	static struct {
		unsigned char token;
		void (*fun)(void);
	} *bp, clauses [] = {
		{DEFINE, define},
		{INCLUDE, include},
		{LINE, line},
		{IFDEF, ifdef},
		{IF, cppif},
		{ELIF, elif},
		{IFNDEF, ifndef},
		{ELSE, cppelse},
		{ENDIF, endif},
		{UNDEF, undef},
		{PRAGMA, pragma},
		{ERROR, usererr},
		{0, NULL}
	};
	int ns;
	char *p;

	for (p = input->p; isspace(*p); ++p)
		;

	if (*p != '#') {
		if (cppoff)
			*input->p = '\0';
		return cppoff;
	}
	input->p = p+1;

	disexpand = 1;
	lexmode = CPPMODE;
	ns = namespace;
	namespace = NS_CPPCLAUSES;
	next();
	namespace = NS_IDEN;

	if (yytoken == '\n')
		goto ret;

	for (bp = clauses; bp->token && bp->token != yytoken; ++bp)
		;
	if (!bp->token) {
		errorp("incorrect preprocessor directive '%s'", yytext);
		goto ret;
	}

	DBG("CPP %s", yytext);

	/*
	 * create a new context to avoid polish the current context,
	 * and to get all the symbols freed at the end
	 */
	pushctx();
	(*bp->fun)();
	popctx();

	/*
	 * #include changes the content of input->line, so the correctness
	 * of the line must be checked in the own include(), and we have
	 * to skip this tests. For the same reason include() is the only
	 * function which does not prepare the next token
	 */
	if (bp->token == INCLUDE)
		goto ret;

	if (yytoken != '\n' && yytoken != EOFTOK && !cppoff)
		cpperror("trailing characters after preprocessor directive");

ret:
	disescape = 0;
	disexpand = 0;
	lexmode = CCMODE;
	namespace = ns;

	/*
	 * at this point we know that the cpp line is processed, and any error
	 * is generated but as next is called we cannot be sure that input is
	 * valid anymore, but in case of begin valid we want to discard any
	 * pending input in the current line
	 */
	if (input)
		*input->p = '\0';

	return 1;
}

void
ppragmaln(void)
{
	static char file[FILENAME_MAX];
	static unsigned nline;
	char *s;

	putchar('\n');
	if (strcmp(file, filenam)) {
		strcpy(file, filenam);
		s = "#line %u \"%s\"\n";
	} else if (nline+1 != lineno) {
		s = "#line %u\n";
	} else {
		s = "";
	}
	nline = lineno;
	printf(s, nline, file);
}

void
outcpp(void)
{
	int c;
	char *s, *t;

	for (next(); yytoken != EOFTOK; next()) {
		if (onlyheader)
			continue;
		if (yytoken != STRING) {
			printf("%s ", yytext);
			continue;
		}
		for (s = yytext; (c = *s) != '\0'; ++s) {
			switch (c) {
			case '\n':
				t = "\\n";
				goto print_str;
			case '\v':
				t = "\\v";
				goto print_str;
			case '\b':
				t = "\\b";
				goto print_str;
			case '\t':
				t = "\\t";
				goto print_str;
			case '\a':
				t = "\\a";
				goto print_str;
			case '\f':
				t = "\\f";
				goto print_str;
			case '\r':
				t = "\\r";
				goto print_str;
			case '"':
				if (s == yytext || s[1] == '\0')
					goto print_chr;
				t = "\\\"";
				goto print_str;
			case '\'':
				t = "\\'";
				goto print_str;
			case '\?':
				t = "\\\?";
				goto print_str;
			case '\\':
				putchar('\\');
			default:
			print_chr:
				if (!isprint(c))
					printf("\\x%x", c);
				else
					putchar(c);
				break;
			print_str:
				fputs(t, stdout);
				break;
			}
		}
		putchar(' ');
	}
	putchar('\n');
}
