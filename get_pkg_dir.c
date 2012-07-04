#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "repo-checkvers.h"

char*
get_pkg_dir(const char* dir_name, const char* srcpkgs)
{
	size_t srcpkgdir_len;
	size_t dir_len;
	size_t path_len;
	char* path;
	int ret;

	srcpkgdir_len = strlen(srcpkgs);
	dir_len = strlen(dir_name);
	path_len = srcpkgdir_len+dir_len+2;
	path = malloc(sizeof(char)*(path_len));
	ret = snprintf(path, path_len, "%s/%s", srcpkgs, dir_name);

	if (ret < 1)
		fprintf(stderr, "Error: %s.\n", strerror(errno));

	return path;
}
