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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xmem.h"
#include "str_map.h"

str_map *
str_map_create(void)
{
	str_map *map;
	if ((map = xmalloc(sizeof(str_map))) == NULL)
		return NULL;
	map->front = map;
	map->next = NULL;
	map->key = NULL;
	map->value = NULL;
	return map;
}

str_map *
str_map_find(str_map *map, const char *key)
{
	str_map *cur = map->front;
	while (cur) {
		if (cur->key && strcmp(cur->key, key) == 0)
			return cur;
		cur = cur->next;
	}
	return NULL;
}

str_map *
str_map_add(str_map *map, const char *key, const char *value)
{
	size_t klen, vlen;
	str_map **cur = &(map->front);
	str_map *found;

	if (value == NULL) return NULL;

	klen = strlen(key);
	vlen = strlen(value);
	found = str_map_find(*cur, key);
	if (found) {
		if (found->value && strcmp(found->value, value) == 0)
			return found;
		if (found->value) {
			found->value = memset(found->value, '\0',
				(strlen(found->value) + 1) * sizeof(char));
			xfree(found->value);
		}
		if ((found->value = xcalloc(vlen + 1, sizeof(char))) == NULL)
			return NULL;
		found->value = strncpy(found->value, value, vlen);
		found->value[vlen] = '\0';
		return found;
	}
	if ((*cur)->key && (*cur)->value) {
		while(*cur)
			cur = &((*cur)->next);
		*cur = str_map_create();
		(*cur)->front = map;
	}
	if (((*cur)->key = xcalloc(klen + 1, sizeof(char))) == NULL)
		return NULL;
	if (((*cur)->value = xcalloc(vlen + 1, sizeof(char))) == NULL)
		return NULL;
	(*cur)->key = strncpy((*cur)->key, key, klen);
	(*cur)->value = strncpy((*cur)->value, value, vlen);
	(*cur)->key[klen] = '\0';
	(*cur)->value[vlen] = '\0';
	return *cur;
}

void
str_map_del(str_map *map, const char *key)
{
	str_map *found = str_map_find(map, key);
	if (found) {
		found->key = memset(found->key, '\0',
			(strlen(found->key) + 1) * sizeof(char));
		found->value = memset(found->value, '\0',
			(strlen(found->value) + 1) * sizeof(char));
		xfree(found->key);
		xfree(found->value);
		found->key = NULL;
		found->value = NULL;
	}
}

void
str_map_destroy(str_map *map)
{
	str_map *tmp, *cur = map->front;
	while(cur) {
		tmp = cur->next;
		if (cur->key) xfree(cur->key);
		if (cur->value) xfree(cur->value);
		xfree(cur);
		cur = tmp;
	}
}
