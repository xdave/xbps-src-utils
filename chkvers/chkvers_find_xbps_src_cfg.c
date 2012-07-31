#include <stdio.h>
#include <string.h>
#include "chkvers.h"

void
chkvers_find_xbps_src_cfg(chkvers *chk)
{
	FILE *fp;
	const char *cfg;
	const char *locations[] = {
		"/etc/xbps/xbps-src.conf",
		"/usr/local/etc/xbps/xbps-src.conf",
		ETCDIR "/xbps/xbps-src.conf", NULL
	};

	for (;(cfg = (*locations)++);) {
		if ((fp = fopen(cfg, "r")) != NULL) {
			fclose(fp);
			strcpy(chk->xbps_src_cfg, cfg);
			chk->xbps_src_cfg[strlen(cfg)] = '\0';
			break;
		}
	}
}
