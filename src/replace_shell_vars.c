#include <glib.h>
#include "rcv.h"

static GDateTime *utc;

void
replace_shell_vars(gpointer key, gpointer val, gpointer data)
{
	GHashTable *ht = (GHashTable *)data;
	const gchar *varname = (const gchar*)key;
	const gchar *value = (const gchar*)val;
	gchar *version = g_hash_table_lookup(ht, "version");
	if (varname[0] == '_') {
		replace_in_version(ht, varname, value);
	}
	if (g_strcmp0("\"$(date -u +%Y%m%d)\"", version) == 0) {
		utc = g_date_time_new_now_utc();
		replace_in_version(ht, "\"$(date -u +%Y%m%d)\"",
			g_date_time_format(utc, "%Y%m%d"));
		g_date_time_unref(utc);
	}
}
