#include <stdlib.h>
#include <string.h>

#include <glib.h>
#include <glib/gprintf.h>

#include "rcv.h"

int
main(int argc, char** argv)
{
	rcv_t *r = rcv_init();
	GError *err = NULL;

	if (argc < 2) {
		g_free(r);
		usage(argv[0]);
	}

	r->srcpkgs = argv[1];

	r->dir_p = g_dir_open(r->srcpkgs, 0, &err);
	if (err != NULL) {
		g_fprintf(stderr, "%s\n", err->message);
		g_error_free(err);
		g_free(r);
		usage(argv[0]);
	}

	if(g_strrstr(r->srcpkgs, "/srcpkgs") == NULL) {
		g_dir_close(r->dir_p);
		g_free(r);
		g_fprintf(stderr, "Error: %s\n",
			"This doesn't appear to be a srcpkgs directory.");
		usage(argv[0]);
	}

	while ((r->dir_name = g_dir_read_name(r->dir_p)) != NULL) {
		r->path = g_build_filename(r->srcpkgs,
					   r->dir_name,
					   (gchar*)NULL);
		if(g_file_test(r->path, G_FILE_TEST_IS_SYMLINK) == FALSE) {
			g_fprintf(stdout, "pkgname: %s\n", r->dir_name);
		}
		g_free(r->path);
	}

	g_dir_close(r->dir_p);
	g_free(r);
	exit(EXIT_SUCCESS);
}
