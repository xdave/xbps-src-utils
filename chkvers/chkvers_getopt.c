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
	int rv, optc;
	char *distdirp;
	char srcpkgdir[BUFSIZ] = {'\0'};
	char distdir[PATH_MAX] = {'\0'};

	xopt optv[] = {
		{ "-h", "--help", NULL, "Show this message" },
		{ "-c", NULL, "FILENAME", "Override detected xbps-src.conf" },
		{ "-C", NULL, "FILENAME", "Override detected xbps.conf" },
		{ "-d", "--distdir", "DIRECTORY", "Override detected DISTDIR" },
		{ "-s", "--show-missing", NULL, "Show missing binary pkgs" },
		{ "-x", "--xtrace", NULL, "Show shell xtrace info" },
		{ "-D", "--debug", NULL, "Show parser debug info" }
	};

	distdirp = NULL;
	optc = sizeof(optv)/sizeof(*optv);
	memset(&x, 0, sizeof(xgetopt));
	x.usage_text = "[FILES...]";

	chkvers_find_xbps_src_cfg(chk);

	while ((rv = xgetopts(&x, optc, optv, *argc, *argv)) != -1) {
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
			distdirp = realpath(optarg, distdir);
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
