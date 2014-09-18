#include <stdint.h>

unsigned char in(unsigned short _port);
void out(unsigned short _port, unsigned char _data);
uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t data);
void outw(uint16_t port, uint16_t data);
void outl(uint16_t port, uint32_t data);
