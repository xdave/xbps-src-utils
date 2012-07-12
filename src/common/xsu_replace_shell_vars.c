#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include "xsu_api.h"

void
xsu_replace_shell_vars(gpointer key, gpointer val, gpointer data)
{
	const char *varname, *value, *version, *date, *datef;
	GDateTime *utc;

	varname = (const char*)key;
	value = (const char*)val;
	version = g_hash_table_lookup(checkvers->ht, "version");
	date = "\"$(date -u +%Y%m%d)\"";
	datef = "%Y%m%d";
	utc = (GDateTime *)data;

	if (varname[0] == '_')
		xsu_hash_str_replace(checkvers->ht, "version", varname, value);

	if (version == NULL) {
		fprintf(stderr, "%s:%d: Why the fuck is 'version' NULL?\n",
				__FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}

	if (g_strcmp0(version, date) == 0)
		xsu_hash_str_replace(checkvers->ht, "version", date,
				     g_date_time_format(utc, datef));
}
