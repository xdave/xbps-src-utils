#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "rcv.h"

char *
rcv_refs(rcv_t *rcv, const char *s, size_t len)
{
	map_item_t item = map_new_item();
	size_t i = 0, j = 0, k = 0, count = len*3;
	char *ref = calloc(count, sizeof(char));
	char *buf = calloc(count, sizeof(char));
	while (i < len) {
		if (s[i] == '$' && s[i+1] != '(') {
			j = 0;
			i++;
			if (s[i] == '{') { i++; }
			while (isalpha(s[i]) || s[i] == '_') {
				ref[j++] = s[i++];
			}
			if (s[i] == '}') { i++; }
			ref[j++] = '\0';
			item = map_find(rcv->env, ref);
			if ((strncmp(ref, item.k.s, strlen(ref)) == 0)) {
				buf = strcat(buf, item.v.s);
				k += item.v.len;
			} else {
				buf = strcat(buf, "NULL");
				k += 4;
			}
		} else {
			if (s[i] != '\n')
				buf[k++] = s[i++];
		}
	}
	buf[k] = '\0';
	free(ref);
	return buf;
}
