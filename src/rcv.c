#include <stdlib.h>
#include <string.h>
#include <xbps_api.h>
#include <glib/gprintf.h>

#include "rcv.h"

rcv_t *
rcv_init(const gchar *progname)
{
	rcv_t *r = g_malloc0(sizeof(rcv_t));
	r->progname = progname;
	r->err = NULL;
	return r;
}

void
rcv_free(rcv_t *r)
{
	if (r->err != NULL) g_error_free(r->err);
	g_free(r);
}
