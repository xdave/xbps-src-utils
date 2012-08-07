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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chkvers.h"
#include "shp_api.h"
#include "shp_parser.h"
#include "str_map.h"

#define PFMT "pkgname: %s repover: %s srcpkgver: %s\n"

void
chkvers_process_dir(chkvers *chk, const char *directory, const char **accept)
{
	shp s;
	const char *version, *revision;
	char errbuf[BUFSIZ] = {'\0'};
	size_t slen;

	chk->dirpath = directory;
	chk->dir = opendir(chk->dirpath);

error:
	if (errno != 0) {
		strerror_r(errno, errbuf, BUFSIZ);
		printf("Error: (%s) (%s) %s\n",
			chk->dirpath, chk->pkgname, errbuf);
		str_map_destroy(chk->env);
		chkvers_end(chk);
		exit(EXIT_FAILURE);
	}

	while(readdir_r(chk->dir, &chk->entry, &chk->result) == 0) {
		if (chk->result == NULL) break;
		if (strcmp(chk->result->d_name, ".") == 0) continue;
		if (strcmp(chk->result->d_name, "..") == 0) continue;
		strcpy(chk->pkgname, chk->result->d_name);
		if (chdir(chk->dirpath) != 0) goto error;
		if (lstat(chk->pkgname, &chk->st) != 0) goto error;
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
			if (chk->show_missing == 1)
				printf(PFMT, chk->pkgname, "?", chk->srcpkgver);
			continue;
		}
		dict_get(chk->pkgd, "version", &chk->repover);
		if (xbps_cmpver(chk->repover, chk->srcpkgver) > -1) continue;
		printf(PFMT, chk->pkgname, chk->repover, chk->srcpkgver);
	}

	closedir(chk->dir);
}
