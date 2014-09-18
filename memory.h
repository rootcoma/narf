#ifndef MEMORY_H
#define MEMORY_H
#include <stddef.h>

void *malloc (size_t size);
void free (void *ptr);

unsigned int *next_address;

#endif
