#include <stdlib.h>
#include "chkvers.h"
#include "xmem.h"

void
chkvers_free(chkvers *chk)
{
	chkvers_end(chk);
	xfree(chk);
	chk = NULL;
}
