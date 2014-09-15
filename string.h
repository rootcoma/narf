#ifndef STRING_H
#define STRING_H
#include <stddef.h>

void
*memcpy (void *dst, const void *src, size_t size);

void
*memset (void *dst, int val, size_t size);

size_t
strlen (const char *s);

#endif
