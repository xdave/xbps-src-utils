#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "repo-checkvers.h"

char*
get_tmpl(const char* path)
{
	char* tmpl;
	size_t tmpl_len;
	int ret;

	tmpl_len = strlen(path) + strlen("template") + 2;
	tmpl = malloc(sizeof(char)*tmpl_len);
	ret = snprintf(tmpl, tmpl_len, "%s/template", path);

	if (ret < 1) {
		fprintf(stderr, "Error: %s.\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	tmpl[tmpl_len] = '\0';

	return tmpl;
}
