#ifndef XSU_API_H
#define XSU_API_H

#include <stdbool.h>
#include <glib.h>

#include "xsu_checkvers.h"

struct _xsu {
	const char *progname, *srcpkgs;
} xsu_t;


/* utility functions */
gchar *xsu_str_replace(const char*, const char*, const char*);

/* GHashTable-related */
void xsu_replace_shell_vars(gpointer, gpointer, gpointer);
void xsu_hash_str_replace(GHashTable*, const char*, const char*, const char*);
bool xsu_hash_str_cmp(void*, void*) __attribute__ ((pure));

/* Error-check related */
void xsu_gerror_check(GError*);

/* global instance for everything */
xsu_checkvers_t *checkvers;

#endif /* XSU_API_H */
