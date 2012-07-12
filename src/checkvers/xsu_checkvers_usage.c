#include <stdio.h>
#include <stdlib.h>
#include "xsu_api.h"

void
xsu_checkvers_usage(void)
{
	printf(
"Usage: %s <dir>\n"
"Options:\n"
"\t<dir>\tlocation of the xbps-src source pkgs.\n"
"\t\t(ie. $HOME/xbps-packages/srcpkgs)\n\n"
"This program shows which XBPS binary packages need to be rebuilt on your\n"
"system by comparing the versions of the binary packages which are available \n"
"in the XBPS repositories registered in your `xbps.conf' with the latest \n"
"available versions of them in the source package tree.\n\n",
		checkvers->progname);
	xsu_checkvers_free();
	exit(EXIT_FAILURE);
}

