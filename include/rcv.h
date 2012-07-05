#ifndef RCV_H
#define RCV_H

#include <glib.h>
#include <xbps_api.h>

typedef struct rcv_t {
	const gchar *progname;
	const gchar *pkgname;
	const gchar *srcpkgs;
	gchar *path;
	gchar *tmpl;
	GDir *dir_p;
	GError *err;
} rcv_t;

rcv_t *rcv_init(const gchar *);
void rcv_free(rcv_t *);

void rcv_parse_tmpl(rcv_t *, struct xbps_handle *, const gchar *);

void rcv_usage(rcv_t *);

#endif /* RCV_H */
