#include "memory.h"

void *malloc (size_t size)
{
    unsigned int *target = next_address;
    next_address += (size & 0xFFFFF000) + 0x1000;
    return target;
}

void free (void *ptr)
{
    if (!ptr)
        return;
    return;
}
