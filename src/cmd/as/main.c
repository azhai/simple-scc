#include <errno.h>
#include <ctype.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/arg.h>
#include <scc/mach.h>
#include <scc/scc.h>

#include "as.h"

char *argv0;
char *outfile = "a.out", *infile;
int endpass;

static void
cleanup(void)
{
	if (outfile)
		remove(outfile);
}

static Ins *
decode(char *s)
{
	int c;
	char *p;
	unsigned h, id;
	Ins *ins;

	for (p = s; c = *p; ++p)
		*p = toupper(c);

	h = (unsigned short) genhash(s);
	h = (h*K >> S) & M;
	id = hashmap[h];
	if (id == 0)
		return NULL;

	ins = &instab[id-1];
	if (strcmp(ins->str, s) != 0)
		return NULL;

	return ins;
}

static void
translate(char *text, char *xargs)
{
	Ins *ins;
	Op *op, *lim;
	Node **args;

	ins = decode(text);
	if (!ins) {
		error("invalid instruction '%s'", text);
		return;
	}

	args = getargs(xargs);
	lim = &optab[ins->end];
	for (op = &optab[ins->begin]; op < lim; ++op) {
		if (match(op, args))
			break;
	}
	if (op == lim) {
		error("invalid operands for '%s'", text);
		return;
	}
	(*op->format)(op, args);
}

static int
dopass(char *fname)
{
	struct line line;
	extern int nerrors;
	extern jmp_buf recover;

	addinput(fname);
	cleansecs();

	endpass = 0;
	setjmp(recover);
	while (!endpass && nextline(&line)) {
		linesym = NULL;

		if (line.label)
			linesym = deflabel(line.label);

		if (line.op)
			translate(line.op, line.args);
		else if (line.args)
			error("arguments without an opcode");

		if (linesym)
			linesym->flags |= FDEF;
	}

	return nerrors == 0;
}

static void
asm(char *argv[])
{
	char **p;

	for (pass = 1; pass <= 2; pass++) {
		for (p = argv; infile = *p; ++p) {
			if (!dopass(infile))
				exit(EXIT_FAILURE);
		}
		if (pass == 1)
			killtmp();
	}
}

static void
usage(void)
{
	fputs("usage: as [-o outfile] filename ...\n", stderr);
	exit(1);
}

int
main(int argc, char *argv[])
{
	ARGBEGIN {
	case 'o':
		outfile = EARGF(usage());
		break;
	default:
		usage();
	} ARGEND

	if (argc == 0)
		usage();

	atexit(cleanup);
	iarch();
	isecs();
	asm(argv);
	writeout(outfile);

	return 0;
}
