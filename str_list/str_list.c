#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xmem.h"
#include "str_list.h"

str_list *
str_list_create(void)
{
	str_list *list;
	if ((list = xmalloc(sizeof(str_list))) == NULL)
		return NULL;
	list->front = list;
	list->next = NULL;
	list->value = NULL;
	return list;
}

str_list *
str_list_find(str_list *list, const char *value)
{
	str_list *cur = list->front;
	while (cur) {
		if (cur->value && strcmp(cur->value, value) == 0)
			return cur;
		cur = cur->next;
	}
	return NULL;
}

str_list *
str_list_add(str_list *list, const char *value)
{
	size_t vlen;
	str_list **cur = &(list->front);
	str_list *found;

	if (value == NULL) return NULL;

	vlen = strlen(value);
	found = str_list_find(*cur, value);
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
	if ((*cur)->value) {
		while(*cur)
			cur = &((*cur)->next);
		*cur = str_list_create();
		(*cur)->front = list;
	}
	if (((*cur)->value = xcalloc(vlen + 1, sizeof(char))) == NULL)
		return NULL;
	(*cur)->value = strncpy((*cur)->value, value, vlen);
	(*cur)->value[vlen] = '\0';
	return *cur;
}

void
str_list_del(str_list *list, const char *value)
{
	str_list *found = str_list_find(list, value);
	if (found) {
		xfree(found->value);
		found->value = NULL;
	}
}

void
str_list_destroy(str_list *list)
{
	str_list *tmp, *cur = list->front;
	while(cur) {
		tmp = cur->next;
		if (cur->value) xfree(cur->value);
		xfree(cur);
		cur = tmp;
	}
}
