#include <stdlib.h>
#include <string.h>
#include <xbps_api.h>
#include <glib/gprintf.h>

#include "rcv.h"

rcv_t *
rcv_init(gint argc, gchar **argv)
{
	r = g_malloc0(sizeof(rcv_t));
	r->progname = argv[0];
	r->err = NULL;
	r->regex = g_regex_new("^(__?[A-z]+|revision|version)=.*$",
			G_REGEX_MULTILINE, 0, NULL);
	r->ht = g_hash_table_new_full(g_str_hash, hash_equal, g_free, g_free);

	if (argc < 2) rcv_usage();
	r->srcpkgs = argv[1];

	r->dir_p = g_dir_open(r->srcpkgs, 0, &r->err);
	if (r->err != NULL) {
		g_fprintf(stderr, "%s\n", r->err->message);
		rcv_usage();
	}

	if (g_strrstr(r->srcpkgs, "srcpkgs") == NULL) {
		g_dir_close(r->dir_p);
		g_fprintf(stderr, "Error: %s\n",
			  "This doesn't appear to be a srcpkgs directory.");
		rcv_usage();
	}

	return r;
}

void
rcv_free(void)
{
	if (r->err != NULL) g_error_free(r->err);
	g_regex_unref(r->regex);
	g_hash_table_destroy(r->ht);
	g_free(r);
}
