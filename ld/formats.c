static char sccsid[] = "@(#) ./ld/probe.c";

#include <stdio.h>

#include "../inc/scc.h"
#include "ld.h"

/* TODO: Autogenerate this file */
struct objfmt coff32;

struct objfmt *formats[] = {
	&coff32,
	NULL,
};
