#include <stdio.h>
#include <stdarg.h>
#include "shp_api.h"

void
shp_xtrace(shp *s, const char *fmt, ...)
{
	va_list ap;
	if (s->print_xtrace == 1) {
		va_start(ap, fmt);
		vfprintf(stderr, fmt, ap);
		va_end(ap);
	}
}
