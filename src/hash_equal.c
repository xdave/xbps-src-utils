#include <glib.h>
#include "rcv.h"

gboolean
hash_equal(gconstpointer a, gconstpointer b)
{
	return (g_strcmp0(a, b) == 0);
}
