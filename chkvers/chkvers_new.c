#include <stdlib.h>
#include "chkvers.h"
#include "xmem.h"

chkvers *
chkvers_new(void)
{
	chkvers *chk = xmalloc(sizeof(chkvers));
	chkvers_init(chk);
	return chk;
}
