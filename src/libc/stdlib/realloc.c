#include <stdlib.h>
#include <string.h>

#include "malloc.h"
#undef realloc

void *
realloc(void *ptr, size_t nbytes)
{
	Header *oh, *prev, *next, *new;
	size_t nunits, avail, onbytes, n;

	if (nbytes == 0)
		return NULL;

	if (!ptr)
		return malloc(nbytes);

	nunits = (nbytes+sizeof(Header)-1)/sizeof(Header) + 1;
	oh = (Header *) ptr - 1;

	if (oh->h.size == nunits)
		return ptr;

	new = oh + nunits;

	if (nunits < oh->h.size) {
		new->h.size = oh->h.size - nunits;
		oh->h.size = nunits;
		free(new + 1);
		return ptr;
	}

	prev = _prevchunk(oh);
	next = prev->h.next;

	if (oh + oh->h.size == next) {
		/*
		 * if there is free space adjacent
		 * to the current memory
		 */
		avail = oh->h.size + next->h.size;

		if (avail == nunits) {
			oh->h.size = nunits;
			prev->h.next = next->h.next;
			return ptr;
		}

		if (nunits < avail) {
			oh->h.size = nunits;
			prev->h.next = new;
			new->h.next = next->h.next;
			new->h.size = avail - nunits;
			return ptr;
		}
	}

	if ((new = malloc(nbytes)) == NULL)
		return NULL;

	n = (oh->h.size - 1) * sizeof(Header);
	if (n > nbytes)
		n = nbytes;
	memcpy(new, ptr, n);
	free(ptr);

	return new;
}
