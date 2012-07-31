#include <stdio.h>
#include <stdlib.h>
#include "chkvers.h"

int
chkvers_show_usage(const char *progname)
{
	fprintf(stderr, "Usage: %s [-hcCdxD] [FILEs]\n", progname);
	fprintf(stderr, "%s\n", "  -h		Show this message.");
	fprintf(stderr, "%s\n", "  -c		Override xbps-src.conf");
	fprintf(stderr, "%s\n", "  -C		Override xbps.conf");
	fprintf(stderr, "%s\n", "  -d		Override XBPS_DISTDIR");
	fprintf(stderr, "%s\n", "  -x		Show script xtrace info.");
	fprintf(stderr, "%s\n", "  -D		Show parser debug info.");
	fprintf(stderr, "%s\n", "  [FILEs]	Parse files from disk.");
	return (EXIT_FAILURE);
}
