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

#include <dirent.h>
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
	int ret = 0;

	dir = opendir(path);

	(void)chdir(path);
	while(readdir_r(dir, &entry, &result) == 0) {
		if (result == NULL) break;
		if (strcmp(result->d_name, ".") == 0) continue;
		if (strcmp(result->d_name, "..") == 0) continue;
		lstat(result->d_name, &st);
		if (S_ISLNK(st.st_mode) != 0) continue;
		(void)chdir("..");
		strcpy(filename, "srcpkgs/");
		strcat(filename, result->d_name);
		strcat(filename, "/template");
		ret = process(rcv, filename, rcv_check_version);
		(void)chdir(path);
	}

	closedir(dir);

	return ret;
}
