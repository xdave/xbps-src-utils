#include "chkvers.h"
	
void
chkvers_end(chkvers *chk)
{
	xbps_end(&chk->xhp);
}
