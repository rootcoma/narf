#include "string.h"

char
*strchr(char *s, int c)
{
	while(*s) {
		if(*s == c)
			return s;
		s++;
	}
	return 0;
}

char
*strcpy (char *dst, const char *src)
{
	unsigned int i=0;
	while((dst[i++] = src[i++]));
	return dst;
}

void
*memcpy (void *dst, const void *src, size_t size)
{
	unsigned char* b_dst = (unsigned char*)dst;
	unsigned char* b_src = (unsigned char*)src;
	
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
