/*
 * Copyright (c) 2012-2013 Dave Elusive <davehome@redthumb.info.tm>
 * All rights reserved
 *
 * You may redistribute this file and/or modify it under the terms of the GNU
 * General Public License version 2 as published by the Free Software
 * Foundation. For the terms of this license, see 
 * <http://www.gnu.org/licenses/>.
 *
 * You are free to use this file under the terms of the GNU General Public
 * License, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rcv.h"

void
rcv_find_conf(rcv_t *rcv)
{
	FILE *fp;
	rcv_t c;

	const char **lp, *conf, *xsrc_locs[] = {
		ETCDIR "/xbps/xbps-src.conf",
		"/etc/xbps/xbps-src.conf",
		"/usr/local/etc/xbps/xbps-src.conf", NULL
	};

	const char *xbps_locs[] = {
		ETCDIR "/xbps/xbps.conf",
		"/etc/xbps/xbps.conf",
		"/usr/local/etc/xbps/xbps.conf", NULL
	};

	for (lp = xsrc_locs; (conf = *lp++) && !rcv->xsrc_conf;) {
		if ((fp = fopen(conf, "r")) != NULL) {
			fclose(fp);
			rcv->xsrc_conf = calloc(strlen(conf) + 1, sizeof(char));
			rcv->xsrc_conf = strcpy(rcv->xsrc_conf, conf);
			rcv->xsrc_conf[strlen(conf)] = '\0';
			break;
		}
	}

	for (lp = xbps_locs; (conf = *lp++) && !rcv->xbps_conf;) {
		if ((fp = fopen(conf, "r")) != NULL) {
			fclose(fp);
			rcv->xbps_conf = calloc(strlen(conf) + 1, sizeof(char));
			rcv->xbps_conf = strcpy(rcv->xbps_conf, conf);
			rcv->xbps_conf[strlen(conf)] = '\0';
			break;
		}
	}

	rcv_init(&c, rcv->prog);
	c.xsrc_conf = c.xbps_conf = c.distdir = c.pkgdir = NULL;
	rcv_process_file(&c, rcv->xsrc_conf, rcv_parse_config);
	rcv->distdir = strdup(c.distdir);
	rcv->pkgdir = strdup(c.distdir);
	rcv->pkgdir = realloc(rcv->pkgdir,
			sizeof(char)*(strlen(c.distdir)+strlen("/srcpkgs")+1));
	rcv->pkgdir = strcat(rcv->pkgdir, "/srcpkgs");
	rcv_end(&c);

	/* TODO: parse xbps.conf */
}
