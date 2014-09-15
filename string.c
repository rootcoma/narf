#include "string.h"

void
*memcpy (void *dst, const void *src, size_t size)
{
	unsigned char* b_dst = (unsigned char*)dst;
	unsigned char* b_src = (unsigned char*)src;
	//bytes to align source pointer
	while (size && ((unsigned int)b_src & 0x3) != 0) {
		*b_dst++ = *b_src++;
		size--;
	}
	
	unsigned int* w_dst = (unsigned int*)b_dst;
	unsigned int* w_src = (unsigned int*)b_src;
	while (size >= 4) {
		*w_dst++ = *w_src++;
		size -= 4;
	}

	// Copy trailing bytes
	if (size > 0) {
		b_dst = (unsigned char*)w_dst;
		b_src = (unsigned char*)w_src;
		while (size > 0) {
			*b_dst++ = *b_src++;
			size--;
		}
	}

	return dst;
}

void
*memset (void *dst, int val, size_t size)
{
	unsigned char* b_dst = (unsigned char*)dst;
	while(size--)
		*(char *)b_dst++ = val;
	return dst;
}

size_t
strlen (const char *s)
{
	size_t c = 0;
	while (s[c])
		c++;
	return c;
}

