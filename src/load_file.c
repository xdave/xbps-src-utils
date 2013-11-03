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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rcv.h"

bool
rcv_load_file(rcv_t *rcv, const char *fname)
{
	FILE *file;
	rcv->fname = fname;

	if ((file = fopen(rcv->fname, "r")) == NULL) {
		if (!rcv->manual) {
			fprintf(stderr, "FileError: can't open '%s': %s\n",
				rcv->fname, strerror(errno));
		}
		return false;
	}

	fseek(file, 0, SEEK_END);
	rcv->len = (size_t)ftell(file);
	fseek(file, 0, SEEK_SET);

	if (rcv->input != NULL)
		free(rcv->input);

	if ((rcv->input = calloc(rcv->len + 1, sizeof(char))) == NULL) {
		fprintf(stderr, "MemError: can't allocate memory: %s\n",
			strerror(errno));
		fclose(file);
		return false;
	}

	(void)fread(rcv->input, sizeof(char), rcv->len, file);
	rcv->input[rcv->len] = '\0';
	fclose(file);
	rcv->ptr = rcv->input;

	return true;
}
