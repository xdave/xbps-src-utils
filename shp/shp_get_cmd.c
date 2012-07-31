#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shp_api.h"

int
shp_get_cmd(shp *s, char *dest, const char *src)
{
	FILE *stream;
	size_t read;
	int rv = 0;
	char c = 0, prev = 0;
	size_t count = 0, reflen = 0, tmplen = 0;
	int ref = 0;
	char var[BUFSIZ] = {'\0'};
	char buf[BUFSIZ] = {'\0'};
	char tmp[BUFSIZ] = {'\0'};
	const char *ptr = src;
	while ((c = *ptr++) != '\0') {
		switch(c) {
		case '(':
			if (prev == '$') {
				ref = 1;
				count--;
			} else {
				buf[count++] = c;
			}
			break;
		case ')':
			if (ref == 1) {
				var[reflen] = '\0';
				fflush(stderr);
				shp_xtrace(s, "+ %s\n", var);
				stream = popen(var, "r");
				read = fread(&tmp[0], sizeof(char), BUFSIZ,
					stream);
				rv = pclose(stream);

				if (errno != 0||rv != 0|| read == 0) {
					return (EXIT_FAILURE);
				}
				if (tmp[read-1] == '\n') tmp[read-1] = '\0';
				tmplen = strlen(tmp);
				strncpy(&buf[count], tmp, tmplen);
				count += tmplen;
				reflen = 0;
				ref = 0;
			} else {
				buf[count++] = c;
			}
			break;
		default:
			if (ref == 1) {
				var[reflen++] = c;
			} else {
				buf[count++] = c;
			}
			break;
		}
		prev = c;
	}
	buf[count] = '\0';
	strncpy(dest, buf, count + 1);
	return (EXIT_SUCCESS);
}
