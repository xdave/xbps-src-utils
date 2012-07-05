#ifndef REPO_CHECKVERS_H
#define REPO_CHECKVERS H

#include <glib.h>
#include <glib/gprintf.h>

typedef struct rcv_t {
	GDir *dir_p;
	const gchar *dir_name;
	const gchar *srcpkgs;
	gchar *path;
} rcv_t;

#endif /* REPO_CHECKVERS_H */
