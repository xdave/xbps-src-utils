#include <glib.h>
#include "rcv.h"

void
replace_shell_vars(gpointer key, gpointer val, gpointer data)
{
	GDateTime *utc = (GDateTime *)data;
	const gchar *varname = (const gchar*)key;
	const gchar *value = (const gchar*)val;
	const gchar *version = g_hash_table_lookup(r->ht, "version");
	const gchar *date = "\"$(date -u +%Y%m%d)\"";
	const gchar *datef = "%Y%m%d";

	if (varname[0] == '_')
		g_hash_str_replace(r->ht, "version", varname, value);

	if (version != NULL && g_strcmp0(version, date) == 0)
		g_hash_str_replace(r->ht, "version", date,
				   g_date_time_format(utc, datef));
}
