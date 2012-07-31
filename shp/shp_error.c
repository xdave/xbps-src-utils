#include "shp_api.h"
#include "shp_parser.h"
#include "shp_lexer.h"

void
shp_error(SHPLTYPE *loc, shp *s, struct yyguts_t *yyg, const char *message)
{
	yyg = yyg; /* get rid of gcc warning */

	fprintf(stderr, "%s:%d:%d-%d: %s\n",
		s->fname,
		loc->first_line,
		loc->first_column,
		loc->last_column,
		message);
}
