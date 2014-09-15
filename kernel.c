#include <stdbool.h> /* C doesn't have booleans by default. */
#include <stddef.h>
#include <stdint.h>

#include "memory.h"
#include "math.h"
#include "video.h"
#include "string.h"

/* Check if the compiler thinks if we are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

unsigned char g_640x480x16[] =
{
/* MISC */
    0xE3,
/* SEQ */
    0x03, 0x01, 0x08, 0x00, 0x06,
/* CRTC */
    0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0x0B, 0x3E,
    0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xEA, 0x0C, 0xDF, 0x28, 0x00, 0xE7, 0x04, 0xE3,
    0xFF,
/* GC */
    0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x05, 0x0F,
    0xFF,
/* AC */
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
    0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
    0x01, 0x00, 0x0F, 0x00, 0x00
};

unsigned char g_320x200x256[] =
{
/* MISC */
    0x63,
/* SEQ */
    0x03, 0x01, 0x0F, 0x00, 0x0E,
/* CRTC */
    0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
    0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x9C, 0x0E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3,
    0xFF,
/* GC */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
    0xFF,
/* AC */
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x41, 0x00, 0x0F, 0x00, 0x00
};

unsigned char in(unsigned short _port)
{
	unsigned char result;
	__asm__ ("in %%dx, %%al" : "=a" (result) : "d" (_port));
	return result;
}

void out(unsigned short _port, unsigned char _data)
{
	__asm__ ("out %%al, %%dx" : : "a" (_data), "d" (_port));
}

void memory_initialize()
{
	unsigned int end;
	asm("movl %%esp,%0" : "=r"(end));
	unsigned int page_aligned_end = ((unsigned int)end & 0xFFFFF000) + 0x1000;
	next_address = (unsigned int*)page_aligned_end;
}

void write_regs(unsigned char *regs)
{
	unsigned i;

/* write MISCELLANEOUS reg */
	out(VGA_MISC_WRITE, *regs);
	regs++;
/* write SEQUENCER regs */
	for(i = 0; i < VGA_NUM_SEQ_REGS; i++) {
		out(VGA_SEQ_INDEX, i);
		out(VGA_SEQ_DATA, *regs);
		regs++;
	}
/* unlock CRTC registers */
	out(VGA_CRTC_INDEX, 0x03);
	out(VGA_CRTC_DATA, in(VGA_CRTC_DATA) | 0x80);
	out(VGA_CRTC_INDEX, 0x11);
	out(VGA_CRTC_DATA, in(VGA_CRTC_DATA) & ~0x80);
/* make sure they remain unlocked */
	regs[0x03] |= 0x80;
	regs[0x11] &= ~0x80;
/* write CRTC regs */
	for (i = 0; i < VGA_NUM_CRTC_REGS; i++) {
		out(VGA_CRTC_INDEX, i);
		out(VGA_CRTC_DATA, *regs);
		regs++;
	}
/* write GRAPHICS CONTROLLER regs */
	for (i = 0; i < VGA_NUM_GC_REGS; i++) {
		out(VGA_GC_INDEX, i);
		out(VGA_GC_DATA, *regs);
		regs++;
	}
/* write ATTRIBUTE CONTROLLER regs */
	for (i = 0; i < VGA_NUM_AC_REGS; i++) {
		in(VGA_INSTAT_READ);
		out(VGA_AC_INDEX, i);
		out(VGA_AC_WRITE, *regs);
		regs++;
	}
/* lock 16-color palette and unblank display */
	in(VGA_INSTAT_READ);
	out(VGA_AC_INDEX, 0x20);
}

void init_vga(void)
{
	g_video.w = g_video_buffer.w = 320;
	g_video.h = g_video_buffer.h = 200;
	g_video.memory = (void*)0xA0000;
 	g_video_buffer.memory = malloc(g_video.w*g_video.h);
	write_regs((unsigned char*)g_320x200x256);
	/* clear screen */
	memset((void*)g_video.memory, 0, g_video_buffer.w*g_video_buffer.h);
}

void loop_circle(void)
{
	double i = 0;
	int dir = 1;
	int color = 1;
	double tick = 0;
	for(;color;i=-PI*2) {
		clear_screen();
		tick+=.01;
		if (tick>PI*2) {
			dir *=-1;
			tick=-PI*2;
		}
		while (i <= PI*2) {
			color = color % 13 + 1;
   			putpixel((int)(cos(tick-i)*60.5)+g_video.w/2, -(int)(sin(-i)*60.5)+g_video.h/2, COLOR_BROWN);
   			putpixel((int)(cos(i*4+tick*2)*60.0)+g_video.w/2, (int)(sin(i*4)*60.0)+g_video.h/2, COLOR_RED);
			putpixel((int)(sin(i+tick)*50.0)+g_video.w/2, i*(g_video.h/(PI*2)), color);
   			putpixel(-(int)(sin(i+tick)*50.0)+g_video.w/2, i*(g_video.h/(PI*2)), color);
			i += .01;
		}
		
		swap_buffer();
	}
}

void kernel_main()
{
	//terminal_initialize();
	memory_initialize();
	//char out[255] = "Switching to VBE mode.\0";
	//terminal_writestring(out);	
	init_vga();
	loop_circle();
}

/*

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

uint8_t make_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}
 
uint16_t make_vgaentry(char c, uint8_t color)
{
	uint16_t c16 = c;
	uint16_t color16 = color;
	return c16 | color16 << 8;
}

 
void terminal_initialize()
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for ( size_t y = 0; y < VGA_HEIGHT; y++ )
	{
		for ( size_t x = 0; x < VGA_WIDTH; x++ )
		{
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = make_vgaentry(' ', terminal_color);
		}
	}
}
 
void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}
 
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = make_vgaentry(c, color);
}

void terminal_setcursor(const size_t x, const size_t y)
{
	const size_t pos = y*VGA_WIDTH + x;
	out(0x3D4, 14);
	out(0x3D5, pos >> 8);
	out(0x3D4, 15);
	out(0x3D5, pos & 255);	
}

void terminal_wrap(size_t num)
{
	memcpy(terminal_buffer, terminal_buffer+(num*VGA_WIDTH), (VGA_WIDTH*VGA_HEIGHT*2)-(num*VGA_WIDTH*2));	
	memset(terminal_buffer+(VGA_HEIGHT-num)*VGA_WIDTH, 0, VGA_HEIGHT*VGA_WIDTH*2-(VGA_HEIGHT-num)*VGA_WIDTH*2);
}

void terminal_putchar(char c)
{
	if (c == '\n') {
		terminal_row++;
		terminal_column = 0;
	} else {
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
		terminal_column++;
	}
	if (terminal_column >= VGA_WIDTH) {
		terminal_column = 0;
		terminal_row++;
	}
	if(terminal_row >= VGA_HEIGHT) {
		terminal_row = VGA_HEIGHT-1;
		terminal_column = 0;
		terminal_wrap(1);
	}
	terminal_setcursor(terminal_column, terminal_row);
}

void terminal_writestring(const char* data)
{
	size_t datalen = strlen(data);
	for (size_t i = 0; i < datalen; i++)
		terminal_putchar(data[i]);
}
*/
