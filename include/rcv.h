#ifndef RCV_H
#define RCV_H

#include <glib.h>

typedef struct rcv_t {
	const gchar *progname;
	const gchar *dir_name;
	const gchar *srcpkgs;
	gchar *path;
	GDir *dir_p;
	GError *err;
} rcv_t;

rcv_t *rcv_init(const gchar *);
void rcv_free(rcv_t *);
void usage(rcv_t *) __attribute__((noreturn));

#endif /* RCV_H */
