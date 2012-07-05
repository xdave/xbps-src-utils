#include <glib.h>
#include <glib/gprintf.h>
#include <stdlib.h>
#include <string.h>
#include <xbps_api.h>

#include "rcv.h"

void
rcv_parse_tmpl(rcv_t *r, struct xbps_handle *xh, const gchar *file)
{
	gchar *data;
	gsize len;
	GRegex *regex;
	GMatchInfo *match_info;
	GError *error = NULL;
	const gchar *repover;

	prop_dictionary_t pkgd;

	pkgd = xbps_rpool_find_pkg(xh, r->pkgname, false, true);
	if (pkgd != NULL) {
		prop_dictionary_get_cstring_nocopy(pkgd, "version", &repover);
		/*xbps_end(&xh);*/
		free(pkgd);
	}

	g_file_get_contents(file, &data, &len, &r->err);

	if (r->err != NULL) {
		g_fprintf(stderr, "%s\n", r->err->message);
		rcv_free(r);
		exit(EXIT_FAILURE);
	}

	regex = g_regex_new("(__?[A-z]+|revision|version)=.*", 0, 0, NULL);
	g_regex_match_full(regex, data, -1, 0, 0, &match_info, &error);
	g_print("%s -- repover: %s\n", r->pkgname, repover);
	while (g_match_info_matches(match_info)) {
		gchar *word = g_match_info_fetch(match_info, 0);
		gchar **keyval = g_strsplit(word, "=", 2);
		g_print("%s:\t(%s = '%s')\n", r->pkgname, keyval[0], keyval[1]);
		g_free(word);
		g_strfreev(keyval);
		g_match_info_next(match_info, &error);
	}

	g_match_info_free(match_info);
	g_regex_unref(regex);

	if (error != NULL) {
		g_printerr ("ERROR finding version: %s\n", error->message);
		g_error_free (error);
	}

	g_free(data);
}
