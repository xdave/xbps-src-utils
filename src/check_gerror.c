#include <glib.h>
#include <stdlib.h>
#include "rcv.h"

void
check_gerror(GError *err)
{
	if (err != NULL) {
		fprintf(stderr, "%s\n", err->message);
		rcv_free();
		exit(EXIT_FAILURE);
	}
}
