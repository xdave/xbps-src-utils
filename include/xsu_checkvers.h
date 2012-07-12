#ifndef XSU_CHECKVERS_H
#define XSU_CHECKVERS_H

#include <stdbool.h>
#include <glib.h>

struct _prop_dictionary;
typedef struct _prop_dictionary *prop_dictionary_t;
struct xbps_handle;

struct _xsu_checkvers;
typedef struct _xsu_rcv {
	const char *progname, *pkgname, *srcpkgs, *repover;
	char *path, *tmpl;
	GDir *dir_p;
	GError *err;
	GRegex *regex;
	GMatchInfo *match_info;
	GHashTable *ht;
	GDateTime *utc;
	prop_dictionary_t pkgd;
} xsu_checkvers_t;

/* main program stuff */
void xsu_checkvers_init(int, char**);
void xsu_checkvers_free(void);
void xsu_checkvers_parse_tmpl(struct xbps_handle*, const char*);
void xsu_checkvers_usage(void) __attribute__ ((noreturn));

#endif /* XSU_CHECKVERS_H */
