/* See LICENSE file for copyright and license details. */

#define __USE_MACROS
#include <ctype.h>

int
tolower(int c)
{
	return (isupper(c)) ? c & ~0x20 : c;
}