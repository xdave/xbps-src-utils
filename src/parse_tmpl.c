#include <glib.h>
#include <glib/gprintf.h>
#include <stdlib.h>
#include <string.h>
#include <xbps_api.h>
#include "rcv.h"

void
rcv_parse_tmpl(struct xbps_handle *xh, const gchar *file)
{
	gchar *srcpkgver, *data;
	gsize len;

	const gchar *replacements[] = { "$", "{", "}", "\"", "'" };

	r->pkgd = xbps_rpool_find_pkg(xh, r->pkgname, false, true);
	if (r->pkgd == NULL) {
		/*fprintf(stderr, "WARNING: Missing a binpkg for '%s'!\n",
			r->pkgname);*/
		return;
	}
	prop_dictionary_get_cstring_nocopy(r->pkgd, "version", &r->repover);

	g_file_get_contents(file, &data, &len, &r->err);
	check_gerror(r->err);

	g_regex_match_full(r->regex, data, -1, 0, 0, &r->match_info, &r->err);
	while (g_match_info_matches(r->match_info)) {
		gchar *word = g_match_info_fetch(r->match_info, 0);
		gchar **set = g_strsplit(word, "=", 2);
		g_hash_table_insert(r->ht, g_strdup(set[0]), g_strdup(set[1]));
		g_free(word);
		g_strfreev(set);
		g_match_info_next(r->match_info, &r->err);
	}
	check_gerror(r->err);

	g_hash_table_foreach(r->ht, replace_shell_vars, r->utc);

	{
		guint i = 0;
		for (;i < G_N_ELEMENTS(replacements); i++)
			g_hash_str_replace(r->ht, "version",
					   replacements[i], "");
	}

	srcpkgver = g_strdup_printf("%s_%s",
		(const gchar*)g_hash_table_lookup(r->ht, "version"),
		(const gchar*)g_hash_table_lookup(r->ht, "revision"));
	if (xbps_cmpver(r->repover, (const gchar *)srcpkgver) == -1) {
		fprintf(stdout, "pkgname: %s ", r->pkgname);
		fprintf(stdout, "repover: %s ", r->repover);
		fprintf(stdout, "srcpkgver: %s\n", (const gchar *)srcpkgver);
	}

	g_hash_table_remove_all(r->ht);
	g_match_info_free(r->match_info);
	g_free(data);
	g_free(srcpkgver);
}
