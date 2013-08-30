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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rcv.h"

map_item_t
map_new_item(void)
{
	return (map_item_t){ .k = { NULL, 0 }, .v = { NULL, 0, 0 } };
}

map_t *
map_create(void)
{
	size_t i = 0;
	map_t *map = malloc(sizeof(map_t));
	map->size = 16;
	map->len = 0;
	map->items = calloc(map->size, sizeof(map_item_t));
	for (; i < map->size; i++) {
		map->items[i] = map_new_item();
	}
	return map;
}

static map_item_t
map_find_n(map_t *map, const char *k, size_t n)
{
	size_t i = 0;
	map_item_t item = map_new_item();

	if (map->len == 0)
		return item;

	while(i < map->len) {
		item = map->items[i++];
		if (item.k.len != 0)
			if ((strncmp(k, item.k.s, n) == 0))
				break;
	}
	return item;
}

map_item_t
map_add_n(map_t *map, const char *k, size_t kn, const char *v, size_t vn)
{
	size_t i;
	map_item_t item;
	if (++map->len > map->size) {
		map->size += 16;
		map->items = realloc(map->items,
			sizeof(map_item_t)*(map->size));
		for (i = map->size - 10; i < map->size; i++) {
			map->items[i] = map_new_item();
		}
	}
	item = map_find_n(map, k, kn);
	if (item.k.len == 0) {
		item = map_new_item();
		item.k = (string){ (char *)k, kn };
		item.i = map->len - 1;
	}
	if (item.v.vmalloc == 1)
		free(item.v.s);
	item.v = (string){ (char *)v, vn };
	map->items[item.i] = item;
	return map->items[item.i];
}

map_item_t
map_add(map_t *map, const char *k, const char *v)
{
	return map_add_n(map, k, strlen(k), v, strlen(v));
}


map_item_t
map_find(map_t *map, const char *k)
{
	return map_find_n(map, k, strlen(k));
}

void
map_destroy(map_t *map)
{
	size_t i = 0;
	while (i < map->len) {
		if (map->items[i].v.vmalloc == 1) {
			if (map->items[i].v.s != NULL) {
				free(map->items[i].v.s);
			}
		}
		i++;
	}
	free(map->items);
	free(map);
}
