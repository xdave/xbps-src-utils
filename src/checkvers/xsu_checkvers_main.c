#include <glib.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <xbps_api.h>
#include "xsu_api.h"

int
main(int argc, char** argv)
{
	int rv;
	struct xbps_handle xh;

	xsu_checkvers_init(argc, argv);

	memset(&xh, 0, sizeof(xh));
	if ((rv = xbps_init(&xh)) != 0) {
		fprintf(stderr, "xbps_init() failed: %s\n", strerror(rv));
		xsu_checkvers_free();
		exit(EXIT_FAILURE);
	}

	while ((checkvers->pkgname = g_dir_read_name(checkvers->dir_p)) != NULL) {
		checkvers->path = g_build_filename(checkvers->srcpkgs,
					   checkvers->pkgname,
					   (gchar*)NULL);
		if(g_file_test(checkvers->path, G_FILE_TEST_IS_SYMLINK) == FALSE) {
			checkvers->tmpl = g_build_filename(checkvers->path,
						   "template",
						   (gchar*)NULL);
			if (g_file_test(checkvers->tmpl, G_FILE_TEST_EXISTS) == TRUE) {
				if (g_strcmp0(checkvers->pkgname, "apache-mpm-event") != 0 &&
				    g_strcmp0(checkvers->pkgname, "apache-mpm-worker") != 0 &&
				    g_strcmp0(checkvers->pkgname, "gvim") != 0 &&
				    g_strcmp0(checkvers->pkgname, "vim-x11") != 0 &&
				    g_strcmp0(checkvers->pkgname, "cyrus-sasl") != 0 &&
				    g_strcmp0(checkvers->pkgname, "poppler-qt4") != 0) {
					xsu_checkvers_parse_tmpl(&xh, checkvers->tmpl);
				}
			} else {
				fprintf(stderr, "'%s': %s\n",
					  checkvers->tmpl, strerror(ENOENT));
				g_free(checkvers->tmpl);
				g_free(checkvers->path);
				break;
			}
			g_free(checkvers->tmpl);
		}
		g_free(checkvers->path);
	}

	g_dir_close(checkvers->dir_p);
	xbps_end(&xh);
	xsu_checkvers_free();
	exit(EXIT_SUCCESS);
}
