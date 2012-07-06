#include <stdlib.h>
#include <glib/gprintf.h>

#include "rcv.h"

void
rcv_usage(rcv_t *r)
{
	g_printf(
"Usage: %s <dir>\n"
"Options:\n"
"\t<dir>\tlocation of the xbps-src source pkgs.\n"
"\t\t(ie. $HOME/xbps-packages/srcpkgs)\n\n"
"This program shows which XBPS binary packages need to be rebuilt on your\n"
"system by comparing the versions of the binary packages which are available \n"
"in the XBPS repositories registered in your `xbps.conf' with the latest \n"
"available versions of them in the source package tree\n\n",
		r->progname);
	rcv_free(r);
	exit(EXIT_FAILURE);
}

