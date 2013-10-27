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

int
rcv_process_file(rcv_t *rcv, const char *fname, rcv_check_func check)
{

	rcv->env = map_create();
	rcv->have_vars = 0;

	if (!rcv_load_file(rcv, fname)) {
		map_destroy(rcv->env);
		rcv->env = NULL;
		return EXIT_FAILURE;
	}

	map_add(rcv->env, "HOME", getenv("HOME"));

	while ((rcv_get_pkgver(rcv)) > 0);

	check(rcv);

	map_destroy(rcv->env);
	rcv->env = NULL;
	
	return EXIT_SUCCESS;
}
