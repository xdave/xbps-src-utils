#include <stdlib.h>
#include <glib/gprintf.h>

#include "rcv.h"

void
usage(gchar *progname)
{
	g_printf(	"Usage: %s <dir>\n"
			"\t<dir>\tlocation of the xbps-src source pkgs.\n"
			"\t\t(ie. $HOME/xbps-packages/srcpkgs)\n",
			progname);
	exit(EXIT_FAILURE);
}

