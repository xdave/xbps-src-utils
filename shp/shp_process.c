#include "shp_api.h"
#include "shp_parser.h"
#include "shp_lexer.h"

void
shp_process(shp *s, shp *parent)
{
	if (parent != NULL)
		s->print_xtrace = parent->print_xtrace;

	shpparse(s, s->scanner);
}
