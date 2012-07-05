#include <errno.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <stdlib.h>
#include <string.h>
#include "rcv.h"

int
main(int argc, char** argv)
{
	struct xbps_handle xh;
	int rv;

	rcv_t *r = rcv_init(argv[0]);
	if (argc < 2) rcv_usage(r);
	r->srcpkgs = argv[1];

	r->dir_p = g_dir_open(r->srcpkgs, 0, &r->err);
	if (r->err != NULL) {
		g_fprintf(stderr, "%s\n", r->err->message);
		rcv_usage(r);
	}

	if (g_strrstr(r->srcpkgs, "srcpkgs") == NULL) {
		g_dir_close(r->dir_p);
		g_fprintf(stderr, "Error: %s\n",
			  "This doesn't appear to be a srcpkgs directory.");
		rcv_usage(r);
	}

	memset(&xh, 0, sizeof(xh));
	if ((rv = xbps_init(&xh)) != 0) {
		g_fprintf(stderr, "xbps_init() failed: %s\n", strerror(rv));
		rcv_free(r);
		exit(EXIT_FAILURE);
	}

	while ((r->pkgname = g_dir_read_name(r->dir_p)) != NULL) {
		r->path = g_build_filename(r->srcpkgs,
					   r->pkgname,
					   (gchar*)NULL);
		if(g_file_test(r->path, G_FILE_TEST_IS_SYMLINK) == FALSE) {
			r->tmpl = g_build_filename(r->path,
						   "template",
						   (gchar*)NULL);
			if (g_file_test(r->tmpl, G_FILE_TEST_EXISTS) == TRUE) {
				if (g_strcmp0(r->pkgname, "apache-mpm-event") != 0 &&
				    g_strcmp0(r->pkgname, "apache-mpm-worker") != 0 &&
				    g_strcmp0(r->pkgname, "gvim") != 0 &&
				    g_strcmp0(r->pkgname, "poppler-qt4") != 0) {
					rcv_parse_tmpl(r, &xh, r->tmpl);
				}
			} else {
				g_fprintf(stderr, "'%s': %s\n",
					  r->tmpl, strerror(ENOENT));
				g_free(r->tmpl);
				g_free(r->path);
				break;
			}
			g_free(r->tmpl);
		}
		g_free(r->path);
	}

	g_dir_close(r->dir_p);
	xbps_end(&xh);
	rcv_free(r);
	exit(EXIT_SUCCESS);
}
