#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
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
	const char *dir_name;
	const char* srcpkgs;

	if (argc < 2) {
		fprintf(stderr, "Error: Must pass srcpkgdir.\n");
		exit(EXIT_FAILURE);
	}

	srcpkgs = argv[1];

	dir_p = opendir(srcpkgs);

	if (dir_p == NULL) {
		fprintf(stderr, "Error: Can't open '%s.\n", srcpkgs);
		exit(EXIT_FAILURE);
	}

	while ((dir_ent = readdir(dir_p)) != NULL) {
		dir_name = dir_ent->d_name;
	
		if (strcmp(".", dir_name) == 0 || strcmp("..", dir_name) == 0)
			continue;

		path = get_pkg_dir(dir_name, srcpkgs);
		lstat(path, &dir_stat);
		free(path);

		if (S_ISLNK(dir_stat.st_mode) == 0)
			fprintf(stdout, "pkgname: %s\n", dir_name);
	}

	closedir(dir_p);

	return EXIT_SUCCESS;
}
