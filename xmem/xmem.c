#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "xmem.h"

void *
xmalloc(size_t size)
{
	size_t i, len;
	char *ptr;

	len = _arrlen(__HEAP_BLOCK__);
	ptr = malloc(size + len);
	if (ptr == NULL || errno != 0)
		return NULL;

	ptr = memset(ptr, 0, size + len);
	if (ptr == NULL || errno != 0)
		return NULL;

	for (i = 0; i < len; i++)
		ptr[i] = (__HEAP_BLOCK__)[i];

	return (void *)(ptr + len);
}

void *
xcalloc(size_t nmemb, size_t size)
{
	return xmalloc(nmemb * size);
}

void *
xrealloc(void *block, size_t size)
{
	size_t i, len;
	char *ptr;

	if (!block)
		return xmalloc(size);

	len = _arrlen(__HEAP_BLOCK__);
	ptr = ((char *)block) - len;

	for (i = 0; i < len; i++)
		if (ptr[i] != (__HEAP_BLOCK__)[i])
			return block;

	ptr = realloc(ptr, sizeof(char) * (len + size));
	if (!ptr)
		return block;

	return (void *)(ptr + len);
}

void
xfree(void *block)
{
	size_t i, len;
	char *ptr;

	i = 0;
	len = _arrlen(__HEAP_BLOCK__);

	if (!block)
		return;

	ptr = ((char *)block) - len;

	for (; i < len; i++)
		if (ptr[i] != (__HEAP_BLOCK__)[i])
			return;

	ptr = memset(ptr, 0, sizeof(char) * len);

	free(ptr);
}
