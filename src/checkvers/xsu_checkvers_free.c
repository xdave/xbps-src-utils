#include <glib.h>
#include "xsu_api.h"
	
void
xsu_checkvers_free(void)
{
	if (checkvers->err != NULL) g_error_free(checkvers->err);
	g_regex_unref(checkvers->regex);
	g_hash_table_destroy(checkvers->ht);
	g_date_time_unref(checkvers->utc);
	g_free(checkvers);
}
