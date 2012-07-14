#ifndef LIBXBPS_H
#define LIBXBPS_H

typedef struct _prop_dictionary *prop_dictionary_t;
typedef struct xbps_handle xhp;
typedef struct _xbps xbps;

struct xbps_handle;
struct _prop_dictionary;
struct _xbps {
	xhp *xh;
	prop_dictionary_t pkgd;
};

xbps *xhp_init();
void xhp_free(xbps *);
void xhp_get_pkg_version(xbps *, const char *, char **);

#endif /* LIBXBPS_H */
