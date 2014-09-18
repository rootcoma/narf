#include "portio.h"

unsigned char in(unsigned short _port)
{
    unsigned char result;
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (_port));
    return result;
}

uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
    /* TODO: Is it wrong to use 'N' for the port? It's not a 8-bit constant. */
    /* TODO: Should %1 be %w1? */
    return ret;
}

void outb(uint16_t port, uint8_t val)
{
    __asm__( "outb %0, %1" : : "a"(val), "d"(port) );
}

void outw(uint16_t port, uint16_t val)
{
    __asm__( "outw %0, %1" : : "a"(val), "d"(port) );
}

void outl(uint16_t port, uint32_t val)
{
    __asm__( "outl %0, %1" : : "a"(val), "d"(port) );
}

void out(unsigned short _port, unsigned char _data)
{
    outb(_port, _data);
    //__asm__("out %%al, %%dx" : : "a" (_data), "d" (_port));
}
