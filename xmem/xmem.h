#ifndef __XMEM_H__
#define __XMEM_H__

#include <sys/types.h>

#ifndef __arrlen
#define _arrlen(arr) ((sizeof(arr)) / (sizeof(*arr)))
#endif

#ifndef __HEAP_BLOCK__
#define __HEAP_BLOCK__ "__H34P_BL0CK__"
#endif

void *xmalloc(size_t);
void *xcalloc(size_t, size_t);
void *xrealloc(void *, size_t);
void xfree(void *);

#endif /* __XMEM_H__ */
