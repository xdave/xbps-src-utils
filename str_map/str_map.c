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
		if (found->value)
			xfree(found->value);
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
