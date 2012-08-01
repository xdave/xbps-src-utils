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
