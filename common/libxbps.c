/*#include <errno.h>*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <xbps_api.h>
#include "libxbps.h"

xbps *
xhp_init()
{
	int rv;
	xbps *x;

	x = calloc(1, sizeof(xbps));
	x->xh = calloc(1, sizeof(xhp));
	if ((rv = xbps_init(x->xh)) != 0) {
		fprintf(stderr, "xbps_init() failed: %s\n", strerror(rv));
		exit(EXIT_FAILURE);
	}
	return x;
}

void
xhp_free(xbps *x)
{
	xbps_end(x->xh);
	free(x);
}

void
xhp_get_pkg_version(xbps *x, const char *pkgname, char **repover)
{
	const char **output = (const char **)repover;
	x->pkgd = xbps_rpool_find_pkg(x->xh, pkgname, false, true);
	if (x->pkgd == NULL) {
		*output = NULL;
		return;
	}
	prop_dictionary_get_cstring_nocopy(x->pkgd, "version", output);
}
