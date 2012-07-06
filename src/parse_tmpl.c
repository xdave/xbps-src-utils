#include <glib.h>
#include <glib/gprintf.h>
#include <stdlib.h>
#include <string.h>
#include <xbps_api.h>
#include "rcv.h"

void
rcv_parse_tmpl(struct xbps_handle *xh, const gchar *file)
{
	gchar *srcpkgver;
	gchar *data;
	gsize len;
	const gchar *replacements[] = { "$", "{", "}", "\"", "'" };

	r->pkgd = xbps_rpool_find_pkg(xh, r->pkgname, false, true);
	if (r->pkgd == NULL) { return; }
	prop_dictionary_get_cstring_nocopy(r->pkgd, "version", &r->repover);

	g_file_get_contents(file, &data, &len, &r->err);
	if (r->err != NULL) {
		g_fprintf(stderr, "%s\n", r->err->message);
		rcv_free();
		exit(EXIT_FAILURE);
	}

	r->err = NULL;
	g_regex_match_full(r->regex, data, -1, 0, 0, &r->match_info, &r->err);
	while (g_match_info_matches(r->match_info)) {
		gchar *word = g_match_info_fetch(r->match_info, 0);
		gchar **set = g_strsplit(word, "=", 2);
		g_hash_table_insert(r->ht, g_strdup(set[0]), g_strdup(set[1]));
		g_free(word);
		g_strfreev(set);
		g_match_info_next(r->match_info, &r->err);
	}

	g_match_info_free(r->match_info);
	g_free(data);

	if (r->err != NULL) {
		g_printerr("ERROR finding version: %s\n", r->err->message);
		rcv_free();
		exit(EXIT_FAILURE);
	}

	g_hash_table_foreach(r->ht, replace_shell_vars, r->ht);

	{
		guint i = 0;
		for (;i < G_N_ELEMENTS(replacements); i++)
			replace_in_version(r->ht, replacements[i], "");
	}

	srcpkgver = g_strdup_printf("%s_%s",
		(const gchar*)g_hash_table_lookup(r->ht, "version"),
		(const gchar*)g_hash_table_lookup(r->ht, "revision"));
	if (xbps_cmpver(r->repover, (const gchar *)srcpkgver) == -1) {
		printf("pkgname: %s ", r->pkgname);
		printf("repover: %s ", r->repover);
		printf("srcpkgver: %s\n", (const gchar *)srcpkgver);
	}

	g_free(srcpkgver);
	g_hash_table_remove_all(r->ht);
}
