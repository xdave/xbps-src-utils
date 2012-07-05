#include <stdlib.h>
#include <glib/gprintf.h>

#include "rcv.h"

void
usage(rcv_t *r)
{
	g_printf(	"Usage: %s <dir>\n"
			"\t<dir>\tlocation of the xbps-src source pkgs.\n"
			"\t\t(ie. $HOME/xbps-packages/srcpkgs)\n",
			r->progname);
	rcv_free(r);
	exit(EXIT_FAILURE);
}

