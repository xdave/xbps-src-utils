#ifndef RCV_H
#define RCV_H

#include <glib.h>
#include <xbps_api.h>

typedef struct rcv_t {
	const gchar *progname;
	const gchar *pkgname;
	const gchar *srcpkgs;
	const gchar *repover;
	gchar *path;
	gchar *tmpl;
	GDir *dir_p;
	GError *err;
	GRegex *regex;
	GMatchInfo *match_info;
	GHashTable *ht;
	GDateTime *utc;
	prop_dictionary_t pkgd;
} rcv_t;

rcv_t *r;

rcv_t *rcv_init(gint, gchar **);
void rcv_free(void);
void rcv_parse_tmpl(struct xbps_handle *, const gchar *);
void rcv_usage(void) __attribute__((noreturn));

gchar *str_replace(const gchar *, const gchar *, const gchar *);

void replace_shell_vars(gpointer, gpointer, gpointer);
void g_hash_str_replace(GHashTable *, const gchar *key,
		        const gchar *, const gchar *);
gboolean hash_equal(gconstpointer, gconstpointer);

void check_gerror(GError *);

#endif /* RCV_H */
