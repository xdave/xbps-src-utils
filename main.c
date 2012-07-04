#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "repo-checkvers.h"

int
main(int argc, char** argv)
{
	DIR* dir_p;
	struct dirent *dir_ent;
	struct stat dir_stat;
	char *path;
	size_t path_len;
	int ret;
	const char *dir_name;
	const char* srcpkgs;

	if (argc < 2) {
		errno = EINVAL;
		DIE("Must pass srcpkgdir.\nExample:\n\t%s /foo\n", argv[0]);
	}

	srcpkgs = argv[1];

	dir_p = opendir(srcpkgs);
	if (dir_p == NULL) {
		DIE("Can't open '%s'.", srcpkgs);
	}

	while ((dir_ent = readdir(dir_p)) != NULL) {
		dir_name = dir_ent->d_name;

		if (strcmp(".", dir_name) == 0 || strcmp("..", dir_name) == 0)
			continue;

		path_len = strlen(dir_name) + strlen(srcpkgs) + 2;
		if ((path = malloc(sizeof(char) * path_len)) == NULL) {
			DIE("%s", "Can't allocate memory for a string.");
		}

		ret = snprintf(path, path_len, "%s/%s", srcpkgs, dir_name);
		if (ret < 1) {
			DIE("%s", "Output of snprintf() was truncated.");
		}

		lstat(path, &dir_stat);
		free(path);

		if (S_ISLNK(dir_stat.st_mode) == 0)
			fprintf(stdout, "pkgname: %s\n", dir_name);
	}

	closedir(dir_p);

	return EXIT_SUCCESS;
}
