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
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "rcv.h"

int
rcv_process_dir(rcv_t *rcv, const char *path, rcv_proc_func process)
{
	DIR *dir;
	struct dirent entry, *result;
	struct stat st;
	char filename[BUFSIZ];
	int i, ret = 0;

	dir = opendir(path);
error:
	if (errno > 0) {
		fprintf(stderr, "Error: while processing dir '%s': %s\n", path,
			strerror(errno));
		exit(1);
	}

	if ((chdir(path)) == -1) goto error;
	while(1) {
		i = readdir_r(dir, &entry, &result);
		if (i > 0) goto error;
		if (result == NULL) break;
		if (strcmp(result->d_name, ".") == 0) continue;
		if (strcmp(result->d_name, "..") == 0) continue;
		if ((lstat(result->d_name, &st)) != 0) goto error;
		if (S_ISLNK(st.st_mode) != 0) continue;
		if ((chdir("..")) == -1) goto error;
		strcpy(filename, "srcpkgs/");
		strcat(filename, result->d_name);
		strcat(filename, "/template");
		ret = process(rcv, filename, rcv_check_version);
		if ((chdir(path)) == -1) goto error;
	}

	if ((closedir(dir)) == -1) goto error;

	return ret;
}
