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
	const char *version, *revision;
	char errbuf[BUFSIZ] = {'\0'};
	char *file, *dname, *bname;
	char *srcpkgs = str_map_find(chk->env, "XBPS_SRCPKGDIR")->value;
	size_t i, slen;

error:
	if (errno != 0) {
		strerror_r(errno, errbuf, BUFSIZ);
		printf("Error: (%s) (%s) %s\n", srcpkgs, file, errbuf);
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
		memset(chk->pkgname, 0, BUFSIZ);
		strncpy(chk->pkgname, dname, strlen(dname) - strlen(bname));
		if (chdir(srcpkgs) != 0) goto error;
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
		snprintf(chk->srcpkgver, slen, "%s_%s", version, revision);
		chk->pkgd = find_pkg(&chk->xhp, chk->pkgname);
		errno = 0;
		if (chk->pkgd == NULL) {
			chk->repover = "(NULL)";
		} else {
			dict_get(chk->pkgd, "version", &chk->repover);
			if (xbps_cmpver(chk->repover, chk->srcpkgver) > -1)
				continue;
		}
		printf("pkgname: %s repover: %s srcpkgver: %s [MANUAL]\n",
			chk->pkgname, chk->repover, chk->srcpkgver);
	}
}
