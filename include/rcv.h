#ifndef REPO_CHECKVERS_H
#define REPO_CHECKVERS H

#include <glib.h>

typedef struct rcv_t {
	GDir *dir_p;
	const gchar *dir_name;
	const gchar *srcpkgs;
	gchar *path;
} rcv_t;

rcv_t *rcv_init();

void usage(gchar *);

#endif /* REPO_CHECKVERS_H */
