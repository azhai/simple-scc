
static char sccsid[] = "@(#) ./nm/probe.c";

#include <stdio.h>

#include "nm.h"

/* TODO: Autogenerate this file */
struct objfile coff32;

struct objfile *formats[] = {
	&coff32,
	NULL,
};
