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
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "xgetopt.h"
#include "chkvers.h"
#include "shp_api.h"
#include "shp_parser.h"
#include "str_map.h"

int
chkvers_getopt(chkvers *chk, int *argc, char ***argv)
{
	shp s;
	xgetopt x;
	int optc;
	char *distdirp;
	char srcpkgdir[BUFSIZ] = {'\0'};
	char distdir[PATH_MAX] = {'\0'};

	xopt optv[] = {
		{ "-h", "--help", NULL,
			"Show this helpful help-message for help." },
		{ "-c", NULL, "FILENAME",
			"Set (or override) the `xbps-src.conf' (which may have "
			"automatically been detected)." },
		{ "-C", NULL, "FILENAME",
			"Set (or override) the `xbps.conf' (which may have "
			"automatically been detected)." },
		{ "-d", "--distdir", "DIRECTORY",
			"Set (or override) the XBPS_DISTDIR setting (which may "
			"have been set in your `xbps-src.conf' file)." },
		{ "-s", "--show-missing", NULL,
			"List any binary packages which are not built." },
		{ "-x", "--xtrace", NULL,
			"Show shell xtrace info (works almost exactly the "
			"same as `/bin/sh -x')." },
		{ "-D", "--debug", NULL,
			"Show internal bison parser stack debug information" },
		{ NULL, NULL, "[FILES...]", "Extra packages to process with "
			"the outdated ones (only processed if missing)." }
	};

	distdirp = NULL;
	optc = sizeof(optv)/sizeof(*optv);
	memset(&x, 0, sizeof(xgetopt));
	x.usage_text = "[FILES...]\n\nCopyright (c) 2012 The AUTHORS. "
		"See the AUTHORS file.\n"
		"See the COPYING file for license(s)/distribution details.";

	chkvers_find_xbps_src_cfg(chk);

	while (xgetopts(&x, optc, optv, *argc, *argv) != -1) {
		if (x.opt == &optv[0]) {
			chkvers_end(chk);
			xopt_usage(&x, optc, optv);
		}
		if (x.opt == &optv[1]) {
			strcpy(chk->xbps_src_cfg, x.optarg);
			chk->xbps_src_cfg[strlen(x.optarg)] = '\0';
		}
		if (x.opt == &optv[2]) { /* TODO */ }
		if (x.opt == &optv[3])
			distdirp = realpath(x.optarg, distdir);
		if (x.opt == &optv[4])
			chk->show_missing = 1;
		if (x.opt == &optv[5])
			chk->xtrace = 1;
		if (x.opt == &optv[6])
			chk->debug = 1;
	}

	shp_init(&s, chk->xbps_src_cfg, NULL);
	shpdebug = chk->debug;
	s.print_xtrace = chk->xtrace;
	s.env = chk->env;
	shp_process(&s, NULL);
	shp_end(&s);

	if (distdir[0] != '\0')
		str_map_add(chk->env, "XBPS_DISTDIR", distdirp);

	strcat(srcpkgdir, str_map_find(chk->env, "XBPS_DISTDIR")->value);
	strcat(srcpkgdir, "/srcpkgs");
	str_map_add(chk->env, "XBPS_SRCPKGDIR", srcpkgdir);

	*argc -= optind;
	*argv += optind;

	return (EXIT_SUCCESS);
}
