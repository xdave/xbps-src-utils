#include <glib.h>
#include <glib/gprintf.h>
#include <stdlib.h>
#include <string.h>
#include <xbps_api.h>
#include "xsu_api.h"

void
xsu_checkvers_parse_tmpl(struct xbps_handle *xh, const char *file)
{
	char *srcpkgver, *data;
	size_t len;

	const char *replacements[] = { "$", "{", "}", "\"", "'" };

	checkvers->pkgd = xbps_rpool_find_pkg(xh, checkvers->pkgname, false, true);
	if (checkvers->pkgd == NULL) {
		/*fprintf(stderr, "WARNING: Missing a binpkg for '%s'!\n",
			checkvers->pkgname);*/
		return;
	}
	prop_dictionary_get_cstring_nocopy(checkvers->pkgd, "version", &checkvers->repover);

	g_file_get_contents(file, &data, &len, &checkvers->err);
	xsu_gerror_check(checkvers->err);

	g_regex_match_full(checkvers->regex, data, -1, 0, 0, &checkvers->match_info, &checkvers->err);
	while (g_match_info_matches(checkvers->match_info)) {
		gchar *word = g_match_info_fetch(checkvers->match_info, 0);
		gchar **set = g_strsplit(word, "=", 2);
		g_hash_table_insert(checkvers->ht, g_strdup(set[0]), g_strdup(set[1]));
		g_free(word);
		g_strfreev(set);
		g_match_info_next(checkvers->match_info, &checkvers->err);
	}
	xsu_gerror_check(checkvers->err);

	g_hash_table_foreach(checkvers->ht, xsu_replace_shell_vars, checkvers->utc);

	{
		size_t i = 0;
		for (;i < G_N_ELEMENTS(replacements); i++)
			xsu_hash_str_replace(checkvers->ht, "version",
					   replacements[i], "");
	}

	srcpkgver = g_strdup_printf("%s_%s",
		(const char*)g_hash_table_lookup(checkvers->ht, "version"),
		(const char*)g_hash_table_lookup(checkvers->ht, "revision"));
	if (xbps_cmpver(checkvers->repover, (const char *)srcpkgver) == -1) {
		fprintf(stdout, "pkgname: %s ", checkvers->pkgname);
		fprintf(stdout, "repover: %s ", checkvers->repover);
		fprintf(stdout, "srcpkgver: %s\n", (const char *)srcpkgver);
	}

	g_hash_table_remove_all(checkvers->ht);
	g_match_info_free(checkvers->match_info);
	g_free(data);
	g_free(srcpkgver);
}
