#include <glib.h>
#include "rcv.h"
	
void
replace_in_version(GHashTable *ht, const gchar *pat, const gchar *repl)
{
	gchar *version_key =  g_strdup("version");
	gchar *version = g_hash_table_lookup(ht, version_key);
	gchar *new_version = NULL;
	new_version = str_replace(version, pat, repl);
	g_hash_table_replace(ht, version_key, new_version);
}
