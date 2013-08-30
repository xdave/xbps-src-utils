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

char *
rcv_cmd(rcv_t *rcv, const char *s, size_t len)
{
	int c, rv = 0;
	FILE *stream;
	size_t i = 0, j = 0, k = 0, count = len*3;
	char *cmd = calloc(count, sizeof(char));
	char *buf = calloc(count, sizeof(char));
	(void)rcv;
	while (i < len) {
		if (s[i] == '$' && s[i+1] != '{') {
			j = 0;
			i++;
			if (s[i] == '(') { i++; }
			while (s[i] != ')') { cmd[j++] = s[i++]; }
			if (s[i] == ')') { i++; }
			cmd[j++] = '\0';
			if ((stream = popen(cmd, "r")) == NULL)
				goto error;
			while ((c = fgetc(stream)) != EOF && c != '\n') {
				buf[k++] = (char)c;
			}
			rv = pclose(stream);
error:
			if (rv > 0 || errno > 0) {
				fprintf(stderr,
					"Shell cmd failed: '%s' for "
					"template '%s'",
					cmd, rcv->fname);
				if (errno > 0) {
					fprintf(stderr, ": %s\n",
						strerror(errno));
				}
				putchar('\n');
				exit(1);
			}

		} else {
			if (s[i] != '\n')
				buf[k++] = s[i++];
		}
	}
	buf[k] = '\0';
	free(cmd);
	free((char *)s);
	return buf;


}
