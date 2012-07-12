#include <stdbool.h>
#include <string.h>
#include "xsu_api.h"

bool
xsu_hash_str_cmp(void *a, void *b)
{
	return (strncmp(a, b, strlen(a)) == 0);
}
