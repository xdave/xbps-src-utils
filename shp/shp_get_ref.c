#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shp_api.h"
#include "str_map.h"

void
shp_get_ref(shp *s, char *dest, char *src)
{
	char c = 0, prev = 0;
	size_t count = 0, reflen = 0, tmplen = 0;
	int ref = 0;
	char var[BUFSIZ] = {'\0'};
	char buf[BUFSIZ] = {'\0'};
	char *tmp = NULL;
	char *ptr = src;
	str_map *tmpvar;
	while ((c = *ptr++) != '\0') {
		switch(c) {
		case '{':
			if (prev == '$') {
				ref = 1;
				count--;
			} else {
				buf[count++] = c;
			}
			break;
		case '}':
			if (ref == 1) {
				var[reflen] = '\0';
				tmpvar = str_map_find(s->env, var);
				if (tmpvar) tmp = tmpvar->value;
				if (!tmp) tmp = getenv(var);
				if (!tmp) {
					reflen = 0;
					ref = 0;
					break;
				}
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
}
