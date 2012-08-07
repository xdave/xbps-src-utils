/*-
 * Copyright (c) 2012 Dave Elusive <davehome@redthumb.info.tm>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chkvers.h"
#include "shp_api.h"
#include "shp_parser.h"
#include "str_map.h"

void
chkvers_process_files(chkvers *chk, char **files, size_t fc,
		const char **accept)
{
	shp s;
	str_map *processed;
	const char *version, *revision;
	char errbuf[BUFSIZ] = {'\0'};
	char *file, *dname, *bname;
	char *srcpkgs = str_map_find(chk->env, "XBPS_SRCPKGDIR")->value;
	size_t i, slen;
	processed = str_map_create();

error:
	if (errno != 0) {
		strerror_r(errno, errbuf, BUFSIZ);
		printf("Error: (%s) (%s) %s\n", srcpkgs, file, errbuf);
		str_map_destroy(processed);
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < fc; i++) {
		if (chk->show_missing == 1) break;
		file = files[i];
		if (strcmp(file, ".") == 0) continue;
		if (strcmp(file, "..") == 0) continue;
		if (strncmp(file, "common/", 7) == 0) continue;
		if (strncmp(file, "srcpkgs/", 8) != 0) continue;
		dname = file + 8;
		bname = strstr(dname, "/");
		if (!bname) continue;
		memset(chk->pkgname, 0, BUFSIZ);
		strncpy(chk->pkgname, dname, strlen(dname) - strlen(bname));
		if (str_map_find(processed, chk->pkgname)) continue;
		if (chdir(srcpkgs) != 0) goto error;
		/* Instead of erroring out here, assume deleted pkg */
		if (lstat(chk->pkgname, &chk->st) != 0) continue;
		if (S_ISLNK(chk->st.st_mode) != 0) continue;
		if (chdir(chk->pkgname) != 0) goto error;
		shp_init(&s, "template", accept);
		shpdebug = chk->debug;
		s.print_xtrace = chk->xtrace;
		s.env = chk->env;
		shp_process(&s, NULL);
		shp_end(&s);
		version = str_map_find(chk->env, "version")->value;
		revision = str_map_find(chk->env, "revision")->value;
		slen = strlen(version) + strlen(revision) + 2;
		memset(chk->srcpkgver, '\0', BUFSIZ);
		snprintf(chk->srcpkgver, slen, "%s_%s", version, revision);
		chk->pkgd = find_pkg(&chk->xhp, chk->pkgname);
		errno = 0;
		if (chk->pkgd == NULL) {
			chk->repover = "(NULL)";
		} else {
			dict_get(chk->pkgd, "version", &chk->repover);
			if (xbps_cmpver(chk->repover, chk->srcpkgver) > -1) {
				str_map_add(processed, chk->pkgname, "");
				continue;
			}
		}
		printf("pkgname: %s repover: %s srcpkgver: %s [MANUAL]\n",
			chk->pkgname, chk->repover, chk->srcpkgver);
		str_map_add(processed, chk->pkgname, "");
	}
	str_map_destroy(processed);
}
