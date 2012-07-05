#include "rcv.h"

rcv_t *
rcv_init()
{
	return g_malloc0(sizeof(rcv_t));
}
