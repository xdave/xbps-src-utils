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
#include "rcv.h"

int
rcv_show_usage(const char *prog)
{

fprintf(stderr,
"Usage: %s [OPTIONS] "
"[FILES...]\n\nCopyright (c) 2012-2013 The AUTHORS. "
"See the AUTHORS file.\n"
"See the COPYING file for license(s)/distribution details.\n\n"
" Options:\n"
"  -h,--help			Show this helpful help-message for help.\n"
"  -c,--xbps-src-conf=FILENAME	Set (or override) the `xbps-src.conf' (which\n"
"				may have automatically been detected).\n"
"  -C,--xbps-conf=FILENAME	Set (or override) the `xbps.conf' (which may\n"
"				have automatically been detected).\n"
"  -d,--distdir=DIRECTORY	Set (or override) the XBPS_DISTDIR setting\n"
"				(which may have been set in your\n"
"				`xbps-src.conf' file).\n"
"  -s,--show-missing		List any binary packages which are not built.\n"
"\n  [FILES...]			Extra packages to process with the outdated\n"
"				ones (only processed if missing).\n\n",
prog);
	return EXIT_FAILURE;
}

