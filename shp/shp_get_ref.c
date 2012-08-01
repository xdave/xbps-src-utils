/*-
 * Copyright (c) 2012 Dave Elusive <davehome@redthumb.info.tm>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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
