#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include "xsu_api.h"

void
xsu_gerror_check(GError *err)
{
	if (err != NULL) {
		fprintf(stderr, "%s\n", err->message);
		xsu_checkvers_free();
		exit(EXIT_FAILURE);
	}
}
