#include <glib.h>
#include <glib/gprintf.h>
#include <stdlib.h>
#include <string.h>
#include <xbps_api.h>
#include "rcv.h"

void
rcv_parse_tmpl(rcv_t *r, struct xbps_handle *xh, const gchar *file)
{
	prop_dictionary_t pkgd;
	const gchar *repover;
	gchar *srcpkgver;
	gchar *data;
	gsize len;
	GRegex *regex;
	GMatchInfo *match_info;
	GHashTable *ht;
	int i;
	const gchar *replacements[] = { "$", "{", "}", "\"", "'" };

	ht = g_hash_table_new_full(g_str_hash, hash_equal, g_free, g_free);

	pkgd = xbps_rpool_find_pkg(xh, r->pkgname, false, true);
	if (pkgd == NULL) { return; }
	prop_dictionary_get_cstring_nocopy(pkgd, "version", &repover);

	g_file_get_contents(file, &data, &len, &r->err);
	if (r->err != NULL) {
		g_fprintf(stderr, "%s\n", r->err->message);
		rcv_free(r);
		exit(EXIT_FAILURE);
	}

	r->err = NULL;
	regex = g_regex_new("^(__?[A-z]+|revision|version)=.*$",
			G_REGEX_MULTILINE, 0, NULL);
	g_regex_match_full(regex, data, -1, 0, 0, &match_info, &r->err);
	while (g_match_info_matches(match_info)) {
		gchar *word = g_match_info_fetch(match_info, 0);
		gchar **set = g_strsplit(word, "=", 2);
		g_hash_table_insert(ht, g_strdup(set[0]), g_strdup(set[1]));
		g_free(word);
		g_strfreev(set);
		g_match_info_next(match_info, &r->err);
	}

	g_match_info_free(match_info);
	g_regex_unref(regex);
	g_free(data);

	if (r->err != NULL) {
		g_printerr("ERROR finding version: %s\n", r->err->message);
		exit(EXIT_FAILURE);
	}

	g_hash_table_foreach(ht, replace_shell_vars, ht);

	for (i = 0; i < 5; i++)
		replace_in_version(ht, replacements[i], "");

	srcpkgver = g_strdup_printf("%s_%s",
		(const gchar*)g_hash_table_lookup(ht, "version"),
		(const gchar*)g_hash_table_lookup(ht, "revision"));
	if (xbps_cmpver(repover, (const gchar *)srcpkgver) == -1) {
		printf("pkgname: %s ", r->pkgname);
		printf("repover: %s ", repover);
		printf("srcpkgver: %s\n", (const gchar *)srcpkgver);
	}

	g_free(srcpkgver);
	g_hash_table_destroy(ht);
}
