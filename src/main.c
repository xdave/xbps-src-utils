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

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rcv.h"

int
main(int argc, char **argv)
{
	int i, c;
	rcv_t rcv;
	const char *prog = argv[0], *sopts = "hc:C:d:s", *tmpl;
	const struct option lopts[] = {
		{ "help", no_argument, NULL, 'h' },
		{ "xbps-src-conf", required_argument, NULL, 'c' },
		{ "xbps-conf", required_argument, NULL, 'C' },
		{ "distdir", required_argument, NULL, 'd' },
		{ "show-missing", no_argument, NULL, 's' },
		{ NULL, 0, NULL, 0 }
	};

	rcv.xsrc_conf = rcv.xbps_conf = rcv.distdir = rcv.pkgdir = NULL;
	rcv.show_missing = false;

	rcv_find_conf(&rcv);

	while ((c = getopt_long(argc, argv, sopts, lopts, NULL)) != -1) {
		switch (c) {
		case 'h':
			return rcv_show_usage(prog);
		case 'c':
			free(rcv.xsrc_conf);
			rcv.xsrc_conf = strdup(optarg);
			break;
		case 'C':
			free(rcv.xbps_conf);
			rcv.xbps_conf = strdup(optarg);
			break;
		case 'd':
			free(rcv.distdir); rcv.distdir = NULL;
			free(rcv.pkgdir); rcv.pkgdir = NULL;
			rcv_set_distdir(&rcv, optarg);
			break;
		case 's':
			rcv.show_missing = true;
			break;
		default:
			return rcv_show_usage(prog);
		}
	}

	argc -= optind;
	argv += optind;

	rcv_init(&rcv, prog);
	rcv.manual = false;
	rcv_process_dir(&rcv, rcv.pkgdir, rcv_process_file);
	rcv.manual = true;
	if (argc > 0) {
		for(i = 0; i < argc; i++) {
			tmpl = argv[i] + (strlen(argv[i]) - strlen("template"));
			if ((strcmp("template", tmpl)) == 0) {
				rcv_process_file(&rcv, argv[i],
					rcv_check_version);
			}
		}
	}
	rcv_end(&rcv);
	
	return 0;
}
