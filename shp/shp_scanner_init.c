#include <stdio.h>
#include <stdlib.h>
#include "shp_api.h"
#include "shp_parser.h"
#include "shp_lexer.h"
#include "xmem.h"

void
shp_scanner_init(shp *s)
{
	if (shplex_init(&s->scanner) != 0) {
		fprintf(stderr, "%s\n", "Error: Can't init scanner!");
		if (s->input != NULL) xfree(s->input);
		if (s->file != NULL) fclose(s->file);
		xfree(s);
		exit(EXIT_FAILURE);
	}
}
