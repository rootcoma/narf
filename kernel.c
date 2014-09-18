#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "portio.h"
#include "memory.h"
#include "math.h"
#include "video.h"
#include "string.h"
#include "rand.h"
#include "clife.h"

#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "This needs to be compiled with a ix86-elf compiler"
#endif

// #define MODE VGA_320x200x256
#define MODE VGA_640x480x16

void memory_initialize()
{
	unsigned int end;
	__asm__("movl %%esp,%0" : "=r"(end));
	unsigned int page_aligned_end = ((unsigned int)end & 0xFFFFF000) + 0x1000;
	next_address = (unsigned int*)page_aligned_end;
}

void draw_life (void)
{
    int i, j, l, k;
    for (i=0;i<board.h;i++)
    	for (j=0;j<board.w;j++)
    		if (get_bit(j, i, &board))
				//for (l=0;l<2;l++)
					//for(k=0;k<2;k++)
    					putpixel(j*2, i*2, 15-count_neighbors(j, i, &board));

}

void plasma (double tick)
{
    int color;
    double i = 0;
    double v, x, y, cx, cy;
    for(i=0;i<640*480;i++) {
        x = (int)i%640;
        y = (int)i/640;
        cx = 80 + 6*sin(x/(4.0*PI*2)*sin(tick));
        cy = 80 + 6*sin(y/(3.0*PI*2)*sin(tick));
        v = cx+cy;
        color = (int)(sin(sqrt(1000*((int)v)))*5)%5;
        if(color) {
        	g_video_buffer.memory[(int)y*640+(int)x] = color;
        }
    }
}

void loop_circle(double tick)
{
	double i = 0;
	int j;
	while (i < PI*2) {
		putpixel((int)(cos(i)*80.0*sin(tick))+g_video.w/2, (int)(sin(i)*80.0)+g_video.h/2, COLOR_RED);
		putpixel((int)(cos(i+tick*2)*40.0)+g_video.w/2, (int)(sin(i*3)*20.0)+g_video.h/2, COLOR_BROWN);
		i += .01;
	}
}

void shift_up(void)
{
	//get the last line
	uint8_t buf[4096] = {0};
	unsigned int dw = get_aligned_width(&board)/8;
	memcpy(buf, board.data, dw);
	// shift up
	memcpy(board.data, board.data+dw, get_board_size(&board)-dw);
	// copt last line back
	memcpy(board.data+get_board_size(&board)-dw, buf, dw);
}

void update_ball(int *_x, int *_y, int *_dir)
{
	int x = *_x;
	int y = *_y;
	int dir = *_dir;
	if (x>=g_video.w && (dir == 0 || dir == 3))
		dir=(dir+2)%4;
	if (y<=0 && (dir == 0 || dir == 2))
		dir=(dir+3)%4;
	if (y>=g_video.h && (dir == 1 || dir == 3))
		dir=(dir+1)%4;
	if (x<=0 && (dir == 1 || dir == 2))
		dir=(dir+2)%4;

	if (dir==0 || dir==3)
		x++;
	else
		x--;

	if (dir==0 || dir==2)
		y--;
	else
		y++;
	*_dir = dir;
	*_y = y;
	*_x = x;
}

draw_ball(int x, int y)
{
	int i, j;
	for (i=0;i<10;i++)
		for(j=0;j<10;j++)
			putpixel(x+j-5, y+i-5, 2);
}

int cur_x = 0;
int cur_y = 0;
void put_char (unsigned char c, int color)
{
	if (c == '\n') {
		cur_y++;
		cur_x = 0;
	} else {
		draw_char(cur_x*9, cur_y*16, c, color);
    	cur_x += 1;
    }
    if (cur_x*9 > g_video.w) {
    	cur_y++;
    	cur_x = 0;
    }
    if (cur_y*16 > g_video.h) {
    	cur_y = 0;
    	cur_x = 0;
    }
}

void put_str (char *s, int color)
{
	while (*s) {
		put_char(*s++, color);
	}
}

void loop_life(void)
{
	init_life(NULL);
	//create_board(g_video.w, g_video.h, &board);
    //resize_life(g_video.w, g_video.w);
	//randomize_life();
	//step_life();
	//draw_life();
	double tick = 2;
	int shiftc = 0;
	int ballx = 4;
	int bally = 4;
	int balldir = 0;
	char buf[255] = {0};
	plasma(tick);
	for (;;) {
		//tick+=.1;
		//if (tick>PI*2)
		//	tick=-PI*2;
		//if(!(shiftc++ % 12))
			//shift_up();
		//update_ball(&ballx, &bally, &balldir);
		//plasma(tick);
		tick += .01;
		if(!(shiftc++ % 3)) {
			//step_life();
			//clear_screen();
			// //draw_life();
			// draw_ball(ballx, bally);
			// loop_circle(tick);
			// cur_x = 0;
			// cur_y = 0;
			// put_str(">2015\n", COLOR_GREEN);
			// put_str(">not working on demos\n", COLOR_GREEN);
			// facebook_uint32_to_str(ballx, buf);
			// put_str(buf, COLOR_WHITE);
			// put_str(",", COLOR_WHITE);
			// facebook_uint32_to_str(bally, buf);
			// put_str(buf, COLOR_WHITE);
			// put_str("\n", COLOR_WHITE);
		}
		swap_buffer();
	}
}

void kernel_main()
{
	memory_initialize();
	init_vga(MODE);
	init_rand(145);
	loop_life();
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
