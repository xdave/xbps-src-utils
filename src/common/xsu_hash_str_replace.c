#include <glib.h>
#include "xsu_api.h"
	
void
xsu_hash_str_replace(GHashTable *ht, const gchar *key,
		     const gchar *pat, const gchar *repl)
{
	const gchar *value;
	gchar *key_copy, *new_value;		/* GHashTable g_free()s these */

	key_copy = g_strdup(key);
	value = g_hash_table_lookup(ht, key_copy);
	new_value = xsu_str_replace(value, pat, repl);
	g_hash_table_replace(ht, key_copy, new_value);
}
