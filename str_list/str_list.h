#ifndef __STR_LIST_H__
#define __STR_LIST_H__

typedef struct str_list_t str_list;

struct str_list_t {
	char *value;
	str_list *front;
	str_list *next;
};

str_list *str_list_create(void);
str_list *str_list_find(str_list *, const char *) __attribute__((pure));
str_list *str_list_add(str_list *, const char *);
void str_list_del(str_list *, const char *);
void str_list_destroy(str_list *);

#endif /*__STR_LIST_H__*/
