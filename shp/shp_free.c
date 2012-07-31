#include "shp_api.h"
#include "shp_parser.h"
#include "shp_lexer.h"
#include "xmem.h"

void
shp_end(shp *s)
{
	shp_delete_buffer(s->state, s->scanner);
	shplex_destroy(s->scanner);
	if (s->input != NULL) xfree(s->input);
	if (s->file != NULL) fclose(s->file);
}

void
shp_free(shp *s)
{
	shp_end(s);
	xfree(s);
}
