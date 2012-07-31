#ifndef __STR_MAP_H__
#define __STR_MAP_H__

typedef struct str_map_t str_map;

struct str_map_t {
	char *key, *value;
	str_map *front;
	str_map *next;
};

str_map *str_map_create(void);
str_map *str_map_find(str_map *, const char *) __attribute__((pure));
str_map *str_map_add(str_map *, const char *, const char *);
void str_map_del(str_map *, const char *);
void str_map_destroy(str_map *);

#endif /*__STR_MAP_H__*/
