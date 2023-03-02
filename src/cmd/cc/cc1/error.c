#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <scc/scc.h>
#include "cc1.h"

#define MAXERRNUM 10

extern int failure;
static unsigned nerrors;

#ifndef NDEBUG
char *
tokstr(int tok)
{
	char *strings[] = {
		[CONST] = "CONST",
		[RESTRICT] = "RESTRICT",
		[VOLATILE] = "VOLATILE",
		[INLINE] = "INLINE",
		[TQUALIFIER] = "TQUALIFIER",
		[MACROPAR] = "MACROPAR",
		[CONCAT] = "CONCAT",
		[STRINGIZE] = "STRINGIZE",
		[TYPE] = "TYPE",
		[IDEN] = "IDEN",
		[SCLASS] = "SCLASS",
		[CONSTANT] = "CONSTANT",
		[STRING] = "STRING",
		[SIZEOF] = "SIZEOF",
		[INDIR] = "INDIR",
		[INC] = "INC",
		[DEC] = "DEC",
		[SHL] = "SHL",
		[SHR] = "SHR",
		[LE] = "LE",
		[GE] = "GE",
		[EQ] = "EQ",
		[NE] = "NE",
		[AND] = "AND",
		[OR] = "OR",
		[MUL_EQ] = "MUL_EQ",
		[DIV_EQ] = "DIV_EQ",
		[MOD_EQ] = "MOD_EQ",
		[ADD_EQ] = "ADD_EQ",
		[SUB_EQ] = "SUB_EQ",
		[AND_EQ] = "AND_EQ",
		[XOR_EQ] = "XOR_EQ",
		[OR_EQ] = "OR_EQ",
		[SHL_EQ] = "SHL_EQ",
		[SHR_EQ] = "SHR_EQ",
		[ELLIPSIS] = "ELLIPSIS",
		[CASE] = "CASE",
		[DEFAULT] = "DEFAULT",
		[IF] = "IF",
		[ELSE] = "ELSE",
		[SWITCH] = "SWITCH",
		[WHILE] = "WHILE",
		[DO] = "DO",
		[FOR] = "FOR",
		[GOTO] = "GOTO",
		[VOID] = "VOID",
		[FLOAT] = "FLOAT",
		[INT] = "INT",
		[BOOL] = "BOOL",
		[VA_LIST] = "VA_LIST",
		[STRUCT] = "STRUCT",
		[UNION] = "UNION",
		[CHAR] = "CHAR",
		[DOUBLE] = "DOUBLE",
		[SHORT] = "SHORT",
		[LONG] = "LONG",
		[LLONG] = "LLONG",
		[COMPLEX] = "COMPLEX",
		[TYPEDEF] = "TYPEDEF",
		[EXTERN] = "EXTERN",
		[STATIC] = "STATIC",
		[AUTO] = "AUTO",
		[REGISTER] = "REGISTER",
		[ENUM] = "ENUM",
		[TYPEIDEN] = "TYPEIDEN",
		[UNSIGNED] = "UNSIGNED",
		[SIGNED] = "SIGNED",
		[CONTINUE] = "CONTINUE",
		[BREAK] = "BREAK",
		[RETURN] = "RETURN",
		[DEFINE] = "DEFINE",
		[DEFINED] = "DEFINED",
		[INCLUDE] = "INCLUDE",
		[LINE] = "LINE",
		[PRAGMA] = "PRAGMA",
		[ERROR] = "ERROR",
		[IFDEF] = "IFDEF",
		[ELIF] = "ELIF",
		[IFNDEF] = "IFNDEF",
		[UNDEF] = "UNDEF",
		[ENDIF] = "ENDIF",
		[BUILTIN] = "BUILTIN",
		[EOFTOK] = "EOFTOK"
	};

	if (tok >= NELEM(strings) || !strings[tok])
		return "unkown token";
	return strings[tok];
}

char *
opstr(int op)
{
	char *strings[] = {
		[OADD] = "OADD",
		[OMUL] = "OMUL",
		[OSUB] = "OSUB",
		[OINC] = "OINC",
		[ODEC] = "ODEC",
		[ODIV] = "ODIV",
		[OMOD] = "OMOD",
		[OSHL] = "OSHL",
		[OSHR] = "OSHR",
		[OBAND] = "OBAND",
		[OBXOR] = "OBXOR",
		[OBOR] = "OBOR",
		[OSNEG] = "OSNEG",
		[ONEG] = "ONEG",
		[OCPL] = "OCPL",
		[OAND] = "OAND",
		[OOR] = "OOR",
		[OEQ] = "OEQ",
		[ONE] = "ONE",
		[OLT] = "OLT",
		[OGE] = "OGE",
		[OLE] = "OLE",
		[OGT] = "OGT",
		[OASSIGN] = "OASSIGN",
		[OA_MUL] = "OA_MUL",
		[OA_DIV] = "OA_DIV",
		[OA_MOD] = "OA_MOD",
		[OA_ADD] = "OA_ADD",
		[OA_SUB] = "OA_SUB",
		[OA_SHL] = "OA_SHL",
		[OA_SHR] = "OA_SHR",
		[OA_AND] = "OA_AND",
		[OA_XOR] = "OA_XOR",
		[OA_OR] = "OA_OR",
		[OADDR] = "OADDR",
		[OCOMMA] = "OCOMMA",
		[OCAST] = "OCAST",
		[OPTR] = "OPTR",
		[OSYM] = "OSYM",
		[OASK] = "OASK",
		[OCOLON] = "OCOLON",
		[OFIELD] = "OFIELD",
		[OLABEL] = "OLABEL",
		[ODEFAULT] = "ODEFAULT",
		[OCASE] = "OCASE",
		[OJUMP] = "OJUMP",
		[OBRANCH] = "OBRANCH",
		[OEXPR] = "OEXPR",
		[OEFUN] = "OEFUN",
		[OELOOP] = "OELOOP",
		[OBLOOP] = "OBLOOP",
		[OFUN] = "OFUN",
		[OPAR] = "OPAR",
		[OCALL] = "OCALL",
		[OCALLE] = "OCALLE",
		[ORET] = "ORET",
		[ODECL] = "ODECL",
		[OBSWITCH] = "OBSWITCH",
		[OESWITCH] = "OESWITCH",
		[OINIT] = "OINIT",
		[OBUILTIN] = "OBUILTIN",
		[OTYP] = "OTYP",
	};

	if (op >= NELEM(strings) || !strings[op])
		return "unkown operation";
	return strings[op];
}
#endif

static void
warn_error(int flag, char *fmt, va_list va)
{
	if (flag == 0)
		return;
	fprintf(stderr, "%s:%u: %s: ",
	       filenam, lineno,
	       (flag < 0) ? "error" : "warning");
	vfprintf(stderr, fmt, va);
	putc('\n', stderr);

	if (flag < 0) {
		if (!failure)
			fclose(stdout);
		failure = 1;
		if (++nerrors == MAXERRNUM) {
			fputs("too many errors\n", stderr);
			exit(1);
		}
	}
}

void
warn(char *fmt, ...)
{
	extern int warnings;

	va_list va;
	va_start(va, fmt);
	warn_error(warnings, fmt, va);
	va_end(va);
}

void
error(char *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	warn_error(-1, fmt, va);
	va_end(va);
	discard();
}

void
errorp(char *fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	warn_error(-1, fmt, va);
	va_end(va);
}

void
cpperror(char *fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	warn_error(-1, fmt, va);
	va_end(va);

	/* discard input until the end of the line */
	if (input)
		*input->p = '\0';
	next();
}

void
unexpected(void)
{
	error("unexpected '%s'", yytext);
}
