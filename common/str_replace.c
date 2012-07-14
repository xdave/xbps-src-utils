#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *
str_replace(const char *orig, const char *pat, const char *repl)
{
	size_t orilen = strlen(orig);
	size_t patlen = strlen(pat);
	size_t replen = strlen(repl);
	size_t skplen = 0, retlen = 0, patcnt = 0;

	const char *oriptr = orig;
	const char *patloc;
	char *returned;
	char *retptr;

	/* Copy input to output if this is a useless call */
	if (strcmp(pat, repl) == 0)
	    return strdup(orig);

	/* find how many times the pattern occurs in the original string */
	for (;(patloc = strstr(oriptr, pat)); oriptr = patloc + patlen)
		patcnt++;

	/* allocate memory for the new string */
	retlen = orilen + (patcnt * (replen - patlen));
	returned = malloc(sizeof(char) * (retlen + 1));
	if (returned == NULL) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		exit(1);
	}

	if (returned != NULL) {
		/* copy the original string,
		 * replacing all the instances of the pattern */
		retptr = returned;
		oriptr = orig;
		for (;(patloc = strstr(oriptr, pat)); oriptr = patloc+patlen) {
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
