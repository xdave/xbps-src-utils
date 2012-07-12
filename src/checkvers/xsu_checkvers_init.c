#include <stdlib.h>
#include <string.h>
#include <xbps_api.h>
#include <glib/gprintf.h>
#include "xsu_api.h"

void
xsu_checkvers_init(int argc, char **argv)
{
	checkvers = g_malloc0(sizeof(xsu_checkvers_t));
	checkvers->progname = argv[0];
	checkvers->err = NULL;
	checkvers->regex = g_regex_new("^(__?[A-z]+|revision|version)=.*$",
			G_REGEX_MULTILINE, 0, NULL);
	checkvers->ht = g_hash_table_new_full(g_str_hash,
					      (GEqualFunc)xsu_hash_str_cmp,
					      g_free, g_free);
	checkvers->utc = g_date_time_new_now_utc();

	if (argc < 2) xsu_checkvers_usage();
	checkvers->srcpkgs = argv[1];

	checkvers->dir_p = g_dir_open(checkvers->srcpkgs, 0, &checkvers->err);
	if (checkvers->err != NULL) {
		g_fprintf(stderr, "%s\n", checkvers->err->message);
		xsu_checkvers_usage();
	}

	if (g_strrstr(checkvers->srcpkgs, "srcpkgs") == NULL) {
		g_dir_close(checkvers->dir_p);
		g_fprintf(stderr, "Error: %s\n",
			  "This doesn't appear to be a srcpkgs directory.");
		xsu_checkvers_usage();
	}
}


