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

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rcv.h"

int
rcv_check_version(rcv_t *rcv)
{
	map_item_t pkgname, version, revision;
	const char *repover = NULL;
	char _srcver[BUFSIZ] = { '\0' };
	char *srcver = _srcver;

	if (rcv->have_vars < 3) {
		printf("Error in '%s': missing '%s', '%s', or '%s' vars!\n",
			rcv->fname, "pkgname", "version", "revision");
		exit(EXIT_FAILURE);
	}

	pkgname = map_find(rcv->env, "pkgname");
	version = map_find(rcv->env, "version");
	revision = map_find(rcv->env, "revision");

	srcver = strncpy(srcver, pkgname.v.s, pkgname.v.len);
	rcv->pkgd = xbps_rpool_get_pkg(&rcv->xhp, srcver);
	srcver = strncat(srcver, "-", 1);
	srcver = strncat(srcver, version.v.s, version.v.len);
	srcver = strncat(srcver, "_", 1);
	srcver = strncat(srcver, revision.v.s, revision.v.len);
	xbps_dictionary_get_cstring_nocopy(rcv->pkgd, "pkgver", &repover);
	if (repover == NULL && rcv->show_missing == true) {
		printf("pkgname: %.*s repover: ? srcpkgver: %s\n",
				pkgname.v.len, pkgname.v.s, srcver + pkgname.v.len  + 1);
	}
	if (repover != NULL && rcv->show_missing == false) {
		if (xbps_cmpver(repover + pkgname.v.len + 1, srcver + pkgname.v.len  + 1) < 0) {
			printf("pkgname: %.*s repover: %s srcpkgver: %s\n",
				pkgname.v.len, pkgname.v.s, repover + pkgname.v.len  + 1, srcver + pkgname.v.len  + 1);
		}
	}
	return 0;
}
