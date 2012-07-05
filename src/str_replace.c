#include <glib.h>
#include <string.h>
#include "rcv.h"

gchar *
str_replace(const gchar *orig, const gchar *pat, const gchar *repl)
{
	size_t orilen = strlen(orig);
	size_t patlen = strlen(pat);
	size_t replen = strlen(repl);
	size_t patcnt = 0;
	size_t skplen;
	size_t retlen;

	const gchar *oriptr = orig;
	const gchar *patloc;
	char *returned;
	char *retptr;

	/* Copy input to output if this is a useless call */
	if (g_strcmp0(pat, repl) == 0)
	    return g_strdup(orig);

	/* find how many times the pattern occurs in the original string */
	for (; (patloc = strstr(oriptr, pat)); oriptr = patloc + patlen)
		patcnt++;

	/* allocate memory for the new string */
	retlen = orilen + patcnt * (replen - patlen);
	returned = g_malloc(retlen + 1);

	if (returned != NULL) {
		/* copy the original string,
		 * replacing all the instances of the pattern */
		retptr = returned;
		oriptr = orig;
		for (; (patloc = strstr(oriptr, pat))
		     ; oriptr = patloc + patlen) {
			skplen = (size_t)(patloc - oriptr);
			/* copy the section until the occurence of pattern*/
			strncpy(retptr, oriptr, skplen);
			retptr += skplen;
			/* copy the replacement */
			strncpy(retptr, repl, replen);
			retptr += replen;
		}
		/* copy the rest of the string. */
		strcpy(retptr, oriptr);
		returned[retlen] = '\0';
	}
	return returned;
}
