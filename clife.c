#include "clife.h"
#include "string.h"

#define min(a,b) a < b ? a : b
#define max(a,b) a > b ? a : b

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0
#define NULL 0

#define B "23"
#define S "3"
#define DATA_WIDTH 8

#define MAX_WIDTH 320
#define MAX_HEIGHT 240


unsigned int get_aligned_width (const board_t *b)
{
	return b->w+DATA_WIDTH-(b->w%DATA_WIDTH); /* align by DATA_WIDTH bytes */
}

unsigned int get_board_size (const board_t *b)
{
	return get_aligned_width(b)*b->h/DATA_WIDTH;
}

unsigned int offset (const int x,const int y, const board_t *b)
{
	return (x+y*get_aligned_width(b))/DATA_WIDTH;
}

bool get_bit (const int x, const int y, const board_t *b)
{
	return 1 & (b->data[offset(x,y,b)] >> (x%DATA_WIDTH));
}

void set_bit (const int x, const int y, const bool p, board_t *b)
{
	if (get_bit(x,y,b) != p)
		b->data[offset(x,y,b)] ^= 1 << (x%DATA_WIDTH);
}

void create_board (const int w, const int h, board_t *b)
{
	if (b->data != NULL)
		free(b->data);
	b->w = w;
	b->h = h;
	b->data = malloc(sizeof(uint8_t)*get_board_size(b));
}

void resize_life (const int w, const int h)
{
	const int new_width = min(MAX_WIDTH, w);
	const int new_height = min(MAX_HEIGHT, h);
	if (new_width != last_width || new_height != last_height) {
		last_width = new_width;
		last_height = new_height;	
		create_board(new_width, new_height, &board_buffer);
		int i, j;
		for (i=0;i<new_height && i<board.h;i++)
			for (j=0;j<new_width && j<board.w;j++)
				set_bit(j, i, get_bit(j, i, &board), &board_buffer);
		create_board(new_width, new_height, &board);
		memcpy(board.data, board_buffer.data, sizeof(uint8_t)*get_board_size(&board));
	}	
}

uint8_t count_neighbors (const int x, const int y, const board_t *b)
{
	uint8_t count = 0;
	int i, tar_x, tar_y;
	#define clip_bounds(tar, max, val) for(tar=val;tar<0;tar+=max); tar=tar%max
	for (i=0;i<9;i++) {
		if (i==4)
			continue;
		/* get x, y of target neighbor looped across board */
		clip_bounds(tar_x, b->w, x-1+(i%3));
		clip_bounds(tar_y, b->h, y-1+(i/3));
		if (get_bit(tar_x, tar_y, b))
			count++;
	}
	return count;
}

void randomize_life (void)
{
	int i, j;
	for (i=0;i<board.h;i++)
		for (j=0;j<board.w;j++)
			set_bit(j, i, rand()%2, &board);
}

void step_life (void)
{
	memcpy(board_buffer.data, board.data, sizeof(uint8_t)*get_board_size(&board));
	int i, j;
	uint8_t n;
	bool p;
	for (i=0;i<board.h;i++)
		for (j=0;j<board.w;j++) {
			p = get_bit(j, i, &board_buffer);
			n = count_neighbors(j, i, &board_buffer);
			if (!p && n==3)
				set_bit(j, i, true, &board);
			else if (p && (n!=3 && n!=2))
				set_bit(j, i, false, &board);
			// if(!p && strchr(B, '0'+n))
			// 	set_bit(j, i, true, &board);
			// else if(p && strchr(S, '0'+n))
			// 	set_bit(j, i, false, &board);
		}
}

void init_life (char *rules)
{
	strcpy(life_b, B);
	strcpy(life_s, S);
}

void cleanup_life (void)
{
	if (board.data != NULL) 
		free(board.data);
	if (board_buffer.data != NULL)
		free(board_buffer.data);
}
