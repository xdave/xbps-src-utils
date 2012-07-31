#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chkvers.h"


int
chkvers_init(chkvers *chk)
{
	memset(chk, 0, sizeof(chkvers));
	memset(&chk->xhp, 0, sizeof(struct xbps_handle));
	if (xbps_init(&chk->xhp)) {
		fprintf(stderr, "(chkvers_init): Can't initialize libxbps!\n");
		return (EXIT_FAILURE);
	}
	return 0;
}

