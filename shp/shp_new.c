#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shp_api.h"
#include "shp_parser.h"
#include "shp_lexer.h"
#include "xmem.h"

shp *
shp_new(const char *fname, const char **accept)
{
	char errbuf[BUFSIZ] = {'\0'};

	shp *s = xmalloc(sizeof(shp));
	if (s == NULL) {
		strerror_r(errno, errbuf, BUFSIZ);
		fprintf(stderr, "Couldn't malloc() sh parser: %s\n", errbuf);
		exit(EXIT_FAILURE);
	}

	shp_init(s, fname, accept);

	return s;
}
