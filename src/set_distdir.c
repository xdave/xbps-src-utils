#include <stdlib.h>
#include <string.h>

#include "rcv.h"

void
rcv_set_distdir(rcv_t *rcv, const char *distdir)
{
	if (rcv->distdir == NULL && rcv->pkgdir == NULL) {
		rcv->distdir = strdup(distdir);
		rcv->pkgdir = strdup(distdir);
		rcv->pkgdir = realloc(rcv->pkgdir,
			sizeof(char)*(strlen(distdir)+strlen("/srcpkgs")+1));
		rcv->pkgdir = strcat(rcv->pkgdir, "/srcpkgs");
	}
}
