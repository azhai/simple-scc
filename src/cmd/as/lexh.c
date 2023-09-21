#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#ifndef SIGHUP
#define SIGHUP 0
#endif

extern unsigned genhash(char *);

static sig_atomic_t signalled;
static unsigned long M, K, S;

static unsigned
hash(char *s)
{
	unsigned h;

	h = genhash(s);

	return (unsigned short) h;
}

static int
foundmap(unsigned *th, int ntok)
{
	int i;
	unsigned h;
	char bmap[USHRT_MAX];

	for (K = 1; (unsigned short) (K+1) != 0; K += 2) {
		if (signalled) {
			signalled = 0;
			fprintf(stderr, "M=%lu,K=%lu,S=%lu\n", M, K, S);
		}
		memset(bmap, 0, sizeof(bmap));

		for (i = 0; i < ntok; i++) {
			h = (th[i]*K >> S) & M;
			if (bmap[h])
				break;
			bmap[h] = 1;
		}

		if (i == ntok)
			return 1;
	}

	return 0;
}

static void
phash(char *toks[], int ntok)
{
	int i, j, nbits;
	unsigned *th, h;

	if ((th = calloc(ntok, sizeof(*th))) == NULL) {
		perror("hash");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < ntok; i++) {
		h = hash(toks[i]);
		for (j = 0; j < i && th[j] != h; j++)
			;

		if (i != j) {
			fprintf(stderr,
			        "hash: collision %s-%s\n",
			         toks[i],
			         toks[j]);
			exit(EXIT_FAILURE);
		}
		th[i] = h;
	}

	for (nbits = 1; 1<<nbits < ntok; ++nbits)
		;

	for (i = nbits+1; i < 16; i++) {
		M = ~((unsigned) -1 << i);

		for (j = nbits; j < 32; j++) {
			S = 32 - j;

			if (foundmap(th, ntok))
				goto found;
		}
	}

	fputs("hash: not possible perfect hash\n", stderr);
	exit(EXIT_FAILURE);

found:
	printf("unsigned long K=%lu, M=%lu, S=%lu;\n", K, M, S);

	printf("short hashmap[%ld] = {\n", 1<<i);
	for (i = 0; i < ntok; i++)
		printf("\t[%d] = %d,\n", (th[i]*K >> S) & M, i+1);
	puts("};");
}

static void
sighandler(int num)
{
	signalled = 1;
	signal(SIGHUP, sighandler);
}

int
main()
{
	int nr, n;
	char line[BUFSIZ], **tokens, *tok;

	nr = 0;
	tokens = NULL;
	for (;;) {
		if (!fgets(line, sizeof(line), stdin))
			break;
		n = strlen(line);
		if (n == 0)
			continue;
		if (line[n-1] != '\n') {
			fputs("error: line truncated\n", stderr);
			exit(EXIT_FAILURE);
		}
		line[n-1] = '\0';

		tok = malloc(n);
		tokens = realloc(tokens, (nr+1) * sizeof(*tokens));
		if (!tokens || !tok) {
			perror("hash");
			exit(EXIT_FAILURE);
		}
		tokens[nr++] = memcpy(tok, line, n);
	}

	if (ferror(stdin)) {
		perror("hash");
		exit(EXIT_FAILURE);
	}

	signal(SIGHUP, sighandler);

	if (nr != 0)
		phash(tokens, nr);

	return 0;
}
