#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shp_api.h"
#include "shp_parser.h"
#include "shp_lexer.h"

void
shp_init(shp *s, const char *fname, const char **accept)
{
	char errbuf[BUFSIZ] = {'\0'};

	s = memset(s, 0, sizeof(shp));

	s->accept = accept;
	s->fname = fname;
	s->print_xtrace = 0;
	s->file = stdin;

	if (strcmp(fname, "__STDIN__") != 0) {
		if ((s->file = fopen(fname, "r")) == NULL) {
			strerror_r(errno, errbuf, BUFSIZ);
			fprintf(stderr, "Can't open %s: %s\n", fname, errbuf);
			exit(EXIT_FAILURE);
		}
	}
	shp_scanner_init(s);
	s->state = shp_create_buffer(s->file, YY_BUF_SIZE, s->scanner);
	shpset_in(s->file, s->scanner);
}
